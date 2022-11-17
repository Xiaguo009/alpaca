#ifndef APPS_APP_H_
#define APPS_APP_H_

//1.latics
void pc_ar_main();
void pc_bc_main();
void pc_crc_main();
void pc_cuckoo_main();
void pc_dijkstra_main();
void pc_sort_main();
void pc_sha256_main();
//void pc_aes_main();

//unused
void pc_blowfish_main();
void pc_cem_main();
void pc_rsa_main();

//2.alpaca
void alpaca_ar_main();
void alpaca_bc_main();
void alpaca_crc_main();
void alpaca_cuckoo_main();
void alpaca_dijkstra_main();
void alpaca_sort_main();
void alpaca_sha256_main();
//void alpaca_aes_main();

//unused
void alpaca_blowfish_main();
void alpaca_cem_main();
void alpaca_rsa_main();


typedef void (*FUNCPTR)();

#endif /* APPS_APP_H_ */
