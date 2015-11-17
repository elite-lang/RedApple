
test : meta.o test5.o
	clang++ meta.o test5.o -o test -L. -lruntime -ldyncall_s

meta.o : meta.bc
	llc-3.6 meta.bc -filetype=obj

test5.o : test5.bc
	llc-3.6 test5.bc -filetype=obj

test5.bc meta.bc : test5.red
	./redapple test5.red

.PHONY:
clean :
	-rm -rf *.bc *.o test