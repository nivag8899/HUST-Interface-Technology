`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 02/28/2021 07:44:41 PM
// Design Name: 
// Module Name: ptc_wrapper
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


module ptc_wrapper(
	// WISHBONE Interface
	wb_clk_i, wb_rst_i, wb_cyc_i, wb_adr_i, wb_dat_i, wb_sel_i, wb_we_i, wb_stb_i,
	wb_dat_o, wb_ack_o, wb_err_o, wb_inta_o

	// External PTC Interface
	//gate_clk_pad_i, capt_pad_i, pwm_pad_o, oen_padoen_o
);

parameter dw = 32;
parameter aw = 15+1;
parameter cw = 32;


//
// WISHBONE Interface
//
input wire		wb_clk_i;	// Clock
input wire		wb_rst_i;	// Reset
input wire		wb_cyc_i;	// cycle valid input
input wire	[32-1:0]	wb_adr_i;	// address bus inputs
input wire	[dw-1:0]	wb_dat_i;	// input data bus
input wire	[3:0]		wb_sel_i;	// byte select inputs
input wire			wb_we_i;	// indicates write transfer
input wire		wb_stb_i;	// strobe input
output		[dw-1:0]	wb_dat_o;	// output data bus
output wire		wb_ack_o;	// normal termination
output wire		wb_err_o;	// termination w/ error
output wire		wb_inta_o;	// Interrupt request output

//
// External PTC Interface
//
//input wire	gate_clk_pad_i;	// EClk/Gate input
//input wire	capt_pad_i;	// Capture input
//output wire		pwm_pad_o;	// PWM output
//output wire	oen_padoen_o;	// PWM output driver enable
    
    ptc_top timer_ptc(
        .wb_clk_i     (wb_clk_i), 
        .wb_rst_i     (~wb_rst_i), 
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
        // External PTC Interface
        .gate_clk_pad_i (),
        .capt_pad_i (),
        .pwm_pad_o (),
        .oen_padoen_o ()
   );
endmodule
