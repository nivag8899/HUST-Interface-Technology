`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/02/2021 02:53:47 PM
// Design Name: 
// Module Name: syscon_wrapper
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


module syscon_wrapper(
   input wire        i_clk,
   input wire 	     i_rst,
   input wire        gpio_irq,
   input wire        ptc_irq,
   output wire 	     o_timer_irq,
//   output wire 	     o_sw_irq3,
//   output wire 	     o_sw_irq4,
   input wire 	     i_ram_init_done,
   input wire 	     i_ram_init_error,
   output wire [31:0] o_nmi_vec,
   output wire 	     o_nmi_int,

   input wire [31:0]  i_wb_adr,
   input wire [31:0] i_wb_dat,
   input wire [3:0]  i_wb_sel,
   input wire 	     i_wb_we,
   input wire 	     i_wb_cyc,
   input wire 	     i_wb_stb,
   output wire [31:0] o_wb_rdt,
   output wire 	     o_wb_ack,
   
   output wire [ 7          :0] AN,
   output wire [ 6          :0] Digits_Bits);

swervolf_syscon syscon
     (.i_clk            (i_clk),
      .i_rst            (i_rst),
      .gpio_irq         (gpio_irq),
      .ptc_irq          (ptc_irq),
      .o_timer_irq      (o_timer_irq),
      .o_sw_irq3        (),
      .o_sw_irq4        (),
      .i_ram_init_done  (i_ram_init_done),
      .i_ram_init_error (i_ram_init_error),
      .o_nmi_vec        (o_nmi_vec),
      .o_nmi_int        (o_nmi_int),
      .i_wb_adr         (i_wb_adr[5:0]),
      .i_wb_dat         (i_wb_dat),
      .i_wb_sel         (i_wb_sel),
      .i_wb_we          (i_wb_we),
      .i_wb_cyc         (i_wb_cyc),
      .i_wb_stb         (i_wb_stb),
      .o_wb_rdt         (o_wb_rdt),
      .o_wb_ack         (o_wb_ack),
      .AN (AN),
      .Digits_Bits (Digits_Bits));
    
endmodule
