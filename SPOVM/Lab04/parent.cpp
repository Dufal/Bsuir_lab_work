#include <csignal>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <cstdio>
#include <list>
#include <stdio.h>

using namespace std;

int pFlag=0;
int dFlag=1;

int num;
 unsigned int s1=1,s2;

void handler1(int signum){
if(signum==SIGALRM){
      if(pFlag==1&&dFlag==0){
            pFlag=0;
            dFlag=1;
       }
       if(dFlag==9){
           pFlag=0;
           dFlag=1;
       }else {
            pFlag=1;
            dFlag=0;
       }

    }
    if(signum==SIGUSR1){}
    if(signum==SIGUSR2){}
}


int numCheck(char* tmp){
    char temp[2];
    temp[0]=tmp[1];
    temp[1]=tmp[2];
    return atoi(temp);
}

void flagCheck(list<pid_t> pidlist){
    if (pFlag==1){
        if (dFlag==0) {
            for (auto &temp:pidlist){
                kill(temp,SIGUSR2);
            }
            ualarm(s1,s2);
        }
    }
}

int main(int argc,char** argv){

    list<pid_t> pidlist;
    pid_t PID;
    pid_t TEMP;
    struct sigaction alarmed;

    int counter=0;
    char swt[3];

    signal(SIGALRM,handler1);
    signal(SIGUSR1,handler1);
    signal(SIGUSR2,handler1);

    while(1){
        fflush(stdin);
        system("clear");

flagCheck(pidlist);

        cin>>swt;

        switch(swt[0]){
            case '+':{
                PID=fork();
                if(PID==-1){printf("CHILD BORN ERROR\n");}
                else {if(PID==0){
                        execv("/home/pavel/Projects/Laba4/child",argv);
                }else {pidlist.push_back(PID);}
                cout<<"CHILD "<<counter<<"\n";
                counter++;
                sleep(2);
            }
                break;
            }
            case '-':{
                if (counter==0){cout<<"NO CHILDE\n";}
                else {
                    kill(pidlist.back(),SIGKILL);
                    pidlist.pop_back();
                    cout<<"CHILD №"<<counter<<" DIED\n";
                    counter--;
                }
                break;
            }
            case 's':{
                num=numCheck(swt);

                 if (counter==0){cout<<"NO CHILDE ALIVE\n";}
                else {
                    if (num==0){
                        for (auto &temp:pidlist){
                            kill(temp,SIGUSR1);
                        }

                    }
                        else {
                            int i=0;
                            for (auto &temp:pidlist){
                         i++;
                         if (i==num){kill(temp,SIGUSR1);}
                        }
                        }
                    }
                    break;
                }
            case 'g':{
                
              if (pFlag==1){dFlag=9;}
              else
                  if(pFlag==0){
                      num=numCheck(swt);
                      if (counter==0){cout<<"ALL DIED\n"; break;}
                      if (num!=0){
                          int i=0;
                            for (auto &temp:pidlist){
                         i++;
                         if (i==num){kill(temp,SIGUSR2);}
                        }
                    }
                    else
                    {
                           int i=0;
                            for (auto &temp:pidlist){
                       kill(temp,SIGUSR2);
                        }
                    }
                }
                    break;
                   }
            case 'k':{
                    for (auto &temp:pidlist){
                        kill(temp,SIGKILL);
                    }
                    pidlist.clear();
                    counter=0;
                    cout<<"ALL DIED\n";
                break;
            }
            case 'p':{
                num=numCheck(swt);
                if (num==0){cout<<"NOT ENOUGH PARAMETERS\n"; break;}
                if (counter==0){cout<<"ALL DIED\n"; break;}

                for(auto &temp:pidlist){
                kill(temp,SIGUSR1);
                }
                 int i=0;
                 cout<<"$"<<num<<"$\n";
                            for (auto &temp:pidlist){
                         i++;
                         if (i==num){kill(temp,SIGUSR2);}
                        }
                        alarm(5);
                        dFlag=1;
                break;
            }
            case 'q':{
                  for (auto &temp:pidlist){
                        kill(temp,SIGKILL);
                    }
                    pidlist.clear();
                    counter=0;
                    cout<<"ALL DIED\n";
                    return 1;
            }
        }
    }
}
