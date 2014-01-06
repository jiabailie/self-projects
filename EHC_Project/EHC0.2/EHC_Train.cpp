// EHC_Train.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "restore.h"
#include "train.h"


int parse_command_line(int argc, char **argv, int *flag, char *in_file_name, char *out_file_name)
{
	int i;

 	*flag = 0;
	strcpy(in_file_name,"");
	strcpy(out_file_name, "" );
    if(argc < 2) 
	{
		//-i 56.jpg -o out_56.jpg -f 0 -g 15
		fprintf(stderr, "usage: %s -i MX_in_1.bmp -o MX_out_1.bmp\n", argv[0]);
		return(-1);
    } 
	else 
	{
		for(i = 1; i < argc; i++) 
		{
			if(strcmp(argv[i], "-f") == 0) 
			{
				*flag = atoi(argv[++i]);
			}
			else if(strcmp(argv[i], "-i") == 0) 
			{
				strcpy(in_file_name,argv[++i]);
			}
			else if(strcmp(argv[i], "-o") == 0) 
			{
				strcpy(out_file_name,argv[++i]);
			} 
		}
	}

	return 0;
}
int _tmain(int argc, _TCHAR* argv[])
{

	int trainflg = 0;
	char in_file_name[1024]; 
	char out_file_name[1024]; 
	
	if( parse_command_line(argc, argv, &trainflg, in_file_name, out_file_name) == -1 )
	{
		return -1;
	}

	if( trainflg == 0 ) //train 
	{
		train();
	}
	else               //restore
	{ 
		restore(in_file_name, out_file_name);
	}
	return 0;
}