`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 08/06/2020 05:23:56 PM
// Design Name: 
// Module Name: MAC
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


module RNS_MAC
    #(
    parameter CH_BW      = 32,
    parameter N_CHANNELS = 4,
    parameter RNS_BW     = CH_BW * N_CHANNELS,                 //total RNS buswidth 
    parameter RNS        = {32'd4294967291, 32'd4294967279, 32'd4294967231, 32'd4294967197}
    )
    (
    input  wire CLK,
    input  wire [RNS_BW-1:0]  A,
    input  wire [RNS_BW-1:0]  B,
    input  wire reset,
    output wire  [RNS_BW-1:0]  Z
    );
    
    /*
    wire [RNS_BW-1:0] m_result; 
    RNS_MULT #(CH_BW, N_CHANNELS, RNS_BW, RNS) m2(A, B, m_result); 
    RNS_ADD #(CH_BW, N_CHANNELS, RNS_BW, RNS) a1(m_result, m_result, Z);
    */
    
// Parallel generation
    
    genvar i;
    generate
        for (i = 0; i < N_CHANNELS; i = i + 1) begin
            integer L = CH_BW*i;
             MOD_MAC#(CH_BW, RNS[L+:CH_BW]) mac(CLK,A[L+:CH_BW],B[L+:CH_BW], reset, Z[L+:CH_BW]);
        end
    endgenerate
    
    // end parallel generation
                   
endmodule

