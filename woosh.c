
//Ana and Miral
//Programming assignment 3
//March 8, 2018

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#define MAX_ARGS 10
#define LINE_LENGTH 200


char* path[100];

/**
* Free memory allocated for path
**/
void freeMemory(){
	int i=0;
  	while (path[i]!=NULL){
  		free(path[i]);
  		i++;
  	}
}

/**
* Report error for unexpected bahavior
**/
void reportError() {
  char error_message[30] = "An error has occurred\n";
  write(STDERR_FILENO, error_message, strlen(error_message));
}

/**
* Read in the line and store in an arguments array
**/
int readLine(char* args[], char line[]){
	fgets(line, LINE_LENGTH, stdin);

	//report error if line is greater than 128 bytes
	if (strlen(line)>128){
		return 2;
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

/**
* Concatenate strings into a searchaeable path
**/
char* concat(char *s1, char *s2){
    char *result = (char*)malloc((strlen(s1)+strlen(s2)+strlen("/")+1));
    
    strcpy(result, s1);
    strcat(result, "/");
    strcat(result, s2);
    return result;
}

/**
* Exit the program
**/
void built_in_exit(){
	freeMemory();
	exit(0);
}

/**
* cd into a directory
**/
void cd(char* args[]) {
    if(args[1] == NULL) {
          char *home = getenv("HOME");
          int ret = chdir(home);
        if(ret != 0){
            reportError();
        }
    }
    else{
        int ret = chdir(args[1]);

      if(ret != 0){
            reportError();
        }
    }
}

/**
* write full pathname
**/
void pwd() {
    
  char *path = getcwd(NULL, 0);
  if(path != NULL){
      printf("%s\n", path);
  }
  else {
      reportError();
  }
}

/**
* setpath
**/
void setPath(char * args[]) {

	//reset the all the paths to null
  	int j= 0;
	while (path[j]!=NULL){
		free(path[j]);
		path[j]=NULL;
		j++;
	}

  	int i=0;
    while(args[i+1]!=NULL){
     	path[i] = realloc(path[i], strlen(args[i+1])+1);
     	strcpy(path[i], args[i+1]);
		i++;
	}
}

/**
* print the current path
**/
void printPath() {
  int i=0;

  while (path[i]!='\0'){
  	printf("%s ", path[i]);
  	i++;
  }
  printf("\n");
}

/**
* check if a command exists in current path
**/
int fileExists(const char* file) {
	
    struct stat buf;
    if (stat(file, &buf) == 0){
    	return 1;
    }
    else return 0;
}


/**
* loop through all paths to see if command exists
**/
char* searchPath(char* args[], char* path[]){
	int i=0;
	char* res;
	while (path[i]!=NULL){
		res = concat(path[i], args[0]);
		if (fileExists(res)){
			return res;
		}
		i++;
	}
	free(res);
	return NULL;
}

/**
* determine if command is built in
**/
int parse(char* args[]) {
	if (strcmp(args[0], "exit")==0){
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
	else if(strcmp(args[0], "setpath")==0){
		setPath(args);
		return 0;
	}
	else if(strcmp(args[0], "printpath")==0){
		printPath();
		return 0;
	}
	return 1;
}


/**
* Main
**/
int main(int argc, char **argv)
{
    //if start of program, no additional arguments
    if (argc>1){
  		reportError();
  		freeMemory();
  		exit(1);
 	}
  	//store and read the line
  	char* args[MAX_ARGS];
  	char line[LINE_LENGTH];
  	
  	//set initial path as /bin
  	path[0] = malloc((4+1)* sizeof(char));
  	strcpy(path[0], "/bin");
  	
  	//path after setpath is called
  	char* newPath;
  	while (1){
  		printf("woosh> ");
  		int read = readLine(args, line);
  		if(read == 2){
  			reportError();
  			continue;
  		}

  		int ret = parse(args);
  		if (ret != 0){
  			
	  		//for executable programs
			//modified wait.c which was shown in class
			pid_t pid;
			pid = fork();
			if (pid < 0) {
		  		//reportError();
		  		perror("fork()");
		  		freeMemory();
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
  				newPath = searchPath(args, path);
  				if (newPath!=NULL){
  					execv(newPath, args);
  					free(newPath);
  				}
	  			else{
	  				reportError();
	  				continue;
	  			}
			}
			
		}
  	}
  	freeMemory();

}

