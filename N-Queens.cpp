#include <iostream>
using namespace std;

const int N = 8;
const int POP = 50;
const int MAX_GEN = 500;
const int MUTATION_RATE = 10;

// Random يدوي
int seed1 = 7;
int myRand()
{
    seed1 = (seed1 * 5 + 3) % 1000;
    return seed1;
}
// يمثل حل واحد
struct State
{
    int queen[N];
    int fitness;
};
// العمليات الجينية
class Genetic
{
private:
    int absValue(int x)
    {
        if(x < 0){
            return -x;
        }
        return x;
    }

public:
    // حساب الـ Fitness
    int calculateFitness(int arr[N])
    {
        int safe = 0;
        for(int i=0;i<N;i++){
            for(int j=i+1;j<N;j++){
                bool sameRow =(arr[i] == arr[j]);
                bool sameDiagonal =(absValue(arr[i]-arr[j])==absValue(i-j));
                if(!sameRow&&!sameDiagonal){
                    safe++;
                }
            }
        }
        return safe;
    }
    // إنشاء State عشوائية
    State randomState(){
        State s;
        for(int i=0;i<N;i++){
            s.queen[i]=myRand()%N;
        }
        s.fitness=calculateFitness(s.queen);
        return s;
    }
    // Roulette Wheel
    State roulette(State pop[POP]){
        int total = 0;
        for(int i=0;i<POP;i++){
            total += pop[i].fitness;
        }
        int r=myRand()%total;
        int current = 0;
        for(int i=0;i<POP;i++){
            current += pop[i].fitness;
            if(current >= r){
                return pop[i];
            }
        }

        return pop[0];
    }
    // Tournament Selection
    State tournament(State pop[POP]){
        int a=myRand()%POP;
        int b=myRand()%POP;

        if(pop[a].fitness>pop[b].fitness){
            return pop[a];
        }
        return pop[b];
    }
    // Crossover
    State crossover(State p1,State p2)
    {
        State child;
        int point=myRand()%N;
        for(int i=0;i<N;i++){
            if(i < point){
                child.queen[i]=p1.queen[i];
            }
            else
            {
                child.queen[i]=p2.queen[i];
            }
        }

        child.fitness=calculateFitness(child.queen);
        return child;
    }
    // Mutation
    void mutation(State &s)
    {
        int chance=myRand()%100;
        if(chance < MUTATION_RATE)
        {
            int index=myRand()%N;
            s.queen[index]=myRand()%N;
        }
        s.fitness=calculateFitness(s.queen);
    }
};
// طباعة الرقعة
class Printer
{
public:
    void printBoard(int arr[N])
    {
        cout<<endl;
        for(int row=0;row<N;row++){
            for(int col=0;col<N;col++){
                if(arr[col]==row){
                    cout<<" Q ";
                }
                else{
                    cout<<" . ";
                }
            }
            cout<<endl;
        }
    }
};

// Solver
class Solver
{
private:
    Genetic g;
    Printer p;
    // أفضل State
    int bestState(State pop[POP])
    {
        int best = 0;
        for(int i=1;i<POP;i++)
        {
            if(pop[i].fitness>pop[best].fitness){
                best = i;
            }
        }
        return best;
    }

public:
    void solve(int start[N],int caseNum,int method){
        State pop[POP];
        // أول State من المستخدم
        for(int i=0;i<N;i++){
            pop[0].queen[i]=start[i];
        }
        pop[0].fitness=g.calculateFitness(pop[0].queen);
        // باقي الـ Population عشوائي
        for(int i=1;i<POP;i++){
            pop[i]=g.randomState();
        }
        State bestEver=pop[bestState(pop)];
        int usedGen = 0;
        // Loop الأجيال
        for(int gen=0;gen<MAX_GEN;gen++){
            int best=bestState(pop);
            if(pop[best].fitness > bestEver.fitness){
                bestEver=pop[best];
            }
            // إذا وجد الحل
            if(bestEver.fitness == 28){
                usedGen =gen + 1;
                break;
            }
            State newPop[POP];
            // Elitism
            newPop[0] =pop[best];
            for(int i=1;i<POP;i++){
                State p1;
                State p2;
                // اختيار الـ Parents
                if(method==1){
                    p1 = g.roulette(pop);
                    p2 = g.roulette(pop);
                }
                else{
                    p1 =g.tournament(pop);
                    p2 =g.tournament(pop);
                }
                // إنشاء طفل
                State child =g.crossover(p1,p2);
                // Mutation
                g.mutation(child);
                newPop[i] =child;
            }
            // تحديث الـ Population
            for(int i=0;i<POP;i++)
            {
                pop[i] =newPop[i];
            }
        }
        // طباعة النتائج
        cout<<"\nCase "<<caseNum;
        if(bestEver.fitness == 28){
            cout<<"\nSolved\n";
        }
        else{
            cout<<"\nNot Solved\n";
        }
        cout<<"Fitness : "<<bestEver.fitness<<" / 28\n";
        cout<<"Generations : "<<usedGen<<endl;
        cout<<"Best State : [";
        for(int i=0;i<N;i++){
            cout<<bestEver.queen[i]+1;
            if(i!=N-1)
            {
                cout<<",";
            }
        }
        cout<<"]\n";
        p.printBoard(bestEver.queen);
    }
};
int main(){
    Solver s;
    int choice;
    int cases[5][N];
    cout<<"1 - Roulette\n";
    cout<<"2 - Tournament\n";
    cout<<"\nEnter Method : ";
    cin>>choice;
    // إدخال 5 حالات
    cout<<"\nEnter 5 Cases:\n";
    for(int i=0;i<5;i++){
        cout<<"\nCase "<<i+1<<": ";
        for(int j=0;j<N;j++){
            cin>>cases[i][j];
            // تحويل من 1-8 إلى 0-7
            cases[i][j]--;
        }
    }
    // تشغيل الحالات
    for(int i=0;i<5;i++){
        s.solve(cases[i],i+1,choice);
    }
}
