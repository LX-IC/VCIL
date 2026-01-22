#include "insn.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core_feature_base.h"


int main(void)
{
    unsigned char* message = "abc";
    unsigned int message_len = 3;

    unsigned int begin_cycle, end_cycle, begin_instr, end_instr;
    begin_cycle = __get_rv_cycle();
    begin_instr = __get_rv_instret();
    sm3_nice(message, message_len);
    end_cycle = __get_rv_cycle() - begin_cycle;
    end_instr = __get_rv_instret() - begin_instr;
    printf("end_cycle = %d \n", end_cycle);
    printf("end_instr = %d \n", end_instr);

    message = "abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcd";
    message_len = 64;

    begin_cycle = __get_rv_cycle();
    begin_instr = __get_rv_instret();
    sm3_nice(message, message_len);
    end_cycle = __get_rv_cycle() - begin_cycle;
    end_instr = __get_rv_instret() - begin_instr;
    printf("end_cycle = %d \n", end_cycle);
    printf("end_instr = %d \n", end_instr);

    // unsigned int X  = 0x12345678;
    // unsigned int P0;
    // for (int i=0; i<100; i++) {
    //     P0 = custom3_sm3p0(X);
    // }

    // unsigned int X  = 0x12345678;
    // unsigned int P1;
    // for (int i=0; i<100; i++) {
    //     P1 = custom3_sm3p1(X);
    // }

    return 0;
}
