`default_nettype none
module serv_ctrl
  (
   input wire 	      clk,
   input wire 	      i_rst,
   //State
   input wire 	      i_pc_en,
   input wire 	      i_cnt12to31,
   input wire 	      i_cnt2,
   input wire 	      i_cnt_done,
   //Control
   input wire 	      i_jump,
   input wire 	      i_jal_or_jalr,
   input wire 	      i_utype,
   input wire 	      i_pc_rel,
   input wire 	      i_trap,
   //Data
   input wire 	      i_imm,
   input wire 	      i_buf,
   input wire 	      i_csr_pc,
   output wire 	      o_rd,
   output wire 	      o_bad_pc,
   //External
   output reg [31:0] o_ibus_adr,
   output wire 	      o_ibus_cyc,
   input wire 	      i_ibus_ack);

   parameter RESET_PC = 32'd0;
   parameter WITH_CSR = 1;

   reg 	      en_pc_r;

   wire       pc_plus_4;
   wire       pc_plus_4_cy;
   reg 	      pc_plus_4_cy_r;
   wire       pc_plus_offset;
   wire       pc_plus_offset_cy;
   reg 	      pc_plus_offset_cy_r;
   wire       pc_plus_offset_aligned;
   wire       plus_4;

   wire       pc = o_ibus_adr[0];

   wire       new_pc;

   wire       offset_a;
   wire       offset_b;

   assign plus_4        = i_cnt2;

   assign o_bad_pc = pc_plus_offset_aligned;

   assign {pc_plus_4_cy,pc_plus_4} = pc+plus_4+pc_plus_4_cy_r;

   generate
      if (WITH_CSR)
	assign new_pc = i_trap ? (i_csr_pc & en_pc_r) : i_jump ? pc_plus_offset_aligned : pc_plus_4;
      else
	assign new_pc = i_jump ? pc_plus_offset_aligned : pc_plus_4;
   endgenerate
   assign o_rd  = (i_utype & pc_plus_offset_aligned) | (pc_plus_4 & i_jal_or_jalr);

   assign offset_a = i_pc_rel & pc;
   assign offset_b = i_utype ? (i_imm & i_cnt12to31): i_buf;
   assign {pc_plus_offset_cy,pc_plus_offset} = offset_a+offset_b+pc_plus_offset_cy_r;

   assign pc_plus_offset_aligned = pc_plus_offset & en_pc_r;

   assign o_ibus_cyc = en_pc_r & !i_pc_en;

   always @(posedge clk) begin
      pc_plus_4_cy_r <= i_pc_en & pc_plus_4_cy;
      pc_plus_offset_cy_r <= i_pc_en & pc_plus_offset_cy;

      if (i_pc_en) begin
	 en_pc_r <= 1'b1;
	 o_ibus_adr <= {new_pc, o_ibus_adr[31:1]};
      end else if (o_ibus_cyc & i_ibus_ack)
        en_pc_r <= 1'b0;

      if (i_rst) begin
	 en_pc_r <= 1'b1;
	 o_ibus_adr <= RESET_PC;
      end
   end

endmodule
