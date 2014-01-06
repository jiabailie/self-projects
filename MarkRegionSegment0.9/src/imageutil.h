
#pragma once

float _mae_byte_per_pix(int depth);
int _mae_bits_per_pix(int depth);
long _mae_sizefromdepth(int pixwidth,int pixheight,int depth);

int _mae_width_bytes( int nBits, int nWidth );

unsigned char *_mae_allocate_image(int width, int height, int depth);

unsigned char *_mae_mallocate_image(int width,int  height,int depth);
void _mae_rgb2hsi(unsigned char r1,unsigned char g1,unsigned char b1, unsigned char* h,unsigned char* s,unsigned char* i);
void _mae_rgb2hsv(unsigned char r1, unsigned char g1,unsigned char b1, unsigned char *h, unsigned char *s, unsigned char *v );
//void _mae_RGB2Lab(double R, double G, double B, double *_L, double *_a, double *_b);
//void _mae_Lab2RGB(double L, double a, double b, double *R, double *G, double *B);
void _mae_grayimage_rotate_90(unsigned char *idata, unsigned char *odata, int w, int h);
int _mae_rgb_variance(unsigned char _r,unsigned char _g,unsigned char _b);
unsigned char *_mae_imagedup(unsigned char *image,int  width,int height, int depth);
unsigned char *_mae_cut_image(unsigned char *data,int sw,int sh,
						      int x,int y,int w, int h);
//void _mae_cut_image(unsigned char *data,int sw,int sh,int l,int t,int w,int h , unsigned char **out_data);
