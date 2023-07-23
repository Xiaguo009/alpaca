116

static const uint16_t global_war_size = 10;
static __nv uint16_t  backup_buf[10] = {};

static const bool backup_needed[] = {
    false, true, true, false, false, false, true, true, true
};

war放一起?声明 

数组没备份?备份了


rsa
TEB(ce_1)//1

//war i

TEB(ce_4)//6

//war k

TEB(encrypt_inner_loop)//8

//war en_k en_j

TEB(encrypt_finish)//9

//war en_cnt

TEB(decrypt_inner_loop)//12

//war de_k de_j

TEB(decrypt_finish)//13

//war de_cnt

