#include "datahead.h"

/**
 * yangruiguo 2011-12-06
 * namefile: the file which includes all of the needing file name list
 * namelist: array saves the pics' name
 * return  : 0 success 
 */
int loadsamplenamelist(const char* namefile, Sample_name** namelist, int * num);
int initimagemrf(IMAGE_H_MRF **h_mrf, IMAGE_L_MRF **l_mrf, int num);
int loadsamples(Sample_name* namelist, int num, IMAGE_H_MRF *h_mrf, IMAGE_L_MRF *l_mrf);
int loadhighimage(char *name, IMAGE_H_MRF *h_mrf);
int loadlowimage(char *name, IMAGE_L_MRF *l_mrf);

/**
 * yangruiguo 2011-12-08
 * bi_data: the data of processed pictures
 * width  : the width of processed pictures
 * height : the height of processed pictures
 * h_mrf  : the high quality pictures' MRF
 * return : 0 success 
 */
int _createHMRF(unsigned char *bi_data, int width, int height, IMAGE_H_MRF *h_mrf);

/**
 * yangruiguo 2011-12-08
 * bi_data: the data of processed pictures
 * width  : the width of processed pictures
 * height : the height of processed pictures
 * h_mrf  : the low quality pictures' MRF
 * return : 0 success 
 */
int _createLMRF(unsigned char *bi_data, int width, int height, IMAGE_L_MRF *l_mrf);

/**
 * yangruiguo 2011-12-15
 * bi_data: the data of processed pictures
 * w      : the width of processed pictures
 * xs     : the start position of x axis
 * ys     : the start position of y axis
 * xe     : the end position of x axis
 * ye     : the end position of y axis
 * return : the processed result
 */
int _createSUB_MRF(unsigned char* bi_data, int w, int xs, int ys, int xe, int ye);

int destroy(IMAGE_H_MRF *h_mrf, IMAGE_L_MRF *l_mrf, int num);
int destroy_lmrf(IMAGE_L_MRF *l_mrf, int num);
int destroy_hmrf(IMAGE_H_MRF *h_mrf, int num);
int destroy_lcluster(int l_num, L_cluster* l_cluster);
int destroy_hcluster(int h_num, H_cluster* h_cluster);
int destroy_lstatis(L_statis* l_statis);
int destroy_hstatis(H_statis* h_statis);
int destroy_namelist(Sample_name* namelist);
int destroy_2d_array(int xlen, int** pl_mrf);

void _testLMRF(int width, int height, IMAGE_L_MRF *l_mrf, const char* fname);
void _testHMRF(int width, int height, IMAGE_H_MRF *h_mrf, const char* fname);