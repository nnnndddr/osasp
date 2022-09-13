#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <libgen.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/time.h>

const char* PROC_DIR = "procs";
int SIG1N = 0, SIG2N = 0;
pid_t pid0 = 0, pid1 = 0, pid2 = 0, pid3 = 0, pid4 = 0, pid5 = 0, pid6 = 0, pid7 = 0, pid8 = 0;

char* getNow() {
    struct timeval te;
    errno = 0;
    char* buffer = (char*) malloc(16);
    gettimeofday(&te, NULL);
    if (errno != 0) {
        perror("gettimeofday");
    }
    long msec = te.tv_usec / 1000;
    long sec = te.tv_sec % 60;
    long min = (te.tv_sec / 60) % 60;
    long hours = (te.tv_sec / 3600 + 3) % 24;
    sprintf(buffer, "%02ld:%02ld:%02ld:%03ld", hours, min, sec, msec);
    return buffer;
}

void logger(int childNum, char *action, int sigNum) {
    printf("[ %s ]: PROC%d - PID: %5d - PPID: %5d -%4s SIGUSR%d\n", getNow(), childNum, getpid(), getppid(), action, sigNum);
}

void termLogger(int childNum) {
    printf("PROC%d (PID: %4d; PPID: %4d) terminated after %3d SIGUSR1 and %3d SIGUSR2\n", childNum, getpid(), getppid(), SIG1N, SIG2N);
}

void bornLogger(int parN, int parPid, int chiN, int chiPid) {
    printf("PROC%d (%d) => PROC%d (%d)\n", parN, parPid, chiN, chiPid);
}

void handler7() {//1
    static int received = 0;
    received++;

    logger(7, "get", 1);

    if (received == 101) {
        sleep(1);
        if (kill(-pid4, SIGTERM) == -1)
            perror("kill");
        int children = 2; // process 4,5
        while (children--)
            wait(NULL);
        termLogger(1);////////////////
        exit(0);
    } else {
        if (kill(-pid4, SIGUSR2) == -1) {
            perror("kill");
        } else {
            logger(7, "put", 2);
        }
        SIG2N++;
    }
}

void handler2() {
    logger(2, "get", 2);
    if (kill(pid1, SIGUSR2) == -1)
        perror("kill");
    else
        logger(2, "put", 2);
    SIG2N++;
}

void handler1() {
    logger(1, "get", 2);
    if (kill(pid6, SIGUSR1) == -1)
        perror("kill");
    else
        logger(1, "put", 1);
    SIG1N++;
}

void handler6() {
    logger(6, "get", 1);
    if (kill(pid7, SIGUSR1) == -1)
        perror("kill");
    else
        logger(6, "put", 1);
    SIG1N++;
}

void handler3() {//5
    logger(3, "get", 2);
}

void handler8() {//6
    logger(8, "get", 1);
    if (kill(pid2, SIGUSR2) == -1)
        perror("kill");
    else
        logger(8, "put", 2);
    SIG2N++;
}

void handler5() {
    logger(5, "get", 2);
    if (kill(pid2, SIGUSR1) == -1)
        perror("kill");
    else
        logger(5, "put", 1);
    SIG1N++;
}

void handler4() {
    logger(4, "get", 2);
    if (kill(pid8, SIGUSR1) == -1)
        perror("kill");
    else
        logger(4, "put", 1);
    SIG1N++;
}

void handlerTerm2() {
    if (kill(pid6, SIGTERM) == -1) {
        perror("kill");
    }
    wait(NULL);

    termLogger(2);
    exit(0);
}

void handlerTerm3() {
    if (kill(pid7, SIGTERM) == -1) {
        perror("kill");
    }
    wait(NULL);

    termLogger(3);
    exit(0);
}

void handlerTerm4() {
    if (kill(pid8, SIGTERM) == -1) {
        perror("kill");
    }
    wait(NULL);

    termLogger(4);
    exit(0);
}

void handlerTerm5() {
    termLogger(5);
    exit(0);
}

void handlerTerm6() {
    termLogger(6);
    exit(0);
}

void handlerTerm7() {
    termLogger(7);
    exit(0);
}

void handlerTerm8() {
    termLogger(8);
    exit(0);
}

void createFile(char N) {
    char *filename = (char *) malloc(16);
    sprintf(filename, "%s/%c.pid", PROC_DIR, N);
    FILE *f = NULL;
    f = fopen(filename, "w");
    if (f == NULL) {
        perror("fopen");
        return;
    }
    fprintf(f, "%d", getpid());
    errno = 0;
    fclose(f);
    if (errno != 0) {
        perror("fclose");
    }

    free(filename);
}

pid_t getPidFromFile(char N) {
    char *filename = (char *) malloc(16);
    sprintf(filename, "%s/%c.pid", PROC_DIR, N);
    pid_t result = 0;

    while (result == 0) {
        FILE *f = NULL;
        while ((f = fopen(filename, "r")) == NULL);
        fscanf(f, "%d", &result);
        fclose(f);
    }

    free(filename);
    return result;
}

void createProcessTree() {
    pid0 = getpid();

    pid1 = fork();

    // Creating 1->2
    if (!pid1) {
        pid1 = getpid();
        bornLogger(0, pid0, 1, pid1);
        pid2 = fork();
        if (!pid2) {
            pid2 = getpid();
            bornLogger(1, pid1, 2, pid2);
        }
        else if (pid2 == -1)
            perror("fork");
    }
    else if (pid1 == -1) {
        perror("fork");
    }

    if (pid1 == getpid()) {
        // create process 3
        pid3 = fork();
        if (!pid3) {
            pid3 = getpid();
            bornLogger(1, pid1, 3, pid3);////
        }
        else if (pid3 == -1)
            perror("fork");
    }

    if (pid1 == getpid()) {

        // create process 4
        pid4 = fork();
        if (!pid4) {
            pid4 = getpid();
            bornLogger(1, pid1, 4, pid4);
        }
        else if (pid4 == -1)
            perror("fork");
    }

    if (pid1 == getpid()) {

        // create process 5
        pid5 = fork();
        if (!pid5) {
            pid5 = getpid();
            bornLogger(1, pid1, 5, pid5);
        }
        else if (pid5 == -1)
            perror("fork");
    }

    // if pid4 is created than creating 6
    if (pid1 == getpid()) {
        pid6 = fork();
        if (!pid6) {
            pid6 = getpid();
            bornLogger(1, pid1, 6, pid6);
        }
        else if (pid6 == -1)
            perror("fork");
    }

    // if pid3 is created than creating 7
    if (pid1 == getpid()) {
        pid7 = fork();
        if (!pid7) {
            pid7 = getpid();
            bornLogger(1, pid1, 7, pid7);
        }
        else if (pid7 == -1)
            perror("fork");
    }

    // if pid5 is created than creating 8
    if (pid7 == getpid()) {
        pid8 = fork();
        if (!pid8) {
            pid8 = getpid();
            bornLogger(7, pid7, 8, pid8);
        }
        else if (pid8 == -1)
            perror("fork");
    }
}

void exchangeSignals() {
    struct sigaction sg;
    sigemptyset(&sg.sa_mask);
    sg.sa_flags = SA_SIGINFO;

    struct sigaction sgTerm;
    sigemptyset(&sgTerm.sa_mask);
    sgTerm.sa_flags = SA_SIGINFO;

    if (getpid() == pid1) {
        createFile('1');

        const int required = 8;
        int curr = 0;
        // Wait while all child create pid file
        while (curr != required) {
            curr = 0;
            DIR *currdir;
            if (!(currdir = opendir(PROC_DIR))) {
                perror("opendir");
                exit(1);
            }

            struct dirent *cdirent;
            errno = 0;

            while ((cdirent = readdir(currdir))) {
                if (!strcmp(".", cdirent->d_name) || !strcmp("..", cdirent->d_name)) {
                    continue;
                }
                curr++;
            }
            if (errno)
                perror("");
            if (closedir(currdir) == -1)
                perror("closedir");

        }
        pid2 = getPidFromFile('2');
        pid3 = getPidFromFile('3');
        pid4 = getPidFromFile('4');
        pid5 = getPidFromFile('5');

        sg.sa_sigaction = handler1;
        if (sigaction(SIGUSR1, &sg, NULL) == -1) {
            perror("sigaction");
            exit(1);
        }

        // Send first signal to process group (2, 3, 4, 5)
        if (kill(-pid2, SIGUSR2) == -1)
            perror("kill");
        else
            logger(1, "put", 1);
        SIG1N++;
    }

    if (getpid() == pid2) {
        sg.sa_sigaction = handler2;
        if (sigaction(SIGUSR2, &sg, NULL) == -1) {
            perror("sigaction");
            exit(-1);
        }
        sgTerm.sa_sigaction = handlerTerm2;
        if (sigaction(SIGTERM, &sgTerm, NULL) == -1) {
            perror("sigaction");
            exit(-1);
        }
        pid6 = getPidFromFile('6');
        if (setpgid(pid2, pid2) == -1) {
            perror("sigaction");
            exit(-1);
        }
        createFile('2');
    }

    if (getpid() == pid3) {
        sg.sa_sigaction = handler3;
        if (sigaction(SIGUSR2, &sg, NULL) == -1) {
            perror("sigaction");
            exit(1);
        }
        sgTerm.sa_sigaction = handlerTerm3;
        if (sigaction(SIGTERM, &sgTerm, NULL) == -1) {
            perror("sigaction");
            exit(-1);
        }
        // Getting this pids from file to send signals
        pid2 = getPidFromFile('2');
        pid7 = getPidFromFile('7');
        if (setpgid(pid3, pid2) == -1) {
            perror("sigaction");
            exit(-1);
        }
        createFile('3');
    }

    if (getpid() == pid4) {
        sg.sa_sigaction = handler4;
        if (sigaction(SIGUSR2, &sg, NULL) == -1) {
            perror("sigaction");
            exit(-1);
        }
        sgTerm.sa_sigaction = handlerTerm4;
        if (sigaction(SIGTERM, &sgTerm, NULL) == -1) {
            perror("sigaction");
            exit(-1);
        }
        pid2 = getPidFromFile('2');
        pid8 = getPidFromFile('8');
        if (setpgid(pid4, pid2) == -1) {
            perror("sigaction");
            exit(-1);
        }

        createFile('4');
    }

    if (getpid() == pid5) {
        sg.sa_sigaction = handler5;
        if (sigaction(SIGUSR2, &sg, NULL) == -1) {
            perror("sigaction");
            exit(-1);
        }
        sgTerm.sa_sigaction = handlerTerm5;
        if (sigaction(SIGTERM, &sgTerm, NULL) == -1) {
            perror("sigaction");
            exit(-1);
        }
        pid2 = getPidFromFile('2');
        if (setpgid(pid5, pid2) == -1) {
            perror("sigaction");
            exit(-1);
        }
        createFile('5');
    }


    if (getpid() == pid6) {
        sg.sa_sigaction = handler6;
        if (sigaction(SIGUSR1, &sg, NULL) == -1) {
            perror("sigaction");
            exit(-1);
        }
        sgTerm.sa_sigaction = handlerTerm6;
        if (sigaction(SIGTERM, &sgTerm, NULL) == -1) {
            perror("sigaction");
            exit(-1);
        }

        if (setpgid(pid6, pid6) == -1) {
            perror("sigaction");
            exit(-1);
        }

        createFile('6');
    }

    if (getpid() == pid7) {
        sg.sa_sigaction = handler7;
        if (sigaction(SIGUSR1, &sg, NULL) == -1) {
            perror("sigaction");
            exit(-1);
        }
        sgTerm.sa_sigaction = handlerTerm7;
        if (sigaction(SIGTERM, &sgTerm, NULL) == -1) {
            perror("sigaction");
            exit(-1);
        }
        if (setpgid(pid7, pid7) == -1) {
            perror("sigaction");
            exit(-1);
        }
        createFile('7');
    }

    if (getpid() == pid8) {
        sg.sa_sigaction = handler8;
        if (sigaction(SIGUSR1, &sg, NULL) == -1) {
            perror("sigaction");
            exit(-1);
        }
        sgTerm.sa_sigaction = handlerTerm8;
        if (sigaction(SIGTERM, &sgTerm, NULL) == -1) {
            perror("sigaction");
            exit(-1);
        }
        pid1 = getPidFromFile('1');
        if (setpgid(pid8, pid8) == -1) {
            perror("sigaction");
            exit(-1);
        }
        createFile('8');
    }

    if (getpid() == pid0) {
        wait(NULL);

        DIR *currdir;
        if (!(currdir = opendir(PROC_DIR))) {
            perror("opendir");
            exit(-1);
        }

        struct dirent *cdirent;
        errno = 0;

        while ((cdirent = readdir(currdir))) {
            if (!strcmp(".", cdirent->d_name) || !strcmp("..", cdirent->d_name)) {
                continue;
            }
            char *filename = (char *) malloc(strlen(PROC_DIR) + 7);
            sprintf(filename, "%s/%s", PROC_DIR, cdirent->d_name);
            if (remove(filename) == -1) perror("remove");
            free(filename);
        }
        if (errno)
            perror("");
        if (closedir(currdir) == -1)
            perror("closedir");

        if (rmdir(PROC_DIR) == -1) {
            perror("rmdir");
            exit(-1);
        }
    } else {
        while (1) pause();
    }
}

int main(int argc, char *argv[]) {
    if (mkdir(PROC_DIR, 0777) == -1) {
        perror("mkdir");
        exit(-1);
    }
    createProcessTree();
    exchangeSignals();
    exit(0);
}