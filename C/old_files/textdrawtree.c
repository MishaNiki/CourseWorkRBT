#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>


// ─ │ ┌ ┐└ ┘ ├ ┤┬ ┴ ┼
/*
                    1000000000_R
            ┌────────────┴────────────┐
       1000000000_R              1000000000_R
     ┌──────┴─────┐            ┌──────┴─────┐
1000000000_R 1000000000_R 1000000000_R 1000000000_R

lenght_data_color / 2 + 1 = 7 
2 ^ deeep * 12
*/
char str[10000000];


typedef struct node_ {
	
	int data;
	struct node_ *parent;
	struct node_ *left;
	struct node_ *right;

} node;

node *ROOT = NULL;

node *insert_node(int data) {

	node *current, *parent, *x;

	current = ROOT;
	parent = NULL;

	while(current != NULL){

		if(data == current->data) return current;
		parent = current;
		current = data < current->data ?
			current->left : current->right;
	}

	if((x = malloc(sizeof(*x))) == 0){

		printf ("insufficient memory (insertNode)\n");
		exit(1);
	}

	x->data 	= data;
	x->parent 	= parent;
	x->left 	= NULL;
	x->right 	= NULL;

	if(parent != NULL){

		if(data < parent->data)
			parent->left = x;
		else
			parent->right = x;
	} else {
		ROOT = x;
	}

	return x;
}

void delete_node(node *z) {

	node *x, *y;

	if(z == NULL) return;

	if(z->left == NULL || z->right == NULL){
		y = z;
	} else {
		y = z->right;
		while(y->left != NULL) y = y->left;
	}

	if(y->left != NULL)
		x = y->left;
	else
		x = y->right;

	if(x != NULL) x->parent = y->parent;

	if(y->parent != NULL){
		if(y == y->parent->left)
			y->parent->left = x;
		else
			y->parent->right = x;
	} else {
		ROOT = x;
		ROOT->parent = NULL;
	}
	if(y != z) z->data = y->data;
	free(y);
}


node *find_node(int data) {

	node *current = ROOT;
	while(current != NULL)
		if(data == current->data)
			return current;
		else
			current = data < current->data ?
				current->left : current->right;

	return NULL;
}

void delete_tree(node *x) {

	if(x != NULL) {
		
		delete_tree(x->left);
		delete_tree(x->right);
		free(x);
	}
}

void print_tree(node *x) {

	if(x != NULL) {

		print_tree(x->left);
		fprintf(stdout, "%d ", x->data);
		print_tree(x->right);
	}
}


int max_deep(node *x) {
	if(x != NULL){
		int t1 = max_deep(x->left);
		int t2 = max_deep(x->right);
		return 1 + (t1 < t2 ? t2 : t1); 
	}
	return 0;
}


void draw(int level, char **holst, node *x){

	// ┴ ─ ┌ ┐

	if(x->left != NULL)
		draw(level + 1; holst, x->left);

	//отрисовка данного узла;
	if(level != 0){
		if(x = x->parent->left){
			if(x->left == NULL){
				//вставка если узел левый листок
				int idx_str_count = level * 2 - 1;
				int idx_row_count = strlen(holst[idx_str_count]);
				
				if(idx_row_count != 0){
				
					holst[idx_str_count][idx_row_count] = ' ';
					idx_row_count++;
				}

				
				//вставка числа

				int data = x->data;
				int l = 0;
				while(data > 0){
					data /= 10;
					l++;
				}
				holst[idx_str_count][l + idx_row_count] = '\0';
				data = x->data;
				int len = l / 2 + 1;
				while(data > 0){
			
					holst[idx_str_count][idx_row_count + l - 1] = data % 10 + '0';
					data /= 10;
					l--;
				}

				//вставка проводов
				idx_row_count += len;
				idx_str_count--;
				
				int idx_end_old_str = holst[idx_str_count]; 
				holst[idx_str_count][idx_row_count] = '┌'
				
				while(idx_end_old_str < idx_row_count) {
					holst[idx_str_count][idx_end_old_str] = ' ';
					idx_end_old_str++;
				}

				idx_row_count++;
				holst[idx_str_count][idx_row_count] = '─';
				idx_row_count++;
				holst[idx_str_count][idx_row_count] = '─';
				idx_row_count++;
				holst[idx_str_count][idx_row_count] = '┴';
				
			} else {

				int idx_str_count = level * 2 - 1; // индекс поиска вставки
				int idx_row_count = strlen(holst[idx_str_count + 1]) - 1;

				//код для левых узлов не я вляющихся листьями

			}
		} else {
			///код для правых узлов
		}
	}

	if(x->right != NULL)
		draw(level + 1; holst, node->right);
}


void draw_text_tree(node *x) {

	int count_str = max_deep(x) * 2 - 1;//количество строк на рисунке
	int row = 100000000;
	char **holst = (char**)malloc(row * sizeof(char*));
	for(int i = 0; i < row; i++){
		holst[i] = (char*)malloc(count_str * sizeof(char));
		for(int j = 0; j < row; j++) holst[i][j] = 0;
	}

}


int main() {

	int a, n;
	node *t;

	fscanf(stdin, "%d", &n);

	srand(time(NULL));
	
	for(int i = 0; i < n; i++){
		a = rand()%100;
		if((t = find_node(a)) == NULL){
			insert_node(a);
		}else{
			delete_node(t);
		}
	}

	printf("%d\n", max_deep(ROOT));
	print_tree(ROOT);
	delete_tree(ROOT);

	return 0;
}