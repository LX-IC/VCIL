set_property -dict [list \
  CONFIG_VOLTAGE {3.3} \
  CFGBVS {VCCO} \
  ] [current_design]

create_clock -period 10.00 -name i_clk_sys -waveform {0 5} [get_ports i_clk_sys]

set_property PACKAGE_PIN W19 [get_ports i_clk_sys]
set_property IOSTANDARD LVCMOS33 [get_ports i_clk_sys]
set_property PACKAGE_PIN P20 [get_ports i_rst_n]
set_property IOSTANDARD LVCMOS33 [get_ports i_rst_n]
set_property IOSTANDARD LVCMOS33 [get_ports i_uart_rx]
set_property IOSTANDARD LVCMOS33 [get_ports o_ld_parity]
set_property IOSTANDARD LVCMOS33 [get_ports o_uart_tx]
set_property PACKAGE_PIN P16 [get_ports i_uart_rx]
set_property PACKAGE_PIN R17 [get_ports o_uart_tx]
set_property PACKAGE_PIN J16 [get_ports o_ld_parity]