#include "stdafx.h"
#include <stdlib.h>
#include "regions.h"
#include "memalloc.h"
#include "imageutil.h"
#include "runs.h"
#include "runsprocess.h"
#include "histogram.h"
#include "blocks.h"
#include "chain.h"
#include "imageprocess.h"
#include "segment.h"
#include "detectcolorregion.h"
#include "detectmark.h"
#include "linkregion.h"

__declspec(dllexport) int mae_open_image(const char* file_name, MAE_image *mae_image)
{
	//char szDir[1024];
	//char szDrive[3];
	//char szFName[1024];
	//char szExt[256];
	//// split file fath
	//_splitpath(file_name,szDrive,szDir,szFName,szExt);
	//MAE_image open_image;
	int error;
	//
	//#ifdef MAE_USE_GDIPLUS
	//
	//	ULONG_PTR gdiplusToken;
	//	GdiplusStartupInput gdiplusStartupInput; 
	//	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL); 
	//
	//	WCHAR w_file_name[1024];
	//	memset(w_file_name, 0, 1024);
	//	//LPCSTR w_file_name;
	//	long  len = strlen( file_name ) + 1;
	// //     
	//    int wLen = MultiByteToWideChar( 
	//		CP_ACP, 0, file_name, len, 
	//		w_file_name,  len);
	//
	//	Bitmap *bitmap = Bitmap::FromFile(w_file_name);
	//	//Bitmap bitmap = Bitmap(w_file_name);
	//	int width = bitmap->GetWidth();
	//	int height = bitmap->GetHeight();
	//	Rect rect;
	//	rect.X = 0;
	//	rect.Y = 0;
	//	rect.Width = width;
	//	rect.Height = height;
	//	
	//	BitmapData bmpData;
	//	bitmap->LockBits(&rect,ImageLockModeRead, PixelFormat24bppRGB,&bmpData);
	//	unsigned char* pBuf = (unsigned char*)bmpData.Scan0;
	//	error = _mae_open_image_bitmap( pBuf, width, height, mae_image);
	//	bitmap->UnlockBits(&bmpData);
	//	delete(bitmap);
	//	GdiplusShutdown(gdiplusToken); 
	//
	//#else
	error = _mae_open_image(file_name, mae_image);
	//#endif
	return error;
}

//__declspec(dllexport) int mae_detect_marker_regions( MAE_image* open_mae_image, MAE_region **mae_regions, int *nregions)
__declspec(dllexport) int mae_detect_marker_regions( MAE_image* open_mae_image, MAE_region **mae_regions, int *nregions, MAE_line_node **lpthead)
{
	MAE_region *regions1;
	int region_num1;
	MAE_region *regions2;
	int region_num2;
	//MAE_line_node * cornerhead;
	MAE_region *regions;
	int region_num;
	int i,j,idx;
	unsigned char *region_data;

	unsigned char * i_data;
	unsigned char * out_data;
	unsigned char * bi_data;
	unsigned char * rdata;

	MAE_image high_color_mae_image;
	MAE_image zoom_image;

	int *h_rx, *h_ry, *h_rlen, h_nruns, h_ralloc;
	int *h_rx2, *h_ry2, *h_rlen2, h_nruns2, h_ralloc2;
	int *v_rx, *v_ry, *v_rlen, v_nruns, v_ralloc;


	//MAE_init_feature init_feature;

	//MAE_line  *linelist;
	//int  linenum;
	int o_width;
	int o_height;

	int width;
	int height;
	int depth;

	float xfctr = 0;
	float yfctr = 0;

	int h_line_width;
	int v_line_width;


	o_width = open_mae_image->w;
	o_height = open_mae_image->h;

	_mae_zoom(open_mae_image, &zoom_image, &xfctr, &yfctr);


	width = zoom_image.w;
	height = zoom_image.h;
	depth = zoom_image.depth;



	//detect high color 
	//彩色检测，执行完成后区分彩色部分和文字
	_mae_detect_high_color(&zoom_image, &high_color_mae_image, RGB_DETECT_CR_VARIENCE);

#ifdef MAE_DEBUG
	_mae_save_image("test_hight_color_image.jpg",&high_color_mae_image);
#endif

	i_data = _mae_mallocate_image(width,height,8);
	out_data = _mae_mallocate_image(width,height,8);
	bi_data = _mae_mallocate_image(width,height,8);


	//二值化图像
	_mae_segmention_from_high_color(&high_color_mae_image, out_data);

	//
	_mae_convert_rgb2gray(i_data, &zoom_image, &high_color_mae_image);

	//阈值的图像分割
	_mae_segment_gray(i_data, width, height, bi_data);
	regions1 = NULL;
	region_num1 = 0;
	regions2 = NULL;
	region_num2 = 0;
	regions = NULL;
	region_num = 0;
	region_data = NULL;


	//20110107 add for image dilation end
	rdata = _mae_mallocate_image(height, width, 8);

	//图像旋转90度
	_mae_grayscale_rotate_90(out_data, rdata, width, height);

	//20101225 ADD

	//20110107 add for image dilation start

	_mae_v_dilation(out_data, width, height);
	//_mae_split_image(out_data, width, height, MAE_SPLIT_IMAGE_STEP);
	_mae_get_runs(&h_rx, &h_ry, &h_rlen, &h_nruns, &h_ralloc, out_data, width, height);

	_mae_v_dilation(rdata, height, width);
	//_mae_split_image(rdata, height, width, MAE_SPLIT_IMAGE_STEP);
	_mae_get_runs(&v_rx, &v_ry, &v_rlen, &v_nruns, &v_ralloc, rdata, height, width);
	free(rdata);



	//backup hori runs
	h_nruns2 = h_nruns;
	_mae_malloc_int(&h_rx2, h_nruns2, "mae_detect_marker_regions : rx");
	_mae_malloc_int(&h_ry2, h_nruns2, "mae_detect_marker_regions : ry");
	_mae_malloc_int(&h_rlen2, h_nruns2, "mae_detect_marker_regions : rlen");
	memcpy(h_rx2, h_rx, h_nruns*sizeof(int));
	memcpy(h_ry2, h_ry, h_nruns*sizeof(int));
	memcpy(h_rlen2, h_rlen, h_nruns*sizeof(int));
	//
#ifdef MAE_DEBUG
	MAE_image runs_image;
	runs_image.w = width;
	runs_image.h = height;
	runs_image.type = 1;
	runs_image.depth  = 24;
	runs_image.stride = _mae_width_bytes(24,runs_image.w );
	runs_image.data  = _mae_mallocate_image(runs_image.w,runs_image.h,24);
	_mae_save_h_runs_image(&runs_image, h_rx2, h_ry2, h_rlen2, h_nruns2 );
	_mae_save_image("test_hori_run_to_image.jpg",&runs_image);
	free(runs_image.data);
#endif
	//calculate line width
	//h_line_width = 0;
	//v_line_width = 0;
	//_mae_com_line_width(width, height, h_rx, h_ry, h_rlen, h_nruns, v_rx, v_ry, v_rlen, v_nruns, &h_line_width, &v_line_width);
	h_line_width = MAE_MARK_LINE_WIDTH;
	v_line_width = MAE_MARK_LINE_WIDTH;

	//mae_detect_marker_line_regions( bi_data, out_data, width, height, &regions1, &region_num1, lpthead);
	mae_detect_marker_line_regions(
		&zoom_image, &high_color_mae_image,
		bi_data, out_data, width, height, 
		h_rx, h_ry, h_rlen, h_nruns,
		v_rx, v_ry, v_rlen, v_nruns,
		h_line_width, v_line_width,
		&regions1, &region_num1, lpthead);


	mae_detect_marker_solid_regions( &zoom_image, &high_color_mae_image, bi_data, out_data, width, height, 
		h_rx2, h_ry2, h_rlen2, h_nruns2,
		&regions2, &region_num2);

	//20110118 add for check repeat regions
	_mae_check_s_region_and_l_region(&regions2, &region_num2, &regions1, &region_num1);


	region_num = region_num1 + region_num2;

	_mae_malloc_uchar(&region_data, region_num * sizeof(MAE_region),"mae_detect_marker_regions : regions");
	regions = (MAE_region *)region_data;

	MAE_region *region1;
	idx = 0;
	for( i = 0; i < region_num1; i++ )
	{
		regions[idx].label = regions1[i].label;
		regions[idx].type = regions1[i].type;
		regions[idx].l = regions1[i].l;
		regions[idx].t = regions1[i].t;
		regions[idx].w = regions1[i].w;
		regions[idx].h = regions1[i].h;
		regions[idx].cornernum  = regions1[i].cornernum;
		regions[idx].corners   = regions1[i].corners;
		regions[idx].linenum    = regions1[i].linenum;
		for(j =0; j < regions[idx].linenum; j ++ )
		{
			regions[idx].lines[j].type  = regions1[i].lines[j].type;
			regions[idx].lines[j].sx  = regions1[i].lines[j].sx;
			regions[idx].lines[j].sy  = regions1[i].lines[j].sy;
			regions[idx].lines[j].ex  = regions1[i].lines[j].ex;
			regions[idx].lines[j].ey  = regions1[i].lines[j].ey;
		}
		regions[idx].head   = NULL;
		regions[idx].link  = NULL;
		idx++;
	}
	for( i = 0; i < region_num2; i++ )
	{
		regions[idx].label = regions2[i].label;
		regions[idx].type = regions2[i].type;
		regions[idx].l = regions2[i].l;
		regions[idx].t = regions2[i].t;
		regions[idx].w = regions2[i].w;
		regions[idx].h = regions2[i].h;
		regions[idx].cornernum  = regions2[i].cornernum;
		regions[idx].corners   = regions2[i].corners;
		regions[idx].linenum    = regions2[i].linenum;
		regions[idx].head   = NULL;
		regions[idx].link  = NULL;
		idx++;
	}

	if(regions1 != NULL )	free(regions1);
	if(regions2 != NULL )	free(regions2);

	*mae_regions = regions;
	*nregions = region_num;

	_mae_adjust_region_size_by_fctr(mae_regions, nregions, xfctr, yfctr,o_width,o_height);
	_mae_adjust_point_size_by_fctr(lpthead, xfctr, yfctr,o_width,o_height);

	//*lpthead = cornerhead;
	if( h_nruns > 0 )
	{
		free(h_rx);
		free(h_ry);
		free(h_rlen);

		free(h_rx2);
		free(h_ry2);
		free(h_rlen2);
	}
	if( v_nruns > 0 )
	{
		free(v_rx);
		free(v_ry);
		free(v_rlen);
	}

	free(i_data);
	free(out_data);
	free(bi_data);
	free(zoom_image.data);
	free(high_color_mae_image.data);
	return 0;
}

__declspec(dllexport) int mae_get_marker_image(MAE_image* src, MAE_region *mae_regions, int nregions, MAE_image* mask, int flag)
{
	//return _mae_save_mark_image(src, mae_regions, nregions,mask, flag);
#ifdef MAE_TEST_LINE_CORNER	
	return _mae_save_mark_line_image(src, mae_regions, nregions,mask, flag);
#else
	//return _mae_save_mark_image_from_corners(src, mae_regions, nregions,mask, flag);
	return _mae_save_mark_image_from_corners2(src, mae_regions, nregions,mask, flag);
#endif
}

__declspec(dllexport) int mae_show_point_image(MAE_image* dst, MAE_line_node *lpthead)
{
	//return _mae_save_mark_image(src, mae_regions, nregions,mask, flag);
	return _mae_save_point_image(dst, lpthead);
	//return 0;

}

__declspec(dllexport) int mae_save_image(const char * file_name, const MAE_image *src)
{
	int error = MAE_NO_ERROR;
	//
	//#ifdef MAE_USE_GDIPLUS
	//
	//	ULONG_PTR gdiplusToken;
	//	GdiplusStartupInput gdiplusStartupInput; 
	//	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL); 
	//
	//	WCHAR w_file_name[1024];
	//	memset(w_file_name, 0, 1024);
	//	long  len = strlen( file_name ) + 1;
	//    int wLen = MultiByteToWideChar( 
	//		CP_ACP, 0, file_name, len, 
	//		w_file_name,  len);
	//
	//	Bitmap *  bitmap=new Bitmap(src->w,src->h,PixelFormat24bppRGB);
	//
	//	Rect rect;
	//	rect.X = 0;
	//	rect.Y = 0;
	//	rect.Width = src->w;
	//	rect.Height = src->h;
	//	BitmapData bmpData;
	//	bitmap->LockBits(&rect,ImageLockModeRead, PixelFormat24bppRGB,&bmpData);
	//
	//	unsigned char* pBuf = (unsigned char*)bmpData.Scan0;
	//	
	//	memcpy(pBuf,src->data, src->h * src->stride );
	//
	//	CLSID clsid;
	//	if( GetEncoderClsid(L"image/jpeg", &clsid) == -1 )
	//	{
	//		error = MAE_SAVE_IMAGE_FAIL;
	//		return error;
	//	}
	//
	//	//LONG lQuality = 80;
	//	//EncoderParameters params; 
	//	//params.Parameter[0].Guid = EncoderQuality;
	//	//params.Parameter[0].NumberOfValues = 1;
	//	//params.Parameter[0].Type = EncoderParameterValueTypeLong;
	//	//params.Parameter[0].Value = (VOID*) &lQuality;
	//	//params.Count = 1;
	//
	//	bitmap->Save(w_file_name, &clsid, NULL );
	//	delete bitmap;
	//	GdiplusShutdown(gdiplusToken); 
	//#else
	error = _mae_save_image(file_name, src);
	//#endif
	return error;
}
__declspec(dllexport) int mae_remove_mark_color(MAE_image *io_image, MAE_region * regions, int nregions)
{
	return _mae_remove_mark_color(io_image, regions, nregions);
}
__declspec(dllexport) void mae_destroy_regions(MAE_region* regions, int nregions)
{
	_mae_destroy_regions(regions, nregions);
}
__declspec(dllexport) void mae_destroy_image(MAE_image *mae_image)
{
	free(mae_image->data);
}
__declspec(dllexport)  void mae_destroy_line_node(MAE_line_node *head)
{
	MAE_line_node *node = head;
	MAE_line_node *node_link = NULL;
	while( node != NULL )
	{
		node_link = node->next; 
		free(node);
		node = node_link;
	}
}

int mae_detect_marker_line_regions(
								   MAE_image *in_image, MAE_image *h_image,
								   unsigned char *bi_data,unsigned char * data, int width, int height,
								   int * h_rx, int *h_ry, int *h_rlen, int h_runs,
								   int * v_rx, int *v_ry, int *v_rlen, int v_runs,
								   int h_line_width, int v_line_width,
								   MAE_region **mae_regions, int *nregions, MAE_line_node **lpthead)
{

	//	MAE_node * run_list;
	//    int listnum;
	int i;

	MAE_line  *v_lines;
	int  vlinenum;
	MAE_line  *h_lines;
	int  hlinenum;

	//MAE_node * h_run_list;
	//int  h_run_num;

	//MAE_node * v_run_list;
	//int  v_run_num;

	MAE_region *rghead = NULL;
	MAE_region *pregion, *qregion = NULL;

	int  lptnum = 0;
	int maxlinewidth;

	MAE_line  *linelist;
	int  linenum;

	//	width = zoom_image.w;
	//	height = zoom_image.h;
	//	depth = zoom_image.depth;

	MAE_init_feature init_feature;

	//h_run_list = NULL;
	//h_run_num = 0;

	//v_run_list = NULL;
	//v_run_num = 0;

	v_lines = NULL;
	vlinenum = 0;
	h_lines = NULL;
	hlinenum = 0;

	//_mae_detect_vertlines_blocks(bi_data, data, width, height, &v_lines, &vlinenum, &h_run_list, &h_run_num);
	_mae_detect_vertlines_blocks(bi_data, data, width, height,
		h_rx, h_ry, h_rlen, h_runs, v_line_width,
		&v_lines, &vlinenum);

	//_mae_detect_horilines_blocks(bi_data, data, width, height, &h_lines, &hlinenum, &v_run_list, &v_run_num);
	_mae_detect_horilines_blocks(bi_data, data, width, height,
		v_rx, v_ry, v_rlen, v_runs, h_line_width,
		&h_lines, &hlinenum);

	_mae_adjust_hori_lines(h_lines, hlinenum, width, height);

#ifdef MAE_DEBUG
	MAE_image testimage;
	_mae_save_mark_image_from_lines(in_image, h_lines, hlinenum, v_lines, vlinenum, &testimage);
	_mae_save_image("test_image_lines001.jpg", &testimage);

	_mae_save_mark_image_from_lines(in_image, h_lines, hlinenum, v_lines, 0, &testimage);
	_mae_save_image("test_image_lines001_1.jpg", &testimage);
	_mae_save_mark_image_from_lines(in_image, h_lines, 0, v_lines, vlinenum, &testimage);
	_mae_save_image("test_image_lines001_2.jpg", &testimage);
#endif   
	//chen add 20101119

#ifdef MAE_DEBUG
	//MAE_image testimage;
	_mae_save_segment_image(&testimage,data, width, height, h_image );
	_mae_save_image("test_segment_image_001.jpg", &testimage);
	free(testimage.data);
#endif   

	_mae_adujst_lines(data, width, height, &v_lines, &vlinenum, &h_lines, &hlinenum);

#ifdef MAE_DEBUG
	//	MAE_image testimage;
	_mae_save_mark_image_from_lines(in_image, h_lines, hlinenum, v_lines, vlinenum, &testimage);
	_mae_save_image("test_image_lines002.jpg", &testimage);

	_mae_save_mark_image_from_lines(in_image, h_lines, hlinenum, v_lines, 0, &testimage);
	_mae_save_image("test_image_lines002_1.jpg", &testimage);
	_mae_save_mark_image_from_lines(in_image, h_lines, 0, v_lines, vlinenum, &testimage);
	_mae_save_image("test_image_lines002_2.jpg", &testimage);
#endif   
	////20110111 add
	//原点选图像的左上角
	_mae_check_lines_rect(data, width, height, &v_lines, &vlinenum, &h_lines, &hlinenum);

	//remove small lines and irregular lines
	//	_mae_remove_small_liness(&h_lines, &hlinenum,  MAE_HORI_LINE);
	//	_mae_remove_small_liness(&v_lines, &vlinenum,  MAE_VERT_LINE);

	//MAE_line  *linelist;
	//int  linenum;
	linelist = NULL;
	linenum = 0;

	//20101225 update start
	//_mae_get_line_max_width(h_lines, hlinenum, v_lines, vlinenum, &maxlinewidth);
	maxlinewidth = 0;
	//20101225 update end

	//计算某线的中心线
	_mae_calculate_lines_v_h_center_line(h_lines, hlinenum, v_lines, vlinenum, maxlinewidth, width, height);


	//20110107 add  detect line color
	_mae_get_all_line_color(in_image, h_lines, hlinenum, v_lines, vlinenum);

	//20101226 add for near line add start
	//合并邻接举行
	_mae_check_vert_line_links(&v_lines, &vlinenum);
	_mae_check_hori_line_links(&h_lines, &hlinenum);
	//20101226 add for near line add end

	_mae_sort_hlines(h_lines, hlinenum);
	_mae_sort_vlines(v_lines, vlinenum);

	//	_mae_get_linelist_from_lines(h_lines, hlinenum, v_lines, vlinenum, &linelist, &linenum);
	//_mae_get_regions_from_lines(h_lines, hlinenum, v_lines, vlinenum, mae_regions, nregions);
	//	_mae_sort_lines(linelist, linenum);

	//	_mae_calculate_lines_center_line(&linelist, &linenum, maxlinewidth);

	//_mae_detect_line_corners(linelist, linenum, lpthead, &lptnum);
	//20101227 add

	//20110107 add  detect line color
	_mae_get_all_line_color(in_image, h_lines, hlinenum, v_lines, vlinenum);
#ifdef MAE_DEBUG
	//	MAE_image testimage;
	_mae_save_mark_image_from_lines(in_image, h_lines, hlinenum, v_lines, vlinenum, &testimage);
	_mae_save_image("test_image_lines003.jpg", &testimage);
#endif
	//取水平线和垂直线的交点
	_mae_detect_corners_from_lines(h_lines, hlinenum, v_lines, vlinenum,lpthead, &lptnum);

	//生成图的链接存储形式，有向的
	_mae_create_line_node_net(data, width, height, h_lines, hlinenum, v_lines, vlinenum,lpthead, &lptnum);
	//_mae_remove_alone_line_node(lpthead, &lptnum);

	//20110119 add start
	//_mae_check_corners_net(lpthead, lptnum);	
	//20110119 add end

	//将生成图的区域构成网络
	_mae_parse_region_form_corners_net(lpthead, lptnum, &rghead);	
	//
	//20101225 delete start
	//_mae_adjust_corners(lpthead, &lptnum, maxlinewidth);
	_mae_adjust_corners_orders(&rghead);
	//20101225 delete end

	//20110104 add start
	_mae_calculate_region_rects(&rghead);
	//20110104 add end

	//20110118 add for check 
	//	_mae_parse_region_form_corners_lines(lpthead, lptnum, linelist, linenum, &rghead);	
#ifdef MAE_TEST_LINE_CORNER	
	_mae_get_linelist_from_lines(h_lines, hlinenum, v_lines, vlinenum, &linelist, &linenum);
	_mae_get_regions_from_linelist(linelist, linenum, mae_regions, nregions);
#else
	_mae_get_regions_from_regionlist(rghead, mae_regions, nregions);
#endif	

	//20110117 add for delete small regions
	_mae_remove_small_regions3(mae_regions, nregions, width, height);
	//
	//20101225 add for testing start
#ifdef MAE_TEST_LINE_CORNER
	for(i = 0; i < *nregions; i ++ )
	{
		//if( (*mae_regions)[i].label == MAE_MARK_REGION_NO ) {
		//	continue;
		//}

		//temp_regions[idx].lines     = NULL;
		//(*mae_regions)[i].head   = NULL;
		//(*mae_regions)[i].link  = NULL;

		(*mae_regions)[i].cornernum  = 4;
		_mae_malloc_uchar( (unsigned char**)(&(*mae_regions)[i].corners), 4 * sizeof(MAE_corner),"mae_detect_marker_solid_regions : MAE_corner");

		(*mae_regions)[i].corners[0].x = (*mae_regions)[i].l;
		(*mae_regions)[i].corners[0].y = (*mae_regions)[i].t;

		(*mae_regions)[i].corners[1].x = (*mae_regions)[i].l + (*mae_regions)[i].w;
		(*mae_regions)[i].corners[1].y = (*mae_regions)[i].t;

		(*mae_regions)[i].corners[2].x = (*mae_regions)[i].l + (*mae_regions)[i].w;
		(*mae_regions)[i].corners[2].y = (*mae_regions)[i].t + (*mae_regions)[i].h;

		(*mae_regions)[i].corners[3].x = (*mae_regions)[i].l ;
		(*mae_regions)[i].corners[3].y = (*mae_regions)[i].t + (*mae_regions)[i].h;

	}
#endif
	//
	//20101225 testing end

	//	_mae_adjust_regions_corners(mae_regions, nregions);

	//	_mae_adjust_regions_corners2(mae_regions, nregions);

	//20110104 check regions start
	init_feature.width = width;
	init_feature.height = height;
	init_feature.r_bk = 240;
	init_feature.g_bk = 240;
	init_feature.b_bk = 240;
	init_feature.t_width = 0;
	init_feature.t_height = 0;
	init_feature.flag = 0;
	init_feature.checkflg = 0; //check line region

	_mae_init_image_feature(in_image, &init_feature);
	_mae_detect_mark_color_region_all(in_image, h_image, bi_data, *mae_regions, *nregions, &init_feature);
	//20110104 check regions end


	//if( h_run_list != NULL )
	//{
	//	free(h_run_list);
	//}
	//if( v_run_list != NULL )
	//{
	//	free(v_run_list);
	//}

	if(	v_lines != NULL )
	{
		delete v_lines;
	}
	if(	h_lines != NULL )
	{
		delete h_lines;
	}
	if( linelist != NULL )
	{
		free(linelist);
	}

	pregion = rghead;
	while(pregion != NULL )
	{
		qregion = pregion->link;
		//if(pregion->corners != NULL )
		//{
		//	free(pregion->corners);
		//}
		if(pregion != NULL )
		{
			free(pregion);
		}
		pregion = qregion;
	}

	return 0;
}
//int mae_detect_marker_solid_regions(
//			MAE_image *in_image, MAE_image *h_image,
//			unsigned char *bi_data,unsigned char * data, int width, int height,
//			MAE_region **mae_regions, int *nregions)
//将背景中的粗体也判断为框
int mae_detect_marker_solid_regions( 
									MAE_image *in_image, MAE_image *h_image,
									unsigned char *bi_data,unsigned char * data, int width, int height,
									int * h_rx, int *h_ry, int *h_rlen, int h_runs,
									MAE_region **mae_regions, int *nregions)
{
	//unsigned char * h_data;
	//unsigned char * s_data;

	//MAE_node * run_list;
	//int listnum;

	//MAE_image_feature *features;
	MAE_init_feature init_feature;
	//int  fnum;
	MAE_region *regions;
	int region_num;

	MAE_region *temp_regions; 
	int temp_region_num;
	int i, j;
	int idx;

	unsigned char * bdata;

	//20100823 add
	init_feature.width = width;
	init_feature.height = height;
	//_mae_detect_text_size(&zoom_image, &init_feature);
	init_feature.r_bk = 240;
	init_feature.g_bk = 240;
	init_feature.b_bk = 240;
	init_feature.t_width = 0;
	init_feature.t_height = 0;
	//20100823 add

	//detect all hight color regions include background color region
	//_mae_detect_region_blocks(bi_data, out_data, width, height, mae_regions, nregions);
	_mae_detect_region_blocks(bi_data, data, width, height,
		h_rx, h_ry, h_rlen, h_runs,
		&regions, &region_num);

#ifdef MAE_DEBUG
	MAE_image runs_image;
	runs_image.w = width;
	runs_image.h = height;
	runs_image.type = 1;
	runs_image.depth  = 24;
	runs_image.stride = _mae_width_bytes(24,width );
	runs_image.data  = _mae_mallocate_image(width,height,24);
	_mae_save_h_runs_image(&runs_image, h_rx, h_ry, h_rlen, h_runs );
	_mae_save_image("mae_detect_marker_solid_regions1.bmp",&runs_image);
	free(runs_image.data);
#endif

	////20101110 test line start
	init_feature.flag = 0;
	init_feature.checkflg = 1; 

	_mae_init_image_feature(in_image, &init_feature);

	_mae_detect_mark_color_region_all(in_image, h_image, bi_data, regions, region_num, &init_feature);
	////20101110 test line end

	//20110120 add for detect high color text start 

	_mae_detect_text_region_all(in_image, regions, region_num);

	//20110120 add for detect high color text end
	//20101225 add for move picture start
	//for(i = 0; i < region_num; i ++ )
	//{
	//	if( regions[i].type == MAE_REGION_TYPE_PICT )
	//	{
	//		for(j 
	//	}
	//}
	//20101225 add for move picture end

	temp_region_num = 0;
	for(i = 0; i < region_num; i ++ )
	{
		if( regions[i].label != MAE_MARK_REGION_NO ) {
			temp_region_num ++;
		}
	}

	_mae_malloc_uchar((unsigned char **)(&temp_regions), temp_region_num * sizeof(MAE_region),"mae_detect_marker_solid_regions : regions");
	idx = 0;
	for(i = 0; i < region_num; i ++ )
	{
		if( regions[i].label == MAE_MARK_REGION_NO ) {
			continue;
		}
		temp_regions[idx].label = regions[i].label;
		temp_regions[idx].type = regions[i].type;
		temp_regions[idx].l = regions[i].l;
		temp_regions[idx].t = regions[i].t;
		temp_regions[idx].w = regions[i].w;
		temp_regions[idx].h = regions[i].h;
		temp_regions[idx].linenum    = 0;
		//temp_regions[idx].lines     = NULL;
		temp_regions[idx].head   = NULL;
		temp_regions[idx].link  = NULL;

		temp_regions[idx].cornernum  = 4;
		_mae_malloc_uchar( (unsigned char**)(&temp_regions[idx].corners), 4 * sizeof(MAE_corner),"mae_detect_marker_solid_regions : MAE_corner");

		temp_regions[idx].corners[0].x = temp_regions[idx].l;
		temp_regions[idx].corners[0].y = temp_regions[idx].t;

		temp_regions[idx].corners[1].x = temp_regions[idx].l + temp_regions[idx].w;
		temp_regions[idx].corners[1].y = temp_regions[idx].t;

		temp_regions[idx].corners[2].x = temp_regions[idx].l + temp_regions[idx].w;
		temp_regions[idx].corners[2].y = temp_regions[idx].t + temp_regions[idx].h;

		temp_regions[idx].corners[3].x = temp_regions[idx].l ;
		temp_regions[idx].corners[3].y = temp_regions[idx].t + temp_regions[idx].h;

		idx++;
	}

	*mae_regions = temp_regions;
	*nregions = temp_region_num;
	//20100831 add start  break line
	//merge region based on rect pos
	//20101110 test line start
	//_mae_merge_break_region(mae_regions, nregions);
	//20101110 test line end

	//20100909 add start
	//merge region based on rect pos
	//20101110 test lines add start
	//_mae_merge_region_by_rect2(mae_regions, nregions);
	//20101110 test lines add end


	//20100831 add end
	//20101110 test line start
	//init_feature.flag = 1;
	//_mae_detect_mark_color_region_all(&zoom_image, &high_color_mae_image, bi_data, *mae_regions, *nregions, &init_feature);
	//20101110 test line end

	//20100909 add 
	//20101110 test for lines start
	//_mae_remove_small_regions2(mae_regions, nregions, width, height);
	//20101110 test for lines end
	if( regions != NULL ) 
	{
		free(regions);
	}
	//if( run_list != NULL )
	//{
	//	free(run_list);
	//}
	return 0;
}

