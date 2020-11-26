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
	message_buf buf;
	
	for (int i = 0 ; i < 10 ; i++)
	{
		//sleep(1);
		buf.mtype = i;
		if ((i % 2) == 0)
		{
			strcpy(buf.mtext, "Hello motherfucker");
			int buf_length = strlen(buf.mtext) + 1 ;
			msgsnd(msgid , &buf , buf_length , 0);
		}
		else
		{
			strcpy(buf.mtext, "Hello fatherfucker");
			int buf_length = strlen(buf.mtext) + 1 ;
			msgsnd(msgid , &buf , buf_length , 0);
		};
	};


	return 0;
}