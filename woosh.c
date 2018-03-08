
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

int status;
char* path = "/bin/";



void reportError() {
  char error_message[30] = "An error has occurred\n";
  write(STDERR_FILENO, error_message, strlen(error_message));
}

int readLine(char* args[], char line[]){
	char* ret = fgets(line, LINE_LENGTH, stdin);

	if(strcmp(line, "exitn\n")==0 || ret == NULL){
		status = 0;
	}
	int i =0;
	while (line[i]!='\n'){
		i++;
	}
	line[i] = '\0';

	i = 0;
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

void built_in_exit(){
	exit(0);
}

void cd(char* args[]) {
    printf("in cd");
    if(args[1] == NULL) {
          char *home = getenv("HOME");
          int ret = chdir(home);
        if(ret != 0){
            reportError();
        }
    }
    else{
        //printf("%s\n", line[1] );
        int ret = chdir(args[1]);

      if(ret != 0){
            reportError();
        }
    }
}

void pwd() {
    //printf("in pwd");
  char *pathName = getcwd(NULL, 0);
  if(pathName != NULL){
      printf("%s\n", pathName);
  }
  else {
      reportError();
  }
}

// void setPath(char *newpath) {
//   if(newpath == NULL) {
    
//   } else {
//     path = realloc(5*sizeof(char));
//   }
// }

// void printPath() {

//   printf("%s\n", PATH);
// }

int parse(char* args[]) {
	if (strcmp(args[0], "exitn")==0){
		built_in_exit();
		return 0;
	}
	else if (strcmp(args[0], "cd")==0){
		cd(args);
		return 0;
	}
	else if(strcmp(args[0], "pwd")==0){
		pwd(args);
		return 0;
	}
	// else if(strcmp(args[0], "setpath")==0){
	// 	setpath(args[1]);
	// 	return 0;
	// }

}

int main(int argc, char **argv)
{
    //if start of program, no additional arguments
    if (argc>1){
  		reportError();
  		exit(1);
 	}
  	status =1;
  	//store and read the line
  	char* args[MAX_ARGS];
  	char line[LINE_LENGTH];
  	char path[] = "/bin/";

  	while (status){
  		printf("woosh> ");
  		readLine(args, line);

  		char* newPath = concat(path, args[0]);

  		int ret = parse(args);
  		if (ret ==0){
  			//do nothing;
  		}
  		else{
	  		//for executable programs
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
			
				execv(newPath, args);
			}
			free(newPath);
		}
  	}

}

