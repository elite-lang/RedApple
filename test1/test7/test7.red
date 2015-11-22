`(function !void hello ((set !int k)) (
	(call printf "hello world %d\n" k)
))
		
void main() {
	int k = 0;
	int[] g = new int[10];
	for (int i = 0; i < 10; ++i) {
		printf("%d\n", i);
		g[i] = ++k;
		hello(g[i]);
	}
	for (int j = 9; j >= 0; --j) {
		printf("%d\n", j);
		hello(g[j]);
	}
}
