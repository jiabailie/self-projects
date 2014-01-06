#pragma once
#include "maedefs.h"
//int _mae_detect_h_line(int *rx, int *ry, int *rlen, int nruns, int ** line );
void _mae_destroy_regions(MAE_region* regions, int nregions);
int _mae_detect_region(unsigned char * data, int width, int height, MAE_region **mae_region, int *nregions);
int _mae_test_region(MAE_image *show_region_image, MAE_region *mae_regions, int nregions);
int _mae_save_mark_image(MAE_image *in_image, MAE_region *mae_regions, int nregions, MAE_image *out_image, int flag);
int _mae_save_mark_line_image(MAE_image *in_image, MAE_region *mae_regions, int nregions, MAE_image *out_image, int flag);
int _mae_get_nregions(int *line, int nlines);


int	_mae_get_regions(MAE_region **regions, int *nregions,int width, int height, int *blockst, int *blocked, int blocknum);
//
int _mae_detect_region_hist(unsigned char * data, int width, int height, MAE_region **mae_region, int *nregions);
//
//int _mae_detect_vertlines_blocks(unsigned char * bi_data, unsigned char * data, int width, int height, MAE_region **mae_region, int *nregions);
//int _mae_detect_region_blocks(MAE_image * in_image, unsigned char * bi_data, unsigned char * data, int width, int height, MAE_region **mae_region, int *nregions);
//int _mae_detect_region_blocks(unsigned char *bi_data,unsigned char * _data, int _width, int _height, MAE_region **_mae_region, int *_nregions,
//							  MAE_node **run_list, int *listnum);
int _mae_detect_region_blocks(unsigned char *bi_data,unsigned char * _data, int _width, int _height, 
							 int * h_rx, int *h_ry, int *h_rlen, int h_runs,
							  MAE_region **_mae_region, int *_nregions);

int _mae_get_regions_from_blocks(MAE_block *blocks, int nblocks, MAE_region **_mae_region, int *_nregions);
void _mae_computer_regions_rect(MAE_region **_mae_region, int *_nregions);
void _mae_com_region_rect(MAE_region *_mae_region);

int _mae_get_region_pixnum(MAE_region *region);
void _mae_remove_small_regions(MAE_region **_mae_region, int *_nregions, int w, int h);
void _mae_remove_small_regions3(MAE_region **_mae_region, int *_nregions, int w, int h);


void _mae_merge_region_by_rect(MAE_region **_mae_region, int *_nregions);
void _mae_merge_region_by_rect2(MAE_region **_mae_region, int *_nregions);
int _mae_check_region(int l1, int t1, int w1, int h1, int l2, int t2, int w2, int h2);
int _mae_save_regions(char* filename, MAE_image *in_image, MAE_region *mae_regions, int nregions);
int _mae_save_region(char* filename, MAE_image *in_image, MAE_region *region);
void _mae_adjust_region_list_rect(unsigned char * data, int width, int height, MAE_region **_mae_region, int *_nregions);
void _mae_adjust_region_rect(unsigned char * data, int width, int height, MAE_region *_mae_region);
int _mae_get_8_connect_row(unsigned char * data, int width, int height, int* y, unsigned char * curr, unsigned char * next, int direct);
int _mae_get_8_connect_col(unsigned char * data, int width, int height, int* x, unsigned char * curr, unsigned char * next, int direct);
void _mae_get_8_row_data(unsigned char * data, int width, int height, unsigned char * row, int y, int x1, int x2);
void _mae_get_8_col_data(unsigned char * data, int width, int height, unsigned char * col, int x, int y1, int y2);
void _mae_adjust_region_list_rect2(unsigned char * data, int width, int height, MAE_region **_mae_region, int *_nregions);
void _mae_adjust_region_rect2(unsigned char * data, int width, int height, MAE_region *_mae_region);
int _mae_sum_line_pixs(unsigned char * list, int len);

void _mae_check_back_color_region_all1(unsigned char * data, int w, int h, MAE_region **_mae_region, int *_nregions);
void _mae_check_back_color_region1(unsigned char * data, int w, int h, MAE_region *region);

//void _mae_adjust_region_size_by_fctr(MAE_region **regions, int *nregions, float xfctr, float yfctr);
void _mae_adjust_region_size_by_fctr(MAE_region **regions, int *nregions, float xfctr, float yfctr,int width, int height);
void _mae_adjust_point_size_by_fctr(MAE_line_node **lpthead, float xfctr, float yfctr,int width, int height);

//void _mae_split_region_all(MAE_region **_mae_region, int *_nregions);
//void _mae_split_region(MAE_region *region);
//int  _mae_is_link_region(MAE_region *region1, MAE_region *region2);
//int  _mae_is_link_region(MAE_region *region1, MAE_region *region2, int gap);
int  _mae_is_link_region(MAE_region *region1, MAE_region *region2);
//int  _mae_is_link_region(MAE_image * in_image, MAE_region *region1, MAE_region *region2, int gap);

int _mae_distance(MAE_node *node1, MAE_node *node2);

//void _mae_merge_break_region(MAE_image * in_image, MAE_region **_mae_region, int *_nregions);
void _mae_merge_break_region(MAE_region **_mae_region, int *_nregions);

//void _mae_get_region_e_color(MAE_image * in_image, MAE_region *region, MAE_image_feature * feature);
//void _mae_get_region_e_color(MAE_image * in_image, MAE_region *region, int *re, int *ge, int *be, int *he);
void _mae_remove_small_regions2(MAE_region **_mae_region, int *_nregions, int w, int h);

//20101109 add for detecting lines start
//int _mae_detect_vertlines_blocks(unsigned char *bi_data,unsigned char * _data, int _width, int _height, MAE_line **_vlines, int *_vlinenum,
//							  MAE_node **run_list, int *listnum);
int _mae_detect_vertlines_blocks(unsigned char *bi_data,unsigned char * _data, int _width, int _height, 
							  int * h_rx, int * h_ry, int * h_rlen , int h_runs, int h_line_width,
								MAE_line **_v_lines, int *_vlinenum);
//int _mae_detect_horilines_blocks(unsigned char *bi_data,unsigned char * _data, int _width, int _height, MAE_line **_vlines, int *_vlinenum,
//							  MAE_node **run_list, int *listnum);
int _mae_detect_horilines_blocks(unsigned char *bi_data,unsigned char * _data, int _width, int _height, 
							    int * rx, int * ry, int * rlen , int runs, int v_line_width,
								 MAE_line **_lines, int *_linenum);

void _mae_set_line_type(MAE_line **_lines, int *_num, int type);
int _mae_get_lines_from_blocks(MAE_block *blocks, int nblocks, MAE_line **_lines, int *_vlinenum);
int _mae_get_lines_from_blocks_2(unsigned char* bi_data, unsigned char* _data, int _width, int _height, MAE_block *blocks, int nblocks, MAE_line **_lines, int *_vlinenum);
void _mae_remove_small_liness(MAE_line **_lines, int *_num, int flag);
int _mae_get_regions_from_lines(MAE_line *_h_lines, int _h_num, MAE_line *_v_lines, int _v_num, MAE_region **_mae_region, int *_nregions);
void _mae_adjust_hori_lines(MAE_line *lines, int num, int width, int height);
//20101109 add for detecting lines end

int _mae_save_point_image(MAE_image* dst, MAE_line_node *lpthead);
int _mae_save_mark_image_from_corners(MAE_image *in_image, MAE_region *mae_regions, int nregions, MAE_image *out_image, int flag);
void _mae_draw_line(unsigned char *data, int w, int h, long widthbytes, int movedbits, int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b, int flg);
int _mae_save_mark_image_from_corners2(MAE_image *in_image, MAE_region *mae_regions, int nregions, MAE_image *out_image, int flag);
int _mae_point_is_in_region(int x, int y, MAE_region *region);

