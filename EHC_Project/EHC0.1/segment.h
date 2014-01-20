#pragma once
#include "ehcdefs.h"

void _rgb2hsi(int r,int g,int b, int* h,int* s,int* i);
int _convert_rgb2hsi(unsigned char * h_data, unsigned char * s_data,unsigned char * i_data, EHC_image* ehc_image);
int _convert_rgb2hsv(unsigned char * h_data, unsigned char * s_data,unsigned char * v_data, EHC_image* ehc_image);
int _convert_rgb2gray(unsigned char * i_data, EHC_image* ehc_image, EHC_image* high_color_image);
int _convert_rgb2gray2(unsigned char * i_data, EHC_image* image);
int _segmention_by_threshold(unsigned char *in_data, int width, int height,long nThreshold, unsigned char * out_data);
int _segmention_by_threshold2(unsigned char *in_data, int width, int height,long nThreshold, unsigned char * out_data);
int _segment(unsigned char *in_data, int width, int height, unsigned char * out_data);
void _remove_gray_bg(unsigned char * data, int width, int height);
int _detect_high_color(EHC_image *in_image, EHC_image * out_image, int distance);
int _segment_gray(unsigned char *i_data, int width, int height, unsigned char *bi_data);
int _remove_mark_color3(EHC_image *open_image, EHC_image *in_image, EHC_image *out_image );
int _remove_mark_color2(EHC_image *out_image, unsigned char *in_data );

void _minus_image(unsigned char *dst_data, int width, int height, unsigned char *src_data );
int _segment_only_mark_pixs(unsigned char *in_data, int width, int height, unsigned char *out_data, int flag);
int _get_threshold_otsu_only_mark(long histogram[], long pixs);
int _convert_mark_to_gray(unsigned char * out_data, EHC_image* in_image);
int _convert_rgb_to_gray(unsigned char * out_data, EHC_image* in_image);
int _segmention_from_high_color(EHC_image *h_image, unsigned char * out_data);
void _plus_image(unsigned char *dst_data, int width, int height, unsigned char *src_data );
int _segment_grayforchecktext(unsigned char *in_data, int width, int height, unsigned char *out_data);
