module nice_core (
    input            nice_clk     ,
    input            nice_rst_n   ,
    input      [7:0] i_data_rx    ,
    input            i_data_valid ,
    input            i_state_0    ,
    // input      [1:0] i_random_0_3 , 
    // input      [2:0] i_random_0_7 ,
    output reg [7:0] o_data_tx    , 
    output reg       o_data_valid
);

// State Machine
reg [3:0] state;

localparam IDLE      = 4'b0000;
localparam RECV_INST = 4'b0001;
localparam SM3_STEP1 = 4'b0010;
localparam SM3_STEP2 = 4'b0011;
localparam SM3_STEP3 = 4'b0100;
localparam SM4_STEP1 = 4'b0101;
localparam SM4_STEP2 = 4'b0110;
localparam SM4_STEP3 = 4'b0111;
localparam TX_DATA   = 4'b1000;
localparam TX_ONE    = 4'b1001;

// ID
reg  [79:0] nice_req_inst;
wire [15:0] opcode = nice_req_inst[79:64];
wire [31:0] rs1    = nice_req_inst[63:32];
wire [31:0] rs2    = nice_req_inst[31: 0];

wire custom3_sm3p0 = (opcode==16'h7b71);
wire custom3_sm3p1 = (opcode==16'h7b72);
wire custom3_sm4ks = (opcode==16'h7b73);
wire custom3_sm4ed = (opcode==16'h7b74);

// SM3
reg op_ssm3_p0;
reg op_ssm3_p1;
reg [31:0] sm3_rs1;
reg [31:0] sm3_temp1;
reg [31:0] sm3_temp2;

// SM4
reg op_ssm4_ks;
reg op_ssm4_ed;
reg [31:0] sm4_rs1;
reg [31:0] sm4_rs2;
reg [31:0] sm4_sbox_out;
reg [31:0] sm4_linear_temp;

// SM4 Sbox
wire [7:0] sbox_in;
wire [7:0] sbox_out;
reg  [2:0] sbox_sel;

assign sbox_in = (sbox_sel == 0) ? sm4_rs2[ 7: 0] :
                 (sbox_sel == 1) ? sm4_rs2[15: 8] :
                 (sbox_sel == 2) ? sm4_rs2[23:16] :
                 (sbox_sel == 3) ? sm4_rs2[31:24] : 0;

Sbox i_sm4_sbox (.data_in(sbox_in), .data_out(sbox_out));

// Others
reg calc_done;
reg state_count;
reg [31:0] calc_result;
reg [ 3:0] rx_byte_cnt;
reg [ 2:0] tx_byte_cnt;
reg [ 7:0] cycle_counter;
// reg [ 7:0] random_cycle;

// main
always @(posedge nice_clk or negedge nice_rst_n) begin
    if (!nice_rst_n) begin
        o_data_tx <= 0;
        o_data_valid <= 0;
        state <= IDLE;
        nice_req_inst <= 0;
        op_ssm3_p0 <= 0;
        op_ssm3_p1 <= 0;
        sm3_rs1 <= 0;
        sm3_temp1 <= 0;
        sm3_temp2 <= 0;
        op_ssm4_ks <= 0;
        op_ssm4_ed <= 0;
        sm4_rs1 <= 0;
        sm4_rs2 <= 0;
        sm4_sbox_out <= 0;
        sm4_linear_temp <= 0;
        sbox_sel <= 0;
        calc_done <= 0;
        state_count <= 0;
        calc_result <= 0;  
        rx_byte_cnt <= 0;
        tx_byte_cnt <= 0;
        cycle_counter <= 0; 
        // random_cycle <= 0;
    end
    else begin

        if (1 < state && state < 8 && calc_done == 0) begin
            cycle_counter <= cycle_counter + 1;
        end

        if (i_data_valid && i_data_rx == 8'hff && state != IDLE && state != TX_DATA && state != TX_ONE && i_state_0 == 0 && state_count == 0) begin
            o_data_tx <= calc_done;
            o_data_valid <= 1;
            state_count <= 1;                  
        end
        else if (i_state_0 == 1) begin
            o_data_valid <= 0;
            state_count <= 0;
        end

        if (i_data_valid && i_data_rx == 8'h00 && (state == SM3_STEP3 || state == SM4_STEP3) && calc_done == 1) begin
            state <= TX_DATA;
        end
    
        case (state)
            // rx
            IDLE: begin
                if (i_data_valid && rx_byte_cnt < 12) begin
                    if (rx_byte_cnt != 2 && rx_byte_cnt != 3) begin
                        nice_req_inst <= {nice_req_inst[71:0], i_data_rx};
                    end
                    if ((rx_byte_cnt + 1) % 4 == 0) begin
                        state <= TX_ONE;
                    end
                    else begin
                        state <= IDLE;
                    end
                    rx_byte_cnt <= rx_byte_cnt + 1;
                end 
                else if (rx_byte_cnt == 12) begin
                    state <= RECV_INST;
                end 
                else begin
                    state <= IDLE;
                end
            end
            
            // ID
            RECV_INST: begin
                if (rx_byte_cnt == 12) begin
                    if (custom3_sm3p0 || custom3_sm3p1) begin
                        sm3_rs1 <= rs1;
                        op_ssm3_p0 <= custom3_sm3p0;
                        op_ssm3_p1 <= custom3_sm3p1;
                        // random_cycle <= i_random_0_3;
                        state <= SM3_STEP1;
                    end 
                    else if (custom3_sm4ks || custom3_sm4ed) begin
                        sm4_rs1 <= rs1;
                        sm4_rs2 <= rs2;
                        op_ssm4_ks <= custom3_sm4ks;
                        op_ssm4_ed <= custom3_sm4ed;
                        // random_cycle <= i_random_0_7;
                        state <= SM4_STEP1;
                    end
                    rx_byte_cnt <= 0;
                end 
                else begin
                    state <= IDLE;
                end
            end
            
            // SM3
            SM3_STEP1: begin
                sm3_temp1 <= op_ssm3_p0 ? (sm3_rs1 <<  9) | (sm3_rs1 >> (32- 9)) : 
                             op_ssm3_p1 ? (sm3_rs1 << 15) | (sm3_rs1 >> (32-15)) : 0; // ROL32(9) or ROL32(15)
                state <= SM3_STEP2;
            end
            SM3_STEP2: begin
                sm3_temp2 <= op_ssm3_p0 ? (sm3_rs1 << 17) | (sm3_rs1 >> (32-17)) :
                             op_ssm3_p1 ? (sm3_rs1 << 23) | (sm3_rs1 >> (32-23)) : 0; // ROL32(17) or ROL32(23)
                state <= SM3_STEP3;
            end
            SM3_STEP3: begin
                calc_result <= sm3_rs1 ^ sm3_temp1 ^ sm3_temp2;
                calc_done <= 1;
                // state <= TX_DATA;
            end
            
            // SM4
            SM4_STEP1: begin
                if (sbox_sel == 4) begin
                    sbox_sel <= 0;
                    state <= SM4_STEP2;
                end 
                else begin
                    sm4_sbox_out <= {sbox_out, sm4_sbox_out[31:8]};
                    sbox_sel <= sbox_sel + 1;
                end
            end
            SM4_STEP2: begin
                sm4_linear_temp <= op_ssm4_ks ? sm4_sbox_out ^ {sm4_sbox_out[18:0], sm4_sbox_out[31:19]}
                                                             ^ {sm4_sbox_out[ 8:0], sm4_sbox_out[31: 9]} :
                                   op_ssm4_ed ? sm4_sbox_out ^ {sm4_sbox_out[29:0], sm4_sbox_out[31:30]}
                                                             ^ {sm4_sbox_out[21:0], sm4_sbox_out[31:22]}
                                                             ^ {sm4_sbox_out[13:0], sm4_sbox_out[31:14]}
                                                             ^ {sm4_sbox_out[ 7:0], sm4_sbox_out[31: 8]} : 0;
                state <= SM4_STEP3;
            end
            SM4_STEP3: begin
                calc_result <= sm4_linear_temp ^ sm4_rs1;
                calc_done <= 1;
                // state <= TX_DATA;
            end
            
            // tx
            TX_DATA: begin
                calc_done <= 0;
                if (i_state_0 == 0 && state_count == 0) begin
                    if (tx_byte_cnt == 5) begin
                        o_data_valid <= 0;
                        tx_byte_cnt <= 0;
                        cycle_counter <= 0;
                        state <= IDLE;
                    end
                    else begin
                        case (tx_byte_cnt)
                            0: o_data_tx <= calc_result[31:24];
                            1: o_data_tx <= calc_result[23:16];
                            2: o_data_tx <= calc_result[15: 8];
                            3: o_data_tx <= calc_result[ 7: 0];
                            4: o_data_tx <= cycle_counter;
                            // 4: o_data_tx <= cycle_counter + random_cycle;
                        endcase
                        o_data_valid <= 1;
                        state_count <= 1;
                        tx_byte_cnt <= tx_byte_cnt + 1;
                        state <= TX_DATA;
                    end                  
                end 
                else if (i_state_0 == 1) begin
                    o_data_valid <= 0;
                    state_count <= 0;
                end
            end

            //tx one
            TX_ONE: begin
                if (i_state_0 == 0 && state_count == 0) begin
                    o_data_tx <= 1;
                    o_data_valid <= 1;
                    state_count <= 1;                  
                end 
                else if (i_state_0 == 1) begin
                    o_data_valid <= 0;
                    state_count <= 0;
                    state <= IDLE;
                end
            end
            
            default: state <= IDLE;
        endcase
    end
end

endmodule