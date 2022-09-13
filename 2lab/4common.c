#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
int main(int argc, char *argv[])
{
	if (argc == 3)
	{	
		char *endptr;
		errno = 0;
	    long N = strtol(argv[2],&endptr,10);
		if ((errno == ERANGE && (N == LONG_MAX || N == LONG_MIN))  || (errno != 0 && N == 0))
               {
               	perror("strtol");
               	exit(EXIT_FAILURE);
           	}
			
		if (endptr == argv[2]) {
               	fprintf(stderr, "No digits were found\n");
               	exit(EXIT_FAILURE);
           	}

		if (N<0)
			fputs("N should be >=0\n",stderr);
		else
		{
			FILE *file;
			if (file = fopen(argv[1],"r"))
			{
				int i =0;
				char c; 
				while (!feof(file))
				{
					if (N == 0)
					{
						c = getc(file);
						if (c != EOF)
							putc(c,stdout);
					}	
					else
					{
						if (N == i)
						{
							char key = getc(stdin);
							while (key != 10) 
								key = getc(stdin);
							i = 0;
						}
						c = getc(file);
						if (c != EOF)
							putc(c,stdout);
						if (c == '\n')
							i++;
					}
				}
				if (fclose(file))
				{
					fputs("Error:File doesn't close",stderr);
					return -1;
				}
				else return 0;
			}
			else
			{
				fputs("Can't open file\n",stderr);
			}
		}
	}
	else
	{
		fputs("Incorrect number of parametrs:\nFirst param - name of file\nSecond param - amount of strings to output by\n",stderr);
	}
	
}