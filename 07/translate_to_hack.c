#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAXLINE 1000


void clean(char *line);
int StartsWith(char *a, char *b);
void print_from_segment(char num[MAXLINE], char *segment); 
void print_to_segment(char num[MAXLINE], char *segment);


int main(int argc, char *argv[])
{
	FILE *from_file;
	FILE *to_file;
	char line[MAXLINE];
	char pushed_const[MAXLINE];
	char local_num[MAXLINE];
	char arg_num[MAXLINE];
	char this_num[MAXLINE];
	char that_num[MAXLINE];
	char temp_num[MAXLINE];
	char static_num[MAXLINE];
	char local_vars_num[MAXLINE];
	char label_name[MAXLINE];
	char func_name[MAXLINE];
	int dif_label[3] = {0};

	// from_file = fopen(argv[1], "r");
	from_file = fopen("SimpleFunction.vm", "r");
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

		if (strlen(line) > 0) {
			if (StartsWith(line, "push constant")) 
			{
				sscanf(line, "push constant%s\n", pushed_const);
				printf("@%s\n"
					"D=A\n"
					"@SP\n"
					"A=M\n"
					"M=D\n"
					"@SP\n"
					"M=M+1\n",
					pushed_const);
			} 
			else if (StartsWith(line, "push local")) 
			{
				sscanf(line, "push local%s\n", local_num);
				print_from_segment(local_num, "LCL");
			}
			else if (StartsWith(line, "push argument")) 
			{
				sscanf(line, "push argument%s\n", arg_num);
				print_from_segment(arg_num, "ARG");
			}
			else if (StartsWith(line, "push this")) 
			{
				sscanf(line, "push this%s\n", this_num);
				print_from_segment(this_num, "THIS");
			}
			else if (StartsWith(line, "push that")) 
			{
				sscanf(line, "push that%s\n", that_num);
				print_from_segment(that_num, "THAT");
			}
			else if (StartsWith(line, "push temp")) 
			{
				sscanf(line, "push temp%s\n", temp_num);
				printf("@%s\n"
					"D=A\n"
					"@R14\n"
					"M=D\n"
					"@R5\n"
					"D=A\n"
					"@R14\n"
					"M=M+D\n"
					"A=M\n"
					"D=M\n"
					"@SP\n"
					"A=M\n"
					"M=D\n"
					"@SP\n"
					"M=M+1\n",
					temp_num); 
			}
			else if (StartsWith(line, "push static")) 
			{
				sscanf(line, "push static%s\n", static_num);
				printf("@%s\n"
					"D=A\n"
					"@16\n"
					"D=A+D\n"
					"@R14\n"
					"M=D\n"
					"A=M\n"
					"D=M\n"
					"@SP\n"
					"A=M\n"
					"M=D\n"
					"@SP\n"
					"M=M+1\n",
					static_num); 
			}
			else if (StartsWith(line, "push pointer")) 
			{
				sscanf(line, "push pointer%s\n", this_num);
				if (*this_num == '0')
					printf("@THIS\n"
					"D=M\n"
					"@SP\n"
					"A=M\n"
					"M=D\n"
					"@SP\n"
					"M=M+1\n"); 
				else if (*this_num == '1')
					printf("@THAT\n"
					"D=M\n"
					"@SP\n"
					"A=M\n"
					"M=D\n"
					"@SP\n"
					"M=M+1\n");  
			} 
			else if (StartsWith(line, "pop local")) 
			{
				sscanf(line, "pop local%s\n", local_num);
				print_to_segment(local_num, "LCL");
			}
			else if (StartsWith(line, "pop argument")) 
			{
				sscanf(line, "pop argument%s\n", arg_num);
				print_to_segment(arg_num, "ARG");
			} 
			else if (StartsWith(line, "pop this")) 
			{
				sscanf(line, "pop this%s\n", this_num);
				print_to_segment(this_num, "THIS");
			} 
			else if (StartsWith(line, "pop that")) 
			{
				sscanf(line, "pop that%s\n", that_num);
				print_to_segment(that_num, "THAT");
			} 
			else if (StartsWith(line, "pop temp")) 
			{
				sscanf(line, "pop temp%s\n", temp_num);
				printf("@%s\n"
					"D=A\n"
					"@R14\n"
					"M=D\n"
					"@R5\n"
					"D=A\n"
					"@R14\n"
					"M=M+D\n"
					"@SP\n"
					"AM=M-1\n"
					"D=M\n"
					"@R14\n"
					"A=M\n"
					"M=D\n",
					temp_num); 
			}
			else if (StartsWith(line, "pop pointer")) 
			{
				sscanf(line, "pop pointer%s\n", this_num);
				if (*this_num == '0')
					printf("@SP\n"
					"AM=M-1\n"
					"D=M\n"
					"@THIS\n"
					"M=D\n"); 
				else if (*this_num == '1')
					printf("@SP\n"
					"AM=M-1\n"
					"D=M\n"
					"@THAT\n"
					"M=D\n"); 
			} 
			else if (StartsWith(line, "popstatic"))
			{
				sscanf(line, "pop static%s\n", static_num);
				printf("@%s\n"
					"D=A\n"
					"@16\n"
					"D=A+D\n"
					"@R14\n"
					"M=D\n"
					"@SP\n"
					"AM=M-1\n"
					"D=M\n"
					"@R14\n"
					"A=M\n"
					"M=D\n",
					static_num);
			}
			else if (strncmp(line, "add", strlen(line)) == 0)
				printf("@SP\n"
					"AM=M-1\n"
					"D=M\n"
					"@SP\n"
					"A=M-1\n"
					"M=M+D\n");
			else if (strncmp(line, "sub", strlen(line)) == 0)
				printf("@SP\n"
					"AM=M-1\n"
					"D=M\n"
					"@SP\n"
					"A=M-1\n"
					"M=M-D\n");
			else if (strncmp(line, "neg", strlen(line)) == 0)
				printf("@SP\n"
					"AM=M-1\n"
					"D=M\n"
					"M=-M\n"
					"@SP\n"
					"M=M+1\n");
			else if (strncmp(line, "eq", strlen(line)) == 0) 
				{
					printf("@SP\n"
						"AM=M-1\n"
						"D=M\n"
						"@SP\n"
						"A=M-1\n"
						"D=M-D\n"
						"@EQR%d\n"
						"D;JEQ\n"
						"@SP\n"
						"A=M-1\n"
						"M=0\n"
						"@ENDEQ%d\n"
						"0;JMP\n"
						"(EQR%d)\n"
						"@SP\n"
						"A=M-1\n"
						"M=-1\n"
						"(ENDEQ%d)\n",
						dif_label[0], dif_label[0], dif_label[0], dif_label[0]);
					dif_label[0]++;
				}
			else if (strncmp(line, "gt", strlen(line)) == 0) 
				{
					printf("@SP\n"
						"AM=M-1\n"
						"D=M\n"
						"@SP\n"
						"A=M-1\n"
						"D=M-D\n"
						"@JGR%d\n"
						"D;JGT\n"
						"@SP\n"
						"A=M-1\n"
						"M=0\n"
						"@ENDJG%d\n"
						"0;JMP\n"
						"(JGR%d)\n"
						"@SP\n"
						"A=M-1\n"
						"M=-1\n"
						"(ENDJG%d)\n",
						dif_label[1], dif_label[1], dif_label[1], dif_label[1]);
					dif_label[1]++;
				}
			else if (strncmp(line, "lt", strlen(line)) == 0)
				{
					printf("@SP\n"
						"AM=M-1\n"
						"D=M\n"
						"@SP\n"
						"A=M-1\n"
						"D=M-D\n"
						"@JLR%d\n"
						"D;JLT\n"
						"@SP\n"
						"A=M-1\n"
						"M=0\n"
						"@ENDJL%d\n"
						"0;JMP\n"
						"(JLR%d)\n"
						"@SP\n"
						"A=M-1\n"
						"M=-1\n"
						"(ENDJL%d)\n",
						dif_label[2], dif_label[2], dif_label[2], dif_label[2]);
					dif_label[2]++;
				}
			else if (strncmp(line, "and", strlen(line)) == 0)
				printf("@SP\n"
					"AM=M-1\n"
					"D=M\n"
					"@SP\n"
					"A=M-1\n"
					"M=M&D\n");
			else if (strncmp(line, "or", strlen(line)) == 0)
				printf("@SP\n"
					"AM=M-1\n"
					"D=M\n"
					"@SP\n"
					"A=M-1\n"
					"M=M|D\n");
			else if (strncmp(line, "not", strlen(line)) == 0) {
				printf("@SP\n"
					"AM=M-1\n"
					"D=M\n"
					"M=!M\n"
					"@SP\n"
					"M=M+1\n");
			}
			else if (StartsWith(line, "label")) 
			{
				sscanf(line, "label %s\n", label_name);
				printf("(%s)\n", label_name);
			}
			else if (StartsWith(line, "if-goto")) 
			{
				sscanf(line, "if-goto %s\n", label_name);
				printf(
					"@SP\n"
					"M=M-1\n"
					"@%s\n"
					"D;JGT\n", 
					label_name);
			} 
			else if (StartsWith(line, "goto")) 
			{
				sscanf(line, "goto %s\n", label_name);
				printf(
					"@SP\n"
					"M=M-1\n"
					"@%s\n"
					"0;JMP\n", 
					label_name);
			}
			else if (StartsWith(line, "function")) 
			{
				sscanf(line, "function %[a-zA-Z0-9:_.] %s\n", func_name, local_vars_num);
				printf(
					"@%s\n"
					"D=A\n"
					"@SP\n"
					"M=M+D\n", 
					local_vars_num);
			}
			else if (strncmp(line, "return", strlen(line)) == 0) 
			{
				printf(
					"@LCL\n"
					"D=M\n"
					"@R13\n"
					"M=D\n"

					"@LCL\n"
					"D=M\n"
					"@R14\n"
					"M=D\n"
					"@5\n"
					"D=A\n"
					"@R14\n"
					"M=M-D\n"
					"A=M\n"
					"D=M\n"
					"@R14\n"
					"M=D\n"

					"@SP\n"
					"A=M-1\n"
					"D=M\n"
					"@ARG\n"
					"A=M\n"
					"M=D\n"
					"D=A\n"
					"@SP\n"
					"M=D+1\n"

					"@R13\n"
					"D=M\n"
					"@R15\n"
					"M=D\n"
					"@4\n"
					"D=A\n"
					"@R15\n"
					"M=M-D\n"
					"A=M\n"
					"D=M\n"
					"@LCL\n"
					"M=D\n"

					"@R13\n"
					"D=M\n"
					"@R15\n"
					"M=D\n"
					"@2\n"
					"D=A\n"
					"@R15\n"
					"M=M-D\n"
					"A=M\n"
					"D=M\n"
					"@THIS\n"
					"M=D\n"

					"@R13\n"
					"D=M\n"
					"@R15\n"
					"M=D\n"
					"@1\n"
					"D=A\n"
					"@R15\n"
					"M=M-D\n"
					"A=M\n"
					"D=M\n"
					"@THAT\n"
					"M=D\n"

					"@R13\n"
					"D=M\n"
					"@R15\n"
					"M=D\n"
					"@3\n"
					"D=A\n"
					"@R15\n"
					"M=M-D\n"
					"A=M\n"
					"D=M\n"
					"@ARG\n"
					"M=D\n");
			}
		}
	}
	fclose(from_file);
}


void clean(char *line)
{
	char *cursor = line;
	// char *d = line;
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
		}
		line++;
		cursor++;
	}
	line--;
	while (isspace(*line))
		line--;
	*(line+1) = 0;
	// printf("%s\n", d);
}


int StartsWith(char *a, char *b)
{
   if(strncmp(a, b, strlen(b)) == 0) return 1;
   return 0;
}

void print_from_segment(char *num, char *segment) 
{
	printf("@%s\n"
		"D=A\n"
		"@R14\n"
		"M=D\n"
		"@%s\n"
		"D=M\n"
		"@R14\n"
		"M=M+D\n"
		"A=M\n"
		"D=M\n"
		"@SP\n"
		"A=M\n"
		"M=D\n"
		"@SP\n"
		"M=M+1\n",
		num, segment); 
}


void print_to_segment(char *num, char *segment) 
{
	printf("@%s\n"
		"D=A\n"
		"@R14\n"
		"M=D\n"
		"@%s\n"
		"D=M\n"
		"@R14\n"
		"M=M+D\n"
		"@SP\n"
		"AM=M-1\n"
		"D=M\n"
		"@R14\n"
		"A=M\n"
		"M=D\n",
		num, segment); 
}