#pragma once

#include "datahead.h"

/**
 * Candy 2011-12-21
 * num       : the number of pictures
 * h_num     : the number of high quality images' cluster
 * l_num     : the number of low quality images' cluster
 * hmrf      : the replaced high quality images' MRF
 * h_mrf     : the high quality images' MRF
 * l_mrf	 : the low quality images' MRF
 * h_cluster : the high quality images' clusters
 * l_cluster : the low quality images' clusters
 * x2y       : the calculated result , a 2 dimensions array, y-->x
 * orderx2y  : the order result , a 2 dimensions array, y-->x
 * return    : 0 success 
 */
int _statisX2Y(int num, LL h_num, LL l_num, LL** hmrf, IMAGE_H_MRF* h_mrf, IMAGE_L_MRF* l_mrf, H_cluster* h_cluster, L_cluster* l_cluster, float*** x2y, LL*** orderx2y);

/**
 * Candy 2011-12-26
 * l_num     : the number of low quality images' cluster
 * l_statis  : the low quality images' project
 * l_mrf     : the low images' MRF
 * lmrf      : the replaced low images' MRF
 * return    : 0 success 
 */
int _replaceX2X(LL l_num, L_statis* l_statis, IMAGE_L_MRF *l_mrf, LL* lmrf);

/**
 * Candy 2011-12-26
 * l_num     : the number of low quality images' cluster
 * l_cluster : the low quality images' clusters
 * l_statis  : the projected result
 * return    : 0 success 
 */
int _projectX2X(LL l_num, L_cluster* l_cluster, L_statis* l_statis);
