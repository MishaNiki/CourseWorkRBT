#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "rbtree.h"


int compare(const void *x1, const void *x2) {	// функция сравнения элементов массива
	return (*(int*)x1 - *(int*)x2);				// если результат вычитания равен 0, то числа равны, < 0: x1 < x2; > 0: x1 > x2
}

void print_data(const void *x){
	printf("%d ", *(int*)x);
}

void data_to_string(const void *x, char *data) {
	snprintf(data,  sizeof(data) - 1, "%d", *(int*)x);
}

int main(){

	rbtree *tree = create_rbtree(compare);
	rbnode *tmp;
	int n, a;
	printf("Введи количество \t");
	fscanf(stdin, "%d", &n);

	srand(time(NULL));
	for(int i = 0; 1; i++){		
		a = rand()%20;
		printf("Дальше?\t");
		char c;
		fscanf(stdin, "%c", &c);
		if(c == 'n' || c == 'N')
			break;
		printf("a = %d\n", a);
		if((tmp = find_rbnode(tree, &a)) == NULL){
			insert_rbnode(tree, &a, sizeof(int));
		} else {
			printf("Число %d уже есть, i = %d\n Удаление элемента\n", *(int*)tmp->data, i);
			delete_rbnode(tree, tmp);
			printf("Удаление завершено\n");
		}
		printf("ROOT %d\n", *(int*)tree->root->data);
		draw_rbtree_int(tree, stdout, 16, data_to_string);
	}

	print_rbtree(tree, print_data);
	printf("Максимальная глубина %d\n", deep_rbtree(tree));
	draw_rbtree_int(tree, stdout, 16, data_to_string);
	delete_rbtree(tree);
	return 0;
}

