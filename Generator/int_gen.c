#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void swap (int *a, int *b) {

	int c;
	c = *a;
	*a = *b;
	*b = c;
}


void swapines(int *array, int n){
	for(int i = 0; i < n - 1; i+=3){
		swap(&array[i], &array[i+1]); 
	}
}

int main(){

	int s, s0, n, h, max;
	char idx[64];
	printf("Введите начальный размер входного массива ");
	fscanf(stdin, "%d", &s0);
	printf("Введите количество сгенерированых массивов ");
	fscanf(stdin, "%d", &n);
	printf("Введите шаг размера массива ");
	fscanf(stdin, "%d", &h);
	printf("Введите введите максимальное значение массива ");
	fscanf(stdin, "%d", &max);
	printf("Введите введите название файлов ");
	fscanf(stdin, "%s", &idx);


	FILE *fin;
	char path [64];

	s = s0;
	for(int i = 0; i < n; i++){

		snprintf(path, 63, "../test/rand/%s%d", idx, i);
		fin = fopen(path, "w");

		printf("файл: %s начал загрузку\n", path);

		fprintf(fin, "%d\n", s);

		srand(time(NULL));

		for(int j = 0; j < s; j++)
			fprintf(fin, "%d\n", rand()%max);

		s += h;
		fclose(fin);
		printf("файл: %s закончил загрузку\n", path);
	}

	s = s0;
	for(int i = 0; i < n; i++){

		snprintf(path, 63, "../test/sort/%s%d", idx, i);
		fin = fopen(path, "w");
		printf("файл: %s начал загрузку\n", path);
		fprintf(fin, "%d\n", s);
		for(int j = 0; j < s; j++)
			fprintf(fin, "%d\n", j);

		s += h;
		fclose(fin);
		printf("файл: %s закончил загрузку\n", path);
	}

	s = s0;
	int *array;
	for(int i = 0; i < n; i++){

		snprintf(path, 63, "../test/swap/%s%d", idx, i);
		fin = fopen(path, "w");
		printf("файл: %s начал загрузку\n", path);
		fprintf(fin, "%d\n", s);
		array = (int*)malloc(s * sizeof(int));
		for(int j = 0; j < s; j++)
			array[j] = j;
		swapines(array, s);
		for(int j = 0; j < s; j++)
			fprintf(fin, "%d\n", array[j]);
		free(array);
		s += h;
		fclose(fin);
		printf("файл: %s закончил загрузку\n", path);
	}


	return 0;
}