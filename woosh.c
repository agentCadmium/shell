
//Ana and Miral
//Programming assignment 3
//March 8, 2018

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


char * createListArguments(){
	
}

void reportError() {
  char error_message[30] = "An error has occurred\n";
  write(STDERR_FILENO, error_message, strlen(error_message));
}

int main(int argc, char **argv)
{
  int start = 1; 
  //if start of program, no additional arguments
  if (argc>1 && start){
  	reportError();
  	exit(1);
  }

  char initialPath[] = "/bin";

  char *listArgs = createListArguments();

  printf("woosh> ");

  pid_t pid = fork();

  int cont = 1;
  while (cont){
	//error
	if (pid<0){
	  reportError();
	  exit(1);
	}

	//parent process
	if (pid>0){
	  pid_t waited1;
      int status1;
	  // waiting for a child
      waited1 = wait(&status1);
      printf("I am the parent");

	}

	//child process
	else{
	   printf("I am the child");
	   cont = 0;

	  //execv(initialPath, listArgs);
	}

  }

}
