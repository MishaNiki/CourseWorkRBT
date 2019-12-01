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

	char path[3][64] = {"../test/rand/", "../test/sort/", "../test/swap/"};
	char name_output_file[] = "input";

	int s, *a;

	for(int k = 0; k < 3; k++){
		for(int i = 0; i < n; i++){

			snprintf(buff_path_in, 63, "%s%s%d", path[k], idx, i);
			snprintf(buff_path_out, 63, "%s%s%d", path[k], name_output_file, i);
			fin  = fopen(buff_path_in, "w");
			fout = fopen(buff_path_out, "r");


			fscanf(fout, "%d\n", &s);
			a = (int*)malloc(s * sizeof(int));
			for(int j = 0; j < s; j++)
				fscanf(fout, "%d\n", &a[j]);

			srand(time(NULL));

			fprintf(fin, "%d\n", (s * p) / 100);
			for(int j = 0; j < ((s * p) / 100); j++)
				fprintf(fin, "%d\n", a[rand()%s]);	

			free(a);
			fclose(fin);
			fclose(fout);
		}
	}
	return 0;
}