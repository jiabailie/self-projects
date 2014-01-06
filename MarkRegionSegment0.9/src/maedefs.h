#pragma once

//#define MAE_TEST_LINE_CORNER

#define BFT_ICON   0x4349   /* 'IC' */
#define BFT_BITMAP 0x4d42   /* 'BM' */
#define BFT_CURSOR 0x5450   /* 'PT' */

//output the middle results
//#define MAE_DEBUG

#define MAE_USE_GDIPLUS

#define MAE_NORMAL_X_SIZE  800
//#define MAE_NORMAL_Y_SIZE  1200
#define MAE_NORMAL_Y_SIZE  800

#define MAE_MARK_REGION_NO		-1
#define MAE_MARK_REGION_YES		1
#define MAE_MARK_REGION_MERGE_NEW   5
#define MAE_MARK_REGION_RECHECK   20
#define MAE_LINK_REGION_LABEL   11
#define MAE_NO_LINK_REGION   10

//#define MAE_MARK_RUNS_GAP   30
//#define MAE_MARK_RUNS_GAP   20
#define MAE_MARK_RUNS_GAP   10

//#define MAE_MARK_RUNS_GAP2   5
#define MAE_MARK_RUNS_GAP2   2


#define MAE_MARK_BLOCK_GAP    20
#define MAE_MARK_BREAK_REGION_GAP    10
#define MAE_MARK_BREAK_REGION_GAP2   30

#define MAE_MARK_LINE_GAP    30
#define MAE_MARK_LINE_WIDTH   40

//#define RGB_SEGMENT_VARIENCE  500
//used in detecting high color 
#define RGB_DETECT_CR_VARIENCE  500
//used in segmenting mark reion text 
#define RGB_DETECT_BACK_COLOR_VARIENCE  50
#define RGB_SEGMENT_VARIENCE  100
#define RGB_REMOVE_TEXT_VARIENCE  2000

#define MAE_BLOCK_PIXS_THRESHOLD  100
//#define MAE_BLOCK_PIXS_THRESHOLD  50
#define MAE_REGION_PIXS_THRESHOLD  100

#define MAUTI_NUMBER   10000
#define SHORT_RUNS_THRESHOLD   5

//#define MAE_MARK_LINE_GAP   30

#define MAE_SMALL_REGION_RATIO  0.1

#define MAE_REGION_RECT_CONNECT  10

#define MAE_MARK_TEXT_RATIO  0.2
#define MAE_MARK_MIN_TEXT_RATIO  0.1
#define MAE_MARK_MAX_TEXT_RATIO  0.5

#define MAE_REGION_FILL_MIN_RATIO  0.5
//#define MAE_REGION_FILL_MAX_RATIO  0.55
#define MAE_REGION_FILL_MAX_RATIO  0.5
#define MAE_REGION_FILL_MAX_RATIO2  0.5

#define MAE_MIN_LINE_WIDTH 5


#define MAE_SMALL_LINE_HEIGHT 10
#define MAE_SMALL_LINE_WIDTH  10
#define MAE_NORMAL_LINE_WIDTH 50
#define MAE_MIN_MARK_AREA 800

#define MAE_RGB_VARIANCE 50
#define MAE_RGB_VARIANCE2 180
#define MAE_RGB_VARIANCE3 150
#define MAE_RGB_VARIANCE4 120

//#define MAE_H_VARIANCE 20
#define MAE_H_VARIANCE 50
#define MAE_LINE_SLENDER_DEGREE 0.3
#define MAE_CENTER_MARK_RADIO 0.02
#define MAE_SLENDER_CENTER_MARK_RADIO 0.5


#define MAE_REGION_RECT_L  2
#define MAE_REGION_RECT_T  1
#define MAE_REGION_RECT_R  0
#define MAE_REGION_RECT_B  3

#define MAE_GET_MARK_REGION_MASK  0X00000000  //
#define MAE_GET_MARK_REGION_FLAG1  0X00000001  //reserve mark region
#define MAE_GET_MARK_REGION_FLAG2  0X00000002  //reserve not mark region
#define MAE_GET_MARK_REGION_FLAG3  0X00000004  //draw rect line
#define MAE_GET_MARK_REGION_FLAG4  0X00000008  //draw corners

//20100906 add mae region type
#define MAE_REGION_TYPE_RECT   16
#define MAE_REGION_TYPE_RECT_NL   1
#define MAE_REGION_TYPE_RECT_NT   2
#define MAE_REGION_TYPE_RECT_NR   4
#define MAE_REGION_TYPE_RECT_NB   6

#define MAE_REGION_TYPE_MARK   32
#define MAE_REGION_TYPE_PICT   64

#define MAE_SUBREGION_FILL_RATIO   0.01


#define MAE_HORI_LINE   1
#define MAE_VERT_LINE   2

#define MAE_TRACE_DIRECT_NO   -1
#define MAE_TRACE_DIRECT_R   0
#define MAE_TRACE_DIRECT_U   1
#define MAE_TRACE_DIRECT_L   2
#define MAE_TRACE_DIRECT_D   3

//20110121 update start
//#define MAE_SPLIT_IMAGE_STEP   50
#define MAE_SPLIT_IMAGE_STEP   30
//#define MAE_SPLIT_IMAGE_STEP   10
//20110121 update end

#define MAE_MAX_LINE_WIDHT   60
//#define MAE_MAX_LINE_GAP     50
#define MAE_MAX_LINE_GAP    50
#define MAE_MAX_H_V_LINE_GAP  50
//#define MAE_MASH_IMAGE_STEP   3

typedef struct _MAE_image{
	int type; //1: char, 2: short, 3: long
	int xdpi;
	int ydpi;
	int w, h;
	int depth;
	int stride;
	unsigned char *data;
} MAE_image;

enum _MAE_ERROR_CODES{
	MAE_NO_ERROR = 0,
	MAE_FILE_NOT_EXIST,
	MAE_FILE_UNSUPPORT_FORMAT,
	MAE_OPEN_IMAGE_FAIL,
	MAE_READ_IMAGE_FAIL,
	MAE_SAVE_IMAGE_FAIL,
	MAE_ERROR_NUMS
};


typedef struct _MAE_corner{
	//int type;
	int x;
	int y;
	//_MAE_corner *link;
}MAE_corner;

typedef struct _MAE_line{
	int type; //0: hori 1:vertical
	int label;
	//中心线(sx,sy)->(ex,ey)
	int sx;
	int sy;
	int ex;
	int ey;
	int lw;  //line width
	//左上角(l,t),宽w,高h
	int l;
	int t;
	int w;
	int h;

	//r,g,b的均值
	int re; //red mean
	int ge; //green mean
	int be; //blue mean

	//hsv中的值
	int he;

	//	void *node_l;
	//	void *node_r;
	//	void *node_u;
	//	void *node_d;
	void *node;
}MAE_line;

typedef struct _MAE_line_node{
	int label;
	int type;
	//MAE_line *line_l;
	//MAE_line *line_r;
	//MAE_line *line_u;
	//MAE_line *line_d;
	MAE_line *h_line;
	MAE_line *v_line;
	_MAE_line_node *l_link;
	_MAE_line_node *r_link;
	_MAE_line_node *u_link;
	_MAE_line_node *d_link;
	int x;
	int y;
	_MAE_line_node *next;
	_MAE_line_node *h_link;
	_MAE_line_node *v_link;
}MAE_line_node;


typedef struct _MAE_node{
	int x;  /*pointer of the rx */
	int y;  /*pointer of the ry */
	int len;/*pointer of the run len */
	int label;/*label of the run len */
	void *parent; /*pointer of block that contains this run */
	struct _MAE_node* prev; /* previous node pointer*/
	struct _MAE_node* next; /* next node pointer */
	struct _MAE_node* link; /* next node pointer for link table */
}MAE_node;
// 
//typedef struct _MAE_block{
//	int label;
//	int ralloc;
//	int *rlen;/*array of the run len */
//	int *rx;  /*array of the rx */
//	int *ry;  /*array of the ry */
//	int nruns;/*array of the run len */
//} MAE_block;

typedef struct _MAE_block{
	int label; /*region label */
	int l;
	int t;/*left, top of the surrouding rectangle.*/
	int w, h;/*width, height of the surrounding rectangle.*/
	MAE_node* head; /* head pointer of block link */
	MAE_node* tail; /* tail pointer of block link */
	struct _MAE_block *next; /* next pointer of block link */
} MAE_block;


//typedef struct _MAE_region{
//	int type;
//	int label;
//	int l;
//	int t;/*left, top of the surrouding rectangle.*/
//	int w, h;/*width, height of the surrounding rectangle.*/
//	int num;/*array of the run len */
//	int *rlen;/*array of the run len */
//	int *rx;/*array of the rx */
//	int *ry;/*array of the ry */
//} MAE_region;
//
#define EDGE_MAX_POINT 100
#define EDGE_MAX_LINE 100
typedef struct _MAE_region{
	int type;
	int label;
	int l;
	int t;/*left, top of the surrouding rectangle.*/
	int w, h;/*width, height of the surrounding rectangle.*/
	//	long mk_pix_num; /* all pix num is marked in mark region */
	//	long mt_pix_num; /* only text pix num is marked in mark region */
	int linenum;          //verticline number
	MAE_line lines[EDGE_MAX_LINE];  //line array

	int cornernum;          //verticline number
	MAE_corner *corners;  //line array

	MAE_node* head;
	MAE_node* tail;
	//_MAE_region * h_head; //sub region
	//_MAE_region * h_link;
	//_MAE_region * v_head; //sub region
	//_MAE_region * v_link;
	//_MAE_region * sub_head; //sub region
	//_MAE_region * sub_link; //sub region
	_MAE_region * link; //sub region
} MAE_region;

typedef struct _MAE_image_feature{
	int label;
	int width;
	int height;
	int area;
	int mark_pixs;
	int center_area;
	int center_mark_pixs;

	int hsplitnum;
	int vsplitnum;

	float submpixs[9];

	float r_e[9];
	float g_e[9];
	float b_e[9];
	//float r_v[4];
	//float g_v[4];
	//float b_v[4];
	float h_e[9];
	//float h_v[4];
	float min_re;
	float min_ge;
	float min_be;
	float max_re;
	float max_ge;
	float max_be;
	float min_he;
	float max_he;
	long mk_pix_num; /* all pix num is marked in mark region */
	long mt_pix_num; /* only text pix num is marked in mark region */

	int tw;
	int th;
	int tcount;
	int max_connect_w;
	int max_connect_h;
	//long mrt_pix_num; /* only text pix num is marked in mark region */
	int contour_pixs;
} MAE_image_feature;

typedef struct _MAE_init_feature{
	int width;
	int height;
	int t_width;
	int t_height;
	int r_bk;
	int g_bk;
	int b_bk;
	int flag;
	int checkflg;
} MAE_init_feature;

#define MAE_ASSERT assert