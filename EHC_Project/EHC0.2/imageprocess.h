#pragma once
#include   <stdio.h>
#include "imagedefs.h"
#include "ehcdefs.h"

int _open_image_bitmap(unsigned char * data, int width, int height, EHC_image* ehc_image);
int _open_image(const char* file_name, EHC_image *ehc_image);
int _decode_bmp(FILE * pf, EHC_image *ehc_image);
int  _read_bitmap_info(FILE * pf, BITMAPINFOHEADER *bmpinfo);
int _save_image(const char * file_name, const EHC_image *src);
int _encode_bmp( FILE * pf, const EHC_image *ehc_image);
void _set_bmp_header(BITMAPINFOHEADER *bmpinfoheader,const EHC_image *ehc_image );
int _get_palette_size();
int _set_segment_to_image(unsigned char* data,int width, int height, EHC_image *ehc_image);
int _save_segment_image(EHC_image *out_image,unsigned char *in_data, int w, int h, EHC_image *in_image );
int _save_h_runs_image(EHC_image *out_image,int *rx, int *ry, int *rlen, int nruns );
int _save_v_runs_image(EHC_image *out_image,int *rx, int *ry, int *rlen, int nruns );
int _erosion(unsigned char *in_data, int width, int height, int mode, int * *structure, int stru_size, unsigned char * out_data);
int _dilation(unsigned char *in_data,int width, int height, int mode, int * *structure, int stru_size,  unsigned char * out_data);
int _set_gray_to_image(unsigned char* data,int width, int height, EHC_image *ehc_image);
int _save_gray_image(EHC_image *out_image,unsigned char *in_data, int w, int h, EHC_image *in_image );
void _zoom(EHC_image *in_image, EHC_image *out_image, float *xfector, float *yfector);
void _grayscale_rotate_90(unsigned char *idata, unsigned char *odata,int w,int h);
void _grayscale_rotate_n90(unsigned char *idata, unsigned char *odata,int w, int h);
void _inverse_image(unsigned char *in_data, int width, int height, int rowsize, unsigned char  *out_data);
int  ThiningImage(unsigned char *lpDIBBits, long lWidth, long lHeight);
void _split_image(unsigned char *out_data, long width, long height, int step);
void _v_dilation(unsigned char *out_data, long width, long height);

int init_image_background(int step,int width, int height, EHC_image *ehc_image);
int _set_segment_to_backimage(unsigned char* in_data,int width, int height, EHC_image *ehc_image);
