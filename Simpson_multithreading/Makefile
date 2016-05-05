CC		= gcc
WARNINGS	= -Wall
STANDART	= 
THREADS		= -pthread
MATH		= -lm
#========== SPECIAL =============================
GCOV		= -fprofile-arcs -ftest-coverage
VALGRIND	= 
SPECIAL		= 
#================================================
CFLAGS		=  $(STANDART) $(WARNINGS) $(THREADS) $(MATH) -MD
TARGET		= simpson.out
.PHONY: all clean gcov

all: simpson.out

simpson.out: main.c simpson_api.o
	$(CC) $(CFLAGS) simpson_api.o main.c -o $(TARGET)

simpson_api.o: simpson_api.c
	$(CC) $(CFLAGS) simpson_api.c -c -o simpson_api.o

valgrind:
	valgrind ./$(TARGET) 4

clean:
	rm -rf *.o *.gcda *.gcno *.gcov *.preproc simpson.out
