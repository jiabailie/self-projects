#pragma once
#include "maedefs.h"
#include "common.h"


//MAE_DLL_EXPORT int mae_detect_marker_regions( MAE_image* open_mae_image, MAE_region **mae_regions, int *nregions);
MAE_DLL_EXPORT int mae_detect_marker_regions( MAE_image* open_mae_image, MAE_region **mae_regions, int *nregions, MAE_line_node **lpthead);
//int mae_detect_marker_line_regions( unsigned char *bi_data,unsigned char * _data, int _width, int _height, MAE_region **mae_regions, int *nregions, MAE_line_node **lpthead);
int mae_detect_marker_line_regions(MAE_image *in_image, MAE_image *h_image,
			 unsigned char *bi_data,unsigned char * data, int width, int height,
			 int * h_rx, int *h_ry, int *h_rlen, int h_runs,
			 int * v_rx, int *v_ry, int *v_rlen, int v_runs,
			 int h_line_width, int v_line_width,
			 MAE_region **mae_regions, int *nregions, MAE_line_node **lpthead);

//int mae_detect_marker_solid_regions( MAE_image *in_image, MAE_image *h_image, unsigned char *bi_data,unsigned char * _data, int _width, int _height, MAE_region **mae_regions, int *nregions);
int mae_detect_marker_solid_regions( MAE_image *in_image, MAE_image *h_image, unsigned char *bi_data,unsigned char * _data, int _width, int _height, 
									int * h_rx, int *h_ry, int *h_rlen, int h_runs,
									MAE_region **mae_regions, int *nregions);

MAE_DLL_EXPORT int mae_get_marker_image( MAE_image* src, MAE_region *mae_regions, int nregions, MAE_image* mask, int flag);
MAE_DLL_EXPORT int _mae_save_mark_image_from_points( MAE_image* src, MAE_region *mae_regions, int nregions, MAE_image* mask, int flag);
MAE_DLL_EXPORT int mae_open_image(const char* file_name, MAE_image *mae_image);
MAE_DLL_EXPORT int mae_save_image(const char * file_name, const MAE_image *src);
MAE_DLL_EXPORT int mae_remove_mark_color(MAE_image *io_image, MAE_region * regions, int nregions);
MAE_DLL_EXPORT void mae_destroy_regions(MAE_region* regions, int nregions);
MAE_DLL_EXPORT void mae_destroy_image(MAE_image *mae_image);
MAE_DLL_EXPORT void mae_destroy_line_node(MAE_line_node *head);

MAE_DLL_EXPORT int mae_show_point_image(MAE_image* dst, MAE_line_node *lpthead);
