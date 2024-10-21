#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// Signal handler for SIGHUP
void handle_sighup(int sig) {
    printf("Ouch!\n");
    fflush(stdout);
}

// Signal handler for SIGINT
void handle_sigint(int sig) {
    printf("\rYeah!\n");
    fflush(stdout);
}


int main(int argc, char *argv[]){
    // Install signal handlers
    signal(SIGHUP, handle_sighup);
    signal(SIGINT, handle_sigint);

    // get process PID
    // printf("Process PID: %d\n", getpid());

    // get input n 
    int n = atoi(argv[1]);

    // print first n even numbers
    for (int i = 0; i < n; i++) {
        int even_number = 2 * i;
        printf("%d\n", even_number);
        fflush(stdout);  // Ensure immediate output
        sleep(5);
    }
    
    


    return 0;
}