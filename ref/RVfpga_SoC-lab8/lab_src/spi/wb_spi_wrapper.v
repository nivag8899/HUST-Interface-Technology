
module wb_spi_wrapper(
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
	// SPI	signals
	output wire        			o_accel_sclk,
    output wire        			o_accel_cs_n,
    output wire        			o_accel_mosi,
    input  wire         		i_accel_miso,
	output wire					spi_irq
);

	wire [7:0] 		       		spi_rdt;
	
	assign wb_dat_o = {24'd0,spi_rdt};

	simple_spi spi(
		// Wishbone slave interface
		.clk_i  (wb_clk_i),
		.rst_i  (~wb_rst_i),
		.adr_i  (wb_adr_i[2]? 3'd0 : wb_adr_i[5:3]),
		.dat_i  (wb_dat_i[7:0]),
		.we_i   (wb_we_i),
		.cyc_i  (wb_cyc_i),
		.stb_i  (wb_stb_i),
		.dat_o  (spi_rdt),
		.ack_o  (wb_ack_o),
		.inta_o (spi_irq),
		// SPI interface
		.sck_o  (o_accel_sclk),
		.ss_o   (o_accel_cs_n),
		.mosi_o (o_accel_mosi),
		.miso_i (i_accel_miso)
	);

endmodule
