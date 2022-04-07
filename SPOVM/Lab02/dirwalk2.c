#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define _SVID_SOURCE
void search(char *);
void searchDir(char *);
void searchFile(char *);
void searchLink(char *);
void sortfile (char*path);
char* outPath = {"/home/pavel/Projects/Laba1/outfile.txt"};
void sort_outFile(char* way);
void file_output(char* way);

int main(int argc, char *argv[]){
remove (outPath);
int i=0;
int k=2;
    
    if(argc>2){
    while (argc!=k)
{
if (argv[k][1]=='d') {printf("directories:\n"); searchDir(argv[1]);}
if (argv[k][1]=='f') {printf("files:\n"); searchFile(argv[1]);}
if (argv[k][1]=='l') {printf("links:\n");searchLink(argv[1]);}
if (argv[k][1]=='w') {printf("directories:\n"); searchDir(argv[1]); sort_outFile(outPath);}
if (argv[k][1]=='e') {printf("files:\n"); searchFile(argv[1]); sort_outFile(outPath);}
if (argv[k][1]=='r') {printf("links:\n");searchLink(argv[1]); sort_outFile(outPath);}



k++;
}
}
else if(argc<3)
{
search(argv[1]);
}
   
    file_output(outPath);
    return 0;
}

void search(char * pt){
    char path[5000];
    FILE* outfile  = fopen(outPath, "a");
   
    struct dirent *fi;
  
    if(strcmp(pt, "/")) strcat(pt, "/");
    DIR * di = opendir(pt);
    struct stat stt;
    if(!di){
        printf("Error %s\n", pt);
        return;
    }
    
    while ((fi = readdir(di))) {
    
         if(!strcmp(fi->d_name, ".")||!strcmp(fi->d_name, ".."))continue;
          
         strcpy(path, pt); strcat(path, fi->d_name);
         lstat(path, &stt);
        
         if(S_ISDIR(stt.st_mode))
             search(path);
      
          fprintf(outfile,"ALL: %s\n", path);
          
         
    }
    closedir(di);
    fclose(outfile);
}

void file_output(char* way)
{
    char n;
    FILE* f = fopen(way, "r");
    if(!f)
    {
        printf("\nFile missing... PATH: %s", way);
        exit(1);
    }
    while(!feof(f))
    {
        fscanf(f,"%c",&n);
        printf("%c",n);
    }
    fclose(f);
}



void searchFile(char * pt){
    char path[5000];
    FILE* outfile  = fopen(outPath, "a");
   // if(!outfile)
    //{
      //  printf("\nOut file missing... PATH: %s", outPath);
        //exit(1);
    //}
    struct dirent *fi;
  
    if(strcmp(pt, "/")) strcat(pt, "/");
    DIR * di = opendir(pt);
    struct stat stt;
    if(!di){
        printf("Error %s\n", pt);
        return;
    }
    
    while ((fi = readdir(di))) {
    
         if(!strcmp(fi->d_name, ".")||!strcmp(fi->d_name, ".."))continue;
          
         strcpy(path, pt); strcat(path, fi->d_name);
         lstat(path, &stt);
        
         if(S_ISDIR(stt.st_mode))
             searchFile(path);
             
      if(S_ISREG(stt.st_mode))
          fprintf(outfile,"file: %s\n", path);
          
         
    }
    closedir(di);
    fclose(outfile);
}
void searchLink(char * pt){
    char path[5000];
    
    
    FILE* outfile  = fopen(outPath, "a");
   // if(!outfile)
    //{
      //  printf("\nOut file missing... PATH: %s", outPath);
        //exit(1);
    //}
    struct dirent *fi;
  
    if(strcmp(pt, "/")) strcat(pt, "/");
    DIR * di = opendir(pt);
    struct stat stt;
    if(!di){
        printf("Error %s\n", pt);
        return;
    }
    
    while ((fi = readdir(di))) {
    
         if(!strcmp(fi->d_name, ".")||!strcmp(fi->d_name, ".."))continue;
          
         strcpy(path, pt); strcat(path, fi->d_name);
         lstat(path, &stt);
        
         if(S_ISDIR(stt.st_mode))
             searchLink(path);
             
      if(S_ISLNK(stt.st_mode))
          fprintf(outfile,"link: %s\n", path);
          
         
    }
    closedir(di);
    fclose(outfile);
}
void searchDir(char * pt){
    char path[5000];
    FILE* outfile  = fopen(outPath, "a");
   // if(!outfile)
    //{
      //  printf("\nOut file missing... PATH: %s", outPath);
        //exit(1);
    //}
    struct dirent *fi;
  
    if(strcmp(pt, "/")) strcat(pt, "/");
    DIR * di = opendir(pt);
    struct stat stt;
    if(!di){
        printf("Error %s\n", pt);
        return;
    }
    
    while ((fi = readdir(di))) {
    
         if(!strcmp(fi->d_name, ".")||!strcmp(fi->d_name, ".."))continue;
          
         strcpy(path, pt); strcat(path, fi->d_name);
         lstat(path, &stt);
        
         if(S_ISDIR(stt.st_mode))
             searchDir(path);
             
      if(S_ISDIR(stt.st_mode))
          fprintf(outfile,"directory: %s\n", path);
          
         
    }
    closedir(di);
    fclose(outfile);
}

void sort_outFile(char* way)
{
    FILE* outfile = outfile = fopen(way, "r");
    if(outfile == NULL)
    {
        printf("\nFile missing... PATH: %s", way);
        exit(1);
    }
    char buff[4096+1];
    char** mass = NULL;
    int k = 0;
    
    while(fgets(buff, 4096, outfile))
    {
        k++;
        if(!(mass = (char**)realloc(mass, k*sizeof(char*)*2)))
        {
             printf("Missing memory..."); 
             exit(1);
        }

        if(!(mass[k-1] = (char*)malloc(strlen(buff)+1)))
        {
             printf("Missing memory..."); 
             exit(1);
        }
        strcpy(mass[k-1], buff);
    }
    
    fclose(outfile);
    remove(way);
    
    if(!(outfile = fopen(way, "a")))
    {
        printf("\nOut file missing... PATH: %s", way);
        exit(1);
    }

    for(int i = 0; i < k-1; i++)
        for(int j = 0; j<k-i-1; j++)
            if(strcmp(mass[j],mass[j+1]) > 0)
            {
                  char* tmp = mass[j];
                  mass[j]=mass[j+1];
                  mass[j+1]=tmp;          
            }

    for(int i = 0; i < k; i++)
        fprintf(outfile,"%s", mass[i]);
        
    fclose(outfile);
}

