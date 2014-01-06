// test.cpp : Defines the entry point for the console application.

#include "stdafx.h"

#include "..\\..\\src\\detectmark.h"

typedef void (*MYPROC)(LPTSTR);

//typedef int (*Detect_marker_regions)( MAE_image* open_mae_image,  MAE_region **mae_regions, int *nregions);
typedef int (*Detect_marker_regions)( MAE_image* open_mae_image, MAE_region **mae_regions, int *nregions, MAE_line_node **lpthead);
typedef int (*Get_marker_image)( MAE_image* src, MAE_region *mae_regions, int nregions, MAE_image* mask, int flag);
typedef int (*Open_image)(const char* file_name, MAE_image *mae_image);
typedef int (*Save_image)(const char * file_name, const MAE_image *src);
typedef int (*Remove_mark_color)(MAE_image *io_image, MAE_region * regions, int nregions);
typedef void (*Destroy_regions)(MAE_region* regions, int nregions);

typedef int (*Show_point_image)(MAE_image* dst, MAE_line_node *lpthead);
typedef void (*Destroy_image)(MAE_image *mae_image);
typedef void (*Destroy_line_node)(MAE_line_node *head);

int parse_command_line(int argc, char **argv, char *in_file_name, char *out_file_name, int *flag, int *flag2)
{
	int i;

 	*flag = 0;
	strcpy(in_file_name,"");
	strcpy(out_file_name,"");
    if(argc < 2) 
	{
		//-i 56.jpg -o out_56.jpg -f 0 -g 15
		fprintf(stderr, "usage: %s -i MX_in_1.bmp -o MX_out_1.bmp\n", argv[0]);
		return(-1);
    } else {
		for(i = 1; i < argc; i++) 
		{
			if(strcmp(argv[i], "-i") == 0) 
			{
				strcpy(in_file_name,argv[++i]);
			} else if(strcmp(argv[i], "-o") == 0) 
			{
			  strcpy(out_file_name,argv[++i]);
			} else if(strcmp(argv[i], "-f") == 0) 
			{
				*flag = atoi(argv[++i]);
			} else if(strcmp(argv[i], "-g") == 0) 
			{
				*flag2 = atoi(argv[++i]);
			}
		}
	}

	return 0;
}
int _tmain(int argc, _TCHAR* argv[])
{
	int error;
	int flag = 1;
	int flag2 = MAE_GET_MARK_REGION_MASK;
	//-i xxx.bmp -o xxx.bmp -f 0 -g 15
	char in_file_name[1024]; 
	char out_file_name[1024];
	char file_name[1024] = "G:\\zxj\\projects\\MarkedAreaExtraction\\temp\\5.jpg";
	char file_name2[1024] = "G:\\zxj\\projects\\MarkedAreaExtraction\\temp\\result.jpg";
	MAE_image open_mae_image;
	MAE_image mark_image;
	
	MAE_region *mae_regions = NULL;
	int nregions = 0;
	int i;

	MAE_line_node *lpthead = NULL;

	Open_image open_image;
	Save_image save_image;
	Detect_marker_regions detect_marker_regions;
	Get_marker_image get_marker_image;
	Remove_mark_color remove_mark_color;
	Show_point_image  show_point_image;
	Destroy_regions destroy_regions;
	Destroy_image destroy_image;
	Destroy_line_node destroy_line_node;

	parse_command_line(argc,argv,in_file_name, out_file_name, &flag, &flag2);
	if(strcmp(in_file_name, "") != 0)
	{
		strcpy(file_name, in_file_name);
	}
	if(strcmp(out_file_name, "") != 0)
	{
		strcpy(file_name2,out_file_name);
	}

	open_mae_image.type = 1;

	HINSTANCE   hmarkseg   =   ::LoadLibrary("MarkExtractDll.dll"); 
	if(hmarkseg == NULL )
	{
		return -1;
	}
	destroy_image =  (Destroy_image)::GetProcAddress(hmarkseg,   "mae_destroy_image"); 
	if( destroy_image == NULL )
	{
		return -1;
	}	
	destroy_regions =  (Destroy_regions)::GetProcAddress(hmarkseg,   "mae_destroy_regions"); 
	if( destroy_regions == NULL )
	{
		return -1;
	}	

	destroy_line_node =  (Destroy_line_node)::GetProcAddress(hmarkseg,   "mae_destroy_line_node"); 
	if( destroy_regions == NULL )
	{
		return -1;
	}	

	open_image =  (Open_image)::GetProcAddress(hmarkseg,   "mae_open_image"); 
	if( open_image == NULL )
	{
		return -1;
	}
	error = open_image(file_name, &open_mae_image);
	if(error != MAE_NO_ERROR)
	{
		return 0;
	}
	//detect marker regions
	detect_marker_regions =  (Detect_marker_regions)::GetProcAddress(hmarkseg,   "mae_detect_marker_regions"); 
	if( detect_marker_regions == NULL )
	{
		return -1;
	}	
	detect_marker_regions( &open_mae_image, &mae_regions, &nregions, &lpthead);

	//remove marker color
	remove_mark_color =  (Remove_mark_color)::GetProcAddress(hmarkseg,   "mae_remove_mark_color"); 
	if( remove_mark_color == NULL )
	{
		return -1;
	}	
	if( flag)
	{
		remove_mark_color( &open_mae_image, mae_regions, nregions);
	}

	//get marker image
	get_marker_image =  (Get_marker_image)::GetProcAddress(hmarkseg,   "mae_get_marker_image"); 
	if( get_marker_image == NULL )
	{
		return -1;
	}	
	get_marker_image(&open_mae_image, mae_regions, nregions, &mark_image, flag2);

	//test point 

	if( flag2 & MAE_GET_MARK_REGION_FLAG4)
	{
		show_point_image =  (Show_point_image)::GetProcAddress(hmarkseg,   "mae_show_point_image"); 
		if( show_point_image == NULL )
		{
			return -1;
		}	
		error = show_point_image(&mark_image, lpthead);
	}

	//save marker image to file
	save_image =  (Save_image)::GetProcAddress(hmarkseg,   "mae_save_image"); 
	if( save_image == NULL )
	{
		return -1;
	}	
	error = save_image(file_name2, &mark_image );

	destroy_image(&open_mae_image);
	destroy_image(&mark_image);
	destroy_regions(mae_regions, nregions);
	destroy_line_node(lpthead);

	return 0;
}

