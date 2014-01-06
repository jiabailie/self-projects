#pragma once
#include "maedefs.h"

void _mae_get_runs(int **rx, int **ry, int **rlen, int *nruns,int *ralloc, 
			   unsigned char *cdata, int w, int h);
int _mae_next_run(int *trx, int *try_, int *trlen, 
			  unsigned char *cdata, int *cx, int *cy, int w,int  h);
void _mae_add_run(int trx, int try_, int trlen, 
			 int **rx, int **ry, int **rlen, 
			 int *nruns, int *ralloc);
void _mae_realloc_runs(int **rx, int **ry, int **rlen, 
				  int *ralloc, int incr);
void _mae_runs_test(MAE_image *runs_image, unsigned char* data, int w, int h );
