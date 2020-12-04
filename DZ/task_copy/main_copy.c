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
#include <assert.h>

const int MAX_NAME = 1024;

int copy (int from, char * adr);

int main(int argc, char ** argv){
	assert(argc > 2 && argc < 4);

	errno = 0;
	int dir1_fd = open (argv[1], O_DIRECTORY);
	assert(dir1_fd != -1);

	errno = 0;
	int ret = mkdir (argv[2], 0777);
	assert(ret != -1 || errno == EEXIST);

	copy (dir1_fd, argv[2]);

	close (dir1_fd);
	return 0;
}

int copy (int from, char *adr){

	DIR* direct = fdopendir(from);
	struct dirent *dirstr = NULL;
	int ret = 0;

	while ((dirstr = readdir (direct)) != NULL){
		char * fadr = (char *) calloc ( MAX_NAME, sizeof(fadr[0]));
		sprintf (fadr, "%s/%s", adr, dirstr->d_name);										
		if ((dirstr->d_type) == DT_DIR && strcmp(dirstr->d_name, "..") && strcmp(dirstr->d_name, ".")){
			int tmpdir = openat (from, dirstr->d_name, O_DIRECTORY);
			assert(tmpdir != -1);

			int to = open ( adr, O_DIRECTORY);
			assert(to != -1);

			ret = mkdirat (to, dirstr->d_name, 0777);
			assert(ret != -1 || errno == EEXIST);

			copy (tmpdir, fadr);

			close (tmpdir);
			close (to);

		}
		if ((dirstr->d_type) == DT_REG){
			struct stat buf1 = {};
			struct stat buf2 = {};
			fstatat (from, dirstr->d_name, &buf1, 0);
			stat ( fadr, &buf2);


			if ((buf1.st_mtim.tv_sec) != (buf2.st_mtim.tv_sec)){
				int source = openat (from, dirstr->d_name, O_RDONLY, 0777);
				assert(source >= 0);

				int filefd = open ( fadr, O_CREAT | O_WRONLY, 0777);
				assert(filefd >= 0 || errno == EEXIST);

				sendfile (filefd, source, NULL, buf1.st_size);						

				struct timeval t[2] = { {buf1.st_atim.tv_sec}, {buf1.st_mtim.tv_sec}};	  
				utimes(fadr, t);		

				close (source);
				close (filefd);
			}
		}
		free (fadr);
	}
	return 0;
}