#include"utilities.h"

using namespace std;
pthread_t tid1;
int cpu_Time=1;

process Ready2Running(void * input) {

    int PipeFromNew = atoi((char * ) input);
    while (true) {
        process Temp;
        if (read(PipeFromNew, & Temp, sizeof(Temp)) > 0) {
            
            if (Temp.i_processID != -1) {
                if (Temp.i_processID!=0)
                printf("Running State Got This from Ready %s \n", Temp.stringProc().c_str());
                return Temp;
            }

        }
    }


}
void Running2Exit(void * input,process Temp){
    int PipeTowardsExit = atoi((char * ) input);
    write(PipeTowardsExit,&Temp,sizeof(Temp));
    // printf("GG = %d\n",gg);
    if(Temp.i_processID!=0)
    printf("Process ID %d has Moved to end state at cpu time %d\n",Temp.i_processID,cpu_Time);
}
void signal2Ready(void * input,process Temp){
    int PipeTowardsReady = atoi((char * ) input);
    write(PipeTowardsReady,&Temp,sizeof(Temp));
    // printf("GG = %d\n",gg);
    if(Temp.i_processID!=0)
    printf("Process ID %d Has left Running at Time %d Towards Ready \n",Temp.i_processID,cpu_Time);
}
int main(int argc, char *argv[]){
    int quantumtime=atoi(argv[1]);
    bool Terminated=false;
    process Current_Proc;
    while(!Terminated){
        if(Current_Proc.i_processID==-1)
            Current_Proc=Ready2Running((void * ) argv[2]);

        if(Current_Proc.i_processID==0){
            break;
        }
    if(!Current_Proc.use_cpu()){
        Running2Exit((void * ) argv[4],Current_Proc);
        //Signal back to ready 
        process Temp(0,0,0);
        signal2Ready((void * ) argv[3],Temp);
        Current_Proc.i_processID=-1;
    }
    else{
        cpu_Time++;
        if((quantumtime>0)&&(cpu_Time%quantumtime==1)){
        signal2Ready((void * ) argv[3],Current_Proc);
        Current_Proc.i_processID=-1;
        }

        sleep(1);
    }
    }
    // process Temp(0,0,0);
    // int PipeTowardsBlock = atoi(argv[4]);
    // write(PipeTowardsBlock,&Temp,sizeof(Temp));
    printf("Running State Terminated\n");
    // pthread_create( & tid1, NULL, New2Ready, (void * ) argv[2]);
    // pthread_join(tid1, NULL);
    exit(0);
}