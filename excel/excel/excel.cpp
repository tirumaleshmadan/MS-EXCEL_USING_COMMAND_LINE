#include"excelheader.h"

int main()
{
	char command[100];
	char data[100];
	char total_line[1000];
	char *exportfilename = (char*)malloc(sizeof(char) * 100);
	char *importfilename = (char*)malloc(sizeof(char) * 100);
	char *importheaderfilename = (char*)malloc(sizeof(char) * 100);
	exportfilename = NULL;
	importfilename = NULL;
	importheaderfilename = NULL;
	struct cell ***empty_excel_sheet = (struct cell***)malloc(sizeof(struct cell**) * 11);
	for (int i = 1; i <= 10; i++)
	{
		empty_excel_sheet[i] = (struct cell**)malloc(sizeof(struct cell*) * 11);
		for (int j = 1; j <= 10; j++)
			empty_excel_sheet[i][j] = (struct cell*)malloc(sizeof(struct cell));
	}
	for (int i = 1; i <= 10; i++)
		for (int j = 1; j <= 10; j++)
		{
			empty_excel_sheet[i][j]->data = 0;
			empty_excel_sheet[i][j]->expression = NULL;
			empty_excel_sheet[i][j]->start = NULL;
		}

	//____________________________________________________________________________________________//

	while (1)
	{
		printf(">");
		gets_s(total_line);
		int length = strlen(total_line);
		int command_index = 0;
		int data_index = 0;
		int flag = 0;
		int index = 0;

		//__________________________________________________________________________________________________//

		while (index<length && (total_line[index] == 32 || total_line[index] == 9))
			index++;
		while (index<length && total_line[index] != 32 && total_line[index] != 9)
		{
			command[command_index++] = total_line[index++];
		}
		command[command_index] = '\0';
		while (index<length && (total_line[index] == 32 || total_line[index] == 9))
			index++;
		while (index<length)
		{
			data[data_index++] = total_line[index++];
		}
		data_index--;
		while (data_index>=0 && (data[data_index] == 32 || data[data_index] == 9))
			data_index--;
		data[data_index+1] = '\0';

		//____________________________________________________________________________________________________//

		if (!strlen(command))
			continue;
		if (check_case_insensitive(command, "get", 3))
		{
			if (check_no_of_strings(data) == 1)
				get_the_csv_data(empty_excel_sheet,data);
			else
				printf("INVALID COMMAND FORMAT\n");
			continue;
		}
		if (check_case_insensitive(command, "set", 3))
		{
			set_the_csv_data(empty_excel_sheet,data);
			continue;
		}
		if (check_case_insensitive(command, "print", 5))
		{
			if (strlen(data))
				printf("INVALID COMMAND FORMAT");
			else
				print_the_csv_data(empty_excel_sheet);
			continue;
		}
		if (check_case_insensitive(command, "export", 6))
		{
			if(check_no_of_strings(data) == 1)
				exportfilename = export_the_csv_file(empty_excel_sheet, data);
			else
				printf("INVALID COMMAND FORMAT\n");
			continue;
		}
		if (check_case_insensitive(command, "import", 6))
		{
			if (check_no_of_strings(data) == 1)
				importfilename = import_the_csv_file(empty_excel_sheet, data);
			else if (check_for_header(data))
				importheaderfilename = import_the_csv_file_with_header(empty_excel_sheet, data);
			else
				printf("INVALID COMMAND FORMAT\n");
			continue;
		}
		if (check_case_insensitive(command, "save", 4))
		{
			if (strlen(data))
				printf("INVALID COMMAND FORMAT\n");
			else
				if (importfilename)
					save_the_csv_data(empty_excel_sheet, importfilename);
				else if (exportfilename)
					save_the_csv_data(empty_excel_sheet, exportfilename);
				else 
					printf("no file to save\n");
			continue;
		}
		if (check_case_insensitive(command, "clear", 5))
		{
			system("cls");
			continue;
		}
		if (check_case_insensitive(command, "exit", 4))
			break;
		printf("INVALID COMMAND\n");
	}
}