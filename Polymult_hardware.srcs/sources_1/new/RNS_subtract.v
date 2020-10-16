`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 07/10/2020 01:55:13 PM
// Design Name: 
// Module Name: RNS_subtract
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


module RNS_SUB
#(
    parameter CH_BW        = 5,                           //RNS channel bitwidth
    parameter N_CHANNELS   = 4,                            //Number of RNS channels
    parameter RNS_BW = CH_BW * N_CHANNELS,                 //total RNS buswidth
    parameter RNS_MOD = {5'd7, 5'd9, 5'd11, 5'd13}       //RNS channels
    )
    (
    input  wire  [RNS_BW-1:0] A,
    input  wire  [RNS_BW-1:0] B,
    output wire  [RNS_BW-1:0] Z
    )
    ;
    
    genvar i;
    generate
        for (i = 0; i < N_CHANNELS; i = i + 1) begin 
            MOD_SUB #(RNS_MOD[CH_BW*i +:CH_BW],CH_BW) sub (.A(A[CH_BW*i +:CH_BW]),.B(B[CH_BW*i +:CH_BW]),.Z(Z[CH_BW*i +:CH_BW]));
        end
    endgenerate


//MOD_SUB #(CH_BW) add[N_CHANNELS-1:0] (.A(A),.B(B),.M(RNS_MOD),.Z(Z)); 
   
endmodule
