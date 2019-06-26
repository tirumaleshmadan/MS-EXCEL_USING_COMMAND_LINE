#include"validations.h"
#pragma once

//______________________________________________________________________________________//


int check_case_insensitive(char *input_command, char *original_command, int len)
{
	int lenght_of_input_command = strlen(input_command);
	int flag = 0;
	if (lenght_of_input_command == len)
	{
		for (int i = 0; i < len; i++)
		{
			if (input_command[i] >= 65 && input_command[i] <= 90)
			{
				if (input_command[i] + 32 != original_command[i])
				{
					flag = 1;
					break;
				}
			}
			else if (input_command[i] >= 97 && input_command[i] <= 122)
			{
				if (input_command[i] != original_command[i])
				{
					flag = 1;
					break;
				}
			}
			else
			{
				flag = 1;
				break;
			}
		}
		if (flag == 0)
			return 1;
	}
	return 0;
}


//______________________________________________________________________________________//



void get_the_csv_data(struct cell ***temp_excel,char *argument)
{
	if (argument[0] >= 65 && argument[0] <= 90)
		argument[0] = argument[0] + 32;
	int col = argument[0] - 97 + 1;
	int row = 0;
	for (int index = 1; argument[index] != '\0'; index++)
		row = row * 10 + (argument[index]-48);
	if (col >= 1 && col <= 10 && row >= 1 && row <= 10)
		printf("%d\n", temp_excel[row][col]->data);
	else
		printf("INVALID ARGUMENTS\n");
	return;
}


//______________________________________________________________________________________//



void set_the_csv_data(struct cell ***temp_excel, char *argument)
{
	int index = 0;
	char *expression = (char*)malloc(sizeof(char) * 10000);
	int lenght = strlen(argument);
	int expression_index = 0;
	int temp_col;
	int temp_row;
	int operands_flag = 0;
	int space_flag = 0;
	int row = 0;
	int col = 0;
	if (index<lenght && argument[index] >= 65 && argument[index] <= 90)
		col = argument[index] - 65 + 1;
	else if (index<lenght && argument[index] >= 97 && argument[index] <= 122)
		col = argument[index] -97 + 1;
	if (col > 10 || col < 1)
	{
		printf("INVALID ARGUMENTS\n");
		return;
	}
	index++;
	while (index<lenght && argument[index] >= 48 && argument[index] <= 57)
	{
			row = row * 10 + (argument[index++] - 48);
	}
	if (row > 10 || row < 1)
	{
		printf("INVALID ARGUMENTS\n");
		return;
	}
	while (index<lenght && (argument[index] == 32 || argument[index] == 9))
		index++;
	if ((index<lenght && argument[index] != '=' ) || argument[index]=='\0')
	{
		printf("INVALID ARGUMENTS\n");
		return;
	}
	index++;
	while (argument[index] != '\0')
	{
		while (index<lenght && (argument[index] == 32 || argument[index] == 9))
			index++;
		if (index < lenght && (argument[index] == '{' || argument[index] == '(' || argument[index] == '[' || argument[index] == ']' || argument[index] == '}' || argument[index] == ')'))
		{
			expression[expression_index++] = argument[index++];
			continue;
		}
		else if (index<lenght && ((argument[index] >= 65 && argument[index] <= 90) || (argument[index] >= 97 && argument[index] <= 122)))
		{
			if (space_flag == 1)
			{
				printf("INVALID EXPRESSION\n");
				return;
			}
			temp_col = 0;
			if ((argument[index] >= 65 && argument[index] <= 90))
			{
				expression[expression_index++] = argument[index++] + 32;
				temp_col = expression[expression_index - 1] - 65 + 1;
			}
			else
			{
				expression[expression_index++] = argument[index++];
				temp_col = expression[expression_index - 1] - 97 + 1;
			}
			if (temp_col > 10 || temp_col < 1)
			{
				printf("INVALID EXPRESSION\n");
				return;
			}
			temp_row = 0;
			while (index<lenght && argument[index] >= 48 && argument[index] <= 57)
			{
				expression[expression_index++] = argument[index];
				temp_row = temp_row * 10 + (argument[index++] - 48);
			}
			if (temp_row > 10 || temp_row < 1)
			{
				printf("INVALID EXPRESSION\n");
				return;
			}
			if (index < lenght && ((argument[index] >= 65 && argument[index] <= 90) || (argument[index] >= 97 && argument[index] <= 122)))
			{
				printf("INVALID EXPRESSION\n");
				return;
			}
			operands_flag = 1;
			space_flag = 1;
			continue;
		}
		else if (index<lenght && argument[index] >= 48 && argument[index] <= 57)
		{
			if (space_flag == 1)
			{
				printf("INVALID EXPRESSION\n");
				return;
			}
			while (index<lenght && argument[index] >= 48 && argument[index] <= 57)
			{
				expression[expression_index++] = argument[index++];
			}
			if (index < lenght && ((argument[index] >= 65 && argument[index] <= 90) || (argument[index] >= 97 && argument[index] <= 122)))
			{
				printf("INVALID EXPRESSION\n");
				return;
			}
			space_flag = 1;
			continue;
		}
		else if (index < lenght && (argument[index] == '-' || argument[index] == '+' || argument[index] == '*' || argument[index] == '/'))
		{
			if (space_flag==0)
			{
				printf("INVALID EXPRESSION\n");
				return;
			}
			expression[expression_index++] = argument[index++];
			space_flag = 0;
			continue;
		}
		printf("INVALID EXPRESSION\n");
		return;
	}
	if (!space_flag)
	{
		printf("INVALID EXPRESSION\n");
		return;
	}
	expression[expression_index] = '\0';
	if (operands_flag)
		evaluate_the_expression_with_operands(temp_excel,expression,col,row,expression_index);
	else
		evaluate_the_expression_without_operands(temp_excel,expression,col,row,expression_index);
	return;
}


//______________________________________________________________________________________//



void print_the_csv_data(struct cell ***temp_excel)
{
	printf("%6c",32);
	for (int col = 0; col < 10; col++)
		printf("%6c ", col + 65);
	printf("\n");
	for (int row = 1; row <= 10; row++)
	{
		printf("%6d ", row);
		for (int col = 1; col <= 10; col++)
			printf("%6d ", temp_excel[row][col]->data);
		printf("\n");
	}
	return;
}


//______________________________________________________________________________________//



char* export_the_csv_file(struct cell ***temp_excel,char *argument)
{
	char *updatedfilename = (char*)malloc(sizeof(char) * 1000);
	char *metadatafilename = (char*)malloc(sizeof(char) * 1000);
	int length = strlen(argument);
	if (!length)
	{
		printf("INVALID COMMANlD FORMAT\n");
		return NULL;
	}
	char *extend = ".csv";
	if (!(argument[length - 1] == extend[3] && argument[length - 2] == extend[2] && argument[length - 3] == extend[1] && argument[length - 4] == extend[0]))
		strcat(argument, extend);
	FILE *fstream=NULL;
	fopen_s(&fstream,argument, "w");
	for (int row = 1; row <= 10; row++)
	{
		for (int col = 1; col <= 10; col++)
		{
			if (col==10)
				fprintf(fstream, "%d\n", temp_excel[row][col]->data);
			else
				fprintf(fstream, "%d,", temp_excel[row][col]->data);
		}
	}
	fclose(fstream);
	strcpy(metadatafilename, argument);
	strcat(metadatafilename, ".txt");
	FILE *ftextstream = NULL;
	fopen_s(&ftextstream, metadatafilename, "w");
	for (int row = 1; row <= 10; row++)
	{
		for (int col = 1; col <= 10; col++)
		{
			fprintf(ftextstream, "%s\n", temp_excel[row][col]->expression);
			struct node *temp = temp_excel[row][col]->start;
			while (temp)
			{
				fprintf(ftextstream, "%d\n", temp->row);
				fprintf(ftextstream, "%d\n", temp->col);
				temp = temp->next;
			}
			fprintf(ftextstream, "\n");
		}
	}
	fclose(ftextstream);
	strcpy(updatedfilename,argument);
	return updatedfilename;
}


//______________________________________________________________________________________//



char* import_the_csv_file(struct cell ***temp_excel,char *argument)
{
	char *updatedfilename = (char*)malloc(sizeof(char) * 1000);
	int length = strlen(argument);
	if (!length)
	{
		printf("INVALID COMMAND FORMAT\n");
		return NULL;
	}
	char *extend = ".csv";
	if (!(argument[length - 1] == extend[3] && argument[length - 2] == extend[2] && argument[length - 3] == extend[1] && argument[length - 4] == extend[0]))
		strcat(argument, extend);
	FILE *fstream=NULL;
	fopen_s(&fstream, argument, "r");
	if (fstream == NULL)
	{
		printf("FILE NOT FOUND\n");
		return NULL;
	}
	for (int row = 1; row <= 10; row++)
	{
		for (int col = 1; col <= 10; col++)
		{
				fscanf_s(fstream, "%d,", &temp_excel[row][col]->data);
		}
	}
	fclose(fstream);
	import_the_expressions_from_metadata(temp_excel,argument);
	strcpy(updatedfilename, argument);
	return updatedfilename;
}


//______________________________________________________________________________________//



void save_the_csv_data(struct cell ***temp_excel,char *filename)
{
	char *metadatafilename = (char*)malloc(sizeof(char) * 1000);
	FILE *fstream = NULL;
	fopen_s(&fstream, filename, "w");
	for (int row = 1; row <= 10; row++)
	{
		for (int col = 1; col <= 10; col++)
		{
			if (col == 10)
				fprintf(fstream, "%d\n", temp_excel[row][col]->data);
			else
				fprintf(fstream, "%d,", temp_excel[row][col]->data);
		}
	}
	fclose(fstream);
	strcpy(metadatafilename, filename);
	strcat(metadatafilename, ".txt");
	FILE *ftextstream;
	fopen_s(&ftextstream, metadatafilename, "w");
	for (int row = 1; row <= 10; row++)
	{
		for (int col = 1; col <= 10; col++)
		{
			fprintf(ftextstream, "%s\n", temp_excel[row][col]->expression);
			struct node *temp = temp_excel[row][col]->start;
			while (temp)
			{
				fprintf(ftextstream, "%d\n", temp->row);
				fprintf(ftextstream, "%d\n", temp->col);
				temp = temp->next;
			}
			fprintf(ftextstream, "\n");
		}
	}
	fclose(ftextstream);
}


//______________________________________________________________________________________//



int check_no_of_strings(char *argument)
{
	for (int index = 0; argument[index] != '\0'; index++)
		if (argument[index] == 32)
			return 0;
	return 1;
}


int check_for_header(char *data)
{
	char *header = (char*)malloc(sizeof(char)* 20);
	strcpy(header, "header=true");
	int index = strlen(data)-1;
	int len = 10;
	while (index >= 0 && len >= 0 && header[len--] == data[index--]);
	if (len < 0)
	{
		while (index >= 0 && data[index] == 32 || data[index] == 9)
			index--;
		if (index < 0)
			return 0;
		else
		{
			data[index + 1] = '\0';
			if (check_no_of_strings(data) == 1)
				return 1;
		}
	}
	return 0;
}

char* import_the_csv_file_with_header(struct cell ***temp_excel,char *argument)
{
	char *updatedfilename = (char*)malloc(sizeof(char) * 1000);
	int length = strlen(argument);
	if (!length)
	{
		printf("INVALID COMMAND FORMAT\n");
		return NULL;
	}
	char *extend = ".csv";
	if (!(argument[length - 1] == extend[3] && argument[length - 2] == extend[2] && argument[length - 3] == extend[1] && argument[length - 4] == extend[0]))
		strcat(argument, extend);
	FILE *fstream = NULL;
	fopen_s(&fstream, argument, "r");
	if (fstream == NULL)
	{
		printf("FILE NOT FOUND\n");
		return NULL;
	}
	for (int row = 1; row <= 10; row++)
		for (int col = 1; col <= 10; col++)
			fscanf_s(fstream, "%d,", &temp_excel[row][col]->data);
	fclose(fstream);
}
