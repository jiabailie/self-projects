#pragma once

#include "datahead.h"

int restore(char* infilename, char *outfilename);

/**
 * yangruiguo 2011-12-27
 * num     : the number of low images' cluster for restore
 * lcluster: the low images' clusters
 * l_mrf   : the low images' MRF
 * return  : 0 success
 */
int replaceLMRF_Store(LL num, L_cluster* lcluster, IMAGE_L_MRF *l_mrf);

/**
 * yangruiguo 2011-12-27
 * return  : 0 success
 */
int makeFinalMRF(IMAGE_L_MRF* l_mrf, IMAGE_H_MRF* h_mrf, 
  LL l_num, L_cluster* l_cluster,
  LL h_num, H_cluster* h_cluster,
  float* y2yA, LL ah, LL aw,
  float* y2yB, LL bh, LL bw,
  float* y2yC, LL ch, LL cw,
  float* y2yD, LL dh, LL dw,
  float* x2y, LL xyh, LL xyw,
  float* orderx2y, LL oh, LL ow
  );