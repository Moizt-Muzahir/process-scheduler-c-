#include"utilities.h"

using namespace std;
pthread_t tid1;
pthread_mutex_t mtlock;

int new_Time=0;
bool isEmpty=false;
class ComparisonClassNewState {         //Comparison substitute;
public:  
    bool operator() (process Lhs, process Rhs) {
        return (Lhs.i_ArivalTime>=Rhs.i_ArivalTime);
    }
};
void * Increment_Time(void * input){
    while(true){
           
        pthread_mutex_lock(&mtlock);
        if(isEmpty){

            pthread_mutex_unlock(&mtlock);
            printf("New Queue has run out processes\n");
            break;
        }
        pthread_mutex_unlock(&mtlock);
 
        sleep(1);
        new_Time++;
    }
    pthread_exit(NULL);

}


int main(int argc, char *argv[]){
    pthread_mutex_init (&mtlock,NULL);
    pthread_create(&tid1,NULL,Increment_Time,NULL);
  
        int Pipe2Ready=atoi(argv[2]);
        int Pipe2New = atoi(argv[3]);
    priority_queue<process,vector<process>,ComparisonClassNewState>     NewstateQueue; // Priority queius of procs
    string buffer;

        int fd=open("input.txt",00);
        bool EoF=0;

    if( ((readline(fd,EoF)))=="RR")         //Round Robin Check
        readline(fd,EoF);

    do{
        int ID,Burst,Arrival;
        buffer=readline(fd,EoF);            //      Read File
        // printf("%s \n",buffer.c_str());
        ID=buffer[4]-'0';
        buffer=readline(fd,EoF);
        // printf("%s \n",buffer.c_str());

        Arrival=stoi(buffer);
        buffer=readline(fd,EoF);
        // printf("%s \n",buffer.c_str());

        Burst=stoi(buffer);
       process Temp(Arrival,Burst,ID);
       NewstateQueue.push(Temp);
    }
    while(!EoF);

    close(fd);

    int TotalProcNo=NewstateQueue.size();
    write(Pipe2New,&TotalProcNo,sizeof(TotalProcNo));

    
        while(!NewstateQueue.empty()){  
                    // Transfer through pipes work in progress
        process Temp;
        Temp=NewstateQueue.top();
        while(Temp.i_ArivalTime>new_Time){   }
        NewstateQueue.pop();        
        write(Pipe2Ready,&Temp,sizeof(Temp));
        
        printf("Process ID %d Has left New at Time %d Towards Ready \n",Temp.i_processID,new_Time);

              }
        pthread_mutex_lock(&mtlock);
        isEmpty=true;
        pthread_mutex_unlock(&mtlock);


        process Temp(0,0,0);
        write(Pipe2Ready,&Temp,sizeof(Temp));
        pthread_join(tid1,NULL);
        pthread_mutex_destroy(&mtlock);



    exit(0);

}