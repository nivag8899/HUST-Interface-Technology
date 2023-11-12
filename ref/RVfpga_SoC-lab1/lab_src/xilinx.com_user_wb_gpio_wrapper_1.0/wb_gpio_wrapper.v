`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 02/28/2021 06:30:20 PM
// Design Name: 
// Module Name: gpio_wrapper
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////
`default_nettype wire

module wb_gpio_wrapper(
	input 				wb_clk_i,
	input 				wb_rst_i, 
	input 				wb_cyc_i, 
	input  [7:0]		wb_adr_i,	// address bus inputs
	input  [31:0]		wb_dat_i,	// input data bus
	input  [3:0]     	wb_sel_i,	// byte select inputs
	input             	wb_we_i,	// indicates write transfer
	input             	wb_stb_i,	// strobe input
	output [31:0]  		wb_dat_o,	// output data bus
	output 				wb_ack_o,
	output 				wb_err_o,
	output 				wb_inta_o,
	
	output [31:0]		bidir
);
    
	wire [31:0]			outp;
	wire [31:0]			inp;
	wire [31:0]			oe;
	
    gpio_top gpio(
		.wb_clk_i     (wb_clk_i), 
        .wb_rst_i     (wb_rst_i), 
        .wb_cyc_i     (wb_cyc_i), 
        .wb_adr_i     ({2'b0,wb_adr_i[5:2],2'b0}), 
        .wb_dat_i     (wb_dat_i), 
        .wb_sel_i     (4'b1111),
        .wb_we_i      (wb_we_i), 
        .wb_stb_i     (wb_stb_i), 
        .wb_dat_o     (wb_dat_o),
        .wb_ack_o     (wb_ack_o), 
        .wb_err_o     (wb_err_o),
        .wb_inta_o    (wb_inta_o),
        // External GPIO Interface
        .ext_pad_i    (outp),
        .ext_pad_o    (inp),
        .ext_padoe_o  (oe)
    );
    
	assign bidir = oe? inp : 1'bZ ;
	assign outp  = bidir;    
    
endmodule
