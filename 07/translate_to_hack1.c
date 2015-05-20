#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAXLINE 1000


void clean(char *line);


int main(int argc, char *argv[])
{
	FILE *from_file;
	FILE *to_file;

	// from_file = fopen(argv[1], "r");
	from_file = fopen("SimpleAdd.vm", "r");
	to_file = fopen("my_asm.asm", "w");
	if (from_file == 0)
	{
		printf("Cannot open SimpleAdd.vm");
		exit(1);
	}
	if (to_file == 0)
	{
		printf("Cannot open my_asm.asm\n");
		exit(1);
	}

	while (fgets(line, MAXLINE, from_file) != NULL) 
	{
		clean(line);

		if (strlen(line) > 0)
		{
			printf("%s\n", line);
	}
	fclose(from_file);
}


void clean(char *line)
{
	char *cursor = line;
	while (*cursor != 0)
	{
		if (!isspace(*cursor))
		{
			if ((*cursor == '/') && (*(cursor + 1) == '/')) 
			{
				*line = 0;
				break;
			} 
			*line = *cursor;
			line++;
		}
		cursor++;
	}
	line--;
	while (isspace(*line)) 
	{
		line--;
	}
	*(line+1) = 0;
}