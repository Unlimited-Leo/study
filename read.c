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

void r_m(int signo)
{
    printf("from father message:%s\n",sh_pbuf->buf);

}

int main(void)
{
    int shm_id;
    key_t key;
    pid_t pid;
   // sh_buf *sh_pbuf;


     key = ftok("./",0);
    shm_id = shmget(key,sizeof(struct sh_buf),0666|IPC_CREAT);
    if(shm_id==-1)
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
         printf("child attach shm_id is %p\n",sh_pbuf);
        system("ipcs -m");
    }

    sh_pbuf->pid_r = getpid();
    sh_pbuf->pid_w =0;
    memset((void*)sh_pbuf->buf,0,BUFF);
    while(sh_pbuf->pid_w==0);
    signal(10,r_m());
    //SIGUSR1,SIGUSR2
    do
    {
        pause();
        kill(sh_pbuf->pid_w,12);

    }
    while(strncmp(sh_pbuf->buf,"quit",4));

    printf("write over\n");

        if((shmdt(sh_pbuf))<0) //撤销地址映射
        {
            printf("shmdt error\n");
            exit(1);
        }
        exit(0);

    return 0;

}