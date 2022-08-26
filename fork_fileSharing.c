#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>

#define BUFFER_SIZE 10

int main(int argc, char *argv[])
{
    int fdi, fdo;
    char buf[BUFFER_SIZE];
    int n;
    int pid;
    int status;

    if((fdi=open(argv[1], O_RDONLY)) == -1){
        perror(argv[1]);
        exit(0);
    }
    if((fdo=open(argv[2], O_WRONLY | O_CREAT | O_TRUNC | O_EXCL | S_IRUSR | S_IWUSR)) == -1){
        perror(argv[2]);
        exit(0);
    }
    
    pid = fork();
    pid = fork();
    while((n=read(fdi, &buf, 10)) != 0){
        printf("%d %s\n", pid, buf);
        sleep(1);
        write(fdo, &buf, n);
    }
    if(pid != 0){
        wait(&status);
    }
    close(fdi);
    close(fdo);
    return 0;
}