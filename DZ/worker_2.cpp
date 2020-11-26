#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>

#define MSGSZ 128

typedef struct {
         long    mtype;
         char    mtext[MSGSZ];
         } message_buf;

int main()
{
    key_t key = ftok("messege" , 1);
    int msgid = msgget(key , IPC_CREAT | 0666);
    message_buf buf;
    for (int i = 0 ; i < 10 ; i++)
    {
        //sleep(2);
        msgrcv(msgid, &buf, MSGSZ, i, 0);
        printf("%s\n" , buf.mtext);
    };
    return 0;
}