#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAXLINE 1000


int main()
{
	int i;
	char dest_line[MAXLINE];
	char sourceline[] = "M=D";
	char ljhgjhgj[] = "mmmmmmmmmmmmmmmmmm";

	int s = strchr(sourceline, '=') - sourceline;
	strncpy(dest_line, sourceline, s);
	if (strchr(dest_line, 'M')) {
		printf("lalalla\n");
	}
	printf("%d", s);
	printf("%s", dest_line);
}