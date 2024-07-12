#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"

int main(int argc, char *argv[]){
    if (argc <= 1){
        fprintf(2, "losing time to wait!\n");
        exit(1);
    }
    int num = atoi(argv[1]);
    sleep(num);
    exit(0);
}