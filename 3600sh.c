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

void printPrompt();
void readCommand();
void buildInput(char* input);
char** parseArgs(char* input);
void deleteArgs(char** args);

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
	//printf("EXIT\n");
    // You should probably remove this; right now, it
    // just exits
  }

  return 0;
}

void printPrompt() {
  char* username = getlogin();
  char* hostname = calloc(MAX_HOSTNAME_LENGTH + 1, sizeof(char));
  gethostname(hostname, MAX_HOSTNAME_LENGTH);
  *(hostname + MAX_HOSTNAME_LENGTH) = 0;
  char* directory = calloc(MAX_DIR_LENGTH + 1, sizeof(char));
  getcwd(directory, MAX_DIR_LENGTH );
  *(directory + MAX_DIR_LENGTH) = 0;
  printf("%s@%s:%s> ", username, hostname, directory);
  free(hostname);
  free(directory);
}

void readCommand() {
  char* directory = calloc(MAX_DIR_LENGTH + 1, sizeof(char));
  getcwd(directory, MAX_DIR_LENGTH );
  *(directory + MAX_DIR_LENGTH) = 0;
  char* command = calloc(MAX_INPUT_LENGTH + 1, sizeof(char));
  buildInput(command);
  //scanf("%s", command);
  if (strncmp(command, EXIT_COMMAND, strlen(EXIT_COMMAND)) == 0) {
    do_exit();
  }
  strncat(directory, "/", 1);
  strncat(directory, command, strlen(command));
  char** args = parseArgs(command);
  
  pid_t parent = fork();
  
  if (parent < 0) {
    printf("we fucked up");  
    do_exit();
  }
  else if (!parent) {
    execvp(args[0], args);
    return;
  } else {
    waitpid(parent, NULL, 0);
  }
  deleteArgs(args);
  free(args);
  free(command);
}

void buildInput(char* input) {
  int length = 0;
  char c;
  scanf("%c", &c);
  while (c != '\n' && length <= MAX_INPUT_LENGTH) {
    *(input + length) = c;
    length++;
    scanf("%c", &c);  
  }
  *(input + length) = 0;
}

char** parseArgs(char* input) {
  char** arguments = (char**)calloc(MAX_NUM_ARGS, sizeof(char*));
  int argCount = 0;
  char c;
  char* arg = (char*)calloc(MAX_CMD_LENGTH, sizeof(char));
  int argLength = 0;
  c = *input;
  while (c != 0) {
    if (c == ' ') {
      *(arg + argLength) = 0;
      *(arguments + argCount) = arg;
      argCount++;
      arg = (char*)calloc(MAX_CMD_LENGTH, sizeof(char));
      argLength = 0;
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

// Function which exits, printing the necessary message
//
void do_exit() {
  printf("So long and thanks for all the fish!\n");
  exit(0);
}
