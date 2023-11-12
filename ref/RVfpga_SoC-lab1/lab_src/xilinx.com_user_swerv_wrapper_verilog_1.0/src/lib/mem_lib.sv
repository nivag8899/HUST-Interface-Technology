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

//=========================================================================================================================
//=================================== START OF CCM  =======================================================================
//============= Possible sram sizes for a 39 bit wide memory ( 4 bytes + 7 bits ECC ) =====================================
//-------------------------------------------------------------------------------------------------------------------------

module ram_2048x39
  ( input logic CLK,
    input logic [10:0] ADR,
    input logic [38:0] D,

    output logic [38:0] Q,
    input logic WE );

   // behavior to be replaced by actual SRAM in VLE

   reg [38:0]   ram_core [2047:0];

   always @(posedge CLK) begin
      if (WE) begin// for active high WE - must be specified by user
         ram_core[ADR] <= D; Q <= 'x; end else
           Q <= ram_core[ADR];
   end
   
endmodule // ram_2048x39

// 64x21
module ram_64x21
  ( input logic CLK,
    input logic [5:0] ADR,
    input logic [20:0] D,

    output logic [20:0] Q,
    input logic WE );

   // behavior to be replaced by actual SRAM in VLE

   reg [20:0]   ram_core [63:0];

   always @(posedge CLK) begin
      if (WE) begin// for active high WE - must be specified by user
         ram_core[ADR] <= D; Q <= 'x; end else
           Q <= ram_core[ADR];
   end

endmodule // ram_64x21

// 256x34
module ram_256x34
  ( input logic CLK,
    input logic [7:0] ADR,
    input logic [33:0] D,

    output logic [33:0] Q,
    input logic WE );

   // behavior to be replaced by actual SRAM in VLE

   reg [33:0]   ram_core [255:0];

   always @(posedge CLK) begin
      if (WE) begin// for active high WE - must be specified by user
         ram_core[ADR] <= D; Q <= 'x; end else
           Q <= ram_core[ADR];
   end

endmodule // ram_256x34

