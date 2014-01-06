#pragma once
#include   <stdio.h>
#include "imagedefs.h"
#include "maedefs.h"

int _mae_open_image_bitmap(unsigned char * data, int width, int height, MAE_image* mae_image);
int _mae_open_image(const char* file_name, MAE_image *mae_image);
int _mae_decode_bmp(FILE * pf, MAE_image *mae_image);
int  _mae_read_bitmap_info(FILE * pf, BITMAPINFOHEADER *bmpinfo);
int _mae_save_image(const char * file_name, const MAE_image *src);
int _mae_encode_bmp( FILE * pf, const MAE_image *mae_image);
void _mae_set_bmp_header(BITMAPINFOHEADER *bmpinfoheader,const MAE_image *mae_image );
int _mae_get_palette_size();
int _mae_set_segment_to_image(unsigned char* data,int width, int height, MAE_image *mae_image);
int _mae_save_segment_image(MAE_image *out_image,unsigned char *in_data, int w, int h, MAE_image *in_image );
int _mae_save_h_runs_image(MAE_image *out_image,int *rx, int *ry, int *rlen, int nruns );
int _mae_save_v_runs_image(MAE_image *out_image,int *rx, int *ry, int *rlen, int nruns );
//int //_mae_image_erosion(unsigned char *in_data, int width, int height, unsigned char *out_data);
//int //_mae_image_dilation(unsigned char *in_data, int width, int height, unsigned char *out_data);
int _mae_erosion(unsigned char *in_data, int width, int height, int mode, int * *structure, int stru_size, unsigned char * out_data);
int _mae_dilation(unsigned char *in_data,int width, int height, int mode, int * *structure, int stru_size,  unsigned char * out_data);
int _mae_set_gray_to_image(unsigned char* data,int width, int height, MAE_image *mae_image);
int _mae_save_gray_image(MAE_image *out_image,unsigned char *in_data, int w, int h, MAE_image *in_image );
void _mae_get_region_image(MAE_image *in_image, int l, int t, int w, int h, MAE_image *out_image);
void _mae_set_region_to_image(MAE_image *in_image, MAE_region *region, unsigned char *out_data);
void _mae_set_region_to_image2(MAE_image *in_image, MAE_region *region, unsigned char *out_data, MAE_image * h_image);
void _mae_zoom(MAE_image *in_image, MAE_image *out_image, float *xfector, float *yfector);
void _mae_grayscale_rotate_90(unsigned char *idata, unsigned char *odata,int w,int h);
void _mae_grayscale_rotate_n90(unsigned char *idata, unsigned char *odata,int w, int h);
void _mae_inverse_image(unsigned char *in_data, int width, int height, int rowsize, unsigned char  *out_data);
int  ThiningImage(unsigned char *lpDIBBits, long lWidth, long lHeight);
void _mae_split_image(unsigned char *out_data, long width, long height, int step);
void _mae_v_dilation(unsigned char *out_data, long width, long height);
void _mae_init_image_feature(MAE_image *in_image, MAE_init_feature *init_feature);
void _mae_get_image_rgb_e_v2(MAE_image *in_image, int *re,int *ge, int *be);
