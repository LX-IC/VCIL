module random_generator (
    input  i_clk,
    input  i_rst_n,
    output reg [1:0] o_random_0_3, // 0-3
    output reg [2:0] o_random_0_7  // 0-7
);

    reg [15:0] lfsr;

    always @(posedge i_clk or negedge i_rst_n) begin
        if (!i_rst_n) begin
            lfsr <= 16'hACE1;
        end else begin
            lfsr <= {lfsr[14:0], lfsr[15] ^ lfsr[13] ^ lfsr[12] ^ lfsr[10]};
        end
    end

    always @(posedge i_clk) begin
        o_random_0_3 <= lfsr[1:0];
    end

    always @(posedge i_clk) begin
        o_random_0_7 <= lfsr[2:0];
    end

endmodule