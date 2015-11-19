
void print(int k) {
	@for_n (i, k) {
		printf("hello-%d\n", i);
	}
}

defmacro for_n (p, size, code)  {
	for (int p = 1; p <= size; p = p+1)
		code;
}


void main() {
	FunctionCall("print", 5);
}