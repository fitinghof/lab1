

all:
	gcc -O2 -o gaussianseq gaussianseq.c
	gcc -O2 -o gaussianmulti gaussianmulti.c -lpthread
	gcc -O2 -o sortseq qsortseq.c
	gcc -O2 -o sortmulti qsortmulti.c -lpthread

