#pragma once
//int _mae_find_runs_max_len(int *rx, int *ry, int *rlen, int num);
int _mae_remove_short_runs(int *rx, int *ry, int *rlen, int num, int threshold);
int _mae_remove_long_runs(int *rx, int *ry, int *rlen, int num, int threshold);
int _mae_remove_long_runs2(int *rx, int *ry, int *rlen, int num, int width, int threshold);
int _mae_get_vline_width(unsigned char *data, int w, int h, int *rx, int *ry, int *rlen, int num, int *lw);
int _mae_get_hline_width(unsigned char *data, int w, int h, int *rx, int *ry, int *rlen, int num, int *lw);
//int _mae_sort_runs_inc(int *rx, int *ry, int *rlen, int num);
//int _mae_compute_runs_average_len(int *rx, int *ry,int *rlen, int num);
int _mae_link_break_runs(int *rx, int *ry, int *rlen, int num, int threshold);
