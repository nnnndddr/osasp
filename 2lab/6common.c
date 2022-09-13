#include <stdio.h>
#include <dirent.h>

int printdir(const char *pathname);

int main(int argc,char* argv[])
{
	puts("Current directory:");
	printdir("./");
	printf("\n\n");
	puts("Root directory:");
	printdir("/");
}

int printdir(const char *pathname)
{
	DIR *dir;
	if (!(dir = opendir(pathname)))
	{
		fputs("Can't open directory",stderr);
		return -1;
	} 
	
	struct dirent *dr;
	int i = 1;
    	while(dr = readdir(dir)){
        	printf("%3d)%s\n", i++, dr->d_name);
    	}
    	
    	 if(closedir(dir))
    	 {
    	 	fputs("Can't close directory",stderr);
    	 	return -1;
    	 }
    	 return 0;
}