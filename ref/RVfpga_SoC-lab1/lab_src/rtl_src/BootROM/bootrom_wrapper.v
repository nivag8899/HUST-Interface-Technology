`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/02/2021 04:05:29 PM
// Design Name: 
// Module Name: bootrom_wrapper
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


module bootrom_wrapper(
   input wire 			     i_clk,
   input wire 			     i_rst,
   input wire [31:0] i_wb_adr,
   input wire [31:0] 		     i_wb_dat,
   input wire [3:0] 		     i_wb_sel,
   input wire 			     i_wb_we ,
   input wire 			     i_wb_cyc,
   input wire 			     i_wb_stb,
   output wire 			     o_wb_ack,
   output wire [31:0] 		     o_wb_rdt);
    
    
   wb_mem_wrapper bootrom
     (.i_clk    (i_clk),
      .i_rst    (i_rst),
      .i_wb_adr (i_wb_adr),
      .i_wb_dat (i_wb_dat),
      .i_wb_sel (i_wb_sel),
      .i_wb_we  (i_wb_we),
      .i_wb_cyc (i_wb_cyc),
      .i_wb_stb (i_wb_stb),
      .o_wb_rdt (o_wb_rdt),
      .o_wb_ack (o_wb_ack));    
endmodule
