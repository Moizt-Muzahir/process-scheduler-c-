#include"utilities.h"

using namespace std;

pthread_t tid1;
pthread_t tid2;
pthread_t tid3;
pthread_t tid4;
pthread_t tid5;

pthread_mutex_t QueueLock;
int ready_Time = 0;
bool RunningEmpty = true;
bool Terminated = false;
queue < process > ReadyQueue;
void * Increment_Time(void * input){
    while(!Terminated){
            break;

        sleep(1);
        pthread_mutex_lock( & QueueLock);
         
            queue<process> TempQ;
            while(!ReadyQueue.empty()){
                process Temp;
                Temp=ReadyQueue.front();
                ReadyQueue.pop();
                TempQ.push(Temp);
            }
            while(!TempQ.empty()){
                printf("Something\n");
                process Temp;
                Temp=TempQ.front();
                TempQ.pop();
                Temp.wait();
                ReadyQueue.push(Temp);
            }
        ready_Time++;
        pthread_mutex_unlock( & QueueLock);
         
        
    }
    pthread_exit(NULL);
}
void *Termination_Signal(void * input){
    int PipeFromExit = atoi((char * ) input);
    while (!Terminated) {
        int Temp=0;
        if (read(PipeFromExit, & Temp, sizeof(Temp)) > 0) {
            if (Temp) {
                pthread_mutex_lock( & QueueLock);
                Terminated=true;
                printf("Ready State Terminated\n");
                pthread_mutex_unlock( & QueueLock);
            }

        }
    }
    pthread_exit(NULL);
}
void * New2Ready(void * input) {

    int PipeFromNew = atoi((char * ) input);
    while (true) {
        process Temp;
        if (read(PipeFromNew, & Temp, sizeof(Temp)) > 0) {
            if (Temp.i_processID == 0)
                break;
            if (Temp.i_processID != -1) {
                pthread_mutex_lock( & QueueLock);
                ReadyQueue.push(Temp);
                printf("Ready State Received From New %s  \n", Temp.stringProc().c_str());
                pthread_mutex_unlock( & QueueLock);
            }

        }
    }
    pthread_exit(NULL);

}
void * Running2Ready(void* input){
    
    int PipeFromRunning = atoi((char * ) input);
        while (!Terminated) {
            process Temp;
            if (read(PipeFromRunning, & Temp, sizeof(Temp)) > 0) {
                if (Temp.i_processID == 0){
                    pthread_mutex_lock( & QueueLock);
                    RunningEmpty=true;
                    pthread_mutex_unlock( & QueueLock);


                }
                else
                    if (Temp.i_processID != -1) {
                    pthread_mutex_lock( & QueueLock);
                        RunningEmpty=true;
                        ReadyQueue.push(Temp);
                    pthread_mutex_unlock( & QueueLock);
                    printf("Ready State Received from Running %s \n", Temp.stringProc().c_str());
                }



            }
        }

        pthread_exit(NULL);
    
}
void * Ready2Running(void* input){
    int PipeTowardsRunning = atoi((char * ) input);
    while(!Terminated){
       
    if(!ReadyQueue.empty()){
        process Temp;
        pthread_mutex_lock( & QueueLock);

        Temp=ReadyQueue.front();
                if(RunningEmpty){
                    RunningEmpty=false;

                    write(PipeTowardsRunning,&Temp,sizeof(Temp));

                    printf("Process ID %d Has left Ready at Time %d Towards Running \n",Temp.i_processID,ready_Time);
                    
                        ReadyQueue.pop();
                    

                }
        pthread_mutex_unlock( & QueueLock);

        }
        else {

        }
        
    }  

        process Temp(0,0,0);
        write(PipeTowardsRunning,&Temp,sizeof(Temp));
    pthread_exit(NULL);


}
int main(int argc, char * argv[]) {

    pthread_mutex_init( & QueueLock, NULL);

    pthread_create( &tid1,NULL,Increment_Time,NULL);
    pthread_create( & tid2, NULL, New2Ready, (void * ) argv[1]);

    
    pthread_create( & tid3, NULL, Ready2Running, (void * ) argv[3]);
    pthread_create( & tid4, NULL, Running2Ready, (void * ) argv[2]);
    pthread_create( & tid5, NULL, Termination_Signal, (void * ) argv[4]);
    

    pthread_join(tid1,NULL);
    pthread_join(tid2, NULL);//   Fix at End State
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);
    pthread_join(tid5, NULL);



    pthread_mutex_destroy( & QueueLock);

    exit(0);
}