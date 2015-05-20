#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAXLINE 1000

struct table_entry_struct {
	char *string;
	int address;
	int string_len;
};
void clean(char *line);
void label_command(char *line, int *num_struct_elms, struct table_entry_struct *table_entry, int *variable_address, char **string_of_lines);
void a_command(char *line, FILE *file);
void c_command_dest(char *line, char *c_binary_line);
void c_command_jump(char *line, char *c_binary_line);
void c_command_comp(char *line, char *c_binary_line);
void from_decimal_to_binary(int decimal, char *line);


int main(int argc, char *argv[])
{
	int i;
	int num_struct_elms;
	char line[MAXLINE];
	int first_running = 1;
	int second_running = 0;		
	struct table_entry_struct *table_entry;
	table_entry = (struct table_entry_struct *)malloc(1000*sizeof(struct table_entry_struct));
	char *string_of_lines;
	string_of_lines = (char*)malloc(20000*sizeof(char));
	int variable_address = 16;
	FILE *from_file;
	FILE *to_file;

	from_file = fopen(argv[1], "r");
	to_file = fopen("my_hack.hack", "w");
	if (from_file == 0)
	{
		printf("Cannot open %s\n", argv[1]);
		exit(1);
	}
	if (to_file == 0)
	{
		printf("Cannot open my_hack.hack\n");
		exit(1);
	}

	// FIRST TIME RUNNING
	if (first_running) 
	{
		int LABELS_ADDRESS[] = { 1, 2, 3, 4, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16384, 24576, 0};
		char *LABELS[] = {"LCL", "ARG", "THIS", "THAT", "R0", "R1",
					"R2", "R3", "R4", "R5", "R6", "R7", 
					"R8", "R9", "R10", "R11", "R12", "R13",
					"R14", "R15", "SCREEN", "KBD", "SP"};
		int count = 0;
		int h = 0;
		int string_len;
		char *suline;
		int fd = 0;

		while (fgets(line, MAXLINE, from_file) != NULL && fd == 0) 
		{
			clean(line);	
			if (*line == '(') {
				sscanf(line, "(%[^)]", suline);
				if (isalpha(suline[0])) 
				{
					int pos_h = h;
					int string_len = strlen(suline);
					while (h-- > 0) {
						if (strncmp(table_entry[h].string, suline, string_len) == 0) 
							fd = 1;	
					}
					h = pos_h;
					if (fd == 0) 
					{
						string_len = strlen(suline);
						strcat(string_of_lines, suline);
						table_entry[h].string = string_of_lines;	
						string_of_lines = string_of_lines + string_len;
						table_entry[h].string_len = string_len;
						table_entry[h].address = count;
						h++;
					}	
					fd = 0;			}
			} else if (strlen(line) > 0) 
				count++;	
		}
		
		for (int i=0; i<23; i++) 
		{
			string_len = strlen(LABELS[i]);
			strcat(string_of_lines, LABELS[i]);
			table_entry[h].string = string_of_lines;	
			string_of_lines = string_of_lines + string_len;
			table_entry[h].string_len = string_len;
			table_entry[h].address = LABELS_ADDRESS[i];
			h++;
		}
		num_struct_elms = h;
		second_running = 1;
	}

	// SECOND TIME RUNNING
	if (second_running == 0)
		return 0;
	rewind(from_file);
	while (fgets(line, MAXLINE, from_file) != NULL) 
	{
		clean(line);
		char c_binary_line[] = "1110000000000000";

		if (strlen(line) > 0)
		{
			if (*line == '@' && isalpha(line[1])) 
			{
				label_command(line, &num_struct_elms, table_entry, &variable_address, &string_of_lines);
				fprintf(to_file, "%s\n", line);
			}
			else if (*line == '@') 
				a_command(line, to_file);
			else if (strchr(line, '=') != NULL) 
			{
				c_command_dest(line, c_binary_line);
				c_command_comp(line, c_binary_line);
				fprintf(to_file, "%s\n", c_binary_line);
			}
			else if (strchr(line, ';') != NULL) 
			{
				c_command_jump(line, c_binary_line);
				fprintf(to_file, "%s\n", c_binary_line);
			}
		}
	}
	fclose(from_file);
}

void label_command(char *sourceline, int *num_struct_elms, struct table_entry_struct *table_entry, int *variable_address, char **string_of_lines) 
{
	char line[MAXLINE];
	sscanf(sourceline, "@%s", line);
	int h = *num_struct_elms;
	while (h-- > 0) 
	{
		if (strncmp(table_entry[h].string, line, table_entry[h].string_len) == 0) 
		{
			from_decimal_to_binary(table_entry[h].address, sourceline);
			h = 0;
		}		
	}
	if (*sourceline == '@') 
	{
		from_decimal_to_binary(*variable_address, sourceline);
		int h = *num_struct_elms;
		int string_len = strlen(line);
		strcat(*string_of_lines, line);
		table_entry[h].string = *string_of_lines;	
		*string_of_lines = *string_of_lines + string_len;
		table_entry[h].string_len = string_len;
		table_entry[h].address = *variable_address;
		*num_struct_elms = *num_struct_elms + 1;
		*variable_address = *variable_address + 1;
	}
}

void a_command(char *sourceline, FILE *file)
{
	char line[MAXLINE];
	sscanf(sourceline, "@%s", line);
	int decimalnum = atoi(line);
	from_decimal_to_binary(decimalnum, line);
	fprintf(file, "%s\n", line);
}

void c_command_dest(char *sourceline, char *line)
{	
	int i;
	char dest_line[3];

	int s = strchr(sourceline, '=') - sourceline;
	strncpy(dest_line, sourceline, s);
	if (strchr(dest_line, 'M'))
		line[12] = '1';
	if (strchr(dest_line, 'D'))
		line[11] = '1';
	if (strchr(dest_line, 'A')) 
		line[10] = '1';
}

void c_command_jump(char *sourceline, char *line)
{	
	char *COMPS[] = {"0", "1", "-1", "D", "A", "!D", "!A",
					"-D", "-A", "D+1", "1+D", "A+1", "1+A", 
					"D-1", "A-1", "D+A", "A+D", "D-A", "A-D"
					"D&A", "A&D", "D|A", "A|D"};
	char *COMPS_VALUES[] = {"101010", "111111", "1110101", "001100", "110000", "001101", "110001",
							"001111", "110011", "011111", "011111", "110111", "110111",
							"001110", "110010", "000010", "000010", "010011", "000111",
							"000000", "000000", "010101", "010101"};
	int i;
	char *jump_line;
	char jump_cmp_line[3];

	int d = strchr(sourceline, ';') - sourceline;
	strncpy(jump_cmp_line, sourceline, d);
	for (int i=0; i < 23; i++) 
	{
		if (strcmp(jump_cmp_line, COMPS[i]) == 0) 
		{
			for (int k=0; k < 6; k++) 
			{
				line[4+k] = COMPS_VALUES[i][0+k];
			}
		}
	}

	// 2 = the string do not include ';'' and 'J'
	jump_line = sourceline + d + 2;
	if (strchr(jump_line, 'L')) {
		line[13] = '1';
	}
	if (strchr(jump_line, 'E')) {
		line[14] = '1';
	}
	if (strchr(jump_line, 'G')) {
		line[15] = '1';
	}
	if (strchr(jump_line, 'M')) {
		line[13] = '1';
		line[14] = '1';
		line[15] = '1';
	} 
	if (strchr(jump_line, 'N')) {
		line[13] = '1';
		line[14] = '0';
		line[15] = '1';
	}
}

void c_command_comp(char *sourceline, char *line)
{	
	// 23 
	char *COMPS[] = {"0", "1", "-1", "D", "A", "!D", "!A",
					"-D", "-A", "D+1", "1+D", "A+1", "1+A", 
					"D-1", "A-1", "D+A", "A+D", "D-A", "A-D",
					"D&A", "A&D", "D|A", "A|D"};
	char *COMPS_VALUES[] = {"101010", "111111", "1110101", "001100", "110000", "001101", "110001",
							"001111", "110011", "011111", "011111", "110111", "110111",
							"001110", "110010", "000010", "000010", "010011", "000111",
							"000000", "000000", "010101", "010101"};
	char *comp_line;

	int d = strchr(sourceline, '=') - sourceline;
	comp_line = sourceline + d + 1;

	if (strchr(comp_line, 'M') != NULL) 
	{
		line[3] = '1';
		int s = strchr(comp_line, 'M') - comp_line;
		if (strchr(sourceline, 'M') != NULL) 
		{
			comp_line = comp_line + s;
			*comp_line = 'A';
			comp_line = comp_line - s;
		}
	}

	for (int i=0; i < 23; i++) 
	{
		if (strcmp(comp_line, COMPS[i]) == 0) 
		{
			for (int k=0; k < 6; k++) 
			{
				line[4+k] = COMPS_VALUES[i][0+k];
			}
		}
	}
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

void from_decimal_to_binary(int quotient, char *line)
{
	int i = 0;
	char pre_binary_num[16];
	while (quotient != 0)
	{
		pre_binary_num[i] = quotient % 2 + '0';
		quotient = quotient / 2;
		i++;
	}
	for (int k = 0; k < (16-i); k++) 
	{
		*line = '0';
		line++;
	}
	while (i > 0)
	{
		i--;
		*line = pre_binary_num[i];
		line++;
	}
	*line = 0;
}