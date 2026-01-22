#include "Bridge_Socket_Uart.h"

Bridge_Socket_Uart::Bridge_Socket_Uart(sc_core::sc_module_name name, uint32_t Max_address, uint32_t cycle)
:sc_module(name),
 p_Max_address(Max_address),
 SM4_p_cycle(cycle),
 socket("socket")
{
// Bridge_Socket_Uart::Bridge_Socket_Uart(sc_core::sc_module_name name, uint32_t Max_address, uint32_t cycle)
// :sc_module(name),
//  p_Max_address(Max_address),
//  SM4_p_cycle(cycle),
//  socket("socket"),
//  PwtModule(this)
// {
    // #ifdef LIBTLMPWT
	// set_activity(SM4_set_activity);//libtlmpwt
	// #endif
    assert(p_Max_address >= 0 && SM4_p_cycle >= 0);
    socket.register_b_transport(this, &Bridge_Socket_Uart::b_transport);
}

void Bridge_Socket_Uart::b_transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay){
    tlm::tlm_command cmd = trans.get_command();
    uint32_t addr = (uint32_t)trans.get_address();
    uint32_t *data = (uint32_t*)trans.get_data_ptr();

    trans.set_response_status(tlm::TLM_OK_RESPONSE);

    if(cmd == tlm::TLM_WRITE_COMMAND) {
        SM4_write(addr, *data);
    }
    else if(cmd == tlm::TLM_READ_COMMAND) {
        SM4_read(addr, data);
    }
    else trans.set_response_status(tlm::TLM_COMMAND_ERROR_RESPONSE);

    // #ifdef LIBTLMPWT
    // sc_core::sc_time clock_cycle = sc_core::sc_time(10, sc_core::SC_NS);
	// add_activity(SM4_add_activity, (delay.value() / clock_cycle.value())*mul_cycle);//libtlmpwt
	// #endif
}

int32_t Bridge_Socket_Uart::open_dev(const char *device){
    /* 打开串口终端 */
    fd = open(device, O_RDWR | O_NOCTTY);
    if (0 > fd) {
        fprintf(stderr, "open error: %s: %s\n", device, strerror(errno));
        return -1;
    }

    /* 获取串口当前的配置参数 */
    if (0 > tcgetattr(fd, &old_cfg)) {
        fprintf(stderr, "tcgetattr error: %s\n", strerror(errno));
        close(fd);
        return -1;
    }

    return 0;
}

int32_t Bridge_Socket_Uart::set_dev(int32_t fd){
    /* 设置为原始模式 */
    cfmakeraw(&new_cfg);

    /* 使能接收 */
    new_cfg.c_cflag |= CREAD;

    /* 设置波特率 */
    if (0 > cfsetspeed(&new_cfg, B115200)) {
        fprintf(stderr, "cfsetspeed error: %s\n", strerror(errno));
        return -1;
    }

    /* 设置数据位大小 */
    new_cfg.c_cflag &= ~CSIZE;
    new_cfg.c_cflag |= CS8;

    /* 设置奇偶校验 */
    new_cfg.c_cflag &= ~PARENB;
    new_cfg.c_iflag &= ~INPCK;

    /* 设置停止位 */
    new_cfg.c_cflag &= ~CSTOPB;

    /* 将MIN和TIME设置为0 */
    new_cfg.c_cc[VTIME] = 0;
    new_cfg.c_cc[VMIN] = 0;

     /* 清空缓冲区 */
    if (0 > tcflush(fd, TCIOFLUSH)) {
        fprintf(stderr, "tcflush error: %s\n", strerror(errno));
        return -1;
    }

    /* 写入配置、使配置生效 */
    if (0 > tcsetattr(fd, TCSANOW, &new_cfg)) {
        fprintf(stderr, "tcsetattr error: %s\n", strerror(errno));
        return -1;
    }

    /* 配置OK,退出 */
    return 0;
}

/*///////////////////////////////////////////
* SM4_Uart register address:
*
* addr_x_in_0  = 32'h50004000;
* addr_x_in_1  = 32'h50004004;
* addr_x_in_2  = 32'h50004008;
* addr_x_in_3  = 32'h5000400c;
* addr_x_in_4  = 32'h50004010;
*
* addr_y_out_0 = 32'h50004014;
*
* addr_state   = 32'h50004018;
*
*///////////////////////////////////////////

void Bridge_Socket_Uart::SM4_write(uint32_t addr, uint32_t data){

#ifdef DEBUG_Bridge_Socket_Uart
    cout << hex << "write_addr: " << addr << endl;
    cout << hex << "write_data: " << data << endl;
#endif

    if(addr == 0x00) {
        write_buff[0] = (uint8_t)((data>>24)&0x000000ff);
        write_buff[1] = (uint8_t)((data>>16)&0x000000ff);
        write_buff[2] = (uint8_t)((data>>8)&0x000000ff);
        write_buff[3] = (uint8_t)(data&0x000000ff);
        device = "/dev/ttyUSB1";
        open_dev(device);
        set_dev(fd);
        write(fd, write_buff, 4);
        while(1)
        {
            nread = read(fd, read_buff, 1);
            if (nread > 0) {
                state = 1;
                break;
            }
        }
        close(fd);
    }
    else if(addr == 0x04) {
        write_buff[0] = (uint8_t)((data>>24)&0x000000ff);
        write_buff[1] = (uint8_t)((data>>16)&0x000000ff);
        write_buff[2] = (uint8_t)((data>>8)&0x000000ff);
        write_buff[3] = (uint8_t)(data&0x000000ff);
        device = "/dev/ttyUSB1";
        open_dev(device);
        set_dev(fd);
        write(fd, write_buff, 4);
        while(1)
        {
            nread = read(fd, read_buff, 1);
            if (nread > 0) {
                state = 2;
                break;
            }
        }
        close(fd);
    }
    else if(addr == 0x08) {
        write_buff[0] = (uint8_t)((data>>24)&0x000000ff);
        write_buff[1] = (uint8_t)((data>>16)&0x000000ff);
        write_buff[2] = (uint8_t)((data>>8)&0x000000ff);
        write_buff[3] = (uint8_t)(data&0x000000ff);
        device = "/dev/ttyUSB1";
        open_dev(device);
        set_dev(fd);
        write(fd, write_buff, 4);
        while(1)
        {
            nread = read(fd, read_buff, 1);
            if (nread > 0) {
                state = 3;
                break;
            }
        }
        close(fd);
    }
    else if(addr == 0x0c) {
         read_buff[0] = 0;
        write_buff[0] = (uint8_t)(data&0x000000ff);
        device = "/dev/ttyUSB1";
        open_dev(device);
        set_dev(fd);
        while(1) {
            if (read_buff[0] == 1) {
                state = 4;
                break;
            }
            else {
                write(fd, write_buff, 1);
                while(1)
                {
                    nread = read(fd, read_buff, 1);
                    if (nread > 0) {
                        break;
                    }
                }
            }
        }
        close(fd);
    }
    else if(addr == 0x10) {
        write_buff[0] = (uint8_t)(data&0x000000ff);
        device = "/dev/ttyUSB1";
        open_dev(device);
        set_dev(fd);
        write(fd, write_buff, 1);
        while(1)
        {
            nread = read(fd, read_buff, 5);
            if (nread > 0) {
                state = 5;
                break;
            }
        }
        close(fd);
    }
    else return;
}

void Bridge_Socket_Uart::SM4_read(uint32_t addr, uint32_t *data){

    if(addr == 0x18)
        *data = state;
    else if(addr == 0x14)
        *data = (uint32_t)(((read_buff[0]<<24)&0xff000000)+((read_buff[1]<<16)&0x00ff0000)+((read_buff[2]<<8)&0x0000ff00)+(read_buff[3]&0x000000ff));
    else return;

#ifdef DEBUG_Bridge_Socket_Uart
    cout << hex << "read_addr: " <<  addr << endl;
    cout << hex << "read_data: " << *data << endl;
#endif

}