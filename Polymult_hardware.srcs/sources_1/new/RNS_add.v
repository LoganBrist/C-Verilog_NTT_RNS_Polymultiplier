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
    parameter integer CH_BW        = 5,                           //RNS channel bitwidth
    parameter integer N_CHANNELS   = 4,                            //Number of RNS channels
    parameter RNS_BW = CH_BW * N_CHANNELS,                 //total RNS buswidth
    parameter RNS_MOD = {5'd7, 5'd9, 5'd11, 5'd13}      //RNS channels
    )
    (
    input  wire  [RNS_BW-1:0] A,
    input  wire  [RNS_BW-1:0] B,
    output wire  [RNS_BW-1:0] Z
    );
    //assign Z = A + B;
    
    genvar i;
    generate
        for (i = 0; i < N_CHANNELS; i = i + 1) begin
            integer L = CH_BW*i;
            assign Z[CH_BW*i +:CH_BW]  = (A[L+:CH_BW] + B[L+:CH_BW]) % RNS_MOD[L+:CH_BW];
        end
    endgenerate
    
endmodule
