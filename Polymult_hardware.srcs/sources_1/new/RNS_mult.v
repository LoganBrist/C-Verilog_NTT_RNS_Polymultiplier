`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 08/06/2020 06:17:38 PM
// Design Name: 
// Module Name: RNS_mult
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

module RNS_MULT
#(
    parameter integer CH_BW        = 5,                           //RNS channel bitwidth
    parameter integer N_CHANNELS   = 4,                            //Number of RNS channels
    parameter RNS_BW = CH_BW * N_CHANNELS,                 //total RNS buswidth
    parameter RNS_MOD = {5'd7, 5'd3, 5'd5, 5'd6}       //RNS channels
    )
    
    (
    input  wire  [RNS_BW-1:0] A,
    input  wire  [RNS_BW-1:0] B,
    output wire  [RNS_BW-1:0] Z
    );

/*
    genvar index;
    generate
        for (index = 0; index < N_CHANNELS; index = index + 1) begin
            integer L = CH_BW*index;
            assign Z[index*CH_BW+:CH_BW] = (A[L+:CH_BW] * B[L+:CH_BW]) % RNS_MOD[L+:CH_BW];
        end
    endgenerate
*/    

//MOD_MULT #(CH_BW,RNS_MOD) add[N_CHANNELS-1:0] (.A(A),.B(B),.Z(Z)); 

    genvar i;
    generate
        for (i = 0; i < N_CHANNELS; i = i + 1) begin
            MOD_MULT #(CH_BW,RNS_MOD[i*CH_BW+:CH_BW]) add (.A(A[i*CH_BW+:CH_BW]),.B(B[i*CH_BW+:CH_BW]),.Z(Z[i*CH_BW+:CH_BW]));   //Using Barrett
        end
    endgenerate
endmodule

