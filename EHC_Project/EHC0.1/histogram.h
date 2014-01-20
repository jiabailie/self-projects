#pragma once

void _count_hist(unsigned char *data,int width,int height,
       int orient, int **hists,int *len);

void _count_h_hist(unsigned char *data,int width, int height,
         int **hists,int *len);
void _count_h_row_hist(unsigned char *data,int width, int height, int x1, int x2,
         int **hists,int *len);

void _count_h_rect_hist(unsigned char *data,int width, int height, 
       int x1, int y1, int w, int h,
             int **hists,int *len);

void _count_v_hist(unsigned char *data,int width,int height,
         int **hists,int *len);
void _count_v_col_hist(unsigned char *data,int width,int height,int y1,int y2,
         int **hists,int *len);
void _count_v_rect_hist(unsigned char *data,int width,int height,
       int x1, int y12, int w,int h,
         int **hists,int *len);
void _compute_hist_blocks(int *hists, int len, int**blockst, int**blocked, int *blocknum);

void _hist_h_runs(int *rx,int * ry,int * rlen, 
       int nruns, int w, int h, 
       int **h_hist,int * nh_hist);
void _histogram(unsigned char * data, int width, int height, long * histogram);
void _histogram_only_mark_pixs(unsigned char * data, int width, int height, long * histogram, long *pixs, int flag);
void _histogram_text_pixnum(unsigned char * data, int width, int height, long *pix_num);
void _histogram_mark_pixnum(unsigned char * data, int width, int height, long *pix_num);

int _hist_rect_pixs(unsigned char *data,int width, int height, 
       int x1, int y1, int w, int h);
             
void _hist_high_rgb(unsigned char *data,int width, int height, int depth, int widthbytes,
       int *rhist,int *ghist, int *bhist,long *pixs);
void _histogram_mark(unsigned char * data, int width, int height, long * histogram, long *pixs);
void _hist_high_color_pixs(unsigned char *data,int width, int height, int depth, int widthbytes,
          int x1, int y1, int w, int h, long *pixs);
void _hist_contour_pixs(unsigned char *data,int width, int height, int depth, int widthbytes, int bkcol,
          int x1, int y1, int w, int h, long *pixs);
int _mass_center_region(unsigned char *data,int width, int height, int flag,
       int x1, int y1, int w, int h, int *cx, int *cy, int *linewidth);
void _com_line_width(int w, int h,int *h_rx, int *h_ry, int *h_rlen, int h_nruns, 
       int *v_rx, int *v_ry, int *v_rlen, int v_nruns, 
       int *h_line_width, int *v_line_width);
void _hist_high_rgb2(unsigned char *data,int width, int height, int depth, int widthbytes,
       int *rhist,int *ghist, int *bhist);
