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

localparam TOTAL_INT        = 9;

localparam DCCM_BITS        = 16;
localparam DCCM_BANK_BITS   = 3;
localparam DCCM_NUM_BANKS   = 8;
localparam DCCM_DATA_WIDTH  = 32;
localparam DCCM_FDATA_WIDTH = 39;
localparam DCCM_BYTE_WIDTH  = 4;
localparam DCCM_ECC_WIDTH   = 7;

localparam LSU_RDBUF_DEPTH  = 8;
localparam DMA_BUF_DEPTH    = 4;
localparam LSU_STBUF_DEPTH  = 8;
localparam LSU_SB_BITS      = 16;

localparam DEC_INSTBUF_DEPTH = 4;

localparam ICCM_SIZE         = 512;
localparam ICCM_BITS         = 19;
localparam ICCM_NUM_BANKS    = 8;
localparam ICCM_BANK_BITS    = 3;
localparam ICCM_INDEX_BITS   = 14;
localparam ICCM_BANK_HI      = 4 + (3/4);

localparam ICACHE_TAG_HIGH  = 12;
localparam ICACHE_TAG_LOW   = 6;
localparam ICACHE_IC_DEPTH  = 8;
localparam ICACHE_TAG_DEPTH = 64;

localparam LSU_BUS_TAG     = 4;
localparam DMA_BUS_TAG     = 1;
localparam SB_BUS_TAG      = 1;

localparam IFU_BUS_TAG     = 3;


