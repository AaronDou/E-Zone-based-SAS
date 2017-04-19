#include "common.h"
#include "parameters.h"


int main(int argc, char **argv) {
	
	pairing_t pairing;
	element_t g, Z;
	element_t a, b, pk_a, pk_b, rk;
	element_t tempz, temp1, tempt;
        
        // Timer variables
	double t0, t1;

	int i, k;
	double temp;
	
        // Read cryptosystem parameters
	const char *param_file[2] = { "", "param/a.param" };
	pbc_demo_pairing_init(pairing, 2, param_file);

	element_init_Zr(tempz, pairing);
	
	
	printf("===== E-Zone-based SAS test=====\n");
	element_init_G1(g, pairing);
	element_init_GT(Z, pairing);

	element_random(g);
	element_pairing(Z, g, g);
	

	printf("* Key generation\n");

	element_init_Zr(a, pairing);
	element_init_G1(pk_a, pairing);
 	t0 = pbc_get_time();

	element_random(a);	
	element_pow_zn(pk_a,g,a);

  	t1 = pbc_get_time();

	printf("Established IU key pair, time elapsed %fms\n",(t1-t0)*1000);

	element_init_Zr(b, pairing);
	element_init_G1(pk_b, pairing);
 	t0 = pbc_get_time();

	element_random(b);	
	element_pow_zn(pk_b,g,b);

  	t1 = pbc_get_time();

	printf("Established SU key pair, time elapsed %fms\n",(t1-t0)*1000);

	element_init_G1(rk, pairing);
 	t0 = pbc_get_time();

	element_invert(tempz, a);
	element_pow_zn(rk, pk_b, tempz);

  	t1 = pbc_get_time();

	printf("Established conversion key, time elapsed %fms\n",(t1-t0)*1000);


        
        printf("\n# of entries in T_k:%d\n", N);
        
	ct * T_ct = (ct *)malloc(N * sizeof(ct));
        init_array_ct(T_ct, pairing);
        
        // Aggregation
        for(k = 0; k < IUs; k++){
            t0 = pbc_get_time();
            ct* T_k_ct = generate_e_zone_ct(pk_a, Z, pairing);
            t1 = pbc_get_time();
            printf("E-Zone data generation of IU %d, time elapsed %fms\n",k, (t1-t0)*1000);
            
            
            t0 = pbc_get_time();
            for(i = 0; i < N; i++){
                element_mul((T_ct+i)->c1, (T_ct+i)->c1, (T_k_ct+i)->c1);
                element_mul((T_ct+i)->c2, (T_ct+i)->c2, (T_k_ct+i)->c2);
            }
            t1 = pbc_get_time();
            printf("Aggregation of IU %d, time elapsed %fms\n",k, (t1-t0)*1000);
        }
        
        printf("\nStarting to serve SU......\n");
        // SU processing       	
        int su_id = 0;
	element_t c1o;
	element_t c2o;
	element_init_GT(c1o, pairing);
	element_init_GT(c2o, pairing);

 	t0 = pbc_get_time();
	ReEnc(c1o, c2o, (T_ct+su_id)->c1, (T_ct+su_id)->c2, rk);
  	t1 = pbc_get_time();

	printf("\nData rencryption to SU's key, time elapsed %fms\n",(t1-t0)*1000);

	element_t Md;
	element_init_GT(Md, pairing);
	
	
 	t0 = pbc_get_time();
	ReDec(Md, c1o, c2o, b, pairing);
  	t1 = pbc_get_time();
	printf("\nData decryption by SU's key, time elapsed %fms\n",(t1-t0)*1000);
	element_printf("Decrypted plaintext: %B\n", Md);		
}