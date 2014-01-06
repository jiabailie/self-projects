#include "stdafx.h"
#include <stdlib.h>
#include "chain.h"
#include <math.h>
MAE_node* _mae_create_link_list(int n)
{
	int i;
	MAE_node * head;
	MAE_node *p;
	MAE_node *q;
	if(n == 0) return NULL;
	head = (MAE_node*)malloc(sizeof(MAE_node));
	p = head;
	for(i = 1; i < n; i ++ )
	{
		q = (MAE_node*)malloc(sizeof(MAE_node));
		p->next = q;
		p = q;
	}
    p->next = NULL;
	return head;
}


void _mae_add_node(MAE_node ** head, MAE_node **tail, MAE_node* node )
{
	MAE_node *q;
	q = node;
	if( *head == NULL )
	{
		*head = q;
		*tail = q;
	}else{
		(*tail)->next = q;
		q->prev = (*tail);
		*tail = q;
	}
}
void _mae_add_node_link(MAE_node ** head, MAE_node* node )
{
	MAE_node *q;
	MAE_node *p;
	p = *head;
	q = node;
	if( p == NULL )
	{
		*head = q;
	}else{
		while(p->link != NULL )  
		{
			p = p->link;
		}
		p->link = q;
		q->link = NULL;
	}
}

void _mae_add_node_list(MAE_node ** head, MAE_node **tail, MAE_node* head2, MAE_node * tail2 )
{
	//MAE_node *q,*p;
	if( *head == NULL )
	{
		*head = head2;
		*tail = tail2;
	}else{
		(*tail)->next = head2;
		head2->prev = (*tail);
		*tail = tail2;
	}
}

void _mae_insert_node(MAE_node ** head, MAE_node* cur_node, MAE_node* new_node)
{
	MAE_node *p, *q;
	q = new_node;
	if(*head == NULL )
	{
		*head = q;
	}else{
		p = *head;
		while((p != cur_node) && (p->next != NULL) )
		{
			p = p->next;
		}
		q->next = p->next ;
		q->prev = p;
		p->next = q;

	}

}

int _mae_delete_node(MAE_node **head, MAE_node* node)
{
	MAE_node *p, *q;
	p = NULL;
	q = *head;
	if(q == NULL ) return 1;
	if(q == node) 
	{
		*head = q->next;
		(*head)->prev = NULL;
		free(q);
		return 0;
	}else{
		while((q != node ) && q->next != NULL )
		{
			p = q;
			q = q->next;
		}
		if( q == node )
		{
			p->next = q->next;
			q->next->prev = p;
			free(q);
			return 0;
		}else{
			return 1;
		}
	}
}

//¿éÁ´±úëà¹Ø
void _mae_add_block(MAE_block ** head, MAE_block **tail, MAE_block* node )
{
	MAE_block *q;
	q = node;
	if( *head == NULL )
	{
		*head = q;
		*tail = q;
	}else{
		(*tail)->next = q;
		*tail = q;
	}
}

int _mae_delete_block(MAE_block **head, MAE_block* node)
{
	MAE_block *p, *q;
	p = NULL;
	q = *head;
	if(q == NULL ) 
	{
		return 1;
	}
	if(q == node) 
	{
		*head = q->next;
		free(q);
		return 0;
	}else{
		while( (q != node ) && q->next != NULL )
		{
			p = q;
			q = q->next;
		}
		if( q == node )
		{
			p->next = q->next;
			free(q);
			return 0;
		}else{
			return 1;
		}
	}
}

void _mae_add_region(MAE_region ** head, MAE_region* node)
{
	MAE_region *q;
	MAE_region *p;
	p = *head;
	q = node;
	if( p == NULL )
	{
		*head = q;
	}else{
		while(p->link != NULL )  
		{
			p = p->link;
		}
		p->link = q;
		q->link = NULL;
	}
}
//
//void _mae_add_v_region(MAE_region ** head, MAE_region* node)
//{
//	MAE_region *q;
//	MAE_region *p;
//	p = *head;
//	q = node;
//	if( p == NULL )
//	{
//		*head = q;
//	}else{
//		while(p->v_link != NULL )  
//		{
//			p = p->v_link;
//		}
//		p->v_link = q;
//		q->v_link = NULL;
//	}
//}

int _mae_distance(MAE_node *node1, MAE_node *node2)
{
	int dist = 0; 
	int x1,y1,x2,y2;
	int x3,y3,x4,y4;
	x1 = node1->x;
	y1 = node1->y;
	x2 = x1 + node1->len;
	y2 = y1;
	x3 = node2->x;
	y3 = node2->y;
	x4 = x3 + node2->len;
	y4 = y3;
	
	if( x1 <= x4 && x1 >= x3 )
	{
		dist = abs(y3 - y1); 
		return dist;
	}
	if( x2 <= x4 && x2 >= x3 )
	{
		dist = abs(y3 - y1); 
		return dist;
	}
	if( x3 <= x2 && x3 >= x1 )
	{
		dist = abs(y3 - y1); 
		return dist;
	}
	if( x4 <= x2 && x4 >= x1 )
	{
		dist = abs(y3 - y1); 
		return dist;
	}
	if( x1 > x4 )
	{
		dist = (int)sqrt((double)((x1 - x4)*(x1 - x4) + (y1 - y4)*(y1 - y4) ) ) ;
		return dist;
	}
	if( x2 < x3 )
	{
		dist = (int)sqrt((double)((x1 - x3)*(x2 - x3) + (y2 - y3)*(y2 - y3)) ) ;
		return dist;
	}
	return 9999;

}

void _mae_add_line_node(MAE_line_node ** head, MAE_line_node* node)
{
	MAE_line_node *q;
	MAE_line_node *p;
	p = *head;
	q = node;
	if( p == NULL )
	{
		*head = q;
	}else{
		while(p->next != NULL )  
		{
			p = p->next;
		}
		p->next = q;
		q->next = NULL;
	}
}

void _mae_add_line_h_link(MAE_line_node ** head, MAE_line_node* node)
{
	MAE_line_node *q;
	MAE_line_node *p;
	p = *head;
	q = node;
	if( p == NULL )
	{
		*head = q;
	}else{
		while(p->h_link != NULL )  
		{
			p = p->h_link;
		}
		p->h_link = q;
		q->h_link = NULL;
	}
}
void _mae_add_line_v_link(MAE_line_node ** head, MAE_line_node* node)
{
	MAE_line_node *q;
	MAE_line_node *p;
	p = *head;
	q = node;
	if( p == NULL )
	{
		*head = q;
	}else{
		while(p->v_link != NULL )  
		{
			p = p->v_link;
		}
		p->v_link = q;
		q->v_link = NULL;
	}
}


int _mae_delete_line_node(MAE_line_node **head, MAE_line_node* node)
{
	MAE_line_node *p, *q;
	p = NULL;
	q = *head;
	if(q == NULL ) 
	{
		return 1;
	}
	if(q == node) 
	{
		*head = q->next;
		free(q);
		return 0;
	}else{
		while( (q != node ) && q->next != NULL )
		{
			p = q;
			q = q->next;
		}
		if( q == node )
		{
			p->next = q->next;
			free(q);
			return 0;
		}else{
			return 1;
		}
	}
}
//
//
//void _mae_add_line_node_h_link(MAE_line_node * node1, MAE_line_node* node2)
//{
//	MAE_line_node *q;
//	MAE_line_node *p;
//	MAE_line_node *prev;
//	MAE_line_node *after;
//	if( node1->x < node2->x )
//	{
//		prev = node1;
//		after = node2;
//	}else{
//		prev = node2;
//		after = node1;
//	}
//	if( prev->r_link == NULL && after->l_link == NULL )
//	{
//		prev->r_link = after;
//		after->l_link = prev;
//	}else if( prev->r_link == NULL && after->l_link != NULL ){
//
//		while(p->next != NULL )  
//		{
//			p = p->next;
//		}
//		p->next = q;
//		q->next = NULL;
//	}
//}