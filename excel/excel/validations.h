#include"version3.h"
#pragma once

void import_the_expressions_from_metadata(struct cell ***excel_sheet,char *argument)
{
	char *metadatafilename = (char*)malloc(sizeof(char) * 1000);
	char a[100];
	strcpy(metadatafilename,argument);
	strcat(metadatafilename, ".txt");
	FILE *fstream;
	fopen_s(&fstream, metadatafilename, "r");
	for (int row = 1; row <= 10; row++)
	{
		for (int col = 1; col <= 10; col++)
		{
			fgets(a, sizeof(a), fstream);
			if (strcmp(a, "(null)\n") == 0)
				excel_sheet[row][col]->expression = NULL;
			else
			{
				excel_sheet[row][col]->expression = (char*)malloc(sizeof(char) * 1000);
				int index = 0;
				int exp_index = 0;
				while (a[index] != '\n')
					excel_sheet[row][col]->expression[exp_index++] = a[index++];
				excel_sheet[row][col]->expression[exp_index] = '\0';
			}
			excel_sheet[row][col]->start = NULL;
			struct node *temp_present = NULL;
			while (fgets(a, sizeof(a), fstream))
			{
				if (a[0] == '\n')
					break;
				int rownum = 0;
				int index = 0;
				while (a[index] != '\n')
					rownum = rownum * 10 + (a[index++] - 48);
				fgets(a, sizeof(a), fstream);
				int colnum = 0;
				index = 0;
				while (a[index] != '\n')
					colnum = colnum * 10 + (a[index++] - 48);
				struct node *new_node = (struct node*)malloc(sizeof(struct node));
				new_node->row = rownum;
				new_node->col = colnum;
				new_node->next=NULL;
				if (excel_sheet[row][col]->start == NULL)
					excel_sheet[row][col]->start = new_node;
				else
					temp_present->next = new_node;
				temp_present = new_node;
			}
		}
	}
	fclose(fstream);
}

void update_the_value(struct cell ***excel_sheet, char *expression, int col, int row)
{
	int len = strlen(expression);
	char *infix_expression = (char*)malloc(sizeof(char)*(len * 10));
	int infix_index = 0;
	int index = 0;
	int value, reverse_value = 1;
	int temp_col, temp_row = 0;
	while (index < len)
	{
		temp_row = 0;
		if (expression[index] >= 97 && expression[index] <= 122)
		{
			temp_col = expression[index++] - 97 + 1;
			while (expression[index] >= 48 && expression[index] <= 57)
				temp_row = temp_row * 10 + (expression[index++] - 48);
			value = excel_sheet[temp_row][temp_col]->data;
			if (!value)
				infix_expression[infix_index++] = '0';
			while (value)
			{
				reverse_value = reverse_value * 10 + value % 10;
				value = value / 10;
			}
			while (reverse_value / 10)
			{
				infix_expression[infix_index++] = reverse_value % 10 + 48;
				reverse_value /= 10;
			}
			continue;
		}
		infix_expression[infix_index++] = expression[index++];
	}
	infix_expression[infix_index] = '\0';
	char *postfix_expression = (char*)malloc(sizeof(char) * infix_index);
	convert_expression_to_postfix(infix_expression, postfix_expression, strlen(infix_expression));
	value = evaluation_of_postfix_expression(postfix_expression);
	excel_sheet[row][col]->data = value;
}

void update_all_affected_cells(struct cell ***excel_sheet, int col, int row)
{
	struct node *temp = excel_sheet[row][col]->start;
	while (temp)
	{
		update_the_value(excel_sheet, excel_sheet[temp->row][temp->col]->expression, temp->col, temp->row);
		update_all_affected_cells(excel_sheet, temp->col, temp->row);
		temp = temp->next;
	}
}

void evaluate_the_expression_with_operands(struct cell ***excel_sheet,char *expression,int col,int row,int len)
{
	char *infix_expression = (char*)malloc(sizeof(char)*(len * 10));
	int infix_index = 0;
	int index = 0;
	int value,reverse_value=1;
	int temp_col,temp_row=0;
	while (index < len)
	{
		temp_row = 0;
		if (expression[index] >= 97 && expression[index] <= 122)
		{
			temp_col = expression[index++] - 97 + 1;
			while (expression[index] >= 48 && expression[index] <= 57)
				temp_row = temp_row * 10 + (expression[index++] - 48);
			value = excel_sheet[temp_row][temp_col]->data;
			if (!value)
				infix_expression[infix_index++] = '0';
			while (value)
			{
				reverse_value = reverse_value * 10 + value % 10;
				value = value / 10;
			}
			while (reverse_value/10)
			{
				infix_expression[infix_index++] = reverse_value % 10 + 48;
				reverse_value /= 10;
			}
			continue;
		}
		infix_expression[infix_index++] = expression[index++];
	}
	infix_expression[infix_index] = '\0';
	char *postfix_expression = (char*)malloc(sizeof(char) * infix_index);
	if (convert_expression_to_postfix(infix_expression, postfix_expression,strlen(infix_expression)))
	{
		if (detect_the_cycle(excel_sheet,expression,col,row))
			printf("THIS EXPRESSION LEADS TO CYCLE . SRY CAN,T TO SET\n");
		else
		{
			value = evaluation_of_postfix_expression(postfix_expression);
			excel_sheet[row][col]->data = value;
			update_all_affected_cells(excel_sheet, col, row);
		}
	}
	else
		printf("INVALID EXPRESSION\n");
	return;
}

void evaluate_the_expression_without_operands(struct cell ***excel_sheet, char *expression, int col, int row,int len)
{
	int value;
	char *postfix_expression = (char*)malloc(sizeof(char) * len);
	if (convert_expression_to_postfix(expression, postfix_expression, len))
	{
		value = evaluation_of_postfix_expression(postfix_expression);
			excel_sheet[row][col]->data = value;
		update_all_affected_cells(excel_sheet, col, row);
	}
	else
		printf("INVALID EXPRESSION\n");
	return;
}