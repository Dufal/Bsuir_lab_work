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

/////////////////////////////

using namespace std;
	const int PR = 1;
	extern char ** environ;

////////////НАЧАЛО///////////

int main(int argc, char** argv)
{
int t=0;
char massiv[50];
char **envir;
int n = 0;
char locate[]="/home/pavel/Projects/Laba3/input.txt";
FILE* f = fopen(locate, "r+");


    if(f == NULL)
    {
    printf ("Cannot open file f\n");
        exit(1);
    }
    while(fgets(massiv, 50, f))
    {  
      n++;

    for (int i = 0; environ[i] != NULL; i++)
	 {
	     if (!strncmp (environ[i], massiv, strlen (massiv)-2))
	    	{	
            
            if(!(envir = (char**)realloc(envir, n*sizeof(char*)*2)))
        {
             printf("Missing memory..."); 
             exit(1);
        }

        if(!(envir[n-1] = (char*)malloc(strlen(environ[i])+1)))
        {
             printf("Missing memory..."); 
             exit(1);
        }
     
        strcpy(envir[n-1],environ[i]);
	  }	
	    }

     }
   

 for(int i = 0; i < n-1; i++)
 {
        for(int j = 0; j<n-i-1; j++)
        {  
            if(strcmp(envir[j],envir[j+1]) > 0)
            {
                  char* tmp = envir[j];
                  envir[j]=envir[j+1];
                  envir[j+1]=tmp;          
            }
        }
} 

//////для вывода переменных среды////////
//for(int i = 0; i < n; i++)
 //{
     //printf("%s\n",envir[i]);
 //}
 ///////////////////////////////////////
 
    char a[2]; 
    int reciver = 0;
    char symbol;
    pid_t pid;
    list<pid_t> pidList;

    sigset_t waitSet;
    sigemptyset(&waitSet);
    sigaddset(&waitSet, SIGUSR2);
    sigprocmask(SIG_BLOCK, &waitSet, NULL);
    cout << "\n\t\t $$$$$WELCOME TO MY PROGRAMM$$$$$\n "<<endl;
    
    cout << "\tThis is a parent process" << endl;
    
    cout << "\t________________________\n" << endl;
    
    cout << "1) To create child process using fork / execv press '+'" << endl;
    
    cout << "2) To create child process using the third parameter press '*'" << endl;
    
    cout << "3) To create child process using array of environment settings press '&'" << endl;
    
    cout << "4) To delete previous child process press '-'" << endl;
    
    cout << "5) Exite 'q'" << endl;
   
    while(true)
    {
a[0]=(char)(((int)'0')+t/10);
a[1]=(char)(((int)'0')+t%10);
char *a1[]={a,"/home/pavel/Projects/Laba3/c","+",NULL};
char *a2[]={a,"/home/pavel/Projects/Laba3/c","*",NULL};
char *a3[]={a,"/home/pavel/Projects/Laba3/c","&",NULL};

        cin.get(symbol);
        switch(symbol)
        {
case '+':
            
                pid = fork();
                switch(pid)
                {
    case -1:
                    cout << "Erorr with creating child process cause of fork" << endl << endl;
                    exit(EXIT_FAILURE);
    case 0:
                 
                     
                    execv("/home/pavel/Projects/Laba3/c",a1);
                   
                    cout << "Problem with loading child process cause of excecv" << endl << endl;
                    
                    exit(127);
    default:
                    t++;
                    pidList.push_back(pid);
                    sleep(PR);
                    break;
                }
                break;


    case '*':
                pid = fork();
                switch(pid)
                {
    case -1:
                    cout << "Problem with creating child process cause of fork" << endl << endl;
                    
                    exit(EXIT_FAILURE);
    case 0:
                  
                   
                   execv("/home/pavel/Projects/Laba3/c",a2);
                  
                    cout << "Problem with loading child process cause of excecv" << endl << endl;
                    exit(127);
    default:
                      t++;
                    pidList.push_back(pid);
                    sleep(PR);
                    break;
                }
                break;
                
    case '&':
                pid = fork();
                switch(pid)
                {
    case -1:
                    cout << "Problem with creating child process cause of fork" << endl << endl;
                    exit(EXIT_FAILURE);
    case 0:
                
                  
                   execv("/home/pavel/Projects/Laba3/c",a3);
                 
                    cout << "Problem with loading child process cause of excecv" << endl << endl;
                    exit(127);
                default:
                  t++;
                    pidList.push_back(pid);
                    sleep(PR);
                    break;
                }
                break;    
                   
            
case '-':
                if(pidList.empty())
                {
                    cout << "There are no chidren to delete!" << endl;
                }
                else
                {
                    kill(pidList.back(), SIGKILL);
                    pidList.pop_back();
                }
                break;

            case 'q':
                if(!pidList.empty())
                {
                    for(auto &childPid : pidList)
                    {
                        kill(childPid, SIGKILL);
                    }
                    pidList.clear();
                }
                return 3;

            default:
                continue;
        }
        cin.ignore();

        for(auto &childPid: pidList)
        {
            kill(childPid, SIGUSR1);
            sigwait(&waitSet, &reciver);
        }
    }
return 0;
}

