NEW:clean fifo_readmany fifo_writemany
	@echo success

fifo_writemany:fifo_writemany.c
	-gcc -g fifo_writemany.c -o fifo_writemany

fifo_readmany:fifo_readmany.c
	-gcc -g fifo_readmany.c -o fifo_readmany

clean:
	-rm log log1 log2 log3 log4 log5 test_fork

