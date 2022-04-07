#include <csignal>
#include <cstring>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <cstdio>
#include <list>

using namespace std;
int keep_going=1;
struct stat{
    int a=0,b=0,c=0,d=0;
}st;

struct two_words{
    int a=0;
    int b=0;
}track;

bool permis=true;

void acc(){
    if (permis==true){
        cout<<"PID:"<<getpid()<<" PPID:"<<getppid()<<endl;
        cout<<"00="<<st.a<<" 01="<<st.b<<" 10="<<st.c<<" 11="<<st.d<<endl;
    }
}

void handler(int signal){
    if(signal==SIGALRM){
        keep_going=0;
        if((track.a==0)&&(track.b==0)) {st.a++;}
         if((track.a==0)&&(track.b==1)) {st.b++;}
          if((track.a==1)&&(track.b==0)) {st.c++;}
           if((track.a==1)&&(track.b==1)) {st.d++;}
    }

    if (signal==SIGUSR1){permis=false;}
    if (signal==SIGUSR2){permis=true;}
}

int main(int argc,char**argv)
{
    unsigned int t1=10000,t2;
signal(SIGALRM,handler);
signal(SIGUSR1,handler);
signal(SIGUSR2,handler);

cout <<"CHILD"<<endl;
pid_t pid=getpid();

while (1)
{
    for(int i=0;i<500;i++){
        ualarm(t1,t2);
        keep_going=1;
        while(keep_going)
        {
            track.a=track.b=0;
            track.a=track.b=1;
        }

    }
    acc();
}
return 0;
}
