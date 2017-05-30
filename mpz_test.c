#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <syscall.h>
#include <errno.h>
#include <linux/random.h>
#include <gmp.h>
#include <sys/syscall.h>
#include <linux/random.h>
#include <unistd.h>

/*
 * 
 */
int main() {

    mpz_t temp;
    unsigned long int s;
    mp_bitcnt_t n = 10;
    s = 12345678912;
    mpz_init(temp);
    gmp_randstate_t state;
    gmp_randinit_default (state);
    gmp_randseed_ui(state, s);
    mpz_urandomb (temp, state, n);
    
    gmp_printf ("limb array %Zd\n", temp);
    printf("The original number: ");
    mpz_out_str(stdout, 2, temp);
    printf("\n");
    mpz_fdiv_q_2exp(temp, temp, 4);
    printf("After 4 bit right shift: ");
    mpz_out_str(stdout, 2, temp);
    printf("\n");
    mpz_mul_2exp (temp, temp, 3);
    printf("Then 3 bit left shift: ");
    mpz_out_str(stdout, 2, temp);
    printf("\n");
    
    return (EXIT_SUCCESS);
}
