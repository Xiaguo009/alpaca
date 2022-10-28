
#include <app/app_api.h>
#include <app/app_global.h>
#include <scheduling/scheduler.h>
#include <scheduling/scheduler_based_task.h>

__SHARED_VAR(
uint16_t candidate_e;
uint16_t e_number;
uint16_t enc_index;
uint16_t dec_index;

uint16_t public_n;
uint16_t phi_n;
uint16_t sqrt_candidate_e;

uint16_t secret_d[RSA_MSGLENGTH];
uint16_t public_e[RSA_MSGLENGTH];
uint16_t enc_cipher[RSA_MSGLENGTH];
uint16_t dec_plain[RSA_MSGLENGTH];
)


__TASK_ENTRY(init,

__GET(public_n) = p * q;
__GET(phi_n) = (p - 1) * (q - 1);
__GET(candidate_e) = p + q - 1;
__GET(e_number) = 0;
__GET(enc_index) = 0;
__GET(dec_index) = 0;

return 1;

)

__TASK(1, get_e_sqrt,

__GET(sqrt_candidate_e) = RSA_Sqrt16(__GET(candidate_e));
return 2;

)

__TASK(2, e_verify,

if (RSA_isPrime(__GET(candidate_e), __GET(sqrt_candidate_e)))
   __GET(public_e[__GET(e_number)++]) = __GET(candidate_e);

if (__GET(e_number) < RSA_MSGLENGTH)
{
   __GET(candidate_e) += 2;
   return 1;
}

return 3;

)

__TASK(3, enc_main,

uint16_t i = __GET(enc_index)++;
__GET(enc_cipher[i]) =
       RSA_PowerMod(rsa_msg[i], __GET(public_e[i]), __GET(public_n));

if (__GET(enc_index) < RSA_MSGLENGTH)
    return 3;
else
    return 4;
)

__TASK(4, calculate_d_main,

uint16_t i = __GET(dec_index);
__GET(secret_d[i]) = RSA_ModInv(__GET(public_e[i]), __GET(phi_n));
return 5;

)

__TASK(5, dec_main,

uint16_t i = __GET(dec_index)++;
__GET(dec_plain[i]) =
       RSA_PowerMod(__GET(enc_cipher[i]), __GET(secret_d[i]), __GET(public_n));

if (__GET(dec_index) < RSA_MSGLENGTH)
   return 4;
else {
    return TASK_FINISH;
}

)



void rsa_regist()
{

    task_regist(0, init,                false);
    task_regist(1, get_e_sqrt,          false);
    task_regist(2, e_verify,            true );
    task_regist(3, enc_main,            true );
    task_regist(4, calculate_d_main,    false);
    task_regist(5, dec_main,            true );

    WAR_REGIST(4);
}




