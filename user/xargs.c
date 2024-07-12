#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]){
    if (argc < 2){
        fprintf(2, "Usage: xargs <command> [args...]\n");
        exit(1);
    }

    char *maxArgs[MAXARG] = {0};
    int index = 0;

    // Copy initial arguments (command and its arguments)
    for (int i = 1; i < argc; ++i){
        maxArgs[index++] = argv[i];
    }

    char buf[1024];
    int n;
    while ((n = read(0, buf, sizeof(buf))) > 0) {
        int bufIndex = 0;
        while (bufIndex < n) {
            while (bufIndex < n && (buf[bufIndex] == ' ' )) {
                bufIndex++;
            }
            if (bufIndex >= n || buf[bufIndex] == '\n') break;

            maxArgs[index++] = &buf[bufIndex];
            while (bufIndex < n && buf[bufIndex] != ' ' && buf[bufIndex] != '\n') {
                bufIndex++;
            }

            if (bufIndex < n) {
                buf[bufIndex++] = '\0';
            }
        }

        if (fork() == 0) {
            exec(maxArgs[0], maxArgs);
            fprintf(2, "exec %s failed\n", maxArgs[0]);
            exit(1);
        } else {
            wait(0);
        }

        index = argc - 1; // Reset index for the next input
    }

    exit(0);
}
