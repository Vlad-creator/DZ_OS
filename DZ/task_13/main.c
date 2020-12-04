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

int worker_1()
{
    key_t key = ftok("messege" , 1);
    int msgid = msgget(key , IPC_CREAT | 0666);
    message_buf buf_check;
    message_buf buf;
    buf.mtype = 3;
    char check[] = "1";
    int det = 0;

        msgrcv(msgid, &buf_check, MSGSZ, 1, 0);
        if (strcmp(check , buf_check.mtext) != 0)
        {
            printf("ERROR");
            strcpy(buf.mtext, "error");
            int buf_length = strlen(buf.mtext) + 1 ;
            msgsnd(msgid , &buf , buf_length , 0);
            return 1;
        }
        else
        {
            strcpy(buf.mtext, "1");
            int buf_length = strlen(buf.mtext) + 1 ;
            msgsnd(msgid , &buf , buf_length , 0);
        }
            return 0;
}

int worker_2()
{
    key_t key = ftok("messege" , 1);
    int msgid = msgget(key , IPC_CREAT | 0666);
    int det = 0;
    message_buf buf_check;
    message_buf buf;
    buf.mtype = 3;
    char check[] = "1";

    msgrcv(msgid, &buf_check, MSGSZ, 2, 0);
  	if (strcmp(check , buf_check.mtext) != 0)
    {
        printf("ERROR");
        strcpy(buf.mtext, "error");
        int buf_length = strlen(buf.mtext) + 1 ;
        msgsnd(msgid , &buf , buf_length , 0);
        return 1;
    }
    else
    {
        strcpy(buf.mtext, "1");
        int buf_length = strlen(buf.mtext) + 1 ;
        msgsnd(msgid , &buf , buf_length , 0);
    }

    return 0;
}

int worker_3_send()
{
	key_t key = ftok("messege" , 1);
	int msgid = msgget(key , IPC_CREAT | 0666);
	char check[] = "1";
	message_buf buf_1;
	message_buf buf_2;
	
	buf_1.mtype = 1;
	buf_2.mtype = 2;
	
	strcpy(buf_1.mtext, "1");
	strcpy(buf_2.mtext, "1");
	
	int buf_length = strlen(buf_1.mtext) + 1 ;
	
	msgsnd(msgid , &buf_1 , buf_length , 0);
	msgsnd(msgid , &buf_2 , buf_length , 0);
	return 0;
}


int worker_3_rcv()
{
	key_t key = ftok("messege" , 1);
	int msgid = msgget(key , IPC_CREAT | 0666);
	char check[] = "1";
	message_buf buf_3;

	msgrcv(msgid, &buf_3, MSGSZ, 3, 0);
	if (strncmp(check , buf_3.mtext , 1) != 0)
	{
		printf("ERROR");
		return 1;
	}
	msgrcv(msgid, &buf_3, MSGSZ, 3, 0);
	if (strncmp(check , buf_3.mtext , 1) != 0)
	{
		printf("ERROR");
		return 1;
	}
	printf("detail is ready\n");
	return 0;
}

int main()
{
	int count_det = 0;
	scanf("%d" , &count_det);

	for (int i = 0 ; i < count_det ; ++i)
	{
		worker_3_send();
		worker_2();
		worker_1();
		worker_3_rcv();
	}
	return 0;
}