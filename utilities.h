#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string>
#include<sys/wait.h>
#include<stdlib.h>
#include<fcntl.h> 
#include<errno.h>
#include<queue>
#include<sys/stat.h>
#include"process.cpp"
#include<pthread.h>
#include<time.h> 



std::string readline(int fd,bool& EoF){
   
    if(fd<0){
        printf("Error in opening file");
    }
    std::string to_return;   
    char buffer=0;
    for(int k=0; ;k++){
        buffer=0;
       if(read(fd,&buffer,1)==0){
            EoF=1;
            break;
       }
        if((buffer=='\n')||(buffer=='\0')){
            
            break;
        }
        
        to_return+=buffer;    
    }
    return to_return;
}