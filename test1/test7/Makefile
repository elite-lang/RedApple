
test : meta.o test7.o
	clang++ meta.o test7.o -o test -L. -lruntime -ldyncall_s

meta.o : meta.bc
	llc-3.6 meta.bc -filetype=obj

test7.o : test7.bc
	llc-3.6 test7.bc -filetype=obj

test7.bc meta.bc : test7.red
	./redapple test7.red

.PHONY:
clean :
	-rm -rf *.bc *.o test