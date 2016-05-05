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

const double STEP = 0.00001;
const double SEC_TO_NANOSEC_api = 1000000000;

void* simpson_thread(void* data)
{
	struct timespec start, end;
	clock_gettime(CLOCK_REALTIME, &start);

	struct thread_simpson_arg *my_arg = (struct thread_simpson_arg*) data;
	double (*func)(double arg) = my_arg->func;
	double x1 = my_arg->x1;
	double x2 = my_arg->x2;
	double step = my_arg->step;
	double tmp_sum = 0;

	double coef_1 = step / 6;
	double coef_2 = step / 2;
	while (x1 + step < x2) {
		tmp_sum += coef_1 * (
				       func (x1)
			      	     + func (x1 + step)
			      	     + 4 * func (x1 + coef_2));
		x1 += step;
	}

	*(my_arg->res) += tmp_sum;

	clock_gettime(CLOCK_REALTIME, &end);
	double thread_time =
		(double) (end.tv_sec - start.tv_sec)		\
		* SEC_TO_NANOSEC_api				\
	     	+ (double) (end.tv_nsec - start.tv_nsec);
	printf ("thread_time: %lg\n", thread_time);

	return NULL;
}

double simpson_multi(double (*func)(double arg),
		   double x1,
		   double x2,
		   uint32_t nthreads)
{
	DEBUG	ASSERT (func, "func NULL pointer");
	DEBUG	ASSERT (x1 <= x2, "x1 > x2");

	pthread_t threads[nthreads];
	struct thread_simpson_arg *thread_args = (struct thread_simpson_arg*)\
		calloc (nthreads, sizeof(struct thread_simpson_arg));

	double	threads_results[nthreads];
	double  from_arg = x1;
	double  part_size = (x2 - x1) / nthreads;

	#define _CALL_THREAD_SIMPSON(x2_arg)				\
	do {								\
		thread_args[i].func = foo;				\
		thread_args[i].x1   = from_arg;				\
		from_arg = from_arg + part_size;			\
		thread_args[i].x2   = x2_arg;				\
		thread_args[i].step = STEP;				\
		thread_args[i].res  = threads_results + i;		\
		ASSERT(pthread_create(&threads[i],			\
				      NULL,				\
				      simpson_thread,			\
				      (void*) (thread_args + i)) == 0,	\
			"pthread_create failed");} while (0)

	uint32_t tmp = nthreads - 1;
	uint32_t i = 0;
	for (i = 0; i < tmp; i++) {
		_CALL_THREAD_SIMPSON(from_arg);
	}
	_CALL_THREAD_SIMPSON(x2);
	#undef _CALL_THREAD_SIMPSON

	for (i = 0; i < nthreads; i++)  pthread_join (threads[i], NULL);
	double result = 0;
	for (i = 0; i < nthreads; i++)  result += threads_results[i];
	free (thread_args);
	return  result;
}

double simpson(double (*func)(double arg), double x1, double x2)
{
	DEBUG	ASSERT (func,		"func NULL pointer");
	DEBUG	ASSERT (x1 <= x2,	"x1 > x2");

	return  (x2 - x1) / 6 * (func (x1) + 4 * func ((x1 + x2)/2) + func (x2));
}

//I keep print_diff in case i will use gettimeofday
/*void print_diff(struct timeval begin, struct timeval end, char* begin_str)
{
	long long seconds = (long long)end.tv_sec  - (long long)begin.tv_sec;
	long long micros  = (long long)end.tv_usec - (long long)begin.tv_usec;
	if (micros < 0)
	{
		micros += 1000000;
		seconds -= 1;
	}
	fprintf(stderr, "%s %lld.%06lld\n", begin_str, seconds, micros);
}*/
