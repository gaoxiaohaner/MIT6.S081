#include "kernel/types.h"
#include "user/user.h"

void primes(int fd[])
{
    // fd管道信息
    // 需要接收信息吧，那就把写端关闭掉吧
    close(fd[1]);
    int number = 0;

    int next_pipe[2]; // 下一个管道 下一个管道对于当前的primes来说是右邻居
    pipe(next_pipe);

    // 这里sizeof(int) 与下边的sizeof(i) 一样吗获取到的数据
    // 这里程序得健壮一下吧，读取失败咋办
    if (read(fd[0], &number, sizeof(int)) == sizeof(int)) // 接收一个int型的长度的信息 从上一个管道接收来的
    {
        printf("prime %d\n", number);

        // 创建一个进程去接收当前管道的信息
        int pid = fork();
        if (pid != 0){
            // 要让当前父进程，提前去进行下一轮循环，方便接收子进程传递过来的素数数据
            primes(next_pipe);
            exit(0);
        }
        else{
            // 子进程接着筛选
            // 因为primes的第一个一定是素数，所以下边就是要循环的接收，然后判断是否是素数，如果不是pass
            // 如果是就继续向右传递
            close(next_pipe[0]);
            int num = 0;
            while (read(fd[0], &num, sizeof(int)) == sizeof(int))
            {
                // read 如果没有数据返回的是0
                // 如果从左邻居一直有数据进行传输的话 就需要进行筛选往右邻居去
                // 素数判断
                if (num % number != 0)
                {
                    write(next_pipe[1], &num, sizeof(int));
                }
            }
            // 结束循环
            close(next_pipe[1]);
            wait(0);
        }
    }
}

int main()
{

    int fd[2];
    pipe(fd); // pipe(int *)
    // fd[0] 读端、fd[1] 写端

    // 去开始创建子进程，然后求素数
    // 难点在于，通过轮回的递归的使用管道，来进行每一轮次的素数筛选

    int pid = fork();
    if (pid == 0)
    {
        // 去作为左邻居，求素数，然后发送给右邻居
        primes(fd); // 将管道进行传输
    }
    else if (pid > 0)
    {
        // father 将所有的数据发送给子进程进行筛选
        for (int i = 2; i <= 35; i++)
        {

            // 这里可以进行健壮性的检查
            write(fd[1], &i, sizeof(i));
        }
        close(fd[1]);
        wait(0);
    }
    else
    {
        fprintf(2, "fork error.\n");
        exit(1);
    }
    exit(0);
}