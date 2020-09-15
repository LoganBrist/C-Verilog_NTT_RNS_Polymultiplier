`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 08/13/2020 03:20:25 PM
// Design Name: 
// Module Name: RNS_SUB_SINGLE_MOD
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


module RNS_SUB_SINGLE_MOD
#(
    parameter CH_BW        = 32,                           //RNS channel bitwidth
    parameter N_CHANNELS   = 4,                            //Number of RNS channels
    parameter RNS_BW = CH_BW * N_CHANNELS,                 //total RNS buswidth
    parameter MOD = 32'd13                                 //To reduce each channel by
    )
    (
    input  wire  [RNS_BW-1:0] A,
    input  wire  [RNS_BW-1:0] B,
    output wire  [RNS_BW-1:0] Z
    )
    ;
    genvar index;
    generate
        for (index = 0; index < N_CHANNELS; index = index + 1) begin
            integer L = CH_BW*index;
            assign Z = (A[L+:CH_BW] + MOD - B[L+:CH_BW]) % MOD;
        end
    endgenerate
endmodule