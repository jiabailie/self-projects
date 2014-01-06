#include "StdAfx.h"
#include <stdlib.h>
#include "runs.h"
#include "memalloc.h"
#include "imageutil.h"
#include "imageprocess.h"
#include "runsprocess.h"

#define RUN_NUMS     1000    /* reallocate run lists in incremental nums */

void _mae_get_runs(int **rx, int **ry, int **rlen, int *nruns,int *ralloc, 
			   unsigned char *cdata, int w, int h)
{
   int cx, cy;
   int trx, try_, trlen;

   *ralloc = 0;
   *nruns = 0;
   _mae_realloc_runs(rx, ry, rlen, ralloc, RUN_NUMS);

   cx = 0;
   cy = 0;
   while(_mae_next_run(&trx, &try_, &trlen, cdata, &cx, &cy, w, h)){
      _mae_add_run(trx, try_, trlen, rx, ry, rlen, nruns, ralloc);
   }
}

/**********************************************************/
int _mae_next_run(int *trx, int *try_, int *trlen, 
			  unsigned char *cdata, int *cx, int *cy, int w,int  h)
{
   unsigned char *cptr, *end, *endline;

   /* set pointer to resume search */
   cptr = cdata + ((*cy) * w) + (*cx);
   /* compute address for end of image */
   end = cdata + (w*h);
   /* compute address for end of current line */
   /* (actually beginning of next line ) */
   endline = cdata + (((*cy)+1) * w);

   /* while not at the end of the image ... */
   while(cptr < end){
      /* while not at the end of the line and have white pixels ... */
      while((cptr < endline) && (*cptr == 0)){
         cptr++;
         (*cx)++;
      }
      /* if at the end of the line (at beginning of next line ) */
      if(cptr == endline){
         /* reset x index and bump y index */
         (*cx) = 0;
         (*cy)++;
         /* bump end of line address */
         endline += w;
      }
      /* otherwise not at the end of the line, */
      /* so must be at the start of a new run of black pixels  ... */
      else{
         /* store the start position */
         *trx = (*cx);
         *try_ = (*cy);
         *trlen = 0;
         /* count the length of the run ... */
         while((cptr < endline) && (*cptr == 1)){
            cptr++;
            (*cx)++;
            (*trlen)++;
         }
         /* we now have the run, so before returning */
         /* if at the end of the line (at beginning of next line ) ... */
         if(cptr == endline){
            /* reset x index and bump y index */
            (*cx) = 0;
            (*cy)++;
         }
         return(1);
      }
   }
   /* otherwise, at the end of the image and no more runs found */
   return(0);
}

/**********************************************************/
void _mae_add_run(int trx, int try_, int trlen, 
			 int **rx, int **ry, int **rlen, 
			 int *nruns, int *ralloc)
{
   if(*nruns >= *ralloc)
      _mae_realloc_runs(rx, ry, rlen, ralloc, RUN_NUMS);
   (*rx)[*nruns] = trx;
   (*ry)[*nruns] = try_;
   (*rlen)[*nruns] = trlen;
   (*nruns)++;
}

/**********************************************************/
void _mae_realloc_runs(int **rx, int **ry, int **rlen, 
				  int *ralloc, int incr)
{
   /* allocate for the first time ... */
   if((*ralloc) == 0){
      (*ralloc) += incr;
      _mae_malloc_int(rx, *ralloc, "realloc_runs : rx");
      _mae_malloc_int(ry, *ralloc, "realloc_runs : ry");
      _mae_malloc_int(rlen, *ralloc, "realloc_runs : rlen");
      return;
   }

   /* reallocate if previously allocated lists are full ... */
   (*ralloc) += incr;
   _mae_realloc_int(rx, *ralloc, "realloc_runs : rx");
   _mae_realloc_int(ry, *ralloc, "realloc_runs : ry");
   _mae_realloc_int(rlen, *ralloc, "realloc_runs : rlen");
}
void _mae_runs_test(MAE_image *runs_image, unsigned char *_data, int _width, int _height)
{
   int *h_rx, *h_ry, *h_rlen, h_nruns, h_ralloc;
 //  int *v_rx, *v_ry, *v_rlen, v_nruns, v_ralloc;
   unsigned char *data;
//   unsigned char *v_data;
   int w;
   int h;
   //int v_w;
  // int v_h;

   data = _data;
   w = _width;
   h = _height;
   /* get all horizontal runs in the image */
   _mae_get_runs(&h_rx, &h_ry, &h_rlen, &h_nruns, &h_ralloc, data, w, h);

///////////////////
 //    FILE *fp = fopen("E:\\sharp\\±ÅE«∑÷∏˚x\data\\runs.txt","w");
	//if(fp!=NULL)
	//{
	//	for(int i = 0; i < h_nruns; i ++ )
	//	{
	//		fprintf(fp,"%d, %d, %d\n",h_ry[i],h_rx[i],h_rlen[i] );
	//	}
	//	fclose(fp);
	//}
///////////////////////////

   /* get all vertical runs in the image */
   //v_w = h;
   //v_h = w;
   //v_data = _mallocate_image(v_w, v_h, 8);
   //_grayimage_rotate_90(data, v_data, w, h);
   //_get_runs(&v_rx, &v_ry, &v_rlen, &v_nruns, &v_ralloc, v_data, v_w, v_h);


	runs_image->data  = _mae_mallocate_image(w,h,24);
	//_mae_remove_short_runs(h_rx, h_ry, h_rlen, h_nruns);
	_mae_save_h_runs_image(runs_image, h_rx, h_ry, h_rlen, h_nruns );
	//_save_v_runs_image(runs_image, v_ry, v_rx, v_rlen, v_nruns );

	free(h_rx);
	free(h_ry);
	free(h_rlen);

	//free(v_rx);
	//free(v_ry);
	//free(v_rlen);
	//
	//free(v_data);
}


