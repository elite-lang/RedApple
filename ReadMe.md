RedApple Programming language

C++编写的一款简单的类C风格的编译语言
使用flex和bison制作的词法分析器和语法分析器
使用LLVM-3.6构建后端
使用CMake构建项目


目前支持的语法特性有：

* 全局函数，无需声明
* 简单的类型系统，支持int、float、double和结构体struct
* 简化的指针，所有结构体变量声明的都是指针
* 支持常量字符串
* for while if 等逻辑控制语句
* import 其他文件
* meta元数据和反射调用
* 用户自定义宏

示例代码：
```
void hello(int k, int g) {
	int y = k + g;
	printf("%d\n", y);
	if (k + g < 5) printf("right\n");
}	


void go(int k) {
	int a = 0;
	while (a < k) {
		printf("go-%d\n", a);
		a = a + 1;
	}
}

void print(int k) {
	for (int i = 0; i < 10; i = i+1) {
		printf("hello-%d\n",i);
	} 
}


void main() {
	printf("hello world\n");
	hello(1,2);
	print(9);
}
```


反射调用：
```
void print(int k) {
	printf("hello-%d\n",k);
}

void main() {
	FunctionCall("print", 5);
}

```

自定义宏的使用：
```
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
```