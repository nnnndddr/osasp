#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<errno.h>
#include<limits.h>
#include<libgen.h>

char *AppName;

int ProcessFolder( FILE *outfile, char *curPath){
	DIR *curDir;
	curDir = opendir(curPath);
	if(curDir == NULL) {
		fprintf(stderr,"%s: %s: %s\n", AppName, curPath, strerror(errno));
		errno = 0;
		return -1;
	}
	struct dirent *dent;
	struct stat buf;
	char *file = alloca(strlen(curPath) + NAME_MAX + 2);
	if(file==NULL){
		fprintf(stderr,"%s: %s.", AppName, strerror(errno));
		return -1;
	}
	long int sum = 0;
	int count = 0;
	int maxSize = -1;
	char *maxFile = alloca(NAME_MAX);
	if(maxFile==NULL){
		fprintf(stderr,"%s: %s.", AppName, strerror(errno));
		return -1;
	}
	maxFile[0] = 0;
	errno = 0;
	while(dent = readdir(curDir)){
		if(strcmp(".", dent->d_name) && strcmp("..", dent->d_name)){
			strcpy(file, curPath);
			strcat(file, "/");
 			strcat(file, dent->d_name);
			if (lstat(file,&buf) == -1) {
				fprintf(stderr,"%s: %s: %s\n", AppName, curPath, strerror(errno));
				return -1;
			}
			if(!S_ISDIR(buf.st_mode)){
				if(buf.st_size > maxSize){
					maxSize = buf.st_size;
					strcpy(maxFile, basename(file));
				}
				sum+=buf.st_size;
				count++;
			}
			if(S_ISDIR(buf.st_mode)){
				ProcessFolder(outfile, file);
			}
		}
	}

	if(errno!=0){
		fprintf(stderr,"%s: %s: %s\n", AppName, curPath, strerror(errno));
		return -1;
	}

	printf("%s %d %ld %s\n", curPath, count, sum, maxFile);
	fprintf(outfile,"%s %d %ld %s\n", curPath, count, sum, maxFile);
	if(closedir(curDir)==-1){
		fprintf(stderr,"%s: %s: %s\n", AppName, curPath, strerror(errno));
		return -1;
	}
	return 0;
}

int main(int argc, char **argv){
	AppName = alloca(strlen(basename(argv[0])));
	strcpy(AppName, basename(argv[0]));
	if(argc<3){
		fprintf(stderr, "%s: Invalid number of parametrs\n1 parametr - First file\n2 parametr - Second File 2\n.\n", AppName);
		return -1;
	}
	FILE *file;
	if((file = fopen(argv[2],"w")) == NULL){
		fprintf(stderr,"%s: %s: %s\n", AppName, argv[2], strerror(errno));
		return -1;
	}

	if(realpath(argv[1], NULL) == NULL) {
		fprintf(stderr,"%s: %s: %s\n", AppName, argv[1], strerror(errno));
		return -1;
	}

	ProcessFolder(file, realpath(argv[1], NULL));

	if(fclose(file)!=0){
		fprintf(stderr,"%s: %s: %s\n", AppName, argv[2], strerror(errno));
		return -1;
	}
	return 0;
}