#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>

void CheckError(int Expression, char * message){
	if (Expression){
		perror (message);
		exit (EXIT_FAILURE);
	}
}

const int MAX_FULL_NAME = 1024;

int copy_dir (int dir_from, char * path);

int main(int argc, char ** argv){
	if ((argc < 2) || (argc > 3)){
		printf ("not right program start\n%s <dir> <dir>\n", argv[0]);
		exit (1);
	}

	errno = 0;
	int dir1_fd = open (argv[1], O_DIRECTORY);
	CheckError (dir1_fd == -1, "open error");

	errno = 0;
	int ret = mkdir (argv[2], 0777);
	CheckError (ret == -1 && errno != EEXIST, "mkdir error 1");

	copy_dir (dir1_fd, argv[2]);

	close (dir1_fd);

	return 0;
}

int copy_dir (int dir_from, char *path){

	DIR* dir = fdopendir(dir_from);

	struct dirent *dirstruct = NULL;
	int ret = 0;

	while ((dirstruct = readdir (dir)) != NULL){

		char * filepath = (char *) calloc ( MAX_FULL_NAME, sizeof(filepath[0]));
		sprintf (filepath, "%s/%s", path, dirstruct->d_name);										//create full dir name
 
		if ((dirstruct->d_type) == DT_DIR && strcmp(dirstruct->d_name, "..") && strcmp(dirstruct->d_name, ".")){
			
			int tmpdir = openat (dir_from, dirstruct->d_name, O_DIRECTORY);
			CheckError (tmpdir == -1, "open error");

			int dir_to = open ( path, O_DIRECTORY);
			CheckError (dir_to == -1, "open error");

			ret = mkdirat (dir_to, dirstruct->d_name, 0777);
			CheckError (ret == -1 && errno != EEXIST, "mkdir error 2");


			copy_dir (tmpdir, filepath);

			close (tmpdir);
			close (dir_to);

		}

		if ((dirstruct->d_type) == DT_REG){

			struct stat statbuf1 = {};
			struct stat statbuf2 = {};
			fstatat (dir_from, dirstruct->d_name, &statbuf1, 0);
			stat ( filepath, &statbuf2);


			if ((statbuf1.st_mtim.tv_sec) != (statbuf2.st_mtim.tv_sec)){

				int filesource = openat (dir_from, dirstruct->d_name, O_RDONLY, 0777);
				CheckError (filesource < 0, "open error");

				int filefd = open ( filepath, O_CREAT | O_WRONLY, 0777);
				CheckError (filefd < 0 && errno != EEXIST, "open error");

				sendfile (filefd, filesource, NULL, statbuf1.st_size);								//send file

				struct timeval tvp[2] = { {statbuf1.st_atim.tv_sec}, {statbuf1.st_mtim.tv_sec}};	//create timeval structure   

				utimes(filepath, tvp);																//change newfile modification time

				close (filesource);
				close (filefd);
			}
		}
		free (filepath);
	}
	return 0;

}