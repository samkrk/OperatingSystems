#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

#define NV 20      /* max number of command tokens */
#define NL 100     /* input buffer size */
char line[NL];     /* command input buffer */
int backgroundCounter = 0; /* counter for number of background processes */

// Define a struct for background processes
typedef struct {
    pid_t pid;
    char command[NL];
    int backgroundNum;
} BackgroundProcess;

// Array to keep track of background processes
BackgroundProcess backgroundProcesses[NV] = {0};

// Function to add a new background process to the array
void newBackgroundProcess(pid_t pid, char *command) {
    // Increment counter
    backgroundCounter++;
    // Find empty slot in background array
    for (int i = 0; i < NV; i++) {
        if (backgroundProcesses[i].pid == 0) {
            backgroundProcesses[i].pid = pid;
            // Get command
            strncpy(backgroundProcesses[i].command, command, NL - 1);
            // Terminate null character
            backgroundProcesses[i].command[NL - 1] = '\0';
            // Print that the background process has started
            printf("[%d] %d\n", backgroundCounter, backgroundProcesses[i].pid);
            // Set background position
            backgroundProcesses[i].backgroundNum = backgroundCounter;
            break;
        }
    }
}

// Function to handle background processes
void handleBackground(int signal) {
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        for (int i = 0; i < NV; i++) {
            if (backgroundProcesses[i].pid == pid) {
                // Print that the background process has completed
                printf("[%d]  + done       %s", backgroundProcesses[i].backgroundNum, backgroundProcesses[i].command);
                // Clear the process from the array
                backgroundProcesses[i].pid = 0;
                backgroundCounter--;
                break;
            }=\]
            '
            
        }
    }
}

void prompt(void) {
    fflush(stdout);
}

int main(int argc, char *argv[], char *envp[]) {
    int frkRtnVal;   /* value returned by fork sys call */
    char *v[NV];     /* array of pointers to command line tokens */
    char *sep = " \t\n"; /* command line token separators */
    int i;           /* parse index */
    int background;  /* variable to determine if process is background or not */

    /* Set up the SIGCHLD handler */
    signal(SIGCHLD, handleBackground);

    /* Prompt for and process one command line at a time */
    while (1) {   /* do forever */
        prompt();
        fgets(line, NL, stdin);
        fflush(stdin);

        if (feof(stdin)) {   /* non-zero on EOF */
            exit(0);
        }
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\000') {
            continue;   /* to prompt */
        }

        v[0] = strtok(line, sep);
        for (i = 1; i < NV; i++) {
            v[i] = strtok(NULL, sep);
            if (v[i] == NULL) break;
        }

        /* Check if the last token is "&" (background command) */
        background = 0;
        if (i > 1 && strcmp(v[i - 1], "&") == 0) {
            background = 1;    /* Mark as background command */
            v[i - 1] = NULL;   /* Remove "&" from the arguments */
        }

        /* Handle the "cd" command */
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

        /* Fork a child process to exec the command in v[0] */
        switch (frkRtnVal = fork()) {
            case -1:   /* fork returns error to parent process */
                perror("fork failed");
                exit(1);
            case 0:    /* Code executed only by child process */
                execvp(v[0], v);
                perror("exec failed");   /* Exec failed */
                exit(1);   /* Exit if exec fails */
            default:   /* Code executed only by parent process */
                if (background) {
                    newBackgroundProcess(frkRtnVal, v[0]);
                } else {
                    /* Wait for the child to finish only if it's not a background process */
                    if (waitpid(frkRtnVal, NULL, 0) == -1 && errno != ECHILD) {
                        perror("waitpid failed");
                    }
                }
        }  /* switch */
    }  /* while */
    return 0;
}
