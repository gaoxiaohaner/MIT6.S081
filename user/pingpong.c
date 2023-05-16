#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv){
    // 创建2个管道 . 一个管道有读取两端
    int pfather[2]; 
    int pson[2]; 
    pipe(pfather);
    pipe(pson);

    // pipe(p) p[0] 是管道的读取端 p[1] 是管道的写入端

    // fork()在父进程中返回子进程的PID
    int pid = fork();
    if(pid == 0 ) {
        // 这是子进程
        char buf[128];

        close(pfather[0]); // 关闭掉father的读,像father中写入
        close(pson[1]); // 关掉son的写段,只用读

        // 子进程先打印出来
        
        // 如果读取的是一个字节的话
        if(read(pson[0], buf, 1) == 1){
            printf("%d: received ping\n",getpid());
        }else{
            printf("son received error \n");
            exit(1);
        }
        close(pson[0]);

        if(write(pfather[1], "pong\n", 1)!=1){
            printf("son write error \n");
            exit(1);
        }        
        close(pfather[1]);

    } else if(pid > 0) {
        // 这是父进程
        char buf[128];

        close(pson[0]); 
        close(pfather[1]);

        // 写入是否是一个字节
        if(write(pson[1], "ping\n", 1)!=1){
            printf("father write error \n");
            exit(1);
        }
        close(pson[1]);

        // 如果读取的是一个字节的话
        if(read(pfather[0], buf, 1) == 1){
            printf("%d: received pong\n",getpid());
        }else{
            printf("father received error \n");
            exit(1);
        }
        close(pfather[0]);
    }else{
        fprintf(2,"fork error\n");
        exit(1);
    }


    exit(0);

}