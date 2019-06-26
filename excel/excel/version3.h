#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#pragma once

struct node
{
	int col;
	int row;
	struct node *next;
};

struct cell
{
	int data;
	char *expression;
	struct node *start;
};

int priority(char ope)
{
	if (ope == '*' || ope == '/')
		return 2;
	else if (ope == '+' || ope == '-')
		return 1;
	return 0;
}

int convert_expression_to_postfix(char *expression, char *postfix_expression, int len)
{
	char *stack = (char*)malloc(sizeof(char)*len);
	char *postfix = (char*)malloc(sizeof(char)*(len + 100));
	int top = -1;
	int index_p = 0;
	int flag;
	for (int index = 0; index < len; index++)
	{
		flag = 0;
		while (index < len && expression[index] >= 48 && expression[index] <= 57)
		{
			postfix[index_p++] = expression[index++];
			flag = 1;
		}
		if (flag)
		{
			index--;
			postfix[index_p++] = '.';
			continue;
		}
		if (expression[index] == '(')
			stack[++top] = '(';
		else if (expression[index] == ')')
		{
			while (top >= 0 && stack[top] != '(')
				postfix[index_p++] = stack[top--];
			if (top == -1)
				return 0;
			top--;
		}
		else
		{
			while (top >= 0 && priority(expression[index]) <= priority(stack[top]))
				postfix[index_p++] = stack[top--];
			stack[++top] = expression[index];
		}
	}
	while (top >= 0)
	{
		if (stack[top] == '(')
			return 0;
		postfix[index_p++] = stack[top--];
	}
	postfix[index_p] = '\0';
	strcpy(postfix_expression, postfix);
	return 1;
}

int evaluation_of_postfix_expression(char *postfix)
{
	int *stack = (int*)malloc(sizeof(int) * 1000);
	int top = -1;
	int value;
	int left_operand;
	int right_operand;
	for (int index = 0; index < strlen(postfix); index++)
	{
		if (postfix[index] >= 48 && postfix[index] <= 57)
		{
			value = 0;
			while (postfix[index] != '.')
				value = value * 10 + (postfix[index++] - 48);
			stack[++top] = value;
		}
		else
		{
			right_operand = stack[top--];
			left_operand = stack[top--];
			switch (postfix[index])
			{
			case '+': value = left_operand + right_operand;
				break;
			case '-': value = left_operand - right_operand;
				break;
			case '*': value = left_operand * right_operand;
				break;
			case '/': if (right_operand == 0)
					  {
						  printf("DIVISION EXCEPTION\n");
						  break;
					  }
					   value = left_operand / right_operand;
				break;
			}
			stack[++top] = value;
		}
	}
	return stack[top];
}

int check_cycle_under_recursion(struct cell ***excel_sheet,int **visited,int col,int row)
{
	struct node *temp = excel_sheet[row][col]->start;
	if (visited[row][col])
		return 1;
	visited[row][col] = 1;
	while (temp)
	{
		int flag=check_cycle_under_recursion(excel_sheet,visited, temp->col, temp->row);
		if (flag)
			return 1;
		temp = temp->next;
	}
	return 0;
}

int cycle_detected(struct cell ***excel_sheet,int col,int row)
{
	int **visited=(int**)malloc(sizeof(int*)*11);
	for (int i = 1; i <= 10; i++)
	{
		visited[i] = (int*)malloc(sizeof(int) * 11);
		for (int j = 1; j <= 10; j++)
			visited[i][j] = 0;
	}
	return check_cycle_under_recursion(excel_sheet,visited,col,row);
}

struct cell*** delete_the_affected_node(struct cell ***excel_sheet,int temp_col,int temp_row,int col,int row)
{
	struct node *temp = excel_sheet[temp_row][temp_col]->start;
	struct node *pre = temp;
	if (temp->col == col && temp->row == row)
		excel_sheet[temp_row][temp_col]->start=temp->next;
	else
	{
		while (temp)
		{
			if (temp->col == col && temp->row == row)
			{
				pre->next = temp->next;
				return excel_sheet;
			}
			pre = temp;
			temp = temp->next;
		}
	}
	return excel_sheet;
}

struct cell*** insert_the_affected_node(struct cell ***excel_sheet, int temp_col, int temp_row, int col, int row)
{
	struct node *temp = excel_sheet[temp_row][temp_col]->start;
	struct node *pre = temp;
	while (temp)
	{
		if (temp->col == col && temp->row == row)
		{
			return excel_sheet;
		}
		pre = temp;
		temp = temp->next;
	}
	struct node *new_node = (struct node*)malloc(sizeof(struct node));
	new_node->col = col;
	new_node->row = row;
	new_node->next = NULL;
	if (!excel_sheet[temp_row][temp_col]->start)
		excel_sheet[temp_row][temp_col]->start = new_node;
	else
		pre->next = new_node;
	return excel_sheet;
}

struct cell*** update_all_related_values_for_expression(struct cell ***excel_sheet,char *expression,int col,int row)
{
	char *old_expression = (char*)malloc(sizeof(char) * 10000);
	int len = 0;
	if (excel_sheet[row][col]->expression)
	{
		strcpy(old_expression, excel_sheet[row][col]->expression);
		len = strlen(old_expression);
	}
	int index = 0;
	int temp_row, temp_col;
	while (index < len)
	{
		temp_row = 0;
		if (old_expression[index] >= 97 && old_expression[index] <= 122)
		{
			temp_col = old_expression[index++] - 97 + 1;
			while (old_expression[index] >= 48 && old_expression[index] <= 57)
				temp_row = temp_row * 10 + (old_expression[index++] - 48);
			delete_the_affected_node(excel_sheet, temp_col, temp_row, col, row);
			continue;
		}
		index++;
	}
	if (expression)
	{
		excel_sheet[row][col]->expression = (char*)malloc(sizeof(char) * 1000);
		strcpy(excel_sheet[row][col]->expression, expression);
	}
	index = 0;
	len = strlen(expression);
	while (index < len)
	{
		temp_row = 0;
		if (expression[index] >= 97 && expression[index] <= 122)
		{
			temp_col = expression[index++] - 97 + 1;
			while (expression[index] >= 48 && expression[index] <= 57)
				temp_row = temp_row * 10 + (expression[index++] - 48);
			insert_the_affected_node(excel_sheet,temp_col,temp_row,col,row);
			continue;
		}
		index++;
	}
	return excel_sheet;
}


int detect_the_cycle(struct cell ***excel_sheet,char *expression,int col,int row)
{
	struct cell ***temp_excel_sheet = (struct cell***)malloc(sizeof(struct cell**) * 11);
	for (int i = 1; i <= 10; i++)
	{
		temp_excel_sheet[i] = (struct cell**)malloc(sizeof(struct cell*) * 11);
		for (int j = 1; j <= 10; j++)
			temp_excel_sheet[i][j] = (struct cell*)malloc(sizeof(struct cell));
	}
	for (int i = 1; i <= 10; i++)
		for (int j = 1; j <= 10; j++)
		{
			temp_excel_sheet[i][j]->data = 0;
			temp_excel_sheet[i][j]->expression = NULL;
			temp_excel_sheet[i][j]->start = NULL;
		}
	for (int i = 1; i <= 10; i++)
		for (int j = 1; j <= 10; j++)
		{
			if (excel_sheet[i][j]->expression)
			{
				temp_excel_sheet[i][j]->expression = (char*)malloc(sizeof(char) * 1000);
				strcpy(temp_excel_sheet[i][j]->expression, excel_sheet[i][j]->expression);
			}
			struct node *temp = excel_sheet[i][j]->start;
			struct node *temp_present = NULL;
			int flag = 0;
			while (temp)
			{
				struct node *new_node = (struct node*)malloc(sizeof(struct node));
				new_node->col = temp->col;
				new_node->row = temp->row;
				new_node->next = NULL;
				if (temp_excel_sheet[i][j]->start == NULL)
					temp_excel_sheet[i][j]->start = new_node;
				else
					temp_present->next = new_node;
				temp_present = new_node;
				temp = temp->next;
			}
		}

	excel_sheet=update_all_related_values_for_expression(excel_sheet,expression,col,row);
	if (cycle_detected(excel_sheet,col,row))
	{
		for (int i = 1; i <= 10; i++)
			for (int j = 1; j <= 10; j++)
			{
				excel_sheet[i][j]->data = 0;
				excel_sheet[i][j]->expression = (char*)malloc(sizeof(char) * 1000);
				excel_sheet[i][j]->expression = NULL;
				excel_sheet[i][j]->start = NULL;
			}
		for (int i = 1; i <= 10; i++)
			for (int j = 1; j <= 10; j++)
			{
				if (temp_excel_sheet[i][j]->expression)
				{
					excel_sheet[i][j]->expression = (char*)malloc(sizeof(char) * 1000);
					strcpy(excel_sheet[i][j]->expression, temp_excel_sheet[i][j]->expression);
				}
				else
					excel_sheet[i][j]->expression = NULL;
				struct node *temp = temp_excel_sheet[i][j]->start;
				struct node *temp_present = NULL;
				while (temp)
				{
					struct node *new_node = (struct node*)malloc(sizeof(struct node));
					new_node->col = temp->col;
					new_node->row = temp->row;
					new_node->next = NULL;
					if (excel_sheet[i][j]->start == NULL)
						excel_sheet[i][j]->start = new_node;
					else
						temp_present->next = new_node;
					temp_present = new_node;
					temp = temp->next;
				}
			}
		return 1;
	}
	else
		return 0;
}