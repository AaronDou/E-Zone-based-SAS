#include "common.h"

void hash(element_t output, char* input){
  unsigned char ibuf[LENGTH];
  unsigned char obuf[LENGTH];	

  memset(ibuf, 0, sizeof ibuf);
  strcat(ibuf,input);
  SHA1(ibuf,strlen(ibuf),obuf); 
  element_from_hash(output,obuf,LENGTH); 
}

void Enc(element_t c1, element_t c2, element_t M, element_t pk, element_t Z, pairing_t pairing) {
// everything should be initialized!!!
// M, c1: Gt
// pk, c2 in G1
// Z= e(g,g)
	
	element_t r, tempt;
	element_init_Zr(r, pairing);
	element_init_GT(tempt, pairing);
	element_random(r);
//        element_printf("Print r, %B\n", r);
        
	element_pow_zn(tempt, Z, r);
	element_mul(c1, tempt, M);
	element_pow_zn(c2, pk, r);    
}

//still feasible by c2= e(g,pk)^r
void Enc2(element_t c1, element_t c2, element_t M, element_t pk, element_t Z, element_t g, pairing_t pairing) {
// everything should be initialized!!!
// M, c1, c2: Gt
// pk, in G1
// Z= e(g,g)
	
	element_t r, tempt;
	element_init_Zr(r, pairing);
	element_init_GT(tempt, pairing);
	element_random(r);

	element_pow_zn(tempt, Z, r);

	element_mul(c1, tempt, M);

	element_pairing(tempt, g, pk);
	element_pow_zn(c2, tempt, r);
}

void ReEnc(element_t c1o, element_t c2o, element_t c1i, element_t c2i, element_t rk) {
// everything should be initialized!!!
// c1i, c1o, c2o: Gt
// rk, c2i in G1
// Z= e(g,g)
	element_set(c1o, c1i);
	element_pairing(c2o, c2i, rk);	
}


void Dec(element_t M, element_t c1, element_t c2, element_t sk, element_t g, pairing_t pairing){
	element_t tempz, tempt, temp1;
	element_init_Zr(tempz, pairing);
	element_init_GT(tempt, pairing);
	element_init_G1(temp1, pairing);

	element_invert(tempz, sk);
	element_pow_zn(temp1, c2, tempz);
	element_pairing(tempt, temp1, g);

	element_div(M, c1, tempt);
}

void ReDec(element_t M, element_t c1, element_t c2, element_t sk, pairing_t pairing){
	element_t tempz, tempt;
	element_init_Zr(tempz, pairing);
	element_init_GT(tempt, pairing);

	element_invert(tempz, sk);
	element_pow_zn(tempt, c2, tempz);

	element_div(M, c1, tempt);
}

