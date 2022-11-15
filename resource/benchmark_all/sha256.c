//1 .h

// TODO: Modify it for 16-bit MCU.
typedef unsigned uint32_t;      
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;


typedef uint32_t WORD;

#define BIT_SET(word, bit)          word |= ((WORD)1 << (bit))
#define BIT_AND(word1, word2)       ((WORD)(word1) & (WORD)(word2))
#define BIT_NOT(word)               (~(WORD)(word))
#define BIT_XOR(word1, word2)       ((WORD)(word1) ^ (WORD)(word2))
#define SLEFT(word, bit)            ((WORD)(word) << (bit))
#define SRIGHT (word, bit)          ((WORD)(word) >> (bit))
#define ROTATE_SLEFT(word, bit)     (((WORD)(word) << (bit)) | ((WORD)(word) >> (32 - (bit))))
#define ROTATE_SRIGHT(word, bit)    (((WORD)(word) >> (bit)) | ((WORD)(word) << (32 - (bit))))

#define CH(x, y, z)     BIT_XOR(BIT_AND(x, y), BIT_AND(BIT_NOT(x), z))
#define MA(x, y, z)     BIT_XOR(BIT_XOR(BIT_AND(x, y), BIT_AND(x, z)), BIT_AND(y, z))
#define BIGSIG0(x)      BIT_XOR(BIT_XOR(ROTATE_SRIGHT(x, 2), ROTATE_SRIGHT(x, 13)), ROTATE_SRIGHT(x, 22))
#define BIGSIG1(x)      BIT_XOR(BIT_XOR(ROTATE_SRIGHT(x, 6), ROTATE_SRIGHT(x, 11)), ROTATE_SRIGHT(x, 25))
#define SIG0(x)         BIT_XOR(BIT_XOR(ROTATE_SRIGHT(x, 7), ROTATE_SRIGHT(x, 18)), SRIGHT(x, 3))
#define SIG1(x)         BIT_XOR(BIT_XOR(ROTATE_SRIGHT(x, 17), ROTATE_SRIGHT(x, 19)), SRIGHT(x, 10))

const WORD INIT_VEC[] = { 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };
const WORD RCST[] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

//only in this scope
const unsigned char HASH_MSG[] = "Hello, SHA256!"

#define __nv 

//1 .h

//1a TODO: to global_declaration.h


//__SHARED_VAR 
__nv WORD msg[16];  // message block for 512-bit.

__nv uint16_t ei, ri;
__nv WORD msg_exp[64];       // expanded message blocks.
__nv WORD msg_hash[8];       // hash results.
__nv WORD round_regs[64][8]; // round registers.


//
void vector_init() {
    for (uint16_t i = 0; i < 8; ++i)
        msg_hash[i] = INIT_VEC[i];
}

void message_padding() {
    // Set message to all 0-bits.
    memset(msg, 0, sizeof(WORD) * 16);      

    // Copy HASH_MSG to the beginning of message.
    uint16_t msg_len = strlen(HASH_MSG);
    for (uint16_t i = 0; i < msg_len; ++i)
        ((uint8_t*)msg)[i] = HASH_MSG[i];
    
    // Pad 1 in the following bit.
    uint16_t word_index = msg_len / 4;
    uint16_t bit_index = 8 * (msg_len % 4);
    BIT_SET(msg[word_index], bit_index);
    // The 0 is automatically padded at first.
    // Pad len of the message.
    msg[14] = 0;
    msg[15] = (WORD)(8 * msg_len);

    ei = 0;
    return message_expansion;
}

void message_expansion() {
    if (ei < 16) {
        msg_exp[ei] = msg[ei];
    } else {
        msg_exp[ei] = SIG1(msg_exp[ei - 2]) + msg_exp[ei - 7] + SIG0(msg_exp[ei - 15]) + SIG0(msg_exp[ei - 16]);
    }

    ei++;
    if (ei >= 64)   return message_round;
    else  return message_round_init;
}

void message_round_init() {
    for (uint16_t i = 0; i < 8; ++i) {
        round_regs[0][i] = msg_hash[i];
    }
    ri = 0;
    return message_round;
}

void message_round() {

    WORD t1 = round_regs[ri][7] + BIGSIG1(round_regs[ri][4]) + CH(round_regs[ri][4], round_regs[ri][5], round_regs[ri][6]) + RCST[ri] + msg_exp[ri];
    WORD t2 = BIGSIG0(round_regs[ri][0]) + MA(round_regs[ri][0], round_regs[ri][1], round_regs[ri][2]);

    round_regs[ri][7] = round_regs[ri][6];
    round_regs[ri][6] = round_regs[ri][5];
    round_regs[ri][5] = round_regs[ri][4];
    round_regs[ri][4] = round_regs[ri][3] + t1;
    round_regs[ri][3] = round_regs[ri][2];
    round_regs[ri][2] = round_regs[r1][1];
    round_regs[ri][1] = round_regs[ri][0];
    round_regs[ri][0] = t1 + t2;

    ri++;
    if (ri >= 64) return message_round_final;
    else return message_round;
}

void message_round_final() {
    for (uint16_t i = 0; i < 8; ++i) {
        msg_hash[i] = round_regs[63][i];
    }
}
