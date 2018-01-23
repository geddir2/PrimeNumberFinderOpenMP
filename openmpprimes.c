#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#ifdef _OPENMP
#include <omp.h>
#endif /* _OPENMP */

#define END   100000000
//THIS FUNCTON TAKES IN THE BRACKER VARIABLE AND CONVERTS IT TO WHAT POWER OF 10 IT IS
//ONCE THIS IS FOUND WE CAN FIND WHICH SPOT IN THE PRIME TO STORE PRIME_COUNT
uint32_t basketFind(uint32_t input){
    if (input == 10){return 1;}
    if (input == 100){return 2;}
    if (input == 1000){return 3;}
    if (input == 10000){return 4;}
    if (input == 100000){return 5;}
    if (input == 1000000){return 6;}
    if (input == 10000000){return 7;}
    if (input == 100000000){return 8;}
    return 0;
}



// Only pass odd values to this function! :)
//FUNCTION THAT DETERMINES IF THERE ARE ANY PRIME FACTORS OF V, IF SO RETURN 0, ELSE RETURN 1
uint32_t is_prime(uint32_t v)
{
    uint32_t end = sqrt(v);
    for (uint32_t i = 3; i <= end; i += 2) {
        if ((v % i) == 0) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char **argv)
{
    #ifdef _OPENMP // ONLY NEEDED FOR OPENMP
    uint32_t threads = 4; // THE AMOUNT OF THREADS FOR PARALLELISM
    #endif
    // We'll grab 2 as it's the only even prime
    uint32_t prime[9] = {0,0,0,0,0,0,0,0,0}; // ARRAY TO STORE PRIME COUNT FOR ALL 9 "Ns"
    #ifdef _OPENMP
    #pragma omp parallel num_threads(threads)
    #endif
    {
        uint32_t prime_count = 1; // INTERGER TO STORE HOW MANY PRIMES THERE ARE
        uint32_t bracket = 1; // INTERGER TO STORE HOW THE CURRENT BRACKET SIZE
        
        // Wow!  Quite a bit of parallel...
        // Maybe too much for one time!
        #ifdef _OPENMP
        #pragma omp for
        #endif
        for (uint32_t i = 3; i < END; i += 2) {
            if (i >= bracket) { // IF YOU REACH THE END OF BRACKET ENTER AND CHANGE PRIME AND BRACKET
                if (prime_count > prime[basketFind(bracket)]){ // IF STATEMENT FOR PARALLELISM TO ONLY STORE THE THREAD WITH THE CORRECT AMOUNT
                     prime[basketFind(bracket)] = prime_count;
                }
                bracket *= 10; // ALTERING BRACKET SIZE
            }
            if (is_prime(i)) { // IF I IS PRIME ADD TO THE PRIME COUNT
                prime_count++;
            }
        }
        prime[8] +=  prime_count; //SUMMING UP THE LAST PRIME
    }
    
     #ifdef _OPENMP // IF IN OPEN MP ACCOUNT FOR WHEN SUMMING UP PRIME[8] THAT THERE ARE A GIVEN THREDS AND BECUASE PRIME_COUNT IS INITALIZED AT 1  THERE IS A OVERLAP IN THE ACCOUNTING AND YOU NEED TO SUBRACT (THREADS-1) TO ACCOUNT FOT THE OVERLAP
    prime[8] = prime[8] - threads +1; // ACCOUNTING FOR OVERLAP
     #endif
    
    for (uint32_t i = 0; i<9; i++){ //PRINTING OUT THE BRACKET SIZE AND THE AMOUNT OF PRIMES IN SAID BRACKET
        printf("%12d\t%12d\n", (int)pow(10,i), prime[i]);
        
    }
    return 0;
}
