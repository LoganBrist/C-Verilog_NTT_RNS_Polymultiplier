`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 07/10/2020 01:54:40 PM
// Design Name: 
// Module Name: RNS_add
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

module RNS_ADD
#(
parameter integer CH_BW        = 32,                           //RNS channel bitwidth
parameter integer N_CHANNELS   = 9,                            //Number of RNS channels
parameter RNS_BW = CH_BW * N_CHANNELS,                 //total RNS buswidth
parameter RNS_MOD = {32'd4294967291, 32'd4294967279, 32'd4294967231, 32'd4294967197,32'd4294967189, 32'd4294967161, 32'd4294967143, 32'd4294967111, 32'd4294967087}      //RNS channels
    )
    (
    input  wire  [RNS_BW-1:0] A,
    input  wire  [RNS_BW-1:0] B,
    output wire  [RNS_BW-1:0] Z
    );

/////////////////////////////////////////////////////////////////////////////////////////////////////
    genvar i;
    generate
        for (i = 0; i < N_CHANNELS; i = i + 1) begin 
            MOD_ADD #(RNS_MOD[CH_BW*i +:CH_BW],CH_BW) add (.A(A[CH_BW*i +:CH_BW]),.B(B[CH_BW*i +:CH_BW]),.Z(Z[CH_BW*i +:CH_BW]));
        end
    endgenerate


//MOD_ADD #(CH_BW) add[N_CHANNELS-1:0] (.A(A),.B(B),.M(RNS_MOD),.Z(Z));


endmodule
