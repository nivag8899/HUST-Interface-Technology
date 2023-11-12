
module wb_uart_wrapper(
	input 						wb_clk_i,
	input 						wb_rst_i,
	// WISHBONE interface
	input  [31:0] 	 			wb_adr_i,
	input  [31:0] 	 			wb_dat_i,
	output [31:0] 	 			wb_dat_o,
	input 						wb_we_i,
	input 						wb_stb_i,
	input 						wb_cyc_i,
	input  [3:0]				wb_sel_i,
	output 						wb_ack_o,
	// UART	signals
	input 						i_uart_rx,
	output 						o_uart_tx,
	output 						uart_irq
);

	wire [7:0] 		       		uart_rdt;
	
	assign wb_dat_o = {24'd0, uart_rdt};

	uart_top uart16550_0(
		// Wishbone slave interface
		.wb_clk_i	(wb_clk_i),
		.wb_rst_i	(~wb_rst_i),
		.wb_adr_i	(wb_adr_i[4:2]),
		.wb_dat_i	(wb_dat_i[7:0]),
		.wb_we_i	(wb_we_i),
		.wb_cyc_i	(wb_cyc_i),
		.wb_stb_i	(wb_stb_i),
		.wb_sel_i	(4'b0), 		// Not used in 8-bit mode
		.wb_dat_o	(uart_rdt),
		.wb_ack_o	(wb_ack_o),
	
		// Outputs
		.int_o     (uart_irq),
		.stx_pad_o (o_uart_tx),
		.rts_pad_o (),
		.dtr_pad_o (),
	
		// Inputs
		.srx_pad_i (i_uart_rx),
		.cts_pad_i (1'b0),
		.dsr_pad_i (1'b0),
		.ri_pad_i  (1'b0),
		.dcd_pad_i (1'b0)
	);

endmodule
