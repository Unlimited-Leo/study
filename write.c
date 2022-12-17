#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <wait.h>

#define BUFF 1024

struct sh_buf
{
    int pid_w;
    int pid_r;
    char buf[BUFF];
}*sh_pbuf;

void w_w(int signo)
{
    memset((void*)sh_pbuf->buf,0,BUFF);
    printf("in father process:\nplease write message\n");
    gets(sh_pbuf->buf);
    //当从标准输入中获取数据，发送信号给对方，告诉已发送数据

}

int main(void)
{
    int shm_id;
    key_t key;
    pid_t pid;
    //sh_buf *sh_pbuf;

    key = ftok("./",0);

    shm_id = shmget(key,sizeof(struct sh_buf),0666);
   if(shm_id == -1)
    {
        perror("shmget error\n");
        exit(1);
    }
    else
    {
        printf("shm_id is %d\n",shm_id);
        system("ipcs -m");
    }

    sh_pbuf=shmat(shm_id,NULL,0);
    if(sh_pbuf == (void *)-1)
    {
            printf("shmat error\n");
            exit(1);
    }
    else
    {
         printf("father attach shm_id is %p\n",sh_pbuf);
        system("ipcs -m");
    }

    sh_pbuf->pid_w = getpid();
    signal(SIGUSR2,w_w);
    printf("in father process:\nplease write message\n");
    gets(sh_pbuf->buf);

    do
    {
        kill(sh_pbuf->pid_r,SIGUSR1);
        pause();
    }
    while(strncmp(sh_pbuf->buf,"quit",4));
    kill(sh_pbuf->pid_r,SIGUSR1);
    waitpid(sh_pbuf->pid_r,NULL,0);
    printf("read over\n");
    if((shmdt(sh_pbuf))<0) //撤销地址映射
        {
            printf("shmdt error\n");
            exit(1);
        }
        if((shmctl(shm_id,IPC_RMID,NULL))<0)
        {
            printf("shmctl error\n");
            exit(1);

        }
        exit(0);

    return 0;

}