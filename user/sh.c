#include "types.h"
#include "user.h"
#include "fcntl.h"

#define MAXARGS 10
#define MAXHIST 50

char history[MAXHIST][128];
int hist_count = 0;

void add_history(char *cmd){
    if(hist_count < MAXHIST){
        strcpy(history[hist_count++], cmd);
    } else {
        for(int i=1; i<MAXHIST; i++)
            strcpy(history[i-1], history[i]);
        strcpy(history[MAXHIST-1], cmd);
    }
}

void runcmd(char *cmd){
    add_history(cmd);

    char *argv[MAXARGS];
    int argc=0;
    char *token = strtok(cmd, " ");
    while(token && argc<MAXARGS){
        argv[argc++] = token;
        token = strtok(NULL, " ");
    }
    argv[argc]=0;

    if(argc==0) return;
    if(fork()==0){
        exec(argv[0], argv);
        printf(2,"exec %s failed\n",argv[0]);
        exit();
    } else {
        wait();
    }
}

int main(void){
    char buf[128];
    while(1){
        printf(1,"$ ");
        gets(buf,sizeof(buf));
        if(buf[0]==0) continue;
        runcmd(buf);
    }
}
