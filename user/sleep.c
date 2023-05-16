#include "kernel/types.h"
#include "user/user.h"

int main(int argc , char *argv[]){
    // argc是命令行参数的个数
    // argv就是指针数组，每个元素保存变量内容

    // 根据提示写代码
    if(argc != 2){
        fprintf(2,"args error.\n");
        exit(1);
    } 
    else{
        int times = atoi(argv[1]);
        sleep(times);
        exit(0);
    }
}