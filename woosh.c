
//Ana and Miral
//Programming assignment 3
//March 8, 2018

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_ARGS 10
#define LINE_LENGTH 128



void reportError() {
  char error_message[30] = "An error has occurred\n";
  write(STDERR_FILENO, error_message, strlen(error_message));
}

void readLine(char line[]){
	char* ret = fgets(line, LINE_LENGTH, stdin);

	if(strcmp(line, "exit\n")==0 || ret == NULL){
		exit(0);
	}
	int i =0;
	while (line[i]!='\n'){
		i++;
	}
	line[i] = '\0';
}


int parseLine(char* args[], char line[]){
	int i =0;
	args[i] = strtok(line, " ");
	if (args[i] == NULL){
		return 1;
	}

	while(args[i]!=NULL){
		i++;
		args[i] =strtok(NULL, " ");
	}
	return 1;
}

//copied from stack overflow, for now
char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the null-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}


int main(int argc, char **argv)
{
    int start = 1; 
    //if start of program, no additional arguments
    if (argc>1 && start){
  		reportError();
  		exit(1);
 	}
  
  	//store and read the line
  	char* args[MAX_ARGS];
  	char line[LINE_LENGTH];
  

  	char path[] = "/bin/";

  	pid_t pid = fork();
  	pid_t wpid;

  	while (1){
  		printf("woosh> ");
  		readLine(line);
  		parseLine(args, line);

  		char* newPath = concat(path, args[0]);

		//modified wait.c which was shown in class
		pid_t pid;
		pid = fork();
		if (pid < 0) {
	  		fprintf(stderr,"Error: can't fork a process\n");
	  		perror("fork()");
	  		exit(1);
		} 
		else if (pid) { // I am the parent
	  		int status;
	  		// wait for the child
	  		if (waitpid(pid,&status,0) == -1) {
	    	perror("waitpid()");
	  		}
		}
		else {

	  	//fprintf(stdout,"I am the child and my pid is %d\n",getpid());
	  	execv(newPath, args);
		}
		free(newPath);
  	}

}

