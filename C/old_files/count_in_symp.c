#include <stdio.h>
#include <string.h>

int main() {
	
	int d;
	fscanf(stdin, "%d", &d);
	int data = d;
	int l = 0;
	char c[16];

	while(data > 0){
		data /= 10;
		l++;
	}

	c[l] = '\0';
	data = d;

	while(data > 0){
		
		c[l - 1] = data % 10 + '0';
		data /= 10;
		l--;
	}

	printf("%s\n", c);
	return 0;
}
	