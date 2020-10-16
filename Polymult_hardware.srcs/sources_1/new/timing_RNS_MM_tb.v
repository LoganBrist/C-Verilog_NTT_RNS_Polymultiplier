`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/16/2020 02:11:34 AM
// Design Name: 
// Module Name: timing_RNS_MM_tb
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


module timing_RNS_MM_tb #(
parameter CH_BW        = 32,                                 //RNS channel bitwidth
parameter N_CHANNELS   = 4,                                  //Number of RNS channels
parameter RNS_BW = CH_BW * N_CHANNELS,                       //total RNS buswidth
parameter EXT_BW = CH_BW * (N_CHANNELS+1) ,                     //total RNS_EXT buswidth + m_r
parameter RNS     = {32'd4294967291, 32'd4294967279, 32'd4294967231, 32'd4294967197},  //RNS channels
parameter RNS_EXT = {32'd4294967189, 32'd4294967161, 32'd4294967143, 32'd4294967111, 32'd4294967087},  //extended RNS channels with redundant channel 
parameter TOTAL_RNS = {32'd4294967291, 32'd4294967279, 32'd4294967231, 32'd4294967197,32'd4294967189, 32'd4294967161, 32'd4294967143, 32'd4294967111, 32'd4294967087},
parameter TOTAL_CH = N_CHANNELS*2 + 1,
parameter TOTAL_BW = RNS_BW + EXT_BW
)
(
input  wire CLK,
input  wire [TOTAL_BW-1:0] A, 
input  wire [TOTAL_BW-1:0] B,
output reg  [TOTAL_BW-1:0] Z_reg
);


reg  [TOTAL_BW-1:0]  A_reg; 
reg  [TOTAL_BW-1:0]  B_reg;
wire [TOTAL_BW-1:0]  Z;

always @ (posedge CLK) begin
     A_reg  <= A;
     B_reg  <= B;
     Z_reg  <= Z;
 end
 
 RNS_modmult #(CH_BW, N_CHANNELS, RNS_BW, EXT_BW , RNS, RNS_EXT, TOTAL_RNS, TOTAL_CH, TOTAL_BW) MODMULT (CLK, A_reg, B_reg, Z);

endmodule
