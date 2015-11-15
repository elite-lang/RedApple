
test : test4.o test5.o
	clang test4.o test5.o -o test

test4.o : test4.bc
	llc-3.6 test4.bc -filetype=obj

test5.o : test5.bc
	llc-3.6 test5.bc -filetype=obj

test4.bc : test4.red
	./redapple test4.red

test5.bc : test5.red
	./redapple test5.red
