#include <stdio.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{

    char exit_point = 'q';
    if (argc != 2)
    {
        perror("Enter only the file path");
        return 0;
    }
    FILE* file;
    char inputChar;
    int f1;
    if (f1 = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0644))
    {
        file = fdopen(f1, "w");
         while ((inputChar = getchar()) != exit_point) 
         {
             if(putc(inputChar, file) == EOF)
             {
                    perror("Error while writing in file.");
             }
         }
    }
    else 
    perror("Error while opening file");

    if (fclose(file) != 0)
		perror("Could not close file\n");

    return 0;
}