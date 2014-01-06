#include "StdAfx.h"
#include "linkregion.h"
#include "imageutil.h"
#include "imageprocess.h"
#include "runs.h"
#include "runsprocess.h"
#include "blocks.h"
#include "regions.h"
#include "memalloc.h"
#include "chain.h"
#include "histogram.h"
#include "segment.h"
#include "detectcolorregion.h"
#include <assert.h>
//#include <stdio.h>
//#include <stdlib.h>
int _mae_detect_link_region_all(unsigned char *data, int width, int height, MAE_region ** regions, int *nregions)
{
	int error = MAE_NO_ERROR;
	int i;
	int num;
	unsigned char *region_data;
	MAE_region *tmp_regions;
	MAE_region *_tmp;
	int link_flag;

	MAE_region *h_lines;
	int h_nums = 0;
	MAE_region *v_lines;
	int v_nums = 0;


	for(i = 0; i < *nregions; i ++ )
	{
		h_nums = 0;
		v_nums = 0;
		h_lines = NULL;
		v_lines = NULL;
		//do not show region deleted
		if( (*regions)[i].label == MAE_MARK_REGION_NO ) 
		{
			continue;
		}
		if((*regions)[i].w < MAE_SMALL_LINE_WIDTH || (*regions)[i].h < MAE_SMALL_LINE_HEIGHT )
		{
			(*regions)[i].label = MAE_MARK_REGION_NO;
			continue;
		}
		_mae_detect_link_region(data, width, height,&(*regions)[i], &h_lines, &h_nums, &v_lines, &v_nums);
		_mae_set_region_lines(&(*regions)[i], h_lines, h_nums, v_lines, v_nums);
		if( h_nums > 0 )	free(h_lines);
		if( v_nums > 0 )	free(v_lines);
	}



	//
	//num = 0;
	//link_flag = 0;
	//for(i = 0; i < *nregions; i ++ )
	//{
	//	if( (*regions)[i].label == MAE_MARK_REGION_NO ) 
	//	{
	//		continue;
	//	}
	//	//if( (*regions)[i].label == 2 ) 
	//	if( (*regions)[i].label == MAE_LINK_REGION_LABEL ) 
	//	{
	//		num += 2;
	//		link_flag = 1;
	//	}else{
	//		num++;
	//	}
	//}
	//
	//if( link_flag )
	//{
	//	_mae_malloc_uchar(&region_data, num * sizeof(MAE_region),"_mae_detect_link_region_all : regions");
	//	tmp_regions = (MAE_region *)region_data;
	//	int _idx = 0;
	//	MAE_region *region1;
	//	for( i = 0; i < *nregions; i++ )
	//	{
	//		if( (*regions)[i].label == -1 ) 
	//		{
	//			continue;
	//		}
	//		//if( (*regions)[i].label == 2 ) 
	//		if( (*regions)[i].label == MAE_LINK_REGION_LABEL ) 
	//		{
	//			region1 = (*regions)[i].sub_head;
	//			int count = 0;
	//			while(region1 != NULL )
	//			{
	//				count++;
	//				if( count > 2 ) break;

	//				tmp_regions[_idx].label = MAE_MARK_REGION_MERGE_NEW;

	//				tmp_regions[_idx].l = region1->l;
	//				tmp_regions[_idx].t = region1->t;
	//				tmp_regions[_idx].w = region1->w;
	//				tmp_regions[_idx].h = region1->h;
	//				_idx++;
	//				region1 = region1->sub_link;

	//			}
	//		}else{
	//			tmp_regions[_idx].label = (*regions)[i].label;
	//			tmp_regions[_idx].l = (*regions)[i].l;
	//			tmp_regions[_idx].t = (*regions)[i].t;
	//			tmp_regions[_idx].w = (*regions)[i].w;
	//			tmp_regions[_idx].h = (*regions)[i].h;
	//			_idx++;
	//		}
	//	}

	//	for( i = 0; i < *nregions; i++ )
	//	{
	//		//if( (*regions)[i].label == 2 ) 
	//		if( (*regions)[i].label == MAE_LINK_REGION_LABEL ) 
	//		{
	//			region1 = (*regions)[i].sub_head;
	//			while(region1 != NULL )
	//			{
	//				_tmp = region1->sub_link;
	//				free(region1);
	//				region1 = _tmp;
	//			}
	//		}
	//	}
	//	free(*regions);
	//	*regions = tmp_regions;
	//	*nregions = num;
	//}

	return error;
}

/*******************************************************/
/* function name: _mae_detect_link_region
/* parameter	: MAE_image *in_image : original input image
/*                MAE_image * h_image : detect hight color image
/*                MAE_region *region  : detect region
/* return	    : void
/* function		: detect one color region is background image or mark region
/*******************************************************/
//void _mae_detect_link_region(unsigned char *data, int width, int height, MAE_region* region)
void _mae_detect_link_region(unsigned char *data, int width, int height, MAE_region* region, MAE_region **h_lines, int *h_num, MAE_region **v_lines, int *v_num)
{
	unsigned char * mdata;
	int w;
	int h;
	int i;
	int pix_num1,pix_num2;
	int x,y,sx,sy;
	//20100826 add start
	//if( _mae_check_is_link_region2(region) == 0 )
	//{
	//	return;
	//}
	//20100826 add end

	//unsigned char * gray_data;
	//unsigned char * bi_data;
	//long mk_pix_num = 0;
	//long mt_pix_num = 0;
	w = region->w;
	h = region->h;
	//get mark region image
	mdata = _mae_cut_image(data, width, height, region->l,region->t, w, h);

	if( _mae_check_is_link_region(mdata, w, h) == 0 )
	{
		free(mdata);
		return;
	}
	//
	//test
	//MAE_image test_image;
	//test_image.depth = 24;
	//test_image.w = w;
	//test_image.h = h;
	//test_image.stride = _mae_width_bytes( 24 , w);
	//test_image.type = 0;
	//test_image.data  = _mae_mallocate_image(test_image.w,test_image.h,test_image.depth);
	//_mae_set_segment_to_image(mdata, w, h, &test_image);
	//_mae_save_image("mark_002.bmp", &test_image);
	//free(test_image.data);
	//test
	//MAE_region *h_lines;
	//int h_nums = 0;
	//MAE_region *v_lines;
	//int v_nums = 0;
	int h_nlinks = 0;
	int v_nlinks = 0;
	//detect hori mark lines
	_mae_detect_vert_mark_lines(mdata, w, h, v_lines, v_num);

	//20101103 for vertical lines
	for(i = 0; i < *v_num; i ++ )
	{
		x = (*v_lines)[i].l;
		y = (*v_lines)[i].t;
		sx = (*v_lines)[i].w / 2;
		sy = (*v_lines)[i].h;
		pix_num1 = _mae_hist_rect_pixs(mdata, w, h, x, y, sx, sy);
		x = (*v_lines)[i].l + (*v_lines)[i].w / 2;
		y = (*v_lines)[i].t;
		sx = (*v_lines)[i].w / 2;
		sy = (*v_lines)[i].h;
		pix_num2 = _mae_hist_rect_pixs(mdata, w, h, x, y, sx, sy);
		if( pix_num1 > pix_num2 )
		{
			(*v_lines)[i].type = 0;
		}else{
			(*v_lines)[i].type = 1;
		}
	}


	////detect vert mark lines
	_mae_detect_hori_mark_lines(mdata, w, h, h_lines, h_num);
	////adjust vertical runs
	_mae_adjust_vert_runs(*h_lines, *h_num, w, h);

	////20101103 for vertical lines
	for(i = 0; i < *h_num; i ++ )
	{
		x = (*h_lines)[i].l;
		y = (*h_lines)[i].t;
		sx = (*h_lines)[i].w;
		sy = (*h_lines)[i].h/2;
		pix_num1 = _mae_hist_rect_pixs(mdata, w, h, x, y, sx, sy);
		x = (*h_lines)[i].l;
		y = (*h_lines)[i].t + (*h_lines)[i].h/2;
		sx = (*h_lines)[i].w;
		sy = (*h_lines)[i].h/2;
		pix_num2 = _mae_hist_rect_pixs(mdata, w, h, x, y, sx, sy);
		if( pix_num1 > pix_num2 )
		{
			(*h_lines)[i].type = 0;
		}else{
			(*h_lines)[i].type = 1;
		}
	}

	return ;
}


int _mae_detect_vert_mark_lines(unsigned char * _data, int _width, int _height, MAE_region **_regions, int *_num)
{
	int *rx, *ry, *rlen, nruns, ralloc;
	unsigned char *data;
	int w;
	int h;

	MAE_block *blocks;
	int nblocks;

	MAE_node * run_list;
	int listnum;

	data = _data;
	w = _width;
	h = _height;
	/* get all horizontal runs in the image */
	_mae_get_runs(&rx, &ry, &rlen, &nruns, &ralloc, data, w, h);

	//link break runs
	_mae_link_break_runs(rx, ry, rlen, nruns, MAE_MARK_RUNS_GAP);
	//remove short runs
	//_mae_remove_short_runs(rx, ry, rlen, nruns, MAE_MARK_LINE_WIDTH);
	//remove long runs

	_mae_remove_long_runs(rx, ry, rlen, nruns, MAE_MARK_LINE_WIDTH);
	//test start
	//MAE_image runs_image;
	//runs_image.w = _width;
	//runs_image.h = _height;
	//runs_image.type = 1;
	//runs_image.depth  = 24;
	//runs_image.stride = _mae_width_bytes(24,_width );
	//runs_image.data  = _mae_mallocate_image(w,h,24);
	//_mae_save_h_runs_image(&runs_image, rx, ry, rlen, nruns );
	//_mae_save_image("run___2___image.bmp",&runs_image);
	//free(runs_image.data);
	//test end

	/* get run chain list */
	_mae_create_runs_chain(&run_list,&listnum , rx, ry, rlen, nruns);

	/* 20100727 detect hori mark line  */

	//detect blocks
	_mae_detect_blocks(run_list, listnum, &blocks, &nblocks);

	//20100906 ADD
	//	_mae_computer_blocks_rect(&blocks,&nblocks);

	//delete small blocks
	//	_mae_delete_small_blocks(blocks, nblocks);

	//20100831 update start
	*_regions = NULL;
	*_num = 0;
	if( nblocks > 0 )
	{
		//get lines
		_mae_get_regions_from_blocks(blocks, nblocks, _regions,_num);

		//computer line rect
		_mae_computer_regions_rect(_regions,_num);

		//merge line based on rect pos
		//		_mae_merge_region_by_rect(_regions,_num);

		//merge line based on rect pos
		//_mae_remove_small_regions(_regions,_num, w, h);


	}
	//
	free(rx);
	free(ry);
	free(rlen);
	free(run_list);
	_mae_destroy_blocks(blocks);

	return 0;
}
int _mae_detect_hori_mark_lines(unsigned char * _data, int _width, int _height, MAE_region **_regions, int *_num)
{
	int *rx, *ry, *rlen, nruns, ralloc;
	unsigned char *data, *rdata;
	int w;
	int h;

	MAE_block *blocks;
	int nblocks;

	MAE_node * run_list;
	int listnum;

	data = _data;
	w = _width;
	h = _height;

	// detect vertical runs
	rdata = _mae_mallocate_image(h, w, 8);
	_mae_grayscale_rotate_90(data, rdata, w, h);
	_mae_get_runs(&rx, &ry, &rlen, &nruns, &ralloc, rdata, h, w);
	free(rdata);
	//link break runs
	_mae_link_break_runs(rx, ry, rlen, nruns, MAE_MARK_RUNS_GAP);
	//remove short runs
	//_mae_remove_short_runs(rx, ry, rlen, nruns, MAE_MARK_LINE_WIDTH);
	//remove long runs
	_mae_remove_long_runs(rx, ry, rlen, nruns, MAE_MARK_LINE_WIDTH);

	//test start
	//MAE_image runs_image;
	//runs_image.depth = 24;
	//runs_image.w = w;
	//runs_image.h = h;
	//runs_image.stride =  _mae_width_bytes(24,runs_image.w );
	//runs_image.type = 1;
	//runs_image.data  = _mae_mallocate_image(runs_image.w,runs_image.h,runs_image.depth);
	//_mae_set_segment_to_image(data, w, h, &runs_image);
	//_mae_save_image("segment_01image.bmp",&runs_image);
	//free(runs_image.data);

	//runs_image.depth = 24;
	//runs_image.w = h;
	//runs_image.h = w;
	//runs_image.stride =  _mae_width_bytes(24,runs_image.w );
	//runs_image.type = 1;
	//runs_image.data  = _mae_mallocate_image(runs_image.w,runs_image.h,runs_image.depth);
	//_mae_set_segment_to_image(rdata, h, w, &runs_image);
	//_mae_save_image("segment_02image.bmp",&runs_image);
	//free(runs_image.data);

	//runs_image.w = h;
	//runs_image.h = w;
	//runs_image.type = 1;
	//runs_image.depth  = 24;
	//runs_image.stride = _mae_width_bytes(24,runs_image.w );
	//runs_image.data  = _mae_mallocate_image(runs_image.w, runs_image.h,24);
	//_mae_save_h_runs_image(&runs_image, rx, ry, rlen, nruns );
	//_mae_save_image("vert_run__2__image.bmp",&runs_image);
	//free(runs_image.data);
	//test end
	/* get run chain list */
	_mae_create_runs_chain(&run_list,&listnum , rx, ry, rlen, nruns);

	//detect blocks
	_mae_detect_blocks(run_list, listnum, &blocks, &nblocks);

	//20100908 ADD
	//	_mae_computer_blocks_rect(&blocks,&nblocks);

	//delete small blocks
	_mae_delete_small_blocks(blocks, nblocks);

	//20100831 update start
	*_regions = NULL;
	*_num = 0;
	if( nblocks > 0 )
	{
		//get lines
		_mae_get_regions_from_blocks(blocks, nblocks, _regions, _num);

		//computer line rect
		_mae_computer_regions_rect(_regions, _num);

		//merge line based on rect pos
		//		_mae_merge_region_by_rect(_regions, _num);

		//merge line based on rect pos
		_mae_remove_small_regions(_regions, _num, w, h);
	}
	//
	free(rx);
	free(ry);
	free(rlen);
	//free(h_hist);
	free(run_list);
	_mae_destroy_blocks(blocks);

	return 0;	
}

void _mae_adjust_vert_runs(MAE_region *lines, int num, int width, int height)
{
	int i;
	MAE_region region;
	//_width is not be used
	int _width;
	_width = width;

	for(i = 0; i < num; i ++ )
	{
		if( lines[i].label == -1 )
		{
			continue;
		}
		region.w = lines[i].h;
		region.h = lines[i].w;
		region.l =  lines[i].t;
		region.t = height - 1 - region.h - lines[i].l;

		lines[i].l = region.l;
		lines[i].t = region.t;
		lines[i].w = region.w;
		lines[i].h = region.h;
	}
}
//
//void _mae_split_link_region(MAE_region *h_lines, int h_num, MAE_region *v_lines, int v_num, MAE_region *_region)
//{
//	int i, j;
//	int hmax;
//	int vmax;
//	int hidx;
//	int vidx;
//	int l,t,r,b;
//	int check;
////	down_head = NULL;
////	up_head = NULL;
//	//head = NULL;
//	MAE_region *region1;
//	MAE_region *region2;
//	
//	int *h_label;
//	int *v_label;
//
//	_mae_malloc_int(&h_label, h_num,"_mae_split_link_region:h_label");
//	_mae_malloc_int(&v_label, v_num,"_mae_split_link_region:v_label");
//	
//	for(i = 0; i < h_num; i ++ )
//	{
//		h_lines[i].sub_head = NULL;
//		h_lines[i].sub_link = NULL;
//		h_label[i] = 0;
//	}
//	for(i = 0; i < v_num; i ++ )
//	{
//		v_lines[i].sub_head = NULL;
//		v_lines[i].sub_link = NULL;
//		v_label[i] = 0;
//	}
//	
//	for(i = 0; i < h_num; i ++ )
//	{
//		if( h_lines[i].label == -1 ) continue;
//
//		//check down link
//		for(j = 0; j < v_num; j ++ )
//		{
//			if( v_lines[j].label == -1 ) continue;
//			//if( _mae_is_h_link_down_line(&h_lines[i], &v_lines[j]) )
//			check = _mae_is_h_link_line(&h_lines[i], &v_lines[j]);
//			if( check == 2 || check == 3)
//			{
//				h_label[i] ++;
//			}
//		}
//		//check up link
//		for(j = 0; j < v_num; j ++ )
//		{
//			if( v_lines[j].label == -1 ) continue;
//			//if( _mae_is_h_link_up_line(&h_lines[i], &v_lines[j]) )
//			check = _mae_is_h_link_line(&h_lines[i], &v_lines[j]);
//			if( check == 1 || check == 3)
//			{
//				h_label[i] ++;
//			}
//		}
//	}
//	for(i = 0; i < v_num; i ++ )
//	{
//		if( v_lines[i].label == -1 ) continue;
//
//
//		//check left link
//		for(j = 0; j < h_num; j ++ )
//		{
//			if( h_lines[j].label == -1 ) continue;
//			//if( _mae_is_v_link_left_line(&v_lines[i], &h_lines[j]) )
//			check = _mae_is_v_link_line(&v_lines[i], &h_lines[j]);
//			if( check == 1 || check == 3)
//			{
//				v_label[i] ++;
//			}
//		}
//		//check right link
//		for(j = 0; j < h_num; j ++ )
//		{
//			if( h_lines[j].label == -1 ) continue;
//			//if( _mae_is_v_link_right_line(&v_lines[i], &h_lines[j]) )
//			check = _mae_is_v_link_line(&v_lines[i], &h_lines[j]);
//			if( check == 2 || check == 3) 
//			{
//				v_label[i] ++;
//			}
//		}
//	}
//
//	hmax = 0;
//	hidx = 0;
//	for(i = 0; i < h_num; i++ )
//	{
//		if( hmax < h_label[i] )
//		{
//			hmax = h_label[i];
//			hidx = i;
//		}
//	}
//	vmax = 0;
//	vidx = 0;
//	for(i = 0; i < v_num; i++ )
//	{
//		if( vmax < v_label[i] )
//		{
//			vmax = v_label[i];
//			vidx = i;
//		}
//	}
//
//
//	_mae_malloc_uchar((unsigned char **)(&region1), sizeof(MAE_region),"_mae_split_link_region : region");
//	_mae_malloc_uchar((unsigned char **)(&region2), sizeof(MAE_region),"_mae_split_link_region : region");
//	region1->l = 0;
//	region1->t = 0;
//	region1->w = 0;
//	region1->h = 0;
//	region1->label = 0;
//	region1->head = NULL;
//	region1->tail = NULL;
//	region1->sub_head = NULL;
//	region1->sub_link = NULL;
//	region2->l = 0;
//	region2->t = 0;
//	region2->w = 0;
//	region2->h = 0;
//	region2->label = 0;
//	region2->head = NULL;
//	region2->tail = NULL;
//	region2->sub_head = NULL;
//	region2->sub_link = NULL;
//
//	//MAE_region *h_head1;
//	//MAE_region *v_head1;
//	//MAE_region *h_head2;
//	//MAE_region *v_head2;
//	//h_head1 = NULL;
//	//v_head1 = NULL;
//	//h_head2 = NULL;
//	//v_head2 = NULL;
//	int splitline = 0;
//	int splitline1 = 0;
//	int splitline2 = 0;
//	int h_idx1;
//	int v_idx1;
//	int h_idx2;
//	int v_idx2;
//	MAE_region h_region1[2];
//	MAE_region v_region1[2];
//	MAE_region h_region2[2];
//	MAE_region v_region2[2];
//	for(i = 0; i < 2; i ++ )
//	{
//		h_region1[i].l = 0;
//		h_region1[i].t = 0;
//		h_region1[i].w = 0;
//		h_region1[i].h = 0;
//		v_region1[i].l = 0;
//		v_region1[i].t = 0;
//		v_region1[i].w = 0;
//		v_region1[i].h = 0;
//		h_region2[i].l = 0;
//		h_region2[i].t = 0;
//		h_region2[i].w = 0;
//		h_region2[i].h = 0;
//		v_region2[i].l = 0;
//		v_region2[i].t = 0;
//		v_region2[i].w = 0;
//		v_region2[i].h = 0;
//	}
//	h_idx1 = 0;
//	v_idx1 = 0;
//	h_idx2 = 0;
//	v_idx2 = 0;
//	if( hmax >= vmax )
//	{
//
//		splitline = h_lines[hidx].t + h_lines[hidx].h / 2;
//		//up rect hori line
//		h_region1[0].l = h_lines[hidx].l;
//		h_region1[0].t = h_lines[hidx].t;
//		h_region1[0].w = h_lines[hidx].w;
//		h_region1[0].h = h_lines[hidx].h;
//		h_idx1++;
//		//up rect hori line
//		for(i = 0; i < h_num; i ++ )
//		{
//			if( h_lines[i].label == -1 ) continue;
//			if( i == hidx ) continue;
//			if( (h_lines[i].t + h_lines[i].h / 2 )   <  splitline )
//			{
//				h_region1[1].l = h_lines[i].l;
//				h_region1[1].t = h_lines[i].t;
//				h_region1[1].w = h_lines[i].w;
//				h_region1[1].h = h_lines[i].h;
//				h_idx1++;
//				break;
//			}
//		}
//		////adjust most up y value
//		//if( h_region1[1].t > _region->t ) 
//		//{
//		//	h_region1[1].t = _region->t;
//		//}
//		//up rect ver line
//		for(i = 0; i < v_num; i ++ )
//		{
//			if( v_lines[i].label == -1 ) continue;
//			check = _mae_is_h_link_line(&h_lines[hidx], &v_lines[i]);
//			//if( check == 1 || check == 3)
//			if( check == 1)
//			{
//				if( v_idx1 < 2 )
//				{
//					v_region1[v_idx1].l = v_lines[i].l;
//					v_region1[v_idx1].t = v_lines[i].t;
//					v_region1[v_idx1].w = v_lines[i].w;
//					v_region1[v_idx1].h = v_lines[i].h;
//					v_idx1++;
//				}
//			}
//		}
//		if(v_idx1 < 2 )
//		{
//			for(i = 0; i < v_num; i ++ )
//			{
//				if( v_lines[i].label == -1 ) continue;
//				check = _mae_is_h_link_line(&h_lines[hidx], &v_lines[i]);
//				//if( check == 1 || check == 3)
//				if( check == 3)
//				{
//					if( v_idx1 < 2 )
//					{
//						v_region1[v_idx1].l = v_lines[i].l;
//						v_region1[v_idx1].t = v_lines[i].t;
//						v_region1[v_idx1].w = v_lines[i].w;
//						v_region1[v_idx1].h = v_lines[i].h;
//						v_idx1++;
//					}
//				}
//			}
//		}
//		//down rect hori line
//		h_region2[0].l = h_lines[hidx].l;
//		h_region2[0].t = h_lines[hidx].t;
//		h_region2[0].w = h_lines[hidx].w;
//		h_region2[0].h = h_lines[hidx].h;
//		h_idx2++;
//		//right rect hori line
//		for(i = 0; i < h_num; i ++ )
//		{
//			if( h_lines[i].label == -1 ) continue;
//			if( i == hidx ) continue;
//			if( (h_lines[i].t + h_lines[i].h / 2 )  >  splitline )
//			{
//				h_region2[1].l = h_lines[i].l;
//				h_region2[1].t = h_lines[i].t;
//				h_region2[1].w = h_lines[i].w;
//				h_region2[1].h = h_lines[i].h;
//				h_idx2++;
//				break;
//			}
//		}
//		//right rect ver line
//		for(i = 0; i < v_num; i ++ )
//		{
//			if( v_lines[i].label == -1 ) continue;
//			check = _mae_is_h_link_line(&h_lines[hidx], &v_lines[i]);
//			//if( check == 2 || check == 3)
//			if( check == 2)
//			{
//				if( v_idx2 < 2 )
//				{
//					v_region2[v_idx2].l = v_lines[i].l;
//					v_region2[v_idx2].t = v_lines[i].t;
//					v_region2[v_idx2].w = v_lines[i].w;
//					v_region2[v_idx2].h = v_lines[i].h;
//					v_idx2++;
//				}
//			}
//		}		
//		if( v_idx2 < 2 )
//		{
//			for(i = 0; i < v_num; i ++ )
//			{
//				if( v_lines[i].label == -1 ) continue;
//				check = _mae_is_h_link_line(&h_lines[hidx], &v_lines[i]);
//				//if( check == 2 || check == 3)
//				if( check == 3)
//				{
//					if( v_idx2 < 2 )
//					{
//						v_region2[v_idx2].l = v_lines[i].l;
//						v_region2[v_idx2].t = v_lines[i].t;
//						v_region2[v_idx2].w = v_lines[i].w;
//						v_region2[v_idx2].h = v_lines[i].h;
//						v_idx2++;
//					}
//				}
//			}		
//		}
//
//		//
//	}else{
//		splitline = v_lines[vidx].l + v_lines[vidx].w / 2;
//
//		//left rect vertical line
//		v_region1[0].l = v_lines[vidx].l;
//		v_region1[0].t = v_lines[vidx].t;
//		v_region1[0].w = v_lines[vidx].w;
//		v_region1[0].h = v_lines[vidx].h;
//		v_idx1++;
//		//left rect vertical line
//		for(i = 0; i < v_num; i ++ )
//		{
//			if( v_lines[i].label == -1 ) continue;
//			if( i == vidx ) continue; 			
//			//left
//			if( (v_lines[i].l +  v_lines[i].w / 2) <  splitline )
//			{
//				v_region1[1].l = v_lines[i].l;
//				v_region1[1].t = v_lines[i].t;
//				v_region1[1].w = v_lines[i].w;
//				v_region1[1].h = v_lines[i].h;
//				v_idx1++;
//				break;
//			}
//		}
//		//left rect hori line
//		for(i = 0; i < h_num; i ++ )
//		{
//			if( h_lines[i].label == -1 ) continue;
//			check = _mae_is_v_link_line(&v_lines[vidx], &h_lines[i]);
//			//if( check == 1 || check == 3)
//			if( check == 1)
//			{
//				if( h_idx1 < 2 )
//				{
//					h_region1[h_idx1].l = h_lines[i].l;
//					h_region1[h_idx1].t = h_lines[i].t;
//					h_region1[h_idx1].w = h_lines[i].w;
//					h_region1[h_idx1].h = h_lines[i].h;
//					h_idx1++;
//				}
//			}
//		}
//		if( h_idx1 < 2 )
//		{
//			for(i = 0; i < h_num; i ++ )
//			{
//				if( h_lines[i].label == -1 ) continue;
//				check = _mae_is_v_link_line(&v_lines[vidx], &h_lines[i]);
//				//if( check == 1 || check == 3)
//				if( check == 3)
//				{
//					if( h_idx1 < 2 )
//					{
//						h_region1[h_idx1].l = h_lines[i].l;
//						h_region1[h_idx1].t = h_lines[i].t;
//						h_region1[h_idx1].w = h_lines[i].w;
//						h_region1[h_idx1].h = h_lines[i].h;
//						h_idx1++;
//					}
//				}
//			}
//		}
//		//right rect verticla line
//		v_region2[0].l = v_lines[vidx].l;
//		v_region2[0].t = v_lines[vidx].t;
//		v_region2[0].w = v_lines[vidx].w;
//		v_region2[0].h = v_lines[vidx].h;
//		v_idx2++;
//		//right rect verticla line
//		for(i = 0; i < v_num; i ++ )
//		{
//			if( v_lines[i].label == -1 ) continue;
//			if( i == vidx ) continue; 			
//			//left
//			if( (v_lines[i].l +  v_lines[i].w / 2) >  splitline )
//			{
//				v_region2[1].l = v_lines[i].l;
//				v_region2[1].t = v_lines[i].t;
//				v_region2[1].w = v_lines[i].w;
//				v_region2[1].h = v_lines[i].h;
//				v_idx2++;
//				break;
//			}
//		}
//		//right rect hori line
//		for(i = 0; i < h_num; i ++ )
//		{
//			if( h_lines[i].label == -1 ) continue;
//			check = _mae_is_v_link_line(&v_lines[vidx], &h_lines[i]);
//			if( check == 2 || check == 3)
//			{
//				if( h_idx2 < 2 )
//				{
//					h_region2[h_idx2].l = h_lines[i].l;
//					h_region2[h_idx2].t = h_lines[i].t;
//					h_region2[h_idx2].w = h_lines[i].w;
//					h_region2[h_idx2].h = h_lines[i].h;
//					h_idx2++;
//				}
//			}
//
//		}
//		if( h_idx2 < 2 )
//		{
//			for(i = 0; i < h_num; i ++ )
//			{
//				if( h_lines[i].label == -1 ) continue;
//				check = _mae_is_v_link_line(&v_lines[vidx], &h_lines[i]);
//				if( check == 3)
//				{
//					if( h_idx2 < 2 )
//					{
//						h_region2[h_idx2].l = h_lines[i].l;
//						h_region2[h_idx2].t = h_lines[i].t;
//						h_region2[h_idx2].w = h_lines[i].w;
//						h_region2[h_idx2].h = h_lines[i].h;
//						h_idx2++;
//					}
//				}
//
//			}
//		}	
//	}
//	// recomputer rect of region1 
//
//	MAE_region * region;
//
//	l = _region->w;
//	t = _region->h;
//	r = 0;
//	b = 0;
//	for(i = 0; i < 2; i ++ )
//	{
//		region = &h_region1[i];
//		if( t > region->t ) t = region->t;
//		if( b < (region->t + region->h) ) b = region->t + region->h;
//	}
//	for(i = 0; i < 2; i ++ )
//	{
//		region = &v_region1[i];
//		if( l > region->l ) l = region->l;
//		if( r < (region->l + region->w) ) r = region->l + region->w;
//	}
//	region1->l = l;
//	region1->t = t;
//	region1->w = r - l;
//	region1->h = b - t;
//
//	// recomputer rect of region2
//	l = _region->w;
//	t = _region->h;
//	r = 0;
//	b = 0;
//	for(i = 0; i < 2; i ++ )
//	{
//		region = &h_region2[i];
//		if( t > region->t ) t = region->t;
//		if( b < (region->t + region->h) ) b = region->t + region->h;
//	}
//	for(i = 0; i < 2; i ++ )
//	{
//		region = &v_region2[i];
//		if( l > region->l ) l = region->l;
//		if( r < (region->l + region->w) ) r = region->l + region->w;
//	}
//	region2->l = l;
//	region2->t = t;
//	region2->w = r - l;
//	region2->h = b - t;
//
//	region1->l = _region->l + region1->l;
//	region1->t = _region->t + region1->t;
//	region2->l = _region->l + region2->l;
//	region2->t = _region->t + region2->t;
//
//	//20100816 add start 
//	if( region1->l < _region->l )
//	{
//		region1->l = _region->l;
//	}
//	if( region1->t < _region->t )
//	{
//		region1->t = _region->t;
//	}
//	if( (region1->l + region1->w) > (_region->l + _region->w))
//	{
//		region1->w = _region->w;
//	}
//	if( (region1->t + region1->h) > (_region->t + _region->h))
//	{
//		region1->h = _region->h;
//	}
//	if( region2->l < _region->l )
//	{
//		region2->l = _region->l;
//	}
//	if( region2->t < _region->t )
//	{
//		region2->t = _region->t;
//	}
//	if( (region2->l + region2->w) > (_region->l + _region->w))
//	{
//		region2->w = _region->w;
//	}
//	if( (region2->t + region2->h) > (_region->t + _region->h))
//	{
//		region2->h = _region->h;
//	}
//	//20100816 add end
//
//	_region->sub_head = NULL;
//	_region->sub_link = NULL;
//	_mae_add_region(&_region->sub_head, region1);
//	_mae_add_region(&_region->sub_head, region2);
//
//	free(h_label);
//	free(v_label);
//
//	return;	
//}
//
//void _mae_split_link_region(MAE_region *h_lines, int h_num, MAE_region *v_lines, int v_num, MAE_region *_region)
//{
//	int i, j;
//	int hmax;
//	int vmax;
//	int hidx;
//	int vidx;
//	int l,t,r,b;
//	int check;
////	down_head = NULL;
////	up_head = NULL;
//	//head = NULL;
//	MAE_region *region1;
//	MAE_region *region2;
//	
//	int *h_label;
//	int *v_label;
//
//	_mae_malloc_int(&h_label, h_num,"_mae_split_link_region:h_label");
//	_mae_malloc_int(&v_label, v_num,"_mae_split_link_region:v_label");
//	
//	for(i = 0; i < h_num; i ++ )
//	{
//		//h_lines[i].sub_head = NULL;
//		//h_lines[i].sub_link = NULL;
//		h_label[i] = 0;
//	}
//	for(i = 0; i < v_num; i ++ )
//	{
//		//v_lines[i].sub_head = NULL;
//		//v_lines[i].sub_link = NULL;
//		v_label[i] = 0;
//	}
//	
//	for(i = 0; i < h_num; i ++ )
//	{
//		if( h_lines[i].label == -1 ) continue;
//
//		//check down link
//		for(j = 0; j < v_num; j ++ )
//		{
//			if( v_lines[j].label == -1 ) continue;
//			//if( _mae_is_h_link_down_line(&h_lines[i], &v_lines[j]) )
//			check = _mae_is_h_link_line(&h_lines[i], &v_lines[j]);
//			if( check == 2 || check == 3)
//			{
//				h_label[i] ++;
//			}
//		}
//		//check up link
//		for(j = 0; j < v_num; j ++ )
//		{
//			if( v_lines[j].label == -1 ) continue;
//			//if( _mae_is_h_link_up_line(&h_lines[i], &v_lines[j]) )
//			check = _mae_is_h_link_line(&h_lines[i], &v_lines[j]);
//			if( check == 1 || check == 3)
//			{
//				h_label[i] ++;
//			}
//		}
//	}
//	for(i = 0; i < v_num; i ++ )
//	{
//		if( v_lines[i].label == -1 ) continue;
//
//
//		//check left link
//		for(j = 0; j < h_num; j ++ )
//		{
//			if( h_lines[j].label == -1 ) continue;
//			//if( _mae_is_v_link_left_line(&v_lines[i], &h_lines[j]) )
//			check = _mae_is_v_link_line(&v_lines[i], &h_lines[j]);
//			if( check == 1 || check == 3)
//			{
//				v_label[i] ++;
//			}
//		}
//		//check right link
//		for(j = 0; j < h_num; j ++ )
//		{
//			if( h_lines[j].label == -1 ) continue;
//			//if( _mae_is_v_link_right_line(&v_lines[i], &h_lines[j]) )
//			check = _mae_is_v_link_line(&v_lines[i], &h_lines[j]);
//			if( check == 2 || check == 3) 
//			{
//				v_label[i] ++;
//			}
//		}
//	}
//
//	hmax = 0;
//	hidx = 0;
//	for(i = 0; i < h_num; i++ )
//	{
//		if( hmax < h_label[i] )
//		{
//			hmax = h_label[i];
//			hidx = i;
//		}
//	}
//	vmax = 0;
//	vidx = 0;
//	for(i = 0; i < v_num; i++ )
//	{
//		if( vmax < v_label[i] )
//		{
//			vmax = v_label[i];
//			vidx = i;
//		}
//	}
//
//
//	_mae_malloc_uchar((unsigned char **)(&region1), sizeof(MAE_region),"_mae_split_link_region : region");
//	_mae_malloc_uchar((unsigned char **)(&region2), sizeof(MAE_region),"_mae_split_link_region : region");
//	region1->l = 0;
//	region1->t = 0;
//	region1->w = 0;
//	region1->h = 0;
//	region1->label = 0;
//	region1->head = NULL;
//	region1->tail = NULL;
//	//region1->sub_head = NULL;
//	//region1->sub_link = NULL;
//	region2->l = 0;
//	region2->t = 0;
//	region2->w = 0;
//	region2->h = 0;
//	region2->label = 0;
//	region2->head = NULL;
//	region2->tail = NULL;
////	region2->sub_head = NULL;
////	region2->sub_link = NULL;
//
//	//MAE_region *h_head1;
//	//MAE_region *v_head1;
//	//MAE_region *h_head2;
//	//MAE_region *v_head2;
//	//h_head1 = NULL;
//	//v_head1 = NULL;
//	//h_head2 = NULL;
//	//v_head2 = NULL;
//	int splitline = 0;
//	//int splitline1 = 0;
//	//int splitline2 = 0;
//	int h_idx1;
//	int v_idx1;
//	int h_idx2;
//	int v_idx2;
//	int tmpidx;
//	int tmplen;
//	MAE_region h_region1[2];
//	MAE_region v_region1[2];
//	MAE_region h_region2[2];
//	MAE_region v_region2[2];
//	for(i = 0; i < 2; i ++ )
//	{
//		h_region1[i].l = 0;
//		h_region1[i].t = 0;
//		h_region1[i].w = 0;
//		h_region1[i].h = 0;
//		v_region1[i].l = 0;
//		v_region1[i].t = 0;
//		v_region1[i].w = 0;
//		v_region1[i].h = 0;
//		h_region2[i].l = 0;
//		h_region2[i].t = 0;
//		h_region2[i].w = 0;
//		h_region2[i].h = 0;
//		v_region2[i].l = 0;
//		v_region2[i].t = 0;
//		v_region2[i].w = 0;
//		v_region2[i].h = 0;
//	}
//	h_idx1 = 0;
//	v_idx1 = 0;
//	h_idx2 = 0;
//	v_idx2 = 0;
//	if( hmax >= vmax )
//	{
//
//		splitline = h_lines[hidx].t + h_lines[hidx].h / 2;
//		//up rect hori line
//		h_region1[0].l = h_lines[hidx].l;
//		h_region1[0].t = h_lines[hidx].t;
//		h_region1[0].w = h_lines[hidx].w;
//		h_region1[0].h = h_lines[hidx].h;
//		h_idx1++;
//		//up rect hori line
//		tmpidx = 0;
//		tmplen = 0;
//		for(i = 0; i < h_num; i ++ )
//		{
//			if( h_lines[i].label == -1 ) continue;
//			if( i == hidx ) continue;
//			if( (h_lines[i].t + h_lines[i].h / 2 )   <  splitline )
//			{
//				//20100909 update
//				//h_region1[1].l = h_lines[i].l;
//				//h_region1[1].t = h_lines[i].t;
//				//h_region1[1].w = h_lines[i].w;
//				//h_region1[1].h = h_lines[i].h;
//				//h_idx1++;
//				//break;
//				if( tmplen < h_lines[i].w )
//				{
//					tmplen = h_lines[i].w;
//					tmpidx = i;
//				}
//			}
//		}
//		//20100909 add
//		h_region1[1].l = h_lines[tmpidx].l;
//		h_region1[1].t = h_lines[tmpidx].t;
//		h_region1[1].w = h_lines[tmpidx].w;
//		h_region1[1].h = h_lines[tmpidx].h;
//		h_idx1++;
//		//
//		////adjust most up y value
//		//if( h_region1[1].t > _region->t ) 
//		//{
//		//	h_region1[1].t = _region->t;
//		//}
//		//up rect ver line
//		for(i = 0; i < v_num; i ++ )
//		{
//			if( v_lines[i].label == -1 ) continue;
//			check = _mae_is_h_link_line(&h_lines[hidx], &v_lines[i]);
//			//if( check == 1 || check == 3)
//			if( check == 1)
//			{
//				if( v_idx1 < 2 )
//				{
//					v_region1[v_idx1].l = v_lines[i].l;
//					v_region1[v_idx1].t = v_lines[i].t;
//					v_region1[v_idx1].w = v_lines[i].w;
//					v_region1[v_idx1].h = v_lines[i].h;
//					v_idx1++;
//				}
//			}
//		}
//		if(v_idx1 < 2 )
//		{
//			for(i = 0; i < v_num; i ++ )
//			{
//				if( v_lines[i].label == -1 ) continue;
//				check = _mae_is_h_link_line(&h_lines[hidx], &v_lines[i]);
//				//if( check == 1 || check == 3)
//				if( check == 3)
//				{
//					if( v_idx1 < 2 )
//					{
//						v_region1[v_idx1].l = v_lines[i].l;
//						v_region1[v_idx1].t = v_lines[i].t;
//						v_region1[v_idx1].w = v_lines[i].w;
//						v_region1[v_idx1].h = v_lines[i].h;
//						v_idx1++;
//					}
//				}
//			}
//		}
//		//down rect hori line
//		h_region2[0].l = h_lines[hidx].l;
//		h_region2[0].t = h_lines[hidx].t;
//		h_region2[0].w = h_lines[hidx].w;
//		h_region2[0].h = h_lines[hidx].h;
//		h_idx2++;
//		//right rect hori line
//		tmpidx = 0;
//		tmplen = 0;
//		for(i = 0; i < h_num; i ++ )
//		{
//			if( h_lines[i].label == -1 ) continue;
//			if( i == hidx ) continue;
//			if( (h_lines[i].t + h_lines[i].h / 2 )  >  splitline )
//			{
//				//h_region2[1].l = h_lines[i].l;
//				//h_region2[1].t = h_lines[i].t;
//				//h_region2[1].w = h_lines[i].w;
//				//h_region2[1].h = h_lines[i].h;
//				//h_idx2++;
//				//break;
//				if( tmplen < h_lines[i].w )
//				{
//					tmplen = h_lines[i].w;
//					tmpidx = i;
//				}
//			}
//		}
//		//20100909 add
//		h_region2[1].l = h_lines[tmpidx].l;
//		h_region2[1].t = h_lines[tmpidx].t;
//		h_region2[1].w = h_lines[tmpidx].w;
//		h_region2[1].h = h_lines[tmpidx].h;
//		h_idx2++;
//		//
//		//right rect ver line
//		for(i = 0; i < v_num; i ++ )
//		{
//			if( v_lines[i].label == -1 ) continue;
//			check = _mae_is_h_link_line(&h_lines[hidx], &v_lines[i]);
//			//if( check == 2 || check == 3)
//			if( check == 2)
//			{
//				if( v_idx2 < 2 )
//				{
//					v_region2[v_idx2].l = v_lines[i].l;
//					v_region2[v_idx2].t = v_lines[i].t;
//					v_region2[v_idx2].w = v_lines[i].w;
//					v_region2[v_idx2].h = v_lines[i].h;
//					v_idx2++;
//				}
//			}
//		}		
//		if( v_idx2 < 2 )
//		{
//			for(i = 0; i < v_num; i ++ )
//			{
//				if( v_lines[i].label == -1 ) continue;
//				check = _mae_is_h_link_line(&h_lines[hidx], &v_lines[i]);
//				//if( check == 2 || check == 3)
//				if( check == 3)
//				{
//					if( v_idx2 < 2 )
//					{
//						v_region2[v_idx2].l = v_lines[i].l;
//						v_region2[v_idx2].t = v_lines[i].t;
//						v_region2[v_idx2].w = v_lines[i].w;
//						v_region2[v_idx2].h = v_lines[i].h;
//						v_idx2++;
//					}
//				}
//			}		
//		}
//
//		//
//	}else{
//		splitline = v_lines[vidx].l + v_lines[vidx].w / 2;
//
//		//left rect vertical line
//		v_region1[0].l = v_lines[vidx].l;
//		v_region1[0].t = v_lines[vidx].t;
//		v_region1[0].w = v_lines[vidx].w;
//		v_region1[0].h = v_lines[vidx].h;
//		v_idx1++;
//		//left rect vertical line
//		tmpidx = 0;
//		tmplen = 0;
//		for(i = 0; i < v_num; i ++ )
//		{
//			if( v_lines[i].label == -1 ) continue;
//			if( i == vidx ) continue; 			
//			//left
//			if( (v_lines[i].l +  v_lines[i].w / 2) <  splitline )
//			{
//				//v_region1[1].l = v_lines[i].l;
//				//v_region1[1].t = v_lines[i].t;
//				//v_region1[1].w = v_lines[i].w;
//				//v_region1[1].h = v_lines[i].h;
//				//v_idx1++;
//				//break;
//				if( tmplen < v_lines[i].h )
//				{
//					tmplen = v_lines[i].h;
//					tmpidx = i;
//				}
//			}
//		}
//		//20100909 add
//		v_region1[1].l = v_lines[tmpidx].l;
//		v_region1[1].t = v_lines[tmpidx].t;
//		v_region1[1].w = v_lines[tmpidx].w;
//		v_region1[1].h = v_lines[tmpidx].h;
//		v_idx1++;
//		//
//		//left rect hori line
//		for(i = 0; i < h_num; i ++ )
//		{
//			if( h_lines[i].label == -1 ) continue;
//			check = _mae_is_v_link_line(&v_lines[vidx], &h_lines[i]);
//			//if( check == 1 || check == 3)
//			if( check == 1)
//			{
//				if( h_idx1 < 2 )
//				{
//					h_region1[h_idx1].l = h_lines[i].l;
//					h_region1[h_idx1].t = h_lines[i].t;
//					h_region1[h_idx1].w = h_lines[i].w;
//					h_region1[h_idx1].h = h_lines[i].h;
//					h_idx1++;
//				}
//			}
//		}
//		if( h_idx1 < 2 )
//		{
//			for(i = 0; i < h_num; i ++ )
//			{
//				if( h_lines[i].label == -1 ) continue;
//				check = _mae_is_v_link_line(&v_lines[vidx], &h_lines[i]);
//				//if( check == 1 || check == 3)
//				if( check == 3)
//				{
//					if( h_idx1 < 2 )
//					{
//						h_region1[h_idx1].l = h_lines[i].l;
//						h_region1[h_idx1].t = h_lines[i].t;
//						h_region1[h_idx1].w = h_lines[i].w;
//						h_region1[h_idx1].h = h_lines[i].h;
//						h_idx1++;
//					}
//				}
//			}
//		}
//		//right rect verticla line
//		v_region2[0].l = v_lines[vidx].l;
//		v_region2[0].t = v_lines[vidx].t;
//		v_region2[0].w = v_lines[vidx].w;
//		v_region2[0].h = v_lines[vidx].h;
//		v_idx2++;
//		//right rect verticla line
//		tmpidx = 0;
//		tmplen = 0;
//		for(i = 0; i < v_num; i ++ )
//		{
//			if( v_lines[i].label == -1 ) continue;
//			if( i == vidx ) continue; 			
//			//left
//			if( (v_lines[i].l +  v_lines[i].w / 2) >  splitline )
//			{
//				//v_region2[1].l = v_lines[i].l;
//				//v_region2[1].t = v_lines[i].t;
//				//v_region2[1].w = v_lines[i].w;
//				//v_region2[1].h = v_lines[i].h;
//				//v_idx2++;
//				//break;
//				if( tmplen < v_lines[i].h )
//				{
//					tmplen = v_lines[i].h;
//					tmpidx = i;
//				}
//			}
//		}
//		//20100909 add
//		v_region2[1].l = v_lines[tmpidx].l;
//		v_region2[1].t = v_lines[tmpidx].t;
//		v_region2[1].w = v_lines[tmpidx].w;
//		v_region2[1].h = v_lines[tmpidx].h;
//		v_idx2++;
//		//
//		//right rect hori line
//		for(i = 0; i < h_num; i ++ )
//		{
//			if( h_lines[i].label == -1 ) continue;
//			check = _mae_is_v_link_line(&v_lines[vidx], &h_lines[i]);
//			if( check == 2 || check == 3)
//			{
//				if( h_idx2 < 2 )
//				{
//					h_region2[h_idx2].l = h_lines[i].l;
//					h_region2[h_idx2].t = h_lines[i].t;
//					h_region2[h_idx2].w = h_lines[i].w;
//					h_region2[h_idx2].h = h_lines[i].h;
//					h_idx2++;
//				}
//			}
//
//		}
//		if( h_idx2 < 2 )
//		{
//			for(i = 0; i < h_num; i ++ )
//			{
//				if( h_lines[i].label == -1 ) continue;
//				check = _mae_is_v_link_line(&v_lines[vidx], &h_lines[i]);
//				if( check == 3)
//				{
//					if( h_idx2 < 2 )
//					{
//						h_region2[h_idx2].l = h_lines[i].l;
//						h_region2[h_idx2].t = h_lines[i].t;
//						h_region2[h_idx2].w = h_lines[i].w;
//						h_region2[h_idx2].h = h_lines[i].h;
//						h_idx2++;
//					}
//				}
//
//			}
//		}	
//	}
//	// recomputer rect of region1 
//
//	MAE_region * region;
//
//	l = _region->w;
//	t = _region->h;
//	r = 0;
//	b = 0;
//	for(i = 0; i < 2; i ++ )
//	{
//		region = &h_region1[i];
//		if( t > region->t ) t = region->t;
//		if( b < (region->t + region->h) ) b = region->t + region->h;
//	}
//	for(i = 0; i < 2; i ++ )
//	{
//		region = &v_region1[i];
//		if( l > region->l ) l = region->l;
//		if( r < (region->l + region->w) ) r = region->l + region->w;
//	}
//	region1->l = l;
//	region1->t = t;
//	region1->w = r - l;
//	region1->h = b - t;
//
//	// recomputer rect of region2
//	l = _region->w;
//	t = _region->h;
//	r = 0;
//	b = 0;
//	for(i = 0; i < 2; i ++ )
//	{
//		region = &h_region2[i];
//		if( t > region->t ) t = region->t;
//		if( b < (region->t + region->h) ) b = region->t + region->h;
//	}
//	for(i = 0; i < 2; i ++ )
//	{
//		region = &v_region2[i];
//		if( l > region->l ) l = region->l;
//		if( r < (region->l + region->w) ) r = region->l + region->w;
//	}
//	region2->l = l;
//	region2->t = t;
//	region2->w = r - l;
//	region2->h = b - t;
//
//	region1->l = _region->l + region1->l;
//	region1->t = _region->t + region1->t;
//	region2->l = _region->l + region2->l;
//	region2->t = _region->t + region2->t;
//
//	//20100816 add start 
//	if( region1->l < _region->l )
//	{
//		region1->l = _region->l;
//	}
//	if( region1->t < _region->t )
//	{
//		region1->t = _region->t;
//	}
//	if( (region1->l + region1->w) > (_region->l + _region->w))
//	{
//		region1->w = _region->w;
//	}
//	if( (region1->t + region1->h) > (_region->t + _region->h))
//	{
//		region1->h = _region->h;
//	}
//	if( region2->l < _region->l )
//	{
//		region2->l = _region->l;
//	}
//	if( region2->t < _region->t )
//	{
//		region2->t = _region->t;
//	}
//	if( (region2->l + region2->w) > (_region->l + _region->w))
//	{
//		region2->w = _region->w;
//	}
//	if( (region2->t + region2->h) > (_region->t + _region->h))
//	{
//		region2->h = _region->h;
//	}
//	//20100816 add end
//
////	_region->sub_head = NULL;
////	_region->sub_link = NULL;
//	_mae_add_region(&_region->sub_head, region1);
//	_mae_add_region(&_region->sub_head, region2);
//
//	free(h_label);
//	free(v_label);
//
//	return;	
//}

//returh value: 0 not linking 
//            : 1 up
//            : 2 down
int  _mae_is_h_link_line(MAE_region *h_line, MAE_region *v_line)
{

	int vcx,vcy,vy1,vy2;
	int hx1,hx2,hy;

	vcx = v_line->l + v_line->w / 2;
	vcy = v_line->t + v_line->h / 2;
	vy1 = v_line->t;
	vy2 = v_line->t + v_line->h;

	hx1 = h_line->l;
	hx2 = h_line->l + h_line->w;
	hy = h_line->t + h_line->h/2;


	if( vcx > (hx1 -  MAE_MARK_LINE_GAP)  && vcx < (hx2 +  MAE_MARK_LINE_GAP) )
	{
		if(     vcy < hy  
			&& (    (vy2 > hy && abs(vy2-hy) < MAE_MARK_LINE_GAP) 
			|| (vy2 < hy && abs(vy2-hy) < MAE_MARK_LINE_GAP) ))
		{
			return 1; // up
		}
		if( vcy > hy 
			&& (   ( vy1 < hy && abs(hy - vy1 ) < MAE_MARK_LINE_GAP )
			|| abs(vy1-hy) < MAE_MARK_LINE_GAP ) )
		{
			return 2; //down
		}
		if(    abs(vy1-hy) >= MAE_MARK_LINE_GAP 
			&& abs(vy2-hy) >= MAE_MARK_LINE_GAP
			&& hy > vy1 && hy < vy2
			) 
		{
			return 3; //up and down
		}

	}

	return 0;
}
//

//////////////////////////////////////////////////////////////
//return value : 0 not linking line
//             : 1 left link               
//             : 2 right link               
//////////////////////////////////////////////////////////////
int  _mae_is_v_link_line(MAE_region *v_line, MAE_region *h_line)
{
	int hcx,hcy, hx1, hx2;
	int vy1,vy2,vx;
	hx1 = h_line->l;
	hx2 = h_line->l + h_line->w;
	hcx = h_line->l + h_line->w / 2;
	hcy = h_line->t + h_line->h / 2;
	vy1 = v_line->t;
	vy2 = v_line->t + v_line->h;
	vx  = v_line->l + v_line->w / 2;
	if(    hcy < (vy2 + MAE_MARK_LINE_GAP)  && hcy > (vy1 - MAE_MARK_LINE_GAP) )
	{
		if( hcx < vx && (hx2 > vx || abs(hx2-vx) < MAE_MARK_LINE_GAP) )
		{
			return 1; //left
		}
		if( hcx > vx && (hx1 < vx || abs(hx1-vx) < MAE_MARK_LINE_GAP ) )
		{
			return 2; //right
		}
		if(    abs(hx2-vx) >= MAE_MARK_LINE_GAP 
			&& abs(hx1-vx) >= MAE_MARK_LINE_GAP 
			&& vx > hx1 && vx < hx2
			) 
		{
			return 3; //left and right
		}
	}

	return 0;
}
//int _mae_check_hori_vert_lines(MAE_region *h_lines, int h_num, MAE_region *v_lines, int v_num)
int _mae_check_hori_vert_lines(MAE_region *h_lines, int h_num, MAE_region *v_lines, int v_num, int *h_nlinks, int *v_nlinks)
{
	int i;
	int j;
	int *h_label;
	int *v_label;
	int check;
	int *h_flag;
	int *v_flag;
	int result;
	int hlink = 0;
	int vlink = 0;
	_mae_malloc_int(&h_label, h_num,"_mae_check_hori_vert_lines:h_label");
	_mae_malloc_int(&v_label, v_num,"_mae_check_hori_vert_lines:v_label");
	_mae_malloc_int(&h_flag, h_num,"_mae_check_hori_vert_lines:h_label");
	_mae_malloc_int(&v_flag, v_num,"_mae_check_hori_vert_lines:v_label");
	for(i = 0; i < h_num; i ++ )
	{
		h_label[i] = 0;
		h_flag[i] = 0;
	}
	for(i = 0; i < v_num; i ++ )
	{
		v_label[i] = 0;
		v_flag[i] = 0;
	}
	for(i = 0; i < h_num; i ++ )
	{
		if( h_lines[i].label == -1 ) continue;
		hlink ++;
		//check down link
		for(j = 0; j < v_num; j ++ )
		{
			if( v_lines[j].label == -1 ) continue;
			check = _mae_is_h_link_line(&h_lines[i], &v_lines[j]);
			if( check == 2 || check == 3)
			{
				h_label[i] ++;
				h_flag[i] += check;
			}
		}
		//check up link
		for(j = 0; j < v_num; j ++ )
		{
			if( v_lines[j].label == -1 ) continue;
			check = _mae_is_h_link_line(&h_lines[i], &v_lines[j]);
			if( check == 1 || check == 3)
			{
				h_label[i] ++;
				h_flag[i] += check;
			}
		}
	}


	for(i = 0; i < v_num; i ++ )
	{
		if( v_lines[i].label == -1 ) continue;
		vlink ++;


		//check left link
		for(j = 0; j < h_num; j ++ )
		{
			if( h_lines[j].label == -1 ) continue;
			//if( _mae_is_v_link_left_line(&v_lines[i], &h_lines[j]) )
			check = _mae_is_v_link_line(&v_lines[i], &h_lines[j]);
			if( check == 1 || check == 3)
			{
				v_label[i] ++;
				v_flag[i] += check;
			}
		}
		//check right link
		for(j = 0; j < h_num; j ++ )
		{
			if( h_lines[j].label == -1 ) continue;
			//if( _mae_is_v_link_right_line(&v_lines[i], &h_lines[j]) )
			check = _mae_is_v_link_line(&v_lines[i], &h_lines[j]);
			if( check == 2 || check == 3) 
			{
				v_label[i] ++;
				v_flag[i] += check;
			}
		}
	}
	result = 0;
	for( i = 0; i < h_num; i ++ )
	{
		if(      h_label[i] < 2 
			//|| ( h_label[i] == 2 && h_flag[i] == 6 )
			) 
		{
			//h_lines[i].label = -1;
			h_lines[i].label = MAE_NO_LINK_REGION;
			result = -1;
		}
	}

	for( i = 0; i < v_num; i ++ )
	{
		if(    v_label[i] < 2  
			//|| (v_label[i] == 2 && v_flag[i] == 6 )
			) 
		{
			//v_lines[i].label = -1;
			v_lines[i].label = MAE_NO_LINK_REGION;
			result = -1;
		}
	}

	free(h_label);
	free(v_label);

	*h_nlinks = hlink;
	*v_nlinks = vlink;
	return result;
}
int _mae_check_is_link_region(unsigned char *data, int width, int height)
{
	int step_len = 5;
	int i;
	int x,y,w,h;
	int steps;

	steps = 0;
	for( i = 0; i < 10; i++ )
	{
		x = 0;
		y = 0;
		w = (i+1) * step_len;
		h = (i+1) * step_len;
		if( x < 0 || y < 0 || w >= width || h >= height )
		{
			break;
		}
		if( _mae_hist_rect_pixs(data, width, height, x, y, w, h) > 5 )
		{
			break;
		}
		steps ++;
	}
	if( steps > 1 )
	{
		return 1;
	}
	steps = 0;
	for( i = 0; i < 10; i++ )
	{
		x = width - (step_len * (i + 1) );
		y = 0;
		w = (i+1) * step_len;
		h = (i+1) * step_len;
		if( x < 0 || y < 0 || w >= width || h >= height )
		{
			break;
		}
		if( _mae_hist_rect_pixs(data, width, height, x, y, w, h) > 5 )
		{
			break;
		}
		steps ++;
	}
	if( steps > 1 )
	{
		return 1;
	}

	steps = 0;
	for( i = 0; i < 10; i++ )
	{
		x = 0;
		y = height - (i+1) * step_len;
		w = (i+1) * step_len;
		h = (i+1) * step_len;
		if( x < 0 || y < 0 || w >= width || h >= height )
		{
			break;
		}
		if( _mae_hist_rect_pixs(data, width, height, x, y, w, h) > 5 )
		{
			break;
		}
		steps ++;
	}
	if( steps > 1 )
	{
		return 1;
	}

	steps = 0;
	for( i = 0; i < 10; i++ )
	{
		x = width - (i+1) * step_len;;
		y = height - (i+1) * step_len;
		w = (i+1) * step_len;
		h = (i+1) * step_len;
		if( x < 0 || y < 0 || w >= width || h >= height )
		{
			break;
		}
		if( _mae_hist_rect_pixs(data, width, height, x, y, w, h) > 5 )
		{
			break;
		}
		steps ++;
	}
	if( steps > 1 )
	{
		return 1;
	}

	return 0;
}
int _mae_check_is_link_region2(MAE_region *region)
{
	int i,j;
	int y;
	int x;
	int h_lines;
	int v_lines;
	int * h_hist;
	int * h_diff;
	int h_count;
	int * v_hist;
	int * v_diff;
	int v_count;
	int result;

	int miny, maxy,minx,maxx;
	MAE_node	*pnode;
	miny = region->t + region->h;
	maxy = region->t;
	minx = region->l + region->w;
	maxx = region->l;

	pnode = region->head;
	while(pnode != NULL )
	{
		if( minx > pnode->x ) minx = pnode->x;
		if( maxx < pnode->x + pnode->len ) maxx = pnode->x + pnode->len;
		if( miny > pnode->y ) miny = pnode->y;
		if( maxy < pnode->y ) maxy = pnode->y;
		pnode = pnode->next;
	}


	h_count = maxy - miny  + 2;
	v_count = maxx - minx  + 2;
	_mae_malloc_int(&h_hist ,  h_count, "_mae_check_is_link_region2 : h_hist");
	_mae_malloc_int(&h_diff ,  h_count, "_mae_check_is_link_region2 : h_hist");
	_mae_malloc_int(&v_hist ,  v_count, "_mae_check_is_link_region2 : v_hist");
	_mae_malloc_int(&v_diff ,  v_count, "_mae_check_is_link_region2 : v_hist");

	for(i = 0; i < h_count; i ++ )
	{
		h_hist[i] = 0;
		h_diff[i] = 0;
	}
	for(i = 0; i < v_count; i ++ )
	{
		v_hist[i] = 0;
		v_diff[i] = 0;
	}
	pnode = region->head;
	while(pnode != NULL )
	{
		y = pnode->y - miny;
		h_hist[y] += pnode->len;

		x = pnode->x - minx;		
		for(j = x; j < x+ pnode->len; j ++ )
		{
			v_hist[j] ++;
		}

		pnode = pnode->next;
	}
	////////////////////////////////////////////////
	//FILE *fp = fopen("E:\\sharp\\marksegment\\test_1.txt","w");
	//for(i = 0; i < v_count; i ++ )
	//{
	//	fprintf(fp,"   %d,", v_hist[i]);
	//}
	//fprintf(fp, "\n");
	//for(i = 0; i < h_count; i ++ )
	//{
	//	fprintf(fp,"%d\n,", h_hist[i]);
	//}
	//fclose(fp);
	////////////////////////////////////////////////
	for(i = 0; i < h_count - 2; i ++ )
	{
		h_diff[i] = h_hist[i] + h_hist[i+1] + h_hist[i+2];
	}
	for(i = 0; i < v_count - 2; i ++ )
	{
		v_diff[i] = v_hist[i] + v_hist[i+1] + v_hist[i+2];
	}
	////////////////////////////////////////////////
	//fp = fopen("E:\\sharp\\marksegment\\test_diff.txt","w");
	//for(i = 0; i < v_count; i ++ )
	//{
	//	fprintf(fp,"   %d,", v_diff[i]);
	//}
	//fprintf(fp, "\n");
	//for(i = 0; i < h_count; i ++ )
	//{
	//	fprintf(fp,"%d\n,", h_diff[i]);
	//}
	//fclose(fp);
	////////////////////////////////////////////////
	//different
	memcpy(h_hist, h_diff,h_count);
	memcpy(v_hist, v_diff,v_count);
	for(i = 0; i < h_count - 1; i ++ )
	{
		h_diff[i] = h_hist[i+1] - h_hist[i];
	}
	for(i = 1; i < v_count - 1; i ++ )
	{
		v_diff[i] = v_hist[i+1] - v_hist[i];
	}

	////////////////////////////////////////////////
	//FILE *fp;
	//fp = fopen("E:\\sharp\\marksegment\\test_diff2.txt","w");
	//for(i = 0; i < v_count; i ++ )
	//{
	//	fprintf(fp,"   %d,", v_diff[i]);
	//}
	//fprintf(fp, "\n");
	//for(i = 0; i < h_count; i ++ )
	//{
	//	fprintf(fp,"%d\n,", h_diff[i]);
	//}
	//fclose(fp);
	//////////////////////////////////////////////////

	for(i = 0; i < h_count; i ++ )
	{
		if( h_diff[i] < 30 )
		{
			h_diff[i] = 0;
		}
	}
	for(i = 0; i < v_count - 1; i ++ )
	{
		if( v_diff[i] < 30 )
		{
			v_diff[i] = 0;
		}
	}
	//////////////////////////////////////////////////
	//fp = fopen("E:\\sharp\\marksegment\\test_diff_2.txt","w");
	//for(i = 0; i < v_count; i ++ )
	//{
	//	fprintf(fp,"   %d,", v_diff[i]);
	//}
	//fprintf(fp, "\n");
	//for(i = 0; i < h_count; i ++ )
	//{
	//	fprintf(fp,"%d\n,", h_diff[i]);
	//}
	//fclose(fp);
	////////////////////////////////////////////////


	h_lines = 0;
	for(i = 0; i < h_count; i ++ )
	{
		if( h_diff[i] !=  0 )
		{
			h_lines++;
		}
	}
	v_lines = 0;
	for(i = 0; i < v_count; i ++ )
	{
		if( v_diff[i] !=  0 )
		{
			v_lines++;
		}
	}
	h_lines = h_lines / 2;
	v_lines = v_lines / 2;

	if(h_lines <= 2 &&  v_lines<= 2)
	{
		result =  0;
	}else{
		result =  1;
	}

	free(h_hist);
	free(h_diff);
	free(v_hist);
	free(v_diff);

	return result;
}

void _mae_set_region_lines(MAE_region* region, MAE_region *_hlines, int _hnum, MAE_region *_vlines, int _vnum)
{
	int i;
	int count;
	region->linenum = _hnum + _vnum;
	count = 0;
	for(i = 0; i < _hnum; i ++ )
	{
		if( _hlines[i].type == 0 )
		{
			region->lines[count].type = 0;
			region->lines[count].sx = region->l + _hlines[i].l;
			region->lines[count].sy = region->t + _hlines[i].t + _hlines[i].h / 4;
			region->lines[count].ex = region->l + _hlines[i].l + _hlines[i].w;
			region->lines[count].ey = region->t + _hlines[i].t + _hlines[i].h / 4;
			region->lines[count].lw = _hlines[i].h;

			region->lines[count].l = region->l + _hlines[i].l;
			region->lines[count].t = region->t + _hlines[i].t;
			region->lines[count].w = _hlines[i].w;
			region->lines[count].h = _hlines[i].h;
			count ++;
		}else{
			region->lines[count].type = 0;
			region->lines[count].sx = region->l + _hlines[i].l;
			region->lines[count].sy = region->t + _hlines[i].t + _hlines[i].h / 4 * 3;
			region->lines[count].ex = region->l + _hlines[i].l + _hlines[i].w;
			region->lines[count].ey = region->t + _hlines[i].t + _hlines[i].h / 4 * 3;
			region->lines[count].lw = _hlines[i].h;
			region->lines[count].l = region->l + _hlines[i].l;
			region->lines[count].t = region->t + _hlines[i].t;
			region->lines[count].w = _hlines[i].w;
			region->lines[count].h = _hlines[i].h;
			count ++;
		}
	}
	for(i = 0; i < _vnum; i ++ )
	{
		if( _vlines[i].type == 0 )
		{
			region->lines[count].type = 1;
			region->lines[count].sx = region->l + _vlines[i].l + _vlines[i].w / 4;
			region->lines[count].sy = region->t + _vlines[i].t;
			region->lines[count].ex = region->l + _vlines[i].l + _vlines[i].w / 4;
			region->lines[count].ey = region->t + _vlines[i].t + _vlines[i].h;
			region->lines[count].lw = _vlines[i].w;
			region->lines[count].l = region->l + _vlines[i].l;
			region->lines[count].t = region->t + _vlines[i].t;
			region->lines[count].w = _vlines[i].w;
			region->lines[count].h = _vlines[i].h;
			count ++;
		}else{
			region->lines[count].type = 1;
			region->lines[count].sx = region->l + _vlines[i].l + _vlines[i].w / 4 * 3;
			region->lines[count].sy = region->t + _vlines[i].t;
			region->lines[count].ex = region->l + _vlines[i].l + _vlines[i].w / 4 * 3;
			region->lines[count].ey = region->t + _vlines[i].t + _vlines[i].h;
			region->lines[count].lw = _vlines[i].w;
			region->lines[count].l = region->l + _vlines[i].l;
			region->lines[count].t = region->t + _vlines[i].t;
			region->lines[count].w = _vlines[i].w;
			region->lines[count].h = _vlines[i].h;
			count ++;
		}
	}
}

void _mae_check_lines_rect(unsigned char *data,int w, int h, 
						   MAE_line **v_lines, int *v_num,
						   MAE_line **h_lines, int *h_num)
{
	int i;
	for(i = 0; i < *v_num; i ++ )
	{
		if( (*v_lines)[i].l < 0 ) (*v_lines)[i].l = 0;
		if( (*v_lines)[i].t < 0 ) (*v_lines)[i].t = 0;
		if( (*v_lines)[i].l + (*v_lines)[i].w >= w ) (*v_lines)[i].w = w - (*v_lines)[i].l - 1;
		if( (*v_lines)[i].t + (*v_lines)[i].h >= h ) (*v_lines)[i].h = h - (*v_lines)[i].t - 1;
	}

	for(i = 0; i < *h_num; i ++ )
	{
		if( (*h_lines)[i].l < 0 ) (*h_lines)[i].l = 0;
		if( (*h_lines)[i].t < 0 ) (*h_lines)[i].t = 0;
		if( (*h_lines)[i].l + (*h_lines)[i].w >= w ) (*h_lines)[i].w = w - (*h_lines)[i].l - 1;
		if( (*h_lines)[i].t + (*h_lines)[i].h >= h ) (*h_lines)[i].h = h - (*h_lines)[i].t - 1;
	}
}
//
void _mae_adujst_lines(unsigned char *data,int w, int h, 
					   MAE_line **v_lines, int *v_num,
					   MAE_line **h_lines, int *h_num)
{
	int i;
	int x,y,sx,sy,cx,cy;
	int w1,w2;
	int area;
	int dx, dy;
	int adjust;
	int linewidth;
	adjust = 4;
	//_mae_remove_small_liness(v_lines, v_num, w,h,MAE_VERT_LINE);
	for(i = 0; i < *v_num; i ++ )
	{
		if( (*v_lines)[i].l < 0 ) (*v_lines)[i].l = 0;
		if( (*v_lines)[i].t < 0 ) (*v_lines)[i].t = 0;
		if( (*v_lines)[i].l + (*v_lines)[i].w >= w ) (*v_lines)[i].w = w - (*v_lines)[i].l - 1;
		if( (*v_lines)[i].t + (*v_lines)[i].h >= h ) (*v_lines)[i].h = h - (*v_lines)[i].t - 1;

		cx = 0;
		cy = 0;
		linewidth = 0;
		x = (*v_lines)[i].l;
		y = (*v_lines)[i].t;
		sx = (*v_lines)[i].w;
		sy = (*v_lines)[i].h;


		if(sy == 0 || sx == 0 )
		{
			(*v_lines)[i].label = -1;
			continue;
		}
		_mae_mass_center_region(data, w, h, 1, x, y, sx, sy, &cx, &cy, &linewidth);
		//area = _mae_hist_rect_pixs(data, w, h, x, y, sx, sy);
		//dx = area / sy + adjust;
		dx = linewidth;
		if( cx != 0 && cy != 0 )
		{
			(*v_lines)[i].l = cx - dx / 2;
			if( (*v_lines)[i].l <  0 ) (*v_lines)[i].l = 0;
			(*v_lines)[i].w = dx;
			//w1 = abs(cx - (*v_lines)[i].l ); 
			//w2 = abs((*v_lines)[i].l + (*v_lines)[i].w +  - cx  ) ;
			//if( w1 > w2 )
			//{
			//	(*v_lines)[i].l = cx - w2;
			//	(*v_lines)[i].w = 2*w2;
			//}else{
			//	(*v_lines)[i].l = cx - w1;
			//	(*v_lines)[i].w = 2*w1;
			//}
			//if( (*v_lines)[i].l < 0 ) (*v_lines)[i].l = 0;
			//if( (*v_lines)[i].l + (*v_lines)[i].w >= w ) (*v_lines)[i].w = w - (*v_lines)[i].l - 1;
		}
	}
	_mae_remove_small_liness(v_lines, v_num, MAE_VERT_LINE);



	for(i = 0; i < *h_num; i ++ )
	{
		if( (*h_lines)[i].l < 0 ) (*h_lines)[i].l = 0;
		if( (*h_lines)[i].t < 0 ) (*h_lines)[i].t = 0;
		if( (*h_lines)[i].l + (*h_lines)[i].w >= w ) (*h_lines)[i].w = w - (*h_lines)[i].l - 1;
		if( (*h_lines)[i].t + (*h_lines)[i].h >= h ) (*h_lines)[i].h = h - (*h_lines)[i].t - 1;

		cx = 0;
		cy = 0;
		x = (*h_lines)[i].l;
		y = (*h_lines)[i].t;
		sx = (*h_lines)[i].w;
		sy = (*h_lines)[i].h;

		if(sy == 0 || sx == 0 )
		{
			(*h_lines)[i].label = -1;
			continue;
		}
		_mae_mass_center_region(data, w, h, 0, x, y, sx, sy, &cx, &cy, &linewidth);
		//area = _mae_hist_rect_pixs(data, w, h, x, y, sx, sy);
		//dy = area / sx + adjust;
		dy = linewidth;
		//if(cy - dy / 2 < 0) dy = 2*cy;
		if( cx != 0 && cy != 0 )
		{
			(*h_lines)[i].t = cy - dy / 2;
			if((*h_lines)[i].t < 0 ) (*h_lines)[i].t = 0;
			assert( (*h_lines)[i].t >= 0);
			(*h_lines)[i].h = dy;
			//w1 = abs(cy - (*h_lines)[i].t ); 
			//w2 = abs((*h_lines)[i].t + (*h_lines)[i].h +  - cy  ) ;
			//if( w1 > w2 )
			//{
			//	(*h_lines)[i].t = cy - w2;
			//	(*h_lines)[i].h = 2*w2;
			//}else{
			//	(*h_lines)[i].t = cy - w1;
			//	(*h_lines)[i].h = 2*w1;
			//}
			//if( (*h_lines)[i].t < 0 ) (*h_lines)[i].t = 0;
			//if( (*h_lines)[i].t + (*h_lines)[i].h >= h ) (*h_lines)[i].h = h - (*h_lines)[i].t - 1;
		}
	}
	_mae_remove_small_liness(h_lines, h_num,MAE_HORI_LINE);

}
//
//void _mae_remove_small_liness(MAE_line **_lines, int *_nnums, int flag)
//{
//	int i;
//	long pixnum;
//	MAE_line * line;
//	int num;
//	int width;
//	int height;
//
//
//	num = *_nnums;
//	for(i = 0; i < num; i ++ )
//	{
//		line = &((*_lines)[i]);
//		width = line->w;
//		height = line->h;
//		if( flag == MAE_VERT_LINE )
//		{
//			if(width > height )
//			{
//				line->label = -1;
//			}
//		}else{
//			if(width < height )
//			{
//				line->label = -1;
//			}
//		}
//	}
//}

void _mae_show_region_lines(MAE_region **regions, int *nregions)
{
	MAE_region *region;
	int i;
	int count;
	int dx,dy;

	count = 0;

	for(i = 0; i < *nregions; i ++ )
	{
		region = &((*regions)[i]);
		if( region->label == -1 )
		{
			continue;
		}
		region->linenum = 1;
		if( region->type == MAE_HORI_LINE )
		{
			dx = region->h * 3 / 4;
			region->lines[0].type = MAE_HORI_LINE;
			region->lines[0].sx = region->l - dx;
			region->lines[0].sy = region->t + region->h / 2;
			region->lines[0].ex = region->l + region->w + dx;
			region->lines[0].ey = region->t + region->h / 2;
			region->lines[0].lw = region->h;

			region->lines[0].l = region->l;
			region->lines[0].t = region->t;
			region->lines[0].w = region->w;
			region->lines[0].h = region->h;
		}else{
			dy =  region->w * 3 / 4;
			region->lines[0].type = MAE_VERT_LINE;
			region->lines[0].sx = region->l +  region->w / 2;
			region->lines[0].sy = region->t - dy;
			region->lines[0].ex = region->l + region->w / 2;
			region->lines[0].ey = region->t + region->h +  dy;
			region->lines[0].lw = region->w;

			region->lines[0].l = region->l;
			region->lines[0].t = region->t;
			region->lines[0].w = region->w;
			region->lines[0].h = region->h;
		}
	}

}

void _mae_calculate_lines_center_line(MAE_line **linelist, int *linenum, int maxw)
{
	MAE_line *line;
	int i;
	int count;
	int dx,dy;

	count = 0;

	for(i = 0; i < *linenum; i ++ )
	{
		line = &((*linelist)[i]);
		if( line->label == -1 )
		{
			continue;
		}
		if( line->type == MAE_HORI_LINE )
		{
			dx = line->h * 3 / 4;
			line->type = MAE_HORI_LINE;
			line->sx = line->l - dx;
			line->sy = line->t + line->h / 2;
			line->ex = line->l + line->w + dx;
			line->ey = line->t + line->h / 2;
			line->lw = line->h;
		}else{
			dy =  line->w * 3 / 4;
			line->type = MAE_VERT_LINE;
			line->sx = line->l +  line->w / 2;
			line->sy = line->t - dy;
			line->ex = line->l + line->w / 2;
			line->ey = line->t + line->h +  dy;
			line->lw = line->w;
		}
	}

}
//
//void _mae_detect_line_corners(MAE_line *linelist, int linenum , MAE_line_node **lpthead, int *lptNum )
//{
//	int i, j;
//	int count;
//	int left;
//	int top;
//	MAE_line *line;
//
//	//for(i = 0; i < linenum; i ++ )
//	//{
//	//	line = &linelist[i];
//	//	if( line->label == -1 )
//	//	{
//	//		continue;
//	//	}
//	//	//lines.label = i+1;
//	//}
//	MAE_line *line1;
//	MAE_line *line2;
//	//MAE_point pt;
//	MAE_line_node *lptnode;	
//	MAE_line_node *pnode = NULL;
//
//	//pt.x = 0;
//	//pt.y = 0;
//	//pt.type = 0;
//
//	count = 0;
//	for(i = 0; i < linenum; i ++ )
//	for(j = i+1; j < linenum; j ++ )
//	{
//
//		line1 = &(linelist[i]);
//		line2 = &(linelist[j]);
//		if( _mae_is_line_intersection(line1, line2, &lptnode) )
//		{
//		   //_mae_malloc_uchar((unsigned char **)(&pnode) ,  1*sizeof(MAE_line_node), "_mae_detect_region_points : MAE_line_node");
//		   //pnode->type = lptnode.type;
//		   //pnode->label = 0;
//		   //pnode->x = lptnode.x;
//		   //pnode->y = lptnode.y;
//		   //pnode->line_l = lptnode.line_l;
//		   //pnode->line_r = lptnode.line_r;
//		   //pnode->line_u = lptnode.line_u;
//		   //pnode->line_d = lptnode.line_d;
//		   //pnode->link = NULL;
//		   _mae_add_line_node(lpthead, lptnode);
//		   lptnode = NULL;
//			count++;
//		}
//	}
//
//	*lptNum = count;
//	return;
//}

//
//int  _mae_is_line_intersection(MAE_line *line1, MAE_line *line2, MAE_line_node **pnode )
//{
//	int flag;
//	int px,py;
//	int cx,cy;
//	int dx,dy;
//	int gap = 0;
//	int dx1,dx2, dy1,dy2;
//	int dist;
//	MAE_line_node *lptnode;
//
//	flag = 0;
//	px = 0;
//	py = 0;
//	if( line1->type == line2->type )
//	{
//		if( line1->type == MAE_HORI_LINE )
//		{
//			gap = (line1->h + line2->h ) / 2;
//
//			if( abs(line1->sy - line2->sy) > gap  )
//			{
//				return 0;
//			}
//			if( line1->sx < line2->sx )
//			{
//				px = (line1->ex + line2->sx) / 2;
//				dx1 = abs(px - line1->sx);
//				dx2 = abs(line2->ex - px);
//			}else{
//				px = (line1->sx + line2->ex) / 2;
//				dx1 = abs(px - line2->sx);
//				dx2 = abs(line1->ex - px);
//			}
//			py = (line1->sy + line2->sy ) / 2;
//			dist = dx1 + dx2;
//			if( dist < (abs(line1->ex - line1->sx) + abs(line2->ex - line2->sx) + gap) )
//			{
//			    _mae_malloc_uchar((unsigned char **)(pnode) ,  1*sizeof(MAE_line_node), "_mae_is_line_intersection : MAE_line_node");
//				lptnode = *pnode;
//				lptnode->type = 0;
//				lptnode->link = NULL;
//				lptnode->label = 0;
//				lptnode->x = px;
//				lptnode->y = py;
//				lptnode->line_l = NULL;
//				lptnode->line_r = NULL;
//				lptnode->line_u = NULL;
//				lptnode->line_d = NULL;
//				if( line1->sx < px )
//				{
//				// ***********  ***************** 
//					lptnode->line_l = line1;
//					line1->node_r = lptnode;
//					lptnode->line_r = line2;
//					line2->node_l = lptnode;
//				}else{
//					lptnode->line_l = line2;
//					line2->node_r = lptnode;
//					lptnode->line_r = line1;
//					line1->node_l = lptnode;
//				}
//				return 1;
//			}
//		}else{
//			gap = (line1->w + line2->w ) / 2;
//			if( abs(line1->sx - line2->sx) > gap  )
//			{
//				return 0;
//			}
//
//			if( line1->sy < line2->sy )
//			{
//				py = (line1->ey + line2->sy ) / 2;
//				dy1 = abs(py-line1->sy);
//				dy2 = abs(py-line2->ey);
//			}else{
//				py = (line2->ey + line1->sy ) / 2;
//				dy1 = abs(py-line2->sy);
//				dy2 = abs(py-line1->ey);
//			}
//
//			px = (line1->sx + line2->sx ) / 2;
//			dist = dy1 + dy2;
//			if( dist < (abs(line1->ey - line1->sy) + abs(line2->ey - line2->sy) + gap) )
//			{
//			    _mae_malloc_uchar((unsigned char **)(pnode) ,  1*sizeof(MAE_line_node), "_mae_is_line_intersection : MAE_line_node");
//				lptnode = *pnode;
//				lptnode->type = 0;
//				lptnode->link = NULL;
//				lptnode->label = 0;
//				lptnode->x = px;
//				lptnode->y = py;
//				lptnode->line_l = NULL;
//				lptnode->line_r = NULL;
//				lptnode->line_u = NULL;
//				lptnode->line_d = NULL;
//				if( line1->sy < py )
//				{
//				//			 *
//				//           *
//				//           *
//				//           *
//
//				//           *
//				//			 *
//				//           *
//				//           *
//				//           *
//					lptnode->line_u = line1;
//					line1->node_d = lptnode;
//					lptnode->line_d = line2;
//					line2->node_u = lptnode;
//				}else{
//					lptnode->line_u = line2;
//					line2->node_d = lptnode;
//					lptnode->line_d = line1;
//					line1->node_u = lptnode;
//				}
//				return 1;
//			}
//		}
//		return 0;
//	}
//	if( line1->type == MAE_HORI_LINE ) //hori
//	{
//		py = line1->sy;
//		px = line2->sx;
//		cx = (line1->sx + line1->ex) / 2;
//		cy = (line2->sy + line2->ey) / 2;
//		if( line1->h < line2->w)
//		{
//			gap = line2->w / 2;
//		}else{
//			gap = line1->h / 2;
//		}
//		dx = min(abs(px - line1->sx), abs(px-line1->ex));
//		dy = min(abs(py - line2->sy), abs(py-line2->ey));
//	}else{
//		px = line1->sx;
//		py = line2->sy;
//		cx = (line2->sx + line2->ex) / 2;
//		cy = (line1->sy + line1->ey) / 2;
//		if( line1->w < line2->h)
//		{
//			gap = line2->h / 2;
//		}else{
//			gap = line1->w / 2;
//		}
//		dx = min(abs(px - line2->sx), abs(px-line2->ex));
//		dy = min(abs(py - line1->sy), abs(py-line1->ey));
//	}
//
//	//20101207 test
//	gap = 100;
//    //
//	if( line1->type == MAE_HORI_LINE )
//	{
//		if( px >= line1->sx - gap && px <= line1->ex + gap &&
//			py >= line2->sy - gap && py <= line2->ey + gap &&
//			dx < 50 &&
//			dy < 50 )
//		{
//			flag = 1;
//		}
//	}else{
//		if( px >= line2->sx - gap && px <= line2->ex + gap &&
//			py >= line1->sy - gap && py <= line1->ey + gap &&
//			dx < 50 &&
//			dy < 50 )
//		{
//			flag = 1;
//		}
//	}
//
//	if( flag == 1 )
//	{
//	    _mae_malloc_uchar((unsigned char **)(pnode) ,  1*sizeof(MAE_line_node), "_mae_is_line_intersection : MAE_line_node");
//		lptnode = *pnode;
//		lptnode->type = 0;
//		lptnode->link = NULL;
//		lptnode->label = 0;
//		lptnode->x = px;
//		lptnode->y = py;
//		lptnode->line_l = NULL;
//		lptnode->line_r = NULL;
//		lptnode->line_u = NULL;
//		lptnode->line_d = NULL;
//		dx = px - cx;
//		dy = py - cy;
//		if( line1->type == MAE_HORI_LINE )
//		{
//			//point->lineno1 = line1->label;
//			//point->lineno2 = line2->label;
//			if( dx > 0 && dy > 0)
//			{
//				//          *
//				//		    *
//				//	 	    *
//				//		    *
//				//***********
//
//				//lptnode->line_l = line1 ;
//				//line1->node_r = lptnode;
//				//lptnode->line_d  = line2;
//				//line2->node_u = lptnode;
//				lptnode->line_l = line1 ;
//				line1->node_r = lptnode;
//				lptnode->line_u  = line2;
//				line2->node_d = lptnode;
//			}
//			if( dx < 0 && dy > 0)
//			{
//				//  *
//				//	*
//				//	*
//				//	*
//				//  ***********
//				//lptnode->line_r = line1 ;
//				//line1->node_l = lptnode;
//				//lptnode->line_d  = line2;
//				//line2->node_u = lptnode;
//				lptnode->line_r = line1 ;
//				line1->node_l = lptnode;
//				lptnode->line_u  = line2;
//				line2->node_d = lptnode;
//			}
//			if( dx < 0 && dy < 0)
//			{
//				// ***********
//				// *
//				// *
//				// *
//				// *
//				//lptnode->line_u = line2 ;
//				//line2->node_d = lptnode;
//				//lptnode->line_r  = line1;
//				//line1->node_l = lptnode;
//				lptnode->line_r  = line1;
//				line1->node_l = lptnode;
//				lptnode->line_d = line2 ;
//				line2->node_u = lptnode;
//			}
//			if( dx > 0 && dy < 0)
//			{
//				// ***********
//				//           *
//				//           *
//				//           *
//				//           *
//				//lptnode->line_l = line1 ;
//				//line1->node_r = lptnode;
//				//lptnode->line_u  = line2;
//				//line2->node_d = lptnode;
//				lptnode->line_l = line1 ;
//				line1->node_r = lptnode;
//				lptnode->line_d  = line2;
//				line2->node_u = lptnode;
//			}
//		}else{
//			//point->lineno1 = line2->label;
//			//point->lineno2 = line1->label;
//			if( dx > 0 && dy > 0)
//			{
//				//          *
//				//		    *
//				//	 	    *
//				//		    *
//				//***********
//				//lptnode->line_l = line2 ;
//				//line2->node_r = lptnode;
//				//lptnode->line_d  = line1;
//				//line1->node_u = lptnode;
//				lptnode->line_l = line2 ;
//				line2->node_r = lptnode;
//				lptnode->line_u  = line1;
//				line1->node_d = lptnode;
//			}
//			if( dx < 0 && dy > 0)
//			{
//				//  *
//				//	*
//				//	*
//				//	*
//				//  ***********
//				//lptnode->line_r = line2 ;
//				//line2->node_l = lptnode;
//				//lptnode->line_d  = line1;
//				//line1->node_u = lptnode;
//				lptnode->line_r = line2 ;
//				line2->node_l = lptnode;
//				lptnode->line_u  = line1;
//				line1->node_d = lptnode;
//			}
//			if( dx < 0 && dy < 0)
//			{
//				// ***********
//				// *
//				// *
//				// *
//				// *
//				//lptnode->line_u = line1 ;
//				//line1->node_d = lptnode;
//				//lptnode->line_r  = line2;
//				//line2->node_l = lptnode;
//				lptnode->line_d = line1 ;
//				line1->node_u = lptnode;
//				lptnode->line_r  = line2;
//				line2->node_l = lptnode;
//			}
//			if( dx > 0 && dy < 0)
//			{
//				// ***********
//				//           *
//				//           *
//				//           *
//				//           *
//				//lptnode->line_u = line1 ;
//				//line1->node_d = lptnode;
//				//lptnode->line_l  = line2;
//				//line2->node_r = lptnode;
//				lptnode->line_d = line1 ;
//				line1->node_u = lptnode;
//				lptnode->line_l  = line2;
//				line2->node_r = lptnode;
//			}
//		}
//
//	}
//	return flag;
//}
//

int _mae_get_linelist_from_lines(MAE_line *_h_lines, int _h_num, MAE_line *_v_lines, int _v_num, MAE_line **_linelist, int *_linenum)
{
	int i,j;

	unsigned char *list_data;
	int list_num;
	int idx;

	list_num = 0;
	for( i = 0; i < _h_num; i ++ )
	{
		if( _h_lines[i].label != -1 )
		{
			list_num ++;
		}
	}
	for( i = 0; i < _v_num; i ++ )
	{
		if( _v_lines[i].label != -1 )
		{
			list_num ++;
		}
	}

	if( list_num == 0 )
	{
		return 0;
	}

	_mae_malloc_uchar(&list_data, (list_num ) * sizeof(MAE_line),"_mae_get_regions_from_lines : regions");
	*_linelist = (MAE_line *)list_data;
	*_linenum = list_num ;

	//initialize
	for( i = 0; i < list_num ; i ++ )
	{
		(*_linelist)[i].type = 0;
		(*_linelist)[i].label = 0;
		(*_linelist)[i].sx = 0;
		(*_linelist)[i].ex = 0;
		(*_linelist)[i].sy = 0;
		(*_linelist)[i].ey = 0;
		(*_linelist)[i].l = 0;
		(*_linelist)[i].t = 0;
		(*_linelist)[i].w = 0;
		(*_linelist)[i].h = 0;
		//(*_linelist)[i].node_d  = NULL;
		//(*_linelist)[i].node_u  = NULL;
		//(*_linelist)[i].node_l  = NULL;
		//(*_linelist)[i].node_r  = NULL;

	}
	idx = 0;
	for(i = 0; i < _h_num; i ++ )
	{
		if(_h_lines[i].label == -1 ) continue;

		(*_linelist)[idx].type = _h_lines[i].type;
		(*_linelist)[idx].sx = _h_lines[i].sx;
		(*_linelist)[idx].ex = _h_lines[i].ex;
		(*_linelist)[idx].sy = _h_lines[i].sy;
		(*_linelist)[idx].ey = _h_lines[i].ey;

		(*_linelist)[idx].l = _h_lines[i].l;
		(*_linelist)[idx].t = _h_lines[i].t;
		(*_linelist)[idx].w = _h_lines[i].w;
		(*_linelist)[idx].h = _h_lines[i].h;

		idx ++;

	}
	for(i = 0; i < _v_num; i ++ )
	{
		if(_v_lines[i].label == -1 ) continue;

		(*_linelist)[idx].type = _v_lines[i].type;
		(*_linelist)[idx].sx = _v_lines[i].sx;
		(*_linelist)[idx].ex = _v_lines[i].ex;
		(*_linelist)[idx].sy = _v_lines[i].sy;
		(*_linelist)[idx].ey = _v_lines[i].ey;
		(*_linelist)[idx].l = _v_lines[i].l;
		(*_linelist)[idx].t = _v_lines[i].t;
		(*_linelist)[idx].w = _v_lines[i].w;
		(*_linelist)[idx].h = _v_lines[i].h;

		idx ++;

	}

	return 0;
}


int _mae_get_regions_from_linelist(MAE_line *_linelist, int _line_num, MAE_region **_regions, int *_nregion)
{
	int i,j;

	unsigned char *region_data;
	int region_num;
	int idx;

	region_num = 0;
	for( i = 0; i < _line_num; i ++ )
	{
		if( _linelist[i].label != -1 )
		{
			region_num ++;
		}
	}

	if( region_num == 0 )
	{
		return 0;
	}

	_mae_malloc_uchar(&region_data, (region_num ) * sizeof(MAE_region),"_mae_get_regions_from_lines : regions");
	*_regions = (MAE_region *)region_data;
	*_nregion = region_num ;

	//initialize
	for( i = 0; i < region_num ; i ++ )
	{
		(*_regions)[i].type = 0;
		(*_regions)[i].label = 0;
		(*_regions)[i].l = 0;
		(*_regions)[i].t = 0;
		(*_regions)[i].w = 0;
		(*_regions)[i].h = 0;
	}
	idx = 0;
	for(i = 0; i < _line_num; i ++ )
	{
		if(_linelist[i].label == -1 ) continue;

		(*_regions)[idx].type = _linelist[i].type;
		(*_regions)[idx].l = _linelist[i].l;
		(*_regions)[idx].t = _linelist[i].t;
		(*_regions)[idx].w = _linelist[i].w;
		(*_regions)[idx].h = _linelist[i].h;


		(*_regions)[idx].linenum = 1;
		(*_regions)[idx].lines[0].type = _linelist[i].type;
		(*_regions)[idx].lines[0].sx = _linelist[i].sx;
		(*_regions)[idx].lines[0].sy = _linelist[i].sy;
		(*_regions)[idx].lines[0].ex = _linelist[i].ex;
		(*_regions)[idx].lines[0].ey = _linelist[i].ey;
		(*_regions)[idx].lines[0].lw = _linelist[i].lw;
		(*_regions)[idx].lines[0].l = _linelist[i].l;
		(*_regions)[idx].lines[0].t = _linelist[i].t;
		(*_regions)[idx].lines[0].w = _linelist[i].w;
		(*_regions)[idx].lines[0].h = _linelist[i].h;

		idx ++;

	}


	return 0;
}

void _mae_get_line_max_width(  MAE_line *_v_lines, int _v_num, MAE_line *_h_lines, int _h_num, int *width)
{
	int i,j;
	int w = 0;
	for(i = 0; i < _h_num; i ++ )
	{
		if(_h_lines[i].label == -1 ) continue;

		if( w < _h_lines[i].h )
		{
			w = _h_lines[i].w;
		}

	}
	for(i = 0; i < _v_num; i ++ )
	{
		if(_v_lines[i].label == -1 ) continue;

		if( w < _v_lines[i].h )
		{
			w = _v_lines[i].h;
		}

	}
	*width = w;
}
//
//void _mae_parse_region_form_corners_lines(MAE_line_node **lpthead, int lptnum, MAE_line *linelist, int linenum, MAE_region **rghead)
//{
//	MAE_line_node * pnode,qnode;
//	MAE_region *region;
//	int cornernum;
//	int label = 0;
//	pnode = *lpthead;
//	for(; pnode != NULL; pnode = pnode->link)
//	{
//		if( pnode->label == -1 )
//		{
//			continue;
//		}
//		if( pnode->label != 0 )
//		{
//			continue;
//		}
//		cornernum = 0;
//		if( _mae_check_lptnode(pnode, lpthead, lptnum, &cornernum) )
//		{
//			_mae_malloc_uchar((unsigned char**)(&region), (1) * sizeof(MAE_region),"_mae_parse_region_form_corners_lines : region");
//			region->link = NULL;
//			region->l = 0;
//			region->t = 0;
//			region->w = 0;
//			region->h = 0;
//			region->linenum = 0;
//			region->cornernum = 0;
//			region->corners  = NULL;
//
//			label++;
//			if( _mae_add_corner_to_region(region, pnode, lpthead, lptnum, cornernum, label) == 0 )
//			{
//				if(region->corners != NULL ) 
//				{
//					free(region->corners);
//					region->corners = NULL;
//				}
//				free(region);
//				region = NULL;
//			}else{
//				_mae_add_region(rghead, region);
//				label ++;
//			}
//		}
//	}
//}
//
//int _mae_check_lptnode(MAE_line_node *checknode, MAE_line_node **lpthead, int lptnum, int *connectptnum) 
//{
//	MAE_line_node *prev, *cur;
//	MAE_line_node *start;
//
//	int direct = 0;
//	int count = 0;
//	prev = checknode;
//	start = prev;
//	//if( prev->line_r != NULL )
//	//{
//	//	direct = MAE_TRACE_DIRECT_R;
//	//}else if( prev->line_d != NULL ){
//	//	direct = MAE_TRACE_DIRECT_D;
//	//}else if( prev->line_l != NULL ){
//	//	direct = MAE_TRACE_DIRECT_L;
//	//}else if( prev->line_u != NULL ){
//	//	direct = MAE_TRACE_DIRECT_U;
//	//}else{
//	//	direct = MAE_TRACE_DIRECT_NO;
//	//}
//	//if( prev->line_d != NULL && prev->line_r != NULL )
//	//{
//	//	direct = MAE_TRACE_DIRECT_R;
//	//}else if( prev->line_l != NULL && prev->line_d != NULL ){
//	//	direct = MAE_TRACE_DIRECT_D;
//	//}else if( prev->line_u != NULL && prev->line_l != NULL ){
//	//	direct = MAE_TRACE_DIRECT_L;
//	//}else if( prev->line_r != NULL && prev->line_u != NULL ){
//	//	direct = MAE_TRACE_DIRECT_U;
//	//}else{
//	//	direct = MAE_TRACE_DIRECT_NO;
//	//}
//	//if( direct == MAE_TRACE_DIRECT_NO )
//	//{
//	//	return 0;
//	//}
//
//	if( prev->line_d != NULL && 
//		prev->line_r != NULL &&
//		prev->line_l == NULL &&
//		prev->line_u == NULL 
//		)
//	{
//		direct = MAE_TRACE_DIRECT_R;
//		// ********************
//		// * ----->
//		// *
//		// *
//		// *
//		// *
//
//	}else if( prev->line_l != NULL && 
//			  prev->line_d != NULL &&
//			  prev->line_r == NULL &&
//			  prev->line_u == NULL 
//		){
//		direct = MAE_TRACE_DIRECT_D;
//		// ********************
//		//                    *
//		//                    *
//		//                    *
//		//                    *
//		//                    *
//	}else if( prev->line_u != NULL && 
//		      prev->line_l != NULL &&
//		      prev->line_r == NULL &&
//		      prev->line_d == NULL 
//		){
//		direct = MAE_TRACE_DIRECT_L;
//		//                    *
//		//                    *
//		//                    *
//		//                    *
//		//                    *
//		// ********************
//
//	}else if( prev->line_r != NULL && 
//			  prev->line_u != NULL &&
//			  prev->line_l == NULL &&
//			  prev->line_d == NULL 
//			  ){
//		direct = MAE_TRACE_DIRECT_U;
//		// *
//		// *
//		// *
//		// *
//		// *
//		// ********************
//
//	}else{
//		direct = MAE_TRACE_DIRECT_NO;
//	}
//	if( direct == MAE_TRACE_DIRECT_NO )
//	{
//		return 0;
//	}
//
//	count = 0;
//
//	cur = NULL;
//	while(prev != NULL )
//	{
//
//		switch(direct)
//		{
//		case MAE_TRACE_DIRECT_R:
//				cur = (MAE_line_node *)prev->line_r->node_r;
//				count ++;
//				break;
//		case MAE_TRACE_DIRECT_D:
//				cur = (MAE_line_node *)prev->line_d->node_d;
//				count ++;
//				break;
//		case MAE_TRACE_DIRECT_L:
//				cur = (MAE_line_node *)prev->line_l->node_l;
//				count ++;
//				break;
//		case MAE_TRACE_DIRECT_U:
//				cur = (MAE_line_node *)prev->line_u->node_u;
//				count ++;
//				break;
//			break;
//		}
//		if( cur == NULL )
//		{
//			return 0;
//		}
//
//		if( cur->x == start->x && cur->y == start->y)
//		{
//			*connectptnum = count;
//			return 1;
//		}
//
//		if( count >= lptnum )
//		{
//			return 0;
//		}
//
//		prev = cur;
//		switch(direct)
//		{
//		case MAE_TRACE_DIRECT_R:
//				if( prev->line_d != NULL )
//				{
//					direct = MAE_TRACE_DIRECT_D;
//				}else if( prev->line_r != NULL ){
//					direct = MAE_TRACE_DIRECT_R;
//				}else if( prev->line_u != NULL ){
//					direct = MAE_TRACE_DIRECT_U;
//				}else{
//					direct = MAE_TRACE_DIRECT_NO;
//				}
//			break;
//		case MAE_TRACE_DIRECT_D:
//				if( prev->line_l != NULL )
//				{
//					direct = MAE_TRACE_DIRECT_L;
//				}else if( prev->line_d != NULL ){
//					direct = MAE_TRACE_DIRECT_D;
//				}else if( prev->line_r != NULL ){
//					direct = MAE_TRACE_DIRECT_R;
//				}else{
//					direct = MAE_TRACE_DIRECT_NO;
//				}
//			break;
//		case MAE_TRACE_DIRECT_L:
//				if( prev->line_u != NULL )
//				{
//					direct = MAE_TRACE_DIRECT_U;
//				}else if( prev->line_l != NULL ){
//					direct = MAE_TRACE_DIRECT_L;
//				}else if( prev->line_d != NULL ){
//					direct = MAE_TRACE_DIRECT_D;
//				}else{
//					direct = MAE_TRACE_DIRECT_NO;
//				}
//			break;
//		case MAE_TRACE_DIRECT_U:
//				if( prev->line_r != NULL )
//				{
//					direct = MAE_TRACE_DIRECT_R;
//				}else if( prev->line_u != NULL ){
//					direct = MAE_TRACE_DIRECT_U;
//				}else if( prev->line_l != NULL ){
//					direct = MAE_TRACE_DIRECT_L;
//				}else{
//					direct = MAE_TRACE_DIRECT_NO;
//				}
//			break;
//		}
//
//		if( direct == MAE_TRACE_DIRECT_NO )
//		{
//			return 0;
//		}
//	}
//
//	return 0;
//}
//
//int _mae_add_corner_to_region(MAE_region *region, MAE_line_node *checknode, MAE_line_node **lpthead, int lptnum, int cornernum, int label)
//{
//	MAE_line_node *prev, *cur;
//	MAE_line_node *start;
//	//MAE_line *prevline, *nextline; 
//	unsigned char * cornerdata = NULL;
//	int result = 0;
//
//	int direct = 0;
//	int count = 0;
//	int idx = 0;
//	int corner_x;
//	int corner_y;
//	int dx, dy;
//	int dx1, dy1;
//	int dx2, dy2;
//	int prevdirect;
//	int gap = 1;
//	//int corner_count = 0;
//
//	MAE_line *line1, *line2;
//
//	region->corners = NULL;
//	_mae_malloc_uchar( &cornerdata, (cornernum) * sizeof(MAE_corner),"_mae_add_corner_to_region : MAE_corner");
//	memset(cornerdata, (unsigned char)0, (cornernum) * sizeof(MAE_corner));
//	region->cornernum = cornernum;
//	region->corners = (MAE_corner*)cornerdata;
//
//	prev = checknode;
//	start = prev;
//	//if( prev->line_r != NULL )
//	//{
//	//	direct = MAE_TRACE_DIRECT_R;
//	//}else if( prev->line_d != NULL ){
//	//	direct = MAE_TRACE_DIRECT_D;
//	//}else if( prev->line_l != NULL ){
//	//	direct = MAE_TRACE_DIRECT_L;
//	//}else if( prev->line_u != NULL ){
//	//	direct = MAE_TRACE_DIRECT_U;
//	//}else{
//	//	direct = MAE_TRACE_DIRECT_NO;
//	//}
//	corner_x = prev->x;
//	corner_y = prev->y;
//
//	if( prev->line_d != NULL && 
//		prev->line_r != NULL &&
//		prev->line_l == NULL &&
//		prev->line_u == NULL 
//		)
//	{
//		direct = MAE_TRACE_DIRECT_R;
//		// ********************
//		// * ----->
//		// *
//		// *
//		// *
//		// *
//		line1 = prev->line_d;
//		dx1 = abs(line1->sx - line1->l - gap);
//		dx2 = abs(line1->l + line1->w - line1->sx - gap);
//		dx = min(dx1, dx2);
//		line2 = prev->line_r;
//		dy1 = abs(line2->sy - line2->t - gap);
//		dy2 = abs(line2->t + line2->h - line2->sy - gap);
//		dy = min(dy1, dy2);
//		corner_x = corner_x - dx;
//		corner_y = corner_y - dy;
//		//if( corner_x > prev->line_d->l ) 
//		//	corner_x = prev->line_d->l;
//		//if( corner_y > prev->line_d->t ) 
//		//	corner_y = prev->line_d->t;
//		//if( corner_x > prev->line_r->l ) 
//		//	corner_x = prev->line_r->l;
//		//if( corner_y > prev->line_r->t ) 
//		//	corner_y = prev->line_r->t;
//	}else if( prev->line_l != NULL && 
//			  prev->line_d != NULL &&
//			  prev->line_r == NULL &&
//			  prev->line_u == NULL 
//		){
//		direct = MAE_TRACE_DIRECT_D;
//		// ********************
//		//                    *
//		//                    *
//		//                    *
//		//                    *
//		//                    *
//		//dx = abs(prev->line_d->l + prev->line_d->w - prev->line_d->sx - 1);
//		//dy = abs(prev->line_l->t - prev->line_l->sy);
//		line1 = prev->line_d;
//		dx1 = abs(line1->sx - line1->l - gap);
//		dx2 = abs(line1->l + line1->w - line1->sx - gap);
//		dx = min(dx1, dx2);
//		line2 = prev->line_l;
//		dy1 = abs(line2->sy - line2->t - gap);
//		dy2 = abs(line2->t + line2->h - line2->sy - gap);
//		dy = min(dy1, dy2);
//		corner_x = corner_x + dx;
//		corner_y = corner_y - dy;
//		//if( corner_x < prev->line_l->l + prev->line_l->w )
//		//	corner_x = prev->line_l->l + prev->line_l->w - 1;
//		//if( corner_y > prev->line_l->t ) 
//		//	corner_y = prev->line_l->t;
//		//if( corner_x < prev->line_d->l + prev->line_d->w ) 
//		//	corner_x = prev->line_d->l + prev->line_d->w - 1;
//		//if( corner_y > prev->line_d->t )
//		//	corner_y = prev->line_d->t;
//	}else if( prev->line_u != NULL && 
//		      prev->line_l != NULL &&
//		      prev->line_r == NULL &&
//		      prev->line_d == NULL 
//		){
//		direct = MAE_TRACE_DIRECT_L;
//		//                    *
//		//                    *
//		//                    *
//		//                    *
//		//                    *
//		// ********************
//		//dx = abs(prev->line_u->l + prev->line_u->w - prev->line_u->sx - 1);
//		//dy = abs(prev->line_l->t + prev->line_l->h - prev->line_l->sy - 1);
//		line1 = prev->line_u;
//		dx1 = abs(line1->sx - line1->l - gap);
//		dx2 = abs(line1->l + line1->w - line1->sx - gap);
//		dx = min(dx1, dx2);
//		line2 = prev->line_l;
//		dy1 = abs(line2->sy - line2->t - gap);
//		dy2 = abs(line2->t + line2->h - line2->sy - gap);
//		dy = min(dy1, dy2);
//		corner_x = corner_x + dx;
//		corner_y = corner_y + dy;
//		//if( corner_x < prev->line_u->l + prev->line_u->w )
//		//	corner_x = prev->line_u->l + prev->line_u->w - 1;
//		//if( corner_y < prev->line_u->t + prev->line_u->h )
//		//	corner_y = prev->line_u->t + prev->line_u->h - 1;
//		//if( corner_x < prev->line_l->l + prev->line_l->w ) 
//		//	corner_x = prev->line_l->l + prev->line_l->w - 1;
//		//if( corner_y < prev->line_l->t + prev->line_l->h ) 
//		//	corner_y = prev->line_l->t + prev->line_l->h - 1;
//	}else if( prev->line_r != NULL && 
//			  prev->line_u != NULL &&
//			  prev->line_l == NULL &&
//			  prev->line_d == NULL 
//			  ){
//		direct = MAE_TRACE_DIRECT_U;
//		// *
//		// *
//		// *
//		// *
//		// *
//		// ********************
//		//dx = abs(prev->line_u->sx - prev->line_u->l);
//		//dy = abs(prev->line_r->t + prev->line_l->h - prev->line_l->sy - 1);
//		line1 = prev->line_u;
//		dx1 = abs(line1->sx - line1->l - gap);
//		dx2 = abs(line1->l + line1->w - line1->sx - gap);
//		dx = min(dx1, dx2);
//		line2 = prev->line_r;
//		dy1 = abs(line2->sy - line2->t - gap);
//		dy2 = abs(line2->t + line2->h - line2->sy - gap);
//		dy = min(dy1, dy2);
//		corner_x = corner_x - dx;
//		corner_y = corner_y + dy;
//		//if( corner_x > prev->line_r->l)
//		//	corner_x = prev->line_r->l;
//		//if( corner_y < prev->line_r->t + prev->line_r->h )
//		//	corner_y = prev->line_r->t + prev->line_r->h - 1;
//		//if( corner_x > prev->line_u->l) 
//		//	corner_x = prev->line_u->l;
//		//if( corner_y < prev->line_u->t + prev->line_u->h ) 
//		//	corner_y = prev->line_u->t + prev->line_u->h - 1;
//	}else{
//		direct = MAE_TRACE_DIRECT_NO;
//	}
//	if( direct == MAE_TRACE_DIRECT_NO )
//	{
//		return 0;
//	}
//
//	count = 0;
//
//	cur = NULL;
//	//add corner into region start
//	prev->label = label;
//
////	corner_x = prev->x;
////	corner_y = prev->y;
//	//_mae_get_corner_point(prev, direct, &corner_x, &corner_y);
//	region->corners[idx].x = corner_x;
//	region->corners[idx].y = corner_y;
//	idx ++;
//	//add corner into region end
//	while(prev != NULL )
//	{
//
//		switch(direct)
//		{
//		case MAE_TRACE_DIRECT_R:
//				cur = (MAE_line_node *)prev->line_r->node_r;
//				count ++;
//				break;
//		case MAE_TRACE_DIRECT_D:
//				cur = (MAE_line_node *)prev->line_d->node_d;
//				count ++;
//				break;
//		case MAE_TRACE_DIRECT_L:
//				cur = (MAE_line_node *)prev->line_l->node_l;
//				count ++;
//				break;
//		case MAE_TRACE_DIRECT_U:
//				cur = (MAE_line_node *)prev->line_u->node_u;
//				count ++;
//				break;
//		}
//		if( cur == NULL )
//		{
//			break;
//		}
//
//		if( cur->x == start->x && cur->y == start->y)
//		{
//			result = 1;
//			region->cornernum = idx;
//
//			break;
//		}
//
//		if( count >= lptnum )
//		{
//			break;;
//		}
//
//		prev = cur;
//		//add corner into region start
//		if( idx >= cornernum )
//		{
//			break;
//		}
//		prev->label = label;
//
//		//region->corners[idx].x = prev->x;
//		//region->corners[idx].y = prev->y;
//		//corner_x = prev->x;
//		//corner_y = prev->y;
//		//_mae_get_corner_point(prev, direct, &corner_x, &corner_y);
//		//region->corners[idx].x = corner_x;
//		//region->corners[idx].y = corner_y;
//		//idx ++;
//		//add corner into region end
//		//prevline = nextline;
//		prevdirect = direct;
//		switch(direct)
//		{
//		case MAE_TRACE_DIRECT_R:
//				if( prev->line_d != NULL )
//				{
//					direct = MAE_TRACE_DIRECT_D;
//					//nextline = prev->line_d;
//				}else if( prev->line_r != NULL ){
//					direct = MAE_TRACE_DIRECT_R;
//					//nextline = prev->line_r;
//				}else if( prev->line_u != NULL ){
//					direct = MAE_TRACE_DIRECT_U;
//					//nextline = prev->line_u;
//				}else{
//					direct = MAE_TRACE_DIRECT_NO;
//				}
//			break;
//		case MAE_TRACE_DIRECT_D:
//				if( prev->line_l != NULL )
//				{
//					direct = MAE_TRACE_DIRECT_L;
//					//nextline = prev->line_l;
//				}else if( prev->line_d != NULL ){
//					direct = MAE_TRACE_DIRECT_D;
//					//nextline = prev->line_d;
//				}else if( prev->line_r != NULL ){
//					direct = MAE_TRACE_DIRECT_R;
//					//nextline = prev->line_r;
//				}else{
//					direct = MAE_TRACE_DIRECT_NO;
//				}
//			break;
//		case MAE_TRACE_DIRECT_L:
//				if( prev->line_u != NULL )
//				{
//					direct = MAE_TRACE_DIRECT_U;
//					//nextline = prev->line_u;
//				}else if( prev->line_l != NULL ){
//					direct = MAE_TRACE_DIRECT_L;
//					//nextline = prev->line_l;
//				}else if( prev->line_d != NULL ){
//					direct = MAE_TRACE_DIRECT_D;
//					//nextline = prev->line_d;
//				}else{
//					direct = MAE_TRACE_DIRECT_NO;
//				}
//			break;
//		case MAE_TRACE_DIRECT_U:
//				if( prev->line_r != NULL )
//				{
//					direct = MAE_TRACE_DIRECT_R;
//					//nextline = prev->line_r;
//				}else if( prev->line_u != NULL ){
//					direct = MAE_TRACE_DIRECT_U;
//					//nextline = prev->line_u;
//				}else if( prev->line_l != NULL ){
//					direct = MAE_TRACE_DIRECT_L;
//					//nextline = prev->line_l;
//				}else{
//					direct = MAE_TRACE_DIRECT_NO;
//				}
//			break;
//		}
//
//		corner_x = prev->x;
//		corner_y = prev->y;
//		_mae_get_corner_point(prev, prevdirect, direct, &corner_x, &corner_y);
//		region->corners[idx].x = corner_x;
//		region->corners[idx].y = corner_y;
//		idx ++;
//
//		if( direct == MAE_TRACE_DIRECT_NO )
//		{
//			break;
//		}
//	}
//
//	return result;
//}


int _mae_get_regions_from_regionlist(MAE_region *head, MAE_region **_regions, int *_nregion)
{
	int i,j;

	unsigned char *region_data;
	int region_num;
	int idx;
	MAE_region *p;

	region_num = 0;
	p = head;
	while(p != NULL )
	{
		region_num ++;
		p = p->link;
	}

	if( region_num == 0 )
	{
		return 0;
	}

	_mae_malloc_uchar(&region_data, (region_num ) * sizeof(MAE_region),"_mae_get_regions_from_lines : regions");
	*_regions = (MAE_region *)region_data;
	*_nregion = region_num ;

	idx = 0;
	p = head;
	for(; p != NULL; p = p->link )
	{
		(*_regions)[idx].type = p->type;
		(*_regions)[idx].l = p->l;
		(*_regions)[idx].t = p->t;
		(*_regions)[idx].w = p->w;
		(*_regions)[idx].h = p->h;

		(*_regions)[idx].cornernum = p->cornernum;
		(*_regions)[idx].corners = p->corners;
		//for(i = 0; i < p->cornernum; i ++ )
		//{
		//	(*_regions)[idx].corners[i].x  = p->corners[i].x;
		//	(*_regions)[idx].corners[i].y  = p->corners[i].y;
		//}
		(*_regions)[idx].linenum = 0;

		//(*_regions)[idx].cornernum  = p->cornernum;
		//_mae_malloc_uchar( (unsigned char**)(&(*_regions)[idx].corners), p->cornernum * sizeof(MAE_corner),"_mae_get_regions_from_regionlist : MAE_corner");
		//for(i = 0; i < (*_regions)[idx].cornernum; i ++ )
		//{
		//	(*_regions)[idx].corners[i].x = p->corners[i].x;
		//	(*_regions)[idx].corners[i].y = p->corners[i].y;
		//}

		//free( p->corners); 
		idx ++;
	}


	return 0;
}



void _mae_calculate_lines_v_h_center_line(MAE_line *h_lines, int h_num, MAE_line *v_lines, int v_num, int maxw, int width, int height)
{
	MAE_line *line;
	int i;
	int count;
	int dx,dy;

	count = 0;

	for(i = 0; i < h_num; i ++ )
	{
		line = &h_lines[i];
		if( line->label == -1 )
		{
			continue;
		}
		//dx = line->h * 3 / 4;
		dx = 0;
		line->type = MAE_HORI_LINE;
		line->sx = line->l - dx;
		if(line->sx < 0 ) line->sx = 0;
		line->sy = line->t + line->h / 2;
		line->ex = line->l + line->w + dx;
		if(line->ex >= width) line->ex = width - 1;
		line->ey = line->t + line->h / 2;
		line->lw = line->h;
		//20101124add
		line->l = line->sx;
		line->w = line->ex - line->sx;
		//
	}
	for(i = 0; i < v_num; i ++ )
	{
		line = &v_lines[i];
		if( line->label == -1 )
		{
			continue;
		}
		//dy =  line->w * 3 / 4;
		dy = 0;
		line->type = MAE_VERT_LINE;
		line->sx = line->l +  line->w / 2;
		line->sy = line->t - dy;
		if(line->sy < 0 ) line->sy = 0;
		line->ex = line->l + line->w / 2;
		line->ey = line->t + line->h +  dy;
		if(line->ey >= height ) line->ey = height - 1;
		line->lw = line->w;
		//20101124add
		line->t = line->sy;
		line->h = line->ey - line->sy;
		//
	}
}
void _mae_sort_hlines(MAE_line *h_lines, int h_num)
{
	MAE_line line;
	MAE_line * line1;
	MAE_line * line2;
	int i, j;
	int swap;
	for(i = 0; i < h_num; i ++ )
	{
		for(j = i+1; j < h_num; j ++ )
		{
			line1 = &(h_lines[i]);
			line2 = &(h_lines[j]);
			swap = 0;
			if( line1->sy > line2->sy)
			{
				swap = 1;
			}else if( line1->sy == line2->sy){
				if(line1->sx > line2->sx )
				{
					swap = 1;
				}
			}
			if(swap == 1 )
			{
				line.type = line1->type;
				line.label = line1->label;
				line.sx = line1->sx;
				line.sy = line1->sy;
				line.ex = line1->ex;
				line.ey = line1->ey;
				line.lw = line1->lw;
				line.l = line1->l;
				line.t = line1->t;
				line.w = line1->w;
				line.h = line1->h;

				line1->type = line2->type;
				line1->label = line2->label;
				line1->sx = line2->sx;
				line1->sy = line2->sy;
				line1->ex = line2->ex;
				line1->ey = line2->ey;
				line1->lw = line2->lw;
				line1->l = line2->l;
				line1->t = line2->t;
				line1->w = line2->w;
				line1->h = line2->h;

				line2->type = line.type;
				line2->label = line.label;
				line2->sx = line.sx;
				line2->sy = line.sy;
				line2->ex = line.ex;
				line2->ey = line.ey;
				line2->lw = line.lw;
				line2->l = line.l;
				line2->t = line.t;
				line2->w = line.w;
				line2->h = line.h;

			}
		}
	}
}

void _mae_sort_vlines(MAE_line *v_lines, int v_num)
{
	MAE_line line;
	MAE_line * line1;
	MAE_line * line2;
	int i, j;
	int swap;
	for(i = 0; i < v_num; i ++ )
	{
		for(j = i+1; j < v_num; j ++ )
		{
			line1 = &(v_lines[i]);
			line2 = &(v_lines[j]);
			swap = 0;
			if( line1->sx > line2->sx)
			{
				swap = 1;
			}else if( line1->sx == line2->sx){
				if(line1->sy > line2->sy )
				{
					swap = 1;
				}
			}
			if(swap == 1 )
			{
				line.type = line1->type;
				line.label = line1->label;
				line.sx = line1->sx;
				line.sy = line1->sy;
				line.ex = line1->ex;
				line.ey = line1->ey;
				line.lw = line1->lw;
				line.l = line1->l;
				line.t = line1->t;
				line.w = line1->w;
				line.h = line1->h;

				line1->type = line2->type;
				line1->label = line2->label;
				line1->sx = line2->sx;
				line1->sy = line2->sy;
				line1->ex = line2->ex;
				line1->ey = line2->ey;
				line1->lw = line2->lw;
				line1->l = line2->l;
				line1->t = line2->t;
				line1->w = line2->w;
				line1->h = line2->h;

				line2->type = line.type;
				line2->label = line.label;
				line2->sx = line.sx;
				line2->sy = line.sy;
				line2->ex = line.ex;
				line2->ey = line.ey;
				line2->lw = line.lw;
				line2->l = line.l;
				line2->t = line.t;
				line2->w = line.w;
				line2->h = line.h;

			}
		}
	}
}

void _mae_sort_lines(MAE_line *lines, int num)
{
	MAE_line line;
	MAE_line * line1;
	MAE_line * line2;
	int i, j;
	int swap;
	for(i = 0; i < num; i ++ )
	{
		for(j = i+1; j < num; j ++ )
		{
			line1 = &(lines[i]);
			line2 = &(lines[j]);
			swap = 0;
			if( line1->sy > line2->sy)
			{
				swap = 1;
			}else if( line1->sy == line2->sy){
				if(line1->sx > line2->sx )
				{
					swap = 1;
				}
			}
			if(swap == 1 )
			{
				line.type = line1->type;
				line.label = line1->label;
				line.sx = line1->sx;
				line.sy = line1->sy;
				line.ex = line1->ex;
				line.ey = line1->ey;
				line.lw = line1->lw;
				line.l = line1->l;
				line.t = line1->t;
				line.w = line1->w;
				line.h = line1->h;

				line1->type = line2->type;
				line1->label = line2->label;
				line1->sx = line2->sx;
				line1->sy = line2->sy;
				line1->ex = line2->ex;
				line1->ey = line2->ey;
				line1->lw = line2->lw;
				line1->l = line2->l;
				line1->t = line2->t;
				line1->w = line2->w;
				line1->h = line2->h;

				line2->type = line.type;
				line2->label = line.label;
				line2->sx = line.sx;
				line2->sy = line.sy;
				line2->ex = line.ex;
				line2->ey = line.ey;
				line2->lw = line.lw;
				line2->l = line.l;
				line2->t = line.t;
				line2->w = line.w;
				line2->h = line.h;

			}
		}
	}
}
int distance(int x1,int y1,int x2,int y2)
{
	int dist;
	//dist = (int)sqrt((double)((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)));
	dist = max(abs(x2-x1),abs(y2-y1));
	return dist;
}
//
//void _mae_adjust_corners(MAE_line_node **lpthead, int *lptNum , int maxlinewidth)
//{
//	MAE_line_node *p, *q;
//	int dist;
//	int threshold;
//	threshold = maxlinewidth * 3 / 2;
//	p = *lpthead;
//	for(; p != NULL; p = p ->link )
//	{
//		if(p->label == -1 ) continue;
//		q = *lpthead;
//		for(; q != NULL; q = q->link )
//		{
//			if(q->label == -1 ) continue;
//			if(q == p ) continue;
//			dist = distance(p->x ,p->y,q->x ,q->y);
//			if(dist < threshold)
//			{
//				//if(p->line_l == NULL && q->line_l != NULL ) 
//				if(q->line_l != NULL ) 
//				{
//					p->line_l = q->line_l;
//					p->line_l->node_r = p;
//				}
//				//if(p->line_r == NULL  && q->line_r != NULL ) 
//				if(q->line_r != NULL ) 
//				{
//					p->line_r = q->line_r;
//					p->line_r->node_l = p;
//				}
//				//if(p->line_u == NULL  && q->line_u != NULL) 
//				if(q->line_u != NULL) 
//				{
//					p->line_u = q->line_u;
//					p->line_u->node_d = p;
//				}
//				//if(p->line_d == NULL  && q->line_d != NULL) 
//				if(q->line_d != NULL) 
//				{
//					p->line_d = q->line_d;
//					p->line_d->node_u = p;
//				}
//				q->label = -1;
//				q->line_l = NULL; 
//				q->line_r = NULL; 
//				q->line_u = NULL; 
//				q->line_d = NULL; 
//			}
//		}
//	}
//}
//
//void _mae_get_corner_point(MAE_line_node *node, int prevdirect, int direct, int *_x_, int *_y_)
//{
//	int x, y;
//	MAE_line *line;
//	int dx, dy;
//	int dx1, dy1;
//	int dx2, dy2;
//	int gap;
//	gap = 1;
//	x = *_x_;
//	y = *_y_;
//	switch(prevdirect)
//	{
//	case MAE_TRACE_DIRECT_R:
//			switch(direct)
//			{
//			case MAE_TRACE_DIRECT_D:
//				//*****************
//				//   ---->        *
//				//                *
//				//                *
//				//                *
//				//                *
//				dx = 0;
//				dy = 0;
//				line = node->line_l;
//				if(line != NULL )
//				{
//					dy = abs(line->sy - line->t) - gap;
//					dy1 = abs(line->t + line->h - line->sy) - gap;
//					dy = min(dy,dy1);
//				}
//				line = node->line_d;
//				if(line != NULL )
//				{
//					dx = abs(line->l + line->w - line->sx) - gap;
//					dx1 = abs(line->sx - line->l) - gap;
//					dx = min(dx, dx1);
//				}
//				x = x + dx;
//				y = y - dy;
//				//line = node->line_l;
//				//if(line != NULL )
//				//{
//				//	if( x < line->ex ) x = line->ex;
//				//	if( y > line->t ) y = line->t;
//				//}
//				//line = node->line_d;
//				//if(line != NULL )
//				//{
//				//	if( x < line->l + line->w ) x = line->l + line->w - 1;
//				//	if( y > line->t ) y = line->t;
//				//}
//				break;
//			case MAE_TRACE_DIRECT_R:
//				//*****************  ****************
//				// ---->
//				dx = 0;
//				dy = 0;
//				line = node->line_l;
//				if(line != NULL )
//				{
//					dy1 = abs(line->sy - line->t) - gap;
//					dy = abs(line->t + line->h - line->sy) - gap;
//					dy = min(dy, dy1);
//				}
//				line = node->line_r;
//				if(line != NULL )
//				{
//					dy1 = abs(line->sy - line->t) - gap;
//					dy2 = abs(line->t + line->h - line->sy) - gap;
//					dy1 = min(dy1, dy2);
//					dy = min(dy, dy1);
//				}
//				//if(dy > dy1 ) dy = dy1;
//				y = y - dy;
//				//line = node->line_l;
//				//if(line != NULL )
//				//{
//				//	if( y > line->t ) y = line->t;
//				//}
//				//line = node->line_r;
//				//if(line != NULL )
//				//{
//				//	if( y > line->t ) y = line->t;
//				//}
//				break;
//			case MAE_TRACE_DIRECT_U:
//				//                *
//				//                *
//				//                *
//				//                *
//				//                *
//				//*****************
//				// ------->
//				dx = 0;
//				dy = 0;
//				line = node->line_l;
//				if(line != NULL )
//				{
//					dy1 = abs(line->t + line->h - line->sy - gap);
//					dy2 = abs(line->sy + line->t - gap);
//					dy = min(dy1,dy2);
//				}
//				line = node->line_u;
//				if(line != NULL )
//				{
//					dx1 = abs(line->l + line->w - line->sx - gap);
//					dx2 = abs(line->sx + line->l - gap);
//					dx = min(dx1, dx2);
//				}
//				x = x - dx;
//				y = y - dy;
//				//if(line != NULL )
//				//{
//				//	if( x > line->ex ) x = line->ex;
//				//	if( y > line->t ) y = line->t;
//				//}
//				//line = node->line_u;
//				//if(line != NULL )
//				//{
//				//	if( x > line->l ) x = line->l;
//				//	if( y > line->ey ) y = line->ey;
//				//}
//				break;
//			}
//			break;
//	case MAE_TRACE_DIRECT_D:
//			switch(direct)
//			{
//			case MAE_TRACE_DIRECT_L:
//				//              | *
//				//              | *
//				//              | *
//				//               /*
//				//                *
//				//*****************
//				dx = 0;
//				dy = 0;
//				line = node->line_u;
//				if(line != NULL )
//				{
//					//if( x < line->l + line->w ) x = line->l + line->w - 1;
//					//if( y < line->ey ) y = line->ey;
//					dx1 = abs(line->l + line->w - line->sx - gap);
//					dx2 = abs(line->sx - line->l - gap);
//					dx = min(dx1, dx2);
//				}
//				line = node->line_l;
//				if(line != NULL )
//				{
//					//if( x < line->ex ) x = line->ex;
//					//if( y < line->t + line->h ) y = line->t + line->h - 1;
//					dy1 = abs(line->t + line->h - line->sy - gap);
//					dy2 = abs(line->sy - line->t - gap);
//					dy = min(dy1, dy2);
//				}
//				x = x + dx;
//				y = y + dy;
//				break;
//			case MAE_TRACE_DIRECT_D:
//				//                *
//				//              | *
//				//              | *
//				//              | *
//				//               /*
//
//				//                *
//				//                *
//				//                *
//				//                *
//				//                *
//				dx = 0;
//				dx1 = 0;
//				line = node->line_u;
//				if(line != NULL )
//				{
//					//if( x < line->l + line->w ) x = line->l + line->w - 1;
//					dx1 = abs(line->l + line->w - line->sx - gap);
//					dx2 = abs(line->sx - line->l - gap);
//					dx1 = min(dx1, dx2);
//				}
//				line = node->line_d;
//				if(line != NULL )
//				{
//					//if( x < line->l + line->w ) x = line->l + line->w - 1;
//					dx = abs(line->l + line->w - line->sx - gap);
//					dx2 = abs(line->sx - line->l - gap);
//
//					dx = min(dx, dx2);
//					dx = min(dx, dx1);
//				}
//				x = x + dx;
//				break;
//			case MAE_TRACE_DIRECT_R:
//				// | *
//				// | *
//				// | *
//				// / *
//				//   *
//				//   *****************
//				dx = 0;
//				dy = 0;
//				line = node->line_u;
//				if(line != NULL )
//				{
//					//if( x < line->l + line->w ) x = line->l + line->w - 1;
//					//if( y > line->ey ) y = line->ey;
//					dx1 = abs(line->sx - line->l - gap);
//					dx2 = abs(line->l + line->w  - line->sx - gap);
//					dx = min(dx1, dx2);
//				}
//				line = node->line_r;
//				if(line != NULL )
//				{
//					//if( x < line->sx ) x = line->sx;
//					//if( y > line->t ) y = line->t ;
//					dy1 = abs(line->sy - line->t - gap);
//					dy2 = abs(line->t +line->h - line->sy - gap);
//					dy = min(dy1, dy2);
//				}
//				x = x + dx;
//				y = y - dy;
//				break;
//			}
//		break;
//	case MAE_TRACE_DIRECT_L:
//			switch(direct)
//			{
//			case MAE_TRACE_DIRECT_U:
//				// *
//				// *
//				// *
//				// *
//				//   <--------
//				// *****************
//				dx = 0;
//				dy = 0;
//				line = node->line_r;
//				if(line != NULL )
//				{
//					//if( x > line->l) x = line->l;
//					//if( y < line->t + line->h ) y = line->t + line->h - 1;
//					dy1 = abs(line->t + line->h - line->sy - gap);
//					dy2 = abs(line->sy - line->t - gap);
//					dy = min(dy1, dy2);
//				}
//				line = node->line_u;
//				if(line != NULL )
//				{
//					//if( x > line->l ) x = line->l;
//					//if( y < line->ey ) y = line->ey ;
//					dx1 = (line->sx - line->l - gap);
//					dx2 = (line->l + line->w - line->sx - gap);
//					dx = min(dx1, dx2);
//				}
//				x = x - dx;
//				y = y + dy;
//				break;
//			case MAE_TRACE_DIRECT_L:
//				//              <--------
//				// *********** *****************
//				dy = 0;
//				dy1 = 0;
//				dy2 = 0;
//				line = node->line_r;
//				if(line != NULL )
//				{
//					//if( y < line->t + line->h ) y = line->t + line->h - 1;
//					dy1 = abs(line->t + line->h - line->sy - gap);
//					dy2 = abs(line->sy - line->t - gap);
//					dy1 = min(dy1, dy2);
//				}
//				line = node->line_l;
//				if(line != NULL )
//				{
//					//if( y < line->t + line->h ) y = line->t + line->h - 1 ;
//					dy1 = abs(line->t + line->h - line->sy - gap);
//					dy2 = abs(line->sy - line->t - gap);
//					dy2 = min(dy1, dy2);
//				}
//				dy = min(dy1,dy2);
//				y = y + dy;
//				break;
//			case MAE_TRACE_DIRECT_D:
//				//   <--------
//				// *****************
//				// *
//				// *
//				// *
//				// *
//				dx = 0;
//				dy = 0;
//				line = node->line_r;
//				if(line != NULL )
//				{
//					//if( x > line->l) x = line->l;
//					//if( y < line->t + line->h ) y = line->t + line->h - 1;
//					dy1 = abs(line->sy - line->t - gap);
//					dy2 = abs(line->t + line->h - line->sy - gap);
//					dy = min(dy1,dy2);
//				}
//				line = node->line_d;
//				if(line != NULL )
//				{
//					//if( x > line->l + line->w) x = line->l + line->w;
//					//if( y < line->sy ) y = line->sy ;
//					dx1 = abs(line->sx - line->l - gap);
//					dx2 = abs(line->l + line->w - line->sx - gap);
//					dx = min(dx1, dx2);
//				}
//				x = x + dx;
//				y = y + dy;
//				break;
//			}
//		break;
//	case MAE_TRACE_DIRECT_U:
//			switch(direct)
//			{
//			case MAE_TRACE_DIRECT_R:
//				//  *****************
//				//  */
//				//  *|
//				//  *|
//				//  *|
//				dx = 0;
//				dy = 0;
//				line = node->line_d;
//				if(line != NULL )
//				{
//					//if( x > line->l) x = line->l;
//					//if( y > line->t) y = line->t;
//					dx = abs(line->sx - line->l - gap);
//					dx1 = abs(line->l + line->w - line->sx - gap);
//					dx = min(dx,dx1);
//				}
//				line = node->line_r;
//				if(line != NULL )
//				{
//					//if( x > line->sx ) x = line->sx;
//					//if( y > line->t ) y = line->t ;
//					dy = abs(line->t - line->sy - gap);
//					dy1 = abs(line->t + line->h - line->sy - gap);
//					dy = min(dy,dy1);
//				}
//				x = x - dx;
//				y = y - dy;
//				break;
//			case MAE_TRACE_DIRECT_U:
//				// *
//				// *
//				// *
//				// *
//
//				// */
//				// *|
//				// *|
//				// *|
//				dx = 0;
//				line = node->line_d;
//				if(line != NULL )
//				{
//					//if( x > line->l) x = line->l;
//					dx = abs(line->sx - line->l - gap);
//					dx1 = abs(line->l + line->w - line->sx - gap);
//					dx = min(dx, dx1);
//				}
//				line = node->line_u;
//				if(line != NULL )
//				{
//					dx = abs(line->sx - line->l - gap);
//					dx1 = abs(line->l + line->w - line->sx - gap);
//					dx1 = min(dx, dx1);
//					dx = min(dx, dx1);
//				}
//				x = x - dx;
//				break;
//			case MAE_TRACE_DIRECT_L:
//				//  *****************
//				//					*/
//				//					*|
//				//					*|
//				//					*|
//				dx = 0;
//				dy = 0;
//				line = node->line_d;
//				if(line != NULL )
//				{
//					//if( x > line->l) x = line->l;
//					//if( y < line->t) y = line->t;
//					dx = abs(line->sx - line->l - gap);
//					dx1 = abs(line->l + line->w - line->sx - gap);
//					dx = min(dx, dx1);
//				}
//				line = node->line_l;
//				if(line != NULL )
//				{
//					//if( x > line->ex ) x = line->ex;
//					//if( y < line->t + line->h ) y = line->t + line->h - 1 ;
//					dy = abs(line->t - line->sy - gap);
//					dy1 = abs(line->t + line->h - line->sy - gap);
//					dy = min(dy,dy1);
//				}
//				x = x - dx;
//				y = y + dy;
//				break;
//			}
//		break;
//	}
//	*_x_ = x;
//	*_y_ = y;
//}

void _mae_adjust_regions_corners(MAE_region **_regions, int *_nregion)
{
	int idx;
	int i;
	int corner_num;
	int dx;
	int dy;
	int x,y;
	MAE_corner * corner1;
	MAE_corner * corner2;
	for(idx = 0; idx < *_nregion; idx ++ )
	{
		corner_num = (*_regions)[idx].cornernum;
		for( i = 0; i < corner_num - 1; i ++ )
		{
			corner1  = &((*_regions)[idx].corners[i]);
			corner2  = &((*_regions)[idx].corners[i+1]);
			dx = abs(corner2->x - corner1->x);
			dy = abs(corner2->y - corner1->y);
			if(dx < dy )
			{
				x = (corner1->x + corner2->x) / 2;
				corner1->x = x;
				corner2->x = x;
			}else{
				y = (corner1->y + corner2->y) / 2;
				corner1->y = y;
				corner2->y = y;
			}
		}
		if( corner_num > 0 )
		{
			corner1  = &((*_regions)[idx].corners[0]);
			corner2  = &((*_regions)[idx].corners[corner_num - 1]);
			dx = abs(corner2->x - corner1->x);
			dy = abs(corner2->y - corner1->y);
			if(dx < dy )
			{
				x = (corner1->x + corner2->x) / 2;
				corner1->x = x;
				corner2->x = x;
			}else{
				y = (corner1->y + corner2->y) / 2;
				corner1->y = y;
				corner2->y = y;
			}
		}

	}

}

void _mae_adjust_regions_corners2(MAE_region **_regions, int *_nregion)
{
	int idx;
	int i;
	int corner_num;
	int dx1;
	int dy1;
	int dx2;
	int dy2;
	int x,y;
	MAE_corner * corner1;
	MAE_corner * corner2;
	MAE_corner * corner3;
	for(idx = 0; idx < *_nregion; idx ++ )
	{
		corner_num = (*_regions)[idx].cornernum;
		for( i = 0; i < corner_num - 2; i ++ )
		{
			corner1  = &((*_regions)[idx].corners[i]);
			corner2  = &((*_regions)[idx].corners[i+1]);
			corner3  = &((*_regions)[idx].corners[i+2]);
			dx1 = abs(corner2->x - corner1->x);
			dy1 = abs(corner2->y - corner1->y);
			dx2 = abs(corner3->x - corner2->x);
			dy2 = abs(corner3->y - corner2->y);
			if(dx1 < dy1 && dx2 < dy2 )
			{
				x = (corner1->x + corner2->x + corner3->x) / 3;
				corner1->x = x;
				corner2->x = x;
				corner3->x = x;
			}
			if(dy1 < dx1 && dy2 < dx2 )
			{
				y = (corner1->y + corner2->y + corner3->y) / 3;
				corner1->y = y;
				corner2->y = y;
				corner3->y = y;
			}
		}
		if( corner_num > 3 )
		{
			corner1  = &((*_regions)[idx].corners[corner_num -2]);
			corner2  = &((*_regions)[idx].corners[corner_num -1]);
			corner3  = &((*_regions)[idx].corners[0]);
			dx1 = abs(corner2->x - corner1->x);
			dy1 = abs(corner2->y - corner1->y);
			dx2 = abs(corner3->x - corner2->x);
			dy2 = abs(corner3->y - corner2->y);
			if(dx1 < dy1 && dx2 < dy2 )
			{
				x = (corner1->x + corner2->x + corner3->x) / 3;
				corner1->x = x;
				corner2->x = x;
				corner3->x = x;
			}
			if(dy1 < dx1 && dy2 < dx2 )
			{
				y = (corner1->y + corner2->y + corner3->y) / 3;
				corner1->y = y;
				corner2->y = y;
				corner3->y = y;
			}
		}

	}

}



void _mae_check_vert_line_links(MAE_line **linelist, int *linenum)
{
	int i, j;
	//int count;
	//int left;
	//int top;
	//MAE_line *line;
	int dx;
	MAE_line *line1;
	MAE_line *line2;
	int gap;


	for(i = 0; i < *linenum; i ++ )
	{
		line1 = &((*linelist)[i]);
		if(line1->label == -1) continue;
		for(j = 0; j < *linenum; j ++ )
		{
			if( j == i ) continue;

			line2 = &((*linelist)[j]);
			if(line2->label == -1) continue;

			if( _mae_line_color_is_thesame(line1, line2) == 0 ) continue;

			if( _mae_is_v_v_link(line1, line2) )
			{

				line1->sx = (line1->sx + line2->sx)/2;
				line1->ex = line1->sx;
				line1->sy = min(line1->sy, line2->sy);
				line1->ey = max(line1->ey, line2->ey);

				line1->t = min(line1->t, line2->t);
				if(line1->t < 0 ) line1->t = 0;

				dx = (line1->w + line2->w) / 2;
				line1->l = line1->sx - dx/2;
				if( line1->l < 0 ) line1->l = 0;
				line1->w = dx;
				line1->h = line1->ey - line1->sy;

				line2->label = -1;
			}
		}
	}
	return;
}
int _mae_is_v_v_link(MAE_line *line1, MAE_line *line2)
{

	int dx,dy;
	int gap = 0;
	int dy1,dy2;
	gap = (line1->w + line2->w ) / 2;
	gap = max(gap, 10);
	if( abs(line1->sx - line2->sx) > gap  )
	{
		return 0;
	}

	if( line1->sy < line2->sy )
	{
		dy = abs(line1->ey - line2->sy);
	}else{
		dy = abs(line2->ey - line1->sy);
	}

	if( dy < MAE_MAX_LINE_GAP )
	{

		return 1;
	}

	return 0;
}
void _mae_check_hori_line_links(MAE_line **linelist, int *linenum)
{
	int i, j;

	int dy;
	MAE_line *line1;
	MAE_line *line2;

	for(i = 0; i < *linenum; i ++ )
	{
		line1 = &((*linelist)[i]);
		if(line1->label == -1) continue;
		for(j = 0; j < *linenum; j ++ )
		{
			if( j == i ) continue;

			line2 = &((*linelist)[j]);
			if(line2->label == -1) continue;

			if( _mae_line_color_is_thesame(line1, line2) == 0 ) continue;

			if( _mae_is_h_h_link(line1, line2) )
			{

				line1->sy = (line1->sy + line2->sy)/2;
				line1->ey = line1->sy;
				line1->sx = min(line1->sx, line2->sx);
				line1->ex = max(line1->ex, line2->ex);
				dy = (line1->h + line2->h) / 2;
				line1->t = line1->sy - dy/2;
				line1->l = min(line1->l, line2->l);
				line1->h = dy;
				line1->w = line1->ex - line1->sx;

				line2->label = -1;
			}
		}
	}
	return;
}
int _mae_is_h_h_link(MAE_line *line1, MAE_line *line2)
{

	int dx,dy;
	int gap = 0;
	//int dx1,dx2;

	gap = (line1->h + line2->h ) / 2;
	gap = max(gap, 5);
	if( abs(line1->sy - line2->sy) > gap  )
	{
		return 0;
	}

	if( line1->sx < line2->sx )
	{
		dx = abs(line1->ex - line2->sx);
	}else{
		dx = abs(line2->ex - line1->sx);
	}

	if( dx < MAE_MAX_LINE_GAP )
	{

		return 1;
	}

	return 0;
}


void _mae_detect_corners_from_lines(MAE_line *h_line, int h_num, MAE_line *v_line, int v_num, MAE_line_node **lpthead, int *lptNum )
{
	int i, j;
	int count;
	int left;
	int top;
	MAE_line *line;

	MAE_line *line1;
	MAE_line *line2;
	MAE_line_node *lptnode;	
	//MAE_line_node *pnode = NULL;


	count = 0;
	for(i = 0; i < h_num; i ++ )
	{
		if( h_line[i].label == -1 ) continue;
		line1 = &(h_line[i]);
		for(j = 0; j < v_num; j ++ )
		{
			if( v_line[j].label == -1 ) continue;
			line2 = &(v_line[j]);
			//20110107 add
			if( _mae_line_color_is_thesame2(line1, line2) == 0 ) continue;
			//
			if( _mae_is_h_v_line_intersection2(line1, line2, &lptnode) )
			{
				_mae_add_line_node(lpthead, lptnode);
				lptnode = NULL;
				count++;
			}
		}
	}
	*lptNum = count;
	return;
}
int  _mae_is_h_v_line_intersection(MAE_line *line1, MAE_line *line2, MAE_line_node **pnode )
{
	//int flag;
	int px,py;
	int cx,cy;
	//int dx,dy;
	//int gap = 0;
	//int dx1,dx2, dy1,dy2;
	int d1,d2;
	//int dist;
	MAE_line_node *lptnode;

	//flag = 0;
	px = 0;
	py = 0;

	py = line1->sy;
	px = line2->sx;
	cx = (line1->sx + line1->ex) / 2;
	cy = (line2->sy + line2->ey) / 2;
	//if( line1->h < line2->w)
	//{
	//	gap = line2->w / 2;
	//}else{
	//	gap = line1->h / 2;
	//}
	//dx = min(abs(px - line1->sx), abs(px-line1->ex));
	//dy = min(abs(py - line2->sy), abs(py-line2->ey));
	if( px <= line1->ex && px >= line1->sx ) 
	{
		d1 = 0;
	}else{
		d1 = min(abs(px-line1->ex), abs(px-line1->sx) );
	}
	if( py <= line2->ey && py >= line2->sy ) 
	{
		d2 = 0;
	}else{
		d2 = min(abs(py-line2->ey), abs(py-line2->sy) );
	}

	if(d1 > MAE_MAX_H_V_LINE_GAP || d2 > MAE_MAX_H_V_LINE_GAP )
	{
		return 0;
	}

	_mae_malloc_uchar((unsigned char **)(pnode) ,  1*sizeof(MAE_line_node), "_mae_is_line_intersection : MAE_line_node");
	lptnode = *pnode;
	lptnode->type = 0;
	lptnode->next = NULL;
	lptnode->label = 0;
	lptnode->x = px;
	lptnode->y = py;
	lptnode->l_link = NULL;
	lptnode->r_link = NULL;
	lptnode->d_link = NULL;
	lptnode->u_link = NULL;
	lptnode->h_line = NULL;
	lptnode->v_line = NULL;
	lptnode->h_line = line1;
	lptnode->v_line = line2;

	lptnode->h_link = NULL;
	lptnode->v_link = NULL;
	_mae_add_line_h_link( (MAE_line_node**)(&line1->node), lptnode);
	_mae_add_line_v_link( (MAE_line_node**)(&line2->node), lptnode);

	return 1;
}

int  _mae_is_h_v_line_intersection2(MAE_line *line1, MAE_line *line2, MAE_line_node **pnode )
{
	//int flag;
	int px,py;
	int cx,cy;
	//int dx,dy;
	//int gap = 0;
	//int dx1,dx2, dy1,dy2;
	int d1,d2;
	//int dist;
	MAE_line_node *lptnode;
	int thresh1, thresh2;

	//flag = 0;
	px = 0;
	py = 0;

	py = line1->sy;
	px = line2->sx;
	cx = (line1->sx + line1->ex) / 2;
	cy = (line2->sy + line2->ey) / 2;
	//if( line1->h < line2->w)
	//{
	//	gap = line2->w / 2;
	//}else{
	//	gap = line1->h / 2;
	//}
	//dx = min(abs(px - line1->sx), abs(px-line1->ex));
	//dy = min(abs(py - line2->sy), abs(py-line2->ey));
	if( px <= line1->ex && px >= line1->sx ) 
	{
		d1 = -min(line1->ex - px, px - line1->sx);
	}else{
		d1 = min(abs(px-line1->ex), abs(px-line1->sx) );
	}
	if( py <= line2->ey && py >= line2->sy ) 
	{
		d2 = -min(line2->ey - py, py - line2->sy);
	}else{
		d2 = min(abs(py-line2->ey), abs(py-line2->sy) );
	}

	thresh1 = 10;
	thresh2 = MAE_MAX_H_V_LINE_GAP + (line1->h + line2->w)/2;

	if(d1 > thresh2 || d2 > thresh2 )
	{
		return 0;
	}

	if( (d1 < -thresh1 && d2 > thresh2/2) || (d2 < -thresh1 && d1 > thresh2/2)){
		return 0;
	}

	_mae_malloc_uchar((unsigned char **)(pnode) ,  1*sizeof(MAE_line_node), "_mae_is_line_intersection : MAE_line_node");
	lptnode = *pnode;
	lptnode->type = 0;
	lptnode->next = NULL;
	lptnode->label = 0;
	lptnode->x = px;
	lptnode->y = py;
	lptnode->l_link = NULL;
	lptnode->r_link = NULL;
	lptnode->d_link = NULL;
	lptnode->u_link = NULL;
	lptnode->h_line = NULL;
	lptnode->v_line = NULL;
	lptnode->h_line = line1;
	lptnode->v_line = line2;

	lptnode->h_link = NULL;
	lptnode->v_link = NULL;
	_mae_add_line_h_link( (MAE_line_node**)(&line1->node), lptnode);
	_mae_add_line_v_link( (MAE_line_node**)(&line2->node), lptnode);

	return 1;
}

void _mae_create_line_node_net(unsigned char *data, int width, int height, MAE_line *h_line, int h_num, MAE_line *v_line, int v_num, MAE_line_node **lpthead, int *lptNum )
{
	int i,j, k;
	int count;
	int idx;
	long *nodelist;
	MAE_line_node *pnode;
	MAE_line_node *qnode;
	long tmp;
	for(k = 0; k < h_num; k ++ )
	{
		if( h_line[k].label == -1 ) continue;
		pnode = (MAE_line_node *)h_line[k].node;
		count = 0;
		while(pnode != NULL )
		{
			count ++;
			pnode = pnode->h_link;
		}
		if(count == 0 ) continue;
		_mae_malloc_long( (&nodelist) ,  count, "_mae_create_line_node_net : MAE_line_node");
		idx = 0;
		pnode = (MAE_line_node *)h_line[k].node;
		while(pnode != NULL )
		{
			nodelist[idx++] = (long)pnode;
			pnode = pnode->h_link;
		}

		//sort corner form left to right
		for(i = 0; i < count; i ++ )
		{
			for(j = i+1; j < count; j ++)
			{
				if( ((MAE_line_node *)nodelist[i])->x >  ((MAE_line_node *)nodelist[j])->x )
				{
					tmp = nodelist[i];
					nodelist[i] = nodelist[j];
					nodelist[j] = tmp;
				}
			}
		}

		//set corner hori link
		for(i = 0; i < count - 1; i ++ )
		{
			//20110118 add check if there is a line in middle of linknodes 
			if( _mae_check_linknode_line(data, width, height, (MAE_line_node *)nodelist[i], (MAE_line_node *)nodelist[i+1]) == 1 )
			{
				((MAE_line_node *)nodelist[i])->r_link = ((MAE_line_node *)nodelist[i+1]);
				((MAE_line_node *)nodelist[i+1])->l_link = ((MAE_line_node *)nodelist[i]);
			}
		}

		free(nodelist);
	}

	for(k = 0; k < v_num; k ++ )
	{
		if( v_line[k].label == -1 ) continue;
		pnode = (MAE_line_node *)v_line[k].node;
		count = 0;
		while(pnode != NULL )
		{
			count ++;
			pnode = pnode->v_link;
		}
		if( count == 0 ) continue;
		_mae_malloc_long( (&nodelist) ,  count, "_mae_create_line_node_net : MAE_line_node");
		idx = 0;
		pnode = (MAE_line_node *)v_line[k].node;
		while(pnode != NULL )
		{
			nodelist[idx++] = (long)pnode;
			pnode = pnode->v_link;
		}

		for(i = 0; i < count; i ++ )
		{
			for(j = i+1; j < count; j ++)
			{
				if( ((MAE_line_node *)nodelist[i])->y >  ((MAE_line_node *)nodelist[j])->y )
				{
					tmp = nodelist[i];
					nodelist[i] = nodelist[j];
					nodelist[j] = tmp;
				}
			}
		}
		for(i = 0; i < count - 1; i ++ )
		{
			//20110118 add check if there is a line in middle of linknodes 
			if( _mae_check_linknode_line(data, width, height, (MAE_line_node *)nodelist[i], (MAE_line_node *)nodelist[i+1]) == 1 )
			{
				((MAE_line_node *)nodelist[i])->d_link = ((MAE_line_node *)nodelist[i+1]);
				((MAE_line_node *)nodelist[i+1])->u_link = ((MAE_line_node *)nodelist[i]);
			}
		}	
		free(nodelist);
	}
}
void _mae_remove_alone_line_node(MAE_line_node **lpthead, int *lptNum )
{
	MAE_line_node *pnode;

	pnode = *lpthead;
	while(pnode != NULL )
	{
		if(pnode->l_link == NULL && pnode->r_link == NULL )
		{
			pnode->label = -1;
		}
		if(pnode->u_link == NULL && pnode->u_link == NULL )
		{
			pnode->label = -1;
		}
		pnode = pnode->next;
	}
}


void _mae_parse_region_form_corners_net(MAE_line_node **lpthead, int lptnum, MAE_region **rghead)
{
	MAE_line_node * pnode,qnode;
	MAE_region *region;
	int cornernum;
	int label = 0;
	pnode = *lpthead;
	for(; pnode != NULL; pnode = pnode->next)
	{
		if( pnode->label == -1 )
		{
			continue;
		}
		//20110113 update start for follow shape start
		//
		//        **********************
		//        *                    *
		//        *                    *
		//        *                    *
		//        *                    *
		//        **********************
		//        *                    *
		//        *                    *
		//        *                    *
		//        *                    *
		//        **********************
		//
		if( pnode->label != 0 )
		{
			continue;
		}

		//if(      pnode->label != 0  
		//	 &&  (pnode->l_link != NULL && pnode->l_link->l_link != 0) 
		//	 &&  (pnode->u_link != NULL && pnode->u_link->l_link != 0) 
		//	 &&  (pnode->r_link != NULL && pnode->r_link->l_link != 0) 
		//	 &&  (pnode->d_link != NULL && pnode->d_link->l_link != 0) 
		//	)
		//{
		//	continue;
		//}

		//20110113 update start for follow shape end
		cornernum = 0;
		if( _mae_check_net_node(pnode, lpthead, lptnum, &cornernum) )
		{
			_mae_malloc_uchar((unsigned char**)(&region), (1) * sizeof(MAE_region),"_mae_parse_region_form_corners_lines : region");
			region->link = NULL;
			region->l = 0;
			region->t = 0;
			region->w = 0;
			region->h = 0;
			region->linenum = 0;
			region->cornernum = 0;
			region->corners  = NULL;

			label++;
			if( _mae_add_new_region_to_list(region, pnode, lpthead, lptnum, cornernum, label) == 0 )
			{
				if(region->corners != NULL ) 
				{
					free(region->corners);
					region->corners = NULL;
				}
				free(region);
				region = NULL;
			}else{
				_mae_add_region(rghead, region);
				label ++;
			}
		}
	}
}

int _mae_get_start_direction(MAE_line_node *checknode) 
{
	//20110113 update for suiting follow shape 
	//
	//        **********************
	//        *                    *
	//        *                    *
	//        *                    *
	//        *                    *
	//        **********************
	//        *                    *
	//        *                    *
	//        *                    *
	//        *                    *
	//        **********************
	//

	//        ***********************
	//        *          *          *
	//        *          *          *
	//        *          *          *
	//        *          *          *
	//        ***********************


	MAE_line_node *prev;

	int direct = 0;
	prev = checknode;

	//
	if( prev->d_link != NULL && 
		prev->r_link != NULL &&
		prev->l_link == NULL &&
		prev->u_link == NULL 
		)
	{
		direct = MAE_TRACE_DIRECT_R;
		// ********************
		// * ----->
		// *
		// *
		// *
		// *

	}else if( prev->l_link != NULL && 
		prev->d_link != NULL &&
		prev->r_link == NULL &&
		prev->u_link == NULL 
		){
			direct = MAE_TRACE_DIRECT_D;
			// ********************
			//                    *
			//                    *
			//                    *
			//                    *
			//                    *
	}else if( prev->u_link != NULL && 
		prev->l_link != NULL &&
		prev->r_link == NULL &&
		prev->d_link == NULL 
		){
			direct = MAE_TRACE_DIRECT_L;
			//                    *
			//                    *
			//                    *
			//                    *
			//                    *
			// ********************

	}else if( prev->r_link != NULL && 
		prev->u_link != NULL &&
		prev->l_link == NULL &&
		prev->d_link == NULL 
		){
			direct = MAE_TRACE_DIRECT_U;
			// *
			// *
			// *
			// *
			// *
			// ********************

			//}else if( prev->l_link != NULL && 
			//		  prev->d_link != NULL &&
			//		  prev->r_link != NULL &&
			//		  prev->u_link == NULL 
			//	){
			//	if( prev->l_link ->label == 0 )
			//	{
			//		direct = MAE_TRACE_DIRECT_D;
			//	}else{
			//		direct = MAE_TRACE_DIRECT_R;
			//	}
			//	// ***************************************
			//	//                    *
			//	//                    *
			//	//                    *
			//	//                    *
			//	//                    *
			//}else if( prev->u_link != NULL && 
			//	      prev->l_link != NULL &&
			//	      prev->r_link != NULL &&
			//	      prev->d_link == NULL 
			//	){
			//	direct = MAE_TRACE_DIRECT_L;
			//	if( prev->u_link ->label == 0 && prev->r_link ->label == 0 )
			//	{
			//		direct = MAE_TRACE_DIRECT_u;
			//	}else if( prev->u_link ->label == 0 && prev->l_link ->label == 0 ){
			//		direct = MAE_TRACE_DIRECT_U;
			//	}else if( prev->l_link ->label == 0 && prev->r_link ->label == 0 ){
			//		direct = MAE_TRACE_DIRECT_r;
			//	}
			//	//                    *
			//	//                    *
			//	//                    *
			//	//                    *
			//	//                    *	
			//	// ***************************************
			//}else if( prev->r_link != NULL && 
			//		  prev->u_link != NULL &&
			//		  prev->l_link == NULL &&
			//		  prev->d_link != NULL 
			//		  ){
			//	if( prev->u_link ->label == 0 )
			//	{
			//		direct = MAE_TRACE_DIRECT_U;
			//	}else{
			//		direct = MAE_TRACE_DIRECT_R;
			//	}
			//	// *
			//	// *
			//	// *
			//	// *
			//	// *
			//	// ********************
			//	// *
			//	// *
			//	// *
			//	// *
			//	// *
			//}else if( prev->r_link == NULL && 
			//		  prev->u_link != NULL &&
			//		  prev->l_link != NULL &&
			//		  prev->d_link != NULL 
			//		  ){
			//	if( prev->u_link ->label == 0 )
			//	{
			//		direct = MAE_TRACE_DIRECT_L;
			//	}else{
			//		direct = MAE_TRACE_DIRECT_D;
			//	}
			//	//					  *
			//	//					  *
			//	//					  *
			//	//					  *
			//	//					  *
			//	// ********************
			//	//					  *
			//	//					  *
			//	//					  *
			//	//					  *
			//	//					  *
	}else{
		direct = MAE_TRACE_DIRECT_NO;
	}

	return direct;
}

int _mae_check_net_node(MAE_line_node *checknode, MAE_line_node **lpthead, int lptnum, int *connectptnum) 
{
	MAE_line_node *prev, *cur;
	MAE_line_node *start;

	int direct = 0;
	int count = 0;
	prev = checknode;
	start = prev;


	//
	//20110113 update start for follow shape start
	//if( prev->d_link != NULL && 
	//	prev->r_link != NULL &&
	//	prev->l_link == NULL &&
	//	prev->u_link == NULL 
	//	)
	//{
	//	direct = MAE_TRACE_DIRECT_R;
	//	// ********************
	//	// * ----->
	//	// *
	//	// *
	//	// *
	//	// *

	//}else if( prev->l_link != NULL && 
	//		  prev->d_link != NULL &&
	//		  prev->r_link == NULL &&
	//		  prev->u_link == NULL 
	//	){
	//	direct = MAE_TRACE_DIRECT_D;
	//	// ********************
	//	//                    *
	//	//                    *
	//	//                    *
	//	//                    *
	//	//                    *
	//}else if( prev->u_link != NULL && 
	//	      prev->l_link != NULL &&
	//	      prev->r_link == NULL &&
	//	      prev->d_link == NULL 
	//	){
	//	direct = MAE_TRACE_DIRECT_L;
	//	//                    *
	//	//                    *
	//	//                    *
	//	//                    *
	//	//                    *
	//	// ********************

	//}else if( prev->r_link != NULL && 
	//		  prev->u_link != NULL &&
	//		  prev->l_link == NULL &&
	//		  prev->d_link == NULL 
	//		  ){
	//	direct = MAE_TRACE_DIRECT_U;
	//	// *
	//	// *
	//	// *
	//	// *
	//	// *
	//	// ********************

	//}else{
	//	direct = MAE_TRACE_DIRECT_NO;
	//}

	direct = _mae_get_start_direction(prev);
	//20110113 update start for follow shape end

	if( direct == MAE_TRACE_DIRECT_NO )
	{
		return 0;
	}

	count = 0;

	cur = NULL;
	while(prev != NULL )
	{

		switch(direct)
		{
		case MAE_TRACE_DIRECT_R:
			cur = (MAE_line_node *)prev->r_link;
			count ++;
			break;
		case MAE_TRACE_DIRECT_D:
			cur = (MAE_line_node *)prev->d_link;
			count ++;
			break;
		case MAE_TRACE_DIRECT_L:
			cur = (MAE_line_node *)prev->l_link;
			count ++;
			break;
		case MAE_TRACE_DIRECT_U:
			cur = (MAE_line_node *)prev->u_link;
			count ++;
			break;
			break;
		}
		if( cur == NULL )
		{
			return 0;
		}

		if( cur->x == start->x && cur->y == start->y)
		{
			*connectptnum = count;
			return 1;
		}

		if( count >= lptnum )
		{
			return 0;
		}

		prev = cur;
		switch(direct)
		{
		case MAE_TRACE_DIRECT_R:
			if( prev->d_link != NULL )
			{
				direct = MAE_TRACE_DIRECT_D;
			}else if( prev->r_link != NULL ){
				direct = MAE_TRACE_DIRECT_R;
			}else if( prev->u_link != NULL ){
				direct = MAE_TRACE_DIRECT_U;
			}else{
				direct = MAE_TRACE_DIRECT_NO;
			}
			break;
		case MAE_TRACE_DIRECT_D:
			if( prev->l_link != NULL )
			{
				direct = MAE_TRACE_DIRECT_L;
			}else if( prev->d_link != NULL ){
				direct = MAE_TRACE_DIRECT_D;
			}else if( prev->r_link != NULL ){
				direct = MAE_TRACE_DIRECT_R;
			}else{
				direct = MAE_TRACE_DIRECT_NO;
			}
			break;
		case MAE_TRACE_DIRECT_L:
			if( prev->u_link != NULL )
			{
				direct = MAE_TRACE_DIRECT_U;
			}else if( prev->l_link != NULL ){
				direct = MAE_TRACE_DIRECT_L;
			}else if( prev->d_link != NULL ){
				direct = MAE_TRACE_DIRECT_D;
			}else{
				direct = MAE_TRACE_DIRECT_NO;
			}
			break;
		case MAE_TRACE_DIRECT_U:
			if( prev->r_link != NULL )
			{
				direct = MAE_TRACE_DIRECT_R;
			}else if( prev->u_link != NULL ){
				direct = MAE_TRACE_DIRECT_U;
			}else if( prev->l_link != NULL ){
				direct = MAE_TRACE_DIRECT_L;
			}else{
				direct = MAE_TRACE_DIRECT_NO;
			}
			break;
		}

		if( direct == MAE_TRACE_DIRECT_NO )
		{
			return 0;
		}
	}

	return 0;
}

int _mae_get_start_direction_pos(MAE_line_node *checknode, int *stx, int *sty) 
{
	MAE_line_node *prev;

	int direct = 0;
	int corner_x;
	int corner_y;
	int dx, dy;
	int dx1, dy1;
	int dx2, dy2;
	int gap = 1;
	MAE_line *line1, *line2;

	corner_x = *stx;
	corner_y = *sty;
	prev = checknode;
	//20110113 update start for follow shape start
	if( prev->d_link != NULL && 
		prev->r_link != NULL &&
		prev->l_link == NULL &&
		prev->u_link == NULL 
		)
	{
		direct = MAE_TRACE_DIRECT_R;
		// ********************
		// * ----->
		// *
		// *
		// *
		// *
		//line1 = prev-line_d;
		line1 = prev->v_line;
		dx1 = abs(line1->sx - line1->l - gap);
		dx2 = abs(line1->l + line1->w - line1->sx - gap);
		dx = min(dx1, dx2);
		line2 = prev->h_line;
		dy1 = abs(line2->sy - line2->t - gap);
		dy2 = abs(line2->t + line2->h - line2->sy - gap);
		dy = min(dy1, dy2);
		corner_x = corner_x - dx;
		corner_y = corner_y - dy;
	}else if( prev->l_link != NULL && 
		prev->d_link != NULL &&
		prev->r_link == NULL &&
		prev->u_link == NULL 
		){
			direct = MAE_TRACE_DIRECT_D;
			// ********************
			//                    *
			//                    *
			//                    *
			//                    *
			//                    *
			line1 = prev->v_line;
			dx1 = abs(line1->sx - line1->l - gap);
			dx2 = abs(line1->l + line1->w - line1->sx - gap);
			dx = min(dx1, dx2);
			line2 = prev->h_line;
			dy1 = abs(line2->sy - line2->t - gap);
			dy2 = abs(line2->t + line2->h - line2->sy - gap);
			dy = min(dy1, dy2);
			corner_x = corner_x + dx;
			corner_y = corner_y - dy;
	}else if( prev->u_link != NULL && 
		prev->l_link != NULL &&
		prev->r_link == NULL &&
		prev->d_link == NULL 
		){
			direct = MAE_TRACE_DIRECT_L;
			//                    *
			//                    *
			//                    *
			//                    *
			//                    *
			// ********************
			line1 = prev->v_line;
			dx1 = abs(line1->sx - line1->l - gap);
			dx2 = abs(line1->l + line1->w - line1->sx - gap);
			dx = min(dx1, dx2);
			line2 = prev->h_line;
			dy1 = abs(line2->sy - line2->t - gap);
			dy2 = abs(line2->t + line2->h - line2->sy - gap);
			dy = min(dy1, dy2);
			corner_x = corner_x + dx;
			corner_y = corner_y + dy;
	}else if( prev->r_link != NULL && 
		prev->u_link != NULL &&
		prev->l_link == NULL &&
		prev->d_link == NULL 
		){
			direct = MAE_TRACE_DIRECT_U;
			// *
			// *
			// *
			// *
			// *
			// ********************
			line1 = prev->v_line;
			dx1 = abs(line1->sx - line1->l - gap);
			dx2 = abs(line1->l + line1->w - line1->sx - gap);
			dx = min(dx1, dx2);
			line2 = prev->h_line;
			dy1 = abs(line2->sy - line2->t - gap);
			dy2 = abs(line2->t + line2->h - line2->sy - gap);
			dy = min(dy1, dy2);
			corner_x = corner_x - dx;
			corner_y = corner_y + dy;
			//}else if( prev->l_link != NULL && 
			//		  prev->d_link != NULL &&
			//		  prev->r_link != NULL &&
			//		  prev->u_link == NULL 
			//	){
			//	line1 = prev->v_line;
			//	dx1 = abs(line1->sx - line1->l - gap);
			//	dx2 = abs(line1->l + line1->w - line1->sx - gap);
			//	dx = min(dx1, dx2);
			//	line2 = prev->h_line;
			//	dy1 = abs(line2->sy - line2->t - gap);
			//	dy2 = abs(line2->t + line2->h - line2->sy - gap);
			//	dy = min(dy1, dy2);
			//	if( prev->l_link ->label == 0 )
			//	{
			//		direct = MAE_TRACE_DIRECT_D;
			//		//corner_x = corner_x - dx;
			//		corner_y = corner_y - dy;
			//	}else{
			//		//corner_x = corner_x - dx;
			//		corner_y = corner_y - dy;
			//		direct = MAE_TRACE_DIRECT_R;
			//	}
			//	// ***************************************
			//	//                    *
			//	//                    *
			//	//                    *
			//	//                    *
			//	//                    *
			//}else if( prev->u_link != NULL && 
			//	      prev->l_link != NULL &&
			//	      prev->r_link != NULL &&
			//	      prev->d_link == NULL 
			//	){
			//	line1 = prev->v_line;
			//	dx1 = abs(line1->sx - line1->l - gap);
			//	dx2 = abs(line1->l + line1->w - line1->sx - gap);
			//	dx = min(dx1, dx2);
			//	line2 = prev->h_line;
			//	dy1 = abs(line2->sy - line2->t - gap);
			//	dy2 = abs(line2->t + line2->h - line2->sy - gap);
			//	dy = min(dy1, dy2);
			//	if( prev->u_link ->label == 0 )
			//	{
			//		direct = MAE_TRACE_DIRECT_L;
			//		//corner_x = corner_x - dx;
			//		corner_y = corner_y + dy;
			//	}else{
			//		direct = MAE_TRACE_DIRECT_U;
			//		//corner_x = corner_x - dx;
			//		corner_y = corner_y + dy;
			//	}
			//	//                    *
			//	//                    *
			//	//                    *
			//	//                    *
			//	//                    *	
			//	// ***************************************
			//}else if( prev->r_link != NULL && 
			//		  prev->u_link != NULL &&
			//		  prev->l_link == NULL &&
			//		  prev->d_link != NULL 
			//		  ){
			//	line1 = prev->v_line;
			//	dx1 = abs(line1->sx - line1->l - gap);
			//	dx2 = abs(line1->l + line1->w - line1->sx - gap);
			//	dx = min(dx1, dx2);
			//	line2 = prev->h_line;
			//	dy1 = abs(line2->sy - line2->t - gap);
			//	dy2 = abs(line2->t + line2->h - line2->sy - gap);
			//	dy = min(dy1, dy2);
			//	if( prev->u_link ->label == 0 )
			//	{
			//		direct = MAE_TRACE_DIRECT_U;
			//		corner_x = corner_x - dx;
			//		//corner_y = corner_y + dy;
			//	}else{
			//		direct = MAE_TRACE_DIRECT_R;
			//		corner_x = corner_x - dx;
			//		//corner_y = corner_y + dy;
			//	}
			//	// *
			//	// *
			//	// *
			//	// *
			//	// *
			//	// ********************
			//	// *
			//	// *
			//	// *
			//	// *
			//	// *
			//}else if( prev->r_link == NULL && 
			//		  prev->u_link != NULL &&
			//		  prev->l_link != NULL &&
			//		  prev->d_link != NULL 
			//		  ){
			//	line1 = prev->v_line;
			//	dx1 = abs(line1->sx - line1->l - gap);
			//	dx2 = abs(line1->l + line1->w - line1->sx - gap);
			//	dx = min(dx1, dx2);
			//	line2 = prev->h_line;
			//	dy1 = abs(line2->sy - line2->t - gap);
			//	dy2 = abs(line2->t + line2->h - line2->sy - gap);
			//	dy = min(dy1, dy2);
			//	if( prev->u_link ->label == 0 )
			//	{
			//		direct = MAE_TRACE_DIRECT_L;
			//		corner_x = corner_x + dx;
			//		//corner_y = corner_y + dy;
			//	}else{
			//		direct = MAE_TRACE_DIRECT_D;
			//		corner_x = corner_x + dx;
			//		//corner_y = corner_y + dy;
			//	}
			//	//					  *
			//	//					  *
			//	//					  *
			//	//					  *
			//	//					  *
			//	// ********************
			//	//					  *
			//	//					  *
			//	//					  *
			//	//					  *
			//	//					  *
	}else{
		direct = MAE_TRACE_DIRECT_NO;
	}

	*stx = corner_x;
	*sty = corner_y;

	return direct;

}


int _mae_add_new_region_to_list(MAE_region *region, MAE_line_node *checknode, MAE_line_node **lpthead, int lptnum, int cornernum, int label)
{
	MAE_line_node *prev, *cur;
	MAE_line_node *start;
	unsigned char * cornerdata = NULL;
	int result = 0;

	int direct = 0;
	int count = 0;
	int idx = 0;
	int corner_x;
	int corner_y;
	int dx, dy;
	int dx1, dy1;
	int dx2, dy2;
	int prevdirect;
	int gap = 1;

	MAE_line *line1, *line2;

	region->corners = NULL;
	_mae_malloc_uchar( &cornerdata, (cornernum) * sizeof(MAE_corner),"_mae_add_corner_to_region : MAE_corner");
	memset(cornerdata, (unsigned char)0, (cornernum) * sizeof(MAE_corner));
	region->cornernum = cornernum;
	region->corners = (MAE_corner*)cornerdata;

	prev = checknode;
	start = prev;
	corner_x = prev->x;
	corner_y = prev->y;

	////20110113 update start for follow shape start
	//if( prev->d_link != NULL && 
	//	prev->r_link != NULL &&
	//	prev->l_link == NULL &&
	//	prev->u_link == NULL 
	//	)
	//{
	//	direct = MAE_TRACE_DIRECT_R;
	//	// ********************
	//	// * ----->
	//	// *
	//	// *
	//	// *
	//	// *
	//	//line1 = prev-line_d;
	//	line1 = prev->v_line;
	//	dx1 = abs(line1->sx - line1->l - gap);
	//	dx2 = abs(line1->l + line1->w - line1->sx - gap);
	//	dx = min(dx1, dx2);
	//	//line2 = prev->line_r;
	//	line2 = prev->h_line;
	//	dy1 = abs(line2->sy - line2->t - gap);
	//	dy2 = abs(line2->t + line2->h - line2->sy - gap);
	//	dy = min(dy1, dy2);
	//	corner_x = corner_x - dx;
	//	corner_y = corner_y - dy;
	//	//if( corner_x > prev->line_d->l ) 
	//	//	corner_x = prev->line_d->l;
	//	//if( corner_y > prev->line_d->t ) 
	//	//	corner_y = prev->line_d->t;
	//	//if( corner_x > prev->line_r->l ) 
	//	//	corner_x = prev->line_r->l;
	//	//if( corner_y > prev->line_r->t ) 
	//	//	corner_y = prev->line_r->t;
	//}else if( prev->l_link != NULL && 
	//		  prev->d_link != NULL &&
	//		  prev->r_link == NULL &&
	//		  prev->u_link == NULL 
	//	){
	//	direct = MAE_TRACE_DIRECT_D;
	//	// ********************
	//	//                    *
	//	//                    *
	//	//                    *
	//	//                    *
	//	//                    *
	//	//dx = abs(prev->line_d->l + prev->line_d->w - prev->line_d->sx - 1);
	//	//dy = abs(prev->line_l->t - prev->line_l->sy);
	//	//line1 = prev->line_d;
	//	line1 = prev->v_line;
	//	dx1 = abs(line1->sx - line1->l - gap);
	//	dx2 = abs(line1->l + line1->w - line1->sx - gap);
	//	dx = min(dx1, dx2);
	//	//line2 = prev->line_l;
	//	line2 = prev->h_line;
	//	dy1 = abs(line2->sy - line2->t - gap);
	//	dy2 = abs(line2->t + line2->h - line2->sy - gap);
	//	dy = min(dy1, dy2);
	//	corner_x = corner_x + dx;
	//	corner_y = corner_y - dy;
	//	//if( corner_x < prev->line_l->l + prev->line_l->w )
	//	//	corner_x = prev->line_l->l + prev->line_l->w - 1;
	//	//if( corner_y > prev->line_l->t ) 
	//	//	corner_y = prev->line_l->t;
	//	//if( corner_x < prev->line_d->l + prev->line_d->w ) 
	//	//	corner_x = prev->line_d->l + prev->line_d->w - 1;
	//	//if( corner_y > prev->line_d->t )
	//	//	corner_y = prev->line_d->t;
	//}else if( prev->u_link != NULL && 
	//	      prev->l_link != NULL &&
	//	      prev->r_link == NULL &&
	//	      prev->d_link == NULL 
	//	){
	//	direct = MAE_TRACE_DIRECT_L;
	//	//                    *
	//	//                    *
	//	//                    *
	//	//                    *
	//	//                    *
	//	// ********************
	//	//dx = abs(prev->line_u->l + prev->line_u->w - prev->line_u->sx - 1);
	//	//dy = abs(prev->line_l->t + prev->line_l->h - prev->line_l->sy - 1);
	//	//line1 = prev->line_u;
	//	line1 = prev->v_line;
	//	dx1 = abs(line1->sx - line1->l - gap);
	//	dx2 = abs(line1->l + line1->w - line1->sx - gap);
	//	dx = min(dx1, dx2);
	//	//line2 = prev->line_l;
	//	line2 = prev->v_line;
	//	dy1 = abs(line2->sy - line2->t - gap);
	//	dy2 = abs(line2->t + line2->h - line2->sy - gap);
	//	dy = min(dy1, dy2);
	//	corner_x = corner_x + dx;
	//	corner_y = corner_y + dy;
	//	//if( corner_x < prev->line_u->l + prev->line_u->w )
	//	//	corner_x = prev->line_u->l + prev->line_u->w - 1;
	//	//if( corner_y < prev->line_u->t + prev->line_u->h )
	//	//	corner_y = prev->line_u->t + prev->line_u->h - 1;
	//	//if( corner_x < prev->line_l->l + prev->line_l->w ) 
	//	//	corner_x = prev->line_l->l + prev->line_l->w - 1;
	//	//if( corner_y < prev->line_l->t + prev->line_l->h ) 
	//	//	corner_y = prev->line_l->t + prev->line_l->h - 1;
	//}else if( prev->r_link != NULL && 
	//		  prev->u_link != NULL &&
	//		  prev->l_link == NULL &&
	//		  prev->d_link == NULL 
	//		  ){
	//	direct = MAE_TRACE_DIRECT_U;
	//	// *
	//	// *
	//	// *
	//	// *
	//	// *
	//	// ********************
	//	//dx = abs(prev->line_u->sx - prev->line_u->l);
	//	//dy = abs(prev->line_r->t + prev->line_l->h - prev->line_l->sy - 1);
	//	//line1 = prev->line_u;
	//	line1 = prev->v_line;
	//	dx1 = abs(line1->sx - line1->l - gap);
	//	dx2 = abs(line1->l + line1->w - line1->sx - gap);
	//	dx = min(dx1, dx2);
	//	//line2 = prev->line_r;
	//	line2 = prev->h_line;
	//	dy1 = abs(line2->sy - line2->t - gap);
	//	dy2 = abs(line2->t + line2->h - line2->sy - gap);
	//	dy = min(dy1, dy2);
	//	corner_x = corner_x - dx;
	//	corner_y = corner_y + dy;
	//	//if( corner_x > prev->line_r->l)
	//	//	corner_x = prev->line_r->l;
	//	//if( corner_y < prev->line_r->t + prev->line_r->h )
	//	//	corner_y = prev->line_r->t + prev->line_r->h - 1;
	//	//if( corner_x > prev->line_u->l) 
	//	//	corner_x = prev->line_u->l;
	//	//if( corner_y < prev->line_u->t + prev->line_u->h ) 
	//	//	corner_y = prev->line_u->t + prev->line_u->h - 1;
	//}else{
	//	direct = MAE_TRACE_DIRECT_NO;
	//}

	direct = _mae_get_start_direction_pos(prev, &corner_x, &corner_y);
	////20110113 update start for follow shape end

	if( direct == MAE_TRACE_DIRECT_NO )
	{
		return 0;
	}

	count = 0;

	cur = NULL;
	//add corner into region start
	prev->label = label;

	//	corner_x = prev->x;
	//	corner_y = prev->y;
	//_mae_get_corner_point(prev, direct, &corner_x, &corner_y);
	region->corners[idx].x = corner_x;
	region->corners[idx].y = corner_y;
	idx ++;
	//add corner into region end
	while(prev != NULL )
	{

		switch(direct)
		{
		case MAE_TRACE_DIRECT_R:
			cur = (MAE_line_node *)prev->r_link;
			count ++;
			break;
		case MAE_TRACE_DIRECT_D:
			cur = (MAE_line_node *)prev->d_link;
			count ++;
			break;
		case MAE_TRACE_DIRECT_L:
			cur = (MAE_line_node *)prev->l_link;
			count ++;
			break;
		case MAE_TRACE_DIRECT_U:
			cur = (MAE_line_node *)prev->u_link;
			count ++;
			break;
		}
		if( cur == NULL )
		{
			break;
		}

		if( cur->x == start->x && cur->y == start->y)
		{
			result = 1;
			region->cornernum = idx;

			break;
		}

		if( count >= lptnum )
		{
			break;;
		}

		prev = cur;
		//add corner into region start
		if( idx >= cornernum )
		{
			break;
		}
		prev->label = label;

		//region->corners[idx].x = prev->x;
		//region->corners[idx].y = prev->y;
		//corner_x = prev->x;
		//corner_y = prev->y;
		//_mae_get_corner_point(prev, direct, &corner_x, &corner_y);
		//region->corners[idx].x = corner_x;
		//region->corners[idx].y = corner_y;
		//idx ++;
		//add corner into region end
		//prevline = nextline;
		prevdirect = direct;
		switch(direct)
		{
		case MAE_TRACE_DIRECT_R:
			if( prev->d_link != NULL )
			{
				direct = MAE_TRACE_DIRECT_D;
				//nextline = prev->line_d;
			}else if( prev->r_link != NULL ){
				direct = MAE_TRACE_DIRECT_R;
				//nextline = prev->line_r;
			}else if( prev->u_link != NULL ){
				direct = MAE_TRACE_DIRECT_U;
				//nextline = prev->line_u;
			}else{
				direct = MAE_TRACE_DIRECT_NO;
			}
			break;
		case MAE_TRACE_DIRECT_D:
			if( prev->l_link != NULL )
			{
				direct = MAE_TRACE_DIRECT_L;
				//nextline = prev->line_l;
			}else if( prev->d_link != NULL ){
				direct = MAE_TRACE_DIRECT_D;
				//nextline = prev->line_d;
			}else if( prev->r_link != NULL ){
				direct = MAE_TRACE_DIRECT_R;
				//nextline = prev->line_r;
			}else{
				direct = MAE_TRACE_DIRECT_NO;
			}
			break;
		case MAE_TRACE_DIRECT_L:
			if( prev->u_link != NULL )
			{
				direct = MAE_TRACE_DIRECT_U;
				//nextline = prev->line_u;
			}else if( prev->l_link != NULL ){
				direct = MAE_TRACE_DIRECT_L;
				//nextline = prev->line_l;
			}else if( prev->d_link != NULL ){
				direct = MAE_TRACE_DIRECT_D;
				//nextline = prev->line_d;
			}else{
				direct = MAE_TRACE_DIRECT_NO;
			}
			break;
		case MAE_TRACE_DIRECT_U:
			if( prev->r_link != NULL )
			{
				direct = MAE_TRACE_DIRECT_R;
				//nextline = prev->line_r;
			}else if( prev->u_link != NULL ){
				direct = MAE_TRACE_DIRECT_U;
				//nextline = prev->line_u;
			}else if( prev->l_link != NULL ){
				direct = MAE_TRACE_DIRECT_L;
				//nextline = prev->line_l;
			}else{
				direct = MAE_TRACE_DIRECT_NO;
			}
			break;
		}

		corner_x = prev->x;
		corner_y = prev->y;
		_mae_adjust_corner_point(prev, prevdirect, direct, &corner_x, &corner_y);
		region->corners[idx].x = corner_x;
		region->corners[idx].y = corner_y;
		idx ++;

		if( direct == MAE_TRACE_DIRECT_NO )
		{
			break;
		}
	}

	return result;
}
void _mae_adjust_corner_point(MAE_line_node *node, int prevdirect, int direct, int *_x_, int *_y_)
{
	int x, y;
	MAE_line *line;
	int dx, dy;
	int dx1, dy1;
	int dx2, dy2;
	int gap;
	gap = 1;
	x = *_x_;
	y = *_y_;
	switch(prevdirect)
	{
	case MAE_TRACE_DIRECT_R:
		switch(direct)
		{
		case MAE_TRACE_DIRECT_D:
			//*****************
			//   ---->        *
			//                *
			//                *
			//                *
			//                *
			dx = 0;
			dy = 0;
			//line = node->line_l;
			line = node->h_line;
			if(line != NULL )
			{
				dy = abs(line->sy - line->t) - gap;
				dy1 = abs(line->t + line->h - line->sy) - gap;
				dy = min(dy,dy1);
			}
			//line = node->line_d;
			line = node->v_line;
			if(line != NULL )
			{
				dx = abs(line->l + line->w - line->sx) - gap;
				dx1 = abs(line->sx - line->l) - gap;
				dx = min(dx, dx1);
			}
			x = x + dx;
			y = y - dy;
			break;
		case MAE_TRACE_DIRECT_R:
			//*****************  ****************
			// ---->
			dx = 0;
			dy = 0;
			//line = node->line_l;
			line = node->h_line;
			if(line != NULL )
			{
				dy1 = abs(line->sy - line->t) - gap;
				dy = abs(line->t + line->h - line->sy) - gap;
				dy = min(dy, dy1);
			}
			//line = node->line_r;
			line = node->h_line;
			if(line != NULL )
			{
				dy1 = abs(line->sy - line->t) - gap;
				dy2 = abs(line->t + line->h - line->sy) - gap;
				dy1 = min(dy1, dy2);
				dy = min(dy, dy1);
			}
			y = y - dy;
			break;
		case MAE_TRACE_DIRECT_U:
			//                *
			//                *
			//                *
			//                *
			//                *
			//*****************
			// ------->
			dx = 0;
			dy = 0;
			//line = node->line_l;
			line = node->h_line;
			if(line != NULL )
			{
				dy1 = abs(line->t + line->h - line->sy - gap);
				dy2 = abs(line->sy + line->t - gap);
				dy = min(dy1,dy2);
			}
			//line = node->line_u;
			line = node->v_line;
			if(line != NULL )
			{
				dx1 = abs(line->l + line->w - line->sx - gap);
				dx2 = abs(line->sx + line->l - gap);
				dx = min(dx1, dx2);
			}
			x = x - dx;
			y = y - dy;
			break;
		}
		break;
	case MAE_TRACE_DIRECT_D:
		switch(direct)
		{
		case MAE_TRACE_DIRECT_L:
			//              | *
			//              | *
			//              | *
			//               /*
			//                *
			//*****************
			dx = 0;
			dy = 0;
			//line = node->line_u;
			line = node->v_line;
			if(line != NULL )
			{
				dx1 = abs(line->l + line->w - line->sx - gap);
				dx2 = abs(line->sx - line->l - gap);
				dx = min(dx1, dx2);
			}
			//line = node->line_l;
			line = node->h_line;
			if(line != NULL )
			{
				dy1 = abs(line->t + line->h - line->sy - gap);
				dy2 = abs(line->sy - line->t - gap);
				dy = min(dy1, dy2);
			}
			x = x + dx;
			y = y + dy;
			break;
		case MAE_TRACE_DIRECT_D:
			//                *
			//              | *
			//              | *
			//              | *
			//               /*

			//                *
			//                *
			//                *
			//                *
			//                *
			dx = 0;
			dx1 = 0;
			//line = node->line_u;
			line = node->v_line;
			if(line != NULL )
			{
				dx1 = abs(line->l + line->w - line->sx - gap);
				dx2 = abs(line->sx - line->l - gap);
				dx1 = min(dx1, dx2);
			}
			//line = node->line_d;
			line = node->v_line;
			if(line != NULL )
			{
				dx = abs(line->l + line->w - line->sx - gap);
				dx2 = abs(line->sx - line->l - gap);

				dx = min(dx, dx2);
				dx = min(dx, dx1);
			}
			x = x + dx;
			break;
		case MAE_TRACE_DIRECT_R:
			// | *
			// | *
			// | *
			// / *
			//   *
			//   *****************
			dx = 0;
			dy = 0;
			//line = node->line_u;
			line = node->v_line;
			if(line != NULL )
			{
				dx1 = abs(line->sx - line->l - gap);
				dx2 = abs(line->l + line->w  - line->sx - gap);
				dx = min(dx1, dx2);
			}
			//line = node->line_r;
			line = node->h_line;
			if(line != NULL )
			{
				dy1 = abs(line->sy - line->t - gap);
				dy2 = abs(line->t +line->h - line->sy - gap);
				dy = min(dy1, dy2);
			}
			x = x + dx;
			y = y - dy;
			break;
		}
		break;
	case MAE_TRACE_DIRECT_L:
		switch(direct)
		{
		case MAE_TRACE_DIRECT_U:
			// *
			// *
			// *
			// *
			//   <--------
			// *****************
			dx = 0;
			dy = 0;
			//line = node->line_r;
			line = node->h_line;
			if(line != NULL )
			{
				dy1 = abs(line->t + line->h - line->sy - gap);
				dy2 = abs(line->sy - line->t - gap);
				dy = min(dy1, dy2);
			}
			//line = node->line_u;
			line = node->v_line;
			if(line != NULL )
			{
				dx1 = (line->sx - line->l - gap);
				dx2 = (line->l + line->w - line->sx - gap);
				dx = min(dx1, dx2);
			}
			x = x - dx;
			y = y + dy;
			break;
		case MAE_TRACE_DIRECT_L:
			//              <--------
			// *********** *****************
			dy = 0;
			dy1 = 0;
			dy2 = 0;
			//line = node->line_r;
			line = node->h_line;
			if(line != NULL )
			{
				dy1 = abs(line->t + line->h - line->sy - gap);
				dy2 = abs(line->sy - line->t - gap);
				dy1 = min(dy1, dy2);
			}
			//line = node->line_l;
			line = node->h_line;
			if(line != NULL )
			{
				dy1 = abs(line->t + line->h - line->sy - gap);
				dy2 = abs(line->sy - line->t - gap);
				dy2 = min(dy1, dy2);
			}
			dy = min(dy1,dy2);
			y = y + dy;
			break;
		case MAE_TRACE_DIRECT_D:
			//   <--------
			// *****************
			// *
			// *
			// *
			// *
			dx = 0;
			dy = 0;
			//line = node->line_r;
			line = node->h_line;
			if(line != NULL )
			{
				dy1 = abs(line->sy - line->t - gap);
				dy2 = abs(line->t + line->h - line->sy - gap);
				dy = min(dy1,dy2);
			}
			//line = node->line_d;
			line = node->v_line;
			if(line != NULL )
			{
				dx1 = abs(line->sx - line->l - gap);
				dx2 = abs(line->l + line->w - line->sx - gap);
				dx = min(dx1, dx2);
			}
			x = x + dx;
			y = y + dy;
			break;
		}
		break;
	case MAE_TRACE_DIRECT_U:
		switch(direct)
		{
		case MAE_TRACE_DIRECT_R:
			//  *****************
			//  */
			//  *|
			//  *|
			//  *|
			dx = 0;
			dy = 0;
			//line = node->line_d;
			line = node->v_line;
			if(line != NULL )
			{
				dx = abs(line->sx - line->l - gap);
				dx1 = abs(line->l + line->w - line->sx - gap);
				dx = min(dx,dx1);
			}
			//line = node->line_r;
			line = node->h_line;
			if(line != NULL )
			{
				dy = abs(line->t - line->sy - gap);
				dy1 = abs(line->t + line->h - line->sy - gap);
				dy = min(dy,dy1);
			}
			x = x - dx;
			y = y - dy;
			break;
		case MAE_TRACE_DIRECT_U:
			// *
			// *
			// *
			// *

			// */
			// *|
			// *|
			// *|
			dx = 0;
			//line = node->line_d;
			line = node->v_line;
			if(line != NULL )
			{
				dx = abs(line->sx - line->l - gap);
				dx1 = abs(line->l + line->w - line->sx - gap);
				dx = min(dx, dx1);
			}
			//line = node->line_u;
			line = node->v_line;
			if(line != NULL )
			{
				dx = abs(line->sx - line->l - gap);
				dx1 = abs(line->l + line->w - line->sx - gap);
				dx1 = min(dx, dx1);
				dx = min(dx, dx1);
			}
			x = x - dx;
			break;
		case MAE_TRACE_DIRECT_L:
			//  *****************
			//					*/
			//					*|
			//					*|
			//					*|
			dx = 0;
			dy = 0;
			//line = node->line_d;
			line = node->v_line;
			if(line != NULL )
			{
				dx = abs(line->sx - line->l - gap);
				dx1 = abs(line->l + line->w - line->sx - gap);
				dx = min(dx, dx1);
			}
			//line = node->line_l;
			line = node->h_line;
			if(line != NULL )
			{
				dy = abs(line->t - line->sy - gap);
				dy1 = abs(line->t + line->h - line->sy - gap);
				dy = min(dy,dy1);
			}
			x = x - dx;
			y = y + dy;
			break;
		}
		break;
	}
	*_x_ = x;
	*_y_ = y;
}

void _mae_adjust_corners_orders(MAE_region **rghead)
{
	int i;
	MAE_region *p;
	unsigned char *cornerdata;
	MAE_corner *corners;
	int  idx;
	int x,y;
	p = *rghead;

	for(; p != NULL; p = p->link)  
	{
		_mae_malloc_uchar( &cornerdata, (p->cornernum) * sizeof(MAE_corner),"_mae_adjust_corners_orders : MAE_corner");
		corners = (MAE_corner *)cornerdata;
		for(i = 0; i < p->cornernum; i ++ )
		{
			corners[i].x = p->corners[i].x;
			corners[i].y = p->corners[i].y;
		}
		//sert left top corner
		idx = 0;
		x = corners[0].x;
		y = corners[0].y;
		for(i = 1; i < p->cornernum; i ++ )
		{
			if( corners[i].y < y )
			{
				idx = i;
				x = corners[i].x;
				y = corners[i].y;
			}else if( corners[i].y == y ){
				if( corners[i].x < x )
				{
					idx = i;
					x = corners[i].x;
					y = corners[i].y;
				}
			}
		}

		if( idx != 0 )
		{
			for( i = 0; i <  p->cornernum; i ++ )
			{    
				if( i+idx < p->cornernum )
				{
					p->corners[i].x  = corners[i+idx].x;
					p->corners[i].y  = corners[i+idx].y;
				}else{
					p->corners[i].x  = corners[i+idx - p->cornernum].x;
					p->corners[i].y  = corners[i+idx - p->cornernum].y;
				}
			}
		}
		free(corners);

	}

}

void _mae_calculate_region_rects(MAE_region **rghead)
{
	int i;
	MAE_region *p;
	p = *rghead;
	int l, t, r, b;
	for(; p != NULL; p = p->link)  
	{
		if( p->cornernum == 0 ) continue;
		if( p->label == -1 ) continue;
		l = p->corners[0].x;
		t = p->corners[0].y;
		r = l;
		b = t;
		for(i = 1; i < p->cornernum; i ++ )
		{
			if( l > p->corners[i].x ) l = p->corners[i].x;
			if( t > p->corners[i].y ) t = p->corners[i].y;
			if( r < p->corners[i].x ) r = p->corners[i].x;
			if( b < p->corners[i].y ) b = p->corners[i].y;
		}
		if( l < 0 ) l = 0;
		if( t < 0 ) t = 0;
		p->l = l;
		p->t = t;
		p->w = r - l;
		p->h = b - t;
	}

}

void _mae_get_all_line_color(MAE_image *h_image, MAE_line *h_line, int h_num, MAE_line *v_line, int v_num)
{
	int i, j;

	for(i = 0; i < h_num; i ++ )
	{
		if( h_line[i].label == -1 ) continue;
		_mae_get_line_color(h_image, &h_line[i]);
	}
	for(j = 0; j < v_num; j ++ )
	{
		if( v_line[j].label == -1 ) continue;
		_mae_get_line_color(h_image, &v_line[j]);
	}
}

void _mae_get_line_color(MAE_image *in_image, MAE_line *line)
{
	int l,t,w,h;
	int mark_pixs;
	int re, ge, be, he;
	MAE_image cut_image;
	MAE_image h_image;

	l = line->l;
	t = line->t;
	w = line->w;
	h = line->h;

	if(t < 0) t= 0;

	_mae_get_region_image(in_image, l, t, w, h, &cut_image );
	_mae_detect_high_color(&cut_image, &h_image, RGB_REMOVE_TEXT_VARIENCE);
	free(cut_image.data);
	re = 0;
	ge = 0;
	be = 0;
	mark_pixs = 0;
	_mae_get_image_rgb_e_v(&h_image, &re, &ge, &be, &mark_pixs);
	free(h_image.data);
	line->re = re;
	line->ge = ge;
	line->be = be;

	he = 0;
	//_mae_get_image_h_e_v(&h_image, &he);
	line->he = he;


}

int  _mae_line_color_is_thesame( MAE_line *line1, MAE_line *line2)
{
	int dre;
	int dge;
	int dbe;
	int sa;
	dre = abs(line1->re - line2->re);
	dge = abs(line1->ge - line2->ge);
	dbe = abs(line1->be - line2->be);
	sa = dre + dge + dbe;
	if( sa > MAE_RGB_VARIANCE4 || dre > 60 || dge > 60 || dbe > 60 )
	{
		return 0;
	}
	return 1;
}
int  _mae_line_color_is_thesame2( MAE_line *line1, MAE_line *line2)
{
	int dre;
	int dge;
	int dbe;
	int sa;
	dre = abs(line1->re - line2->re);
	dge = abs(line1->ge - line2->ge);
	dbe = abs(line1->be - line2->be);
	sa = dre + dge + dbe;
	if( sa > MAE_RGB_VARIANCE4 )
	{
		return 0;
	}
	return 1;
}

int _mae_save_mark_image_from_lines(MAE_image *in_image, MAE_line *h_line, int h_num, MAE_line *v_line, int v_num, MAE_image *out_image)
{
	int error = MAE_NO_ERROR;
	int i,j;
	int k;
	int depth;
	int movedbits;
	long widthbytes;
	unsigned char *data;
	unsigned char *temp;
	unsigned char *in_data;
	unsigned char *in_temp;
	unsigned char *out_data;
	unsigned char *out_temp;

	unsigned char re;
	unsigned char ge;
	unsigned char be;

	int l,t,r,b;
	int x,y;

	out_image->xdpi = in_image->xdpi;
	out_image->ydpi = in_image->ydpi;
	out_image->depth = in_image->depth;
	out_image->type = in_image->type;
	out_image->w  = in_image->w;
	out_image->h = in_image->h;
	out_image->stride = _mae_width_bytes(  out_image->depth, out_image->w );
	out_image->data  = _mae_mallocate_image(out_image->w,out_image->h,out_image->depth);

	memset(out_image->data, (BYTE)255, _mae_sizefromdepth(out_image->w,out_image->h,out_image->depth) );
	//memcpy(out_image->data, in_image->data, _mae_sizefromdepth(out_image->w,out_image->h,out_image->depth) );

	depth = in_image->depth;
	movedbits = 1;
	widthbytes = _mae_width_bytes(depth, in_image->w);
	switch( depth )
	{
	case 8:	
		movedbits = 1;
		break;
	case 16:
		movedbits = 2;
		break;
	case 24:
		movedbits = 3;
		break;
	case 32:
		movedbits = 4;
		break;
	}


	data = out_image->data;

	for(i = 0; i < h_num; i ++ )
	{
		//20110104 add start
		if(h_line[i].label == -1 )
		{
			continue;
		}
		re = h_line[i].re;
		ge = h_line[i].ge;
		be = h_line[i].be;

		l = h_line[i].l;
		t = h_line[i].t;
		r = l +  h_line[i].w -1;
		b = t +  h_line[i].h -1;
		for(y = t; y <= b ; y ++ )
		{
			//in_data = in_image->data + widthbytes * y;
			out_data = out_image->data + widthbytes * y;
			for( x = l; x <= r; x ++ )
			{
				//in_temp = in_data +  movedbits * x;
				out_temp = out_data + movedbits * x;
				if( movedbits == 1 )
				{
					*out_temp = 255;
				}else{
					*(out_temp + 0) = re;
					*(out_temp + 1) = ge;
					*(out_temp + 2) = be;
				}
			}
		}

	}


	for(i = 0; i < v_num; i ++ )
	{
		//20110104 add start
		if(v_line[i].label == -1 )
		{
			continue;
		}
		re = v_line[i].re;
		ge = v_line[i].ge;
		be = v_line[i].be;

		l = v_line[i].l;
		t = v_line[i].t;
		r = l +  v_line[i].w -1;
		b = t +  v_line[i].h -1;
		for(y = t; y <= b ; y ++ )
		{
			//in_data = in_image->data + widthbytes * y;
			out_data = out_image->data + widthbytes * y;
			for( x = l; x <= r; x ++ )
			{
				//in_temp = in_data +  movedbits * x;
				out_temp = out_data + movedbits * x;
				if( movedbits == 1 )
				{
					*out_temp = 255;
				}else{
					*(out_temp + 0) = re;
					*(out_temp + 1) = ge;
					*(out_temp + 2) = be;
				}
			}
		}

	}
	return error;
}

int _mae_check_linknode_line(unsigned char *data, int width, int height, MAE_line_node *lnode, MAE_line_node *rnode)
{
	int x1,y1, x2, y2;
	int w, h;
	int count;
	int checknum;
	x1 = lnode->x;
	y1 = lnode->y;
	x2 = rnode->x;
	y2 = rnode->y;

	if( y1 == y2 )
	{
		w = x2 - x1;
		h = 1;
		checknum = w / 3;
	}else if(x1 == x2){
		w = 1;
		h = y2 - y1;
		checknum = h / 3;
	}else{
		return 0;
	}
	count = _mae_hist_rect_pixs(data, width, height, x1, y1, w, h);

	if( count <= checknum ) 
	{
		return 0;
	}else{
		return 1;
	}
}

void _mae_check_s_region_and_l_region(MAE_region **sregions, int * snum, MAE_region **lregions, int * lnum)
{
	int i;
	int j;
	int k;
	int x1, y1, x2, y2;
	int cx, cy;
	MAE_region * srs;
	int sn;
	MAE_region * lrs;
	int ln;
	int check;
	int check2;
	srs = *sregions;
	sn = *snum;
	lrs = *lregions;
	ln = *lnum;

	for( i = 0; i < ln; i++ )
	{
		for(j = 1; j <= lrs[i].cornernum; j ++ )
		{
			if( j == lrs[i].cornernum )
			{
				x1 = lrs[i].corners[j-1].x;
				y1 = lrs[i].corners[j-1].y;
				x2 = lrs[i].corners[0].x;
				y2 = lrs[i].corners[0].y;
			}else{
				x1 = lrs[i].corners[j-1].x;
				y1 = lrs[i].corners[j-1].y;
				x2 = lrs[i].corners[j].x;
				y2 = lrs[i].corners[j].y;
			}
			for( k = 0; k < sn; k++ )
			{
				if( srs[k].label == -1 ) 
				{
					continue;
				}
				check = _mae_check_region2(x1, y1, x2, y2, &srs[k]);
				if( check != 0 )
				{
					srs[k].label = -1;
				}
			}
		}
	}
	for( k = 0; k < sn; k++ )
	{
		cx = srs[k].l + srs[k].w / 2;
		cy = srs[k].t + srs[k].h / 2;

		for( i = 0; i < ln; i++ )
		{
			check = _mae_point_is_in_region(cx, cy, &lrs[i]);
			if( check == 1 )
			{
				srs[k].label = -1;
				break;
			}
		}
	}
}

int _mae_check_region2(int x1, int y1, int x2, int y2, MAE_region *region)
{
	int l, t, r, b;
	int i;
	int cx,cy;
	int tmp;

	if(x1 > x2 )
	{
		tmp = x1;
		x1 = x2;
		x2 = tmp;
	}
	if(y1 > y2 )
	{
		tmp = y1;
		y1 = y2;
		y2 = tmp;
	}

	cx = (x1 + x2 ) / 2;
	cy = (y1 + y2 ) / 2;
	for( i = 0; i < 2; i ++ )
	{
		l = region->l;
		t = region->t;
		r = region->l + region->w - 1;
		b = region->t + region->h  - 1;

		if( y1 == y2 ) //hori
		{
			if(    y1 >= t 	&& y1 <= b )
			{
				if( (x1 <= r && x1 >= l) ||
					(x2 <= r && x2 >= l) ||
					(l <= x2 && l >= x1) ||
					(r <= x2 && r >= x1) ||
					(cx <= r && cx >= l) 
					)
				{
					return 1;
				}
			}
		} else	if( x1 == x2 ) //vert
		{
			if(    x1 >= l 	&& x1 <= r )
			{
				if( (y1 <= b && y1 >= t) ||
					(y2 <= b && y2 >= t) ||
					(t <= y2 && t  >= y1) ||
					(b <= y2 && b  >= y1) ||
					(cy <= b && cy >= t) 
					)
				{
					return 1;
				}
			}
		}
	}

	return 0;
}


void _mae_check_corners_net(MAE_line_node **lpthead, int lptnum)
{
	MAE_line_node * pnode,*qnode;
	int cornernum;
	int label = 0;
	pnode = *lpthead;
	for(; pnode != NULL; pnode = pnode->next)
	{
		if( pnode->label == -1 )
		{
			continue;
		}
		if(     pnode->l_link != NULL 
			&& pnode->r_link == NULL 
			&& pnode->u_link == NULL 
			&& pnode->d_link == NULL 
			) 
		{
			qnode = pnode->l_link;
			pnode->l_link = NULL;
			qnode->r_link = NULL;

		}	
		if(     pnode->l_link == NULL 
			&& pnode->r_link != NULL 
			&& pnode->u_link == NULL 
			&& pnode->d_link == NULL 
			) 
		{
			qnode = pnode->r_link;
			pnode->r_link = NULL;
			qnode->l_link = NULL;

		}	
		if(     pnode->l_link == NULL 
			&& pnode->r_link == NULL 
			&& pnode->u_link != NULL 
			&& pnode->d_link == NULL 
			) 
		{
			qnode = pnode->u_link;
			pnode->u_link = NULL;
			qnode->d_link = NULL;

		}	
		if(     pnode->l_link == NULL 
			&& pnode->r_link == NULL 
			&& pnode->u_link == NULL 
			&& pnode->d_link != NULL 
			) 
		{
			qnode = pnode->d_link;
			pnode->d_link = NULL;
			qnode->u_link = NULL;

		}	
	}
}
