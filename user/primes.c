#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define RD 0
#define WR 1

void dfs(int p[]){
    //p是与上一个进程建立的管道
    close(p[WR]);
    int num = -1;
    if (read(p[RD], &num, sizeof(int)) == 0){
        close(p[RD]);
        return;
    }else{
        printf("prime %d\n", num);
    }
    int cur[2];//当前进程及其子进程通信的管道
    pipe(cur);
    if (fork() == 0){
        close(p[RD]);
        dfs(cur);
    }else{
        close(cur[RD]);
        int n;
        while (read(p[RD], &n, sizeof(int)) == sizeof(int)){
            if (n % num){
                write(cur[WR], &n, sizeof(int));
            }
        }
        close(cur[WR]);
        wait((void*)(0));
    }
}

int main(int argc, char *argv[]){
    if (argc <= 1){
        fprintf(2, "please enter a num!\n");
        exit(1);
    }
    int num = atoi(argv[1]);
    int p[2];
    pipe(p);
    if (fork() == 0){
        dfs(p);
    }else{
        close(p[RD]);
        //主进程：负责向子进程发送2-35
        for (int i = 2; i <= num; ++i){
            write(p[WR], &i, sizeof(int));
        }
        close(p[WR]);
        wait((void*)(0));
    }

    exit(0);
}