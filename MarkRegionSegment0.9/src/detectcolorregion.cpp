#include "stdafx.h"
#include <stdlib.h>
#include "maedefs.h"
#include "detectcolorregion.h"
#include "imageprocess.h"
#include "imageutil.h"
#include "segment.h"
#include "histogram.h"
#include "runs.h"
#include "blocks.h"
#include "memalloc.h"
//
//void _mae_detect_mark_color(unsigned char *h_data, unsigned char *s_data, int width, int height, MAE_region * regions, int nregions)
//{
//	int i;
//
//	for(i = 0; i < nregions; i ++ )
//	{
//		//do not show region deleted
//		if( regions[i].label == -1 ) continue;
//		if( _mae_detect_color_region2(h_data, s_data, width, height, &regions[i]) )
//		{
//			regions[i].label = -1;
//		}
//	}
//	return;
//}
//
//int _mae_detect_color_region(unsigned char *h_data, unsigned char *s_data, int width, int height, MAE_region* region)
//{
//	int x;
//	int y;
//	int left;
//	int top;
//	int right;
//	int bottom;
//	unsigned char * stemp;
//	unsigned char * stemp2;
//	unsigned char * htemp;
//	unsigned char * htemp2;
//	long _count = 0;
//
//	left = region->l;
//	top = region->t;
//	right  = region->l + region->w;
//	bottom = region->t + region->h;
//
//	for(y = top; y < bottom; y ++ )
//	{
//		stemp = s_data + y * height;
//		for(x = left; x < right; x ++ )
//		{
//			stemp2 = stemp + x;
//			if( *stemp2 == 0 )
//			{
//				continue;
//			}
//			_count++;
//			//if( _count == 1000 ) break;
//		}
//		//if( _count == 1000 ) break;
//	}
//
//	//int k,n;
//	//dataset *head;
//	//dataset *ds;
//	//init(&head, _count);
//
//	//_count = 0;
//	//ds = head;
//	int * hues;
//	_mae_malloc_int(&hues, _count, "_mae_detect_color_region : hues");
//
//	memset(hues,(int)0,_count);
//	_count = 0;
//	for(y = top; y < bottom; y ++ )
//	{
//		stemp = s_data + y * height;
//		htemp = h_data + y * height;
//		for(x = left; x < right; x ++ )
//		{
//			stemp2 = stemp + x;
//			htemp2 = htemp + x;
//			if( *stemp2 == 0 )
//			{
//				continue;
//			}
//			hues[_count++] = *htemp2;
//			//_count ++;
//			//if( _count == 1000 ) break;
//			//ds->p.h = *htemp2;
//			//ds = ds->next;
//			//if( ds == NULL )
//			//{
//			//	break;
//			//}
//		}
//		//if( _count == 1000 ) break;
//	}
//	
//	//if( _mae_opt_k_means(head, _count) > 3 )
//	//{
//	//	return 1;
//	//}
//	_mae_compute_variance(hues, _count);
//
//	free(hues);
//	return 0;
//}
//int _mae_opt_k_means(dataset *head,	int n)
//{
//	int k;
//	int min;
//	int idx;
//	double std[4];
//
//	for( k = 0; k < 4 ; k ++ )
//	{
//		std[k] = 0;
//	}
//	for(k = 1; k < 5; k ++ )
//	{
//		clusters *clhead;
//		kmeans(head, &clhead, k);
//		std[k-1] = _mae_compute_k_means(clhead);	
//		free_clusters(clhead);
//	}
//	free_dataset(head);
//	
//	min = std[0];
//	idx = 0;
//	for( k = 1; k < 4 ; k ++ )
//	{
//		if( min < std[k] )
//		{
//			min = std[k];
//			idx = k;
//		}
//	}
//	
//	return k;
//}
//double _mae_compute_k_means(clusters *clhead)
//{
//	clusters *clsp;
//	cluster *clu;
//	int i;
//	double std = 0;
//	for(clsp=clhead,i=0;clsp!=NULL;clsp=clsp->next,i++)
//	{
//	   for(clu=clsp->clp;clu!=NULL;clu=clu->next)
//	   {
//		   std += distance(clu->p,&clsp->center);
//	   }
//	}
//	return std;
//}
//double subtract_square(double val1, double val2)
//{
//	double tmph;
//	tmph = val1 - val2;
//	return (tmph * tmph); 
//}
//
//int _mae_compute_variance(int *hues, int n)
//{
//	double variance;
//	double mean;
//	dataset *dp;
//	long count;
//	int i;
//	count = 0;
//	mean = 0;
//	int max;
//	int min;
//	max = 0;
//	min = 256;
//	for(i = 0; i < n; i ++)
//	{
//		if( hues[i] == 0 ) continue;
//		mean += hues[i] ;
//		if(max < hues[i]) max  = hues[i];
//		if(min > hues[i]) min  = hues[i];
//	}	
//	mean = mean / (double)n;
//	variance = 0;
//	for(i = 0; i < n; i ++)
//	{
//		if( hues[i] == 0 ) continue;
//		variance += subtract_square(hues[i], mean );
//	}
//	variance = variance  / (double)n;
//	return variance;
//}
//
//
//
//int _mae_detect_color_region2(unsigned char *h_data, unsigned char *s_data, int width, int height, MAE_region* region)
//{
//	int x;
//	int y;
//	int left;
//	int top;
//	int right;
//	int bottom;
//	unsigned char * stemp;
//	unsigned char * stemp2;
//	unsigned char * htemp;
//	unsigned char * htemp2;
//	long _count = 0;
//
//	left = region->l;
//	top = region->t;
//	right  = region->l + region->w;
//	bottom = region->t + region->h;
//
//	for(y = top; y < bottom; y ++ )
//	{
//		stemp = s_data + y * height;
//		for(x = left; x < right; x ++ )
//		{
//			stemp2 = stemp + x;
//			if( *stemp2 == 0 )
//			{
//				continue;
//			}
//			_count++;
//			//if( _count == 1000 ) break;
//		}
//		//if( _count == 1000 ) break;
//	}
//
//	//int k,n;
//	//dataset *head;
//	//dataset *ds;
//	//init(&head, _count);
//
//	//_count = 0;
//	//ds = head;
//	int * hues;
//	_mae_malloc_int(&hues, _count, "_mae_detect_color_region : hues");
//
//	memset(hues,(int)0,_count);
//	_count = 0;
//	for(y = top; y < bottom; y ++ )
//	{
//		stemp = s_data + y * height;
//		htemp = h_data + y * height;
//		for(x = left; x < right; x ++ )
//		{
//			stemp2 = stemp + x;
//			htemp2 = htemp + x;
//			if( *stemp2 == 0 )
//			{
//				continue;
//			}
//			hues[_count++] = *htemp2;
//			//_count ++;
//			//if( _count == 1000 ) break;
//			//ds->p.h = *htemp2;
//			//ds = ds->next;
//			//if( ds == NULL )
//			//{
//			//	break;
//			//}
//		}
//		//if( _count == 1000 ) break;
//	}
//	
//	//if( _mae_opt_k_means(head, _count) > 3 )
//	//{
//	//	return 1;
//	//}
//	_mae_compute_variance(hues, _count);
//
//	free(hues);
//	return 0;
//}
//
//

void _mae_detect_text_size(MAE_image *in_image, MAE_init_feature * init_feature)
{
	MAE_image subimage;	
	int width;
	int height;
	int depth;
	long widthbytes;
	int rhist[256];
	int ghist[256];
	int bhist[256];
	long pixs;

	width = in_image->w / 8;
	height = in_image->h / 8;
	depth = in_image->depth;
	widthbytes = _mae_width_bytes(depth, width);

	_mae_get_region_image(in_image, 0, 0, width, height, &subimage );

	//histogram r g b
	_mae_hist_high_rgb(subimage.data, width, height, depth, widthbytes,
		rhist, ghist, bhist, &pixs);

	free(subimage.data);

	int rtemp;
	int rcol;
	int gtemp;
	int gcol;
	int btemp;
	int bcol;
	int i;
	rtemp = 0;
	gtemp = 0;
	btemp = 0;
	for(i = 0; i < 256; i ++ )
	{
		if(rtemp <  rhist[i] )
		{
			rtemp = rhist[i];
			rcol = i;
		}
		if(gtemp <  ghist[i] )
		{
			gtemp = ghist[i];
			gcol = i;
		}
		if(btemp <  bhist[i] )
		{
			btemp = bhist[i];
			bcol = i;
		}
	}
	init_feature->r_bk = rcol;
	init_feature->g_bk = gcol;
	init_feature->b_bk = bcol;	
	//get runs
	//unsigned char * out_data;
	//out_data = _mae_mallocate_image(width,height,8);
	//int   mode = 2;
	//int stru_size=3;
	//int structure[3][3]={ {-1, 0, -1}, {0, 0, 0}, {-1, 0, -1}  };
	//_mae_dilation(bi_data, width, height, mode, (int**)structure, stru_size, out_data);
	//int *h_rx, *h_ry, *h_rlen, h_nruns, h_ralloc;
	//MAE_block *blocks;
	//int nblocks;
	//MAE_node * run_list;
	//int listnum;
	//int *recthist;
	//int gridx;
	//int gridy;

	////get runs
	//_mae_get_runs(&h_rx, &h_ry, &h_rlen, &h_nruns, &h_ralloc, bi_data, width, height);
	///* get run chain list */
	//_mae_create_runs_chain(&run_list,&listnum , h_rx, h_ry, h_rlen, h_nruns);
	////detect blocks
	//   _mae_detect_blocks(run_list, listnum, &blocks, &nblocks);
	////compute blocks rect	
	//_mae_computer_blocks_rect(&blocks, &nblocks);
	////remove small rect
	//_mae_delete_small_blocks2(blocks, nblocks, 10);	
	//
	////histogram  blocks rect
	//gridx = width / 5 + 1 ;
	//gridy = height / 5 + 1;
	//_mae_malloc_int(&recthist, gridx * gridy, "_mae_hist_mark_region : recthist");
	//_mae_histogram_rect(blocks, nblocks, recthist, gridx, gridy);	
	//int i;
	//int j;
	//int w = 0;
	//int h = 0;
	//int count = 0;
	//for(j = 0; j < gridy; j ++ )
	//{
	//	for(i = 0; i < gridx; i ++ )
	//	{
	//		if( count <= recthist[j*gridx + i] )
	//		{
	//			count = recthist[j*gridx + i];
	//			w = (i - 1) * 5;
	//			h = (j - 1 ) * 5;
	//		}
	//	}	
	//}
	//init_feature->t_width  = w;
	//init_feature->t_height  = h;

	//free(h_rx);
	//free(h_ry);
	//free(h_rlen);
	//free(blocks);
	//free(recthist);
	//free(out_data);		
}

/*******************************************************/
/* function name: _mae_detect_mark_color_region_all
/* parameter	: MAE_image *in_image : original input image
/*                MAE_image * h_image : detect hight color image
/*                MAE_region *regions : detect regions list
/*                int nregions : detect regions number
/*                int flag       1: remove mark color, 0: don't remove mark color
/* return	    : int  error
/* function		: detect all color region is background image or mark region
/*******************************************************/
//int _mae_detect_mark_color_region_all(MAE_image *in_image, MAE_image *h_image, unsigned char *bi_data, unsigned char *out_data, MAE_region * regions, int nregions)
//int _mae_detect_mark_color_region_all(MAE_image *in_image, MAE_image *h_image, unsigned char *bi_data, unsigned char *out_data, MAE_region * regions, int nregions, MAE_init_feature * init_feature)
//int _mae_detect_mark_color_region_all(MAE_image *in_image, MAE_image *h_image, unsigned char *bi_data, unsigned char *out_data, MAE_region * regions, int nregions, MAE_init_feature * init_feature, MAE_image_feature **features, int *fnum)
//int _mae_detect_mark_color_region_all(MAE_image *in_image, MAE_image *h_image, unsigned char *bi_data, unsigned char *out_data, MAE_region * regions, int nregions, MAE_init_feature * init_feature)
int _mae_detect_mark_color_region_all(MAE_image *in_image, MAE_image *h_image, unsigned char *bi_data, MAE_region * regions, int nregions, MAE_init_feature * init_feature)
{
	int error = MAE_NO_ERROR;
	int i;
	//int idx;
	//	MAE_region region;
	//int row_width1;
	//int row_width2;
	//int col_width1;
	//int col_width2;
	//20100902 add for speed
	//if( init_feature->flag == 0 )
	//{
	//	_mae_malloc_uchar((unsigned char**)(features), nregions * sizeof(MAE_image_feature),"_mae_detect_mark_color_region_all : features");
	//	*fnum = nregions;
	//}
	//int step;
	//step = 5;
	if( init_feature->flag == 0 )
	{
		for(i = 0; i < nregions; i ++ )
		{
			//do not show region deleted
			if( regions[i].label == MAE_MARK_REGION_NO ) {
				//((*features)[i]).label = regions[i].label;
				continue;
			}
			if(regions[i].w < MAE_SMALL_LINE_WIDTH || regions[i].h < MAE_SMALL_LINE_HEIGHT )
			{
				regions[i].label = MAE_MARK_REGION_NO;
				//((*features)[i]).label = regions[i].label;
				continue;
			}
			//_mae_detect_mark_color_region(in_image, h_image, bi_data, out_data, &regions[i]);
			//_mae_detect_mark_color_region(in_image, h_image, bi_data, out_data, &regions[i], init_feature, &(*features)[i]);
			//_mae_detect_mark_color_region(in_image, h_image, bi_data, out_data, &regions[i], init_feature);
			_mae_detect_mark_color_region(in_image, h_image, bi_data, &regions[i], init_feature);
		}
	}else{
		for(i = 0; i < nregions; i ++ )
		{
			//do not show region deleted
			if(    regions[i].label == MAE_MARK_REGION_RECHECK 
				|| regions[i].label == MAE_MARK_REGION_MERGE_NEW )
			{
				//_mae_detect_mark_color_region(in_image, h_image, bi_data, out_data, &regions[i]);
				//_mae_detect_mark_color_region(in_image, h_image, bi_data, out_data, &regions[i], init_feature, &(*features)[i]);
				//_mae_detect_mark_color_region(in_image, h_image, bi_data, out_data, &regions[i], init_feature);
				_mae_detect_mark_color_region(in_image, h_image, bi_data, &regions[i], init_feature);
			}
		}
	}
	return error;
}
/*******************************************************/
/* function name: _mae_detect_mark_color_region
/* parameter	: MAE_image *in_image : original input image
/*                MAE_image * h_image : detect hight color image
/*                MAE_region *region : detect region
/*                int flag       1: remove mark color, 0: don't remove mark color
/* return	    : void
/* function		: detect one color region is background image or mark region
/*******************************************************/
//void _mae_detect_mark_color_region(MAE_image *in_image, MAE_image *h_image, unsigned char *bi_data, unsigned char *out_data,MAE_region* region)
//void _mae_detect_mark_color_region(MAE_image *in_image, MAE_image *h_image, unsigned char *bi_data, unsigned char *out_data,MAE_region* region, MAE_init_feature * init_feature, MAE_image_feature *feature)
//void _mae_detect_mark_color_region(MAE_image *in_image, MAE_image *h_image, unsigned char *bi_data, unsigned char *out_data,MAE_region* region, MAE_init_feature * init_feature)
void _mae_detect_mark_color_region(MAE_image *in_image, MAE_image *h_image, unsigned char *bi_data, MAE_region* region, MAE_init_feature * init_feature)
{
	//	long mk_pix_num;
	//	long mt_pix_num;
	//	float ratio;
	MAE_image_feature feature;

	//get feature from region image
	//_mae_hist_mark_region(in_image, h_image, bi_data, region,  &feature);
	//_mae_get_mark_region_feature(in_image, h_image, bi_data, out_data, region, &feature, init_feature);
	//if( region->label != MAE_MARK_REGION_RECHECK )
	//{
	_mae_hist_mark_region(in_image, h_image, bi_data, region,  &feature);
	//_mae_get_mark_region_feature(in_image, h_image, bi_data, out_data, region, &feature, init_feature);
	_mae_get_mark_region_feature(in_image, h_image, bi_data, region, &feature, init_feature);
	//}


	//_mae_check_region_on_feature3(region, &feature, init_feature);
	//_mae_check_region_on_feature3(region, &feature, init_feature);
	_mae_check_region_on_feature3(region, &feature, init_feature);
	if( region->label == MAE_MARK_REGION_NO )
	{
		return;
	}

	//_mae_check_region_on_feature1(region, &feature, init_feature);
	//_mae_check_region_on_feature1(region, &feature, init_feature);
	//if( region->label == MAE_MARK_REGION_NO )
	//{
	//	return;
	//}

	//_mae_hist_mark_region(in_image, h_image, bi_data, region,  &feature);
	//_mae_check_region_on_feature2(region, &feature, init_feature);
	//_mae_check_region_on_feature2(region, &feature, init_feature);
	_mae_check_region_on_feature2(region, &feature, init_feature);

	if( region->label == MAE_MARK_REGION_NO )
	{
		return;
	}


	//if( !flag )
	//{
	//	_mae_hist_mark_region(in_image, h_image, region);
	//}
	//	mk_pix_num = region->mk_pix_num;
	//	mt_pix_num = region->mt_pix_num;
	//ratio = (float)mt_pix_num / (float)mk_pix_num;
	////remains text is less 
	//// ratio should be very tiny
	//if( ratio > MAE_MARK_TEXT_RATIO )
	//{
	//	region->label = -1 ;
	//}

	return ;
}
/*******************************************************/
/* function name: _mae_hist_mark_region
/* parameter	: MAE_image *in_image : original input image
/*                MAE_image * h_image : detect hight color image
/*                MAE_region *region : detect region
/* return	    : void
/* function		: histgram mark region pix num and mark region text pix num
/*******************************************************/
void _mae_hist_mark_region(MAE_image *in_image, MAE_image *h_image, unsigned char *bi_data, MAE_region* region,  MAE_image_feature *feature)
{
	MAE_image tmp_region_image; 
	MAE_image region_image; 

	unsigned char * gray_data;
	unsigned char * seg_data;
	unsigned char * remove_color_data;
	long mk_pix_num = 0;
	long mt_pix_num = 0;
	int width;
	int height;
	MAE_image *para_h_image;
	para_h_image = h_image;
	//get region image
	_mae_get_region_image(in_image, region->l, region->t, region->w, region->h, &tmp_region_image );
	////detect hight color
	_mae_detect_high_color(&tmp_region_image, &region_image, RGB_SEGMENT_VARIENCE);
	free(tmp_region_image.data);

	remove_color_data = _mae_cut_image(bi_data, in_image->w, in_image->h, region->l, region->t, region->w, region->h);
#ifdef MAE_DEBUG
	_mae_save_segment_image(&tmp_region_image, remove_color_data, region->w, region->h, &region_image);
	_mae_save_image("test_remove_color__8888.jpg", &tmp_region_image);
	free(tmp_region_image.data);
#endif

	width = region->w;
	height = region->h;
	gray_data = _mae_mallocate_image(width,height,8);
	seg_data = _mae_mallocate_image(width,height,8);

	_mae_convert_rgb_to_gray(gray_data, &region_image);
	//////////////test start
#ifdef MAE_DEBUG
	_mae_save_gray_image(&tmp_region_image,gray_data, width, height, &region_image );
	_mae_save_image("test_region_gray888.jpg", &tmp_region_image );
	free(tmp_region_image.data);
#endif
	//////////////test end

	_mae_histogram_mark_pixnum(gray_data, width, height, &mk_pix_num);
	feature->mk_pix_num = mk_pix_num;

	_mae_segment_only_mark_pixs(gray_data, width, height, seg_data, 0);
	//////////////test start
#ifdef MAE_DEBUG
	_mae_save_segment_image(&tmp_region_image,seg_data, width, height, &region_image );
	_mae_save_image("test_region_segment888.bmp", &tmp_region_image );
	free(tmp_region_image.data);
#endif
	//////////////test end

	//	_mae_histogram_text_pixnum(seg_data, width, height, &mt_pix_num);
	//	feature->mt_pix_num = mt_pix_num;

	//gray_data minus bi_data
	_mae_plus_image(seg_data, width, height, remove_color_data);

	//#ifdef MAE_DEBUG
	//_mae_save_segment_image(&tmp_region_image, seg_data, width, height, &region_image );
	//_mae_save_image("test_region_BI_segment888.bmp", &tmp_region_image );
	//free(tmp_region_image.data);
	//#endif

	_mae_histogram_text_pixnum(seg_data, width, height, &mt_pix_num);
	feature->mt_pix_num = mt_pix_num;

	//get runs
	/*  
	unsigned char * out_data;
	//unsigned char * tmp_data;
	out_data = _mae_mallocate_image(width,height,8);
	int   mode = 2;
	int stru_size=3;
	int structure[3][3]={ {-1, 0, -1}, {0, 0, 0}, {-1, 0, -1}  };

	//_mae_save_segment_image(&tmp_region_image, seg_data, width, height, &region_image );
	//_mae_save_image("test_region_segment111.jpg", &tmp_region_image );
	//free(tmp_region_image.data);

	_mae_dilation(seg_data, width, height, mode, (int**)structure, stru_size, out_data);
	_mae_erosion(out_data, width, height, mode, (int**)structure, stru_size,seg_data);

	//_mae_save_segment_image(&tmp_region_image, seg_data, width, height, &region_image );
	//_mae_save_image("test_region_segment222.jpg", &tmp_region_image );
	//free(tmp_region_image.data);
	*/

	int *h_rx, *h_ry, *h_rlen, h_nruns, h_ralloc;
	MAE_block *blocks;
	int nblocks;
	MAE_node * run_list;
	int listnum;
	//	int *recthist;
	//	int gridx;
	//	int gridy;

	//get runs
	_mae_get_runs(&h_rx, &h_ry, &h_rlen, &h_nruns, &h_ralloc, seg_data, width, height);
	// get run chain list 
	_mae_create_runs_chain(&run_list,&listnum , h_rx, h_ry, h_rlen, h_nruns);
	//detect blocks
	_mae_detect_blocks(run_list, listnum, &blocks, &nblocks);
	//compute blocks rect	
	_mae_computer_blocks_rect(&blocks, &nblocks);

	//remove small rect
	_mae_delete_small_blocks2(blocks, nblocks, 10);	

	//get max connect size
	_mae_max_connect_size(blocks, nblocks, feature);
	//histogram  blocks rect
	//gridx = width / 5 + 1 ;
	//gridy = height / 5 + 1;
	//_mae_malloc_int(&recthist, gridx * gridy, "_mae_hist_mark_region : recthist");
	//_mae_histogram_rect(blocks, nblocks, recthist, gridx, gridy);	
	//_mae_parse_rect_hist(recthist, gridx, gridy, feature);	

	free(blocks);
	//	free(recthist);
	free(region_image.data );
	free(gray_data);
	free(seg_data);
	//free(out_data);

}
/*******************************************************/
/* function name: _mae_get_mark_region_feature
/* parameter	: MAE_image *in_image : original input image
/*                MAE_image * h_image : detect hight color image
/*                MAE_region *region : detect region
/* return	    : void
/* function		: feature
/*******************************************************/
//void _mae_get_mark_region_feature(MAE_image *in_image, MAE_image *h_image, unsigned char *bi_data, unsigned char *out_data, MAE_region* region, MAE_image_feature *feature, MAE_init_feature * init_feature)
void _mae_get_mark_region_feature(MAE_image *in_image, MAE_image *h_image, unsigned char *bi_data, MAE_region* region, MAE_image_feature *feature, MAE_init_feature * init_feature)
{
	MAE_image tmp_region_image; 
	MAE_image region_image; 

	unsigned char * remove_color_data;
	//unsigned char * gray_data;
	//unsigned char * bi_data;
	//	long mk_pix_num = 0;
	long center_mk_pix_num = 0;
	//long mt_pix_num = 0;
	//int width;
	//int height;
	int cx,cy,cw,ch;
	int i, j;


	//initialize feature
	feature->width = 0;
	feature->height = 0;
	feature->area = 0;
	feature->mark_pixs = 0;
	feature->contour_pixs = 0;
	//20110119 add 
	feature->min_re = 0;
	feature->min_ge = 0;
	feature->min_be = 0;
	feature->max_re = 0;
	feature->max_ge = 0;
	feature->max_be = 0;
	feature->min_he = 0;
	feature->max_he = 0;
	//
	for(i = 0; i < 9; i ++ )
	{
		feature->r_e[i] = 0;
		feature->g_e[i] = 0;
		feature->b_e[i] = 0;
		feature->submpixs[i] = 0;
		//feature->r_v[i] = 0;
		//feature->g_v[i] = 0;
		//feature->b_v[i] = 0;
		feature->h_e[i] = 0;
		//feature->h_v[i] = 0;
	}

	feature->hsplitnum = 3;
	feature->vsplitnum = 3;

	feature->tw = 0;
	feature->th = 0;
	feature->tcount = 0;

	//set width,height and area to feature
	feature->width = region->w;
	feature->height = region->h;
	feature->area = region->w * region->h;

	//20110120 add start
	if(feature->width > 100 )
	{
		feature->hsplitnum = 3;
	}else if(feature->width > 50 ){
		feature->hsplitnum = 2;
	}else{
		feature->hsplitnum = 1;
	}
	if(feature->height > 100 )
	{
		feature->vsplitnum = 3;
	}else if(feature->height > 50 ){
		feature->vsplitnum = 2;
	}else{
		feature->vsplitnum = 1;
	}
	if( feature->hsplitnum == 1 && feature->vsplitnum == 1 )
	{
		return;
	}
	//20110120 add end
	//get region contour pixs
	long contour_pixs = 0;
	int bkcol = (init_feature->b_bk + init_feature->g_bk + init_feature->r_bk) / 3 - 5;
	_mae_hist_contour_pixs(in_image->data, in_image->w, in_image->h, in_image->depth,  
		in_image->stride, bkcol, region->l, region->t, region->w, region->h, &contour_pixs);
	feature->contour_pixs = contour_pixs;

	//	_mae_hist_mark_region(in_image, h_image, bi_data, region,  feature);

	//set mark pixs to feature
	//get region image
	//_mae_get_region_image(in_image, region->l, region->t, region->w, region->h, &tmp_region_image );
	////detect hight color
	//_mae_detect_high_color(&tmp_region_image, &region_image, RGB_REMOVE_TEXT_VARIENCE);
	//free(tmp_region_image.data);

	_mae_get_region_image(h_image, region->l, region->t, region->w, region->h, &region_image );

	remove_color_data = _mae_cut_image(bi_data, in_image->w, in_image->h, region->l, region->t, region->w, region->h);

	//#ifdef MAE_DEBUG
	//MAE_image test_image;
	//_mae_save_segment_image(&test_image, remove_color_data, region->w, region->h, &region_image);
	//_mae_save_image("test_segment_001.jpg", &test_image);
	//free(test_image.data);
	//_mae_save_image("test_h_region_001.jpg", &region_image);
	//#endif
	cx = region_image.w / 2;
	cy = region_image.h / 2;
	cw = region_image.w / 4;
	ch = region_image.h / 4;
	cx = cx - cw/2;
	cy = cy - ch/2;
	center_mk_pix_num = 0;
	//get center rect color pixs
	_mae_hist_high_color_pixs(region_image.data, region_image.w, region_image.h, region_image.depth, region_image.stride,
		cx, cy, cw, ch, &center_mk_pix_num);
	feature->center_area = cw * ch;
	feature->center_mark_pixs = center_mk_pix_num;
	free(remove_color_data);
	free(region_image.data);

	int left[3][3];
	int top[3][3];
	//int width;
	//int height;
	int l, t,w,h;
	l = region->l;
	t = region->t;
	//w = region->w / 3;
	//h = region->h / 3;
	w = region->w / feature->hsplitnum;
	h = region->h / feature->vsplitnum;

	//left half
	//for(j = 0; j < 3; j ++ )
	//for(i = 0; i < 3; i ++ )
	for(j = 0; j < feature->vsplitnum; j ++ )
		for(i = 0; i < feature->hsplitnum; i ++ )
		{
			left[j][i] = l + i * w;
			top[j][i] = t + j * h;
		}
		//left[0] = l;
		//top[0] = region->t;
		//width[0] = region->w / 3;
		//height[0] = region->h / 3;

		////right half
		//left[1] = region->l + region->w / 2;
		//top[1] = region->t;
		//width[1] = region->w / 2;
		//height[1] = region->h;

		////up half
		//left[2] = region->l;
		//top[2] = region->t;
		//width[2] = region->w;
		//height[2] = region->h / 2;

		////down half
		//left[3] = region->l;
		//top[3] = region->t + region->h / 2;
		//width[3] = region->w;
		//height[3] = region->h / 2;
		//
		//	for( int i = 0; i < 4; i ++ )
		int idx = 0;
		int re,ge,be,he;
		int mark_pixs;
		//for( j = 0; j < 3; j ++ )
		//for( i = 0; i < 3; i ++ )
		for(j = 0; j < feature->vsplitnum; j ++ )
			for(i = 0; i < feature->hsplitnum; i ++ )
			{
				l = left[j][i];
				t = top[j][i];
				//w = width[i];
				//h = height[i];

				_mae_get_region_image(in_image, l, t, w, h, &tmp_region_image );
				_mae_detect_high_color(&tmp_region_image, &region_image, RGB_REMOVE_TEXT_VARIENCE);
				//_mae_get_region_image(h_image, l, t, w, h, &region_image );
				free(tmp_region_image.data);
				//#ifdef MAE_DEBUG
				//	_mae_save_image("test_h_region_002.jpg", &region_image);
				//#endif

				//get color expect and variance  from region high color image
				//20100831 update start
				re = 0;
				ge = 0;
				be = 0;
				mark_pixs = 0;
				//_mae_get_image_rgb_e_v(&region_image, feature, idx);
				_mae_get_image_rgb_e_v(&region_image, &re, &ge, &be, &mark_pixs);
				feature->r_e[idx] = (float)re;
				feature->g_e[idx] = (float)ge;
				feature->b_e[idx] = (float)be;
				feature->submpixs[idx] = (float)mark_pixs;
				feature->mark_pixs += mark_pixs;
				//get h color expect and variance  from hsv of high color image
				//_mae_get_image_h_e_v(&region_image, feature, idx);
				_mae_get_image_h_e_v(&region_image, &he);
				feature->h_e[idx] = (float)he;
				//20100831 update end

				idx ++;
				free(region_image.data);
			}
			feature->min_re = 255;
			feature->max_re = 0;
			feature->min_ge = 255;
			feature->max_ge = 0;
			feature->min_be = 255;
			feature->max_be = 0;
			feature->min_he = 255;
			feature->max_he = 0;
			//	for(i = 0; i < 9; i ++ )
			for(i = 0; i < feature->vsplitnum * feature->hsplitnum; i ++ )
			{
				if(feature->r_e[i] != 0 && feature->min_re > feature->r_e[i] )
					//if(feature->min_re > feature->r_e[i] )
				{
					feature->min_re = feature->r_e[i]; 
				}
				if(feature->r_e[i] != 255 && feature->max_re < feature->r_e[i] )
					//if(feature->max_re < feature->r_e[i] )
				{
					feature->max_re = feature->r_e[i]; 
				}
				if(feature->g_e[i] != 0 && feature->min_ge > feature->g_e[i] )
					//if(feature->min_ge > feature->g_e[i] )
				{
					feature->min_ge = feature->g_e[i]; 
				}
				if(feature->g_e[i] != 255 && feature->max_ge < feature->g_e[i] )
					//if(feature->max_ge < feature->g_e[i] )
				{
					feature->max_ge = feature->g_e[i]; 
				}
				if(feature->b_e[i] != 0 && feature->min_be > feature->b_e[i] )
					//if(feature->min_be > feature->b_e[i] )
				{
					feature->min_be = feature->b_e[i]; 
				}
				if(feature->b_e[i] != 255 && feature->max_be < feature->b_e[i] )
					//if(feature->max_be < feature->b_e[i] )
				{
					feature->max_be = feature->b_e[i]; 
				}
				if(feature->h_e[i] != 0 && feature->min_he > feature->h_e[i] )
					//if(feature->min_he > feature->h_e[i] )
				{
					feature->min_he = feature->h_e[i]; 
				}
				if(feature->h_e[i] != 255 && feature->max_he < feature->h_e[i] )
					//if(feature->max_he < feature->h_e[i] )
				{
					feature->max_he = feature->h_e[i]; 
				}
			}



}


//void _mae_get_image_rgb_e_v(MAE_image *in_image, MAE_image_feature *feature, int idx)
void _mae_get_image_rgb_e_v(MAE_image *in_image, int *re,int *ge, int *be, int *mark_pixs)
{
	int depth;
	int width;
	int height;
	long widthbytes;

	int rhist[256];
	int ghist[256];
	int bhist[256];
	long pixs;
	depth = in_image->depth;
	width = in_image->w;
	height = in_image->h;
	widthbytes = _mae_width_bytes(depth, width);
	//histogram r g b
	_mae_hist_high_rgb(in_image->data, width, height, depth, widthbytes,
		rhist, ghist, bhist, &pixs);

	//feature->mark_pixs = pixs;
	*mark_pixs = pixs;

	int rtemp;
	int rcol;
	int gtemp;
	int gcol;
	int btemp;
	int bcol;
	int i;
	int k;
	int step = 8;
	int sum;
	rtemp = 0;
	gtemp = 0;
	btemp = 0;
	rcol = 0;
	gcol = 0;
	bcol = 0;

	sum = 0;
	//	for(i = 0; i < 256; i ++ )
	for(i = 0; i < 256 - step; i ++ )
	{
		sum = 0;
		for(k = 0; k < step; k ++ )
		{
			sum = sum + rhist[i+k];
		}
		if(rtemp <  sum )
		{
			rtemp = sum;
			rcol = i;
		}
		sum = 0;
		for(k = 0; k < step; k ++ )
		{
			sum = sum + ghist[i+k];
		}
		if(gtemp <  sum )
		{
			gtemp = sum;
			gcol = i;
		}
		sum = 0;
		for(k = 0; k < step; k ++ )
		{
			sum = sum + bhist[i+k];
		}
		if(btemp <  sum )
		{
			btemp = sum;
			bcol = i;
		}
	}

	*re = rcol+step/2;
	*ge = gcol+step/2;
	*be = bcol+step/2;
	//feature->r_e[idx] = rcol;
	//feature->g_e[idx] = gcol;
	//feature->b_e[idx] = bcol;

	//histogram r g b
	//float rp[256];
	//float gp[256];
	//float bp[256];
	//float area = pixs;
	//int i;
	//for (i = 0; i < 256; ++ i)
	//{
	//	rp[i] = (float)rhist[i] / area;
	//	gp[i] = (float)ghist[i] / area;
	//	bp[i] = (float)bhist[i] / area;
	//}
	////expect
	//float re = 0;
	//float ge = 0;
	//float be = 0;
	//for (i = 0; i < 256; ++ i)
	//{
	//	re += i * rp[i];
	//	ge += i * gp[i];
	//	be += i * bp[i];
	//}
	//feature->r_e[idx] = re;
	//feature->g_e[idx] = ge;
	//feature->b_e[idx] = be;
	////variance
	//float rv = 0;
	//float gv = 0;
	//float bv = 0;
	//for (i = 0; i < 256; ++ i)
	//{
	//	if( rp[i] != 0 )
	//	{
	//		rv += abs(i - re) * rp[i];
	//		gv += abs(i - ge) * gp[i];
	//		bv += abs(i - be) * bp[i];
	//	}
	//}
	//
	//feature->r_v[idx] = rv;
	//feature->g_v[idx] = gv;
	//feature->b_v[idx] = bv;

}

//void _mae_get_image_h_e_v(MAE_image *h_image, MAE_image_feature *feature, int idx)
void _mae_get_image_h_e_v(MAE_image *h_image, int *he)
{
	unsigned char * h_data;
	unsigned char * s_data;
	unsigned char * v_data;
	int depth;
	int width;
	int height;
	long widthbytes;

	long hhist[256];
	long pixs;
	depth = h_image->depth;
	width = h_image->w;
	height = h_image->h;
	widthbytes = _mae_width_bytes(depth, width);

	h_data = _mae_mallocate_image(width,height,8);
	s_data = _mae_mallocate_image(width,height,8);
	v_data = _mae_mallocate_image(width,height,8);

	//convert rgb to hsi from high colour image
	_mae_convert_rgb2hsv(h_data, s_data, v_data, h_image);

	//#ifdef MAE_DEBUG
	//MAE_image test_image;
	//_mae_save_gray_image(&test_image, h_data, width, height, h_image);
	//_mae_save_image("test_h_gray_2.bmp", &test_image);
	//free(test_image.data);
	//#endif

	_mae_histogram_mark(h_data, width, height, hhist, &pixs);

	//float mark_pixs_radio = (float)pixs /(float)( width * height);
	if( pixs > 10 )
	{
		int i;
		int htemp = 0;
		int hcol = 0;
		int k;
		int sum;
		int step = 8;
		//for(i = 0; i < 256; i ++ )
		for(i = 0; i < 256-step; i ++ )
		{
			sum = 0;
			for(k = 0; k < step; k ++ )
			{
				sum = sum + hhist[i+k];
			}
			if( htemp < sum )
			{
				htemp = sum;
				hcol = i;
			}
		}
		//feature->h_e[idx] = hcol;
		*he = hcol+step/2;
	}else{
		*he = 0;
	}
	//histogram r g b
	//float hp[256];
	//float area = pixs;
	//int i;
	//for (i = 0; i < 256; ++ i)
	//{
	//	hp[i] = (float)hhist[i] / area;
	//}
	////expect
	//float he = 0;
	//for (i = 0; i < 256; ++ i)
	//{
	//	he += i * hp[i];
	//}
	//feature->h_e[idx] = he;
	////variance
	//float hv = 0;
	//for (i = 0; i < 256; ++ i)
	//{
	//	if( hp[i] != 0 )
	//	{
	//		hv += abs(i - he) * hp[i];
	//	}
	//}
	//
	//feature->h_v[idx] = hv;

	free(h_data);
	free(s_data);
	free(v_data);
}
void _mae_check_region_on_feature1(MAE_region* region, MAE_image_feature *feature, MAE_init_feature * init_feature)
{
	//check region based on fillredio and center mark pixs

	long mk_pix_num = feature->mk_pix_num;
	//long mt_pix_num = feature->mt_pix_num;
	long area = feature->area;
	float center_mark = 0;
	float fillredio;
	float w_ratio;

	//mark line width
	w_ratio = (float)mk_pix_num / (float)((feature->width + feature->height) * 2);

	fillredio = (float)mk_pix_num / (float)area;
	//ratio = (float)mt_pix_num / (float)mk_pix_num;
	if( fillredio < MAE_REGION_FILL_MAX_RATIO ) 
	{
		//rect region
		//20100901 add for break mark lines
		if( init_feature->flag )
		{
			center_mark = (float)feature->center_mark_pixs / (float)feature->center_area;
			if( center_mark > MAE_CENTER_MARK_RADIO )
			{
				region->label = MAE_MARK_REGION_NO;
				region->type = MAE_REGION_TYPE_PICT;
				//feature->label = MAE_MARK_REGION_NO;
				return;
			}

			if( w_ratio < MAE_MIN_LINE_WIDTH ) 
			{
				region->label = MAE_MARK_REGION_NO;
				//feature->label = MAE_MARK_REGION_NO;
				region->type = MAE_REGION_TYPE_PICT;
				return;
			}
		}else{
			region->label = MAE_MARK_REGION_RECHECK;
			//feature->label = MAE_MARK_REGION_RECHECK;
			region->type = MAE_REGION_TYPE_RECT;

			float subfill[4];
			//up line
			subfill[0] = (float)feature->submpixs[1] / (float)feature->mark_pixs;
			//left line
			subfill[1] = (float)feature->submpixs[3] / (float)feature->mark_pixs;
			//right line
			subfill[2] = (float)feature->submpixs[5] / (float)feature->mark_pixs;
			//bottom line
			subfill[3] = (float)feature->submpixs[7] / (float)feature->mark_pixs;
			if( subfill[0] < MAE_SUBREGION_FILL_RATIO )
			{
				region->type |= MAE_REGION_TYPE_RECT_NT;
			}
			if( subfill[1] < MAE_SUBREGION_FILL_RATIO )
			{
				region->type |= MAE_REGION_TYPE_RECT_NL;
			}
			if( subfill[2] < MAE_SUBREGION_FILL_RATIO )
			{
				region->type |= MAE_REGION_TYPE_RECT_NR;
			}
			if( subfill[3] < MAE_SUBREGION_FILL_RATIO )
			{
				region->type |= MAE_REGION_TYPE_RECT_NB;
			}
		}

	}
	return;
}

//void _mae_check_region_on_feature2(MAE_region* region, MAE_image_feature *feature, MAE_init_feature * init_feature)
//void _mae_check_region_on_feature2(MAE_region* region, MAE_image_feature *feature)
void _mae_check_region_on_feature2(MAE_region* region, MAE_image_feature *feature, MAE_init_feature * init_feature)
{


	long mk_pix_num = feature->mk_pix_num;
	//long mt_pix_num = feature->mt_pix_num;
	long area = feature->area;
	//	float center_mark = 0;
	float fillredio;
	float contour_radio;

	contour_radio  = (float)feature->contour_pixs / (float)((feature->width + feature->height ) * 2) ;

	fillredio = (float)mk_pix_num / (float)area;
	if(init_feature->checkflg == 1 )
	{
		if( fillredio > MAE_REGION_FILL_MAX_RATIO ) 
		{
			//mark fill whole region 
			//check region based on text size
			if (   feature->max_connect_w > feature->width / 2 
				&& feature->max_connect_h > feature->height / 2 )
			{
				region->label = MAE_MARK_REGION_NO;
				region->type = MAE_REGION_TYPE_PICT;
				//feature->label = MAE_MARK_REGION_NO;
				return;
			}
			if( contour_radio > 0.9)
			{
				region->label = MAE_MARK_REGION_NO;
				//feature->label = MAE_MARK_REGION_NO;
				region->type = MAE_REGION_TYPE_PICT;
				return;
			}

			//int deltx = abs(feature->tw - init_feature->t_width);
			//int delty = abs(feature->th - init_feature->t_height);
			//
			//if( deltx > 5 || delty > 5)
			//{
			//	region->label = -1;
			//	return;
			//}
		}
	}
	return;
}

//void _mae_check_region_on_feature3(MAE_region* region, MAE_image_feature *feature, MAE_init_feature * init_feature)
//void _mae_check_region_on_feature3(MAE_region* region, MAE_image_feature *feature)
void _mae_check_region_on_feature3(MAE_region* region, MAE_image_feature *feature, MAE_init_feature * init_feature)
{

	float re;
	float ge;
	float be;
	float he;
	float sa;
	re = feature->max_re - feature->min_re;
	ge = feature->max_ge - feature->min_ge;
	be = feature->max_be - feature->min_be;
	he = feature->max_he - feature->min_he;
	sa = re + ge + be;
	long mk_pix_num = feature->mk_pix_num;
	//long mt_pix_num = feature->mt_pix_num;
	long area = feature->area;
	//	float center_mark = 0;
	float fillredio;

	fillredio = (float)mk_pix_num / (float)area;
	if(	init_feature->checkflg == 0 )
	{
		if( fillredio > MAE_REGION_FILL_MAX_RATIO2 ) 
		{
			region->label = MAE_MARK_REGION_NO;
			region->type = MAE_REGION_TYPE_PICT;
			return;
		}
	}else{
		if( fillredio > MAE_REGION_FILL_MAX_RATIO ) 
		{
			//mark fill whole region 
			if(he > MAE_H_VARIANCE ) 
				//if( re > MAE_RGB_VARIANCE  || ge > MAE_RGB_VARIANCE || be > MAE_RGB_VARIANCE )
				//if( sa > MAE_RGB_VARIANCE2)
			{
				region->label = MAE_MARK_REGION_NO;
				//feature->label = region->label;
				region->type = MAE_REGION_TYPE_PICT;
				return;
			}
			region->type = MAE_REGION_TYPE_MARK;

		}else{
			region->label = MAE_MARK_REGION_NO;
		}
	}
	//////slender long line 
	//float slender = 0;
	//float center_mark = 0;
	//int min = 0;
	//if( region->w > region->h)
	//{
	//	slender = (float)region->h / (float)region->w;
	//	min = region->h;
	//}else{
	//	slender = (float)region->w / (float)region->h;
	//	min = region->w;
	//}
	//if( slender < MAE_LINE_SLENDER_DEGREE && min < MAE_NORMAL_LINE_WIDTH)
	//{
	//	//center_mark = (float)feature->center_mark_pixs / (float)feature->center_area;
	//	//if(  center_mark < MAE_SLENDER_CENTER_MARK_RADIO )
	//	//{
	//	//	region->label = -1;
	//	//}
	//}else{
	//	center_mark = (float)feature->center_mark_pixs / (float)feature->center_area;
	//	if( center_mark > MAE_CENTER_MARK_RADIO )
	//	{
	//		region->label = -1;
	//	}
	//}

	return;
}
//
//void _mae_check_region_on_feature4(MAE_region* region, MAE_image_feature *feature, MAE_init_feature * init_feature)
//{
//
//	//if( fillredio > MAE_REGION_FILL_MAX_RATIO ) 
//	//{
//	//	;
//	//}else if ( fillredio > MAE_REGION_FILL_MIN_RATIO && fillredio < MAE_REGION_FILL_MAX_RATIO ) 
//	//{
//	//	// mark region or image
//	//	//if( ratio < MAE_MARK_MIN_TEXT_RATIO || ratio > MAE_MARK_MAX_TEXT_RATIO )
//	//	//{
//	//	//	region->label = -1;
//	//	//	return;
//	//	//}
//	//	region->label = -1;
//	//	return;
//	//}else{
//	//	//rect region
//	//	center_mark = (float)feature->center_mark_pixs / (float)feature->center_area;
//	//	if( center_mark > MAE_CENTER_MARK_RADIO )
//	//	{
//	//		region->label = -1;
//	//		return;
//	//	}
//	//}
//
//
//	//float re;
//	//float ge;
//	//float be;
//	//float he;
//	//re = feature->max_re - feature->min_re;
//	//ge = feature->max_ge - feature->min_ge;
//	//be = feature->max_be - feature->min_be;
//	//he = feature->max_he - feature->min_he;
//	//if(re > MAE_RGB_VARIANCE || ge > MAE_RGB_VARIANCE || be > MAE_RGB_VARIANCE ) 
//	//{
//	//	region->label = -1;
//	//	return;
//	//}
//
//	//if(hv > MAE_H_VARIANCE ) 
//	//{
//	//	region->label = -1;
//	//	return;
//	//}
//	//////slender long line 
//	//float slender = 0;
//	//float center_mark = 0;
//	//int min = 0;
//	//if( region->w > region->h)
//	//{
//	//	slender = (float)region->h / (float)region->w;
//	//	min = region->h;
//	//}else{
//	//	slender = (float)region->w / (float)region->h;
//	//	min = region->w;
//	//}
//	//if( slender < MAE_LINE_SLENDER_DEGREE && min < MAE_NORMAL_LINE_WIDTH)
//	//{
//	//	//center_mark = (float)feature->center_mark_pixs / (float)feature->center_area;
//	//	//if(  center_mark < MAE_SLENDER_CENTER_MARK_RADIO )
//	//	//{
//	//	//	region->label = -1;
//	//	//}
//	//}else{
//	//	center_mark = (float)feature->center_mark_pixs / (float)feature->center_area;
//	//	if( center_mark > MAE_CENTER_MARK_RADIO )
//	//	{
//	//		region->label = -1;
//	//	}
//	//}
//
//	return;
//}



void _mae_histogram_rect(MAE_block *_mae_blocks, int _nblocks, int* hist, int gridx, int gridy)
{
	int i;
	int w;
	int h;

	for(i = 0; i < gridx * gridy; i ++ )
	{
		hist[i] = 0;
	}	

	for(i = 0; i < _nblocks; i ++ )
	{
		if( _mae_blocks[i].label == -1 ) 
		{
			continue;
		}
		w = _mae_blocks[i].w;
		h = _mae_blocks[i].h;
		w = int((float)w / 5 + 0.5 );
		h = int((float)h / 5 + 0.5 );
		hist[h * gridx + w ] ++;
	}
	return;
}

void _mae_parse_rect_hist(int* hist, int gridx, int gridy, MAE_image_feature *feature)
{
	int i;
	int j;
	int w;
	int h;
	int count = 0;
	w = 0;
	h = 0;
	for(j = 1; j < gridy; j ++ )
	{
		for(i = 1; i < gridx; i ++ )
		{
			if( count <= hist[j * gridx + i ] )
			{
				count = hist[j * gridx + i];
				w = (i - 1 )  * 5;
				h = (j - 1 ) * 5;
			}
		}	
	}
	feature->tw = w;
	feature->th = h;
	feature->tcount = count;
	return;
}

void _mae_max_connect_size(MAE_block *_mae_blocks, int _nblocks, MAE_image_feature *feature)
{
	int i;
	int w;
	int h;
	w = 0;
	h = 0;
	for(i = 0; i < _nblocks; i ++ )
	{
		if( _mae_blocks[i].label == -1 )
		{
			continue;
		}
		if( w < _mae_blocks[i].w )
		{
			w = _mae_blocks[i].w;
		}
		if( h < _mae_blocks[i].h )
		{
			h = _mae_blocks[i].h;
		}

	}
	feature->max_connect_w = w;
	feature->max_connect_h = h;

	return;
}

int _mae_detect_text_region_all(MAE_image *in_image, MAE_region * regions, int nregions)
{
	int error = MAE_NO_ERROR;
	int i;
	for(i = 0; i < nregions; i ++ )
	{
		if( regions[i].label == MAE_MARK_REGION_NO ) {
			continue;
		}
		_mae_detect_text_region(in_image, &regions[i]);
	}
	return error;
}

void _mae_detect_text_region(MAE_image *in_image, MAE_region* region)
{
	MAE_image tmpimage;
	unsigned char * gdata;
	unsigned char * bdata;
	int width;
	int height;
	int x,y,w,h;
	//int x1,y1,w1,h1;
	//int x2,y2,w2,h2;
	int *hists;
	int len;
	//int max1;
	//int min1;
	//int max2;
	//int min2;
	int k;
	float radio;
	int count;
	//FILE *fp;
	int threshold;
	threshold = 2;

	count = 0;

	_mae_get_region_image(in_image, region->l, region->t, region->w, region->h, &tmpimage );

	width = tmpimage.w;
	height = tmpimage.h;
	gdata = _mae_mallocate_image(tmpimage.w, tmpimage.h, 8);
	bdata = _mae_mallocate_image(tmpimage.w, tmpimage.h, 8);
	_mae_convert_rgb2gray2(gdata, &tmpimage);


	_mae_segment_grayforchecktext(gdata, tmpimage.w, tmpimage.h, bdata);

#ifdef MAE_DEBUG
	MAE_image test_image;
	_mae_save_gray_image(&test_image, gdata, tmpimage.w, tmpimage.h, &tmpimage);
	_mae_save_image("test_region_gray.bmp", &test_image);
	free(test_image.data);

	_mae_save_segment_image(&test_image, bdata, tmpimage.w, tmpimage.h, &tmpimage);
	_mae_save_image("test_region_segment.bmp", &test_image);
	free(test_image.data);
#endif

	if( tmpimage.w > 2*tmpimage.h )
	{
		x = 1;
		y = 1;
		w = tmpimage.w - 2;
		h = tmpimage.h - 2;
		hists = NULL;
		len = 0;
		_mae_count_v_rect_hist(bdata, width, height,
			x,  y, w, h,
			&hists, &len);
		/////////////////
		//if((fp=fopen("test_hist.txt","w")) == NULL)
		//{
		//	;
		//}
		//for( k = 0; k < len; k ++ )
		//{
		//	fprintf(fp,"%d, ", hists[k]);
		//}
		//fclose(fp);
		/////////////

		threshold = min(3, tmpimage.h/4);

		for( k = 0; k < len; k ++ )
		{
			if( hists[k] < threshold )
			{
				hists[k] = 0;
			}
		}


		for( k = 0; k < len - 1; k ++ )
		{
			if( hists[k] == 0 && hists[k+1] > 0 )
			{
				count++;
			}
		}
		if( count > threshold ) 
		{
			region->label = -1;
		}
		free(hists);

	}else if( tmpimage.h > 2*tmpimage.w ){
		x = 1;
		y = 1;
		w = tmpimage.w - 2;
		h = tmpimage.h - 2;

		hists = NULL;
		len = 0;
		_mae_count_h_rect_hist(bdata, width, height,
			x,  y, w, h,
			&hists, &len);

		threshold = min(3, tmpimage.w/4);
		for( k = 0; k < len; k ++ )
		{
			if( hists[k] < threshold )
			{
				hists[k] = 0;
			}
		}

		for( k = 0; k < len - 1; k ++ )
		{
			if( hists[k] == 0 && hists[k+1] > 0 )
			{
				count++;
			}
		}
		if( count >threshold ) 
		{
			region->label = -1;
		}

		free(hists);

	}
	free(gdata);
	free(bdata);
	//if( region->w > 2*region->h )
	//{
	//	x = region->l - 3;
	//	y = region->t;
	//	w = region->w - 6;
	//	h = region->h;
	//	if( x < 0) x = 0;
	//	if( y < 0 ) y = 0;
	//	if(w < MAE_SMALL_LINE_WIDTH)
	//	{
	//		region->label = -1;
	//		return;
	//	}

	//	x1 = x;
	//	y1 = y;
	//	w1 = w / 2;
	//	h1 = h;

	//	hists = NULL;
	//	len = 0;
	//	 _mae_count_v_rect_hist(data, width, height,
	//						    x1,  y1, w1, h1,
	//				            &hists, &len);
	//	min1 = h1;
	//	max1 = 0;
	//	for( k = 0; k < len; k ++ )
	//	{	
	//		if( max1 < hists[k] )
	//		{
	//			max1 = hists[k];
	//		}
	//		if( min1 > hists[k] )
	//		{
	//			min1 = hists[k];
	//		}
	//	}
	//	free(hists);


	//	x2 = x + w / 2;
	//	y2 = y;
	//	w2 = w / 2;
	//	h2 = h;

	//	hists = NULL;
	//	len = 0;
	//	 _mae_count_v_rect_hist(data, width, height,
	//						    x2,  y2, w2, h2,
	//				            &hists, &len);
	//	min2 = h2;
	//	max2 = 0;
	//	for( k = 0; k < len; k ++ )
	//	{	
	//		if( max2 < hists[k] )
	//		{
	//			max2 = hists[k];
	//		}
	//		if( min2 > hists[k] )
	//		{
	//			min2 = hists[k];
	//		}
	//	}
	//	free(hists);

	//	radio = ((float)min1 / (float)max1  + (float)min2 / (float)max2) / 2;
	//	if(radio < 0.5 ) 
	//	{
	//		region->label = -1;
	//	}
	//}else if( region->h > 2*region->w ){
	//	x = region->l;
	//	y = region->t - 3;
	//	w = region->w;
	//	h = region->h - 6;
	//	if( x < 0) x = 0;
	//	if( y < 0 ) y = 0;
	//	if(h < MAE_SMALL_LINE_HEIGHT )
	//	{
	//		region->label = -1;
	//		return;
	//	}

	//	x1 = x;
	//	y1 = y;
	//	w1 = w ;
	//	h1 = h / 2;

	//	hists = NULL;
	//	len = 0;
	//	 _mae_count_h_rect_hist(data, width, height,
	//						    x1,  y1, w1, h1,
	//				            &hists, &len);
	//	min1 = w1;
	//	max1 = 0;
	//	for( k = 0; k < len; k ++ )
	//	{	
	//		if( max1 < hists[k] )
	//		{
	//			max1 = hists[k];
	//		}
	//		if( min1 > hists[k] )
	//		{
	//			min1 = hists[k];
	//		}
	//	}
	//	free(hists);


	//	x2 = x;
	//	y2 = y + h / 2;
	//	w2 = w;
	//	h2 = h / 2;

	//	hists = NULL;
	//	len = 0;
	//	 _mae_count_h_rect_hist(data, width, height,
	//						    x2,  y2, w2, h2,
	//				            &hists, &len);
	//	min2 = w2;
	//	max2 = 0;
	//	for( k = 0; k < len; k ++ )
	//	{	
	//		if( max2 < hists[k] )
	//		{
	//			max2 = hists[k];
	//		}
	//		if( min2 > hists[k] )
	//		{
	//			min2 = hists[k];
	//		}
	//	}
	//	free(hists);

	//	radio = ((float)min1 / (float)max1  + (float)min2 / (float)max2) / 2;
	//	if(radio < 0.5 ) 
	//	{
	//		region->label = -1;
	//	}
	//}

	return;
}
