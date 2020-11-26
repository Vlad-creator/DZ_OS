#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <iostream>
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
    for (int i = 0 ; i < 10 ; ++i)
    {
    	sleep(3);
	    msgrcv(msgid, &buf, MSGSZ, 1, 0);
	    std::cout << buf.mtext << std::endl;
	};

    return 0;
}