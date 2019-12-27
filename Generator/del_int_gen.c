#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(){

	int n, p;
	char idx[64];
	printf("Введите количество сгенерированых массивов ");
	fscanf(stdin, "%d", &n);
	printf("Введите процент удаления ");
	fscanf(stdin, "%d", &p);
	printf("Введите введите название файлов ");
	fscanf(stdin, "%s", &idx);

	FILE *fin, *fout;
	char buff_path_in [64];
	char buff_path_out [64];

	char path[5][64] = {"../test/rand/", "../test/sort/", "../test/swap/", "../test/decsort/", "../test/insort/"};
	char name_input_file[] = "input";

	int s, *a;
	int *sizes = (int*)malloc(n * sizeof(int));

	for(int k = 0; k < 3; k++){
		for(int i = 0; i < n; i++){

			snprintf(buff_path_in, 63, "%s%s%d", path[k], name_input_file, i);
			snprintf(buff_path_out, 63, "%s%s%d", path[k], idx, i);
			fin  = fopen(buff_path_in, "r");
			fout = fopen(buff_path_out, "w");

			printf("файл: %s начал загрузку\n", buff_path_out);
			fscanf(fin, "%d\n", &s);
			sizes[i] = s;
			a = (int*)malloc(s * sizeof(int));
			for(int j = 0; j < s; j++)
				fscanf(fin, "%d\n", &a[j]);

			srand(time(NULL));

			fprintf(fout, "%d\n", (s * p) / 100);
			for(int j = 0; j < ((s * p) / 100); j++)
				fprintf(fout, "%d\n", a[rand()%s]);	

			free(a);
			fclose(fin);
			fclose(fout);

			printf("файл: %s закончил загрузку\n", buff_path_out);
		}
	}

	for(int i = 0; i < n; i++){
		snprintf(buff_path_out, 63, "%s%s%d", path[3], idx, i);
		fout = fopen(buff_path_out, "w");
		printf("файл: %s начал загрузку\n", buff_path_out);
		fprintf(fout, "%d\n", (sizes[i] * p) / 100);
		for(int j = 0, l = ((sizes[i]) / 2), r = ((sizes[i]) / 2); j < ((sizes[i] * p) / 100) ; j++){
			if(j % 2 == 0) {
				l--;
				fprintf(fout, "%d\n", l);
			} else {
				r++;
				fprintf(fout, "%d\n", r);
			}
		}
		printf("файл: %s закончил загрузку\n", buff_path_out);
	}
	for(int i = 0; i < n; i++){
		snprintf(buff_path_out, 63, "%s%s%d", path[4], idx, i);
		fout = fopen(buff_path_out, "w");
		printf("файл: %s начал загрузку\n", buff_path_out);
		fprintf(fout, "%d\n", (sizes[i] * p) / 100);
		for(int j = 0; j < ((sizes[i] * p) / 100) ; j++)
			fprintf(fout, "%d\n", j);
		printf("файл: %s закончил загрузку\n", buff_path_out);
	}
	free(sizes);
	return 0;
}