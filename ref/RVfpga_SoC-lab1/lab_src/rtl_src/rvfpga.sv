// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Western Digital Corporation or its affiliates.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//********************************************************************************
// $Id$
//
// Function: SweRVolf toplevel for Nexys A7 board
// Comments:
//
//********************************************************************************

`default_nettype none

module rvfpga(
	input  wire 	       	clk,
    input  wire 	       	rstn,
    output wire [12:0] 		ddram_a,
    output wire [2:0]  		ddram_ba,
    output wire        		ddram_ras_n,
    output wire        		ddram_cas_n,
    output wire        		ddram_we_n,
    output wire        		ddram_cs_n,
    output wire [1:0]  		ddram_dm,
    inout  wire [15:0]  	ddram_dq,
    inout  wire [1:0]  		ddram_dqs_p,
    inout  wire [1:0]  		ddram_dqs_n,
    output wire        		ddram_clk_p,
    output wire        		ddram_clk_n,
    output wire        		ddram_cke,
    output wire        		ddram_odt,
    output wire        		o_flash_cs_n,
    output wire        		o_flash_mosi,
    input  wire 	       	i_flash_miso,
    input  wire 	       	i_uart_rx,
    output wire        		o_uart_tx,
    inout  wire [15:0]  	i_sw,
    output reg  [15:0]  	o_led,
    output reg  [7:0]   	AN,
    output reg         		CA, CB, CC, CD, CE, CF, CG,
    output wire        		o_accel_cs_n,
    output wire        		o_accel_mosi,
    input  wire         	i_accel_miso,
    output wire        		accel_sclk
);

	localparam RAM_SIZE = 32'h10000;

	wire [15:0] 	    	gpio_out;
	wire [15:0]         	gpio_out2;
	wire 	       			cpu_tx;
	wire					litedram_tx;
	
	wire 	       			litedram_init_done;
	wire 	       			litedram_init_error;
	
	wire 	 				clk_core;
	wire 	 				rst_core;
	wire 	 				user_clk;
	wire 	 				user_rst;
	
	clk_gen_nexys clk_gen(
		.i_clk 				(user_clk),
		.i_rst 				(user_rst),
		.o_clk_core 		(clk_core),
		.o_rst_core 		(rst_core)
	);
	
	AXI_BUS #(32, 64, 6, 1) mem();
	AXI_BUS #(32, 64, 6, 1) cpu();
	
	assign cpu.aw_atop = 6'd0;
	assign cpu.aw_user = 1'b0;
	assign cpu.ar_user = 1'b0;
	assign cpu.w_user  = 1'b0;
	assign cpu.b_user  = 1'b0;
	assign cpu.r_user  = 1'b0;
	assign mem.b_user  = 1'b0;
	assign mem.r_user  = 1'b0;
	
	axi_cdc_intf #(
		.AXI_USER_WIDTH (1),
		.AXI_ADDR_WIDTH (32),
		.AXI_DATA_WIDTH (64),
		.AXI_ID_WIDTH   (6)
	) cdc(
		.src_clk_i  	(clk_core),
		.src_rst_ni 	(~rst_core),
		.src        	(cpu),
		.dst_clk_i  	(user_clk),
		.dst_rst_ni 	(~user_rst),
		.dst        	(mem)
	);
	
	litedram_top #(
		.ID_WIDTH 		(6)
	) ddr2(
		.serial_tx   	(litedram_tx),
		.serial_rx   	(i_uart_rx),
		.clk100      	(clk),
		.rst_n       	(rstn),
		.pll_locked  	(),
		.user_clk    	(user_clk),
		.user_rst    	(user_rst),
		.ddram_a     	(ddram_a),
		.ddram_ba    	(ddram_ba),
		.ddram_ras_n 	(ddram_ras_n),
		.ddram_cas_n 	(ddram_cas_n),
		.ddram_we_n  	(ddram_we_n),
		.ddram_cs_n  	(ddram_cs_n),
		.ddram_dm    	(ddram_dm),
		.ddram_dq    	(ddram_dq),
		.ddram_dqs_p 	(ddram_dqs_p),
		.ddram_dqs_n 	(ddram_dqs_n),
		.ddram_clk_p 	(ddram_clk_p),
		.ddram_clk_n 	(ddram_clk_n),
		.ddram_cke   	(ddram_cke),
		.ddram_odt   	(ddram_odt),
		.init_done  	(litedram_init_done),
		.init_error 	(litedram_init_error),
		.i_awid    		(mem.aw_id),
		.i_awaddr  		(mem.aw_addr[26:0]),
		.i_awlen   		(mem.aw_len  ),
		.i_awsize  		({1'b0,mem.aw_size}),
		.i_awburst 		(mem.aw_burst),
		.i_awvalid 		(mem.aw_valid),
		.o_awready 		(mem.aw_ready),
		.i_arid    		(mem.ar_id   ),
		.i_araddr  		(mem.ar_addr[26:0]),
		.i_arlen   		(mem.ar_len),
		.i_arsize  		({1'b0,mem.ar_size}),
		.i_arburst 		(mem.ar_burst),
		.i_arvalid 		(mem.ar_valid),
		.o_arready 		(mem.ar_ready),
		.i_wdata   		(mem.w_data),
		.i_wstrb   		(mem.w_strb),
		.i_wlast   		(mem.w_last),
		.i_wvalid  		(mem.w_valid),
		.o_wready  		(mem.w_ready),
		.o_bid     		(mem.b_id),
		.o_bresp   		(mem.b_resp),
		.o_bvalid  		(mem.b_valid),
		.i_bready  		(mem.b_ready),
		.o_rid     		(mem.r_id),
		.o_rdata   		(mem.r_data),
		.o_rresp   		(mem.r_resp),
		.o_rlast   		(mem.r_last),
		.o_rvalid  		(mem.r_valid),
		.i_rready  		(mem.r_ready)
	);
	
	wire        dmi_reg_en;
	wire [6:0]  dmi_reg_addr;
	wire        dmi_reg_wr_en;
	wire [31:0] dmi_reg_wdata;
	wire [31:0] dmi_reg_rdata;
	wire        dmi_hard_reset;
	wire        flash_sclk;
	
	STARTUPE2 STARTUPE2(
		.CFGCLK    (),
		.CFGMCLK   (),
		.EOS       (),
		.PREQ      (),
		.CLK       (1'b0),
		.GSR       (1'b0),
		.GTS       (1'b0),
		.KEYCLEARB (1'b1),
		.PACK      (1'b0),
		.USRCCLKO  (flash_sclk),
		.USRCCLKTS (1'b0),
		.USRDONEO  (1'b1),
		.USRDONETS (1'b0)
	);
	
	bscan_tap tap(
		.clk            (clk_core),
		.rst            (rst_core),
		.jtag_id        (31'd0),
		.dmi_reg_wdata  (dmi_reg_wdata),
		.dmi_reg_addr   (dmi_reg_addr),
		.dmi_reg_wr_en  (dmi_reg_wr_en),
		.dmi_reg_en     (dmi_reg_en),
		.dmi_reg_rdata  (dmi_reg_rdata),
		.dmi_hard_reset (dmi_hard_reset),
		.rd_status      (2'd0),
		.idle           (3'd0),
		.dmi_stat       (2'd0),
		.version        (4'd1)
	);
	
	swerv_soc_wrapper swervolf(
		.clk_0  			(clk_core),
		.rst_0 				(~rst_core),
		.dmi_reg_rdata_0  	(dmi_reg_rdata),
		.dmi_reg_wdata_0  	(dmi_reg_wdata),
		.dmi_reg_addr_0   	(dmi_reg_addr ),
		.dmi_reg_en_0     	(dmi_reg_en   ),
		.dmi_reg_wr_en_0  	(dmi_reg_wr_en),
		.dmi_hard_reset_0 	(dmi_hard_reset),
//      .o_flash_sclk   	(flash_sclk),
//      .o_flash_cs_n   	(o_flash_cs_n),
//      .o_flash_mosi   	(o_flash_mosi),
//      .i_flash_miso   	(i_flash_miso),
//      .i_uart_rx      	(i_uart_rx),
//      .o_uart_tx      	(cpu_tx),
		.ram_awid     		(cpu.aw_id),
		.ram_awaddr   		(cpu.aw_addr),
		.ram_awlen    		(cpu.aw_len),
		.ram_awsize   		(cpu.aw_size),
		.ram_awburst  		(cpu.aw_burst),
		.ram_awlock   		(cpu.aw_lock),
		.ram_awcache  		(cpu.aw_cache),
		.ram_awprot   		(cpu.aw_prot),
		.ram_awregion 		(cpu.aw_region),
		.ram_awqos    		(cpu.aw_qos),
		.ram_awvalid  		(cpu.aw_valid),
		.ram_awready  		(cpu.aw_ready),
		.ram_arid     		(cpu.ar_id),
		.ram_araddr   		(cpu.ar_addr),
		.ram_arlen    		(cpu.ar_len),
		.ram_arsize   		(cpu.ar_size),
		.ram_arburst  		(cpu.ar_burst),
		.ram_arlock   		(cpu.ar_lock),
		.ram_arcache  		(cpu.ar_cache),
		.ram_arprot   		(cpu.ar_prot),
		.ram_arregion 		(cpu.ar_region),
		.ram_arqos    		(cpu.ar_qos),
		.ram_arvalid  		(cpu.ar_valid),
		.ram_arready  		(cpu.ar_ready),
		.ram_wdata    		(cpu.w_data),
		.ram_wstrb    		(cpu.w_strb),
		.ram_wlast    		(cpu.w_last),
		.ram_wvalid   		(cpu.w_valid),
		.ram_wready   		(cpu.w_ready),
		.ram_bid      		(cpu.b_id),
		.ram_bresp    		(cpu.b_resp),
		.ram_bvalid   		(cpu.b_valid),
		.ram_bready   		(cpu.b_ready),
		.ram_rid      		(cpu.r_id),
		.ram_rdata    		(cpu.r_data),
		.ram_rresp    		(cpu.r_resp),
		.ram_rlast    		(cpu.r_last),
		.ram_rvalid   		(cpu.r_valid),
		.ram_rready   		(cpu.r_ready),
		.i_ram_init_done_0  (litedram_init_done),
		.i_ram_init_error_0 (litedram_init_error),
		.bidir				({gpio_out, gpio_out2}),
		.AN_0            	(AN),
		.Digits_Bits_0   	({CA,CB,CC,CD,CE,CF,CG})
//      .o_accel_sclk   (accel_sclk),
//      .o_accel_cs_n   (o_accel_cs_n),
//      .o_accel_mosi   (o_accel_mosi),
//      .i_accel_miso   (i_accel_miso)
	);

	always @(posedge clk_core) begin
		o_led[15:0] <= gpio_out2[15:0];
	end
	
	assign o_uart_tx = 1'b0 ? litedram_tx : cpu_tx;

endmodule
