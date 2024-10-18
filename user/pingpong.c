#include "kernel/types.h"
#include "user.h"

int main(int argc, char* argv[])
{
    int c2f[2];
    int f2c[2];
    pipe(c2f);
    pipe(f2c);

    int pid = fork();
    if(pid < 0)
    {
        fprintf(2, "fork failed\n");
        exit(-1); 
    }

    if (pid == 0) {
        // Child process
        int fpid;
        int cpid = getpid();
        read(f2c[0], &fpid, sizeof(fpid));  
        write(c2f[1], &cpid, sizeof(cpid));  
        fprintf(1, "%d: received ping from pid %d\n", cpid, fpid); 

        exit(0); 
    } else {
        // Parent process
        int fpid = getpid();
        int cpid;
        write(f2c[1], &fpid, sizeof(fpid));  
        read(c2f[0], &cpid, sizeof(cpid));  
        wait(0);
        fprintf(1, "%d: received pong from pid %d\n", fpid, cpid);  
    }

    exit(0);
}