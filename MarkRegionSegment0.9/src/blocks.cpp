#include "stdafx.h"
#include "blocks.h"
#include "memalloc.h"
#include "runs.h"
#include <stdlib.h>
#include "chain.h"
#include <math.h>

#define MAE_IMAGE_BLOCKS     2000    /* reallocate block lists in incremental nums */

//get blocks from runs chain 
void _mae_create_runs_chain(MAE_node ** _runlist_, int *listnum,int *rx, int *ry, int *rlen, int nruns)
{
	MAE_node * run_list;
	//MAE_node * head = NULL;
	//	MAE_node * tail = NULL;
	int i;

	_mae_malloc_uchar((unsigned char**)(&run_list), nruns * sizeof(MAE_node),"_mae_create_runs_chain : run_list");

	for( i = 0; i < nruns; i ++ )
	{
		run_list[i].x = rx[i]; 
		run_list[i].y = ry[i]; 
		run_list[i].len = rlen[i];
		//run_list[i].label = -1;
		run_list[i].prev = NULL;
		run_list[i].next = NULL;
		run_list[i].link = NULL;
		run_list[i].parent = NULL;
		//_mae_add_node(&head, &tail,&run_list[i]);	
	}
	//*_head_ = head;
	*_runlist_ = run_list;
	*listnum = nruns; 
}
void _mae_create_runs_chain_split(MAE_node ** _runlist_, int *listnum,int *rx, int *ry, int *rlen, int nruns)
{
	MAE_node * run_list;
	int idx = 0;
	//MAE_node * head = NULL;
	//	MAE_node * tail = NULL;
	int i;

	_mae_malloc_uchar((unsigned char**)(&run_list), nruns * sizeof(MAE_node),"_mae_create_runs_chain : run_list");

	for( i = 0; i < nruns; i ++ )
	{
		if( ry[i] % MAE_SPLIT_IMAGE_STEP == 0 )
		{
			continue;
		}
		run_list[idx].x = rx[i]; 
		run_list[idx].y = ry[i]; 
		run_list[idx].len = rlen[i];
		run_list[idx].prev = NULL;
		run_list[idx].next = NULL;
		run_list[idx].link = NULL;
		run_list[idx].parent = NULL;

		idx++;
	}
	for( i = idx; i < nruns; i ++ )
	{
		run_list[idx].x = 0; 
		run_list[idx].y = 0; 
		run_list[idx].len = 0;
		run_list[idx].label = -1;
		run_list[idx].prev = NULL;
		run_list[idx].next = NULL;
		run_list[idx].link = NULL;
		run_list[idx].parent = NULL;

		idx++;
	}
	//*_head_ = head;
	*_runlist_ = run_list;
	*listnum = nruns; 
}

int _mae_cluster_runs_chain(MAE_node* run_list, int listnum, MAE_block **blocks, int* nblocks)
{
	//	MAE_block *block;
	MAE_node * p;

	MAE_node * pr_head;
	MAE_node * cr_head;

	MAE_node * d_node;

	int i;
	//int cur_blocks = 0;
	int ralloc = 0;
	//int idx;

	pr_head = NULL;
	cr_head = NULL;

	//_mae_realloc_blocks(blocks, &ralloc, MAE_IMAGE_BLOCKS);
	ralloc = listnum;
	_mae_alloc_blocks(blocks, &ralloc);

	int label = 0;
	*nblocks = 0;

	for( i = 0; i < listnum; i ++ )
	{
		p = &run_list[i];
		if(p->len != 0 ) 
		{
			//new line runs
			if(cr_head == NULL || (p->y == cr_head->y) )
			{
				_mae_add_node_link( &(cr_head ), p );
			}else{
				pr_head = cr_head;
				cr_head = NULL;
				_mae_add_node_link( &(cr_head ), p );
			}
			//
			d_node = _mae_detect_run_by_prevrow(pr_head, p);
			if(d_node == NULL )
			{
				_mae_run_add_new_block(p, blocks, &ralloc, nblocks, label);
				label ++;
			}else{
				_mae_run_add_block(p, blocks, nblocks, d_node);
				_mae_become_other_block_label(pr_head, d_node,blocks, nblocks, p);
			}

		}
	}

	return 0;
}

int  _mae_runs_near_chain(MAE_node *p,MAE_node *q )
{
	if(    (p->y  == q->y  + 1  || q->y  == p->y + 1 ) 
		&& (    p->x >= q->x && p->x <= q->x + q->len + 1 
		|| q->x >= p->x && q->x <= p->x + p->len + 1  ) )
	{
		return 1;
	}
	return 0;

}
//
void _mae_run_add_new_block(MAE_node* node, MAE_block **blocks, int* ralloc, int* nblocks, int label)
{
	int  block_num;
	block_num = *ralloc;
	//if(*nblocks >= *ralloc)
	//{
	// _mae_realloc_blocks(blocks, ralloc, MAE_IMAGE_BLOCKS);
	//}
	(*blocks)[*nblocks].label = label;
	(*blocks)[*nblocks].head = NULL;
	(*blocks)[*nblocks].tail = NULL;

	(*nblocks)++;
	label ++;
	_mae_run_add_block(node, blocks, nblocks, NULL );	
}
void _mae_run_add_block(MAE_node *node, MAE_block **blocks, int* nblocks, MAE_node *d_node)
{
	int _idx;
	MAE_block *block;
	if(d_node == NULL ) // new blocks
	{
		_idx = (*nblocks) - 1;
		block = &((*blocks)[_idx] );
	}else{
		block = (MAE_block *)d_node->parent;
	}
	node->parent = block;
	//_mae_add_node( &((*blocks)[_idx].head ), &((*blocks)[_idx].tail), node );
	_mae_add_node( &(block->head ), &(block->tail), node );
}
void _mae_realloc_blocks(MAE_block **blocks, int *ralloc, int incr)
{
	//unsigned char * _data;

	/* allocate for the first time ... */
	if((*ralloc) == 0){
		(*ralloc) += incr;
		_mae_malloc_uchar((unsigned char**)blocks, incr * sizeof(MAE_block),"_mae_realloc_blocks : blocks");
		return;
	}
	/* reallocate if previously allocated lists are full ... */
	(*ralloc) += incr;
	_mae_realloc_uchar((unsigned char**)blocks, (*ralloc) * sizeof(MAE_block),"_realloc_regions : region");
}
void _mae_alloc_blocks(MAE_block **blocks, int *alloc)
{
	_mae_malloc_uchar((unsigned char**)blocks, (*alloc) * sizeof(MAE_block),"_mae_alloc_blocks : blocks");
}

//void _mae_destroy_blocks(MAE_block* blocks, int nblocks)
void _mae_destroy_blocks(MAE_block* blocks)
{
	free(blocks);
}

/////////////////////////
MAE_node*  _mae_detect_run_by_prevrow(MAE_node* pr_head,MAE_node* node)
{
	MAE_node *p;
	p = pr_head;
	if(p == NULL )
	{
		return NULL;
	}
	if(p->y < node->y - 1 )
	{
		return NULL;
	}
	while(p != NULL)
	{
		if( _mae_runs_near_chain(p,node) )
		{
			return p;
		}
		p = p->link;
	}

	return NULL;
}
void _mae_become_other_block_label(MAE_node* pr_head,MAE_node* d_node, MAE_block **blocks, int *nblocks, MAE_node *node)
{
	MAE_node *p;
	MAE_node *prhead;
	MAE_block *d_block;
	MAE_block *block;
	//	MAE_block *block2;
	int label;
	int k;

	//prhead is not be used
	prhead = pr_head;
	//

	p = d_node->link;
	d_block = (MAE_block *)node->parent;

	while(p != NULL)
	{
		if( _mae_runs_near_chain(p,node) )
		{
			block = (MAE_block *)p->parent;
			label = block->label;
			block->label = d_block->label;
			for(k = 0; k < *nblocks; k ++ )
			{
				if( (*blocks)[k].label == label )
				{
					(*blocks)[k].label = d_block->label;
				}
			}

		}
		p = p->link;
	}
	return;
	//MAE_node *p, *q;
	//MAE_block *d_block;
	//MAE_block *block;
	//MAE_block *block2;
	//int label;
	//int label2;
	//int k;

	//d_block = (MAE_block *)d_node->parent;
	//label =d_block->label;

	//p = d_node->link;
	//while(p != NULL)
	//{
	//	if( _mae_runs_near_chain(p,node) )
	//	{
	//		block = (MAE_block *)p->parent;
	//		label2 = block->label;
	//		if(label2 != label)
	//		{
	//			q = pr_head;
	//			while(q != NULL )
	//			{
	//				block = (MAE_block *)p->parent;
	//				if(label2 == block->label )
	//				{
	//					//合并block
	//					if( block->label != -1 )
	//					{
	//						_mae_add_node_list(&d_block->head, &d_block->tail,  block->head, block->tail);
	//						block->head = NULL;
	//						block->tail = NULL;
	//						block->label = -1;
	//					}else{
	//						p->parent = d_block;
	//					}
	//				}
	//				q = q->link;
	//			}
	//		}

	//	}
	//	p = p->link;
	//}
	//   return;
}


int _mae_get_label_blocks(MAE_block *i_blocks, int i_nblocks, MAE_block **o_blocks, int *o_nblocks)
{
	int i, k;
	int * count_label;
	int label;

	unsigned char *blocks_data;

	MAE_block * blocks;
	int nblocks;

	int block_num;

	blocks = i_blocks;
	nblocks = i_nblocks;

	int idx;

	_mae_malloc_int(&count_label, nblocks, "_mae_get_regions_from_blocks : label");
	for(i = 0; i < nblocks; i ++ )
	{
		count_label[i]  = 0;
	}

	for(i = 0; i < nblocks; i ++ )
	{
		label = blocks[i].label;
		count_label[label] ++;
	}
	block_num = 0;
	for(i = 0; i < nblocks; i ++ )
	{
		if(count_label[i] > 0 )
		{
			block_num ++;
		}
	}

	_mae_malloc_uchar(&blocks_data, block_num * sizeof(MAE_block),"_mae_get_label_blocks : blocks");
	*o_blocks = (MAE_block *)blocks_data;
	*o_nblocks = block_num;
	//initialize
	for( i = 0; i < block_num; i ++ )
	{
		(*o_blocks)[i].label = 0;
		(*o_blocks)[i].head = NULL;
		(*o_blocks)[i].tail = NULL;
	}

	idx = 0;
	for(i = 0; i < nblocks; i ++ )
	{
		if(count_label[i] > 0 )
		{
			(*o_blocks)[idx].label = i;
			(*o_blocks)[idx].head = NULL;
			(*o_blocks)[idx].tail = NULL;
			for(k = 0; k < nblocks; k ++ )
			{
				if( blocks[k].label == i )
				{
					_mae_add_node_list(&((*o_blocks)[idx].head), &((*o_blocks)[idx].tail),  blocks[k].head, blocks[k].tail);
				}
			}

			idx++;
		}
	}
	free(count_label);

	return 0;
}

int _mae_detect_blocks(MAE_node* run_list, int listnum, MAE_block **o_blocks, int* o_nblocks)
{
	MAE_block *blocks;
	int nblocks;
	int error;
	error = _mae_cluster_runs_chain(run_list, listnum, &blocks, &nblocks);
	error = _mae_get_label_blocks(blocks, nblocks, o_blocks, o_nblocks);
	free(blocks);
	return error;
}

int _mae_get_block_pixnum(MAE_block *block)
{
	MAE_node *p;
	int pix_num;
	p = block->head;
	pix_num = 0;
	while(p != NULL)
	{
		pix_num = pix_num + p->len;
		p = p->next;
	}
	return pix_num;
}

int _mae_delete_small_blocks(MAE_block *blocks, int nblocks)
{
	int i;
	int pixnum;
	for(i = 0; i < nblocks; i ++ )
	{
		pixnum = _mae_get_block_pixnum(&blocks[i]);

		if(pixnum < MAE_BLOCK_PIXS_THRESHOLD )
		{
			blocks[i].label = -1; //不要了
		}
	}
	return 0;
}
int _mae_delete_small_blocks3(MAE_block *blocks, int nblocks, int flag)
{
	int i;
	int pixnum;
	int maxw = 0;
	int maxh = 0;
	if( flag == 0 )
	{
		for(i = 0; i < nblocks; i ++ )
		{
			if( maxh < blocks[i].h)
			{
				maxh = blocks[i].h;
			}
		}
	}else{
		for(i = 0; i < nblocks; i ++ )
		{
			if( maxw < blocks[i].w)
			{
				maxw = blocks[i].w;
			}
		}
	}
	for(i = 0; i < nblocks; i ++ )
	{
		if( flag == 0 )
		{
			if( blocks[i].h < maxh / 2 )
			{
				blocks[i].label = -1; 
			}
		}else{
			if( blocks[i].w < maxw / 2 )
			{
				blocks[i].label = -1; 
			}
		}
		pixnum = _mae_get_block_pixnum(&blocks[i]);

		if(pixnum < MAE_BLOCK_PIXS_THRESHOLD )
		{
			blocks[i].label = -1; 
		}
	}
	return 0;
}

int _mae_delete_small_blocks2(MAE_block *blocks, int nblocks, int threshold)
{
	int i;
	int pixnum;
	for(i = 0; i < nblocks; i ++ )
	{

		if( blocks[i].w < 5 && blocks[i].h < 5 )
		{
			blocks[i].label = -1; 
			continue;
		}
		pixnum = _mae_get_block_pixnum(&blocks[i]);

		if(pixnum < threshold )
		{
			blocks[i].label = -1; 
		}
	}
	return 0;
}


void _mae_computer_blocks_rect(MAE_block **_mae_blocks, int *_nblocks)
{
	int i;
	int num;

	num = *_nblocks;

	for(i = 0; i < num; i ++ )
	{
		_mae_com_block_rect(&((*_mae_blocks)[i]));

	}
	return;
}

void _mae_com_block_rect(MAE_block *block)
{
	//MAE_node *head;
	MAE_node *p;
	int _l,_t,_r,_b;

	_l = 10000;
	_t = 10000;
	_r = 0;
	_b = 0;
	p =  block->head;
	while(p != NULL )
	{
		if( _l > p->x ) _l = p->x;
		if( _r < p->x + p->len ) _r = p->x + p->len;
		if( _t > p->y ) _t = p->y;
		if( _b < p->y  ) _b = p->y;
		p = p->next;
	}		
	block->l = _l;
	block->t = _t;
	block->w = _r-_l;
	block->h = _b-_t;

	return;
}



void _mae_merge_blocks_by_rect(MAE_block *_blocks, int num)
{
	int i,j;
	int block_num;

	//MAE_node *head;
	//MAE_node *p;

	int l1,t1,w1,h1;
	int l2,t2,w2,h2;
	block_num = num;

	for(i = 0; i < block_num; i ++ )
	{
		if( _blocks[i].label == -1 ) continue;
		for(j = 0; j < block_num; j ++ )
		{
			if( j == i ) continue;
			if( _blocks[j].label == -1 ) continue;

			l1 = _blocks[i].l;
			t1 = _blocks[i].t;
			w1 = _blocks[i].w;
			h1 = _blocks[i].h;
			l2 = _blocks[j].l;
			t2 = _blocks[j].t;
			w2 = _blocks[j].w;
			h2 = _blocks[j].h;
			if(_mae_check_block(l1, t1, w1, h1, l2, t2, w2, h2))
			{
				_mae_add_node_list(&(_blocks[i].head), &(_blocks[i].tail), _blocks[j].head, _blocks[j].tail );
				_blocks[j].label = -1;
				_blocks[j].head = NULL;
				_blocks[j].tail = NULL;
				_mae_com_block_rect(&(_blocks[i]));
			}
			else if( _mae_is_link_block( &(_blocks[i]), &(_blocks[j]) ,MAE_MARK_BLOCK_GAP) ){
				_mae_add_node_list(&(_blocks[i].head), &(_blocks[i].tail), _blocks[j].head, _blocks[j].tail );
				_blocks[j].label = -1;
				_blocks[j].head = NULL;
				_blocks[j].tail = NULL;
				_mae_com_block_rect(&(_blocks[i]));
			}
		}
	}
	return;	
}

int  _mae_is_link_block(MAE_block *block1, MAE_block *block2, int gap)
{
	MAE_node *node1;
	MAE_node *node2;

	node1 = block1->head;
	node2 = block2->head;
	int dist;
	int min_dist = 1000;

	while(node1 != NULL )
	{
		while(node2 != NULL )
		{
			dist = _mae_distance(node1,node2);
			if(min_dist > dist )
			{
				min_dist = dist;
			}
			node2 = node2->next;
		}
		node1 = node1->next;
	}

	if (min_dist < gap )
	{
		return 1;
	}
	return 0;
	//int h_l;
	//int h_t;
	//int h_r;
	//int h_b;
	//int v_l;
	//int v_t;
	//int v_r;
	//int v_b;
	//h_l = region1->l;
	//h_t = region1->t;
	//h_r = region1->l + region1->w;
	//h_b = region1->t + region1->h;
	//v_l = region2->l;
	//v_t = region2->t;
	//v_r = region2->l + region2->w;
	//v_b = region2->t + region2->h;
	//if(    abs(v_t - h_t) < MAE_MARK_LINE_GAP 
	//	|| abs(v_t - h_b) < MAE_MARK_LINE_GAP )
	//{
	//	return 1;
	//}
	//if(    abs(v_b - h_t) >= MAE_MARK_LINE_GAP 
	//	&& abs(v_b - h_b) >= MAE_MARK_LINE_GAP 
	//	&&  h_t > v_t 
	//	&&  h_t < v_b 
	//	&&  h_b > v_t 
	//	&&  h_b < v_b 
	//	)
	//{
	//	if(    abs(v_l - h_l) < MAE_MARK_LINE_GAP 
	//		|| abs(v_r - h_l) < MAE_MARK_LINE_GAP )
	//	{
	//		return 1;
	//	}
	//	if(    abs(v_l - h_r) < MAE_MARK_LINE_GAP 
	//		|| abs(v_r - h_r) < MAE_MARK_LINE_GAP )
	//	{
	//		return 1;
	//	}
	//}
	//if(    abs(v_b - h_t) < MAE_MARK_LINE_GAP 
	//	|| abs(v_b - h_b) < MAE_MARK_LINE_GAP )
	//{
	//	return 1;
	//}

	//if(    abs(v_t - h_t) >= MAE_MARK_LINE_GAP 
	//	&& abs(v_t - h_b) >= MAE_MARK_LINE_GAP 
	//	&&  h_t > v_t 
	//	&&  h_t < v_b 
	//	&&  h_b > v_t 
	//	&&  h_b < v_b 
	//	)
	//{
	//	if(    abs(v_l - h_l) < MAE_MARK_LINE_GAP 
	//		|| abs(v_r - h_l) < MAE_MARK_LINE_GAP )
	//	{
	//		return 1;
	//	}
	//	if(    abs(v_l - h_r) < MAE_MARK_LINE_GAP 
	//		|| abs(v_r - h_r) < MAE_MARK_LINE_GAP )
	//	{
	//		return 1;
	//	}
	//}
	//return 0;
}


int _mae_check_block(int l1, int t1, int w1, int h1, int l2, int t2, int w2, int h2)
{
	int left1,top1, right1, bottom1;
	int left2,top2, right2, bottom2;
	int i;
	for( i = 0; i < 2; i ++ )
	{
		if( i == 0 )
		{
			left1 = l1 -1;
			top1 = t1  - 1;
			right1 = l1 + w1 + 1;
			bottom1 = t1 + h1 + 1;

			left2 = l2 - 1;
			top2 = t2 - 1;
			right2 = l2 + w2 + 1;
			bottom2 = t2 + h2 + 1;
		}else{
			left1 = l2 - 1;
			top1 = t2 - 1;
			right1 = l2 + w2 + 1;
			bottom1 = t2 + h2 + 1;

			left2 = l1 - 1;
			top2 = t1 - 1;
			right2 = l1 + w1 + 1;
			bottom2 = t1 + h1 + 1;
		}
		if(    left1 >= left2 
			&& left1 <= right2 
			&& top1 >= top2 
			&& top1 <= bottom2 
			)
		{
			return 1;
		}
		if(    right1 >= left2 
			&& right1 <= right2 
			&& top1 >= top2 
			&& top1 <= bottom2 
			)
		{
			return 1;
		}
		if(    left1 >= left2 
			&& left1 <= right2 
			&& bottom1 >= top2 
			&& bottom1 <= bottom2 
			)
		{
			return 1;
		}
		if(    right1 >= left2 
			&& right1 <= right2 
			&& bottom1 >= top2 
			&& bottom1 <= bottom2 
			)
		{
			return 1;
		}
	}

	return 0;
}


int _mae_compute_line_width_fromblocks(MAE_block *blocks, int nblocks)
{
	int i;
	int pixnum;
	int width;
	width = 0;
	for(i = 0; i < nblocks; i ++ )
	{
		if( blocks[i].w > MAE_MAX_LINE_WIDHT )
		{
			blocks[i].label = -1;
		}
		if( blocks[i].h < MAE_SPLIT_IMAGE_STEP-4)
		{
			blocks[i].label = -1;
		}
	}
	for(i = 0; i < nblocks; i ++ )
	{
		if( blocks[i].label == -1)
		{
			continue;
		}
		if( width < blocks[i].w)
		{
			width = blocks[i].w;
		}
	}
	if( width < 10 ) width = 10;
	if( width > MAE_MAX_LINE_WIDHT ) width = MAE_MAX_LINE_WIDHT;
	return width;
}