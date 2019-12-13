#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "rbtree.h"

/*
-------------------------------------------------------------------------
	compare - cравнение элементов
	int compare(const void* x1, const void*  x2)
	возвращает:
		0 - если элементы равны
		отрицательное - если x1 < x2
		положительное - если x1 > x2
-------------------------------------------------------------------------
	print_data - вывод данных
	пример:
		void *print_data(const void* data) {
			printf("%d\n", *(type*)data);
		}
-------------------------------------------------------------------------
	data_to_string - перевод данных к строчному типу
	пример:
		void data_to_string(const void *x, char *data) {
			snprintf(data,  sizeof(data) - 1, "%d", *(int*)x, data);
		}
-------------------------------------------------------------------------
*/

/*
	Определение цвета узла
*/
rbnode_color node_color(rbnode *x) {
	if(x == NULL)
		return BLACK;
	return x->color;
}

/*
	Поиск дедушки
*/
rbnode *grandparen(rbnode *x) {
	if(x == NULL || x->parent == NULL)
		return NULL;
	return x->parent->parent;
}

/*
	Поиск дяди
*/
rbnode *uncle(rbnode *x) {
	rbnode *g = grandparen(x);
	if(g == NULL)
		return NULL;
	if(x->parent == g->left)
		return g->right;
	else
		return g->left;
}

/*
	Поиск брата
*/
rbnode *sibling(rbnode *x) {

	if(x->parent == NULL)
		return NULL;
	if(x == x->parent->left)
		return x->parent->right;
	else
		return x->parent->left;
}

/*
	Замена узлов
*/
void replace_rbnode(rbtree *tree,rbnode *x, rbnode *child) {
	if(x->parent == NULL)
		tree->root = child;
	if(child == NULL)
		return;
	child->parent = x->parent;
	if(x == x->parent->left)
		x->parent->left = child;
	else
		x->parent->right = child;
}

/*
	Максимальный элемент в дереве(поддереве)
*/
rbnode *maximum_node(rbnode *x) {
	if(x == NULL){
		return NULL;
	}
	while (x->right != NULL) {
		x = x->right;
	}
	return x;
}

/*
	Левый поворот
*/
void rotate_left(rbtree *tree, rbnode *x) {

	if(x == NULL || x->right == NULL) return; //причины по которым невозможен поворот

	rbnode *y = x->right;
	x->right = y->left;
	if(y->left != NULL) y->left->parent = x;
	y->parent = x->parent;
	
	if(x->parent != NULL){
		if(x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;
	} else {
		tree->root = y;
	}
	y->left = x;
	x->parent = y;
}

/*
	Правый поворот
*/
void rotate_right(rbtree *tree, rbnode *x) {

	if(x == NULL || x->left == NULL) return; //причины по которым невозможен поворот

	rbnode *y = x->left;
	x->left = y->right;
	if(y->right != NULL) y->right->parent = x;
	y->parent = x->parent;

	if(x->parent != NULL){
		if(x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;
	} else {
		tree->root = y;
	}
	y->right = x;
	x->parent = y;	
}


/*
	Присваивание
*/
void assignment(void *a, void *b, size_t size_type) {
	char *_a = (a), *_b = (b);
	do {
		*_a = *_b;
		_a++; _b++;
	} while(--size_type > 0);
}

/*
	Вставка в дерево нового элемента
	Возращает указатель на вставленный элемент

*/

void insert_case5(rbtree *tree, rbnode *x){
	rbnode *g = grandparen(x);
	x->parent->color = BLACK;
	g->color = RED;
	if(x == x->parent->left && x->parent == g->left)
		rotate_right(tree, g);
	else
		rotate_left(tree, g);
}

void insert_case4(rbtree *tree, rbnode *x){
	rbnode *g = grandparen(x);
	if(x == x->parent->right && x->parent == g->left) {
		rotate_left(tree, x->parent);
		x = x->left;
	} else if(x == x->parent->left && x->parent == g->right) {
		rotate_right(tree, x->parent);
		x = x->right;
	}
	insert_case5(tree, x);

}

void insert_case1(rbtree *tree, rbnode *x);
void insert_case3(rbtree *tree, rbnode *x) {
	rbnode *u = uncle(x), *g;
	if(node_color(u) == RED) {
		x->parent->color = BLACK;
		u->color = BLACK;
		g = grandparen(x);
		g->color = RED;
		insert_case1(tree, g);
	}else {
		insert_case4(tree, x);
	}
}

void insert_case2(rbtree *tree, rbnode *x) {
	if(x->parent->color == BLACK)
		return;
	else
		insert_case3(tree, x);
}

void insert_case1(rbtree *tree, rbnode *x) {
	if(x->parent == NULL)
		x->color = BLACK;
	else
		insert_case2(tree, x);
}

rbnode *insert_rbnode(rbtree *tree, void *data, size_t size_type) {

	rbnode *current = tree->root, *parent = NULL, *x;

	while(current != NULL){
		if(tree->comparator(data, current->data) == 0) return current;
		parent = current;
		current = tree->comparator(data, current->data) < 0 ?
			current->left : current->right;
	}

	if((x = malloc(sizeof(*x))) == 0) {
		printf ("insufficient memory (insertNode)\n");
		exit(1);
	}

	if((x->data = malloc(size_type)) == 0){
		printf ("insufficient memory (insertNode)\n");
		exit(1);
	}

	assignment(x->data, data, size_type);
	x->parent 	= parent;
	x->left		= NULL;
	x->right	= NULL;
	x->color	= RED;

	if(parent != NULL) {
		if(tree->comparator(data, parent->data) < 0)
			parent->left = x;
		else
			parent->right = x;
	} else {
		tree->root = x;
	}

	insert_case1(tree, x);

	return x;
}

/*
	Восстановление красно-чёрных свойств дерева
	после удаления узла
*/
void delete_case6(rbtree *tree, rbnode *x) {
	
	rbnode *sib = sibling(x);
	sib->color = x->parent->color;
	x->parent->color = BLACK;
	if(x == x->parent->left) {
		sib->right->color = BLACK;
		rotate_left(tree, x->parent);
	} else {
		sib->left->color = BLACK;
		rotate_right(tree, x->parent);
	}
}

void delete_case5(rbtree *tree, rbnode *x) {
	
	rbnode *sib = sibling(x);
	if(sib != NULL && node_color(sib) == BLACK) {
		
		if(x == x->parent->left && node_color(sib->right) == BLACK && node_color(sib->left) == RED) {
			sib->color = RED;
			sib->left->color = BLACK;
			rotate_right(tree, sib);
		} else if(x == x->parent->right && node_color(sib->right) == RED && node_color(sib->left) == BLACK) {
			sib->color = RED;
			sib->right->color = BLACK;
			rotate_left(tree, sib);
		}
	}
	delete_case6(tree, x);
}

void delete_case4(rbtree *tree, rbnode *x) {
	
	rbnode *sib = sibling(x);
	if(sib != NULL && node_color(x->parent) == RED && node_color(sib) == BLACK && 
		node_color(sib->left) == BLACK && node_color(sib->right) == BLACK) {
		sib->color = RED;
		x->parent->color = BLACK;
	} else {
		delete_case5(tree, x);
	}
}

void delete_case1(rbtree *tree, rbnode *x);
void delete_case3(rbtree *tree, rbnode *x) {
	
	rbnode *sib = sibling(x);
	if(sib != NULL && node_color(x->parent) == BLACK && node_color(sib) == BLACK && 
		node_color(sib->left) == BLACK && node_color(sib->right) == BLACK) {
		sib->color = RED;
		delete_case1(tree, x->parent);
	} else {
		delete_case4(tree, x);
	}
}

void delete_case2(rbtree *tree, rbnode *x) {
	rbnode *sib = sibling(x);
	if(node_color(sib) == RED) {
		x->parent->color = RED;
		sib->color = BLACK;
		if(x == x->parent->left)
			rotate_left(tree, x->parent);
		else
			rotate_right(tree, x->parent);
	}
	delete_case3(tree, x);
}

void delete_case1(rbtree *tree, rbnode *x) {
	if(x->parent != NULL)
		delete_case2(tree, x);
}

/*
	Удаление узла дерева
*/
void delete_rbnode(rbtree *tree, rbnode *x) {

	if(x == NULL)
		return;

	if(x->left != NULL && x->right != NULL) {
		rbnode *pred = maximum_node(x->left);
		free(x->data);
		x->data = pred->data;
		x = pred;
	} else {
		free(x->data);
	}

	if(x->left == NULL || x->right == NULL) {
		rbnode *child;
		if(x->right == NULL)
			child = x->left;
		else
			child = x->right;
		if(x->color = BLACK) {
			x->color = node_color(child);
			delete_case1(tree, x);
		}
		replace_rbnode(tree, x, child);
		if(x->parent == NULL && child != NULL) {
			child->color = BLACK;
		}
	}
	free(x);
}

/*
	Удаление дерева
	После выполнения: *tree == NULL
*/
void del_rbt(rbnode *x);

void delete_rbtree(rbtree *tree) {
	del_rbt(tree->root);
	free(tree);
}

void del_rbt(rbnode *x) {
	
	if(x != NULL) {		
		del_rbt(x->left);
		del_rbt(x->right);
		free(x->data);
		free(x);
	}
}

/*
	Вывод содержимого дерева в консоль
	в отсортированном виде 
*/
void pt_rbt(rbnode *x, void (*print_data)(const void*));

void print_rbtree(rbtree *tree, void (*print_data)(const void*)) {
	printf("Дерево\n");
	if(tree != NULL){
		pt_rbt(tree->root, print_data);
	}
	printf("\n");
}

void pt_rbt(rbnode *x, void (*print_data)(const void*)) {

	if(x != NULL) {
		pt_rbt(x->left, print_data);
		print_data(x->data);
		pt_rbt(x->right, print_data);
	}
}

/*
 	Поиск элемента по данным в дереве

*/
rbnode *find_rbnode(rbtree *tree, void *data) {

	rbnode *current = tree->root;
	while(current != NULL)
		if(tree->comparator(data, current->data) == 0)
			return current;
		else
			current = tree->comparator(data, current->data) < 0 ?
				current->left : current->right;

	return NULL;
}

/*
	Создание дерева
*/
rbtree *create_rbtree(int (*comparator)(const void*, const void*)) {

	rbtree *tree;
	if((tree = malloc(sizeof(*tree))) == 0) {
		printf ("insufficient memory (insertNode)\n");
		exit(1);
	}
	tree->root = NULL;
	tree->comparator = comparator;
	return tree;
}


/*
	Определение максимальной глубины дерева
*/

int max_deep_rbtree(rbnode *x){
	
	if(x == NULL) return 0;
	int x1 = max_deep_rbtree(x->left);
	int x2 = max_deep_rbtree(x->right);
	return (x1 > x2 ? x1 + 1: x2 + 1);
}

int deep_rbtree(rbtree *tree){
	return max_deep_rbtree(tree->root);
}


/*
	Определение количества узлов в дереве
*/

int amount_rbnode(rbnode *x){
	if(x == NULL) return 0;
	return 1 + amount_rbnode(x->left) + amount_rbnode(x->right);
}

int size_rbtree(rbtree *tree) {
	return amount_rbnode(tree->root);
}

/*
	Отрисовка дерева в поток FILE
*/
void draw(rbnode *x, char *prefix, bool tail, char *str, 
		char *data, int lenght, void (*data_to_string)(const void*, char*)) {

	if(x->right != NULL){
		char *new_prefix = (char*)malloc((lenght) * sizeof(char));
		strcpy(new_prefix, prefix);
		if(tail){
			strcat(new_prefix, "│   ");
		} else {
			strcat(new_prefix, "    ");
		}
		draw(x->right, new_prefix, false, str, data, lenght, data_to_string);
		free(new_prefix);
	}
	strcat(str, prefix);
	if(tail){
		strcat(str, "└── ");
	}else{
		strcat(str, "┌── ");
	}
	memset(data, 0, sizeof(data));
	data_to_string(x->data, data);
	if(x->color == RED){
		strcat(data, "-R");
	} else {
		strcat(data, "-B");
	}
	strcat(str, data);
	strcat(str, "\n");
	
	if(x->left != NULL){
		char *new_prefix = (char*)malloc((lenght) * sizeof(char));
		strcpy(new_prefix, prefix);
		if(tail){
			strcat(new_prefix, "    ");
		} else {
			strcat(new_prefix, "│   ");	
		}
		draw(x->left, new_prefix, true, str, data, lenght, data_to_string);
		free(new_prefix);
	}
}
/*
	Выделение памяти и подготовка к отрисовке дерева
	определения размера строки в которую будет помещаться рисунок
	выделение и чиска памяти

-------------------------------------------------------------------------
	draw_rbtree
		tree - дерево которое нужно отрисовать
		stream - поток вывода
		max_size_data - максимальная длина строки из числа
		data_to_string - 	
-------------------------------------------------------------------------

*/
void draw_rbtree(rbtree *tree, FILE *stream, 
		int max_size_data, void (*data_to_string)(const void*, char*)) {

	int lenght = max_size_data + (deep_rbtree(tree) + 1) * 4; // максимальный размер одной строки в рисунке дерева
	int count_node = size_rbtree(tree); //количество строк в рисунке дерева
	char *str = (char*)malloc(lenght * count_node * sizeof(char));
	char s[2]; // пустая строка
	memset(str, 0, sizeof(str)); //чистка, при частом вызове строка бывает заполнена мусором 
	char *data = (char*)malloc((max_size_data + 3) * sizeof(char));
	draw(tree->root, s, true, str, data, lenght, data_to_string);
	fprintf(stream, "%s\n", str);
	free(str);
	free(data);
}
/*
	заменить к чёртовой матери это долбанное удаление она здесь нафиг не нужно
*/ 
