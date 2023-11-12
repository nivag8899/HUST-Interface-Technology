//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 01/12/2021 11:16:39 AM
// Design Name: 
// Module Name: swerv_wrapper_verilog
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
module swerv_wrapper_verilog(
    input wire                       clk,
   input wire                       rst,
//   input wire                       dbg_rst_l,
//   input wire [31:1]                rst_vec,
   input wire                       nmi_int,
   input wire [31:0]                nmi_vec,


//   output wire [63:0] trace_rv_i_insn_ip,
//   output wire [63:0] trace_rv_i_address_ip,
//   output wire [2:0]  trace_rv_i_valid_ip,
//   output wire [2:0]  trace_rv_i_exception_ip,
//   output wire [4:0]  trace_rv_i_ecause_ip,
//   output wire [2:0]  trace_rv_i_interrupt_ip,
//   output wire [31:0] trace_rv_i_tval_ip,

   // Bus signals

   //-------------------------- LSU AXI signals--------------------------
   // AXI Write Channels
   output wire                            lsu_axi_awvalid,
   input  wire                            lsu_axi_awready,
   output wire [4-1:0]      lsu_axi_awid,
   output wire [31:0]                     lsu_axi_awaddr,
   output wire [3:0]                      lsu_axi_awregion,
   output wire [7:0]                      lsu_axi_awlen,
   output wire [2:0]                      lsu_axi_awsize,
   output wire [1:0]                      lsu_axi_awburst,
   output wire                            lsu_axi_awlock,
   output wire [3:0]                      lsu_axi_awcache,
   output wire [2:0]                      lsu_axi_awprot,
   output wire [3:0]                      lsu_axi_awqos,
          
   output wire                            lsu_axi_wvalid,
   input  wire                            lsu_axi_wready,
   output wire [63:0]                     lsu_axi_wdata,
   output wire [7:0]                      lsu_axi_wstrb,
   output wire                            lsu_axi_wlast,
          
   input  wire                            lsu_axi_bvalid,
   output wire                            lsu_axi_bready,
   input  wire [1:0]                      lsu_axi_bresp,
   input  wire [4-1:0]      lsu_axi_bid,
          
   // AXI wireChannels
   output wire                            lsu_axi_arvalid,
   input  wire                            lsu_axi_arready,
   output wire [4-1:0]      lsu_axi_arid,
   output wire [31:0]                     lsu_axi_araddr,
   output wire [3:0]                      lsu_axi_arregion,
   output wire [7:0]                      lsu_axi_arlen,
   output wire [2:0]                      lsu_axi_arsize,
   output wire [1:0]                      lsu_axi_arburst,
   output wire                            lsu_axi_arlock,
   output wire [3:0]                      lsu_axi_arcache,
   output wire [2:0]                      lsu_axi_arprot,
   output wire [3:0]                      lsu_axi_arqos,
          
   input  wire                            lsu_axi_rvalid,
   output wire                            lsu_axi_rready,
   input  wire [4-1:0]      lsu_axi_rid,
   input  wire [63:0]                     lsu_axi_rdata,
   input  wire [1:0]                      lsu_axi_rresp,
   input  wire                            lsu_axi_rlast,
          
   //-----wire---------------- IFU AXI signals--------------------------
   // AXI wire Channels
//   output wire                            ifu_axi_awvalid,
//   input  wire                            ifu_axi_awready,
//   output wire [3-1:0]      ifu_axi_awid,
//   output wire [31:0]                     ifu_axi_awaddr,
//   output wire [3:0]                      ifu_axi_awregion,
//   output wire [7:0]                      ifu_axi_awlen,
//   output wire [2:0]                      ifu_axi_awsize,
//   output wire [1:0]                      ifu_axi_awburst,
//   output wire                            ifu_axi_awlock,
//   output wire [3:0]                      ifu_axi_awcache,
//   output wire [2:0]                      ifu_axi_awprot,
//   output wire [3:0]                      ifu_axi_awqos,
          
//   output wire                            ifu_axi_wvalid,
//   input  wire                            ifu_axi_wready,
//   output wire [63:0]                     ifu_axi_wdata,
//   output wire [7:0]                      ifu_axi_wstrb,
//   output wire                            ifu_axi_wlast,
          
//   input  wire                            ifu_axi_bvalid,
//   output wire                            ifu_axi_bready,
//   input  wire [1:0]                      ifu_axi_bresp,
//   input  wire [3-1:0]      ifu_axi_bid,
          
   // AXI wireChannels
   output wire                            ifu_axi_arvalid,
   input  wire                            ifu_axi_arready,
   output wire [3-1:0]      ifu_axi_arid,
   output wire [31:0]                     ifu_axi_araddr,
   output wire [3:0]                      ifu_axi_arregion,
   output wire [7:0]                      ifu_axi_arlen,
   output wire [2:0]                      ifu_axi_arsize,
   output wire [1:0]                      ifu_axi_arburst,
   output wire                            ifu_axi_arlock,
   output wire [3:0]                      ifu_axi_arcache,
   output wire [2:0]                      ifu_axi_arprot,
   output wire [3:0]                      ifu_axi_arqos,
          
   input  wire                            ifu_axi_rvalid,
   output wire                            ifu_axi_rready,
   input  wire [3-1:0]      ifu_axi_rid,
   input  wire [63:0]                     ifu_axi_rdata,
   input  wire [1:0]                      ifu_axi_rresp,
   input  wire                            ifu_axi_rlast,
          
   //-----wire---------------- SB AXI signals--------------------------
   // AXI wire Channels
   output wire                            sb_axi_awvalid,
   input  wire                            sb_axi_awready,
   output wire [1-1:0]       sb_axi_awid,
   output wire [31:0]                     sb_axi_awaddr,
   output wire [3:0]                      sb_axi_awregion,
   output wire [7:0]                      sb_axi_awlen,
   output wire [2:0]                      sb_axi_awsize,
   output wire [1:0]                      sb_axi_awburst,
   output wire                            sb_axi_awlock,
   output wire [3:0]                      sb_axi_awcache,
   output wire [2:0]                      sb_axi_awprot,
   output wire [3:0]                      sb_axi_awqos,
          
   output wire                            sb_axi_wvalid,
   input  wire                            sb_axi_wready,
   output wire [63:0]                     sb_axi_wdata,
   output wire [7:0]                      sb_axi_wstrb,
   output wire                            sb_axi_wlast,
          
   input  wire                            sb_axi_bvalid,
   output wire                            sb_axi_bready,
   input  wire [1:0]                      sb_axi_bresp,
   input  wire [1-1:0]       sb_axi_bid,
          
   // AXI wireChannels
   output wire                            sb_axi_arvalid,
   input  wire                            sb_axi_arready,
   output wire [1-1:0]       sb_axi_arid,
   output wire [31:0]                     sb_axi_araddr,
   output wire [3:0]                      sb_axi_arregion,
   output wire [7:0]                      sb_axi_arlen,
   output wire [2:0]                      sb_axi_arsize,
   output wire [1:0]                      sb_axi_arburst,
   output wire                            sb_axi_arlock,
   output wire [3:0]                      sb_axi_arcache,
   output wire [2:0]                      sb_axi_arprot,
   output wire [3:0]                      sb_axi_arqos,
          
   input  wire                            sb_axi_rvalid,
   output wire                            sb_axi_rready,
   input  wire [1-1:0]       sb_axi_rid,
   input  wire [63:0]                     sb_axi_rdata,
   input  wire [1:0]                      sb_axi_rresp,
   input  wire                            sb_axi_rlast,

   //-------------------------- DMA AXI signals--------------------------
   // AXI Write Channels
//   input  wire                         dma_axi_awvalid,
//   output wire                         dma_axi_awready,
//   input  wire [1-1:0]   dma_axi_awid,
//   input  wire [31:0]                  dma_axi_awaddr,
//   input  wire [2:0]                   dma_axi_awsize,
//   input  wire [2:0]                   dma_axi_awprot,
//   input  wire [7:0]                   dma_axi_awlen,
//   input  wire [1:0]                   dma_axi_awburst,

//   input  wire                    dma_axi_wvalid,
//   output wire                         dma_axi_wready,
//   input  wire [63:0]                  dma_axi_wdata,
//   input  wire [7:0]                   dma_axi_wstrb,
//   input  wire                         dma_axi_wlast,

//   output wire                         dma_axi_bvalid,
//   input  wire                         dma_axi_bready,
//   output wire [1:0]                   dma_axi_bresp,
//   output wire [1-1:0]   dma_axi_bid,

   // AXI wireChannels
//   input  wire                         dma_axi_arvalid,
//   output wire                         dma_axi_arready,
//   input  wire [1-1:0]   dma_axi_arid,
//   input  wire [31:0]                  dma_axi_araddr,
//   input  wire [2:0]                   dma_axi_arsize,
//   input  wire [2:0]                   dma_axi_arprot,
//   input  wire [7:0]                   dma_axi_arlen,
//   input  wire [1:0]                   dma_axi_arburst,

//   output wire                         dma_axi_rvalid,
//   input  wire                         dma_axi_rready,
//   output wire [1-1:0]   dma_axi_rid,
//   output wire [63:0]                  dma_axi_rdata,
//   output wire [1:0]                   dma_axi_rresp,
//   output wire                         dma_axi_rlast,

   // clk ratio signals
//   input wire                       lsu_bus_clk_en, // Clock ratio b/w cpu core clk & AHB master interface
//   input wire                       ifu_bus_clk_en, // Clock ratio b/w cpu core clk & AHB master interface
//   input wire                       dbg_bus_clk_en, // Clock ratio b/w cpu core clk & AHB master interface
//   input wire                       dma_bus_clk_en, // Clock ratio b/w cpu core clk & AHB slave interface


//   inpuwireic                   ext_int,
   input wire                       timer_int,
   input wire [8:1] extintsrc_req,

//   output wire [1:0] dec_tlu_perfcnt0, // toggles when perf counter 0 has an event inc
//   output wire [1:0] dec_tlu_perfcnt1,
//   output wire [1:0] dec_tlu_perfcnt2,
//   output wire [1:0] dec_tlu_perfcnt3,

   //Debugwirele
   input  wire                  dmi_reg_en,
   input  wire [6:0]            dmi_reg_addr,
   input  wire                  dmi_reg_wr_en,
   input  wire [31:0] 	         dmi_reg_wdata,
   output wire [31:0] 	         dmi_reg_rdata,
   input  wire                  dmi_hard_reset

   // external MPC halt/run interface
//   input wire mpc_debug_halt_req, // Async halt request
//   input wire mpc_debug_run_req, // Async run request
//   input wire mpc_reset_run_req, // Run/halt after reset
//   output wire mpc_debug_halt_ack, // Halt ack
//   output wire mpc_debug_run_ack, // Run ack
//   output wire debug_brkpt_status, // debug breakpoint

//   input wire                       i_cpu_halt_req, // Async halt req to CPU
//   output wire                      o_cpu_halt_ack, // core response to halt
//   output wire                      o_cpu_halt_status, // 1'b1 indicates core is halted
//   output wire                      o_debug_mode_status, // Core to the PMU that core is in debug mode. When core is in debug mode, the PMU should refrain from sendng a halt or run request
//   input wire                       i_cpu_run_req, // Async restart req to CPU
//   output wire                      o_cpu_run_ack, // Core response to run req
//   input wire                       scan_mode, // To enable scan mode
//   input wire                       mbist_mode // to enable mbist

    );
    
    
    swerv_wrapper_dmi swerv_eh1_2
     (
      .clk     (clk),
      .rst_l   (rst),
      .dbg_rst_l   (rst),
      .rst_vec (31'h40000000),
      .nmi_int (nmi_int),
      .nmi_vec (nmi_vec[31:1]),

      .trace_rv_i_insn_ip      (),
      .trace_rv_i_address_ip   (),
      .trace_rv_i_valid_ip     (),
      .trace_rv_i_exception_ip (),
      .trace_rv_i_ecause_ip    (),
      .trace_rv_i_interrupt_ip (),
      .trace_rv_i_tval_ip      (),

      // Bus signals
      //-------------------------- LSU AXI signals--------------------------
      .lsu_axi_awvalid  (lsu_axi_awvalid),
      .lsu_axi_awready  (lsu_axi_awready),
      .lsu_axi_awid     (lsu_axi_awid   ),
      .lsu_axi_awaddr   (lsu_axi_awaddr ),
      .lsu_axi_awregion (lsu_axi_awregion),
      .lsu_axi_awlen    (lsu_axi_awlen  ),
      .lsu_axi_awsize   (lsu_axi_awsize ),
      .lsu_axi_awburst  (lsu_axi_awburst),
      .lsu_axi_awlock   (lsu_axi_awlock ),
      .lsu_axi_awcache  (lsu_axi_awcache),
      .lsu_axi_awprot   (lsu_axi_awprot ),
      .lsu_axi_awqos    (lsu_axi_awqos  ),

      .lsu_axi_wvalid   (lsu_axi_wvalid),
      .lsu_axi_wready   (lsu_axi_wready),
      .lsu_axi_wdata    (lsu_axi_wdata),
      .lsu_axi_wstrb    (lsu_axi_wstrb),
      .lsu_axi_wlast    (lsu_axi_wlast),

      .lsu_axi_bvalid   (lsu_axi_bvalid),
      .lsu_axi_bready   (lsu_axi_bready),
      .lsu_axi_bresp    (lsu_axi_bresp ),
      .lsu_axi_bid      (lsu_axi_bid   ),

      .lsu_axi_arvalid  (lsu_axi_arvalid ),
      .lsu_axi_arready  (lsu_axi_arready ),
      .lsu_axi_arid     (lsu_axi_arid    ),
      .lsu_axi_araddr   (lsu_axi_araddr  ),
      .lsu_axi_arregion (lsu_axi_arregion),
      .lsu_axi_arlen    (lsu_axi_arlen   ),
      .lsu_axi_arsize   (lsu_axi_arsize  ),
      .lsu_axi_arburst  (lsu_axi_arburst ),
      .lsu_axi_arlock   (lsu_axi_arlock  ),
      .lsu_axi_arcache  (lsu_axi_arcache ),
      .lsu_axi_arprot   (lsu_axi_arprot  ),
      .lsu_axi_arqos    (lsu_axi_arqos   ),

      .lsu_axi_rvalid   (lsu_axi_rvalid),
      .lsu_axi_rready   (lsu_axi_rready),
      .lsu_axi_rid      (lsu_axi_rid   ),
      .lsu_axi_rdata    (lsu_axi_rdata ),
      .lsu_axi_rresp    (lsu_axi_rresp ),
      .lsu_axi_rlast    (lsu_axi_rlast ),

      //-------------------------- IFU AXI signals--------------------------
      //-------------------------- IFU AXI signals--------------------------
      .ifu_axi_awvalid  (),
      .ifu_axi_awready  (1'b0),
      .ifu_axi_awid     (),
      .ifu_axi_awaddr   (),
      .ifu_axi_awregion (),
      .ifu_axi_awlen    (),
      .ifu_axi_awsize   (),
      .ifu_axi_awburst  (),
      .ifu_axi_awlock   (),
      .ifu_axi_awcache  (),
      .ifu_axi_awprot   (),
      .ifu_axi_awqos    (),

      .ifu_axi_wvalid   (),
      .ifu_axi_wready   (1'b0),
      .ifu_axi_wdata    (),
      .ifu_axi_wstrb    (),
      .ifu_axi_wlast    (),

      .ifu_axi_bvalid   (1'b0),
      .ifu_axi_bready   (),
      .ifu_axi_bresp    (2'b00),
      .ifu_axi_bid      (3'd0),

      .ifu_axi_arvalid  (ifu_axi_arvalid ),
      .ifu_axi_arready  (ifu_axi_arready ),
      .ifu_axi_arid     (ifu_axi_arid    ),
      .ifu_axi_araddr   (ifu_axi_araddr  ),
      .ifu_axi_arregion (ifu_axi_arregion),
      .ifu_axi_arlen    (ifu_axi_arlen   ),
      .ifu_axi_arsize   (ifu_axi_arsize  ),
      .ifu_axi_arburst  (ifu_axi_arburst ),
      .ifu_axi_arlock   (ifu_axi_arlock  ),
      .ifu_axi_arcache  (ifu_axi_arcache ),
      .ifu_axi_arprot   (ifu_axi_arprot  ),
      .ifu_axi_arqos    (ifu_axi_arqos   ),

      .ifu_axi_rvalid   (ifu_axi_rvalid),
      .ifu_axi_rready   (ifu_axi_rready),
      .ifu_axi_rid      (ifu_axi_rid   ),
      .ifu_axi_rdata    (ifu_axi_rdata ),
      .ifu_axi_rresp    (ifu_axi_rresp ),
      .ifu_axi_rlast    (ifu_axi_rlast ),

      //-------------------------- SB AXI signals-------------------------
      .sb_axi_awvalid  (sb_axi_awvalid ),
      .sb_axi_awready  (sb_axi_awready ),
      .sb_axi_awid     (sb_axi_awid    ),
      .sb_axi_awaddr   (sb_axi_awaddr  ),
      .sb_axi_awregion (sb_axi_awregion),
      .sb_axi_awlen    (sb_axi_awlen   ),
      .sb_axi_awsize   (sb_axi_awsize  ),
      .sb_axi_awburst  (sb_axi_awburst ),
      .sb_axi_awlock   (sb_axi_awlock  ),
      .sb_axi_awcache  (sb_axi_awcache ),
      .sb_axi_awprot   (sb_axi_awprot  ),
      .sb_axi_awqos    (sb_axi_awqos   ),
      .sb_axi_wvalid   (sb_axi_wvalid  ),
      .sb_axi_wready   (sb_axi_wready  ),
      .sb_axi_wdata    (sb_axi_wdata   ),
      .sb_axi_wstrb    (sb_axi_wstrb   ),
      .sb_axi_wlast    (sb_axi_wlast   ),
      .sb_axi_bvalid   (sb_axi_bvalid  ),
      .sb_axi_bready   (sb_axi_bready  ),
      .sb_axi_bresp    (sb_axi_bresp   ),
      .sb_axi_bid      (sb_axi_bid     ),
      .sb_axi_arvalid  (sb_axi_arvalid ),
      .sb_axi_arready  (sb_axi_arready ),
      .sb_axi_arid     (sb_axi_arid    ),
      .sb_axi_araddr   (sb_axi_araddr  ),
      .sb_axi_arregion (sb_axi_arregion),
      .sb_axi_arlen    (sb_axi_arlen   ),
      .sb_axi_arsize   (sb_axi_arsize  ),
      .sb_axi_arburst  (sb_axi_arburst ),
      .sb_axi_arlock   (sb_axi_arlock  ),
      .sb_axi_arcache  (sb_axi_arcache ),
      .sb_axi_arprot   (sb_axi_arprot  ),
      .sb_axi_arqos    (sb_axi_arqos   ),
      .sb_axi_rvalid   (sb_axi_rvalid  ),
      .sb_axi_rready   (sb_axi_rready  ),
      .sb_axi_rid      (sb_axi_rid     ),
      .sb_axi_rdata    (sb_axi_rdata   ),
      .sb_axi_rresp    (sb_axi_rresp   ),
      .sb_axi_rlast    (sb_axi_rlast   ),

      //-------------------------- DMA AXI signals--------------------------
      .dma_axi_awvalid  (1'b0),
      .dma_axi_awready  (),
      .dma_axi_awid     (1'd0),
      .dma_axi_awaddr   (32'd0),
      .dma_axi_awsize   (3'd0),
      .dma_axi_awprot   (3'd0),
      .dma_axi_awlen    (8'd0),
      .dma_axi_awburst  (2'd0),

      .dma_axi_wvalid   (1'b0),
      .dma_axi_wready   (),
      .dma_axi_wdata    (64'd0),
      .dma_axi_wstrb    (8'd0),
      .dma_axi_wlast    (1'b0),

      .dma_axi_bvalid   (),
      .dma_axi_bready   (1'b0),
      .dma_axi_bresp    (),
      .dma_axi_bid      (),

      .dma_axi_arvalid  (1'b0),
      .dma_axi_arready  (),
      .dma_axi_arid     (1'd0),
      .dma_axi_araddr   (32'd0),
      .dma_axi_arsize   (3'd0),
      .dma_axi_arprot   (3'd0),
      .dma_axi_arlen    (8'd0),
      .dma_axi_arburst  (2'd0),

      .dma_axi_rvalid   (),
      .dma_axi_rready   (1'b0),
      .dma_axi_rid      (),
      .dma_axi_rdata    (),
      .dma_axi_rresp    (),
      .dma_axi_rlast    (),

      // clk ratio signals
      .lsu_bus_clk_en (1'b1),
      .ifu_bus_clk_en (1'b1),
      .dbg_bus_clk_en (1'b1),
      .dma_bus_clk_en (1'b1),

      .timer_int (timer_int),
      .extintsrc_req (extintsrc_req),
      .dec_tlu_perfcnt0 (),
      .dec_tlu_perfcnt1 (),
      .dec_tlu_perfcnt2 (),
      .dec_tlu_perfcnt3 (),
      
      .dmi_reg_rdata    (dmi_reg_rdata),
      .dmi_reg_wdata    (dmi_reg_wdata),
      .dmi_reg_addr     (dmi_reg_addr),
      .dmi_reg_en       (dmi_reg_en),
      .dmi_reg_wr_en    (dmi_reg_wr_en),
      .dmi_hard_reset   (dmi_hard_reset),

      .mpc_debug_halt_req (1'b0),
      .mpc_debug_run_req  (1'b0),
      .mpc_reset_run_req  (1'b1),
      .mpc_debug_halt_ack (),
      .mpc_debug_run_ack  (),
      .debug_brkpt_status (),

      .i_cpu_halt_req      (1'b0),
      .o_cpu_halt_ack      (),
      .o_cpu_halt_status   (),
      .o_debug_mode_status (),
      .i_cpu_run_req       (1'b0),
      .o_cpu_run_ack       (),

      .scan_mode  (1'b0),
      .mbist_mode (1'b0));
    
endmodule