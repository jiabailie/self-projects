#pragma once
#include "maedefs.h"

void _mae_rgb2hsi(int r,int g,int b, int* h,int* s,int* i);
int _mae_convert_rgb2hsi(unsigned char * h_data, unsigned char * s_data,unsigned char * i_data, MAE_image* mae_image);
int _mae_convert_rgb2hsv(unsigned char * h_data, unsigned char * s_data,unsigned char * v_data, MAE_image* mae_image);
int _mae_convert_rgb2gray(unsigned char * i_data, MAE_image* mae_image, MAE_image* high_color_image);
int _mae_convert_rgb2gray2(unsigned char * i_data, MAE_image* image);
int _mae_segmention_by_threshold(unsigned char *in_data, int width, int height,long nThreshold, unsigned char * out_data);
int _mae_segmention_by_threshold2(unsigned char *in_data, int width, int height,long nThreshold, unsigned char * out_data);
int _mae_segment(unsigned char *in_data, int width, int height, unsigned char * out_data);
void _mae_remove_gray_bg(unsigned char * data, int width, int height);
//int _mae_segment_rgb(MAE_image *in_image, MAE_image * out_image);
int _mae_detect_high_color(MAE_image *in_image, MAE_image * out_image, int distance);
int	_mae_segment_gray(unsigned char *i_data, int width, int height, unsigned char *bi_data);
//int _mae_remove_high_color(MAE_image *in_image, MAE_image * out_image);
int _mae_remove_mark_color3(MAE_image *open_image, MAE_image *in_image, MAE_image *out_image );
int _mae_remove_mark_color2(MAE_image *out_image, unsigned char *in_data );
//int _mae_remove_mark_color(MAE_image *open_image, MAE_image *in_image, MAE_image *out_image );
//int _mae_remove_mark_color2(MAE_image *out_image, unsigned char *in_data );

//remove mark color by threshold segment start
void _mae_minus_image(unsigned char *dst_data, int width, int height, unsigned char *src_data );
//void _mae_remove_region_high_color(MAE_image *io_image, MAE_image *h_image, MAE_region* region);
//int _mae_remove_mark_color(MAE_image *io_image, MAE_image *h_image, MAE_region * regions, int nregions);
void _mae_remove_region_high_color(MAE_image *io_image, MAE_region* region);
int _mae_remove_mark_color(MAE_image *io_image,  MAE_region * regions, int nregions);
int	_mae_segment_only_mark_pixs(unsigned char *in_data, int width, int height, unsigned char *out_data, int flag);
int _mae_get_threshold_otsu_only_mark(long histogram[], long pixs);
int _mae_convert_mark_to_gray(unsigned char * out_data, MAE_image* in_image);
int _mae_convert_rgb_to_gray(unsigned char * out_data, MAE_image* in_image);

int _mae_segmention_from_high_color(MAE_image *h_image, unsigned char * out_data);

void _mae_plus_image(unsigned char *dst_data, int width, int height, unsigned char *src_data );

int	_mae_segment_grayforchecktext(unsigned char *in_data, int width, int height, unsigned char *out_data);
