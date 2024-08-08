#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// Signal handler for SIGHUP
void handle_sighup(int sig) {
    printf("Ouch!\n");
    // Code to re-read configuration files or perform other actions
}

// Signal handler for SIGINT
void handle_sigint(int sig) {
    printf("Yeah!\n");
}


int main(void){
    // Install signal handlers
    signal(SIGHUP, handle_sighup);
    signal(SIGINT, handle_sigint);

    // get process PID
    printf("Process PID: %d\n", getpid());

    // get input n 
    int n = 0;
    printf("Enter number n: ");
    scanf("%d", &n);

    // print even numbers 
    int k = 0;
    while (k <= n){
        printf("%d\n",k);
        fflush(stdout); 
        sleep(5);
        k += 2;
    }
    
    


    return 0;
}