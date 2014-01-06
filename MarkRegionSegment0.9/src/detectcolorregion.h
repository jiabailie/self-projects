#pragma once
//#include "k_means.h"
//
//void _mae_detect_mark_color(unsigned char *h_data, unsigned char *s_data, int width, int height, MAE_region * regions, int nregions);
//int _mae_detect_color_region(unsigned char *h_data, unsigned char *s_data, int width, int height, MAE_region* region);
//int _mae_detect_color_region2(unsigned char *h_data, unsigned char *s_data, int width, int height, MAE_region* region);
//int _mae_opt_k_means(dataset *head,	int n);
//double _mae_compute_k_means(clusters *clhead);
//int _mae_compute_variance(int *hues,	int n);

//int _mae_detect_mark_color_region_all(MAE_image *in_image, MAE_image *h_image, MAE_region * regions, int nregions, int flag);
//int _mae_detect_mark_color_region_all(MAE_image *in_image, MAE_image *h_image, unsigned char *bi_data, unsigned char *out_data, MAE_region * regions, int nregions);
//int _mae_detect_mark_color_region_all(MAE_image *in_image, MAE_image *h_image, unsigned char *bi_data, unsigned char *out_data, MAE_region * regions, int nregions, MAE_init_feature * init_feature);
//int _mae_detect_mark_color_region_all(MAE_image *in_image, MAE_image *h_image, unsigned char *bi_data, unsigned char *out_data, MAE_region * regions, int nregions, MAE_init_feature * init_feature, MAE_image_feature **features, int *fnum);
//int _mae_detect_mark_color_region_all(MAE_image *in_image, MAE_image *h_image, unsigned char *bi_data, unsigned char *out_data, MAE_region * regions, int nregions, MAE_init_feature * init_feature);
int _mae_detect_mark_color_region_all(MAE_image *in_image, MAE_image *h_image, unsigned char *bi_data, MAE_region * regions, int nregions, MAE_init_feature * init_feature);
int _mae_detect_text_region_all(MAE_image *in_image, MAE_region * regions, int nregions);

//void _mae_detect_mark_color_region(MAE_image *in_image, MAE_image *h_image, MAE_region* region, int flag);
//void _mae_detect_mark_color_region(MAE_image *in_image, MAE_image *h_image, unsigned char *bi_data, unsigned char *out_data,MAE_region* region);
//void _mae_detect_mark_color_region(MAE_image *in_image, MAE_image *h_image, unsigned char *bi_data, unsigned char *out_data,MAE_region* region, MAE_init_feature * init_feature);
//void _mae_detect_mark_color_region(MAE_image *in_image, MAE_image *h_image, unsigned char *bi_data, unsigned char *out_data,MAE_region* region, MAE_init_feature * init_feature, MAE_image_feature *feature);
//void _mae_detect_mark_color_region(MAE_image *in_image, MAE_image *h_image, unsigned char *bi_data, unsigned char *out_data,MAE_region* region, MAE_init_feature * init_feature);
void _mae_detect_mark_color_region(MAE_image *in_image, MAE_image *h_image, unsigned char *bi_data, MAE_region* region, MAE_init_feature * init_feature);
void _mae_detect_text_region(MAE_image *in_image, MAE_region* region);

//void _mae_hist_mark_region(MAE_image *in_image, MAE_image *h_image, unsigned char * bi_data, MAE_region* region);
//void _mae_get_mark_region_feature(MAE_image *in_image, MAE_image *h_image, MAE_region* region, MAE_image_feature *feature);
//void _mae_get_mark_region_feature(MAE_image *in_image, MAE_image *h_image, unsigned char *bi_data, unsigned char *out_data, MAE_region* region, MAE_image_feature *feature);
//void _mae_get_mark_region_feature(MAE_image *in_image, MAE_image *h_image, unsigned char *bi_data, unsigned char *out_data, MAE_region* region, MAE_image_feature *feature, MAE_init_feature * init_feature);
void _mae_get_mark_region_feature(MAE_image *in_image, MAE_image *h_image, unsigned char *bi_data, MAE_region* region, MAE_image_feature *feature, MAE_init_feature * init_feature);

void _mae_hist_mark_region(MAE_image *in_image, MAE_image *h_image, unsigned char * bi_data, MAE_region* region,  MAE_image_feature *feature);

//void _mae_get_image_rgb_e_v(MAE_image *in_image, MAE_image_feature *feature);
//void _mae_get_image_rgb_e_v(MAE_image *in_image, MAE_image_feature *feature, int idx);
void _mae_get_image_rgb_e_v(MAE_image *in_image, int *re,int *ge, int *be, int *mark_pixs);

//void _mae_get_image_h_e_v(MAE_image *in_image, MAE_image_feature *feature);
//void _mae_get_image_h_e_v(MAE_image *h_image, MAE_image_feature *feature, int idx);
void _mae_get_image_h_e_v(MAE_image *h_image, int *he);

//void _mae_check_region_on_feature(MAE_region* region, MAE_image_feature *feature);
void _mae_check_region_on_feature1(MAE_region* region, MAE_image_feature *feature, MAE_init_feature * init_feature);
//void _mae_check_region_on_feature2(MAE_region* region, MAE_image_feature *feature, MAE_init_feature * init_feature);
//void _mae_check_region_on_feature2(MAE_region* region, MAE_image_feature *feature);
void _mae_check_region_on_feature2(MAE_region* region, MAE_image_feature *feature, MAE_init_feature * init_feature);
//void _mae_check_region_on_feature3(MAE_region* region, MAE_image_feature *feature, MAE_init_feature * init_feature);
//void _mae_check_region_on_feature3(MAE_region* region, MAE_image_feature *feature);
void _mae_check_region_on_feature3(MAE_region* region, MAE_image_feature *feature, MAE_init_feature * init_feature);
//void _mae_check_region_on_feature4(MAE_region* region, MAE_image_feature *feature, MAE_init_feature * init_feature);


void _mae_histogram_rect(MAE_block *_mae_blocks, int _nblocks, int* hist, int gridx, int gridy);

void _mae_parse_rect_hist(int* hist, int gridx, int gridy, MAE_image_feature *feature);

//void _mae_detect_text_size(unsigned char *bi_data, int width, int height, MAE_init_feature * init_feature);
void _mae_detect_text_size(MAE_image *in_image, MAE_init_feature * init_feature);

void _mae_max_connect_size(MAE_block *_mae_blocks, int _nblocks, MAE_image_feature *feature);

