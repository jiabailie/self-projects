#include "StdAfx.h"
#include "regions.h"
#include "memalloc.h"
#include <stdlib.h>
#include "imageutil.h"
#include "runs.h"
#include "runsprocess.h"
#include "histogram.h"
#include "blocks.h"
#include "chain.h"
#include "imageprocess.h"
#include "segment.h"
//#include <math.h>
//#include "linkregion.h"
#include "detectcolorregion.h"

#define HORI_GAP  2
#define VERT_GAP  2
void _mae_destroy_regions(MAE_region* regions, int nregions)
{
	//int num;
	//num = nregions;
	int i;
	//for(i = 0; i < nregions; i++ )
	//{
	//	if(regions[i].rx != NULL ) 	free(regions[i].rx);
	//	if(regions[i].ry != NULL )  free(regions[i].ry);
	//	if(regions[i].rlen != NULL )  free(regions[i].rlen);
	//}
	//free(regions);
	for(i = 0; i < nregions; i ++ )
	{
		if( regions[i].corners != NULL )
		{
			free(regions[i].corners);
		}
	}
	if(regions != NULL )
	{
		free(regions);
	}
}


//int _mae_detect_region_blocks(unsigned char *bi_data,unsigned char * _data, int _width, int _height, MAE_region **_mae_region, int *_nregions)
//int _mae_detect_region_blocks(unsigned char *bi_data,unsigned char * _data, int _width, int _height, MAE_region **_mae_region, int *_nregions,
//							  MAE_node ** _run_list, int *_listnum)
int _mae_detect_region_blocks(unsigned char *bi_data,unsigned char * _data, int _width, int _height, 
							  int * h_rx, int *h_ry, int *h_rlen, int h_nruns,
							  MAE_region **_mae_region, int *_nregions)
{
	// int *h_rx, *h_ry, *h_rlen, h_nruns, h_ralloc;
	//   int *v_rx, *v_ry, *v_rlen, v_nruns, v_ralloc;
	//unsigned char *data;
	//unsigned char *v_data;
	//int *blockst;
	//int *blocked;
	//int blocknum;
	//  int i;
	unsigned char *data;
	int w;
	int h;
	// int v_w;
	// int v_h;
	//MAE_region *regions;
	//int nregions;
	//unsigned char *reg_data;

	MAE_block *blocks;
	int nblocks;

	MAE_node * run_list;
	int listnum;
	//int *h_hist;
	//int nh_hist;
	unsigned char *_bi_data;
	_bi_data = bi_data;

	data = _data;





	w = _width;
	h = _height;
	/* get all horizontal runs in the image */
	//_mae_get_runs(&h_rx, &h_ry, &h_rlen, &h_nruns, &h_ralloc, data, w, h);

	//20100901 add start
	//link break runs
	//_mae_link_break_runs(h_rx, h_ry, h_rlen, h_nruns, MAE_MARK_RUNS_GAP2);
	//20100901 add end

	//remove short runs
	//_mae_remove_short_runs(h_rx, h_ry, h_rlen, h_nruns);
	//#ifdef MAE_DEBUG
	//MAE_image runs_image;
	//runs_image.w = _width;
	//runs_image.h = _height;
	//runs_image.type = 1;
	//runs_image.depth  = 24;
	//runs_image.stride = _mae_width_bytes(24,_width );
	//runs_image.data  = _mae_mallocate_image(w,h,24);
	//_mae_save_h_runs_image(&runs_image, h_rx, h_ry, h_rlen, h_nruns );
	//_mae_save_image("test_hori_run_to_image.bmp",&runs_image);
	//free(runs_image.data);
	//#endif
	/* get run chain list */
	_mae_create_runs_chain(&run_list,&listnum , h_rx, h_ry, h_rlen, h_nruns);

	//20100831 add start
	//  *_run_list = run_list;
	// *_listnum = listnum;
	//20100831 add end

	//detect blocks
	_mae_detect_blocks(run_list, listnum, &blocks, &nblocks);

	//delete small blocks
	//20100831 delete
	//20101110 test for lines start
	_mae_delete_small_blocks(blocks, nblocks);
	//20101110 test for lines end

	//merge blocks based on rect pos
	//20100831 add start
	//_mae_com_block_rect(blocks);
	//_mae_merge_blocks_by_rect(blocks, nblocks);
	//20100831 add end

	//20100831 add start
	*_mae_region = NULL;
	*_nregions = 0;
	if( nblocks == 0 )
	{
		free(h_rx);
		free(h_ry);
		free(h_rlen);
		//free(h_hist);
		free(run_list);
		_mae_destroy_blocks(blocks);
		return 0;
	}
	//20100831 add end
	//get regions
	_mae_get_regions_from_blocks(blocks, nblocks, _mae_region,_nregions);

	//computer region rect
	_mae_computer_regions_rect(_mae_region, _nregions);

#ifdef MAE_DEBUG
	{
		MAE_image runs_image;
		MAE_image mask;
		runs_image.w = _width;
		runs_image.h = _height;
		runs_image.type = 1;
		runs_image.depth  = 24;
		runs_image.stride = _mae_width_bytes(24,_width );
		runs_image.data  = _mae_mallocate_image(w,h,24);
		_mae_save_h_runs_image(&runs_image, h_rx, h_ry, h_rlen, h_nruns );
		_mae_save_image("test_run_to_image_1.bmp",&runs_image);
		_mae_save_mark_image(&runs_image, *_mae_region, *_nregions, &mask, 5);
		_mae_save_image("test_region_to_image4_1.bmp",&mask);
		free(runs_image.data);
		free(mask.data);
	}
#endif

	//merge region based on rect pos
	//20101110 test for lines start
	_mae_merge_region_by_rect(_mae_region, _nregions);
	//20101110 test for lines end

	//remove small region based on rect pos
	//20100831 delete
	//20101110 test for lines start
	_mae_remove_small_regions(_mae_region, _nregions, w, h);
	//20101110 test for lines end

	//adjust region edge based on rect pos
	//20100831 delete 
	//_mae_adjust_region_list_rect2(_data, _width, _height, _mae_region, _nregions);

	//adjust region based on rect pos
	//chen 20100816 delete because there are some wrong in this process 
	//_mae_adjust_region_list_rect(bi_data,_width, _height, _mae_region, _nregions);

	//merge region based on rect pos
	//20101110 test for lines start
	//_mae_merge_region_by_rect(_mae_region, _nregions);
	//20101110 test for lines end

	//remove small region based on rect pos
	//20101110 test for lines start
	//_mae_remove_small_regions(_mae_region, _nregions, w, h);
	//20101110 test for lines end

	//merge region based on rect pos
	//_mae_merge_break_region(_mae_region, _nregions);

#ifdef MAE_DEBUG
	{
		MAE_image runs_image;
		MAE_image mask;
		runs_image.w = _width;
		runs_image.h = _height;
		runs_image.type = 1;
		runs_image.depth  = 24;
		runs_image.stride = _mae_width_bytes(24,_width );
		runs_image.data  = _mae_mallocate_image(w,h,24);
		_mae_save_h_runs_image(&runs_image, h_rx, h_ry, h_rlen, h_nruns );
		_mae_save_image("test_run_to_image.bmp",&runs_image);
		_mae_save_mark_image(&runs_image, *_mae_region, *_nregions, &mask, 5);
		_mae_save_image("test_region_to_image4.bmp",&mask);
		free(runs_image.data);
		free(mask.data);
	}
#endif
	//check back color regions
	//20100819 delete
	//_mae_check_back_color_region_all1(_data, _width, _height, _mae_region, _nregions);

	//detect two region linked occasion 
	//_mae_detect_link_region_all(_data, _width, _height, _mae_region, _nregions);
	//


	//free(h_rx);
	//free(h_ry);
	//free(h_rlen);
	//free(h_hist);
	free(run_list);
	_mae_destroy_blocks(blocks);
	//*_mae_region = regions;
	//*_nregions = nregions;

	return 0;
}
//
//int _mae_detect_h_line(int *rx, int *ry, int *rlen, int nruns, int ** line )
//{
//	//int x1;
//	int y1;
//	//int x2;
//	int y2;
//	int i;
//	//int j;
//	//int *_rx_;
//	//int *_ry_;
//	int *_line_;
//
//	if( nruns == 0 ) 
//	{
//		return 0;
//	}
//
//	//_malloc_int(&_rx_, nruns, "_detect_h_line : _rx_");
//	//_malloc_int(&_ry_, nruns, "_detect_h_line : _ry_");
//	_mae_malloc_int(line, nruns, "_detect_h_line : _line_");
//	_line_ = *line;
//	for(i = 0; i < nruns-1; i ++ )
//	{
//		//_rx_[i] = rx[i];
//		//_ry_[i] = ry[i];
//		_line_[i] = 0;
//	}
//	//hori cluster
//	int _count = 1;
//	_line_[0] = _count;
//	for(i = 1; i < nruns-1; i ++ )
//	{
//		y1 =  ry[i-1];
//		y2 =  ry[i];
//		if(y2 - y1 > HORI_GAP )
//		{
//			_count++;
//		}
//		_line_[i] = _count;
//	}
//
//	*line = _line_;
//	//free(rx_cpy);
//	//free(ry_cpy);
//	return 0;
//}

int _mae_detect_region(unsigned char * _data, int _width, int _height, MAE_region **_mae_region, int *_nregions)
{
	int *h_rx, *h_ry, *h_rlen, h_nruns, h_ralloc;
	//int *v_rx, *v_ry, *v_rlen, v_nruns, v_ralloc;
	unsigned char *data;
	//   unsigned char *v_data;
	int *blockst;
	int *blocked;
	int blocknum;
	//   int i;
	int w;
	int h;
	//int v_w;
	//int v_h;
	MAE_region *regions;
	int nregions;
	//   unsigned char *reg_data;

	int *h_hist;
	int nh_hist;

	data = _data;
	w = _width;
	h = _height;
	/* get all horizontal runs in the image */
	_mae_get_runs(&h_rx, &h_ry, &h_rlen, &h_nruns, &h_ralloc, data, w, h);

	//remove short runs
	_mae_remove_short_runs(h_rx, h_ry, h_rlen, h_nruns, SHORT_RUNS_THRESHOLD);

	//histgram hori runs
	_mae_hist_h_runs(h_rx, h_ry, h_rlen, h_nruns, w, h, 
		&h_hist, &nh_hist);

	//_sort_runs_inc(h_rx, h_ry, h_rlen, h_nruns);
	//   _detect_h_line(h_rx, h_ry, h_rlen, h_nruns,h_hist, nh_hist, &line);
	_mae_compute_hist_blocks(h_hist, nh_hist, &blockst, &blocked, &blocknum);

	//nregions = _get_nregions(line, h_nruns);

	_mae_get_regions(&regions, &nregions, w, h, blockst, blocked, blocknum);

	free(h_rx);
	free(h_ry);
	free(h_rlen);
	free(h_hist);
	free(blockst);
	free(blocked);

	*_mae_region = regions;
	*_nregions = nregions;

	return 0;
}
int _mae_get_regions(MAE_region **regions, int *nregions,int width, int height, int *blockst, int *blocked, int blocknum)
{

	int _height;
	_height = height;
	//label = 0;
	//for(i = 0; i < nregions; i ++ )
	//{
	//	label++;
	//	region = &regions[i];
	//	int _count = 0;
	//	for(j = 0; j < h_nruns; j ++ )
	//	{
	//		if( line[j] == label)
	//		{
	//			_count++;
	//		}
	//	}
	//	if(_count == 0 ) continue;
	//  region->num = _count;
	//  region->label = 	label;
	//     _malloc_int(&region->rx ,  _count, "_get_region_line : rx");
	//     _malloc_int(&region->ry ,  _count, "_get_region_line : ry");
	//     _malloc_int(&region->rlen, _count, "_get_region_line : rlen");
	//  memset(region->rx, (int)0, _count);
	//  memset(region->ry, (int)0, _count);
	//  memset(region->rlen, (int)0, _count);
	//  int _idx = 0;
	//  for(j = 0; j < h_nruns; j ++ )
	//  {
	//	  if( label == line[j] && _idx < _count )
	//	  {
	//			region->rx[_idx] = h_rx[j];
	//			region->ry[_idx] = h_ry[j];
	//			region->rlen[_idx] = h_rlen[j];
	//			_idx ++;
	//	  }
	//  }
	//  left = 10000;
	//  right = 0;
	//  top = 10000;
	//  bottom = 0;
	//  for( j = 0; j < region->num; j ++ )
	//  {
	//	  if( left > region->rx[j] ) left = region->rx[j];
	//	  if( right < (region->rx[j]+region->rlen[j]) ) right = (region->rx[j]+region->rlen[j]);
	//	  if( top > region->ry[j] ) top = region->ry[j];
	//	  if( bottom < region->ry[j] ) bottom = region->ry[j];
	//  }
	//  region->l = left;
	//  region->t = top;
	//  region->w = right - left;
	//  region->h = bottom-top;
	//}
	int i;
	unsigned char *reg_data;
	MAE_region *_regions;
	int _region_num;
	//int left;
	//int right;
	//int bottom;
	//int top;
	//int label;

	_region_num = blocknum;

	_mae_malloc_uchar(&reg_data, _region_num * sizeof(MAE_region),"_detect_region : region");
	_regions = (MAE_region *)reg_data;
	//initialize
	for( i = 0; i < _region_num; i ++ )
	{
		_regions[i].type = 0;
		_regions[i].label = 0;
		_regions[i].l = 0;
		_regions[i].t = 0;
		_regions[i].w = 0;
		_regions[i].h = 0;
		_regions[i].head = NULL;
		_regions[i].tail = NULL;
	}

	int _label = 0;
	for(i = 0; i < _region_num; i ++ )
	{
		_label++;
		_regions[i].label = _label;
		_regions[i].l = 0;
		_regions[i].t = blockst[i];
		_regions[i].w = width - 1;
		_regions[i].h = blocked[i];
	}

	*regions = _regions;
	*nregions = _region_num;
	return 0;
}

int _mae_get_nregions(int *line, int nlines)
{
	int max;
	int i;

	if( nlines == 0 )
	{
		return 0;
	}
	max = line[0];
	for(i = 1; i < nlines; i ++ )
	{
		if( max < line[i] )
		{
			max = line[i];
		}
	}
	return max;

}

int _mae_test_region(MAE_image *out_image, MAE_region *mae_regions, int nregions)
{
	int error = MAE_NO_ERROR;
	int left;
	int top;
	int right;
	int bottom;
	int i,x,y;
	int depth;
	int movedbits;
	long widthbytes;
	unsigned char *data;
	unsigned char *temp;

	depth = out_image->depth;
	data = out_image->data;
	movedbits = 1;
	widthbytes = _mae_width_bytes(depth, out_image->w);
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
	for(i = 0; i < nregions; i ++ )
	{
		//do not show region deleted
		if( mae_regions[i].label == -1 ) continue;

		left = mae_regions[i].l;
		top = mae_regions[i].t;
		right = left +  mae_regions[i].w -1;
		bottom = top +  mae_regions[i].h -1;
		//left edge of box
		for(y = top; y < bottom; y ++ )
		{
			x = left;
			temp = data + widthbytes * y + movedbits * x;
			if( movedbits == 1 )
			{
				*temp = 120;
			}else{
				*(temp + 0) = 255;
				*(temp + 1) = 0;
				*(temp + 2) = 0;
			}
		}
		//right edge of box
		for(y = top; y < bottom; y ++ )
		{
			x = right;
			temp = data + widthbytes * y + movedbits * x;
			if( movedbits == 1 )
			{
				*temp = 120;
			}else{
				*(temp + 0) = 0;
				*(temp + 1) = 0;
				*(temp + 2) = 255;
			}
		}
		//top edge of box
		for(x = left; x < right; x ++ )
		{
			y = top;
			temp = data + widthbytes * y + movedbits * x;
			if( movedbits == 1 )
			{
				*temp = 120;
			}else{
				*(temp + 0) = 255;
				*(temp + 1) = 0;
				*(temp + 2) = 0;
			}
		}
		//bottom edge of box
		for(x = left; x < right; x ++ )
		{
			y = bottom;
			temp = data + widthbytes * y + movedbits * x;
			if( movedbits == 1 )
			{
				*temp = 120;
			}else{
				*(temp + 0) = 0;
				*(temp + 1) = 0;
				*(temp + 2) = 255;
			}
		}
	}

	return error;

}

int _mae_detect_region_hist(unsigned char * data, int width, int height, MAE_region **mae_region, int *nregions)
{
	int *h_hists;
	int h_len;
	int *h_blockst;
	int *h_blocked;
	int h_blocknum;
	//MAE_region *regions;
	//   unsigned char *reg_data;
	//int region_num;
	//int label;

	_mae_count_h_hist(data,width,height,&h_hists,&h_len);
	_mae_compute_hist_blocks(h_hists, h_len, &h_blockst, &h_blocked, &h_blocknum);

	_mae_get_regions(mae_region, nregions, width, height, h_blockst, h_blocked, h_blocknum);

	//region_num = h_blocknum;
	//_malloc_uchar(&reg_data, region_num * sizeof(MAE_region),"_mae_detect_region_hist : region");
	//regions = (MAE_region *)reg_data;
	////initialize
	//for( i = 0; i < region_num; i ++ )
	//{
	//	regions[i].type = 0;
	//	regions[i].label = 0;
	//	regions[i].l = 0;
	//	regions[i].t = 0;
	//	regions[i].r = 0;
	//	regions[i].b = 0;
	//	//regions[i].w = 0;
	//	//regions[i].h = 0;
	//	regions[i].num = 0; 
	//	regions[i].rx = NULL;
	//	regions[i].ry = NULL;
	//	regions[i].rlen = NULL;
	//}

	//label = 0;
	//for(i = 0; i < region_num; i ++ )
	//{
	//	label++;
	//	regions[i].label = 0;
	//	regions[i].l = 0;
	//	regions[i].t = h_blockst[i];
	//	regions[i].r = width - 1;
	//	regions[i].b = h_blocked[i];
	//	//regions[i].w = width - 1;
	//	//regions[i].h = h_blocked[i];
	//}
	//*nregions = region_num;
	//*mae_region = regions;

	free(h_blockst);
	free(h_blocked);
	return 0;
}
//int _mae_detect_region_blocks(unsigned char *bi_data,unsigned char * _data, int _width, int _height, MAE_region **_mae_region, int *_nregions)
//int _mae_detect_region_blocks(unsigned char *bi_data,unsigned char * _data, int _width, int _height, MAE_region **_mae_region, int *_nregions,
//							  MAE_node ** _run_list, int *_listnum)

//int _mae_detect_vertlines_blocks(unsigned char *bi_data,unsigned char * _data, int _width, int _height, MAE_line **_v_lines, int *_vlinenum,
//							  MAE_node ** _run_list, int *_listnum)
int _mae_detect_vertlines_blocks(unsigned char *bi_data,unsigned char * _data, int _width, int _height, 
								 int * h_rx, int * h_ry, int * h_rlen , int h_nruns, int h_line_width,
								 MAE_line **_v_lines, int *_vlinenum)
{
	//int *h_rx, *h_ry, *h_rlen, h_nruns, h_ralloc;
	unsigned char *data;
	int w;
	int h;

	MAE_block *blocks;
	int nblocks;

	MAE_node * run_list;
	int listnum;
	unsigned char *_bi_data;
	int line_width;

	_bi_data = bi_data;

	data = _data;
	w = _width;
	h = _height;
	/* get all horizontal runs in the image */
	//_mae_get_runs(&h_rx, &h_ry, &h_rlen, &h_nruns, &h_ralloc, data, w, h);

	//20100901 add start
	//link break runs
	//   _mae_link_break_runs(h_rx, h_ry, h_rlen, h_nruns, MAE_MARK_RUNS_GAP2);
	//20100901 add end

	//remove short runs
	//_mae_remove_short_runs(h_rx, h_ry, h_rlen, h_nruns);

	//remove long runs
	//_mae_remove_long_runs(h_rx, h_ry, h_rlen, h_nruns, MAE_MARK_LINE_WIDTH);
	//_mae_remove_long_runs(h_rx, h_ry, h_rlen, h_nruns, h_line_width);

#ifdef MAE_DEBUG
	{
		MAE_image runs_image;
		runs_image.w = _width;
		runs_image.h = _height;
		runs_image.type = 1;
		runs_image.depth  = 24;
		runs_image.stride = _mae_width_bytes(24,_width );
		runs_image.data  = _mae_mallocate_image(w,h,24);
		_mae_save_h_runs_image(&runs_image, h_rx, h_ry, h_rlen, h_nruns );
		_mae_save_image("_mae_detect_vertlines_blocks_1.bmp",&runs_image);
		free(runs_image.data);
	}
#endif
	/* get run chain list */
	//20110124 add start
	run_list = NULL;
	listnum = 0;
	//20110124 add end

	_mae_create_runs_chain_split(&run_list,&listnum , h_rx, h_ry, h_rlen, h_nruns);

	//20100831 add start
	//*_run_list = run_list;
	//*_listnum = listnum;
	//20100831 add end

	//detect blocks
	_mae_detect_blocks(run_list, listnum, &blocks, &nblocks);

	//delete small blocks
	//20100831 delete
	//20101110 test for lines start
	//_mae_delete_small_blocks(blocks, nblocks);
	//20101110 test for lines end

	//merge blocks based on rect pos
	//20100831 add start
	//_mae_com_block_rect(blocks);
	//_mae_merge_blocks_by_rect(blocks, nblocks);
	//20100831 add end
	_mae_computer_blocks_rect(&blocks, &nblocks);

	//20101225 add START
	line_width = _mae_compute_line_width_fromblocks(blocks, nblocks);

	_mae_destroy_blocks(blocks);

	_mae_link_break_runs(h_rx, h_ry, h_rlen, h_nruns, MAE_MARK_RUNS_GAP2);
	//_mae_remove_long_runs(h_rx, h_ry, h_rlen, h_nruns, line_width);
	_mae_remove_long_runs2(h_rx, h_ry, h_rlen, h_nruns, _width, line_width);


#ifdef MAE_DEBUG
	{
		MAE_image runs_image;
		runs_image.w = _width;
		runs_image.h = _height;
		runs_image.type = 1;
		runs_image.depth  = 24;
		runs_image.stride = _mae_width_bytes(24,_width );
		runs_image.data  = _mae_mallocate_image(w,h,24);
		_mae_save_h_runs_image(&runs_image, h_rx, h_ry, h_rlen, h_nruns );
		_mae_save_image("_mae_detect_vertlines_blocks_2.bmp",&runs_image);
		free(runs_image.data);
	}
#endif

	//20110124 add start
	if( run_list != NULL )
	{
		free(run_list);
		run_list = NULL;
	}
	//20110124 add end
	_mae_create_runs_chain(&run_list,&listnum , h_rx, h_ry, h_rlen, h_nruns);


	_mae_detect_blocks(run_list, listnum, &blocks, &nblocks);

	_mae_computer_blocks_rect(&blocks, &nblocks);
	//20101225 add END

	//20100831 add start
	*_v_lines = NULL;
	*_vlinenum = 0;
	if( nblocks == 0 )
	{
		free(h_rx);
		free(h_ry);
		free(h_rlen);
		//free(h_hist);
		free(run_list);
		_mae_destroy_blocks(blocks);
		return 0;
	}
	//20100831 add end
	//get regions
	_mae_get_lines_from_blocks_2(bi_data, _data, _width, _height, blocks, nblocks, _v_lines, _vlinenum);

	//computer region rect
	//_mae_computer_regions_rect(_mae_region, _nregions);

	//	_mae_remove_small_liness(_v_lines, _vlinenum,  MAE_VERT_LINE);

	_mae_set_line_type(_v_lines, _vlinenum, MAE_VERT_LINE);

#ifdef MAE_DEBUG
	{
		MAE_image runs_image;
		MAE_image mask;
		runs_image.w = _width;
		runs_image.h = _height;
		runs_image.type = 1;
		runs_image.depth  = 24;
		runs_image.stride = _mae_width_bytes(24,_width );
		runs_image.data  = _mae_mallocate_image(w,h,24);
		_mae_save_h_runs_image(&runs_image, h_rx, h_ry, h_rlen, h_nruns );
		_mae_save_image("_mae_detect_vertlines_blocks_3.bmp",&runs_image);
		//_mae_save_mark_image(&runs_image, *_mae_region, *_nregions, &mask, 5);
		//_mae_save_image("_mae_detect_vertlines_blocks_4.bmp",&mask);
		free(runs_image.data);
		//free(mask.data);
	}
#endif

	//merge region based on rect pos
	//20101110 test for lines start
	//_mae_merge_region_by_rect(_mae_region, _nregions);
	//20101110 test for lines end

	//remove small region based on rect pos
	//20100831 delete
	//20101110 test for lines start
	//_mae_remove_small_regions(_mae_region, _nregions, w, h);
	//20101110 test for lines end

	//adjust region edge based on rect pos
	//20100831 delete 
	//_mae_adjust_region_list_rect2(_data, _width, _height, _mae_region, _nregions);

	//adjust region based on rect pos
	//chen 20100816 delete because there are some wrong in this process 
	//_mae_adjust_region_list_rect(bi_data,_width, _height, _mae_region, _nregions);

	//merge region based on rect pos
	//20101110 test for lines start
	//_mae_merge_region_by_rect(_mae_region, _nregions);
	//20101110 test for lines end

	//remove small region based on rect pos
	//20101110 test for lines start
	//_mae_remove_small_regions(_mae_region, _nregions, w, h);
	//20101110 test for lines end

	//merge region based on rect pos
	//_mae_merge_break_region(_mae_region, _nregions);

#ifdef MAE_DEBUG
	{
		MAE_image runs_image;
		MAE_image mask;
		runs_image.w = _width;
		runs_image.h = _height;
		runs_image.type = 1;
		runs_image.depth  = 24;
		runs_image.stride = _mae_width_bytes(24,_width );
		runs_image.data  = _mae_mallocate_image(w,h,24);
		_mae_save_h_runs_image(&runs_image, h_rx, h_ry, h_rlen, h_nruns );
		_mae_save_image("_mae_detect_vertlines_blocks_5.bmp",&runs_image);
		//_mae_save_mark_image(&runs_image, *_mae_region, *_nregions, &mask, 5);
		//_mae_save_image("_mae_detect_vertlines_blocks_6.bmp",&mask);
		free(runs_image.data);
		//free(mask.data);
	}
#endif
	//check back color regions
	//20100819 delete
	//_mae_check_back_color_region_all1(_data, _width, _height, _mae_region, _nregions);

	//detect two region linked occasion 
	//_mae_detect_link_region_all(_data, _width, _height, _mae_region, _nregions);
	//


	//free(h_rx);
	//free(h_ry);
	//free(h_rlen);
	//free(h_hist);
	free(run_list);
	_mae_destroy_blocks(blocks);
	//*_mae_region = regions;
	//*_nregions = nregions;

	return 0;
}


//int _mae_detect_horilines_blocks(unsigned char *bi_data,unsigned char * _data, int _width, int _height, MAE_line **_lines, int *_linenum,
//							  MAE_node ** _run_list, int *_listnum)
int _mae_detect_horilines_blocks(unsigned char *bi_data,unsigned char * _data, int _width, int _height, 
								 int * rx, int * ry, int * rlen , int nruns, int v_line_width,
								 MAE_line **_lines, int *_linenum)
{
	//int *rx, *ry, *rlen, nruns, ralloc;
	//  unsigned char *data, *rdata;
	unsigned char *data;
	int w;
	int h;

	MAE_block *blocks;
	int nblocks;

	MAE_node * run_list;
	int listnum;
	unsigned char *_bi_data;

	int line_width;

	_bi_data = bi_data;

	data = _data;
	w = _width;
	h = _height;
	// detect vertical runs
	//rdata = _mae_mallocate_image(h, w, 8);
	//_mae_grayscale_rotate_90(data, rdata, w, h);
	//_mae_get_runs(&rx, &ry, &rlen, &nruns, &ralloc, rdata, h, w);
	//free(rdata);

	//20100901 add start
	//link break runs
	//   _mae_link_break_runs(rx, ry, rlen, nruns, MAE_MARK_RUNS_GAP2);
	//20100901 add end

	//remove long runs
	//_mae_remove_long_runs(rx, ry, rlen, nruns, MAE_MARK_LINE_WIDTH);
	//_mae_remove_long_runs(rx, ry, rlen, nruns, v_line_width);

#ifdef MAE_DEBUG
	{
		MAE_image runs_image;
		runs_image.w = _height;
		runs_image.h = _width;
		runs_image.type = 1;
		runs_image.depth  = 24;
		runs_image.stride = _mae_width_bytes(24,runs_image.w );
		runs_image.data  = _mae_mallocate_image(runs_image.w,runs_image.h,24);
		_mae_save_h_runs_image(&runs_image, rx, ry, rlen, nruns );
		_mae_save_image("_mae_detect_horilines_blocks_1.bmp",&runs_image);
		free(runs_image.data);
	}
#endif
	/* get run chain list */
	run_list = NULL;
	listnum = 0;
	_mae_create_runs_chain_split(&run_list,&listnum , rx, ry, rlen, nruns);

	//20100831 add start
	// *_run_list = run_list;
	// *_listnum = listnum;
	//20100831 add end

	//detect blocks
	_mae_detect_blocks(run_list, listnum, &blocks, &nblocks);


	_mae_computer_blocks_rect(&blocks, &nblocks);


	//20101225 add START
	line_width = _mae_compute_line_width_fromblocks(blocks, nblocks);
	_mae_destroy_blocks(blocks);
	// line_width = 15;

	_mae_link_break_runs(rx, ry, rlen, nruns, MAE_MARK_RUNS_GAP2);
	//_mae_remove_long_runs(rx, ry, rlen, nruns, line_width);
	_mae_remove_long_runs2(rx, ry, rlen, nruns, _height, line_width);



#ifdef MAE_DEBUG
	MAE_image runs_image;
	runs_image.w = _height;
	runs_image.h = _width;
	runs_image.type = 1;
	runs_image.depth  = 24;
	runs_image.stride = _mae_width_bytes(24,runs_image.w );
	runs_image.data  = _mae_mallocate_image(runs_image.w,runs_image.h,24);
	_mae_save_h_runs_image(&runs_image, rx, ry, rlen, nruns );
	_mae_save_image("_mae_detect_horilines_blocks_2.bmp",&runs_image);
	free(runs_image.data);
#endif

	if( run_list != NULL )
	{
		free(run_list);
		run_list = 0;
		listnum = 0;
	}
	_mae_create_runs_chain(&run_list,&listnum , rx, ry, rlen, nruns);


	_mae_detect_blocks(run_list, listnum, &blocks, &nblocks);

	_mae_computer_blocks_rect(&blocks, &nblocks);
	//20101225 add END


	//20100831 add start
	*_lines = NULL;
	*_linenum = 0;
	if( nblocks == 0 )
	{
		free(rx);
		free(ry);
		free(rlen);
		//free(h_hist);
		free(run_list);
		_mae_destroy_blocks(blocks);
		return 0;
	}
	//20100831 add end
	//get regions
	_mae_get_lines_from_blocks(blocks, nblocks, _lines, _linenum);

	//computer region rect
	//_mae_computer_regions_rect(_mae_region, _nregions);

	//	_mae_remove_small_liness(_lines, _linenum,  MAE_VERT_LINE);

	_mae_set_line_type(_lines, _linenum, MAE_HORI_LINE);

#ifdef MAE_DEBUG
	{
		MAE_image runs_image;
		MAE_image mask;
		runs_image.w = _height;
		runs_image.h = _width;
		runs_image.type = 1;
		runs_image.depth  = 24;
		runs_image.stride = _mae_width_bytes(24,runs_image.w );
		runs_image.data  = _mae_mallocate_image(h,w,24);
		_mae_save_h_runs_image(&runs_image, rx, ry, rlen, nruns );
		_mae_save_image("_mae_detect_horilines_blocks_3.bmp",&runs_image);
		//_mae_save_mark_image(&runs_image, *_mae_region, *_nregions, &mask, 5);
		//_mae_save_image("test_region_to_image4_1.bmp",&mask);
		free(runs_image.data);
		//free(mask.data);
	}
#endif

	//merge region based on rect pos
	//20101110 test for lines start
	//_mae_merge_region_by_rect(_mae_region, _nregions);
	//20101110 test for lines end

	//remove small region based on rect pos
	//20100831 delete
	//20101110 test for lines start
	//_mae_remove_small_regions(_mae_region, _nregions, w, h);
	//20101110 test for lines end

	//adjust region edge based on rect pos
	//20100831 delete 
	//_mae_adjust_region_list_rect2(_data, _width, _height, _mae_region, _nregions);

	//adjust region based on rect pos
	//chen 20100816 delete because there are some wrong in this process 
	//_mae_adjust_region_list_rect(bi_data,_width, _height, _mae_region, _nregions);

	//merge region based on rect pos
	//20101110 test for lines start
	//_mae_merge_region_by_rect(_mae_region, _nregions);
	//20101110 test for lines end

	//remove small region based on rect pos
	//20101110 test for lines start
	//_mae_remove_small_regions(_mae_region, _nregions, w, h);
	//20101110 test for lines end

	//merge region based on rect pos
	//_mae_merge_break_region(_mae_region, _nregions);

#ifdef MAE_DEBUG
	{
		MAE_image runs_image;
		MAE_image mask;
		runs_image.w = _height;
		runs_image.h = _width;
		runs_image.type = 1;
		runs_image.depth  = 24;
		runs_image.stride = _mae_width_bytes(24,runs_image.w );
		runs_image.data  = _mae_mallocate_image(h,w,24);
		_mae_save_h_runs_image(&runs_image, rx, ry, rlen, nruns );
		_mae_save_image("_mae_detect_horilines_blocks_4.bmp",&runs_image);
		//_mae_save_mark_image(&runs_image, *_mae_region, *_nregions, &mask, 5);
		//_mae_save_image("test_region_to_image4.bmp",&mask);
		free(runs_image.data);
		//free(mask.data);
	}
#endif
	//check back color regions
	//20100819 delete
	//_mae_check_back_color_region_all1(_data, _width, _height, _mae_region, _nregions);

	//detect two region linked occasion 
	//_mae_detect_link_region_all(_data, _width, _height, _mae_region, _nregions);
	//


	//free(rx);
	//free(ry);
	//free(rlen);
	//free(h_hist);
	//free(run_list);
	_mae_destroy_blocks(blocks);
	//*_mae_region = regions;
	//*_nregions = nregions;

	return 0;
}

int _mae_get_regions_from_blocks(MAE_block *blocks, int nblocks, MAE_region **_mae_region, int *_nregions)
{
	int i,j;
	//int * count_label;
	//int label;

	unsigned char *region_data;
	//MAE_region *_regions;
	int region_num;
	int idx;
	////////////////////////////////////////////////
	//FILE *fp = fopen("E:\\sharp\\runs_1.txt","w");
	//if(fp!=NULL)
	//{
	//	for(i = 0; i < nblocks; i ++ )
	//	{
	//		fprintf(fp,"this is ---%d--%d----block\n",i,blocks[i].label );
	//		MAE_node *head = blocks[i].head;
	//		while(head != NULL )
	//		{
	//			fprintf(fp,"%d, %d, %d\n",head->y,head->x,head->len );
	//			head = head->next;
	//		}
	//	}
	//	fclose(fp);
	//}
	////////////////////////////////////////////////

	////////////////////////////////////////
	//_mae_malloc_int(&count_label, nblocks, "_mae_get_regions_from_blocks : label");
	//for(i = 0; i < nblocks; i ++ )
	//{
	//	count_label[i]  = 0;
	//}

	//for(i = 0; i < nblocks; i ++ )
	//{
	//	label = blocks[i].label;
	//	count_label[label] ++;
	//}
	//region_num = 0;
	//for(i = 0; i < nblocks; i ++ )
	//{
	//	if(count_label[i] > 0 )
	//	{
	//		region_num ++;
	//	}
	//}
	////region_num = nblocks;

	//_mae_malloc_uchar(&region_data, region_num * sizeof(MAE_region),"_mae_get_regions_from_blocks : region");
	//*_mae_region = (MAE_region *)region_data;
	//*_nregions = region_num;
	////initialize
	//for( i = 0; i < region_num; i ++ )
	//{
	//	(*_mae_region)[i].type = 0;
	//	(*_mae_region)[i].label = 0;
	//	(*_mae_region)[i].l = 0;
	//	(*_mae_region)[i].t = 0;
	//	(*_mae_region)[i].w = 0;
	//	(*_mae_region)[i].h = 0;
	//	(*_mae_region)[i].head = NULL;
	//	(*_mae_region)[i].tail = NULL;
	//}
	//
	//idx = 0;
	//for(i = 0; i < nblocks; i ++ )
	//{
	//	if(count_label[i] > 0 )
	//	{
	//		(*_mae_region)[idx].type = 0;
	//		(*_mae_region)[idx].label = i;
	//		(*_mae_region)[idx].head = NULL;
	//		(*_mae_region)[idx].tail = NULL;
	//		for(k = 0; k < nblocks; k ++ )
	//		{
	//			if( blocks[k].label == i )
	//			{
	//				_mae_add_node_list(&((*_mae_region)[idx].head), &((*_mae_region)[idx].tail),  blocks[k].head, blocks[k].tail);
	//			}
	//		}

	//		idx++;
	//	}
	//}
	////////////////////////////////////////////////
	//fp = fopen("E:\\sharp\\±EÇ·Ö¸ûx\data\\runs_2.txt","w");
	//if(fp!=NULL)
	//{
	//	for(i = 0; i < region_num; i ++ )
	//	{
	//		fprintf(fp,"this is ---%d---region\n",(*_mae_region)[i].label );
	//		MAE_node *head = (*_mae_region)[i].head;
	//		while(head != NULL )
	//		{
	//			fprintf(fp,"%d, %d, %d\n",head->y,head->x,head->len );
	//			head = head->next;
	//		}
	//	}
	//	fclose(fp);
	//}
	////////////////////////////////////////////////

	/////////////////////////////////////////////
	region_num = 0;
	for( i = 0; i < nblocks; i ++ )
	{
		if( blocks[i].label != -1 )
		{
			region_num ++;
		}
	}

	if( region_num == 0 )
	{
		return 0;
	}

	_mae_malloc_uchar(&region_data, region_num * sizeof(MAE_region),"_mae_get_regions_from_blocks : region");
	*_mae_region = (MAE_region *)region_data;
	*_nregions = region_num;
	//initialize
	for( i = 0; i < region_num; i ++ )
	{
		(*_mae_region)[i].type = 0;
		(*_mae_region)[i].label = 0;
		(*_mae_region)[i].l = 0;
		(*_mae_region)[i].t = 0;
		(*_mae_region)[i].w = 0;
		(*_mae_region)[i].h = 0;
		(*_mae_region)[i].head = NULL;
		(*_mae_region)[i].tail = NULL;
		for( j = 0; j < EDGE_MAX_POINT; j++)
		{
			(*_mae_region)[i].linenum = 0;
			(*_mae_region)[i].lines[j].sx = 0;
			(*_mae_region)[i].lines[j].sy = 0;
			(*_mae_region)[i].lines[j].type = 0;
			(*_mae_region)[i].lines[j].ex = 0;
			(*_mae_region)[i].lines[j].ey = 0;
			(*_mae_region)[i].lines[j].lw = 0;
		}
	}
	idx = 0;
	for(i = 0; i < nblocks; i ++ )
	{
		if(blocks[i].label == -1 ) continue;

		(*_mae_region)[idx].type = 0;
		//(*_mae_region)[idx].label = i;
		(*_mae_region)[idx].label = 0;
		(*_mae_region)[idx].head  = blocks[i].head; 
		(*_mae_region)[idx].tail = blocks[i].tail; 

		idx ++;

	}

	return 0;
}

int _mae_get_lines_from_blocks(MAE_block *blocks, int nblocks, MAE_line **_lines, int *_vlinenum)
{
	int i,j;

	unsigned char *line_data;
	int line_num;
	int idx;

	line_num = 0;
	for( i = 0; i < nblocks; i ++ )
	{
		if( blocks[i].label != -1 )
		{
			line_num ++;
		}
	}

	if( line_num == 0 )
	{
		return 0;
	}

	_mae_malloc_uchar(&line_data, line_num * sizeof(MAE_line),"_mae_get_lines_from_blocks : lines");
	*_lines = (MAE_line *)line_data;
	*_vlinenum = line_num;
	//initialize
	for( i = 0; i < line_num; i ++ )
	{
		(*_lines)[i].type = 0;
		(*_lines)[i].label = 0;
		(*_lines)[i].l = 0;
		(*_lines)[i].t = 0;
		(*_lines)[i].w = 0;
		(*_lines)[i].h = 0;
		(*_lines)[i].node = NULL;
		(*_lines)[i].re = 0;
		(*_lines)[i].ge = 0;
		(*_lines)[i].be = 0;

	}
	idx = 0;
	for(i = 0; i < nblocks; i ++ )
	{
		if(blocks[i].label == -1 ) continue;

		(*_lines)[idx].type = 0;
		(*_lines)[idx].l = blocks[i].l;
		(*_lines)[idx].t = blocks[i].t;
		(*_lines)[idx].w = blocks[i].w;
		(*_lines)[idx].h = blocks[i].h;

		idx ++;

	}

	return 0;
}

int _mae_get_line_from_block_by_analyse_projection(unsigned char* _bi_data, unsigned char* _data, int _width, int _height, MAE_block *block, MAE_line *line)
{
	return 0;
}
int _mae_get_lines_from_blocks_2(unsigned char* _bi_data, unsigned char* _data, int _width, int _height, MAE_block *blocks, int nblocks, MAE_line **_lines, int *_vlinenum)
{
	int i,j;

	unsigned char *line_data;
	int line_num;
	int idx;

	line_num = 0;
	for( i = 0; i < nblocks; i ++ )
	{
		if( blocks[i].label != -1 )
		{
			line_num ++;
		}
	}

	if( line_num == 0 )
	{
		return 0;
	}

	_mae_malloc_uchar(&line_data, line_num * sizeof(MAE_line),"_mae_get_lines_from_blocks : lines");
	*_lines = (MAE_line *)line_data;
	*_vlinenum = line_num;
	//initialize
	for( i = 0; i < line_num; i ++ )
	{
		(*_lines)[i].type = 0;
		(*_lines)[i].label = 0;
		(*_lines)[i].l = 0;
		(*_lines)[i].t = 0;
		(*_lines)[i].w = 0;
		(*_lines)[i].h = 0;
		(*_lines)[i].node = NULL;
		(*_lines)[i].re = 0;
		(*_lines)[i].ge = 0;
		(*_lines)[i].be = 0;

	}
	idx = 0;
	for(i = 0; i < nblocks; i ++ )
	{
		if(blocks[i].label == -1 ) continue;

		(*_lines)[idx].type = 0;
		(*_lines)[idx].l = blocks[i].l;
		(*_lines)[idx].t = blocks[i].t;
		(*_lines)[idx].w = blocks[i].w;
		(*_lines)[idx].h = blocks[i].h;

		idx ++;

	}

	return 0;
}
void _mae_com_region_rect(MAE_region *region)
{
	//MAE_node *head;
	MAE_node *p;
	int _l,_t,_r,_b;
	int i;

	_l = 10000;
	_t = 10000;
	_r = 0;
	_b = 0;
	p =  region->head;
	while(p != NULL )
	{
		if( _l > p->x ) _l = p->x;
		if( _r < p->x + p->len ) _r = p->x + p->len;
		if( _t > p->y ) _t = p->y;
		if( _b < p->y  ) _b = p->y;
		p = p->next;
	}		
	//20100826 add start
	//if( _l > 0 ) _l--;
	//if( _t > 0 ) _t--;
	//if( _r < w - 1 ) _r++;
	//if( _b < h - 1 ) _b++;
	//20100826 add end

	region->l = _l;
	region->t = _t;
	region->w = _r-_l;
	region->h = _b-_t;

	return;
}

void _mae_computer_regions_rect(MAE_region **_mae_region, int *_nregions)
{
	int i;
	int region_num;
	//MAE_node *head;
	//MAE_node *p;
	region_num = *_nregions;

	//int _l,_t,_r,_b;
	for(i = 0; i < region_num; i ++ )
	{
		_mae_com_region_rect(&((*_mae_region)[i]));
		//_l = 10000;
		//_t = 10000;
		//_r = 0;
		//_b = 0;
		//p =  (*_mae_region)[i].head;
		//while(p != NULL )
		//{
		//	if( _l > p->x ) _l = p->x;
		//	if( _r < p->x + p->len ) _r = p->x + p->len;
		//	if( _t > p->y ) _t = p->y;
		//	if( _b < p->y  ) _b = p->y;
		//	p = p->next;
		//}		
		//(*_mae_region)[i].l = _l;
		//(*_mae_region)[i].t = _t;
		//(*_mae_region)[i].w = _r-_l;
		//(*_mae_region)[i].h = _b-_t;
	}
	return;
}


void _mae_merge_region_by_rect(MAE_region **_mae_region, int *_nregions)
{
	int i,j;
	int region_num;

	//MAE_node *head;
	//MAE_node *p;

	int l1,t1,w1,h1;
	int l2,t2,w2,h2;
	region_num = *_nregions;

	for(i = 0; i < region_num; i ++ )
	{
		if( (*_mae_region)[i].label == -1 ) continue;
		for(j = 0; j < region_num; j ++ )
		{
			if( j == i ) continue;
			if( (*_mae_region)[j].label == -1 ) continue;

			l1 = (*_mae_region)[i].l;
			t1 = (*_mae_region)[i].t;
			w1 = (*_mae_region)[i].w;
			h1 = (*_mae_region)[i].h;
			l2 = (*_mae_region)[j].l;
			t2 = (*_mae_region)[j].t;
			w2 = (*_mae_region)[j].w;
			h2 = (*_mae_region)[j].h;
			if(_mae_check_region(l1, t1, w1, h1, l2, t2, w2, h2))
			{
				_mae_add_node_list(&((*_mae_region)[i].head), &((*_mae_region)[i].tail), (*_mae_region)[j].head, (*_mae_region)[j].tail );
				(*_mae_region)[j].label = -1;
				(*_mae_region)[j].head = NULL;
				(*_mae_region)[j].tail = NULL;
				_mae_com_region_rect(&((*_mae_region)[i]));
			}
			//else if( _mae_is_link_region( &((*_mae_region)[i]), &((*_mae_region)[j]) ,MAE_MARK_LINE_GAP) ){
			//	_mae_add_node_list(&((*_mae_region)[i].head), &((*_mae_region)[i].tail), (*_mae_region)[j].head, (*_mae_region)[j].tail );
			//	(*_mae_region)[j].label = -1;
			//	(*_mae_region)[j].head = NULL;
			//	(*_mae_region)[j].tail = NULL;
			//	_mae_com_region_rect(&((*_mae_region)[i]));
			//}
		}
	}
	return;	
}

void _mae_merge_region_by_rect2(MAE_region **_mae_region, int *_nregions)
{
	int i,j;
	int region_num;

	//MAE_node *head;
	//MAE_node *p;

	int l1,t1,r1,b1,w1,h1;
	int l2,t2,r2,b2,w2,h2;
	region_num = *_nregions;

	for(i = 0; i < region_num; i ++ )
	{
		if( (*_mae_region)[i].label == -1 ) continue;
		for(j = 0; j < region_num; j ++ )
		{
			if( j == i ) continue;
			if( (*_mae_region)[j].label == -1 ) continue;

			l1 = (*_mae_region)[i].l;
			t1 = (*_mae_region)[i].t;
			w1 = (*_mae_region)[i].w;
			h1 = (*_mae_region)[i].h;
			l2 = (*_mae_region)[j].l;
			t2 = (*_mae_region)[j].t;
			w2 = (*_mae_region)[j].w;
			h2 = (*_mae_region)[j].h;
			if(_mae_check_region(l1, t1, w1, h1, l2, t2, w2, h2))
			{
				l1 = (*_mae_region)[i].l;
				t1 = (*_mae_region)[i].t;
				r1 = (*_mae_region)[i].l + (*_mae_region)[i].w;
				b1 = (*_mae_region)[i].t + (*_mae_region)[i].h;
				l2 = (*_mae_region)[j].l;
				t2 = (*_mae_region)[j].t;
				r2 = (*_mae_region)[j].l + (*_mae_region)[j].w;
				b2 = (*_mae_region)[j].t + (*_mae_region)[j].h;
				if( l1 > l2 ) l1 = l2;
				if( t1 > t2 ) t1 = t2;
				if( r1 < r2 ) r1 = r2;
				if( b1 < b2 ) b1 = b2;
				(*_mae_region)[i].l = l1;
				(*_mae_region)[i].t = t1;
				(*_mae_region)[i].w = r1 - l1;
				(*_mae_region)[i].h = b1 - t1;
				(*_mae_region)[j].label = MAE_MARK_REGION_NO;
				(*_mae_region)[i].label = MAE_MARK_REGION_MERGE_NEW;
			}

		}
	}
	return;	
}
int _mae_get_region_pixnum(MAE_region *region)
{
	MAE_node *p;
	int pix_num;
	p = region->head;
	pix_num = 0;
	while(p != NULL)
	{
		pix_num = pix_num + p->len;
		p = p->next;
	}
	return pix_num;
}
void _mae_remove_small_regions(MAE_region **_mae_region, int *_nregions, int w, int h)
{
	int i;
	long pixnum;
	MAE_region * region;
	int nregions;
	int width;
	int height;
	//double s_ratio, w_ratio, h_ratio;
	//double w_ratio, h_ratio;
	int _w;
	int _h;
	_w = w;
	_h = h;

	nregions = *_nregions;
	for(i = 0; i < nregions; i ++ )
	{
		region = &((*_mae_region)[i]);
		width = region->w;
		height = region->h;
		//s_ratio = (double)pixnum / (double)(w * h);
		//w_ratio = (double)width / (double)w;
		//h_ratio = (double)height / (double)h;
		//if(    w_ratio < MAE_SMALL_REGION_RATIO 
		//	&& h_ratio < MAE_SMALL_REGION_RATIO ) 
		//{
		//	region->label = -1;
		//	continue;
		//}
		if(    width < MAE_NORMAL_LINE_WIDTH 
			&& height < MAE_NORMAL_LINE_WIDTH ) 
		{
			region->label = -1;
			continue;
		}
		pixnum = _mae_get_region_pixnum(region);
		if( pixnum < MAE_REGION_PIXS_THRESHOLD )
		{
			region->label = -1;
		}
	}
}
void _mae_set_line_type(MAE_line **_lines, int *_num, int type)
{
	int i;
	MAE_line * line;
	int num;

	num = *_num;
	for(i = 0; i < num; i ++ )
	{
		line = &((*_lines)[i]);
		line->type = type;
	}
}
void _mae_remove_small_liness(MAE_line **_lines, int *_lnum, int flag)
{
	int i;
	long pixnum;
	MAE_line * line;
	int num;
	int width;
	int height;


	num = *_lnum;
	for(i = 0; i < num; i ++ )
	{
		line = &((*_lines)[i]);
		width = line->w;
		height = line->h;
		if( width < MAE_SMALL_LINE_WIDTH && height < MAE_SMALL_LINE_HEIGHT )
		{
			line->label = -1;
		}
		if( flag == MAE_VERT_LINE )
		{
			if(width > height )
			{
				line->label = -1;
			}
		}else{
			if(width < height )
			{
				line->label = -1;
			}
		}
	}
}

void _mae_remove_small_regions2(MAE_region **_mae_region, int *_nregions, int w, int h)
{
	int i;
	//long pixnum;
	MAE_region * region;
	int nregions;
	int width;
	int height;
	//double s_ratio, w_ratio, h_ratio;
	//	double w_ratio, h_ratio;
	int _width;
	int _height;
	_width = w;
	_height = h;

	nregions = *_nregions;
	for(i = 0; i < nregions; i ++ )
	{
		region = &((*_mae_region)[i]);
		if( region->label == MAE_MARK_REGION_NO ) continue;

		width = region->w;
		height = region->h;
		if(    (width * height) < MAE_MIN_MARK_AREA 
			) 
		{
			region->label = -1;
			continue;
		}
	}
}
int _mae_check_region(int l1, int t1, int w1, int h1, int l2, int t2, int w2, int h2)
{
	int left1,top1, right1, bottom1;
	int left2,top2, right2, bottom2;
	int i;
	//20110125 add start 
	int flag1;
	int flag2;
	flag1 = 0;
	flag2 = 0;
	if( w1 > h1)
	{
		flag1 = 0; //hori line
	}else if( w1 < h1){
		flag1 = 1; //vert line
	}else{
		flag1 = 2;   //square
	}
	if( w2 > h2)
	{
		flag2 = 0; //hori line
	}else if( w2 < h2){
		flag2 = 1; //vert line
	}else{
		flag2 = 2;   //square
	}
	if( flag1 ==2 || flag2 == 2 )
	{
		;
	}else{
		if(flag1 != flag2 )
		{
			return 0;
		}
	}

	//20110125 add end
	for( i = 0; i < 2; i ++ )
	{
		if( i == 0 )
		{
			//left1 = l1 -2;
			//top1 = t1  - 2;
			//right1 = l1 + w1 + 2;
			//bottom1 = t1 + h1 + 2;

			//left2 = l2 - 2;
			//top2 = t2 - 2;
			//right2 = l2 + w2 + 2;
			//bottom2 = t2 + h2 + 2;
			left1 = l1 -1;
			top1 = t1  - 1;
			right1 = l1 + w1 + 1;
			bottom1 = t1 + h1 + 1;

			left2 = l2 - 1;
			top2 = t2 - 1;
			right2 = l2 + w2 + 1;
			bottom2 = t2 + h2 + 1;
		}else{
			//left1 = l2 - 2;
			//top1 = t2 - 2;
			//right1 = l2 + w2 + 2;
			//bottom1 = t2 + h2 + 2;

			//left2 = l1 - 2;
			//top2 = t1 - 2;
			//right2 = l1 + w1 + 2;
			//bottom2 = t1 + h1 + 2;
			left1 = l2 - 1;
			top1 = t2 - 1;
			right1 = l2 + w2 + 1;
			bottom1 = t2 + h2 + 1;

			left2 = l1 - 1;
			top2 = t1 - 1;
			right2 = l1 + w1 + 1;
			bottom2 = t1 + h1 + 1;
		}
		if(    left1 >= left2 
			&& left1 <= right2 
			&& top1 >= top2 
			&& top1 <= bottom2 
			)
		{
			return 1;
		}
		if(    right1 >= left2 
			&& right1 <= right2 
			&& top1 >= top2 
			&& top1 <= bottom2 
			)
		{
			return 1;
		}
		if(    left1 >= left2 
			&& left1 <= right2 
			&& bottom1 >= top2 
			&& bottom1 <= bottom2 
			)
		{
			return 1;
		}
		if(    right1 >= left2 
			&& right1 <= right2 
			&& bottom1 >= top2 
			&& bottom1 <= bottom2 
			)
		{
			return 1;
		}
	}

	return 0;
}


int _mae_save_regions(char* filename, MAE_image *in_image, MAE_region *mae_regions, int nregions)
{
	int error = MAE_NO_ERROR;
	int i;
	int idx;
	char region_file_name[1024]; 
	strcpy(region_file_name, "" );

	idx = 1;
	for(i = 0; i < nregions; i ++ )
	{
		//do not show region deleted
		if( mae_regions[i].label == -1 ) continue;
		sprintf(region_file_name,"%s_region_%d.bmp",filename,idx);
		idx++;
		_mae_save_region(region_file_name, in_image, &mae_regions[i]);
		if(error != MAE_NO_ERROR )
		{
			return error;
		}
	}
	return error;
}

int _mae_save_region(char* filename, MAE_image *in_image, MAE_region *region)
{
	int error = MAE_NO_ERROR;
	int left;
	int top;
	int width;
	int height;
	int i,j, k;
	int depth;
	int movedbits;
	long in_widthbytes;
	long out_widthbytes;

	unsigned char *in_data;
	unsigned char *out_data;
	unsigned char *in_temp;
	unsigned char *out_temp;
	MAE_image out_image;

	out_image.depth = in_image->depth;
	out_image.type = in_image->type;
	out_image.w  = region->w;
	out_image.h = region->h;
	out_image.stride = _mae_width_bytes(  out_image.depth, out_image.w );
	out_image.data  = _mae_mallocate_image(out_image.w,out_image.h,out_image.depth);


	depth = in_image->depth;
	movedbits = 1;
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

	in_widthbytes = _mae_width_bytes(depth, in_image->w);
	out_widthbytes = _mae_width_bytes(depth, out_image.w);
	left = region->l;
	top = region->t;
	width = region->w ;
	height = region->h;

	for(j = 0; j < height; j ++ )
	{
		out_data = out_image.data + out_widthbytes * j;
		in_data = in_image->data+ in_widthbytes * (j + top);
		for(i = 0; i < width; i ++ )
		{
			out_temp = out_data + movedbits * i;
			in_temp = in_data + movedbits * (i + left);
			for( k = 0; k < movedbits; k ++ )
			{
				*(out_temp + k) = *(in_temp + k);
			}
		}
	}

	error = _mae_save_image(filename, &out_image );

	free(out_image.data );
	return error;
}

void _mae_adjust_region_list_rect(unsigned char * data, int width, int height, MAE_region **_mae_region, int *_nregions)
{
	int i;

	for(i = 0; i < *_nregions; i ++ )
	{
		if( (*_mae_region)[i].label == -1 ) continue;
		_mae_adjust_region_rect(data, width, height, &((*_mae_region)[i]));
	}

}
void _mae_adjust_region_rect(unsigned char * data, int width, int height, MAE_region *region)
{
	int left, top,right, bottom;
	unsigned char *curr_data;
	unsigned char *next_data;
	unsigned char *curr;
	unsigned char *next;
	//unsigned char *temp;
	int count1;
	int count2;
	int x, y;

	left = region->l;
	top = region->t;
	right = left + region->w;
	bottom = top + region->h;

	// detect top and bottom  connect row
	_mae_malloc_uchar(&curr_data, width, "_mae_adjust_region_rect : curr_check");
	_mae_malloc_uchar(&next_data, width, "_mae_adjust_region_rect : curr_check");
	memset(curr_data, (unsigned char)0, width);
	memset(next_data, (unsigned char)0, width);

	//detect top connect row 
	//up move  
	curr = curr_data;
	next = next_data;
	y = top ;
	_mae_get_8_row_data(data,width,height,curr,y,left,right);
	count1 = 0;
	while( _mae_get_8_connect_row(data, width, height, &y, curr, next, MAE_REGION_RECT_T) )
	{
		//curr = next;
		memcpy(curr, next, width);
		memset(next,(unsigned char)0,width);
		count1 ++;
		//if( count > MAE_REGION_RECT_CONNECT ) break;
		if( y <= 0 ) break;
	}
	//down move  
	curr = curr_data;
	next = next_data;
	y = top ;
	_mae_get_8_row_data(data,width,height,curr,y,left,right);
	count2 = 0;
	while( _mae_get_8_connect_row(data, width, height, &y, curr, next, MAE_REGION_RECT_B) )
	{
		//curr = next;
		memcpy(curr, next, width);
		memset(next,(unsigned char)0,width);
		count2 ++;
		//if( count > MAE_REGION_RECT_CONNECT ) break;
		if( y >= height ) break;
	}

	//if( count <= MAE_REGION_RECT_CONNECT )
	//{
	//	region->t = y; 
	//}
	if( count1 > MAE_REGION_RECT_CONNECT )
	{
		count1 = MAE_REGION_RECT_CONNECT;
	}
	if( count2 > MAE_REGION_RECT_CONNECT )
	{
		count2 = MAE_REGION_RECT_CONNECT;
	}
	if( count1 > 0 && (count1 < count2 || count2 >= region->h) )
	{
		region->t = region->t - count1; 
		region->h = region->h + count1;
	}

	//detect right connect row 
	//down move
	curr = curr_data;
	next = next_data;

	y = bottom;
	_mae_get_8_row_data(data,width,height,curr,y,left,right);
	count1 = 0;
	while( _mae_get_8_connect_row(data, width, height, &y, curr, next, MAE_REGION_RECT_B) )
	{
		//curr = next;
		memcpy(curr, next,  width);
		memset(next,(unsigned char)0,width);
		count1 ++;
		//if( count > MAE_REGION_RECT_CONNECT ) break;
		if( y == height - 1 ) break;
	}
	//up move
	curr = curr_data;
	next = next_data;

	y = bottom;
	_mae_get_8_row_data(data,width,height,curr,y,left,right);
	count2 = 0;
	while( _mae_get_8_connect_row(data, width, height, &y, curr, next, MAE_REGION_RECT_T) )
	{
		//curr = next;
		memcpy(curr, next,  width);
		memset(next,(unsigned char)0,width);
		count2 ++;
		//if( count > MAE_REGION_RECT_CONNECT ) break;
		if( y == height - 1 ) break;
	}
	//if( count <= MAE_REGION_RECT_CONNECT )
	//{
	//	region->h = y - region->t;
	//}
	if( count1 > MAE_REGION_RECT_CONNECT )
	{
		count1 = MAE_REGION_RECT_CONNECT;
	}

	if( count1 > 0 && (count1 < count2 || count2 >= region->h) )
	{
		region->h = region->h + count1; 
	}

	free(curr_data);
	free(next_data);


	// detect left and right  connect row
	_mae_malloc_uchar(&curr_data, height, "_mae_adjust_region_rect : curr_check");
	_mae_malloc_uchar(&next_data, height, "_mae_adjust_region_rect : curr_check");
	memset(curr_data, (unsigned char)0, height);
	memset(next_data, (unsigned char)0, height);

	//detect top connect row 
	//left move
	curr = curr_data;
	next = next_data;
	x = left;
	_mae_get_8_col_data(data,width,height,curr,x,top,bottom);
	count1 = 0;
	while( _mae_get_8_connect_col(data, width, height, &x, curr, next, MAE_REGION_RECT_L) )
	{
		//curr = next;
		memcpy(curr,next,height);
		count1 ++;
		//if( count > MAE_REGION_RECT_CONNECT ) break;
		if( x <= 0 ) break;
	}
	//right move
	curr = curr_data;
	next = next_data;
	x = left;
	_mae_get_8_col_data(data,width,height,curr,x,top,bottom);
	count2 = 0;
	while( _mae_get_8_connect_col(data, width, height, &x, curr, next, MAE_REGION_RECT_R) )
	{
		//curr = next;
		memcpy(curr,next,height);
		count2 ++;
		//if( count > MAE_REGION_RECT_CONNECT ) break;
		if( x >= width ) break;
	}

	if( count1 > MAE_REGION_RECT_CONNECT )
	{
		count1 = MAE_REGION_RECT_CONNECT;
	}
	if( count1 > 0 
		&& ( count1 < count2  ||  count2 >= region->w ))
	{
		region->l = region->l - count1; 
		region->w = region->w + count1; 
	}

	//detect right connect row 
	// right move
	curr = curr_data;
	next = next_data;
	x = right;
	_mae_get_8_col_data(data,width,height,curr, x, top, bottom);
	count1 = 0;
	while( _mae_get_8_connect_col(data, width, height, &x, curr, next, MAE_REGION_RECT_R) )
	{
		memcpy(curr,next,height);
		memset(next,(unsigned char)0,height);
		count1 ++;
		//if( count > MAE_REGION_RECT_CONNECT ) break;
		if( x == width - 1 ) break;
	}
	// left move
	curr = curr_data;
	next = next_data;
	x = right;
	_mae_get_8_col_data(data,width,height,curr, x, top, bottom);
	count1 = 0;
	while( _mae_get_8_connect_col(data, width, height, &x, curr, next, MAE_REGION_RECT_L) )
	{
		memcpy(curr,next,height);
		memset(next,(unsigned char)0,height);
		count1 ++;
		//if( count > MAE_REGION_RECT_CONNECT ) break;
		if( x == width - 1 ) break;
	}
	if( count1 > MAE_REGION_RECT_CONNECT )
	{
		count1 = MAE_REGION_RECT_CONNECT; 
	}
	if( count1 > 0 && (count1 < count2 || count2 > region->w ) )
	{
		region->w = region->w + count1; 
	}
	free(curr_data);
	free(next_data);

}

int _mae_get_8_connect_row(unsigned char * data, int width, int height, int *_y_ , unsigned char * curr, unsigned char * next, int direct)
{
	int x, y;
	unsigned char *n_temp;
	unsigned char color;
	int flag;
	flag = 0;

	if(direct == MAE_REGION_RECT_T )
	{
		y = *_y_ - 1;
	}else{
		y = *_y_ + 1;
	}
	*_y_ = y;

	if( y < 0 || y >= height )
	{
		return 0;
	}
	n_temp = data + y * width;
	//left pix
	x = 0;
	if( curr[x] != 0 )
	{
		color = *(n_temp + x );
		if( color )
		{
			next[x] = 1;
			flag = 1;
		}
		color = *(n_temp + x + 1);
		if( color )
		{
			next[x + 1] = 1;
			flag = 1;
		}
	}
	//left pix
	x = width - 1;
	if( curr[x] != 0 )
	{
		color = *(n_temp + x - 1 );
		if( color )
		{
			next[x-1] = 1;
			flag = 1;
		}
		color = *(n_temp + x );
		if( color )
		{
			next[x] = 1;
			flag = 1;
		}
	}
	for(x = 1; x < width-1; x ++ )
	{	
		if( curr[x] != 0 )
		{
			color = *(n_temp + x - 1 );
			if( color )
			{
				next[x-1] = 1;
				flag = 1;
			}
			color = *(n_temp + x );
			if( color )
			{
				next[x] = 1;
				flag = 1;
			}
			color = *(n_temp + x + 1);
			if( color )
			{
				next[x + 1] = 1;
				flag = 1;
			}
		}
	}
	return flag;
}
int _mae_get_8_connect_col(unsigned char * data, int width, int height, int *_x_, unsigned char * curr, unsigned char * next, int direct)
{
	int x, y;
	//unsigned char *temp;
	unsigned char *n_temp;
	unsigned char color;
	int flag;
	flag = 0;

	if(direct == MAE_REGION_RECT_L )
	{
		x = *_x_ - 1;
	}else{
		x = *_x_ + 1;
	}
	*_x_ = x;

	if( x < 0 || x >= width )
	{
		return 0;
	}
	//top pix
	y = 0;
	if( curr[y] != 0 )
	{
		n_temp = data + y * width + x;
		color = *n_temp;
		if( color )
		{
			next[y] = 1;
			flag = 1;
		}
		n_temp = data + (y + 1) * width + x;
		color = *n_temp;
		if( color )
		{
			next[y + 1] = 1;
			flag = 1;
		}
	}
	//bottom pix
	y = height - 1;
	if( curr[y] != 0 )
	{
		n_temp = data + y * width + x;
		color = *n_temp;
		if( color )
		{
			next[y] = 1;
			flag = 1;
		}
		n_temp = data + (y - 1) * width + x;
		color = *n_temp;
		if( color )
		{
			next[y - 1] = 1;
			flag = 1;
		}

	}
	for(y = 1; y < height - 1; y ++ )
	{	
		if( curr[y] != 0 )
		{
			n_temp = data + (y - 1) * width + x;
			color = *n_temp;
			if( color )
			{
				next[y-1] = 1;
				flag = 1;
			}
			n_temp = data + y * width + x;
			color = *n_temp;
			if( color )
			{
				next[y] = 1;
				flag = 1;
			}
			n_temp = data + (y+1) * width + x;
			color = *n_temp;
			if( color )
			{
				next[y + 1] = 1;
				flag = 1;
			}
		}
	}
	return flag;
}

void _mae_get_8_row_data(unsigned char * data, int width, int height, unsigned char * row, int y, int x1, int x2)
{
	int x;
	unsigned char *temp;
	//warning
	int _height;
	_height = height;
	//
	temp = data + y * width;
	for(x = x1; x < x2; x ++ )
	{	
		row[x]= *(temp + x);
		//if(row[x] != 0 )
		//{
		// int a = 0;
		//}
	}
}
void _mae_get_8_col_data(unsigned char * data, int width, int height, unsigned char * col, int x, int y1, int y2)
{
	int y;
	unsigned char *temp;
	//warning
	int _height;
	_height = height;
	//

	for ( y = y1 ; y < y2 ; y++)
	{
		temp = data + y * width;
		col[y] = *(temp + x);
	}

}

int _mae_save_mark_image(MAE_image *in_image, MAE_region *mae_regions, int nregions, MAE_image *out_image, int flag)
{
	int error = MAE_NO_ERROR;
	int left;
	int top;
	int right;
	int bottom;
	//int i,j,x,y;
	int i,j,x,y;
	int depth;
	int movedbits;
	long widthbytes;
	unsigned char bkcolor;
	unsigned char *data;
	unsigned char *temp;
	unsigned char *in_data;
	unsigned char *in_temp;
	unsigned char *out_data;
	unsigned char *out_temp;

	bkcolor = 255;

	out_image->xdpi = in_image->xdpi;
	out_image->ydpi = in_image->ydpi;
	out_image->depth = in_image->depth;
	out_image->type = in_image->type;
	out_image->w  = in_image->w;
	out_image->h = in_image->h;
	out_image->stride = _mae_width_bytes(  out_image->depth, out_image->w );
	out_image->data  = _mae_mallocate_image(out_image->w,out_image->h,out_image->depth);

	memset(out_image->data, (BYTE)bkcolor, _mae_sizefromdepth(out_image->w,out_image->h,out_image->depth) );

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


	if( flag == MAE_GET_MARK_REGION_MASK)
	{
		return error;
	}

	if( flag & MAE_GET_MARK_REGION_FLAG2)
	{
		memcpy(out_image->data, in_image->data, _mae_sizefromdepth(out_image->w,out_image->h,out_image->depth) );
		if( flag & MAE_GET_MARK_REGION_FLAG1)
		{
			for(i = 0; i < nregions; i ++ )
			{
				//do not show region deleted
				if( mae_regions[i].label == -1 ) continue;

				left = mae_regions[i].l;
				top = mae_regions[i].t;
				right = left +  mae_regions[i].w -1;
				bottom = top +  mae_regions[i].h -1;
				for(y = top; y <= bottom ; y ++ )
				{
					in_data = in_image->data + widthbytes * y;
					out_data = out_image->data + widthbytes * y;
					for( x = left; x <= right; x ++ )
					{
						in_temp = in_data +  movedbits * x;
						out_temp = out_data + movedbits * x;
						if( movedbits == 1 )
						{
							*out_temp = *in_temp;
						}else{
							*(out_temp + 0) = *(in_temp + 0);
							*(out_temp + 1) = *(in_temp + 1);
							*(out_temp + 2) = *(in_temp + 2);
						}
					}
				}

			}
		}else{
			for(i = 0; i < nregions; i ++ )
			{
				//do not show region deleted
				if( mae_regions[i].label == -1 ) continue;

				left = mae_regions[i].l;
				top = mae_regions[i].t;
				right = left +  mae_regions[i].w -1;
				bottom = top +  mae_regions[i].h -1;
				for(y = top; y <= bottom ; y ++ )
				{
					//in_data = in_image->data + widthbytes * y;
					out_data = out_image->data + widthbytes * y;
					for( x = left; x <= right; x ++ )
					{
						//in_temp = in_data +  movedbits * x;
						out_temp = out_data + movedbits * x;
						if( movedbits == 1 )
						{
							*out_temp = bkcolor;
						}else{
							*(out_temp + 0) = bkcolor;
							*(out_temp + 1) = bkcolor;
							*(out_temp + 2) = bkcolor;
						}
					}
				}

			}
		}

	}else{
		if( flag & MAE_GET_MARK_REGION_FLAG1)
		{
			for(i = 0; i < nregions; i ++ )
			{
				//do not show region deleted
				if( mae_regions[i].label == -1 ) continue;

				left = mae_regions[i].l;
				top = mae_regions[i].t;
				right = left +  mae_regions[i].w -1;
				bottom = top +  mae_regions[i].h -1;
				for(y = top; y <= bottom ; y ++ )
				{
					in_data = in_image->data + widthbytes * y;
					out_data = out_image->data + widthbytes * y;
					for( x = left; x <= right; x ++ )
					{
						in_temp = in_data +  movedbits * x;
						out_temp = out_data + movedbits * x;
						if( movedbits == 1 )
						{
							*out_temp = *in_temp;
						}else{
							*(out_temp + 0) = *(in_temp + 0);
							*(out_temp + 1) = *(in_temp + 1);
							*(out_temp + 2) = *(in_temp + 2);
						}
					}
				}

			}
		}else{
			for(i = 0; i < nregions; i ++ )
			{
				//do not show region deleted
				if( mae_regions[i].label == -1 ) continue;

				left = mae_regions[i].l;
				top = mae_regions[i].t;
				right = left +  mae_regions[i].w -1;
				bottom = top +  mae_regions[i].h -1;
				for(y = top; y <= bottom ; y ++ )
				{
					//in_data = in_image->data + widthbytes * y;
					out_data = out_image->data + widthbytes * y;
					for( x = left; x <= right; x ++ )
					{
						//in_temp = in_data +  movedbits * x;
						out_temp = out_data + movedbits * x;
						if( movedbits == 1 )
						{
							*out_temp = bkcolor;
						}else{
							*(out_temp + 0) = bkcolor;
							*(out_temp + 1) = bkcolor;
							*(out_temp + 2) = bkcolor;
						}
					}
				}

			}
		}
	}

	data = out_image->data;


	if( flag & MAE_GET_MARK_REGION_FLAG3)
	{
		for(i = 0; i < nregions; i ++ )
		{
			//do not show region deleted
			if( mae_regions[i].label == -1 ) continue;

			left = mae_regions[i].l;
			top = mae_regions[i].t;
			right = left +  mae_regions[i].w -1;
			bottom = top +  mae_regions[i].h -1;
			//left edge of box
			for(y = top; y < bottom; y ++ )
			{
				x = left;
				temp = data + widthbytes * y + movedbits * x;
				if( movedbits == 1 )
				{
					*temp = 120;
				}else{
					*(temp + 0) = 255;
					*(temp + 1) = 0;
					*(temp + 2) = 0;
				}
			}
			//right edge of box
			for(y = top; y < bottom; y ++ )
			{
				x = right;
				temp = data + widthbytes * y + movedbits * x;
				if( movedbits == 1 )
				{
					*temp = 120;
				}else{
					*(temp + 0) = 0;
					*(temp + 1) = 0;
					*(temp + 2) = 255;
				}
			}
			//top edge of box
			for(x = left; x < right; x ++ )
			{
				y = top;
				temp = data + widthbytes * y + movedbits * x;
				if( movedbits == 1 )
				{
					*temp = 120;
				}else{
					*(temp + 0) = 255;
					*(temp + 1) = 0;
					*(temp + 2) = 0;
				}
			}
			//bottom edge of box
			for(x = left; x < right; x ++ )
			{
				y = bottom;
				temp = data + widthbytes * y + movedbits * x;
				if( movedbits == 1 )
				{
					*temp = 120;
				}else{
					*(temp + 0) = 0;
					*(temp + 1) = 0;
					*(temp + 2) = 255;
				}
			}
		}
	}

	data = out_image->data;
	//draw lines for test detect lines start
	for(i = 0; i < nregions; i ++ )
	{

		for(j = 0; j < mae_regions[i].linenum; j ++ )
		{

			left = mae_regions[i].lines[j].l;
			top = mae_regions[i].lines[j].t;
			right = left +  mae_regions[i].lines[j].w -1;
			bottom = top +  mae_regions[i].lines[j].h -1;
			//left edge of box
			for(y = top; y < bottom; y ++ )
			{
				x = left;
				temp = data + widthbytes * y + movedbits * x;
				if( movedbits == 1 )
				{
					*temp = 120;
				}else{
					*(temp + 0) = 255;
					*(temp + 1) = 0;
					*(temp + 2) = 0;
				}
			}
			//right edge of box
			for(y = top; y < bottom; y ++ )
			{
				x = right;
				temp = data + widthbytes * y + movedbits * x;
				if( movedbits == 1 )
				{
					*temp = 120;
				}else{
					*(temp + 0) = 0;
					*(temp + 1) = 0;
					*(temp + 2) = 255;
				}
			}
			//top edge of box
			for(x = left; x < right; x ++ )
			{
				y = top;
				temp = data + widthbytes * y + movedbits * x;
				if( movedbits == 1 )
				{
					*temp = 120;
				}else{
					*(temp + 0) = 255;
					*(temp + 1) = 0;
					*(temp + 2) = 0;
				}
			}
			//bottom edge of box
			for(x = left; x < right; x ++ )
			{
				y = bottom;
				temp = data + widthbytes * y + movedbits * x;
				if( movedbits == 1 )
				{
					*temp = 120;
				}else{
					*(temp + 0) = 0;
					*(temp + 1) = 0;
					*(temp + 2) = 255;
				}
			}
			if(mae_regions[i].lines[j].type == 0 )
			{
				//draw hori line
				left = mae_regions[i].lines[j].sx;
				right = mae_regions[i].lines[j].ex;
				top = mae_regions[i].lines[j].sy;
				for(x = left; x < right; x ++ )
				{
					y = top;
					temp = data + widthbytes * y + movedbits * x;
					if( movedbits == 1 )
					{
						*temp = 120;
					}else{
						*(temp + 0) = 0;
						*(temp + 1) = 255;
						*(temp + 2) = 0;
					}
				}
			}else{
				top = mae_regions[i].lines[j].sy;
				bottom = mae_regions[i].lines[j].ey;
				left = mae_regions[i].lines[j].sx;
				//left edge of box
				for(y = top; y < bottom; y ++ )
				{
					x = left;
					temp = data + widthbytes * y + movedbits * x;
					if( movedbits == 1 )
					{
						*temp = 120;
					}else{
						*(temp + 0) = 0;
						*(temp + 1) = 255;
						*(temp + 2) = 0;
					}
				}
			}
		}
	}
	//draw lines for test detect lines end

	return error;
}

int _mae_save_mark_line_image(MAE_image *in_image, MAE_region *mae_regions, int nregions, MAE_image *out_image, int flag)
{
	int error = MAE_NO_ERROR;
	int left;
	int top;
	int right;
	int bottom;
	//int i,j,x,y;
	int i,j,x,y;
	int depth;
	int movedbits;
	long widthbytes;
	unsigned char bkcolor;
	unsigned char *data;
	unsigned char *temp;
	unsigned char *in_data;
	unsigned char *in_temp;
	unsigned char *out_data;
	unsigned char *out_temp;

	bkcolor = 255;

	out_image->xdpi = in_image->xdpi;
	out_image->ydpi = in_image->ydpi;
	out_image->depth = in_image->depth;
	out_image->type = in_image->type;
	out_image->w  = in_image->w;
	out_image->h = in_image->h;
	out_image->stride = _mae_width_bytes(  out_image->depth, out_image->w );
	out_image->data  = _mae_mallocate_image(out_image->w,out_image->h,out_image->depth);

	memset(out_image->data, (BYTE)bkcolor, _mae_sizefromdepth(out_image->w,out_image->h,out_image->depth) );

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

	memcpy(out_image->data, in_image->data, _mae_sizefromdepth(out_image->w,out_image->h,out_image->depth) );
	data = out_image->data;
	/*
	for(i = 0; i < nregions; i ++ )
	{
	//do not show region deleted
	if( mae_regions[i].label == -1 ) continue;

	left = mae_regions[i].l;
	top = mae_regions[i].t;
	right = left +  mae_regions[i].w -1;
	bottom = top +  mae_regions[i].h -1;
	//left edge of box
	for(y = top; y < bottom; y ++ )
	{
	x = left;
	temp = data + widthbytes * y + movedbits * x;
	if( movedbits == 1 )
	{
	*temp = 120;
	}else{
	if( mae_regions[i].type == MAE_HORI_LINE )
	{
	*(temp + 0) = 255;
	*(temp + 1) = 0;
	*(temp + 2) = 0;
	}else{
	*(temp + 0) = 0;
	*(temp + 1) = 255;
	*(temp + 2) = 0;
	}
	}
	}
	//right edge of box
	for(y = top; y < bottom; y ++ )
	{
	x = right;
	temp = data + widthbytes * y + movedbits * x;
	if( movedbits == 1 )
	{
	*temp = 120;
	}else{
	*(temp + 0) = 0;
	*(temp + 1) = 0;
	*(temp + 2) = 255;
	}
	}
	//top edge of box
	for(x = left; x < right; x ++ )
	{
	y = top;
	temp = data + widthbytes * y + movedbits * x;
	if( movedbits == 1 )
	{
	*temp = 120;
	}else{
	if( mae_regions[i].type == MAE_HORI_LINE )
	{
	*(temp + 0) = 255;
	*(temp + 1) = 0;
	*(temp + 2) = 0;
	}else{
	*(temp + 0) = 0;
	*(temp + 1) = 255;
	*(temp + 2) = 0;
	}
	}
	}
	//bottom edge of box
	for(x = left; x < right; x ++ )
	{
	y = bottom;
	temp = data + widthbytes * y + movedbits * x;
	if( movedbits == 1 )
	{
	*temp = 120;
	}else{
	*(temp + 0) = 0;
	*(temp + 1) = 0;
	*(temp + 2) = 255;
	}
	}

	//draw line start
	for(j = 0; j < mae_regions[i].linenum; j ++ )
	{
	if(mae_regions[i].lines[j].type == MAE_HORI_LINE )
	{
	//draw hori line
	left = mae_regions[i].lines[j].sx;
	right = mae_regions[i].lines[j].ex;
	top = mae_regions[i].lines[j].sy;
	for(x = left; x < right; x ++ )
	{
	y = top;
	temp = data + widthbytes * y + movedbits * x;
	if( movedbits == 1 )
	{
	*temp = 120;
	}else{
	*(temp + 0) = 0;
	*(temp + 1) = 255;
	*(temp + 2) = 0;
	}
	}
	}else{
	top = mae_regions[i].lines[j].sy;
	bottom = mae_regions[i].lines[j].ey;
	left = mae_regions[i].lines[j].sx;
	//left edge of box
	for(y = top; y < bottom; y ++ )
	{
	x = left;
	temp = data + widthbytes * y + movedbits * x;
	if( movedbits == 1 )
	{
	*temp = 120;
	}else{
	*(temp + 0) = 0;
	*(temp + 1) = 255;
	*(temp + 2) = 0;
	}
	}
	}
	}
	//draw line end

	}
	*/
	/*	
	if( flag == MAE_GET_MARK_REGION_MASK)
	{
	return error;
	}

	if( flag & MAE_GET_MARK_REGION_FLAG2)
	{
	memcpy(out_image->data, in_image->data, _mae_sizefromdepth(out_image->w,out_image->h,out_image->depth) );
	if( flag & MAE_GET_MARK_REGION_FLAG1)
	{
	for(i = 0; i < nregions; i ++ )
	{
	//do not show region deleted
	if( mae_regions[i].label == -1 ) continue;

	left = mae_regions[i].l;
	top = mae_regions[i].t;
	right = left +  mae_regions[i].w -1;
	bottom = top +  mae_regions[i].h -1;
	for(y = top; y <= bottom ; y ++ )
	{
	in_data = in_image->data + widthbytes * y;
	out_data = out_image->data + widthbytes * y;
	for( x = left; x <= right; x ++ )
	{
	in_temp = in_data +  movedbits * x;
	out_temp = out_data + movedbits * x;
	if( movedbits == 1 )
	{
	*out_temp = *in_temp;
	}else{
	*(out_temp + 0) = *(in_temp + 0);
	*(out_temp + 1) = *(in_temp + 1);
	*(out_temp + 2) = *(in_temp + 2);
	}
	}
	}

	}
	}else{
	for(i = 0; i < nregions; i ++ )
	{
	//do not show region deleted
	if( mae_regions[i].label == -1 ) continue;

	left = mae_regions[i].l;
	top = mae_regions[i].t;
	right = left +  mae_regions[i].w -1;
	bottom = top +  mae_regions[i].h -1;
	for(y = top; y <= bottom ; y ++ )
	{
	//in_data = in_image->data + widthbytes * y;
	out_data = out_image->data + widthbytes * y;
	for( x = left; x <= right; x ++ )
	{
	//in_temp = in_data +  movedbits * x;
	out_temp = out_data + movedbits * x;
	if( movedbits == 1 )
	{
	*out_temp = bkcolor;
	}else{
	*(out_temp + 0) = bkcolor;
	*(out_temp + 1) = bkcolor;
	*(out_temp + 2) = bkcolor;
	}
	}
	}

	}
	}

	}else{
	if( flag & MAE_GET_MARK_REGION_FLAG1)
	{
	for(i = 0; i < nregions; i ++ )
	{
	//do not show region deleted
	if( mae_regions[i].label == -1 ) continue;

	left = mae_regions[i].l;
	top = mae_regions[i].t;
	right = left +  mae_regions[i].w -1;
	bottom = top +  mae_regions[i].h -1;
	for(y = top; y <= bottom ; y ++ )
	{
	in_data = in_image->data + widthbytes * y;
	out_data = out_image->data + widthbytes * y;
	for( x = left; x <= right; x ++ )
	{
	in_temp = in_data +  movedbits * x;
	out_temp = out_data + movedbits * x;
	if( movedbits == 1 )
	{
	*out_temp = *in_temp;
	}else{
	*(out_temp + 0) = *(in_temp + 0);
	*(out_temp + 1) = *(in_temp + 1);
	*(out_temp + 2) = *(in_temp + 2);
	}
	}
	}

	}
	}else{
	for(i = 0; i < nregions; i ++ )
	{
	//do not show region deleted
	if( mae_regions[i].label == -1 ) continue;

	left = mae_regions[i].l;
	top = mae_regions[i].t;
	right = left +  mae_regions[i].w -1;
	bottom = top +  mae_regions[i].h -1;
	for(y = top; y <= bottom ; y ++ )
	{
	//in_data = in_image->data + widthbytes * y;
	out_data = out_image->data + widthbytes * y;
	for( x = left; x <= right; x ++ )
	{
	//in_temp = in_data +  movedbits * x;
	out_temp = out_data + movedbits * x;
	if( movedbits == 1 )
	{
	*out_temp = bkcolor;
	}else{
	*(out_temp + 0) = bkcolor;
	*(out_temp + 1) = bkcolor;
	*(out_temp + 2) = bkcolor;
	}
	}
	}

	}
	}
	}

	data = out_image->data;


	if( flag & MAE_GET_MARK_REGION_FLAG3)
	{
	for(i = 0; i < nregions; i ++ )
	{
	//do not show region deleted
	if( mae_regions[i].label == -1 ) continue;

	left = mae_regions[i].l;
	top = mae_regions[i].t;
	right = left +  mae_regions[i].w -1;
	bottom = top +  mae_regions[i].h -1;
	//left edge of box
	for(y = top; y < bottom; y ++ )
	{
	x = left;
	temp = data + widthbytes * y + movedbits * x;
	if( movedbits == 1 )
	{
	*temp = 120;
	}else{
	*(temp + 0) = 255;
	*(temp + 1) = 0;
	*(temp + 2) = 0;
	}
	}
	//right edge of box
	for(y = top; y < bottom; y ++ )
	{
	x = right;
	temp = data + widthbytes * y + movedbits * x;
	if( movedbits == 1 )
	{
	*temp = 120;
	}else{
	*(temp + 0) = 0;
	*(temp + 1) = 0;
	*(temp + 2) = 255;
	}
	}
	//top edge of box
	for(x = left; x < right; x ++ )
	{
	y = top;
	temp = data + widthbytes * y + movedbits * x;
	if( movedbits == 1 )
	{
	*temp = 120;
	}else{
	*(temp + 0) = 255;
	*(temp + 1) = 0;
	*(temp + 2) = 0;
	}
	}
	//bottom edge of box
	for(x = left; x < right; x ++ )
	{
	y = bottom;
	temp = data + widthbytes * y + movedbits * x;
	if( movedbits == 1 )
	{
	*temp = 120;
	}else{
	*(temp + 0) = 0;
	*(temp + 1) = 0;
	*(temp + 2) = 255;
	}
	}
	}
	}
	*/
	data = out_image->data;
	//draw lines for test detect lines start
	for(i = 0; i < nregions; i ++ )
	{

		for(j = 0; j < mae_regions[i].linenum; j ++ )
		{

			//left = mae_regions[i].lines[j].l;
			//top = mae_regions[i].lines[j].t;
			//right = left +  mae_regions[i].lines[j].w -1;
			//bottom = top +  mae_regions[i].lines[j].h -1;
			left = mae_regions[i].l;
			top = mae_regions[i].t;
			right = left +  mae_regions[i].w -1;
			bottom = top +  mae_regions[i].h -1;
			//left edge of box
			for(y = top; y < bottom; y ++ )
			{
				x = left;
				temp = data + widthbytes * y + movedbits * x;
				if( movedbits == 1 )
				{
					*temp = 120;
				}else{
					*(temp + 0) = 255;
					*(temp + 1) = 0;
					*(temp + 2) = 0;
				}
			}
			//right edge of box
			for(y = top; y < bottom; y ++ )
			{
				x = right;
				temp = data + widthbytes * y + movedbits * x;
				if( movedbits == 1 )
				{
					*temp = 120;
				}else{
					*(temp + 0) = 0;
					*(temp + 1) = 0;
					*(temp + 2) = 255;
				}
			}
			//top edge of box
			for(x = left; x < right; x ++ )
			{
				y = top;
				temp = data + widthbytes * y + movedbits * x;
				if( movedbits == 1 )
				{
					*temp = 120;
				}else{
					*(temp + 0) = 255;
					*(temp + 1) = 0;
					*(temp + 2) = 0;
				}
			}
			//bottom edge of box
			for(x = left; x < right; x ++ )
			{
				y = bottom;
				temp = data + widthbytes * y + movedbits * x;
				if( movedbits == 1 )
				{
					*temp = 120;
				}else{
					*(temp + 0) = 0;
					*(temp + 1) = 0;
					*(temp + 2) = 255;
				}
			}
			if(mae_regions[i].lines[j].type == MAE_HORI_LINE )
			{
				//draw hori line
				left = mae_regions[i].lines[j].sx;
				right = mae_regions[i].lines[j].ex;
				top = mae_regions[i].lines[j].sy;
				for(x = left; x < right; x ++ )
				{
					y = top;
					temp = data + widthbytes * y + movedbits * x;
					if( movedbits == 1 )
					{
						*temp = 120;
					}else{
						*(temp + 0) = 0;
						*(temp + 1) = 255;
						*(temp + 2) = 0;
					}
				}
			}else{
				top = mae_regions[i].lines[j].sy;
				bottom = mae_regions[i].lines[j].ey;
				left = mae_regions[i].lines[j].sx;
				//left edge of box
				for(y = top; y < bottom; y ++ )
				{
					x = left;
					temp = data + widthbytes * y + movedbits * x;
					if( movedbits == 1 )
					{
						*temp = 120;
					}else{
						*(temp + 0) = 0;
						*(temp + 1) = 255;
						*(temp + 2) = 0;
					}
				}
			}
		}
	}
	//draw lines for test detect lines end

	return error;
}


int _mae_save_mark_image_from_points(MAE_image *in_image, MAE_region *mae_regions, int nregions, MAE_image *out_image, int flag)
{
	int error = MAE_NO_ERROR;
	int left;
	int top;
	int right;
	int bottom;
	//int i,j,x,y;
	int i,x,y;
	int depth;
	int movedbits;
	long widthbytes;
	unsigned char bkcolor;
	unsigned char *data;
	unsigned char *temp;
	unsigned char *in_data;
	unsigned char *in_temp;
	unsigned char *out_data;
	unsigned char *out_temp;

	bkcolor = 255;

	out_image->xdpi = in_image->xdpi;
	out_image->ydpi = in_image->ydpi;
	out_image->depth = in_image->depth;
	out_image->type = in_image->type;
	out_image->w  = in_image->w;
	out_image->h = in_image->h;
	out_image->stride = _mae_width_bytes(  out_image->depth, out_image->w );
	out_image->data  = _mae_mallocate_image(out_image->w,out_image->h,out_image->depth);

	memset(out_image->data, (BYTE)bkcolor, _mae_sizefromdepth(out_image->w,out_image->h,out_image->depth) );

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


	if( flag == MAE_GET_MARK_REGION_MASK)
	{
		return error;
	}

	if( flag & MAE_GET_MARK_REGION_FLAG2)
	{
		memcpy(out_image->data, in_image->data, _mae_sizefromdepth(out_image->w,out_image->h,out_image->depth) );
		if( flag & MAE_GET_MARK_REGION_FLAG1)
		{
			for(i = 0; i < nregions; i ++ )
			{
				//do not show region deleted
				if( mae_regions[i].label == -1 ) continue;

				left = mae_regions[i].l;
				top = mae_regions[i].t;
				right = left +  mae_regions[i].w -1;
				bottom = top +  mae_regions[i].h -1;
				for(y = top; y <= bottom ; y ++ )
				{
					in_data = in_image->data + widthbytes * y;
					out_data = out_image->data + widthbytes * y;
					for( x = left; x <= right; x ++ )
					{
						in_temp = in_data +  movedbits * x;
						out_temp = out_data + movedbits * x;
						if( movedbits == 1 )
						{
							*out_temp = *in_temp;
						}else{
							*(out_temp + 0) = *(in_temp + 0);
							*(out_temp + 1) = *(in_temp + 1);
							*(out_temp + 2) = *(in_temp + 2);
						}
					}
				}

			}
		}else{
			for(i = 0; i < nregions; i ++ )
			{
				//do not show region deleted
				if( mae_regions[i].label == -1 ) continue;

				left = mae_regions[i].l;
				top = mae_regions[i].t;
				right = left +  mae_regions[i].w -1;
				bottom = top +  mae_regions[i].h -1;
				for(y = top; y <= bottom ; y ++ )
				{
					//in_data = in_image->data + widthbytes * y;
					out_data = out_image->data + widthbytes * y;
					for( x = left; x <= right; x ++ )
					{
						//in_temp = in_data +  movedbits * x;
						out_temp = out_data + movedbits * x;
						if( movedbits == 1 )
						{
							*out_temp = bkcolor;
						}else{
							*(out_temp + 0) = bkcolor;
							*(out_temp + 1) = bkcolor;
							*(out_temp + 2) = bkcolor;
						}
					}
				}

			}
		}

	}else{
		if( flag & MAE_GET_MARK_REGION_FLAG1)
		{
			for(i = 0; i < nregions; i ++ )
			{
				//do not show region deleted
				if( mae_regions[i].label == -1 ) continue;

				left = mae_regions[i].l;
				top = mae_regions[i].t;
				right = left +  mae_regions[i].w -1;
				bottom = top +  mae_regions[i].h -1;
				for(y = top; y <= bottom ; y ++ )
				{
					in_data = in_image->data + widthbytes * y;
					out_data = out_image->data + widthbytes * y;
					for( x = left; x <= right; x ++ )
					{
						in_temp = in_data +  movedbits * x;
						out_temp = out_data + movedbits * x;
						if( movedbits == 1 )
						{
							*out_temp = *in_temp;
						}else{
							*(out_temp + 0) = *(in_temp + 0);
							*(out_temp + 1) = *(in_temp + 1);
							*(out_temp + 2) = *(in_temp + 2);
						}
					}
				}

			}
		}else{
			for(i = 0; i < nregions; i ++ )
			{
				//do not show region deleted
				if( mae_regions[i].label == -1 ) continue;

				left = mae_regions[i].l;
				top = mae_regions[i].t;
				right = left +  mae_regions[i].w -1;
				bottom = top +  mae_regions[i].h -1;
				for(y = top; y <= bottom ; y ++ )
				{
					//in_data = in_image->data + widthbytes * y;
					out_data = out_image->data + widthbytes * y;
					for( x = left; x <= right; x ++ )
					{
						//in_temp = in_data +  movedbits * x;
						out_temp = out_data + movedbits * x;
						if( movedbits == 1 )
						{
							*out_temp = bkcolor;
						}else{
							*(out_temp + 0) = bkcolor;
							*(out_temp + 1) = bkcolor;
							*(out_temp + 2) = bkcolor;
						}
					}
				}

			}
		}
	}

	data = out_image->data;

	if( flag & MAE_GET_MARK_REGION_FLAG3)
	{
		for(i = 0; i < nregions; i ++ )
		{
			//do not show region deleted
			if( mae_regions[i].label == -1 ) continue;

			left = mae_regions[i].l;
			top = mae_regions[i].t;
			right = left +  mae_regions[i].w -1;
			bottom = top +  mae_regions[i].h -1;
			//left edge of box
			for(y = top; y < bottom; y ++ )
			{
				x = left;
				temp = data + widthbytes * y + movedbits * x;
				if( movedbits == 1 )
				{
					*temp = 120;
				}else{
					*(temp + 0) = 255;
					*(temp + 1) = 0;
					*(temp + 2) = 0;
				}
			}
			//right edge of box
			for(y = top; y < bottom; y ++ )
			{
				x = right;
				temp = data + widthbytes * y + movedbits * x;
				if( movedbits == 1 )
				{
					*temp = 120;
				}else{
					*(temp + 0) = 0;
					*(temp + 1) = 0;
					*(temp + 2) = 255;
				}
			}
			//top edge of box
			for(x = left; x < right; x ++ )
			{
				y = top;
				temp = data + widthbytes * y + movedbits * x;
				if( movedbits == 1 )
				{
					*temp = 120;
				}else{
					*(temp + 0) = 255;
					*(temp + 1) = 0;
					*(temp + 2) = 0;
				}
			}
			//bottom edge of box
			for(x = left; x < right; x ++ )
			{
				y = bottom;
				temp = data + widthbytes * y + movedbits * x;
				if( movedbits == 1 )
				{
					*temp = 120;
				}else{
					*(temp + 0) = 0;
					*(temp + 1) = 0;
					*(temp + 2) = 255;
				}
			}
		}
	}
	return error;
}


int _mae_save_mark_image_from_corners(MAE_image *in_image, MAE_region *mae_regions, int nregions, MAE_image *out_image, int flag)
{
	int error = MAE_NO_ERROR;
	//int left;
	//int top;
	//int right;
	//int bottom;
	//int i,j,x,y;
	int i,j,x1,y1,x2,y2, x,y,dx,dy;
	int k;
	int depth;
	int movedbits;
	long widthbytes;
	//unsigned char bkcolor;
	unsigned char *data;
	unsigned char *temp;
	unsigned char *in_data;
	unsigned char *in_temp;
	unsigned char *out_data;
	unsigned char *out_temp;

	unsigned char bkr[6];
	unsigned char bkg[6];
	unsigned char bkb[6];

	unsigned char r;
	unsigned char g;
	unsigned char b;

	bkr[0] = 255;
	bkg[0] = 0;
	bkb[0] = 0;
	bkr[1] = 0;
	bkg[1] = 255;
	bkb[1] = 0;
	bkr[2] = 0;
	bkg[2] = 0;
	bkb[2] = 255;
	bkr[3] = 255;
	bkg[3] = 255;
	bkb[3] = 0;
	bkr[4] = 255;
	bkg[4] = 0;
	bkb[4] = 255;
	bkr[5] = 0;
	bkg[5] = 255;
	bkb[5] = 255;
	//bkcolor = 255;

	out_image->xdpi = in_image->xdpi;
	out_image->ydpi = in_image->ydpi;
	out_image->depth = in_image->depth;
	out_image->type = in_image->type;
	out_image->w  = in_image->w;
	out_image->h = in_image->h;
	out_image->stride = _mae_width_bytes(  out_image->depth, out_image->w );
	out_image->data  = _mae_mallocate_image(out_image->w,out_image->h,out_image->depth);

	memset(out_image->data, (BYTE)255, _mae_sizefromdepth(out_image->w,out_image->h,out_image->depth) );
	memcpy(out_image->data, in_image->data, _mae_sizefromdepth(out_image->w,out_image->h,out_image->depth) );

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

	//
	//if( flag == MAE_GET_MARK_REGION_MASK)
	//{
	//	return error;
	//}

	data = out_image->data;

	//if( flag & MAE_GET_MARK_REGION_FLAG3)
	//{
	for(i = 0; i < nregions; i ++ )
	{
		//20110104 add start
		if(mae_regions[i].label == -1 )
		{
			continue;
		}
		//20110104 add end
		if( i < 6 )
		{
			r = bkr[i];
			g = bkg[i];
			b = bkb[i];
		}else{
			r = 0;
			g = 0;
			b = 0;
		}
		for(j = 1; j <= mae_regions[i].cornernum; j ++ )
		{
			if( j == mae_regions[i].cornernum )
			{
				x1 = mae_regions[i].corners[j-1].x;
				y1 = mae_regions[i].corners[j-1].y;
				x2 = mae_regions[i].corners[0].x;
				y2 = mae_regions[i].corners[0].y;
			}else{
				x1 = mae_regions[i].corners[j-1].x;
				y1 = mae_regions[i].corners[j-1].y;
				x2 = mae_regions[i].corners[j].x;
				y2 = mae_regions[i].corners[j].y;
			}
			dx = abs(x2-x1);
			dy = abs(y2-y1);
			if( dx < dy ) //vert line
			{
				if( y1 > y2 )
				{
					y = y1;
					y1 = y2;
					y2 = y;
				}
				if( x1 > x2 )
				{
					x = x1;
					x1 = x2;
					x2 = x;
				}
				_mae_draw_line(data, out_image->w, out_image->h, widthbytes, movedbits, x1, y1, x2, y2,r,g,b, 0);
				//for(k =0 ; k < 3; k ++ )
				//{
				//	for(y = y1; y < y2; y ++ )
				//	{
				//		x = (x1 + x2) / 2 + k - 1;
				//		if( x < 0 ) x = 0;
				//		if( x >= out_image->w ) x = out_image->w - 1;
				//		temp = data + widthbytes * y + movedbits * x;
				//		if( movedbits == 1 )
				//		{
				//			*temp = 120;
				//		}else{
				//			*(temp + 0) = r;
				//			*(temp + 1) = g;
				//			*(temp + 2) = b;
				//		}
				//	}
				//}
			}else{ //hori line
				if( y1 > y2 )
				{
					y = y1;
					y1 = y2;
					y2 = y;
				}
				if( x1 > x2 )
				{
					x = x1;
					x1 = x2;
					x2 = x;
				}
				_mae_draw_line(data, out_image->w, out_image->h, widthbytes, movedbits, x1, y1, x2, y2, r,g,b,1);
				//	for(k =0 ; k < 3; k ++ )
				//	{
				//		for(x = x1; x < x2; x ++ )
				//		{
				//			y = (y1 + y2) / 2 + k - 1;
				//			if( y < 0 ) y = 0;
				//			if( y >= out_image->h ) y = out_image->h - 1;

				//			temp = data + widthbytes * y + movedbits * x;
				//			if( movedbits == 1 )
				//			{
				//				*temp = 120;
				//			}else{
				//				*(temp + 0) = r;
				//				*(temp + 1) = g;
				//				*(temp + 2) = b;
				//			}
				//		}
				//	}
			}
			//if(j == 2 )
			//break;
		}
	}
	//}
	return error;
}

void _mae_draw_line(unsigned char *data, int w, int h, long widthbytes, int movedbits, int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b, int flg)
{
	int k;
	int x, y;
	unsigned char * temp;
	if(flg == 0 ) //virt line
	{
		for(k =0 ; k < 3; k ++ )
		{
			for(y = y1; y < y2; y ++ )
			{
				x = (x1 + x2) / 2 + k - 1;
				if( x < 0 ) x = 0;
				if( x >= w ) x = w - 1;
				if( y < 0 ) y = 0;
				if( y >= h ) y = h - 1;
				temp = data + widthbytes * y + movedbits * x;
				if( movedbits == 1 )
				{
					*temp = 120;
				}else{
					*(temp + 0) = r;
					*(temp + 1) = g;
					*(temp + 2) = b;
				}
			}
		}
	}else{
		for(k =0 ; k < 3; k ++ )
		{
			for(x = x1; x < x2; x ++ )
			{
				y = (y1 + y2) / 2 + k - 1;
				if( x < 0 ) x = 0;
				if( x >= w ) x = w - 1;
				if( y < 0 ) y = 0;
				if( y >= h ) y = h - 1;
				temp = data + widthbytes * y + movedbits * x;
				if( movedbits == 1 )
				{
					*temp = 120;
				}else{
					*(temp + 0) = r;
					*(temp + 1) = g;
					*(temp + 2) = b;
				}
			}
		}
	}
}
void _mae_adjust_region_list_rect2(unsigned char * data, int width, int height, MAE_region **_mae_region, int *_nregions)
{
	int i;

	for(i = 0; i < *_nregions; i ++ )
	{
		if( (*_mae_region)[i].label == -1 ) continue;
		_mae_adjust_region_rect2(data, width, height, &((*_mae_region)[i]));
	}

}
void _mae_adjust_region_rect2(unsigned char * data, int width, int height, MAE_region *region)
{
	int left, top,right, bottom;
	unsigned char *check_data;
	int check_len;
	int count;
	int x, y;
	//unsigned char *n_temp;


	left = region->l + region->w / 3;
	top = region->t;
	right = region->l + region->w - region->w / 3;
	bottom = region->t + region->h;

	check_len = width;
	// detect top and bottom  connect row
	_mae_malloc_uchar(&check_data, check_len, "_mae_adjust_region_rect2 : check_data");
	memset(check_data, (unsigned char)0, check_len);

	//detect top edge 
	//down move  

	count = 0;
	y = top;
	_mae_get_8_row_data(data,width,height,check_data,y,left,right);
	while( _mae_sum_line_pixs(check_data,check_len ) == 0 )
	{
		y ++;
		count ++;
		if( y  == bottom ) break;
		_mae_get_8_row_data(data,width,height,check_data,y,left,right);
	}
	region->t += count;
	region->h -= count;

	//detect bottom edge
	//up move
	count = 0;
	y = bottom;
	_mae_get_8_row_data(data,width,height,check_data,y,left,right);
	while( _mae_sum_line_pixs(check_data,check_len ) == 0 )
	{
		y --;
		count ++;
		if( y  == top ) break;
		_mae_get_8_row_data(data,width,height,check_data,y,left,right);
	}

	region->h -= count;


	free(check_data);

	// detect left and right  edge
	left = region->l;
	top = region->t + region->h / 3;
	right = region->l + region->w;
	bottom = region->t + region->h - region->h / 3;

	check_len = height;
	_mae_malloc_uchar(&check_data, check_len, "_mae_adjust_region_rect : check_data");
	memset(check_data, (unsigned char)0, check_len);

	//detect left edge
	//right move
	count = 0;
	x = left;
	_mae_get_8_col_data(data,width,height,check_data,x,top,bottom);
	while( _mae_sum_line_pixs(check_data,check_len ) == 0 )
	{
		count ++;
		x ++;
		if( x == right ) break;
		_mae_get_8_col_data(data,width,height,check_data,x,top,bottom);
	}

	region->l += count;
	region->w -= count;

	//detect right edge
	//left move
	count = 0;
	x = right;
	_mae_get_8_col_data(data,width,height,check_data,x,top,bottom);
	while( _mae_sum_line_pixs(check_data,check_len ) == 0 )
	{
		count ++;
		x --;
		if( x == left ) break;
		_mae_get_8_col_data(data,width,height,check_data,x,top,bottom);
	}

	region->w -= count;

	free(check_data);

}
int _mae_sum_line_pixs(unsigned char * list, int len)
{
	int sum = 0;
	for( int i = 0; i < len; i ++ )
	{
		sum = sum + list[i];
	}
	return sum;
}


void _mae_check_back_color_region_all1(unsigned char * data, int w, int h, MAE_region **_mae_region, int *_nregions)
{
	int i;

	for(i = 0; i < *_nregions; i ++ )
	{
		if( (*_mae_region)[i].label == -1 ) continue;
		_mae_check_back_color_region1(data, w, h, &((*_mae_region)[i]));
	}

}
void _mae_check_back_color_region1(unsigned char * data, int w, int h, MAE_region *region)
{
	//	int i;
	long pixnum;
	int width;
	int height;
	double w_ratio, h_ratio;
	unsigned char * sdata;
	sdata = data;
	width = region->w;
	height = region->h;
	//s_ratio = (double)pixnum / (double)(w * h);
	w_ratio = (double)width / (double)w;
	h_ratio = (double)height / (double)h;
	if(    w_ratio < MAE_SMALL_REGION_RATIO 
		&& h_ratio < MAE_SMALL_REGION_RATIO ) 
	{
		region->label = -1;
		return;
	}
	pixnum = _mae_get_region_pixnum(region);
	if( pixnum < MAE_REGION_PIXS_THRESHOLD )
	{
		region->label = -1;
	}

}

void _mae_adjust_region_size_by_fctr(MAE_region **regions, int *nregions, float xfctr, float yfctr,int width, int height)
{
	int i,j;
	int xadjust;
	int yadjust;
	int w;
	int h;
	xadjust = (int)((float)1 / xfctr  + 0.5);
	yadjust = (int)((float)1 / yfctr  + 0.5);
	//	w = (int)(width / xfctr + 0.5);
	//	h = (int)(height / yfctr + 0.5);
	w = width;
	h = height;
	for(i = 0; i < *nregions; i ++ )
	{
		//if( (*regions)[i].label == -1 ) continue;
		(*regions)[i].l = (int) ((float)(*regions)[i].l / xfctr) - xadjust;
		(*regions)[i].t = (int) ((float)(*regions)[i].t / yfctr) - yadjust;
		(*regions)[i].w = (int) ((float)(*regions)[i].w / xfctr + 0.5) + xadjust * 2;
		(*regions)[i].h = (int) ((float)(*regions)[i].h / yfctr + 0.5) + yadjust * 2;
		if( (*regions)[i].l < 0 ) (*regions)[i].l = 0;
		if( (*regions)[i].t < 0 )  (*regions)[i].t = 0;
		if( (*regions)[i].l + (*regions)[i].w >= w ) (*regions)[i].w = w - 1 - (*regions)[i].l;
		if( (*regions)[i].t + (*regions)[i].h >= h)  (*regions)[i].h = h - 1 - (*regions)[i].t;
		//20101102 add for detecting lines start
		for(j = 0; j < (*regions)[i].linenum ; j ++ )
		{
			(*regions)[i].lines[j].sx = (*regions)[i].lines[j].sx / xfctr;
			(*regions)[i].lines[j].sy = (*regions)[i].lines[j].sy / yfctr;
			(*regions)[i].lines[j].ex = (*regions)[i].lines[j].ex / xfctr;
			(*regions)[i].lines[j].ey = (*regions)[i].lines[j].ey / yfctr;
			if( (*regions)[i].lines[j].type == MAE_HORI_LINE )
			{
				(*regions)[i].lines[j].lw = (*regions)[i].lines[j].lw / xfctr;
			}else{
				(*regions)[i].lines[j].lw = (*regions)[i].lines[j].lw / yfctr;
			}
			(*regions)[i].lines[j].l = (*regions)[i].lines[j].l / xfctr;
			(*regions)[i].lines[j].t = (*regions)[i].lines[j].t / yfctr;
			(*regions)[i].lines[j].w = (*regions)[i].lines[j].w / xfctr;
			(*regions)[i].lines[j].h = (*regions)[i].lines[j].h / yfctr;
		}
		for(j = 0; j < (*regions)[i].cornernum ; j ++ )
		{
			(*regions)[i].corners[j].x = (*regions)[i].corners[j].x / xfctr;
			(*regions)[i].corners[j].y = (*regions)[i].corners[j].y / yfctr;
		}

		//20101102 add for detecting lines end
	}
}

void _mae_adjust_point_size_by_fctr(MAE_line_node **lpthead, float xfctr, float yfctr,int width, int height)
{
	int i,j;
	int xadjust;
	int yadjust;
	int w;
	int h;
	MAE_line_node *node;
	xadjust = (int)((float)1 / xfctr  + 0.5);
	yadjust = (int)((float)1 / yfctr  + 0.5);
	//w = (int)(width / xfctr + 0.5);
	//h = (int)(height / yfctr + 0.5);
	w = width;
	h = height;

	node = *lpthead;
	while(node != NULL )
	{
		node->x = node->x / xfctr;
		node->y = node->y / yfctr;
		node = node->next;
	}
}
//void _mae_split_region_all(MAE_region **_mae_region, int *_nregions)
//{
//	int i;
//
//	for(i = 0; i < *_nregions; i ++ )
//	{
//		if( (*_mae_region)[i].label == -1 ) continue;
//		_mae_split_region(&((*_mae_region)[i]));
//	}
//
//}
//void _mae_split_region(MAE_region *region)
//{
//}

//20100831 add start  break line
//merge region based on rect pos

void _mae_merge_break_region(MAE_region **_mae_region, int *_nregions)
{
	int i,j;
	int region_num;

	int l1, t1, r1, b1;
	int l2, t2, r2, b2;
	region_num = *_nregions;

	for(i = 0; i < region_num; i ++ )
	{
		if( (*_mae_region)[i].label == MAE_MARK_REGION_NO ) continue;
		for(j = 0; j < region_num; j ++ )
		{
			if( j == i ) continue;
			if( (*_mae_region)[j].label == MAE_MARK_REGION_NO ) continue;

			if( _mae_is_link_region(&((*_mae_region)[i]), &((*_mae_region)[j])) )
			{
				//_mae_add_node_list(&((*_mae_region)[i].head), &((*_mae_region)[i].tail), (*_mae_region)[j].head, (*_mae_region)[j].tail );
				//(*_mae_region)[j].label = -1;
				//(*_mae_region)[j].head = NULL;
				//(*_mae_region)[j].tail = NULL;
				//_mae_com_region_rect(&((*_mae_region)[i]));
				l1 = (*_mae_region)[i].l;
				t1 = (*_mae_region)[i].t;
				r1 = (*_mae_region)[i].l + (*_mae_region)[i].w;
				b1 = (*_mae_region)[i].t + (*_mae_region)[i].h;
				l2 = (*_mae_region)[j].l;
				t2 = (*_mae_region)[j].t;
				r2 = (*_mae_region)[j].l + (*_mae_region)[j].w;
				b2 = (*_mae_region)[j].t + (*_mae_region)[j].h;
				if( l1 > l2 ) l1 = l2;
				if( t1 > t2 ) t1 = t2;
				if( r1 < r2 ) r1 = r2;
				if( b1 < b2 ) b1 = b2;
				(*_mae_region)[i].l = l1;
				(*_mae_region)[i].t = t1;
				(*_mae_region)[i].w = r1 - l1;
				(*_mae_region)[i].h = b1 - t1;
				(*_mae_region)[j].label = MAE_MARK_REGION_NO;
				(*_mae_region)[i].label = MAE_MARK_REGION_MERGE_NEW;
			}
		}
	}
	return;	
}

int  _mae_is_link_region(MAE_region *region1, MAE_region *region2)
{
	int cx1;
	int cy1;
	int w1;
	int h1;
	int cx2;
	int cy2;
	int w2;
	int h2;
	cx1 = region1->l + region1->w/2;
	cy1 = region1->t + region1->h/2;
	cx2 = region2->l + region2->w/2;
	cy2 = region2->t + region2->h/2;
	w1 = region1->w/2;
	h1 = region1->h/2;
	w2 = region2->w/2;
	h2 = region2->h/2;

	int gap = MAE_MARK_BREAK_REGION_GAP;

	float slender1 = 0;
	float slender2 = 0;
	int min1,min2;
	int flag1,flag2;
	if( region1->w > region1->h)
	{
		slender1 = (float)region1->h / (float)region1->w;
		min1 = region1->h;
		flag1 = 0;
	}else{
		slender1 = (float)region1->w / (float)region2->h;
		min1 = region1->w;
		flag1 = 1;
	}
	if( region2->w > region2->h)
	{
		slender2 = (float)region2->h / (float)region2->w;
		min2 = region2->h;
		flag2 = 0;
	}else{
		slender2 = (float)region2->w / (float)region2->h;
		min2 = region2->w;
		flag2 = 1;
	}
	//if(    slender1 < MAE_LINE_SLENDER_DEGREE 
	//	&& slender2 < MAE_LINE_SLENDER_DEGREE 
	//	&& flag1 != flag2 )
	//{
	//	gap = MAE_MARK_BREAK_REGION_GAP2;
	//}else{
	//	gap = MAE_MARK_BREAK_REGION_GAP;
	//}

	if(    region1->type == MAE_REGION_TYPE_RECT 
		|| region2->type == MAE_REGION_TYPE_RECT )
	{
		return 0;
	}

	if(    region1->type == MAE_REGION_TYPE_MARK 
		&& region2->type == MAE_REGION_TYPE_MARK
		)
	{
		if( flag1 != flag2 )
		{
			gap = MAE_MARK_BREAK_REGION_GAP2;
		}else{
			gap = MAE_MARK_BREAK_REGION_GAP;
		}
	}
	if( region1->type & MAE_REGION_TYPE_RECT  )
	{
		if(    region1->type & MAE_REGION_TYPE_RECT_NL 
			|| region1->type & MAE_REGION_TYPE_RECT_NR )
		{
			if(	flag2 == 1 )
			{
				gap = MAE_MARK_BREAK_REGION_GAP2;
			}
		}
		if(    region1->type & MAE_REGION_TYPE_RECT_NT 
			|| region1->type & MAE_REGION_TYPE_RECT_NB )
		{
			if(	flag2 == 0 )
			{
				gap = MAE_MARK_BREAK_REGION_GAP2;
			}
		}
	}


	if(  abs(cx1 - cx2) > ((w1+w2) + gap) )
	{
		return 0;
	}
	if(	 abs(cy1 - cy2) >  ((h1+h2) + gap) )
	{
		return 0;
	}
	return 1;
	//MAE_node *node1;
	//MAE_node *node2;

	//node1 = region1->head;
	//node2 = region2->head;
	//int dist;
	//int min_dist = 1000;

	//while(node1 != NULL )
	//{
	//	while(node2 != NULL )
	//	{
	//		dist = _mae_distance(node1,node2);
	//		if(min_dist > dist )
	//		{
	//			min_dist = dist;
	//		}
	//		node2 = node2->next;
	//	}
	//	node1 = node1->next;
	//}

	//if (min_dist < gap )
	//{
	//	return 1;
	//}
	//	return 0;

}
//
////void _mae_get_region_e_color(MAE_image * in_image, MAE_region *region, MAE_image_feature * feature)
//void _mae_get_region_e_color(MAE_image * in_image, MAE_region *region, int *_re_, int *_ge_, int *_be_, int *_he_)
//{
//	int i, j;
//	MAE_image tmp_region_image; 
//	MAE_image region_image; 
//
//	//for(i = 0; i < 9; i ++ )
//	//{
//	//	feature->r_e[i] = 0;
//	//	feature->g_e[i] = 0;
//	//	feature->b_e[i] = 0;
//	//	feature->h_e[i] = 0;
//	//}
//	//feature->min_re = 0;
//	//feature->min_ge = 0;
//	//feature->min_be = 0;
//	//feature->max_re = 0;
//	//feature->max_ge = 0;
//	//feature->max_be = 0;
//	//feature->min_he = 0;
//	//feature->max_he = 0;
//
//	int left[3][3];
//	int top[3][3];
//	int width;
//	int height;
//	int l, t,w,h;
//
//	l = region->l;
//	t = region->t;
//	w = region->w;
//	h = region->h;
//	//w = region->w / 3;
//	//h = region->h / 3;
//	//left half
//	//for(j = 0; j < 3; j ++ )
//	//for(i = 0; i < 3; i ++ )
//	//{
//	//	left[j][i] = l + i * w;
//	//	top[j][i] = t + j * h;
//	//}
//	int idx = 0;
//	int re,ge,be,he;
//	int mark_pixs;
//	//for( j = 0; j < 3; j ++ )
//	//for( i = 0; i < 3; i ++ )
//	//{
//	//	l = left[j][i];
//	//	t = top[j][i];
//
//		_mae_get_region_image(in_image, l, t, w, h, &tmp_region_image );
//		_mae_detect_high_color(&tmp_region_image, &region_image, RGB_REMOVE_TEXT_VARIENCE);
//		free(tmp_region_image.data);
//
//		re = 0;
//		ge = 0;
//		be = 0;
//		mark_pixs = 0;
//		_mae_get_image_rgb_e_v(&region_image, &re, &ge, &be, &mark_pixs);
//		//feature->r_e[idx] = (float)re;
//		//feature->g_e[idx] = (float)ge;
//		//feature->b_e[idx] = (float)be;
//		//feature->mark_pixs = mark_pixs;
//		*_re_ = re;
//		*_ge_ = ge;
//		*_be_ = be;
//		//get h color expect and variance  from hsv of high color image
//		//_mae_get_image_h_e_v(&region_image, feature, idx);
//		_mae_get_image_h_e_v(&region_image, &he);
//		//feature->h_e[idx] = (float)he;
//		//idx ++;
//		*_he_ = he;
//		free(region_image.data);
////	}
//	//feature->min_re = 255;
//	//feature->max_re = 0;
//	//feature->min_ge = 255;
//	//feature->max_ge = 0;
//	//feature->min_be = 255;
//	//feature->max_be = 0;
//	//feature->min_he = 255;
//	//feature->max_he = 0;
//	//for(i = 0; i < 9; i ++ )
//	//{
//	//	if(feature->r_e[i] != 0 && feature->min_re > feature->r_e[i] )
//	//	{
//	//		feature->min_re = feature->r_e[i]; 
//	//	}
//	//	if(feature->r_e[i] != 255 && feature->max_re < feature->r_e[i] )
//	//	{
//	//		feature->max_re = feature->r_e[i]; 
//	//	}
//	//	if(feature->g_e[i] != 0 && feature->min_ge > feature->g_e[i] )
//	//	{
//	//		feature->min_ge = feature->g_e[i]; 
//	//	}
//	//	if(feature->g_e[i] != 255 && feature->max_ge < feature->g_e[i] )
//	//	{
//	//		feature->max_ge = feature->g_e[i]; 
//	//	}
//	//	if(feature->b_e[i] != 0 && feature->min_be > feature->b_e[i] )
//	//	{
//	//		feature->min_be = feature->b_e[i]; 
//	//	}
//	//	if(feature->b_e[i] != 255 && feature->max_be < feature->b_e[i] )
//	//	{
//	//		feature->max_be = feature->b_e[i]; 
//	//	}
//	//	if(feature->h_e[i] != 0 && feature->min_he > feature->h_e[i] )
//	//	{
//	//		feature->min_he = feature->h_e[i]; 
//	//	}
//	//	if(feature->h_e[i] != 255 && feature->max_he < feature->h_e[i] )
//	//	{
//	//		feature->max_he = feature->h_e[i]; 
//	//	}
//	//}
//
//	//free(region_image.data);
//
//}
//20100831 add end

int _mae_get_regions_from_lines(MAE_line *_h_lines, int _h_num,MAE_line *_v_lines, int _v_num, MAE_region **_regions, int *_nregion)
{
	int i,j;

	unsigned char *region_data;
	int region_num;
	int idx;

	region_num = 0;
	for( i = 0; i < _h_num; i ++ )
	{
		if( _h_lines[i].label != -1 )
		{
			region_num ++;
		}
	}
	for( i = 0; i < _v_num; i ++ )
	{
		if( _v_lines[i].label != -1 )
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
	for(i = 0; i < _h_num; i ++ )
	{
		if(_h_lines[i].label == -1 ) continue;

		(*_regions)[idx].type = _h_lines[i].type;
		(*_regions)[idx].l = _h_lines[i].l;
		(*_regions)[idx].t = _h_lines[i].t;
		(*_regions)[idx].w = _h_lines[i].w;
		(*_regions)[idx].h = _h_lines[i].h;

		idx ++;

	}
	for(i = 0; i < _v_num; i ++ )
	{
		if(_v_lines[i].label == -1 ) continue;

		(*_regions)[idx].type = _v_lines[i].type;
		(*_regions)[idx].l = _v_lines[i].l;
		(*_regions)[idx].t = _v_lines[i].t;
		(*_regions)[idx].w = _v_lines[i].w;
		(*_regions)[idx].h = _v_lines[i].h;

		idx ++;

	}

	return 0;
}


void _mae_adjust_hori_lines(MAE_line *lines, int num, int width, int height)
{
	int i;
	MAE_line line;
	//_width is not be used
	int _width;
	_width = width;

	for(i = 0; i < num; i ++ )
	{
		if( lines[i].label == -1 )
		{
			continue;
		}
		line.w = lines[i].h;
		line.h = lines[i].w;
		line.l =  lines[i].t;
		line.t = height - 1 - line.h - lines[i].l;

		lines[i].l = line.l;
		lines[i].t = line.t;
		lines[i].w = line.w;
		lines[i].h = line.h;
	}
}


//20101122 for test

int _mae_save_point_image(MAE_image* dst, MAE_line_node *lpthead)
{
	int error = MAE_NO_ERROR;
	int left;
	int top;
	int right;
	int bottom;
	//int i,j,x,y;
	int i,x,y;
	int depth;
	int movedbits;
	long widthbytes;
	unsigned char bkcolor;
	unsigned char *data;
	unsigned char *temp;
	unsigned char *in_data;
	unsigned char *in_temp;
	MAE_line_node *node;
	int flag = 0;
	bkcolor = 255;

	depth = dst->depth;
	movedbits = 1;
	widthbytes = _mae_width_bytes(depth, dst->w);
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

	data = dst->data;
	//draw point
	node = lpthead;
	for(; node != NULL; node = node->next)
	{
		if( node->label == -1 ) 
		{
			continue;
		}
		left = node->x - 4;
		top = node->y - 4;
		right = left +  8;
		bottom = top +  8;
		//left edge of box
		for(y = top; y < bottom; y ++ )
			for(x = left; x < right; x ++ )
			{
				temp = data + widthbytes * y + movedbits * x;
				if( movedbits == 1 )
				{
					*temp = 120;
				}else{
					if( flag == 0 )
					{
						*(temp + 0) = 0;
						*(temp + 1) = 0;
						*(temp + 2) = 0;
						flag = 1;
					}else{
						*(temp + 0) = 255;
						*(temp + 1) = 0;
						*(temp + 2) = 0;
					}
				}
			}

			//flag ++;
	}

	return error;
}


void _mae_remove_small_regions3(MAE_region **_mae_region, int *_nregions, int w, int h)
{
	int i;
	long pixnum;
	MAE_region * region;
	int nregions;
	int width;
	int height;
	//double s_ratio, w_ratio, h_ratio;
	//double w_ratio, h_ratio;
	int _w;
	int _h;
	_w = w;
	_h = h;

	nregions = *_nregions;
	for(i = 0; i < nregions; i ++ )
	{
		region = &((*_mae_region)[i]);
		width = region->w;
		height = region->h;
		//s_ratio = (double)pixnum / (double)(w * h);
		//w_ratio = (double)width / (double)w;
		//h_ratio = (double)height / (double)h;
		//if(    w_ratio < MAE_SMALL_REGION_RATIO 
		//	&& h_ratio < MAE_SMALL_REGION_RATIO ) 
		//{
		//	region->label = -1;
		//	continue;
		//}
		if(    width < MAE_NORMAL_LINE_WIDTH 
			&& height < MAE_NORMAL_LINE_WIDTH ) 
		{
			region->label = -1;
			continue;
		}

	}
}


int _mae_point_is_in_region(int x, int y, MAE_region *region)
{
	int l,t,r,b;
	int j,k;
	int *xlist;
	int *ylist;
	int *xlist2;
	int *ylist2;
	MAE_corner *corners;
	int count;
	int quad1;
	int quad2;
	int angle;

	l = region->l;
	t = region->t;
	r = region->l + region->w;
	b = region->t + region->h;
	if(!(x < r && x > l && y < b && y > t) )
	{
		return 0;
	}

	/////////////////
	//x = 0;
	//y = 0;
	//for(j = 0; j < region->cornernum; j ++ )
	//{
	//	x += region->corners[j].x;
	//	y += region->corners[j].y;
	//}
	//x = x / region->cornernum;
	//y = y / region->cornernum;
	/////////////////

	_mae_malloc_int( &xlist, (region->cornernum) * sizeof(int),"_mae_point_is_in_region : MAE_corner");
	_mae_malloc_int( &ylist, (region->cornernum) * sizeof(int),"_mae_point_is_in_region : MAE_corner");

	for(j = 0; j < region->cornernum; j ++ )
	{
		xlist[j] = region->corners[j].x - x;
		ylist[j] = region->corners[j].y - y;
	}

	count = 0;
	for(j = 0; j < region->cornernum; j ++ )
	{
		if( xlist[j] == 0 || ylist[j] == 0) continue;
		count ++;
	}

	_mae_malloc_int( &xlist2, count * sizeof(int),"_mae_point_is_in_region : MAE_corner");
	_mae_malloc_int( &ylist2, count * sizeof(int),"_mae_point_is_in_region : MAE_corner");

	count = 0;
	for(j = 0; j < region->cornernum; j ++ )
	{
		if( xlist[j] == 0 || ylist[j] == 0) continue;
		xlist2[count] = xlist[j];
		ylist2[count] = ylist[j];
		count ++;
	}

	angle = 0;
	for(j = 0; j < count; j ++ )
	{
		k = j + 1;
		if( j == count - 1 ) k = 0;
		if( xlist2[j]  > 0 && ylist2[j] > 0 )
		{
			quad1 = 1;
		}else if( xlist2[j]  > 0 && ylist2[j] < 0 ){
			quad1 = 4;
		}else if( xlist2[j]  < 0 && ylist2[j] > 0 ){
			quad1 = 2;
		}else if( xlist2[j]  < 0 && ylist2[j] < 0 ){
			quad1 = 3;
		}
		if( xlist2[k]  > 0 && ylist2[k] > 0 )
		{
			quad2 = 1;
		}else if( xlist2[k]  > 0 && ylist2[k] < 0 ){
			quad2 = 4;
		}else if( xlist2[k]  < 0 && ylist2[k] > 0 ){
			quad2 = 2;
		}else if( xlist2[k]  < 0 && ylist2[k] < 0 ){
			quad2 = 3;
		}

		if( quad1 == 1 && quad2 == 2)  angle = angle + 90;
		if( quad1 == 1 && quad2 == 4)  angle = angle - 90;

		if( quad1 == 2 && quad2 == 3)  angle = angle + 90;
		if( quad1 == 2 && quad2 == 1)  angle = angle - 90;

		if( quad1 == 3 && quad2 == 4)  angle = angle + 90;
		if( quad1 == 3 && quad2 == 2)  angle = angle - 90;

		if( quad1 == 4 && quad2 == 1)  angle = angle + 90;
		if( quad1 == 4 && quad2 == 3)  angle = angle - 90;

	}


	free(xlist);
	free(ylist);
	free(xlist2);
	free(ylist2);

	if( angle == 360 ) 
	{
		return 1;
	}else{
		return 0;
	}
}

int _mae_save_mark_image_from_corners2(MAE_image *in_image, MAE_region *mae_regions, int nregions, MAE_image *out_image, int flag)
{
	int error = MAE_NO_ERROR;
	int left;
	int top;
	int right;
	int bottom;
	//int i,j,x,y;
	int i,j,x,y;
	int depth;
	int movedbits;
	long widthbytes;
	int x1,y1,x2,y2, dx,dy;
	unsigned char bkcolor;
	unsigned char *data;
	unsigned char *temp;
	unsigned char *in_data;
	unsigned char *in_temp;
	unsigned char *out_data;
	unsigned char *out_temp;
	unsigned char bkr[6];
	unsigned char bkg[6];
	unsigned char bkb[6];

	unsigned char r;
	unsigned char g;
	unsigned char b;

	bkr[0] = 255;
	bkg[0] = 0;
	bkb[0] = 0;
	bkr[1] = 0;
	bkg[1] = 255;
	bkb[1] = 0;
	bkr[2] = 0;
	bkg[2] = 0;
	bkb[2] = 255;
	bkr[3] = 255;
	bkg[3] = 255;
	bkb[3] = 0;
	bkr[4] = 255;
	bkg[4] = 0;
	bkb[4] = 255;
	bkr[5] = 0;
	bkg[5] = 255;
	bkb[5] = 255;

	bkcolor = 255;

	out_image->xdpi = in_image->xdpi;
	out_image->ydpi = in_image->ydpi;
	out_image->depth = in_image->depth;
	out_image->type = in_image->type;
	out_image->w  = in_image->w;
	out_image->h = in_image->h;
	out_image->stride = _mae_width_bytes(  out_image->depth, out_image->w );
	out_image->data  = _mae_mallocate_image(out_image->w,out_image->h,out_image->depth);

	memset(out_image->data, (BYTE)bkcolor, _mae_sizefromdepth(out_image->w,out_image->h,out_image->depth) );

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


	if( flag == MAE_GET_MARK_REGION_MASK)
	{
		return error;
	}

	if( flag & MAE_GET_MARK_REGION_FLAG2)
	{
		memcpy(out_image->data, in_image->data, _mae_sizefromdepth(out_image->w,out_image->h,out_image->depth) );
		if( flag & MAE_GET_MARK_REGION_FLAG1)
		{
			for(i = 0; i < nregions; i ++ )
			{
				//do not show region deleted
				if( mae_regions[i].label == -1 ) continue;

				left = mae_regions[i].l;
				top = mae_regions[i].t;
				right = left +  mae_regions[i].w -1;
				bottom = top +  mae_regions[i].h -1;
				for(y = top; y <= bottom ; y ++ )
				{
					in_data = in_image->data + widthbytes * y;
					out_data = out_image->data + widthbytes * y;
					for( x = left; x <= right; x ++ )
					{
						in_temp = in_data +  movedbits * x;
						out_temp = out_data + movedbits * x;
						if( movedbits == 1 )
						{
							*out_temp = *in_temp;
						}else{
							*(out_temp + 0) = *(in_temp + 0);
							*(out_temp + 1) = *(in_temp + 1);
							*(out_temp + 2) = *(in_temp + 2);
						}
					}
				}

			}
		}else{
			for(i = 0; i < nregions; i ++ )
			{
				//do not show region deleted
				if( mae_regions[i].label == -1 ) continue;

				left = mae_regions[i].l;
				top = mae_regions[i].t;
				right = left +  mae_regions[i].w -1;
				bottom = top +  mae_regions[i].h -1;
				for(y = top; y <= bottom ; y ++ )
				{
					//in_data = in_image->data + widthbytes * y;
					out_data = out_image->data + widthbytes * y;
					for( x = left; x <= right; x ++ )
					{
						//in_temp = in_data +  movedbits * x;
						out_temp = out_data + movedbits * x;

						//detect point is not in region or not 1: in 0: not
						if( _mae_point_is_in_region(x, y, &mae_regions[i]) == 1 ) 
						{
							if( movedbits == 1 )
							{
								*out_temp = bkcolor;
							}else{
								*(out_temp + 0) = bkcolor;
								*(out_temp + 1) = bkcolor;
								*(out_temp + 2) = bkcolor;
							}
						}
					}
				}

			}
		}

	}else{
		if( flag & MAE_GET_MARK_REGION_FLAG1)
		{
			for(i = 0; i < nregions; i ++ )
			{
				//do not show region deleted
				if( mae_regions[i].label == -1 ) continue;

				left = mae_regions[i].l;
				top = mae_regions[i].t;
				right = left +  mae_regions[i].w -1;
				bottom = top +  mae_regions[i].h -1;
				for(y = top; y <= bottom ; y ++ )
				{
					in_data = in_image->data + widthbytes * y;
					out_data = out_image->data + widthbytes * y;
					for( x = left; x <= right; x ++ )
					{
						in_temp = in_data +  movedbits * x;
						out_temp = out_data + movedbits * x;
						//detect point is not in region or not 1: in 0: not
						if( _mae_point_is_in_region(x, y, &mae_regions[i]) == 1 ) 
						{
							if( movedbits == 1 )
							{
								*out_temp = *in_temp;
							}else{
								*(out_temp + 0) = *(in_temp + 0);
								*(out_temp + 1) = *(in_temp + 1);
								*(out_temp + 2) = *(in_temp + 2);
							}
						}
					}
				}

			}
		}else{
			for(i = 0; i < nregions; i ++ )
			{
				//do not show region deleted
				if( mae_regions[i].label == -1 ) continue;

				left = mae_regions[i].l;
				top = mae_regions[i].t;
				right = left +  mae_regions[i].w -1;
				bottom = top +  mae_regions[i].h -1;
				for(y = top; y <= bottom ; y ++ )
				{
					//in_data = in_image->data + widthbytes * y;
					out_data = out_image->data + widthbytes * y;
					for( x = left; x <= right; x ++ )
					{
						//in_temp = in_data +  movedbits * x;
						out_temp = out_data + movedbits * x;
						//detect point is not in region or not 1: in 0: not
						if( _mae_point_is_in_region(x, y, &mae_regions[i]) == 1 ) 
						{
							if( movedbits == 1 )
							{
								*out_temp = bkcolor;
							}else{
								*(out_temp + 0) = bkcolor;
								*(out_temp + 1) = bkcolor;
								*(out_temp + 2) = bkcolor;
							}
						}
					}
				}

			}
		}
	}

	data = out_image->data;


	if( flag & MAE_GET_MARK_REGION_FLAG3)
	{
		for(i = 0; i < nregions; i ++ )
		{
			if(mae_regions[i].label == -1 )
			{
				continue;
			}
			if( i < 6 )
			{
				r = bkr[i];
				g = bkg[i];
				b = bkb[i];
			}else{
				r = 0;
				g = 0;
				b = 0;
			}
			for(j = 1; j <= mae_regions[i].cornernum; j ++ )
			{
				if( j == mae_regions[i].cornernum )
				{
					x1 = mae_regions[i].corners[j-1].x;
					y1 = mae_regions[i].corners[j-1].y;
					x2 = mae_regions[i].corners[0].x;
					y2 = mae_regions[i].corners[0].y;
				}else{
					x1 = mae_regions[i].corners[j-1].x;
					y1 = mae_regions[i].corners[j-1].y;
					x2 = mae_regions[i].corners[j].x;
					y2 = mae_regions[i].corners[j].y;
				}
				dx = abs(x2-x1);
				dy = abs(y2-y1);
				if( dx < dy ) //vert line
				{
					if( y1 > y2 )
					{
						y = y1;
						y1 = y2;
						y2 = y;
					}
					if( x1 > x2 )
					{
						x = x1;
						x1 = x2;
						x2 = x;
					}
					_mae_draw_line(data, out_image->w, out_image->h, widthbytes, movedbits, x1, y1, x2, y2,r,g,b, 0);
				}else{ //hori line
					if( y1 > y2 )
					{
						y = y1;
						y1 = y2;
						y2 = y;
					}
					if( x1 > x2 )
					{
						x = x1;
						x1 = x2;
						x2 = x;
					}
					_mae_draw_line(data, out_image->w, out_image->h, widthbytes, movedbits, x1, y1, x2, y2, r,g,b,1);
				}
			}
		}
	}

	return error;
}