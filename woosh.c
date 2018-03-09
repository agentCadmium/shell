
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
#define LINE_LENGTH 128

//int status;
char* path[100];



void reportError() {
  char error_message[30] = "An error has occurred\n";
  write(STDERR_FILENO, error_message, strlen(error_message));
}

int readLine(char* args[], char line[]){
	char* ret = fgets(line, LINE_LENGTH, stdin);

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

char* concat(const char *s1, const char *s2){
    char *result = (char*)malloc((strlen(s1)+strlen(s2)+strlen("/")+1));//+1 for the null-terminator
    //and for the extra slash between the path and the command 
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, "/");
    strcat(result, s2);
    return result;
}

void built_in_exit(){
	exit(0);
}

void cd(char* args[]) {
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

void setPath(char * args[]) {


  // if(args[1] == NULL) {
    
  // } else {
  	int j= 0;
	while (path[j]!=NULL){
		path[j] = realloc(path[j], 2*sizeof(char*));
		strcpy(path[j], "");
		j++;
	}

  	int i=0;
     while(args[i+1]!=NULL){
     	path[i] = realloc(path[i], (strlen(args[i+1])+1)*sizeof(char*));
     	strcpy(path[i], args[i+1]);
		//printf("Whats in path[i] in setPath %s\n",path[i]);
		
		i++;
	}
  // }
}

void printPath() {
  int i=0;
  //printf("Whats in path[0]%s\n",path[0]);
  while (path[i]!='\0'){
  	printf("%s ", path[i]);
  	i++;
  }
  printf("\n");
}

int fileExists(const char* file) {
	//printf("in file exists");
    struct stat buf;
    if (stat(file, &buf) == 0){
    	return 1;
    }
    else return 0;
}

char* searchPath(char* args[], char* path[]){
	int i=0;
	while (path[i]!=NULL){
		//printf("Whats in path[0] from searchpath%s\n",path[0]);
		char* res = concat(path[i], args[0]);
		//printf("Whats is res from searchpath%s\n",res);
		if (fileExists(res)){
			return res;
		}
		i++;
	}
	return NULL;
}


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


int main(int argc, char **argv)
{
    //if start of program, no additional arguments
    if (argc>1){
  		reportError();
  		exit(1);
 	}
  	//status =1;
  	//store and read the line
  	char* args[MAX_ARGS];
  	char line[LINE_LENGTH];
  	
  	path[0] = (char*)malloc(4 * sizeof(char));
  	strcpy(path[0], "/bin");
  	
  	char* newPath;
  	while (1){
  		printf("woosh> ");
  		readLine(args, line);
  		//char* newPath = concat(path[0], args[0]);

  		int ret = parse(args);
  		if (ret == 0){
  			//do nothing;
  		}
  		else{
  			//printf("Went into here" );
	  		//for executable programs
			//modified wait.c which was shown in class
			pid_t pid;
			pid = fork();
			if (pid < 0) {
		  		//reportError();
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
				if (searchPath(args, path)!=NULL){

  					newPath = searchPath(args, path);
  					execv(newPath, args);
  					free(newPath);
  				}
	  			else{
	  				reportError();
	  			}
			}
			
		}

		
  	}
  	int i=0;
  	//printf("Whats in path[0]%s\n",path[0]);
  	while (path[i]!=NULL){
  		free(path[i]);
  		i++;
  	}

}

