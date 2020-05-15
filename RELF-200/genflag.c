#include <stdio.h>

int main(int argc, char *argv[]) {
		char flag[] = {"flag{E1F_FUnct10n_Math_S01vE}"};
		int as[30],bs[30],sums[30];
		srand((unsigned int)time(NULL));
		for(int i = 0; i < sizeof(flag); ++i) {
			int a = rand() % 100;
			int ax = flag[i] * a;
			int b = rand() % 500;
			int sum = ax + b;
			printf("%d x + %d = %d\n",a,b,sum);
			as[i] = a;
			bs[i] = b;
			sums[i] = sum;
		}
		for(int i = 0; i < 30; ++i) {
			printf("%d, ",as[i]);
		}
		printf("\n");
		for(int i = 0; i < 30; ++i) {
			printf("%d, ",bs[i]);
		}
		printf("\n");
		for(int i = 0; i < 30; ++i) {
			printf("%d, ",sums[i]);
		}
		printf("\n");
}