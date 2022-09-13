#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>

int daytime [4];
pid_t pid1, pid2;
int mesNum;

void GetTime () {
	struct timeval tv;
	gettimeofday( &tv, NULL );
	int msecs = tv.tv_usec / 1000;
	int secs = tv.tv_sec % 60;
	int mins = (tv.tv_sec / 60) % 60;
	int hrs = (tv.tv_sec / 3600 + 3) % 24;
    daytime[0] = hrs;
    daytime[1] = mins;
    daytime[2] = secs;
    daytime[3] = msecs;
}

void ShowInfo (siginfo_t *sigInfo, char *action, int signal) 
{
    GetTime();
    printf("%d PID: %d from %d Time: %02d:%02d:%02d:%03d %s SIGUSR%d\n",mesNum++,getpid(),sigInfo->si_pid,daytime[0],daytime[1],daytime[2],daytime[3], action,signal);
}

void childhandler(int sig,siginfo_t *sigInfo, void *_)
{
    ShowInfo (sigInfo, "get", 1);
    kill(getppid(), SIGUSR2);
    ShowInfo (sigInfo, "put", 2);
}

void parenthandler(int sig, siginfo_t *sigInfo, void *_)
{
    ShowInfo (sigInfo, "get", 2);

    struct timespec t = {.tv_nsec = 100e6, .tv_sec = 0};
    nanosleep(&t, NULL);

    kill(0, SIGUSR1);
    ShowInfo (sigInfo, "put", 1);
}

int main() {
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = parenthandler;
    sigaction(SIGUSR2, &sa, NULL);
    signal(SIGUSR1, SIG_IGN);

    pid1 = fork();
    if (pid1 == 0)  {
        GetTime();
        printf("Child №1 PID: %d Parent: %d Time: %02d:%02d:%02d:%03d\n",getpid(),getppid(),daytime[0],daytime[1],daytime[2],daytime[3]);
        
        struct sigaction sa;
        sa.sa_flags = SA_SIGINFO;
        sa.sa_sigaction = childhandler;
        sigaction(SIGUSR1, &sa, NULL);
    } else if (pid1 > 0) {
    	pid2 = fork();
	    if (pid2 == 0)   {
            GetTime();
            printf("Child №2 PID: %d Parent: %d Time: %02d:%02d:%02d:%03d\n",getpid(),getppid(),daytime[0],daytime[1],daytime[2],daytime[3]);
            
            struct sigaction sa;
            sa.sa_flags = SA_SIGINFO;
            sa.sa_sigaction = childhandler;
            sigaction(SIGUSR1, &sa, NULL);
	    } else if (pid2 > 0) {
            sleep(1);
            kill(0, SIGUSR1);
	    } else {
	    	perror("fork() error"); 
		    exit(-1);
	    }
    } else {
    	perror("fork() error"); 
        exit(-1);
    }

    while(1);
}