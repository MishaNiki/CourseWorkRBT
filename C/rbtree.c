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

rbnode sentinel = {NIL, NIL, NULL, BLACK, NULL}; // инициализация листев дерева

/*
	Левый поворот
*/
void rotate_left(rbtree *tree, rbnode *x) {

	if(x == NIL || x->right == NIL) return; //причины по которым невозможен поворот

	rbnode *y = x->right;
	x->right = y->left;
	if(y->left != NIL) y->left->parent = x;
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

	if(x == NIL || x->left == NIL) return; //причины по которым невозможен поворот

	rbnode *y = x->left;
	x->left = y->right;
	if(y->right != NIL) y->right->parent = x;
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
	Восстановение красно-чёрных свойств дерева
	после вставки узла в дерево
*/
void insert_fixup(rbtree *tree, rbnode *x) {

	while(x != tree->root && x->parent->color == RED){

		if(x->parent == x->parent->parent->left){
			//Отец слева от деда
			rbnode *y = x->parent->parent->right;

			if(y->color == RED) {
				//Дядя красный
				y->color = BLACK;
				x->parent->color = BLACK;
				x->parent->parent->color = RED;
				x = x->parent->parent;
			} else {
				//Дядя чёрный
				if(x == x->parent->right) {
					//Сын справа, левый поворот
					x = x->parent;
					rotate_left(tree, x);
				}
				//Правый поворот
				x->parent->color = BLACK;
				x->parent->parent->color = RED;
				rotate_right(tree, x->parent->parent);

			}
		} else {
			rbnode *y = x->parent->parent->left;
			//Отец справа от деда
			if(y->color == RED) {
				//Дядя красный
				y->color = BLACK;
				x->parent->color = BLACK;
				x->parent->parent->color = RED;
				x = x->parent->parent;
			} else {
				//Дядя чёрный
				if(x == x->parent->left) {
					//Сын слева, правый поворот
					x = x->parent;
					rotate_right(tree, x);				
				}

				x->parent->color = BLACK;
				x->parent->parent->color = RED;
				//Левый поворот
				rotate_left(tree, x->parent->parent);
			}
		}
	}

	tree->root->color = BLACK;//корень всегда нига
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
rbnode *insert_rbnode(rbtree *tree, void *data, size_t size_type) {

	rbnode *current, *parent, *x;

	current = tree->root;
	parent  = NULL;

	while(current != NIL){
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
	x->left		= NIL;
	x->right	= NIL;
	x->color	= RED;

	if(parent != NULL) {
		if(tree->comparator(data, parent->data) < 0)
			parent->left = x;
		else
			parent->right = x;
	} else {
		tree->root = x;
	}

	insert_fixup(tree, x);

	return x;
}

/*
	Восстановление красно-чёрных свойств дерева
	после удаления узла
*/
void delete_fixup(rbtree *tree, rbnode *x) {

	while(x != tree->root && x->color == BLACK) {

		if(x == x->parent->left){
			//x - левый ребёнок
			rbnode *y = x->parent->right; //брат x
			if(y->color == RED){
				//брат красный, левый поворот
				y->color = BLACK;
				x->parent->color = RED;
				rotate_left(tree, x->parent);
				y = x->parent->right;
			}
			if(y->left->color == BLACK && y->right->color == BLACK){
				//дети брата чёрные
				y->color = RED;
				x = x->parent;
			} else {
				if(y->right->color == BLACK) {
					//одинаковый ребёнок чёрного цвета
					//правый поворот
					y->left->color = BLACK;
					y->color = RED;
					rotate_right(tree, y);
					y = x->parent->right;
				}
				y->color = x->parent->color;
				x->parent->color = BLACK;
				y->right->color = BLACK;
				rotate_left(tree, x->parent);
				x = tree->root;
			}
		} else {
			//x - правый ребёнок
			rbnode *y = x->parent->right; //брат x
			if(y->color == RED){
				//брат красный
				y->color = BLACK;
				x->parent->color = RED;
				rotate_right(tree, x->parent);
				y = x->parent->left;
			}
			if(y->left->color == BLACK && y->right->color == BLACK){
				//дети брата чёрные
				y->color = RED;
				x = x->parent;
			} else {
				if(y->left->color == BLACK) {
					//одинаковый ребёнок чёрного цвета
					//правый поворот
					y->right->color = BLACK;
					y->color = RED;
					rotate_left(tree, y);
					y = x->parent->left;
				}
				y->color = x->parent->color;
				x->parent->color = BLACK;
				y->left->color = BLACK;
				rotate_right(tree, x->parent);
				x = tree->root;
			}
		}
	}
	x->color = BLACK;
}
/*
	Удаление узла дерева
*/
void delete_rbnode(rbtree *tree, rbnode *z) {
	rbnode *x, *y;

	if(z == NULL || z == NIL) return;

	if(z->left == NIL || z->right == NIL){
		/*Нет одного ребёнка. Значит можем удалить узел
			и соединить родителя с едиственным ребёнком*/
		y = z;
	} else {
		//Дети все
		//Спускаемся к правому ребёнку и доходим до самого левого чада
		y = z->left;
		while(y->right != NIL) y = y->right;
	}

	if(y->left != NIL)
		x = y->left;
	else
		x = y->right;

	x->parent = y->parent;
	if(y->parent != NULL) {
		if(y == y->parent->left)
			y->parent->left = x;
		else
			y->parent->right = x;
	} else {
		tree->root = x;
	}

	if(y != z) {
		free(z->data);
		z->data = y->data;
		if(y->color == BLACK)
			delete_fixup(tree, x);
	} else {
		free(y->data);
	}
	free(y);
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
	
	if(x != NIL) {		
		del_rbt(x->left);
		del_rbt(x->right);
		free(x->data);
		free(x);
	}
}

/*
	Вывод дерева в консоль
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

	if(x != NIL) {
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
	while(current != NIL)
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
	tree->root = NIL;
	tree->comparator = comparator;
	return tree;
}


/*
	Определение максимальной глубины дерева
*/

int max_deep_rbtree(rbnode *x){
	
	if(x == NIL) return 0;
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
	if(x == NIL) return 0;
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

	if(x->right != NIL){
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
	
	if(x->left != NIL){
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

*/
void draw_rbtree(rbtree *tree, FILE *stream, 
		int max_size_data, void (*data_to_string)(const void*, char*)) {

	int lenght = max_size_data + (deep_rbtree(tree) + 1) * 4; // максимальный размер одной строки в рисунке дерева
	int count_node = size_rbtree(tree); //количество строк в рисунке дерева
	char *str = (char*)malloc(lenght * count_node * sizeof(char));
	char s[2]; // пустая строка
	memset(str, 0, sizeof(str)); //чистка буфера без этого при частом вызове могут быть баги 
	char *data = (char*)malloc((max_size_data + 3) * sizeof(char));
	draw(tree->root, s, true, str, data, lenght, data_to_string);
	fprintf(stream, "%s\n", str);
	free(str);
	free(data);
}
/*
	заменить к чёртовой матери это долбанное удаление она здесь нафиг не нужно
*/ 
