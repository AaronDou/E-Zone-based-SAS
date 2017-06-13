#include "common.h"
#include "parameters.h"

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

int main(){

	int rbits = RBITS; //112 bit security, 10 bit for overflow-proof
	int qbits = 1024;
	pairing_t pairing;
	pbc_param_t param;
	pbc_param_init_a_gen(param, rbits, qbits);
	pairing_init_pbc_param(pairing,param);

	FILE * f;
	f = fopen("proj_a.param" , "w");
	pbc_param_out_str(f,param);


	printf("----Length evaluation----\n");
	printf("--using pairing_length_in_bytes\n");
	printf("size of G1 element is %ld bits\n",pairing_length_in_bytes_x_only_G1(pairing)*8);
	printf("size of G2 element is %ld bits\n",pairing_length_in_bytes_x_only_G2(pairing)*8);
	printf("size of GT element is %ld bits\n",pairing_length_in_bytes_GT(pairing)*8);
	printf("size of Zr element is %ld bits\n",pairing_length_in_bytes_Zr(pairing)*8);

	mpz_t r, tpa;
	unsigned long int s;
	mp_bitcnt_t n = SEC_PARAM;
	s = 12345678912;
	mpz_init(r);
	mpz_init(tpa);

	gmp_randstate_t state;
	gmp_randinit_default (state);
	gmp_randseed_ui(state, s);
	mpz_urandomb (r, state, n);
	mpz_urandomb (tpa, state, n);


	printf("R value: ");
	mpz_out_str(stdout, 10, r);
	printf("\n");
	printf("T+A value ");
	mpz_out_str(stdout, 10, tpa);
	printf("\n");

	element_t pac, R, TPA;
	element_init_Zr(pac, pairing);
	element_init_Zr(R, pairing);
	element_init_Zr(TPA, pairing);
	element_set_mpz(R, r);
	element_set_mpz(TPA, tpa);

	double t0, t1;

 	t0 = pbc_get_time();
	Pac(pac, R, TPA);
  	t1 = pbc_get_time();
	printf("Packed R value and T+A value, time elapsed %fms\n",(t1-t0)*1000);
	element_printf("Packed value: %B\n", pac);

 	t0 = pbc_get_time();
	DePac(pac, R, TPA);
  	t1 = pbc_get_time();
	printf("De-packed R value and T+A value, time elapsed %fms\n",(t1-t0)*1000);
	element_printf("De-packed R value: %B\n", R);
	element_printf("De-packed T+A value: %B\n", TPA);	
	
	

	

	
    return (EXIT_SUCCESS);
}
