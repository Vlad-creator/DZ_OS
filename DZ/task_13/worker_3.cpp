#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

#define MSGSZ 128

typedef struct {
         long    mtype;
         char    mtext[MSGSZ];
         } message_buf;

int main()
{
	key_t key = ftok("messege" , 1);
	int msgid = msgget(key , IPC_CREAT | 0666);
	char check[] = "1";
	message_buf buf_1;
	message_buf buf_2;
	message_buf buf_3;
	int count_det = 0;
	scanf("%d" , &count_det);

	message_buf count;
	count.mtype = count_det;
	msgsnd(msgid , &count , 0 , 0);
	msgsnd(msgid , &count , 0 , 0);
	
	for (int i = 0 ; i < count_det ; i++)
	{
		buf_1.mtype = 1;
		buf_2.mtype = 2;
		
		strcpy(buf_1.mtext, "1");
		strcpy(buf_2.mtext, "1");
		
		int buf_length = strlen(buf_1.mtext) + 1 ;
		
		msgsnd(msgid , &buf_1 , buf_length , 0);
		msgsnd(msgid , &buf_2 , buf_length , 0);

		msgrcv(msgid, &buf_3, MSGSZ, 3, 0);
		if (strncmp(check , buf_3.mtext , 1) != 0)
		{
			printf("ERROR");
			strcpy(buf_1.mtext, "error");
			break;
		}
		msgrcv(msgid, &buf_3, MSGSZ, 3, 0);
		if (strncmp(check , buf_3.mtext , 1) != 0)
		{
			printf("ERROR");
			strcpy(buf_2.mtext, "error");
			break;
		}
		printf("detail is ready\n");
	};
	return 0;
}