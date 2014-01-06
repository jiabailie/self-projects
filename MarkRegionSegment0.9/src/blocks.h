#pragma once
#include "maedefs.h"
void _mae_create_runs_chain(MAE_node ** _runlist_, int *listnum, int *rx, int *ry, int *rlen, int nruns);
void _mae_create_runs_chain_split(MAE_node ** _runlist_, int *listnum,int *rx, int *ry, int *rlen, int nruns);
int _mae_cluster_runs_chain(MAE_node* run_list, int listnum, MAE_block **blocks, int* nblocks);
int  _mae_runs_near_chain(MAE_node *p,MAE_node *q );
void _mae_run_add_new_block(MAE_node* node, MAE_block **blocks, int* ralloc, int* nblocks, int label);
void _mae_realloc_blocks(MAE_block **blocks, int *ralloc, int incr);
//void _mae_destroy_blocks(MAE_block* blocks, int nblocks);
void _mae_destroy_blocks(MAE_block* blocks);

MAE_node*  _mae_detect_run_by_prevrow(MAE_node* pr_head,MAE_node* node);
void _mae_run_add_block(MAE_node *node, MAE_block **blocks, int* nblocks, MAE_node* d_node);
void _mae_become_other_block_label(MAE_node* pr_head,MAE_node* d_node, MAE_block **blocks, int *nblocks, MAE_node *node);
void _mae_alloc_blocks(MAE_block **blocks, int *allock);

int _mae_get_label_blocks(MAE_block *i_blocks, int i_nblocks, MAE_block **o_blocks, int *o_nblocks);
int _mae_detect_blocks(MAE_node* run_list, int listnum, MAE_block **o_blocks, int* o_nblocks);

//block process
int _mae_get_block_pixnum(MAE_block *blocks);
int _mae_delete_small_blocks(MAE_block *blocks, int nblocks);
int _mae_delete_small_blocks2(MAE_block *blocks, int nblocks, int threshold);

void _mae_computer_blocks_rect(MAE_block **_mae_blocks, int *_nblocks);
void _mae_com_block_rect(MAE_block *block);

void _mae_merge_blocks_by_rect(MAE_block *_blocks, int num);
int _mae_check_block(int l1, int t1, int w1, int h1, int l2, int t2, int w2, int h2);
int  _mae_is_link_block(MAE_block *block1, MAE_block *block2, int gap);
int _mae_delete_small_blocks3(MAE_block *blocks, int nblocks, int flag);
int _mae_compute_line_width_fromblocks(MAE_block *blocks, int nblocks);
