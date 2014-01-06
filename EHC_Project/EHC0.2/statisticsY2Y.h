#pragma once

#include "datahead.h"

/**
 * yangruiguo 2011-12-20
 * num       : the number of pictures
 * h_num     : the number of high quality images' cluster
 * h_mrf     : the high quality images' MRF
 * h_cluster : the high quality images' clusters
 * y2yA      : the calculated result A, a 2 dimensions array, right
 * y2yB      : the calculated result B, a 2 dimensions array, top
 * y2yC      : the calculated result C, a 2 dimensions array, top-right
 * y2yD      : the calculated result D, a 2 dimensions array, bottom-right
 * return    : 0 success 
 */
//Modify_Start 20111223 Candy
//int _statisY2Y(int num, LL h_num, IMAGE_H_MRF* h_mrf, H_cluster* h_cluster, float*** y2yA, float*** y2yB, float*** y2yC, float*** y2yD);
int _statisY2Y(int num, LL h_num, IMAGE_H_MRF* h_mrf, LL*** o_hmrf, H_cluster* h_cluster, float*** y2yA, float*** y2yB, float*** y2yC, float*** y2yD);
//Modify_End 20111223

/**
 * yangruiguo 2011-12-20
 * h_num     : the number of high quality images' cluster
 * h_statis  : the high quality images' project
 * h_mrf     : the high images' MRF
 * hmrf      : the replaced high images' MRF
 * return    : 0 success 
 */
int _replaceY2Y(LL h_num, H_statis* h_statis, IMAGE_H_MRF *h_mrf, LL* hmrf);

/**
 * yangruiguo 2011-12-20
 * h_num     : the number of high quality images' cluster
 * h_cluster : the high quality images' clusters
 * h_statis  : the projected result
 * return    : 0 success 
 */
int _projectY2Y(LL h_num, H_cluster* h_cluster, H_statis* h_statis);

/**
 * yangruiguo 2011-12-21
 * h         : the height of hmrf
 * w         : the width of hmrf
 * h_mrf     : the high quality images' MRF
 * ca        : the amount of certain MRF appearance in type a
 * cb        : the amount of certain MRF appearance in type b
 * cc        : the amount of certain MRF appearance in type c
 * cd        : the amount of certain MRF appearance in type d
 * ya        : the calculated result A, a 2 dimensions array
 * yb        : the calculated result B, a 2 dimensions array
 * yc        : the calculated result C, a 2 dimensions array
 * yd        : the calculated result D, a 2 dimensions array
 * return    : 0 success 
 */
int _statisY2Y_step1(int h, int w, LL* hmrf, int* ca, int* cb, int* cc, int* cd, float** ya, float** yb, float** yc, float** yd);

/**
 * yangruiguo 2011-12-21
 * h         : the length of ca
 * ca        : the amount of certain MRF appearance in type a
 * ya        : the calculated result A, a 2 dimensions array
 * return    : 0 success 
 */
int _statisY2Y_step2(LL h, int* ca, float** ya);