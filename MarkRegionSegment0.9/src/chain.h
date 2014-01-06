#pragma once
#include "maedefs.h"

MAE_node* _mae_create_link_list(int n);

//”Œ≥Ã¡¥±˙Î‡πÿ
void _mae_add_node(MAE_node ** head, MAE_node **tail, MAE_node* node );
void _mae_add_node_list(MAE_node ** head, MAE_node **tail, MAE_node* head2, MAE_node * tail2 );

void _mae_insert_node(MAE_node ** head, MAE_node* cur_node, MAE_node* new_node);

int _mae_delete_node(MAE_node **head, MAE_node* new_node);
void _mae_add_node_link(MAE_node ** head, MAE_node* node );

//øÈ¡¥±˙Î‡πÿ
void _mae_add_block(MAE_block ** head, MAE_block **tail, MAE_block* node );
int _mae_delete_block(MAE_block **head, MAE_block* new_node);
void _mae_add_region(MAE_region ** head, MAE_region* node);
//void _mae_add_v_region(MAE_region ** head, MAE_region* node);
int _mae_distance(MAE_node *node1, MAE_node *node2);

void _mae_add_line_node(MAE_line_node ** head, MAE_line_node* node);
int _mae_delete_line_node(MAE_line_node **head, MAE_line_node* new_node);

void _mae_add_line_h_link(MAE_line_node ** head, MAE_line_node* node);
void _mae_add_line_v_link(MAE_line_node ** head, MAE_line_node* node);

//void _mae_add_line_node_h_link(MAE_line_node * head, MAE_line_node* node);
