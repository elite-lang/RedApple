

void main() {
	pair a = new pair();
	pair b = new pair();
	a.x = 1; a.y = 0;
	b.x = 2; b.y = 10;
	printf("a.x = %d  ", a.x);
	printf("a.y = %d\n", a.y);
	printf("b.x = %d  ", b.x);
	printf("b.y = %d\n", b.y);
	printf("ans = %d\n", dot(a,b));
}


int dot(pair a, pair b) {
	int k = a.x + b.y;
	printf("k = %d\n", k);
	return k;
}

struct pair {
	int x;
	int y;
}
