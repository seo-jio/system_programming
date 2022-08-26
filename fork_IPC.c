#include<stdio.h>
#include<stdlib.h>
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
    int flag = 0;
    int fd1[2];
    int fd2[2];
    pipe(fd1);
    pipe(fd2);
    
    pid = fork();
    
    if (pid != 0){   
        pid = fork();
        if (pid == 0){
            flag = 2;
        }
    }
    else{
        flag = 1;
    }

    if(flag == 0){
        close(fd1[0]);
        close(fd2[0]);
        close(fd2[1]);
        if((fdi=open(argv[1], O_RDONLY)) == -1){
            perror(argv[1]);
            exit(0);
        }
        while((n = read(fdi, &buf, 10)) != 0){
            printf("write into pipe1 %s \n", buf);           
            write(fd1[1], &buf, n);
        }
        close(fd1[1]);
        wait(&status);
        wait(&status);
        exit(0);
    }
    else if(flag == 1){
        close(fd1[1]);
        close(fd2[0]);
       
        while((n = read(fd1[0], &buf, 10)) != 0){
            for(int i = 0; i < 10; i++){
                buf[i] = buf[i] - 32;
            }
            write(fd2[1], &buf, n);       
            printf("write into pipe1 %s\n", buf);
        }
        close(fd1[0]);
        close(fd2[1]);
        exit(0);
       
    }
    else{
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[1]);
        if((fdo=open(argv[2], O_WRONLY | O_CREAT | O_TRUNC | O_EXCL | S_IRUSR | S_IWUSR)) == -1){
            perror(argv[2]);
            exit(0);
        }
        while((n = read(fd2[0], &buf, 10)) != 0){
            write(fdo, &buf, 10);
            printf("write into destination file %s\n", buf);
        }
        close(fd2[0]);
        exit(0);
    }
    close(fdi);
    close(fdo);
    return 0;
}