
struct process{
    public:
    int i_ArivalTime,i_BurstTime,i_WaitingTime,i_CompletionTime,i_IoTime,i_remainingIo;
    int i_RemainingBurstTime;
    int i_processID;
    process(const int& arrival,const int& burst,const int& ID){
        i_ArivalTime=arrival;
        i_BurstTime=burst;
        i_RemainingBurstTime=burst;
        i_processID=ID;
        i_WaitingTime=0;
        i_CompletionTime=0;
        i_IoTime=i_remainingIo=0;
    }
    process(){
        i_ArivalTime=-1;
        i_BurstTime=-1;
       i_RemainingBurstTime=-1;
        i_processID=-1;
        i_WaitingTime=-1;
        i_CompletionTime=-1;
        i_IoTime=i_remainingIo=-1;
        
    }

    void wait(){
       i_WaitingTime++;
    }
    bool use_cpu(){
        i_RemainingBurstTime--;
        return i_RemainingBurstTime;
    }
    bool use_io(){
        i_remainingIo--;
        i_IoTime++;
        return i_remainingIo;
    }
    bool Write2file(int fd){
        std::string to_return;
        to_return+="Process ID = ";
        to_return+=std::to_string(i_processID);
        to_return+="\n";

        to_return+="Arrival Time = ";
        to_return+=std::to_string(i_ArivalTime);
        to_return+="\n";

        to_return+="Burst Time = ";
        to_return+=std::to_string(i_BurstTime);
        to_return+="\n"; 
        
        
        

        to_return+="Waiting Time = ";
        to_return+=std::to_string(i_WaitingTime);
        to_return+="\n";

        to_return+="Completion Time = ";
        to_return+=std::to_string(i_CompletionTime);
        to_return+="\n";

        to_return+="I/O Time = ";
        to_return+=std::to_string(i_IoTime);
        to_return+="\n"; 
        
        const char* to_write=to_return.c_str();
        return write(fd,to_write,sizeof(to_write));
    }
    void complete(){
        i_CompletionTime=i_WaitingTime+i_BurstTime+i_IoTime;
    }
    std::string stringProc()const{ 
        std::string to_return;
        to_return+="Process ID = ";
        to_return+=std::to_string(i_processID);
        to_return+="\n";

        to_return+="Arrival Time = ";
        to_return+=std::to_string(i_ArivalTime);
        to_return+="\n";

        to_return+="Burst Time = ";
        to_return+=std::to_string(i_BurstTime);
        to_return+="\n"; 

        to_return+="Remaining Burst Time = ";
        to_return+=std::to_string(i_RemainingBurstTime);
        to_return+="\n";          

        return to_return;
    }
};
