#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h> 
#include <getopt.h>
#include <string.h>
#include "strtok.h"

#define MAX_COUNT 1024

int main(int argc, char** argv)
{
	int pipefd[2];
	int cpid =0;
	int count_str = 0;
	int status;

	if (pipe(pipefd) == -1)
	{
		printf ("FAILED\n");
		return 0;
	}

	if ((cpid = fork()) == -1)
	{
		printf("FAILED\n");
		return 0;
	}

	if (cpid == 0)
	{
		close(pipefd[1]);
		char** comands = (char**)calloc(sizeof(char*) , MAX_COUNT);
		char** strings = (char**)calloc(sizeof(char*) , MAX_COUNT);
		char* buf = (char*)calloc(sizeof(char) , MAX_COUNT);

		size_t readbytes_0 = 0;
		readbytes_0 = read(pipefd[0] , buf , MAX_COUNT);
		if (readbytes_0 == -1) 
		{
			perror("");
			exit(-1);
		}

	    for(int i = 0 ; ; ++i)
	    {
	    	char* f = 0;
		    f = strtok_string(&buf);
		    if (f == NULL)
		        break;
		    strings[i] = f;
		    count_str++;
	    }
		
		for (int j = 0 ; j < count_str ; ++j)
		{

		    for(int i = 0 ; ; ++i)
		    {
		    	char* f = 0;
			    f = strtok_word(&(strings[j]));
			    comands[i] = f;
			    if (f == NULL)
			    {
			    	free(f);
			        break;
			    }
		    }

		    int cpid_t = fork();
		    if (cpid_t == -1)
			{
				printf("FAILED\n");
				return 0;
			}

			if (cpid_t == 0)
			{
			int check = execvp(comands[0] , comands);
			if (check == -1)
				perror("");
			}

			waitpid(cpid_t , &status , WUNTRACED | WCONTINUED);
		}

		free(strings);
		free(comands);
        close(pipefd[0]);
        exit(EXIT_SUCCESS);
	}
	else
	{
		close(pipefd[0]);
	    size_t readbytes = 0;
		int fileid = open(argv[1] , O_RDONLY);
		char* cmd = (char*)calloc(MAX_COUNT, sizeof(char));

		readbytes = read(fileid , cmd, MAX_COUNT);
		if (readbytes == -1) 
		{
			perror("");
			exit(-1);
		}

		write(pipefd[1], cmd, MAX_COUNT);

		free(cmd);
        close(pipefd[1]); 
        close(fileid);
        wait(NULL);
        exit(EXIT_SUCCESS);
	}
}

