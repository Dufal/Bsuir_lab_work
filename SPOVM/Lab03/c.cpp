#include <iostream>
#include <cstdio>
#include <csignal>
#include <cstring>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sstream>

///////////////////

using namespace std;

namespace PATH
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

extern char ** environ;

////////////////////

void plus( )
{
	char locate[]="/home/pavel/Projects/Laba3/input.txt";
	char massiv[50],massiv_1[50],*b;
 	FILE* f= fopen(locate, "r+");

    if(f == NULL)
    {
    printf ("Cannot read file f");
        exit(1);
    }
	while(fgets(massiv, 50, f))
    {
        strncpy (massiv_1, massiv, strlen (massiv)-1);
        b=getenv(massiv_1);
       
		printf("%s=%s\n",massiv_1,b);
	for(int i=0;i<50;i++)
	{
		massiv_1[i]=0;
		massiv[i]=0;
	}
	}
}

//////////////////////////

void zvezda(char **envp)
{
	char locate[]="/home/pavel/Projects/Laba3/input.txt";
	char massiv[50],massiv_1[50],*b;
	
 	FILE* f= fopen(locate, "r+");
    if(f == NULL)
    {
        exit(1);
    }

printf("\n");
	while(fgets(massiv, 50, f))
    {
         int k = 0;
    for (int i = 0; envp[i] != NULL; i++)
	 {
	     if (!strncmp (envp[i], massiv, strlen (massiv)-1))
	    	{	
            printf ("%s found :%d\n", envp[i],i);
		    }	
	    }

     }
}

///////////////////////

void ampersand()
{
	char locate[]="/home/pavel/Projects/Laba3/input.txt";
	char massiv[50],massiv_1[50],*b;
	char **env;
 	FILE* f= fopen(locate, "r+");
    if(f== NULL)
    {
        exit(1);
    }
printf("\n");
	while(fgets(massiv, 50, f))
    {
         int k = 0;
    for (int i = 0; environ[i] != NULL; i++)
	 {
	     if (!strncmp (environ[i], massiv, strlen (massiv)-1))
	    	{	
            printf ("%s found :%d\n", environ[i],i);
		    }	
	    }

     }


}

/////////////////////

int main(int argc, char** argv,char **envp)
{
	char locate[]="/home/pavel/Projects/Laba3/input.txt";
	char massiv[50],massiv_1[50];
	char *b;
 	FILE* f= fopen(locate, "r+");
    if(f == NULL)
    {
        exit(1);
    }

if (!strcmp(argv[2],"+"))
{
printf ("\n\tChild process using fork / execv \n");
printf("\t________________________________\n\n");

	while(fgets(massiv, 50, f))
    {
        strncpy (massiv_1, massiv, strlen (massiv)-1);
        b=getenv(massiv_1);
		printf("%s=%s\n",massiv_1,b);
	for(int i=0;i<50;i++)
	{
		massiv_1[i]=0;
		massiv[i]=0;
	}
}
}
	else if (!strcmp(argv[2],"*"))
{
printf ("\n\tChild process using array of environment settings\n");
printf("\t_________________________________________________\n");
	zvezda(envp);
}
	else if (!strcmp(argv[2],"&"))
{
printf ("\n\tChild process using the third parameter\n");
printf("\t_______________________________________\n");
	ampersand();
}

	int reciver = 0;
	string message = "CHILDE_";
	message.push_back(argv[0][0]);
	message.push_back(argv[0][1]);
	string message_1 = "PID: ";
	string message_2 = "PPID: ";
	string pidStr = PATH::to_string((int)getpid());
	string ppidStr = PATH::to_string((int)getppid());
	message_1 += pidStr;
	message_2+=ppidStr;
	
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGKILL);
	sigprocmask(SIG_BLOCK, &set, NULL);

	while (true)
	{
		sigwait(&set, &reciver);
		cout << message.c_str() << " ";
		cout << message_1.c_str() << " ";
		cout << message_2.c_str() << " "<<endl;

		kill(getppid(), SIGUSR2);
	}
	return 0;
}


