/**
 * CS3600, Spring 2013
 * Project 1 Starter Code
 * (c) 2013 Alan Mislove
 *
 * You should use this (very simple) starter code as a basis for 
 * building your shell.  Please see the project handout for more
 * details.
 */

#include "3600sh.h"

#define USE(x) (x) = (x)

//define max string lengths using same convention as strlen
//e.g. memory allocated = length + 1
#define MAX_DIR_LENGTH 100
#define MAX_CMD_LENGTH 100
#define MAX_HOSTNAME_LENGTH 100
#define MAX_INPUT_LENGTH 100
#define MAX_NUM_ARGS 10
#define EXIT_COMMAND "exit"

typedef char bool;
#define TRUE 1
#define FALSE 0

void printPrompt();
void readCommand();
bool buildInput(char* input);
char** parseArgs(char* input);
void deleteArgs(char** args);
void memoryError();

int main(int argc, char*argv[]) {
  // Code which sets stdout to be unbuffered
  // This is necessary for testing; do not change these lines
  USE(argc);
  USE(argv);
  setvbuf(stdout, NULL, _IONBF, 0); 
  
  // Main loop that reads a command and executes it
  while (1) {         
    // You should issue the prompt here
    printPrompt();
    // You should read in the command and execute it here
    readCommand();
  }

  return 0;
}

void printPrompt() {
  char* username = getenv("USER");
  char* hostname = (char*)calloc(MAX_HOSTNAME_LENGTH + 1, sizeof(char));
  if (!hostname) {
    memoryError();
  }
  gethostname(hostname, MAX_HOSTNAME_LENGTH);
  *(hostname + MAX_HOSTNAME_LENGTH) = 0;
  char* directory = getenv("PWD");
  printf("%s@%s:%s> ", username, hostname, directory);
  free(hostname);
}

void readCommand() {
  char* command = calloc(MAX_INPUT_LENGTH + 1, sizeof(char));
  if (!command) {
    memoryError();
  }
  bool terminate = buildInput(command);
  if (strncmp(command, EXIT_COMMAND, strlen(EXIT_COMMAND)) == 0) {
    do_exit();
  }
  char** args = parseArgs(command);
  
  if(strcmp(args[0],"cd") == 0)
  {
     int error = 0; 
     if(args[1])
     	  error = chdir(args[1]);
     else
	error = chdir("/home");
     if(error)
	printf("cd: %s: No such file or directory\n",args[1]);
	
	return;
  }
  pid_t parent = fork();
  
  if (parent < 0) {
    printf("we fucked up");  
    do_exit();
  }
  else if (!parent && !terminate) {
    if (execvp(args[0], args)) {
      printf("Error: ");
      switch(errno) {
        case 1: printf("Permission denied.\n"); break;
        case 2: printf("Command not found.\n"); break;
        default: printf("Unkown error.\n"); break;
      }
    }
    exit(0);
  } else {
    waitpid(parent, NULL, 0);
   if (terminate) {
      do_exit();
    }
  }
  deleteArgs(args);
  free(args);
  free(command);
}

bool buildInput(char* input) {
  int length = 0;
  char c = getchar();
  while (c != '\n' && c != EOF && length <= MAX_INPUT_LENGTH) {
    *(input + length) = c;
    length++;
    c = getchar();
  }
  *(input + length) = 0;
  if (c == EOF) {
    return TRUE;
  } else {
    return FALSE;
  }
}

char** parseArgs(char* input) {
  char** arguments = (char**)calloc(MAX_NUM_ARGS, sizeof(char*));
  if (!arguments) {
    memoryError();
  }
  int argCount = 0;
  char c = *input;
  char* arg = (char*)calloc(MAX_CMD_LENGTH, sizeof(char));
  if (!arg) {
    memoryError();
  }
  int argLength = 0;
  bool esc_mode;
  bool redirect_output;
  while (c != 0) {
    esc_mode = FALSE;
    redirect_output = FALSE;
    if (c == '\\' && *(input+1)){
      esc_mode = TRUE;
      input++;
      c = *input;
    }
    if (c == '>' && !esc_mode){
	redirect_output = TRUE;
	input++;
	c = *input;
	}
    if ((c == ' ' || c == '\t') && !esc_mode) {
      if (argLength) {
        *(arg + argLength) = 0;
        *(arguments + argCount) = arg;
        argCount++;
        arg = (char*)calloc(MAX_CMD_LENGTH, sizeof(char));
        if (!arg) {
          memoryError();
        }
        argLength = 0;
      }  
    } else {
      *(arg + argLength) = c;
      argLength++;
    }
    c = *(++input);
  }
  *(arg + argLength) = 0;
  *(arguments + argCount) = arg;
  argCount++;
  *(arguments + argCount) = NULL;
  return arguments;
}    
  
void deleteArgs(char** args) {
  char* arg = *(args);
  while (arg != 0) {
    free(arg);
    arg = *(++args);
  }
}

void memoryError() {
  printf("Error: A memory oops has occurred.");
  do_exit();
}

// Function which exits, printing the necessary message
//
void do_exit() {
  printf("So long and thanks for all the fish!\n");
  exit(0);
}
