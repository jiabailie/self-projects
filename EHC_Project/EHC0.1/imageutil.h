#pragma once

float _byte_per_pix(int depth);
int _bits_per_pix(int depth);
long _sizefromdepth(int pixwidth,int pixheight,int depth);

int _width_bytes( int nBits, int nWidth );

unsigned char *_allocate_image(int width, int height, int depth);

unsigned char *_mallocate_image(int width,int  height,int depth);
void _rgb2hsi(unsigned char r1,unsigned char g1,unsigned char b1, unsigned char* h,unsigned char* s,unsigned char* i);
void _rgb2hsv(unsigned char r1, unsigned char g1,unsigned char b1, unsigned char *h, unsigned char *s, unsigned char *v );
void _grayimage_rotate_90(unsigned char *idata, unsigned char *odata, int w, int h);
int _rgb_variance(unsigned char _r,unsigned char _g,unsigned char _b);
unsigned char *_imagedup(unsigned char *image,int  width,int height, int depth);
unsigned char *_cut_image(unsigned char *data,int sw,int sh,
            int x,int y,int w, int h);
