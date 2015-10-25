
void hello(int k, int g) {
	int y = k + g;
	printf("%d\n", y);
	if (k + g < 5) printf("right");
}	


void go(int k) {
	int a = 0;
	while (a < k) {
		printf("go-%d ", a);
		a = a + 1;
	}
}

void print(int k) {
	for (int i = 0; i < 10; i = i+1) {
		printf("hello-%d ",i);
	} 
}


void main() {
	printf("hello world");
	hello(1,2);
	print(9);
}

