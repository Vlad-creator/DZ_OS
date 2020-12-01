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
    message_buf buf_check;
    message_buf buf;
    buf.mtype = 3;
    char check[] = "1";
    int det = 0;

    message_buf count;
    msgrcv(msgid, &count, MSGSZ, 0, 0);
    det = count.mtype;

    for (int i = 0 ; i < det ; i++)
    {
        msgrcv(msgid, &buf_check, MSGSZ, 1, 0);
        if (strcmp(check , buf_check.mtext) != 0)
        {
            printf("ERROR");
            strcpy(buf.mtext, "error");
            int buf_length = strlen(buf.mtext) + 1 ;
            msgsnd(msgid , &buf , buf_length , 0);
            break;
        }
        else
        {
            strcpy(buf.mtext, "1");
            int buf_length = strlen(buf.mtext) + 1 ;
            msgsnd(msgid , &buf , buf_length , 0);
        }
    };
    return 0;
}