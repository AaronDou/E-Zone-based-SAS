#ifndef COMMON_H
#define COMMON_H
#include <pbc.h>
#include <pbc_test.h>
#include <openssl/sha.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#define LENGTH 128
#define TEST 0
#define RBITS 244
#define SEC_PARAM 112

void hash(element_t output, char* input);

void Enc(element_t c1, element_t c2, element_t M, element_t pk, element_t Z, pairing_t pairing) ;

void ReEnc(element_t c1o, element_t c2o, element_t c1i, element_t c2i, element_t rk);

void Dec(element_t M, element_t c1, element_t c2, element_t sk, element_t g, pairing_t pairing);

void ReDec(element_t M, element_t c1, element_t c2, element_t pk, pairing_t pairing) ;

void Enc2(element_t c1, element_t c2, element_t M, element_t sk, element_t Z, element_t g, pairing_t pairing) ;

void Eval4(element_t res, element_t s, element_t a[], int index, pairing_t pairing);

typedef struct{
    element_t c1;
    element_t c2;
}ct;

/* Pac, DePac:
*all assumed to be initialized in Zr
*/
void Pac(element_t pac, element_t r, element_t tpa); 
void DePac(element_t pac, element_t r, element_t tpa); 
void short_rand_Zr(element_t res);


#endif
