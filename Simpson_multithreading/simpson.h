#ifndef	SIMPSON
#define SIMPSON

//===========================================================================
//=============== MODS ======================================================
//===========================================================================
//#define DEBUG_MOD
#ifdef	DEBUG_MOD
#define DEBUG
#else	
#define DEBUG if (0)
#endif

//===========================================================================
//=============== DEFINES ===================================================
//===========================================================================
#define ASSERT( cond, message )					\
	do {							\
		if (!(cond)) {					\
		printf("pid [%d] ERROR: ", getpid());		\
		fflush(stdout);					\
		printf("%s, line %d: ", __FILE__, __LINE__);	\
		fflush(stdout);					\
		printf(message);				\
		fflush(stdout);					\
		putchar ('\n');					\
		exit(-1);					\
		}						\
 	} while (0);

//===========================================================================
//=============== CONSTS ====================================================
//===========================================================================

//===========================================================================
//=============== STRUCTURES ================================================
//===========================================================================
struct thread_simpson_arg {
	double (*func)(double x);
	double x1;
	double x2;
	double step;
	double *res;
};

//===========================================================================
//============ PROTOTYPES ===================================================
//===========================================================================
/**
 * @brief calculates integral using Simpson's formula
 * 
 * @param func a pointer of a math double-func, which needs one double-argument
 * @param x1 the beginning of the interval
 * @param x2 the end of the interval
 * @return the result of calculating the integral of func on the interval [x1, x2]
 */
double simpson(double (*func)(double arg), double x1, double x2);

/**
 * @brief calculates integral using Simpson's formula effectively
 * @details uses threads
 * 
 * @param func a pointer of a math double-func, which needs one double-argument
 * @param x1 the beginning of the interval
 * @param x2 the end of the interval
 * @param nthreads a positive number of threads, which the function will use
 * @return the result of calculating the integral of func on the interval [x1, x2]
 */
double simpson_multi(double (*func) (double arg),
		   double x1,
		   double x2,
		   uint32_t nthreads);

void* simpson_thread(void* data);

/**
 * @brief It's a usual math function
 * 
 * @param x real number, should be possible to be used in function
 * @return the result of calculating the function ;)
 */
double foo(double x);

#endif
