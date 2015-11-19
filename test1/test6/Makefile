
test : meta.o test6.o
	clang++ meta.o test6.o -o test -L. -lruntime -ldyncall_s

meta.o : meta.bc
	llc-3.6 meta.bc -filetype=obj

test6.o : test6.bc
	llc-3.6 test6.bc -filetype=obj

test6.bc meta.bc : test6.red
	./redapple test6.red

.PHONY:
clean :
	-rm -rf *.bc *.o test