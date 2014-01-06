#pragma once
#include "maedefs.h"
int _mae_detect_link_region_all(unsigned char *data, int w, int h, MAE_region ** regions, int *nregions);
//void _mae_detect_link_region(unsigned char *data, int w, int h, MAE_region* region);
void _mae_detect_link_region(unsigned char *data, int width, int height,MAE_region* region, MAE_region **_hlines, int *_hnum, MAE_region **_vlines, int *_vnum);
void _mae_set_region_lines(MAE_region* region, MAE_region *_hlines, int _hnum, MAE_region *_vlines, int _vnum);


void _mae_adjust_vert_runs(MAE_region *regions, int num, int width, int height);
int _mae_detect_hori_mark_lines(unsigned char * _data, int _width, int _height, MAE_region **_mae_region, int *_num);
int _mae_detect_vert_mark_lines(unsigned char * _data, int _width, int _height, MAE_region **_mae_region, int *_num);
//void _mae_split_link_region(MAE_region *h_lines, int h_num, MAE_region *v_lines, int v_num, MAE_region *region);
int  _mae_is_h_link_line(MAE_region *h_line, MAE_region *v_line);
int  _mae_is_v_link_line(MAE_region *v_line, MAE_region *h_line);
int _mae_check_hori_vert_lines(MAE_region *h_lines, int h_num, MAE_region *v_lines, int v_num, int *h_nlins, int *v_nlinks);
int _mae_check_is_link_region(unsigned char *data, int w, int h);
int _mae_check_is_link_region2(MAE_region *region);
void _mae_adujst_lines(unsigned char *data,int w, int h, 
					   MAE_line **_v_lines, int *_vlinenum,
					   MAE_line **_h_lines, int *_hlinenum);
void _mae_check_lines_rect(unsigned char *data,int w, int h, 
					   MAE_line **_v_lines, int *_vlinenum,
					   MAE_line **_h_lines, int *_hlinenum);
//void _mae_remove_small_liness(MAE_line **_lines, int *_nnums, int flag);
void _mae_show_region_lines(MAE_region **regions, int *nregions);
//void _mae_detect_region_points(MAE_region *regions, int *num , MAE_point** pthead, int *ptNum );
//void _mae_detect_line_intersection_points(MAE_line *linelist, int linenum , MAE_line_node **lpthead, int *lptNum );
//void _mae_detect_line_corners(MAE_line *linelist, int linenum , MAE_line_node **lpthead, int *lptNum );
void _mae_detect_corners_from_lines(MAE_line *h_line, int h_num, MAE_line *v_line, int v_num, MAE_line_node **lpthead, int *lptNum );

//int  _mae_is_line_intersection(MAE_line *line1, MAE_line *line2, MAE_point *point );
//int  _mae_is_line_intersection(MAE_line *line1, MAE_line *line2, MAE_line_node **lptnode );
int  _mae_is_h_v_line_intersection(MAE_line *line1, MAE_line *line2, MAE_line_node **lptnode );
int  _mae_is_h_v_line_intersection2(MAE_line *line1, MAE_line *line2, MAE_line_node **lptnode );


int _mae_get_linelist_from_lines(MAE_line *_h_lines, int _h_num, MAE_line *_v_lines, int _v_num, MAE_line **linelist, int *_linenum);
//void _mae_calculate_lines_center_line(MAE_line **linelist, int *linenum);
void _mae_calculate_lines_center_line(MAE_line **linelist, int *linenum, int maxw);

int _mae_get_regions_from_linelist(MAE_line *_linelist, int _line_num, MAE_region **_regions, int *_nregion);

void _mae_get_line_max_width(  MAE_line *v_lines, int v_num, MAE_line *h_lines, int h_num, int *width);

//int _mae_check_lptnode(MAE_line_node *pnode, MAE_line_node **lpthead, int lptnum);

//20101227 
//int _mae_check_lptnode(MAE_line_node *checknode, MAE_line_node **lpthead, int lptnum, int *connectptnum) ;

//20101227 
//void _mae_parse_region_form_corners_lines(MAE_line_node **lpthead, int lptnum, MAE_line *linelist, int linenum, MAE_region **rghead);
void _mae_parse_region_form_corners_net(MAE_line_node **lpthead, int lptnum, MAE_region **rghead);
int _mae_check_net_node(MAE_line_node *checknode, MAE_line_node **lpthead, int lptnum, int *connectptnum) ;
int _mae_add_new_region_to_list(MAE_region *regon, MAE_line_node *checknode, MAE_line_node **lpthead, int lptnum, int cornernum, int label);
int _mae_get_start_direction(MAE_line_node *checknode) ;
int _mae_get_start_direction_pos(MAE_line_node *checknode, int *stx, int *st_y) ;


//20101227 
//int _mae_add_corner_to_region(MAE_region *regon, MAE_line_node *checknode, MAE_line_node **lpthead, int lptnum, int cornernum, int label);

int _mae_get_regions_from_regionlist(MAE_region *head, MAE_region **_regions, int *_nregion);
void _mae_sort_hlines(MAE_line *h_lines, int h_num);
void _mae_sort_vlines(MAE_line *v_lines, int v_num);
void _mae_sort_lines(MAE_line *lines, int num);
void _mae_calculate_lines_v_h_center_line(MAE_line *h_lines, int h_num, MAE_line *v_lines, int v_num, int maxw, int width, int height);
//20101227 
//void _mae_adjust_corners(MAE_line_node **lpthead, int *lptNum , int maxlinewidth );


//void _mae_get_corner_point(MAE_line_node *node, int direct, int *x, int *y);
//20101227 
//void _mae_get_corner_point(MAE_line_node *node, int prevdirect, int direct, int *x, int *y);
void _mae_adjust_corner_point(MAE_line_node *node, int prevdirect, int direct, int *x, int *y);


void _mae_adjust_regions_corners(MAE_region **_regions, int *_nregion);
void _mae_adjust_regions_corners2(MAE_region **_regions, int *_nregion);

//20101226 add
void _mae_check_vert_line_links(MAE_line **linelist, int *linenum);
int _mae_is_v_v_link(MAE_line *line1, MAE_line *line2);
void _mae_check_hori_line_links(MAE_line **linelist, int *linenum);
int _mae_is_h_h_link(MAE_line *line1, MAE_line *line2);
//

void _mae_create_line_node_net(unsigned char *data, int width, int height, MAE_line *h_line, int h_num, MAE_line *v_line, int v_num, MAE_line_node **lpthead, int *lptNum );
void _mae_remove_alone_line_node(MAE_line_node **lpthead, int *lptNum );
void _mae_adjust_corners_orders(MAE_region **rghead);
void _mae_calculate_region_rects(MAE_region **rghead);
void _mae_get_all_line_color(MAE_image *h_image, MAE_line *h_line, int h_num, MAE_line *v_line, int v_num);
void _mae_get_line_color(MAE_image *h_image, MAE_line *line);
int  _mae_line_color_is_thesame( MAE_line *line1, MAE_line *line2);
int  _mae_line_color_is_thesame2( MAE_line *line1, MAE_line *line2);
int _mae_save_mark_image_from_lines(MAE_image *in_image, MAE_line *h_line, int h_num, MAE_line *v_line, int v_num, MAE_image *out_image);

int _mae_check_linknode_line(unsigned char *data, int width, int height, MAE_line_node *lnode, MAE_line_node *rnode);

//20110118 add for check 
void _mae_check_s_region_and_l_region(MAE_region **sregions, int * snum, MAE_region **lregions, int * lnum);
int _mae_check_region2(int x1, int y1, int x2, int y2, MAE_region *region);

void _mae_check_corners_net(MAE_line_node **lpthead, int lptnum);
