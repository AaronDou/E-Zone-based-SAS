#include "common.h"
#include "parameters.h"


int main(int argc, char **argv) {
	
	//Initializing encryption cryptosystem parameters
	pairing_t pairing;
	element_t g, Z;
	element_t a, b, pk_a, pk_b, rk;
	element_t tempz, temp1, tempt, tpa_sum;
        

        //Initializing timer variables
	double t0, t1;

	int i, k;
	double temp;
	

        // Read cryptosystem parameters
	const char *param_file[2] = { "", "param/a.param" };
	pbc_demo_pairing_init(pairing, 2, param_file);

	element_init_Zr(tempz, pairing);
	
	
	//Initializing cryptosystem
	printf("===== E-Zone-based SAS test=====\n");
	element_init_G1(g, pairing);
	element_init_GT(Z, pairing);

	element_random(g);
	element_pairing(Z, g, g);
	

	//Generating IU key pair
	printf("* Key generation\n");

	element_init_Zr(a, pairing);
	element_init_G1(pk_a, pairing);
 	t0 = pbc_get_time();

	element_random(a);	
	element_pow_zn(pk_a,g,a);

  	t1 = pbc_get_time();

	printf("Established IU key pair, time elapsed %fms\n",(t1-t0)*1000);


	//Generating SU key pair
	element_init_Zr(b, pairing);
	element_init_G1(pk_b, pairing);
 	t0 = pbc_get_time();

	element_random(b);	
	element_pow_zn(pk_b,g,b);

  	t1 = pbc_get_time();

	printf("Established SU key pair, time elapsed %fms\n",(t1-t0)*1000);


	//Calculating IU to SU re-encryption conversion key
	element_init_G1(rk, pairing);
 	t0 = pbc_get_time();

	element_invert(tempz, a);
	element_pow_zn(rk, pk_b, tempz);

  	t1 = pbc_get_time();

	printf("Established conversion key, time elapsed %fms\n",(t1-t0)*1000);



        // IU E-Zone generation and aggregation
        printf("\n# of entries in T_k:%d\n", N);
        
	ct * T_ct = (ct *)malloc(N * sizeof(ct));
        init_array_ct(T_ct, pairing);
        ct * R_ct = (ct *)malloc(N * sizeof(ct));
        init_array_ct(R_ct, pairing);

	element_init_Zr(tpa_sum, pairing);
	element_set0(tpa_sum);
 
        for(k = 0; k < IUs; k++){
	    ct * R_k_ct = (ct *)malloc(N * sizeof(ct));
            init_array_ct(R_k_ct, pairing);
            //t0 = pbc_get_time();
	    printf("E-Zone data generation with of IU %d, time elapsed ",k); 
            ct* T_k_ct = generate_e_zone_ct(pk_a, Z, pairing, R_k_ct, tpa_sum);
            //t1 = pbc_get_time();
            //printf("E-Zone data generation of IU %d, time elapsed %fms\n",k, (t1-t0)*1000);      
            
            t0 = pbc_get_time();
            if (k == 0){
                for(i = 0; i < N; i++){          
                    element_set((T_ct+i)->c1, (T_k_ct+i)->c1);
                    element_set((T_ct+i)->c2, (T_k_ct+i)->c2);
		    element_set((R_ct+i)->c1, (R_k_ct+i)->c1);
                    element_set((R_ct+i)->c2, (R_k_ct+i)->c2);
                }
            }
            else{
                for(i = 0; i < N; i++){          
                    element_mul((T_ct+i)->c1, (T_ct+i)->c1, (T_k_ct+i)->c1);
                    element_mul((T_ct+i)->c2, (T_ct+i)->c2, (T_k_ct+i)->c2);
		    element_mul((R_ct+i)->c1, (R_ct+i)->c1, (R_k_ct+i)->c1);
                    element_mul((R_ct+i)->c2, (R_ct+i)->c2, (R_k_ct+i)->c2);
                }
            }
            
            t1 = pbc_get_time();
            printf("Aggregation of IU %d, time elapsed %fms\n\n",k, (t1-t0)*1000);
        }
        
        printf("\n* Starting to serve SU......\n");
        // SU processing       	
        int su_id = 0;
	element_t c1o;
	element_t c2o;
	element_init_GT(c1o, pairing);
	element_init_GT(c2o, pairing);
	element_t r1o;
	element_t r2o;
	element_init_GT(r1o, pairing);
	element_init_GT(r2o, pairing);

 	t0 = pbc_get_time();
	ReEnc(c1o, c2o, (T_ct+su_id)->c1, (T_ct+su_id)->c2, rk);
	ReEnc(r1o, r2o, (R_ct+su_id)->c1, (R_ct+su_id)->c2, rk);
  	t1 = pbc_get_time();

	printf("\nData rencryption to SU's key, time elapsed %fms\n",(t1-t0)*1000);

	element_t Md;
	element_init_GT(Md, pairing);
	element_t Rd;
	element_init_GT(Rd, pairing);
	
 	t0 = pbc_get_time();
	ReDec(Md, c1o, c2o, b, pairing);
	ReDec(Rd, r1o, r2o, b, pairing);
  	t1 = pbc_get_time();

	printf("\nData decryption by SU's key, time elapsed %fms\n",(t1-t0)*1000);
	element_printf("Decrypted plaintext: %B\n", Md);
	element_printf("Decrypted R: %B\n", Rd);
	element_div(Md, Md, Rd);	
	element_printf("Depacked plaintext: %B\n", Md);

	element_t tpa_SUM;
	element_init_GT(tpa_SUM, pairing);
	element_pow_zn(tpa_SUM, Z, tpa_sum);
	printf("TPA Confirmation result: %d\n", element_cmp(tpa_SUM, Md));
}
