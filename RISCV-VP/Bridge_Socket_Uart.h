#ifndef Bridge_Socket_Uart_H
#define Bridge_Socket_Uart_H

#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "systemc.h"
#include <iostream>

#include <stdio.h>      /*标准输入输出*/
#include <string.h>     /*标准字符串处理*/
#include <stdlib.h>     /*标准函数库*/
#include <unistd.h>     /*Unix标准函数*/
#include <sys/types.h>  /*数据类型定义，比如一些XXX_t的类型*/
#include <sys/stat.h>   /*返回值结构*/
#include <fcntl.h>      /*文件控制*/
#include <termios.h>    /*PPSIX终端控制*/
#include <errno.h>      /*错误号*/

#include "define.h"

// #include "core/rv32/libtlmpwt.h"//libtlmpwt
// #include "platform/pwt/pwt_module.hpp"//libtlmpwt

using namespace std;
using namespace tlm;
using namespace sc_core;

class Bridge_Socket_Uart:public sc_module{
// class Bridge_Socket_Uart:public sc_module, public PwtModule{//libtlmpwt
    public:
        int32_t fd = 0;

        struct termios old_cfg = {0};
        struct termios new_cfg = {0};

        uint8_t nread = 0;
        uint8_t write_buff[4] = {0};
        uint8_t read_buff[5]  = {0};
        const char *device;
        uint32_t state = 0;

        void b_transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay);

        int32_t open_dev(const char *device);
        int32_t set_dev(int32_t fd);

        void SM4_read(uint32_t addr, uint32_t *data);
        void SM4_write(uint32_t addr, uint32_t data);

        tlm_utils::simple_target_socket<Bridge_Socket_Uart> socket;

        Bridge_Socket_Uart(sc_core::sc_module_name name, uint32_t Max_address, uint32_t cycle);

    private:
        uint32_t p_Max_address;
        uint32_t SM4_p_cycle;
};

#endif
