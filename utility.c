#include "parameters.h"
#include "common.h"
#include <time.h>
#include <stdlib.h>


int offset( int l, int f, int h_s, int p_ts, int g_rs, int i_s) { 
    return l*F*H_s*P_ts*G_rs*I_s + f*H_s*P_ts*G_rs*I_s + h_s*P_ts*G_rs*I_s + p_ts*G_rs*I_s + g_rs*I_s + i_s;
}

void init_array_ct (ct * array, pairing_t pairing){
    int i;
    for(i = 0; i < N; i++){
        element_init_GT((array + i)->c1, pairing);
        element_init_G1((array + i)->c2, pairing);
    }
}

void init_array_element (element_t * array, pairing_t pairing){
    int i;
    for(i = 0; i < N; i++){
        element_init_GT(*(array + i), pairing);
    }
}

double rand_0_1(){ return (double)rand() / (double)RAND_MAX; }