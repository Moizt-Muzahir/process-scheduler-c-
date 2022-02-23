#include"utilities.h"

using namespace std;

int main() {
    int fd = open("input.txt", 00);

    //  Check Scheduling Type
    bool EoF = 0;
    string ScheduleType = readline(fd, EoF);
    string QuantumTime;
    if (ScheduleType == "RR")
        QuantumTime = readline(fd, EoF);
    else
        QuantumTime = "0";
    printf("Our Schedule type is %s\n", ScheduleType.c_str());
    close(fd);
    //  Check Scheduling Type
    int New2Ready[2], Run2Ready[2], Ready2Run[2],Block2Ready[2],Run2Block[2],Run2Exit[2],New2Exit[2],Exit2Ready[2]; 
    
    pipe(New2Ready);
    pipe(Run2Ready);
    pipe(Ready2Run);
    pipe(Run2Exit);
    pipe(New2Exit);
    pipe(Exit2Ready);

    fcntl(New2Ready[0], F_SETFL, O_NONBLOCK);
    fcntl(Run2Ready[0], F_SETFL, O_NONBLOCK);
    fcntl(Ready2Run[0], F_SETFL, O_NONBLOCK);
    fcntl(Run2Exit[0], F_SETFL, O_NONBLOCK);
    fcntl(New2Exit[0], F_SETFL, O_NONBLOCK);
    fcntl(Exit2Ready[0], F_SETFL, O_NONBLOCK);
    
    for (int i = 0; i < 5; i++) {
        if (fork() == 0) {
            switch (i) {
            case 0:
                {
                    string s1 = to_string(New2Ready[1]);
                    string s2 = to_string(New2Exit [1]);
                    
                    char
                    const * pN2Re = s1.c_str();
                    char
                    const * pN2E  = s2.c_str();
                    close(New2Ready[0]);
                    close(New2Exit [0]);
                    
                    printf(" New Process State\n");
                    execl("./new", "new", ScheduleType.c_str(), pN2Re, pN2E, NULL);

                }
            case 1:
                {
                    if (ScheduleType == "FCFS") {
                        printf(" Ready Process State\n");

                        close(New2Ready[1]);
                        close(Run2Ready[1]);
                        close(Exit2Ready[1]);
                        close(Ready2Run[0]);
                        

                        string s0 = to_string(Exit2Ready[0]);
                        string s1 = to_string(New2Ready[0]);
                        string s2 = to_string(Run2Ready[0]);
                        string s3 = to_string(Ready2Run[1]);
                        
                        
                        char
                        const * pE2Re = s0.c_str();
                        char
                        const * pN2Re = s1.c_str();
                        char
                        const * pRu2Re = s2.c_str();
                        char
                        const * pRe2Ru = s3.c_str();
                        execlp("./readyQ", "readyQ", pN2Re, pRu2Re, pRe2Ru, pE2Re, NULL); //pRe2Ru,pB2Re,NULL);
                    }
                    break;
                }
            case 2:
                {
                    printf(" Running Process State\n");

                    close (Ready2Run[1]);
                    close (Run2Ready[0]);
                    close (Run2Exit[0]);
                    string s1 = to_string(Ready2Run[0]);
                    string s2 = to_string(Run2Ready[1]);
                    string s3 = to_string(Run2Exit[1]);
                    
                    char
                    const * pRe2Ru =s1.c_str();
                    char
                    const * pRu2Re =s2.c_str();
                    char
                    const * pRu2E  =s3.c_str();
                    
                    char
                    const * QT = QuantumTime.c_str();
                    execlp("./running", "running", QT, pRe2Ru, pRu2Re, pRu2E, NULL);
                    break;
                }
            // case 3:
            //     {
            //         printf(" Blocked Process State\n");
            //         execlp("./blocked", "blocked", NULL);
            //         break;
            //     }
            case 4:
                {
                    close(Run2Exit[1]);
                    close(New2Exit[1]);
                    close(Exit2Ready[0]);

                    string s1 = to_string(Run2Exit[0]);
                    string s2 = to_string(New2Exit[0]);
                    string s3 = to_string(Exit2Ready[1]);

                    char
                    const * pRu2E  =s1.c_str();
                    char
                    const * pN2E   =s2.c_str();
                    char
                    const * pE2Re  =s3.c_str();
                    
                    printf(" Exit Process State\n");
                    execlp("./exit", "exit", pRu2E, pN2E, pE2Re, NULL);
                    break;
                }
            }
        }

    }
    for (int i = 0; i < 4; i++)
        wait(NULL);

}