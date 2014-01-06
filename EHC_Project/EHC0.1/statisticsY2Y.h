#pragma once

#include "datahead.h"

/**
 * yangruiguo 2012-02-02
 * num       : the number of pictures
 * h         : the hight of each hmrf
 * w         : the width of each hmrf
 * h_num     : the number of high quality images' cluster
 * ph_mrf    : the project result of high quality images
 * return    : 0 success 
 */
int _statisY2Y(int num, int h, int w, int h_num, int** ph_mrf);

/**
 * yangruiguo 2012-02-02
 * function  : clear the one dimension array
 * len       : the length of data array
 * c         : the count array
 * y         : the statistic result array
 */
void clearDimension(int len, int*& c, float**& y);

/**
 * yangruiguo 2012-02-02
 * h         : the height of hmrf
 * w         : the width of hmrf
 * h_mrf     : the high quality images' MRF
 * c         : the amount of certain MRF appearance in this type
 * y         : the calculated result, a 2 dimensions array
 * return    : 0 success 
 */
int _statisY2Y_step1_A(int h, int w, int* ph_mrf, int* c, float** y);

/**
 * yangruiguo 2012-02-02
 * h         : the height of hmrf
 * w         : the width of hmrf
 * h_mrf     : the high quality images' MRF
 * c         : the amount of certain MRF appearance in this type
 * y         : the calculated result, a 2 dimensions array
 * return    : 0 success 
 */
int _statisY2Y_step1_B(int h, int w, int* ph_mrf, int* c, float** y);

/**
 * yangruiguo 2012-02-02
 * h         : the height of hmrf
 * w         : the width of hmrf
 * h_mrf     : the high quality images' MRF
 * c         : the amount of certain MRF appearance in this type
 * y         : the calculated result, a 2 dimensions array
 * return    : 0 success 
 */
int _statisY2Y_step1_C(int h, int w, int* ph_mrf, int* c, float** y);

/**
 * yangruiguo 2012-02-02
 * h         : the height of hmrf
 * w         : the width of hmrf
 * h_mrf     : the high quality images' MRF
 * c         : the amount of certain MRF appearance in this type
 * y         : the calculated result, a 2 dimensions array
 * return    : 0 success 
 */
int _statisY2Y_step1_D(int h, int w, int* ph_mrf, int* c, float** y);

/**
 * yangruiguo 2011-12-21
 * h         : the length of ca
 * ca        : the amount of certain MRF appearance in type a
 * ya        : the calculated result A, a 2 dimensions array
 * return    : 0 success 
 */
int _statisY2Y_step2(int h, int* ca, float** ya);