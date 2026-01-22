#include "sm4.h"
#include "core_feature_base.h"
#include <time.h>

int main(void)
{
    unsigned int key[4] = {
        0x01234567,
        0x89abcdef,
        0xfedcba98,
        0x76543210
    };
    unsigned int cipher[4] = {
        0x01234567,
        0x89abcdef,
        0xfedcba98,
        0x76543210
    };

    unsigned int begin_cycle, end_cycle, begin_instr, end_instr;
    begin_cycle = __get_rv_cycle();
    begin_instr = __get_rv_instret();
    sm4_nice(key, cipher);
    end_cycle = __get_rv_cycle() - begin_cycle;
    end_instr = __get_rv_instret() - begin_instr;
    printf("end_cycle = %d \n", end_cycle);
    printf("end_instr = %d \n", end_instr);

    // unsigned int K  = 0xa292ffa1;
    // unsigned int TK = 0x8283cb69;
    // unsigned int rk;
    // for (int i=0; i<100; i++) {
    //     rk = custom3_sm4ks(K, TK);
    // }

    // unsigned int X  = 0x01234567;
    // unsigned int TX = 0xf002c39e;
    // unsigned int Y;
    // for (int i=0; i<100; i++) {
    //     Y = custom3_sm4ed(X, TX);
    // }

    return 0;
}
