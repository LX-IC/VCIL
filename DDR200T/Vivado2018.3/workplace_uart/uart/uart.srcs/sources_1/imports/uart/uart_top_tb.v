module uart_top_tb();
    
    reg  clk_sys;
    reg  rst_n;
    reg  uart_in;
    wire uart_out;
    wire parity;
    
    uart_top u_uart_top(
        .i_clk_sys(clk_sys),
        .i_rst_n(rst_n),
        .i_uart_rx(uart_in),
        .o_uart_tx(uart_out),
        .o_ld_parity(parity)
    );   
        
    initial begin
        clk_sys = 1'b0;
        forever begin
            #5 clk_sys = ~clk_sys;
        end
    end

    initial begin
        rst_n      = 1'b0;
        #100 rst_n = 1'b1;
    end

    initial begin
        uart_in = 1'b1;
    end
    
    localparam ELEMENT_TIME = 8680; //1/115200
    reg [7:0] DATA0 = 8'h7b;
    reg [7:0] DATA1 = 8'h73;
    reg [7:0] DATA2 = 8'ha2;
    reg [7:0] DATA3 = 8'h92;
    reg [7:0] DATA4 = 8'hff;
    reg [7:0] DATA5 = 8'ha1;
    reg [7:0] DATA6 = 8'h82;
    reg [7:0] DATA7 = 8'h83;
    reg [7:0] DATA8 = 8'hcb;
    reg [7:0] DATA9 = 8'h69;

    // reg [7:0] DATA0 = 8'h7b;
    // reg [7:0] DATA1 = 8'h71;
    // reg [7:0] DATA2 = 8'h12;
    // reg [7:0] DATA3 = 8'h34;
    // reg [7:0] DATA4 = 8'h56;
    // reg [7:0] DATA5 = 8'h78;
    // reg [7:0] DATA6 = 8'h00;
    // reg [7:0] DATA7 = 8'h00;
    // reg [7:0] DATA8 = 8'h00;
    // reg [7:0] DATA9 = 8'h00;
    
    initial begin
        #20000;       
        uart_in = 1'b0;
        #ELEMENT_TIME
        uart_in = DATA0[0];
        #ELEMENT_TIME
        uart_in = DATA0[1];
        #ELEMENT_TIME
        uart_in = DATA0[2];
        #ELEMENT_TIME
        uart_in = DATA0[3];
        #ELEMENT_TIME
        uart_in = DATA0[4];
        #ELEMENT_TIME
        uart_in = DATA0[5];
        #ELEMENT_TIME
        uart_in = DATA0[6];
        #ELEMENT_TIME
        uart_in = DATA0[7];
        #ELEMENT_TIME
        uart_in = 1'b1;
        #ELEMENT_TIME
        uart_in = 1'b1;
        #ELEMENT_TIME

        uart_in = 1'b0;
        #ELEMENT_TIME
        uart_in = DATA1[0];
        #ELEMENT_TIME
        uart_in = DATA1[1];
        #ELEMENT_TIME
        uart_in = DATA1[2];
        #ELEMENT_TIME
        uart_in = DATA1[3];
        #ELEMENT_TIME
        uart_in = DATA1[4];
        #ELEMENT_TIME
        uart_in = DATA1[5];
        #ELEMENT_TIME
        uart_in = DATA1[6];
        #ELEMENT_TIME
        uart_in = DATA1[7];
        #ELEMENT_TIME
        uart_in = 1'b1;
        #ELEMENT_TIME
        uart_in = 1'b1;
        #ELEMENT_TIME

        uart_in = 1'b0;
        #ELEMENT_TIME
        uart_in = DATA2[0];
        #ELEMENT_TIME
        uart_in = DATA2[1];
        #ELEMENT_TIME
        uart_in = DATA2[2];
        #ELEMENT_TIME
        uart_in = DATA2[3];
        #ELEMENT_TIME
        uart_in = DATA2[4];
        #ELEMENT_TIME
        uart_in = DATA2[5];
        #ELEMENT_TIME
        uart_in = DATA2[6];
        #ELEMENT_TIME
        uart_in = DATA2[7];
        #ELEMENT_TIME
        uart_in = 1'b1;
        #ELEMENT_TIME
        uart_in = 1'b1;
        #ELEMENT_TIME

        uart_in = 1'b0;
        #ELEMENT_TIME
        uart_in = DATA3[0];
        #ELEMENT_TIME
        uart_in = DATA3[1];
        #ELEMENT_TIME
        uart_in = DATA3[2];
        #ELEMENT_TIME
        uart_in = DATA3[3];
        #ELEMENT_TIME
        uart_in = DATA3[4];
        #ELEMENT_TIME
        uart_in = DATA3[5];
        #ELEMENT_TIME
        uart_in = DATA3[6];
        #ELEMENT_TIME
        uart_in = DATA3[7];
        #ELEMENT_TIME
        uart_in = 1'b1;
        #ELEMENT_TIME
        uart_in = 1'b1;
        #ELEMENT_TIME

        uart_in = 1'b0;
        #ELEMENT_TIME
        uart_in = DATA4[0];
        #ELEMENT_TIME
        uart_in = DATA4[1];
        #ELEMENT_TIME
        uart_in = DATA4[2];
        #ELEMENT_TIME
        uart_in = DATA4[3];
        #ELEMENT_TIME
        uart_in = DATA4[4];
        #ELEMENT_TIME
        uart_in = DATA4[5];
        #ELEMENT_TIME
        uart_in = DATA4[6];
        #ELEMENT_TIME
        uart_in = DATA4[7];
        #ELEMENT_TIME
        uart_in = 1'b1;
        #ELEMENT_TIME
        uart_in = 1'b1;
        #ELEMENT_TIME

        uart_in = 1'b0;
        #ELEMENT_TIME
        uart_in = DATA5[0];
        #ELEMENT_TIME
        uart_in = DATA5[1];
        #ELEMENT_TIME
        uart_in = DATA5[2];
        #ELEMENT_TIME
        uart_in = DATA5[3];
        #ELEMENT_TIME
        uart_in = DATA5[4];
        #ELEMENT_TIME
        uart_in = DATA5[5];
        #ELEMENT_TIME
        uart_in = DATA5[6];
        #ELEMENT_TIME
        uart_in = DATA5[7];
        #ELEMENT_TIME
        uart_in = 1'b1;
        #ELEMENT_TIME
        uart_in = 1'b1;
        #ELEMENT_TIME

        uart_in = 1'b0;
        #ELEMENT_TIME
        uart_in = DATA6[0];
        #ELEMENT_TIME
        uart_in = DATA6[1];
        #ELEMENT_TIME
        uart_in = DATA6[2];
        #ELEMENT_TIME
        uart_in = DATA6[3];
        #ELEMENT_TIME
        uart_in = DATA6[4];
        #ELEMENT_TIME
        uart_in = DATA6[5];
        #ELEMENT_TIME
        uart_in = DATA6[6];
        #ELEMENT_TIME
        uart_in = DATA6[7];
        #ELEMENT_TIME
        uart_in = 1'b1;
        #ELEMENT_TIME
        uart_in = 1'b1;
        #ELEMENT_TIME

        uart_in = 1'b0;
        #ELEMENT_TIME
        uart_in = DATA7[0];
        #ELEMENT_TIME
        uart_in = DATA7[1];
        #ELEMENT_TIME
        uart_in = DATA7[2];
        #ELEMENT_TIME
        uart_in = DATA7[3];
        #ELEMENT_TIME
        uart_in = DATA7[4];
        #ELEMENT_TIME
        uart_in = DATA7[5];
        #ELEMENT_TIME
        uart_in = DATA7[6];
        #ELEMENT_TIME
        uart_in = DATA7[7];
        #ELEMENT_TIME
        uart_in = 1'b1;
        #ELEMENT_TIME
        uart_in = 1'b1;
        #ELEMENT_TIME

        uart_in = 1'b0;
        #ELEMENT_TIME
        uart_in = DATA8[0];
        #ELEMENT_TIME
        uart_in = DATA8[1];
        #ELEMENT_TIME
        uart_in = DATA8[2];
        #ELEMENT_TIME
        uart_in = DATA8[3];
        #ELEMENT_TIME
        uart_in = DATA8[4];
        #ELEMENT_TIME
        uart_in = DATA8[5];
        #ELEMENT_TIME
        uart_in = DATA8[6];
        #ELEMENT_TIME
        uart_in = DATA8[7];
        #ELEMENT_TIME
        uart_in = 1'b1;
        #ELEMENT_TIME
        uart_in = 1'b1;
        #ELEMENT_TIME

        uart_in = 1'b0;
        #ELEMENT_TIME
        uart_in = DATA9[0];
        #ELEMENT_TIME
        uart_in = DATA9[1];
        #ELEMENT_TIME
        uart_in = DATA9[2];
        #ELEMENT_TIME
        uart_in = DATA9[3];
        #ELEMENT_TIME
        uart_in = DATA9[4];
        #ELEMENT_TIME
        uart_in = DATA9[5];
        #ELEMENT_TIME
        uart_in = DATA9[6];
        #ELEMENT_TIME
        uart_in = DATA9[7];
        #ELEMENT_TIME
        uart_in = 1'b1;
        #ELEMENT_TIME
        uart_in = 1'b1;  
       
    end

endmodule
