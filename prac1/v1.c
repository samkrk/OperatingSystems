/*********************************************************************
   Program  : miniShell                   Version    : 1.3
 --------------------------------------------------------------------
   skeleton code for linix/unix/minix command line interpreter
 --------------------------------------------------------------------
   File			: minishell.c
   Compiler/System	: gcc/linux

********************************************************************/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define NV 20			/* max number of command tokens */
#define NL 100			/* input buffer size */
char            line[NL];	/* command input buffer */


/*
	shell prompt
 */

void prompt(void)
{
  // fprintf(stdout, "\n msh> ");
  fflush(stdout);

}


int main(int argk, char *argv[], char *envp[])
/* argk - number of arguments */
/* argv - argument vector from command line */
/* envp - environment pointer */

{
  int             frkRtnVal;	/* value returned by fork sys call */
  int             wpid;		/* value returned by wait */
  char           *v[NV];	/* array of pointers to command line tokens */
  char           *sep = " \t\n";/* command line token separators    */
  int             i;		/* parse index */
  int             bckgrnd_tasks = 0;


  /* prompt for and process one command line at a time  */

  while (1) {			/* do Forever */
    prompt();
    fgets(line, NL, stdin);
    fflush(stdin);

    if (feof(stdin)) {		/* non-zero on EOF  */

      // fprintf(stderr, "EOF pid %d feof %d ferror %d\n", getpid(),
	    //  feof(stdin), ferror(stdin));
      exit(0);
    }
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\000')
      continue;			/* to prompt */

    v[0] = strtok(line, sep);
    for (i = 1; i < NV; i++) {
      v[i] = strtok(NULL, sep);
      if (v[i] == NULL)
	break;
    }
    /* assert i is number of tokens + 1 */

    // Check if the last token is "&" (background command)
    int background = 0;
    if (i > 1 && strcmp(v[i-1], "&") == 0) {
      background = 1;    // Mark as background command
      v[i-1] = NULL;     // Remove "&" from the arguments
    } 

    if (strcmp(v[0], "cd") == 0) {
      if (i > 1) {
        if (chdir(v[1]) != 0) {
            perror("cd failed");
        }
      } else {
        fprintf(stderr, "cd: missing argument\n");
    }
    continue;
    }


    /* fork a child process to exec the command in v[0] */
    switch (frkRtnVal = fork()) {
    case -1:			/* fork returns error to parent process */
      {
        perror("fork failed");
        exit(1);
	      break;
      }
    case 0:			/* code executed only by child process */
      {
          execvp(v[0], v);
          perror("exec failed"); // Exec failed
          exit(1); // Exit if exec fails
        
      }
    default:			/* code executed only by parent process */
      if (!background) {
        wpid = wait(NULL);
        if (wpid == -1) {
          perror("wait failed");
        }
      } else {
        bckgrnd_tasks++;
        printf("[%d] %d\n", bckgrnd_tasks, frkRtnVal);
      }
    }				/* switch */
  }				/* while */
  return 0;
}				/* main */
