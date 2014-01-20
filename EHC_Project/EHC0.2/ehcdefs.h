#pragma once

//#define MAE_TEST_LINE_CORNER

#define BFT_ICON   0x4349   /* 'IC' */
#define BFT_BITMAP 0x4d42   /* 'BM' */
#define BFT_CURSOR 0x5450   /* 'PT' */

//#define MAE_DEBUG

#define EHC_USE_GDIPLUS

typedef struct _EHC_image{
 int type; //1: char, 2: short, 3: long
 int xdpi;
 int ydpi;
 int w, h;
 int depth;
 int stride;
 unsigned char *data;
} EHC_image;

enum _EHC_ERROR_CODES{
 EHC_NO_ERROR = 0,
 EHC_FILE_NOT_EXIST,
 EHC_FILE_UNSUPPORT_FORMAT,
 EHC_OPEN_IMAGE_FAIL,
 EHC_READ_IMAGE_FAIL,
 EHC_SAVE_IMAGE_FAIL,
 EHC_ERROR_NUMS
};


#define MAE_ASSERT assert