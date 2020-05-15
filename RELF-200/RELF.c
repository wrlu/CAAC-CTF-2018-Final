#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void f(const char *f, int n);
void c(const char * f, int n);

//flag{E1F_FUnct10n_Math_S01vE}
//102 108 97 103 123 69 49 70 95 70 85 110 99 116 49 48 110 95 77 97 116 104 95 83 48 49 118 69 125 0
void f(const char *f, int n) {
	if (n < 1 || n > 29) {
		printf("Wrong\n");
		return;
	}
	c(f, n);
}

void c(const char * f, int n) {
	int a[]={34, 29, 76, 98, 43, 52, 97, 24, 40, 23, 4, 69, 44, 52, 43, 80, 1, 63, 79, 48, 55, 8, 26, 95, 4, 34, 82, 11, 51, 81};
	int b[]={297, 169, 431, 229, 423, 257, 465, 288, 437, 86, 364, 123, 390, 485, 232, 352, 244, 469, 54, 359, 16, 110, 355, 244, 256, 94, 187, 19, 242, 432};
	int c[]={3765, 3301, 7803, 10323, 5712, 3845, 5218, 1968, 4237, 1696, 704, 7713, 4746, 6517, 2339, 4192, 354, 6454, 6137, 5015, 6396, 942, 2825, 8129, 448, 1760, 9863, 778, 6617, 432};
	for(int i = 0; i < n; ++i) {
		int ax = a[i] * f[i];
		int sum = ax + b[i];
		if (sum != c[i]) {
			printf("Wrong\n");
			return;
		}
	}
	printf("Your flag is: %s\n", f);
}

int main(int argc, char *argv[]) {
	char flag[255];
	printf("Please input your flag:\n");
	scanf("%s", flag);
	f(flag, strlen(flag));
	return 0;
}