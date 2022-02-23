#include"utilities.h"

using namespace std;
pthread_t tid1;
pthread_t tid2;
pthread_t tid3;

pthread_mutex_t mtlock;
double completedProcs=0;
double totalProcs=0;
double Average[3]={0,0,0};
bool Terminate=false; 
void * AverageStatistics(void * input){
    
    while(!Terminate){
        sleep(30);
    if(Terminate)
        break;
        pthread_mutex_lock( & mtlock);
            int Average_CompletionTime=Average[0]/completedProcs;
            int Average_IOtime=Average[1]/completedProcs;
            int Average_WaitingTime=Average[2]/completedProcs;
        pthread_mutex_unlock( & mtlock);
        
            string Statistics="Average Completion Time = ";
            Statistics+=to_string(Average_CompletionTime)+"\nAverage Turn Around Time = ";
            Statistics+="\nAverage Waiting Time = "+to_string(Average_WaitingTime);
            const char *to_Write=Statistics.c_str();
            printf("%s\n",to_Write);
            int fd=open("cumulative_stats.txt",O_WRONLY);
            write(fd,to_Write,(sizeof(to_Write)));
            close (fd);
        
    }
    pthread_exit(NULL);

    
}
void * New2Exit(void * input){
    int PipeFromNew = atoi((char * ) input);
    int Temp=-1;
    while(Temp==-1){
    
        if (read(PipeFromNew, & Temp, sizeof(Temp)) > 0) {
            pthread_mutex_lock( & mtlock);
                totalProcs=Temp;
            pthread_mutex_unlock( & mtlock);
        }
    }
    
    pthread_exit(NULL);
}
void * Running2Exit(void * input){

    int PipeFromRunning = atoi((char * ) input);
    while(totalProcs==0)
        sleep(1);
    while (completedProcs<totalProcs) {

        process Temp;
        if (read(PipeFromRunning, & Temp, sizeof(Temp)) > 0) {
            if (Temp.i_processID == 0)
                break;
            if (Temp.i_processID != -1) {
                pthread_mutex_lock( & mtlock);
                completedProcs++;
                Temp.complete();
                Average[0]+=Temp.i_CompletionTime;
                Average[1]+=Temp.i_IoTime;
                Average[2]+=Temp.i_WaitingTime;
                 printf("Exit State Received This From Running %s  \n", Temp.stringProc().c_str());
                pthread_mutex_unlock( & mtlock);
                int fd=open("processes_stats.txt",O_WRONLY|O_APPEND);
                Temp.Write2file(fd);
                close(fd);
            }

        }
        
    }
    Terminate=true;
    pthread_exit(NULL);
}


int main(int argc, char *argv[]){
    pthread_mutex_init( & mtlock, NULL);
    pthread_create( & tid1, NULL, AverageStatistics,NULL);
    pthread_create( & tid2, NULL, New2Exit, (void * ) argv[2]);
    pthread_create( & tid3, NULL, Running2Exit, (void * ) argv[1]);

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);

    pthread_mutex_destroy( & mtlock);
    int pipe2Ready=atoi(argv[3]);
    bool Temp=1;
    write(pipe2Ready,&Temp,sizeof(Temp));

    exit(0);
}