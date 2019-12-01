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
	Левый поворот
*/
void rotate_left(rbtree *tree, rbnode *x) {
	
}

/*
	Правый поворот
*/
void rotate_right(rbtree *tree, rbnode *x) {
	
}


/*
	Восстановение красно-чёрных свойств дерева
	после вставки узла в дерево
*/

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

	
}

/*
	Восстановление красно-чёрных свойств дерева
	после удаления узла
*/

/*
	Удаление узла дерева
*/
void delete_rbnode(rbtree *tree, rbnode *z) {

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
