/*
1)	pthread_detach
2)	передача аргументов тредам
3)	отлов налажавших тредов
4)	/proc сброс дискового кэша
5)	устранить внешние факторы!!!111
6)	вместо объявления констант в simpson_api.c
	передавать в вычисляющую функцию аргументы
7)	сделать так, чтобы вычисляемая функция определялась в main
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>
#include "simpson.h"

const double X1 = -5000;
const double X2 = 5000;
const double SEC_TO_NANOSEC = 1000000000;

double foo(double x)
{
	return x * x;
}


/**
 * @brief main v1.1
 * @details simple version:
 * 	1) does not measure time at all (you should
 * 	   use time when call the .out file)
 *	2) now argument of main is a number of threads
 *	   for ONLY ONE calculating
 * 
 * @return [description]
 */
int main(int argc, char const *argv[])
{	
//============ GETTING_MAIN_ARGS ============================================
	DEBUG	ASSERT(argc == 2,
 		"the program needs one argument");

	char* end_ptr = NULL;
	uint32_t nthreads = (uint32_t) strtol(argv[1], &end_ptr, 10);
	DEBUG	ASSERT(*end_ptr == 0
			&& errno != ERANGE
			&& nthreads > 0
			&& nthreads < 128,
		"bad input. The program needs one natural number");

//============ CALCULATING ==================================================
	printf ("%lg\n", simpson_multi (foo, X1, X2, nthreads));

	return 0;
}


/**
 * @brief main v1.0
 * @details simple version:
 * 	1) measuring time by clock_gettime
 * 	2) the argument of main is the number of calculations (using
 * 	   different number of threads: from 1 to the argument)
 * 
 * @param argc [description]
 * @param argv [description]
 * 
 * @return [description]
 */
 /*
int main(int argc, char const *argv[])
{	
//============ GETTING_MAIN_ARGS ============================================
	DEBUG	ASSERT(argc == 2,
 		"the program needs one argument");

	char* end_ptr = NULL;
	uint32_t nthreads = (uint32_t) strtol(argv[1], &end_ptr, 10);
	DEBUG	ASSERT(*end_ptr == 0
			&& errno != ERANGE
			&& nthreads > 0
			&& nthreads < 128,
		"bad input. The program needs one natural number");

//============ CALCULATING ==================================================
	struct timespec start, end;
	//time_i - the time of calculating by i cores
	double time_1, time_i = 0;


	#define _SIMPSON_SCALE_TEST(ncores)				      \
	do {								      \
		clock_gettime(CLOCK_REALTIME, &start);			      \
		simpson_multi (foo, X1, X2, ncores);			      \
		clock_gettime(CLOCK_REALTIME, &end);			      \
		time_##ncores = 					      \
			(double) (end.tv_sec - start.tv_sec) * SEC_TO_NANOSEC \
		      + (double) (end.tv_nsec - start.tv_nsec);		      \
		printf ("TIME [%"PRIu32"]: %lg, SCALE: %lg\n\n",	      \
			ncores, time_##ncores,				      \
			time_1 / time_##ncores / ncores);		      \
	} while (0)							      \

	int j = 0;
	for (j = 0; j < 5; j++) {
		_SIMPSON_SCALE_TEST(1);
		uint32_t i = 0;
		for (i = 2; i <= nthreads; i++)
			_SIMPSON_SCALE_TEST(i);
		printf ("=================================================\n");
	}
	#undef _SIMPSON_SCALE_TEST

	return 0;
}*/
