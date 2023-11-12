// Copyright 2018 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the "License"); you may not use this file except in
// compliance with the License. You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

// Author: Florian Zaruba
// Description: Generic up/down counter

module counter #(
    parameter int unsigned WIDTH = 4,
    parameter bit STICKY_OVERFLOW = 1'b0
)(
    input  wire             clk_i,
    input  wire             rst_ni,
    input  wire             clear_i, // synchronous clear
    input  wire             en_i,    // enable the counter
    input  wire             load_i,  // load a new value
    input  wire             down_i,  // downcount, default is up
    input  wire [WIDTH-1:0] d_i,
    output wire [WIDTH-1:0] q_o,
    output wire             overflow_o
);
    delta_counter #(
        .WIDTH          (WIDTH),
        .STICKY_OVERFLOW (STICKY_OVERFLOW)
    ) i_counter (
        .clk_i,
        .rst_ni,
        .clear_i,
        .en_i,
        .load_i,
        .down_i,
        .delta_i({{WIDTH-1{1'b0}}, 1'b1}),
        .d_i,
        .q_o,
        .overflow_o
    );
endmodule
