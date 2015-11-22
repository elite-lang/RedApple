
test : meta.o test8.o
	clang++ meta.o test8.o -o test -L. -lruntime -ldyncall_s

meta.o : meta.bc
	llc-3.6 meta.bc -filetype=obj

test8.o : test8.bc
	llc-3.6 test8.bc -filetype=obj

test8.bc meta.bc : test8.red
	./redapple test8.red

.PHONY:
clean :
	-rm -rf *.bc *.o test