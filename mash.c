/*
Ford Nguyen
TCSS 422 - Assignment 1
Mash Shell
Spring 2020
*/

//  There are few extra include here because I was coding this on MacOS VSCode so the IDE
//  doesn't regconize some of the variables (eg. O_CREAT | O_TRUNC | O_RDWR) 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define MAXCHAR 255
#define MAXARG 5

void mash(char c1[], char c2[], char c3[], char file[]);

/*
Main function that take 3 commands and a filename from user
Then send over to mash function.
*/
int main(int argc, char *argv[])
{
  char cmd1[MAXCHAR];
  char cmd2[MAXCHAR];
  char cmd3[MAXCHAR];
  char file[MAXCHAR];
  int i;
  char c;

  printf("mash-1>");
  i = 0;
  while ((c = fgetc(stdin)) != '\n')
  {
    cmd1[i] = c;
    i++;
  }
  cmd1[i] = '\0';

  // Write cmd1 for a file, this is for EC.
  FILE *cmd1txt = fopen("cmd1.txt", "w");
  int result1 = fputs(cmd1, cmd1txt);
  if (result1 == EOF) {
    printf("Write failed\n");
  }
  fclose(cmd1txt);

  printf("mash-2>");
  i = 0;
  while ((c = fgetc(stdin)) != '\n')
  {
    cmd2[i] = c;
    i++;
  }
  cmd2[i] = '\0';

  // Write cmd1 for a file, this is for EC.
  FILE *cmd2txt = fopen("cmd2.txt", "w");
  int result2 = fputs(cmd2, cmd2txt);
  if (result2 == EOF) {
    printf("Write failed\n");
  }
  fclose(cmd2txt);
  
  printf("mash-3>");
  i = 0;
  while ((c = fgetc(stdin)) != '\n')
  {
    cmd3[i] = c;
    i++;
  }
  cmd3[i] = '\0';

  // Write cmd1 for a file, this is for EC.
  FILE *cmd3txt = fopen("cmd3.txt", "w");
  int result3 = fputs(cmd3, cmd3txt);
  if (result3 == EOF) {
    printf("Write failed\n");
  }
  fclose(cmd3txt);
  
  printf("file>");
  i = 0;
  while ((c = fgetc(stdin)) != '\n')
  {
    file[i] = c;
    i++;
  }
  file[i] = '\0';

  //Let's Mash!!
  mash(cmd1, cmd2, cmd3, file);

  return 0;
}

/*
  Mash method that handle input from terminal, split argument separated by space, nested forked.

  EXTRA CREDIT IMPLEMENTED:
  EC-2 DISPLAY OUTPUT SORTED BY PROCESS COMPLETION
  EC-3 DISPLAY 80-CHAR PROCESS DELIMITER LINE
  EC-4 DISPLAY RUNTIME OF EACH COMMAND
  Print out command header lines (EC) (80 characters) and the 80-character delimiter lines.
  (EC) Determine how to measure and print out the execution time of individual commands.
  (EC) Store the output of each command to a temporary file.
  (EC) Delete the temporary output files of each command. (recommended)
*/
void mash(char c1[], char c2[], char c3[], char file[])
{
  int process1, process2, process3;
  struct timeval t0, t1, p1e, p2e, p3e;
  long p1runtime, p2runtime, p3runtime;

  gettimeofday(&t0, 0);

  process1 = fork();
  //fork failed
  if (process1 < 0)
  {
    printf("Fork failed\n");
  }
  //fork sucessful
  if (process1 == 0)
  {
    char *split = strtok(c1, " ");
    char **args = malloc(sizeof(char *) * MAXARG);
    int i = 0;
    while (split != NULL)
    {
      *(args + i) = split;
      split = strtok(NULL, " ");
      i++;
    }
    args[i] = strdup(file);
    args[i + 1] = NULL;

    //Capture execvp output to a file
    int fd = open("cmd1out.txt", O_CREAT | O_TRUNC | O_RDWR, 0644);
    
    if (fd < 0)
    {
      perror("open()");
      exit(EXIT_FAILURE);
    }
    close(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    if ((execvp(args[0], args)) == -1)
    {
      printf("CMD1:[SHELL1] STATUS CODE=-1\n");
    }
  }
  //fork done return to parent.
  if (process1 > 0)
  {
    process2 = fork();
    //fork failed
    if (process2 < 0)
    {
      printf("Fork failed\n");
    }
    //fork sucessful
    if (process2 == 0)
    {
      char *split = strtok(c2, " ");
      char **args2 = malloc(sizeof(char *) * MAXARG);
      int i = 0;
      while (split != NULL)
      {
        *(args2 + i) = split;
        split = strtok(NULL, " ");
        i++;
      }
      args2[i] = strdup(file);
      args2[i + 1] = NULL;

      
      //Capture execvp output to a file
      int fd = open("cmd2out.txt", O_CREAT | O_TRUNC | O_RDWR, 0644);
      if (fd < 0)
      {
        perror("open()");
        exit(EXIT_FAILURE);
      }
      close(STDOUT_FILENO);
      dup2(fd, STDOUT_FILENO);

      if ((execvp(args2[0], args2)) != 0)
      {
        printf("CMD2:[SHELL2] STATUS CODE=-1\n");
      }
    }
    //fork done return to parent
    if (process2 > 0)
    {
      process3 = fork();
      //fork failed
      if (process3 < 0)
      {
        printf("Fork failed\n");
      }
      if (process3 == 0)
      {
        char *split = strtok(c3, " ");
        char **args3 = malloc(sizeof(char *) * MAXARG);
        int i = 0;
        while (split != NULL)
        {
          *(args3 + i) = split;
          split = strtok(NULL, " ");
          i++;
        }
        args3[i] = strdup(file);
        args3[i + 1] = NULL;

        //Capture execvp output to a file
        int fd = open("cmd3out.txt", O_CREAT | O_TRUNC | O_RDWR, 0644);
        if (fd < 0)
        {
          perror("open()");
          exit(EXIT_FAILURE);
        }
        
        close(STDOUT_FILENO);
        dup2(fd, STDOUT_FILENO);
        if ((execvp(args3[0], args3)) == -1)
        {
          printf("CMD3:[SHELL3] STATUS CODE=-1\n");
        }
      }
      //fork done return to parent.
      if (process3 > 0)
      {
        //Determine which process terminated if the processID from wait matched processID from fork.
        int finished3 = wait(NULL);
        if (finished3 == process1) {
          gettimeofday(&p1e, 0);
          printf("First process finished...\n");
        } else if (finished3 == process2) {
          gettimeofday(&p2e, 0);
          printf("Second process finished...\n");
        } else if (finished3 == process3) {
          gettimeofday(&p3e, 0);
          printf("Third process finished...\n");
        }
      }
      //Determine which process terminated if the processID from wait matched processID from fork.
      int finished2 = wait(NULL);
      if (finished2 == process1) {
          gettimeofday(&p1e, 0);
          printf("First process finished...\n");
        } else if (finished2 == process2) {
          gettimeofday(&p2e, 0);
          printf("Second process finished...\n");
        } else if (finished2 == process3){
          gettimeofday(&p3e, 0);
          printf("Third process finished...\n");
        }
    }
    //Determine which process terminated if the processID from wait matched processID from fork.
    int finished1 = wait(NULL);
    if (finished1 == process1) {
          gettimeofday(&p1e, 0);
          printf("First process finished...\n");
        } else if (finished1 == process2) {
          gettimeofday(&p2e, 0);
          printf("Second process finished...\n");
        } else if (finished1 == process3){
          gettimeofday(&p3e, 0);
          printf("Third process finished...\n");
        }
  }

  gettimeofday(&t1, 0);

  // Calculate total and each process' runtime.
  long elapsed = (t1.tv_sec - t0.tv_sec) * 1000000 + t1.tv_usec - t0.tv_usec;
  p1runtime = (p1e.tv_sec - t0.tv_sec) * 1000000 + p1e.tv_usec - t0.tv_usec;
  p2runtime = (p2e.tv_sec - t0.tv_sec) * 1000000 + p2e.tv_usec - t0.tv_usec;
  p3runtime = (p3e.tv_sec - t0.tv_sec) * 1000000 + p3e.tv_usec - t0.tv_usec;
  // Prevent printing multiple line of children processes IDs when cmd input is wrong.
  if (process1 != 0 && process2 != 0 && process3 != 0)
  {
    //Print CMD1, its output and time taken
    printf("-----CMD 1: ");
    FILE *theFile;
    char cmd1[255];
    theFile = fopen("cmd1.txt", "r");
    fscanf(theFile, "%[^\n]", cmd1);
    printf("%s", cmd1);
    for (int i = 0; i <= 67 - strlen(cmd1); i++) {
      printf("-");
    }
    printf("\n");
    remove("cmd1.txt"); //Remove temporary file
    
    char out1[255];
    theFile = fopen("cmd1out.txt", "r");
    fscanf(theFile, "%[^\n]", out1);
    printf("%s", out1);
    remove("cmd1out.txt"); //Remove temporary file
    printf("\n");
    printf("Result took: %ldms\n", p1runtime / 1000);

    //Print CMD2, its output and time taken
    printf("-----CMD 2: ");
    char cmd2[255];
    theFile = fopen("cmd2.txt", "r");
    fscanf(theFile, "%[^\n]", cmd2);
    printf("%s", cmd2);
    for (int i = 0; i <= 67 - strlen(cmd2); i++) {
      printf("-");
    }
    printf("\n");
    remove("cmd2.txt"); //Remove temporary file
    
    char out2[255];
    theFile = fopen("cmd2out.txt", "r");
    fscanf(theFile, "%[^\n]", out2);
    printf("%s", out2);
    remove("cmd2out.txt"); //Remove temporary file
    printf("\n");
    printf("Result took: %ldms\n", p2runtime / 1000);

    //Print CMD3, its output and time taken
    printf("-----CMD 3: ");
    char cmd3[255];
    theFile = fopen("cmd3.txt", "r");
    fscanf(theFile, "%[^\n]", cmd3);
    printf("%s", cmd3);
    for (int i = 0; i <= 67 - strlen(cmd3); i++) {
      printf("-");
    }
    printf("\n");
    remove("cmd3.txt"); //Remove temporary file
    
    char out3[255];
    theFile = fopen("cmd3out.txt", "r");
    fscanf(theFile, "%[^\n]", out3);
    printf("%s", out3);
    remove("cmd3out.txt"); //Remove temporary file
    fclose(theFile); // Close reader
    printf("\n");
    printf("Result took: %ldms\n", p3runtime / 1000);

    //Delimeter line
    for (int i = 0; i < 80; i++)
    {
      printf("-");
    }

    printf("\n");
    printf("Children process IDs: %d %d %d\n", process1, process2, process3);
    printf("Total elapsed time: %ldms\n", elapsed / 1000);
  }
}
