#ifndef _rbtree_h
#define _rbtree_h 

#include <stddef.h>

typedef enum { BLACK, RED } rbnode_color; //type color rbnode

typedef struct rbnode_ {
	struct rbnode_ *left;	//link to left
	struct rbnode_ *right;	//link to right 
	struct rbnode_ *parent;	//link to parent
	rbnode_color color;		//color node
	void* data;				//data
} rbnode;



typedef struct rbtree_ {
	struct rbnode_ *root;
	int (*comparator)(const void*, const void*);
} rbtree;


rbnode *insert_rbnode(rbtree *tree, void *data, size_t size_type);
void delete_rbnode(rbtree *tree, rbnode *z);
void delete_rbtree(rbtree *tree);
void print_rbtree(rbtree *tree, void (*print_data)(const void*));
rbnode *find_rbnode(rbtree *tree, void *data);
rbtree *create_rbtree(int (*compare)(const void*, const void*));
int deep_rbtree(rbtree *tree);
void draw_rbtree(rbtree *tree, FILE *stream, int max_size_data, void (*data_to_string)(const void*, char*));

#endif