#include <time.h>
#include "common.h"
#include "parameters.h"

ct * generate_e_zone_ct(element_t pk_a, element_t Z, pairing_t pairing){
    
    element_t * T_k = (element_t *)malloc(N * sizeof(element_t));
    init_array_element(T_k, pairing);   
    
    srand(time(NULL));
    
    int i;
    element_t Zero;
    element_init_Zr(Zero, pairing);
    element_set_si(Zero, 0);
    
    for (i = 0; i < N; i++){
        if (rand_0_1() < E_zone_percentage){ // E_zone
            element_pow_zn(*(T_k+i), Z, Zero);
        }
        else{ // Non_E_zone
            element_random(*(T_k+i));
        }
    }
    
    ct * T_k_ct = (ct *)malloc(N * sizeof(ct));
    init_array_ct(T_k_ct, pairing);
    for (i = 0; i < N; i++){
//        element_printf("%B\n", (T_k_ct+i)->c2);
        Enc((T_k_ct + i)->c1, (T_k_ct + i)->c2, *(T_k+i), pk_a, Z, pairing);
    }
    
    return T_k_ct;
}