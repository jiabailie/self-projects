#include "stdafx.h"
#include "memalloc.h"

#define MAUTI_NUMBER   10000
#define SHORT_RUNS_THRESHOLD   10
//
//int _find_runs_max_len(int *rx, int *ry, int *rlen, int num)
//{
//	int maxlen;
//	int i;
//
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
//
//int _remove_short_runs(int *rx, int *ry, int *rlen, int num)
//{
//	int maxlen;
//	int i;
//	for(i = 0; i < num; i ++)
//	{
//		if( rlen[i] < SHORT_RUNS_THRESHOLD )
//		{
//			rx[i] = 0;
//			ry[i] = 0;
//			rlen[i] = 0;
//		}
//	}
//	return 0;
//}
//
//int _sort_runs_inc(int *rx, int *ry, int* rlen, int num)
//{
//	int x_i;
//	int y_i;
//	int x_j;
//	int y_j;
//	int i;
//	int j;
//
//	int _x_temp;
//	int _y_temp;
//	int _len_temp;
//
//	long _pos_i;
//	long _pos_j;
//	
//	//Ã°ÅÝÅÅÐE
//	for(i = 0; i < num; i ++)
//		for(j = i+1; j < num; j ++) 
//		{
//			x_i = rx[i];
//			y_i = ry[i];
//			x_j = rx[j];
//			y_j = ry[j];
//			
//			_pos_i = x_i * MAUTI_NUMBER + y_i;
//			_pos_j = x_j * MAUTI_NUMBER + y_j;
//			if( _pos_i > _pos_j )
//			{
//				_x_temp = rx[i];
//				_y_temp = ry[i];
//				_len_temp = rlen[i];
//				rx[i] = rx[j];
//				ry[i] = ry[j];
//				rlen[i] = rlen[j];
//				rx[j] = _x_temp;
//				ry[j] = _y_temp;
//				rlen[j] = _len_temp;
//			}
//		}
//
//	return 0;
//}
