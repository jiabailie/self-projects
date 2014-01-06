#pragma once

#include "datahead.h"

/**
 * yangruiguo 2012-02-02
 * function  : statistic the table x to y
 * num       : the number of image pairs
 * h         : the height of each image
 * w         : the width of each image
 * l_num     : the number of low images' cluster
 * h_num     : the number of high images' cluster
 * pl_mrf    : the array which saves the projected low quality images 
 * ph_mrf    : the array which saves the projected high quality images
 * return    : 0 success 
 */
int _statisX2Y(int num, int h, int w, int l_num, int h_num, int** pl_mrf, int** ph_mrf);