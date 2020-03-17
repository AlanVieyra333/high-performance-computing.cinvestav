#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>

#define TOTAL_EXP 1000000000

double mod(double x, double y) {
    return sqrt(x*x + y*y);
}

long getMicrotime(){
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

class Rand {
private:
    unsigned long int seed;
public:
    Rand();
    Rand(unsigned long int seed);
    int next(void);
};

Rand::Rand() {
    this->seed = 1;
}

Rand::Rand(unsigned long int seed) {
    this->seed = seed;
}

int Rand::next(void) // RAND_MAX assumed to be 32767
{
    this->seed = this->seed * 1103515245 + 12345;
    return (unsigned int)(this->seed/65536) % RAND_MAX;
}

int main() 
{
    int i;
    double x = 0, y = 0;
    int success_exp = 0;

    #pragma omp parallel shared(success_exp) private(x, y)
    {
        Rand custom_rand(getMicrotime() ^ omp_get_thread_num());

        #pragma omp for
        for (i = 0; i < TOTAL_EXP; ++i) {
            x = custom_rand.next()%100000 / 100000.0;
            y = custom_rand.next()%100000 / 100000.0;
            //printf("%d) Thread %d - \tx = %lf, y = %lf, d = %lf\n", i, omp_get_thread_num(),  x, y, mod(x,y));
    
            if(mod(x,y) <= 1.0) {
                #pragma omp atomic
                success_exp++;
            }
        }
    }

    printf("success_exp: %d\tTotal: %d\n", success_exp, TOTAL_EXP);

    printf("PI = %.8lf\n", 4.0 * success_exp / TOTAL_EXP);

    return 0; 
} 
