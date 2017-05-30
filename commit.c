#include "common.h"
#include "parameters.h"

void commit(element_t c, element_t s, element_t t, element_t g, element_t h, pairing_t pairing)
{
    element_t temp1, temp2;
    element_init_GT(temp1, pairing);
    element_init_GT(temp2, pairing);
    
    element_pow_zn(temp1, g, s);
    element_pow_zn(temp2, h, t);
    
    element_mul(c, temp1, temp2);
}

int rev(element_t c, element_t s, element_t t, element_t g, element_t h, pairing_t pairing)
{
    element_t temp1, temp2;
    element_init_GT(temp1, pairing);
    element_init_GT(temp2, pairing);
    
    element_pow_zn(temp1, g, s);
    element_pow_zn(temp2, h, t);
    
    element_mul(temp1, temp1, temp2);
    element_printf("\ncommit result from rev: %B\n", temp1);
    return element_cmp(c, temp1);
}
