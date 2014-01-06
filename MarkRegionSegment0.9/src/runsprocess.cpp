#include "stdafx.h"
#include "memalloc.h"
#include "runsprocess.h"
#include "maedefs.h"
//
//int _mae_find_runs_max_len(int *rx, int *ry, int *rlen, int num)
//{
//	int maxlen;
//	int i;
//	maxlen = 0;
//	for(i = 0; i < num; i ++)
//	{
//		if(maxlen < rlen[i] )
//		{
//			maxlen = rlen[i];
//		}
//	}
//	return maxlen;
//}

int _mae_remove_short_runs(int *rx, int *ry, int *rlen, int num, int threshold)
{
//	int maxlen;
	int i;
	for(i = 0; i < num; i ++)
	{
		//if( rlen[i] < SHORT_RUNS_THRESHOLD )
		if( rlen[i] < threshold )
			
		{
			rx[i] = 0;
			ry[i] = 0;
			rlen[i] = 0;
		}
	}
	return 0;
}
int _mae_remove_long_runs(int *rx, int *ry, int *rlen, int num, int threshold)
{
//	int maxlen;
	int i;
	for(i = 0; i < num; i ++)
	{
		//if( rlen[i] < SHORT_RUNS_THRESHOLD )
		if( rlen[i] > threshold )
			
		{
			rx[i] = 0;
			ry[i] = 0;
			rlen[i] = 0;
		}
	}
	return 0;
}
int _mae_remove_long_runs2(int *rx, int *ry, int *rlen, int num, int width, int threshold)
{
//	int maxlen;
	int i, k;
    int *projection = (int*) malloc(width*sizeof(int));
    
    //projection
    memset(projection, 0, width*sizeof(int));
    for(i = 0; i < num; i++){
        for(k = rx[i]; k < rx[i] + rlen[i]; k++){
            projection[k] += 1;
        }
    }
	for(i = 0; i < num; i ++)
	{
        int sum = 0;
		//if( rlen[i] < SHORT_RUNS_THRESHOLD )
		if( rlen[i] > threshold )
			
		{
			rx[i] = 0;
			ry[i] = 0;
			rlen[i] = 0;
		}

         for(k = rx[i] + rlen[i]/3; k < rx[i] + 2*rlen[i]/3; k++){
             sum += projection[k];
         }

         if(sum < rlen[i]*rlen[i]/3){
             rx[i] = 0;
             ry[i] = 0;
             rlen[i] = 0;
         }
	}
	return 0;
}
//
//int _mae_sort_runs_inc(int *rx, int *ry, int* rlen, int num)
//{
//	//int x_i;
//	//int y_i;
//	//int x_j;
//	//int y_j;
//	//int i;
//	//int j;
//
//	//int _x_temp;
//	//int _y_temp;
//	//int _len_temp;
//
//	//long _pos_i;
//	//long _pos_j;
//	//
//	////Ã°ÅÝÅÅÐE
//	//for(i = 0; i < num; i ++)
//	//	for(j = i+1; j < num; j ++) 
//	//	{
//	//		x_i = rx[i];
//	//		y_i = ry[i];
//	//		x_j = rx[j];
//	//		y_j = ry[j];
//	//		
//	//		_pos_i = x_i * MAUTI_NUMBER + y_i;
//	//		_pos_j = x_j * MAUTI_NUMBER + y_j;
//	//		if( _pos_i > _pos_j )
//	//		{
//	//			_x_temp = rx[i];
//	//			_y_temp = ry[i];
//	//			_len_temp = rlen[i];
//	//			rx[i] = rx[j];
//	//			ry[i] = ry[j];
//	//			rlen[i] = rlen[j];
//	//			rx[j] = _x_temp;
//	//			ry[j] = _y_temp;
//	//			rlen[j] = _len_temp;
//	//		}
//	//	}
//
//	return 0;
//}
//
//int _mae_compute_runs_average_len(int *rx, int *ry,int *rlen, int num)
//{
//	//int max_len;
//	//int i;
//
//	//for(i = 0; i < num; i ++)
//	//{
//	//	av_len +=  (double)rlen[i] / (double)num;
//	//}
//	//return (int)av_len;
//	return 0;
//}


int _mae_link_break_runs(int *rx, int *ry, int *rlen, int num, int threshold)
{
	int i, j;
	
	for(i = 0; i < num; i ++)
	{
		if(rlen[i] == 0 ) continue;
		for(j = i+1; j < num; j ++)
		{
			if( ry[j] != ry[i] ) break;
			if(    (ry[i] == ry[j]) 
				&& ((rx[j] - rx[i]-rlen[i])< threshold ) )
			{
				rlen[i] += (rx[j] - rx[i] -rlen[i]) + rlen[j] ;
				rx[j] = 0;
				ry[j] = 0;
				rlen[j] = 0;
			}
		}
	}
	return 0;
}

int _mae_get_vline_width(unsigned char *data, int w, int h, int *rx, int *ry, int *rlen, int num, int *lw)
{
	//int i, j;
	//int *runlen;
	//_mae_malloc_int(runlen, h, "_mae_get_vline_width : runlen");
	//for(i = 0; i < h ; i ++ )
	//{
	//	runlen[i] = 0;
	//}
	//for(i = 0; i < num; i ++)
	//{
	//	if(rlen[i] == 0 ) continue;
	//	for(j = i+1; j < num; j ++)
	//	{
	//		if( ry[j] != ry[i] ) break;
	//		if(    (ry[i] == ry[j]) 
	//			&& ((rx[j] - rx[i]-rlen[i])< threshold ) )
	//		{
	//			rlen[i] += (rx[j] - rx[i] -rlen[i]) + rlen[j] ;
	//			rx[j] = 0;
	//			ry[j] = 0;
	//			rlen[j] = 0;
	//		}
	//	}
	//}
	return 0;
}
int _mae_get_hline_width(unsigned char *data, int w, int h, int *rx, int *ry, int *rlen, int num, int *lw)
{
	return 0;
}
