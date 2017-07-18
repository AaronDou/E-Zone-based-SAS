#include <time.h>
#include "common.h"
#include "parameters.h"

ct * generate_e_zone_ct(element_t pk_a, element_t Z, pairing_t pairing, ct * R_ct, element_t tpa_sum){
    FILE * cOut;
    cOut = fopen("commit.txt", "a");
    FILE * rOut;
    rOut = fopen("r.txt", "a");
    int rbits = RBITS;
    int qbits = 1024;
    mpz_t r, tpa;
    unsigned long int s;
    mp_bitcnt_t n = SEC_PARAM;
    s = 12345678912;
    mpz_init(r);
    mpz_init(tpa);
    int i;
    double t0, t1;

    element_t tempc, base, h;
    element_init_GT(base, pairing);
    element_init_GT(h, pairing);
    element_init_GT(tempc, pairing);

    gmp_randstate_t state;
    gmp_randinit_default (state);

    element_t R, TPA, pac, R_Pac;
    element_init_Zr(R, pairing);
    element_init_Zr(TPA, pairing);
    element_init_Zr(pac, pairing);
    element_init_Zr(R_Pac, pairing);

    element_t * T_k = (element_t *)malloc(N * sizeof(element_t));
    init_array_element(T_k, pairing);  

    element_t * R_k = (element_t *)malloc(N * sizeof(element_t));
    init_array_element(R_k, pairing); 

    srand(time(NULL));
    
    element_t Zero;
    element_init_Zr(Zero, pairing);
    element_set_si(Zero, 0);
   
    t0 = pbc_get_time();
    for (i = 0; i < N; i++){

	mpz_urandomb (r, state, n);
	element_set_mpz(R, r);
	element_fprintf(rOut, "%B\n", R);
	//element_printf("R: %B\n", R);
        if (i==0){//rand_0_1() < E_zone_percentage){ // E_zone
            //element_pow_zn(*(T_k+i), Z, Zero);
	    mpz_set_ui(tpa, 1);
        }

        else{ // Non_E_zone
            //element_random(*(T_k+i));
            mpz_urandomb (tpa, state, n);
        }

	element_set_mpz(TPA, tpa);
	if(i==0){
	    element_add(tpa_sum, tpa_sum, TPA);
	}
	commit(tempc, R, TPA, base, h, pairing);
	element_fprintf(cOut, "%B\n", R);
	Pac(pac, R, TPA);
	element_pow_zn(*(T_k+i), Z, pac);
	Pac(R_Pac, R, Zero);
	element_pow_zn(*(R_k+i), Z, R_Pac);
	//element_printf("After Pac TPA: %B\n", *(T_k+i));
	//element_printf("After Pac R: %B\n", *(R_k+i));
    }
    t1 = pbc_get_time();
    printf("%fms\n", (t1-t0)*1000);
    //element_printf("%B\n", (T_k));
    ct * T_k_ct = (ct *)malloc(N * sizeof(ct));
    init_array_ct(T_k_ct, pairing);
    t0 = pbc_get_time();
    for (i = 0; i < N; i++){
//        element_printf("%B\n", (T_k_ct+i)->c2);
        Enc((T_k_ct + i)->c1, (T_k_ct + i)->c2, *(T_k+i), pk_a, Z, pairing);
	Enc((R_ct + i)->c1, (R_ct + i)->c2, *(R_k+i), pk_a, Z, pairing);
    }
    t1 = pbc_get_time();
    printf("E-Zone data encryption time elapsed %fms\n", (t1-t0)*1000);
    return T_k_ct;
}
