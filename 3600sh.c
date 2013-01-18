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

void printPrompt();
void readCommand();

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
  char* command = calloc(MAX_CMD_LENGTH + 1, sizeof(char));
  scanf("%s", command);
  strncat(directory, "/", 1);
  strncat(directory, command, strlen(command));
  char* args [] = {command,NULL};
  int error = execvp(command, args);
  if (error) {
    printf("we fucked up\n");
  }
  free(command);
}

// Function which exits, printing the necessary message
//
void do_exit() {
  printf("So long and thanks for all the fish!\n");
  exit(0);
}
