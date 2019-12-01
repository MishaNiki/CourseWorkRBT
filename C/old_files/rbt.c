#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef int TYPE;
#define compLT(a,b) (a < b)
#define compEQ(a,b) (a == b)

typedef enum {BLACK, RED} node_color;

typedef struct node_ {
	struct node_ *left;
	struct node_ *right;
	struct node_ *parent;
	node_color color;
	TYPE data;
} node;

#define NIL &sentinel /*создание NIL узлов, ими являются все листья*/
node sentinel = {NIL, NIL, 0, BLACK, 0};

node *root  = NIL;

TYPE *input_array, *delete_array; 	//массив ввода данных
size_t size_ia, size_da; 			//размер входного массива

void array_filling (int idx_path, int test) {

	char path[3][64] = {"../test/rand/", "../test/sort/", "../test/swap/"};
	char name_input_file[] 	= "input";
	char name_delete_file[] = "delete";
	char buff_input_file[64], buff_delete_file[64];

	FILE *fin, *fdel;
	snprintf(buff_input_file,  63, "%s%s%d", path[idx_path], name_input_file, test);
	snprintf(buff_delete_file, 63, "%s%s%d", path[idx_path], name_delete_file, test);
	fin  = fopen(buff_input_file, "r");
	fdel = fopen(buff_delete_file, "r");

	if(fin == NULL || fdel == NULL){
		fprintf(stderr, "FILE not found!!!\n");
		exit(1);
	}

	fscanf(fin, "%d", &size_ia);
	fscanf(fdel, "%d", &size_da);

	input_array  = (TYPE*)malloc(size_ia * sizeof(TYPE)); 
	delete_array = (TYPE*)malloc(size_da * sizeof(TYPE));

	for(int i = 0; i < size_ia; i++)
		fscanf(fin, "%d", &input_array[i]);

	for(int i = 0; i < size_da; i++)
		fscanf(fdel, "%d", &delete_array[i]);


}

void rotate_right(node *x) {

	node *y = x->left;
	x->left = y->right;
	if(y->right != NIL) y->right->parent = x;
	if(y != NIL) y->parent = x->parent;
	if(x->parent) {
		if(x == x->parent->right)
			x->parent->right = y;
		else
			x->parent->left = y;
	} else {
		root = y;
	}

	y->right = x;
	if(x != NIL) x->parent = y;

}

void rotate_left(node *x) {

	node *y = x->right;
	x->right = y->left;
	if(y->left != NIL) y->left->parent = x;
	if(y != NIL) y->parent = x->parent;
	if (x->parent) {
		if(x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;
	} else {
		root = y;
	}
	
	y->left = x;
	if (x != NIL) x->parent = y;

}

void insert_fixup(node *x) {

	/*Балансируем дерево после вставки x*/

	while(x != root && x->parent->color == RED) {
		/*Дерево не подходит по Красно-Чёрному признаку*/
		if(x->parent == x->parent->parent->left){
			
			node *y = x->parent->parent->right;
			if(y->color == RED) {
				/*дядя красный*/
				x->parent->color = BLACK;
				y->color = BLACK;
				x->parent->parent->color = RED;
				x = x->parent->parent;
			} else {
				/*дядя чёрный*/
				if(x == x->parent->right){
					x = x->parent;
					rotate_left(x);
				}

				x->parent->color = BLACK;
				x->parent->parent->color = RED;
				rotate_right(x->parent->parent);
			}
		} else {
			
			node *y = x->parent->parent->left;
			if(y->color == RED) {
				/*дядя красный*/
				x->parent->color = BLACK;
				y->color = BLACK;
				x->parent->parent->color = RED;
				x = x->parent->parent;
			} else {
				/*дядя чёрный*/
				if(x == x->parent->left) {
					x = x->parent;
					rotate_right(x);
				}

				x->parent->color = BLACK;
				x->parent->parent->color = RED;
				rotate_left(x->parent->parent);
			}
		}
	}

	root->color = BLACK;
}

node *insert_node(TYPE data) {

	node *current, *parent, *x;
	
	current = root;
	parent = NULL;
	while(current != NIL){
		if(compEQ(data, current->data)) return (current);
		parent = current;
		current = compLT(data, current->data) ?
			current->left : current->right;NIL
	}

	if((x = malloc(sizeof(*x))) == 0) {
		
		printf ("insufficient memory (insertNode)\n");
		exit(1);
	}

	x->data = data;
	x->parent = parent;
	x->left = NIL;
	x->right = NIL;
	x->color = RED;

	if(parent){
		if(compLT(data, parent->data))
			parent->left = x;
		else
			parent->right = x;
	} else {
		root = x;
	}

	insert_fixup(x);
	return x;

}

void delete_fixup(node *x) {


	while(x != root && x->color == BLACK) {
		if(x == x->parent->left) {
			
			node *w = x->parent->right;
			if(w->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;
				rotate_left(x->parent);
				w = x->parent->right;
			}
			
			if(w->left->color == BLACK && w->right->color == BLACK) {
				w->color = RED;
				x = x->parent;
            } else {
				
				if(w->right->color == BLACK) {
				
					w->left->color = BLACK;
					w->color = RED;
					rotate_right(w);
					w = x->parent->right;
                }
				
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->right->color = BLACK;
				rotate_left (x->parent);
				x = root;
			}
		} else {
			
			node *w = x->parent->left;
			
			if(w->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;
				rotate_right(x->parent);
				w = x->parent->left;
			}
			
			if (w->right->color == BLACK && w->left->color == BLACK) {
				
				w->color = RED;
				x = x->parent;
			} else {
				
				if(w->left->color == BLACK) {
				
					w->right->color = BLACK;
					w->color = RED;
					rotate_left(w);
					w = x->parent->left;
				}
				
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->left->color = BLACK;
				rotate_right(x->parent);
				x = root;
			}
		}
	}
	x->color = BLACK;
}

void delete_node(node *z) {

	node *x, *y;

	if(!z || z == NIL) return;

	if(z->left == NIL || z->right == NIL) {
		y = z;
	} else {

		y = z->right;
        while(y->left != NIL) y = y->left;
	}

	if(y->left != NIL)
		x = y->left;
	else
		x = y->right;

	x->parent = y->parent;
	if(y->parent)
		if(y == y->parent->left)
			y->parent->left = x;
		else
			y->parent->right = x;
	else
		root = x;

	if(y != z) z->data = y->data;

	if(y->color == BLACK)
		delete_fixup (x);

	free(y);
}

node *find_node(TYPE data) {

	node *current = root;
	while(current != NIL)
		if(compEQ(data, current->data))
			return current;
		else
			current = compLT(data, current -> data) ?
				current->left : current->right;

	return NULL;
}

void delete_tree(node *tmp){
	if(tmp != NIL){
		delete_tree(tmp -> left);
		delete_tree(tmp -> right);
		free(tmp);
	}
}

int main () {
	
	clock_t start, end, result[10];
	node *t;

	char output_path[3][64] = {"../result/C/rand", "../result/C/sort", "../result/C/swap"};

	for(int k = 0; k < 3; k++){
		for(int i = 0; i < 10; i++){

			array_filling(k, i);
			start = clock();
			
			for(int j = 0; j < size_ia; j++)
				insert_node(input_array[j]);

			for(int j = 0; j < size_da; j++)
				if((t = find_node(delete_array[j])) != NULL)
					delete_node(t);

			delete_tree(root);

			end = clock();
			result[i] = end - start;
			printf("time = %d\tk = %d\ti = %d\n", result[i]/CLOCKS_PER_SEC, k, i);
			
			free(input_array);
			free(delete_array);
		}

		FILE *fout = fopen(output_path[k], "r");
		if(fout == NULL){
			fprintf(stderr, "FILE not found!!!\n");
			exit(1);
		}

		for(int i = 0; i < 10; i++)
			fprintf(fout, "%d%d\n", result[i]/CLOCKS_PER_SEC);
		fclose(fout);

	}
	
	return 0;
}

/*-------------------------------------------------------*
 ¦ 	unsigned int start_time =  clock();					 ¦
 ¦	....................................				 ¦
 ¦	unsigned int end_time = clock();					 ¦	
 ¦	unsigned int search_time = end_time - start_time; 	 ¦
 ¦	fprintf(stdout, "%d\n", search_time);				 ¦
 *------------------------------------------------------*/