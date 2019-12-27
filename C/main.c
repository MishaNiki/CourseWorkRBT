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


int *input_array, *delete_array; 	//массив ввода данных
size_t size_ia, size_da; 			//размер входного массива

void array_filling (int idx_path, int test) {

	int idx_path_in;
	if(idx_path > 2)
		idx_path_in = 1;
	else
		idx_path_in = idx_path;

	char path[5][64] = {"../test/rand/", "../test/sort/", "../test/swap/", "../test/decsort/", "../test/insort/"};
	char name_input_file[] 	= "input";
	char name_delete_file[] = "delete";
	char buff_input_file[64], buff_delete_file[64];

	FILE *fin, *fdel;
	snprintf(buff_input_file,  63, "%s%s%d", path[idx_path_in], name_input_file, test);
	snprintf(buff_delete_file, 63, "%s%s%d", path[idx_path], name_delete_file, test);
	fin  = fopen(buff_input_file, "r");
	fdel = fopen(buff_delete_file, "r");

	if(fin == NULL || fdel == NULL){
		fprintf(stderr, "FILE not found!!!\n");
		exit(1);
	}
	fscanf(fin, "%d", &size_ia);
	fscanf(fdel, "%d", &size_da);

	input_array  = (int*)malloc(size_ia * sizeof(int)); 
	delete_array = (int*)malloc(size_da * sizeof(int));

	for(int i = 0; i < size_ia; i++)
		fscanf(fin, "%d", &input_array[i]);

	for(int i = 0; i < size_da; i++)
		fscanf(fdel, "%d", &delete_array[i]);

	fclose(fin);
	fclose(fdel);	
}


int main () {
	
	clock_t start, end, result[10];
	rbnode *tmp;

	char output_path[5][64] = {"../result/C/rand", "../result/C/sort", "../result/C/swap", "../result/C/decsort", "../result/C/insort"};

	for(int k = 3; k < 5; k++){
		for(int i = 0; i < 10; i++){

			array_filling(k, i);
			printf("Начало работы\n");
			start = clock();
			rbtree *tree = create_rbtree(compare);
			for(int j = 0; j < size_ia; j++)
				insert_rbnode(tree, &input_array[j], sizeof(int));
			for(int j = 0; j < size_da; j++)
				if((tmp = find_rbnode(tree, &delete_array[j])) != NULL)
					delete_rbnode(tree, tmp);
			end = clock();
			delete_rbtree(tree);
			result[i] = end - start;
			printf("time = %.3f\tk = %d\ti = %d\n", (double)result[i]/CLOCKS_PER_SEC, k, i);
			
			free(input_array);
			free(delete_array);
		}

		FILE *fout = fopen(output_path[k], "w");
		if(fout == NULL){
			fprintf(stderr, "FILE not found!!!\n");
			exit(1);
		}

		for(int i = 0; i < 10; i++)
			fprintf(fout, "%d %.3f\n", (i+1)*1000000,(double)result[i]/CLOCKS_PER_SEC);
		fclose(fout);

	}
	
	return 0;
}