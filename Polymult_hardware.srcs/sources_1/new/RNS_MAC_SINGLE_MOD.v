`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 08/13/2020 03:04:38 PM
// Design Name: 
// Module Name: RNS_MAC_SINGLE_MOD
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


module RNS_MAC_SINGLE_MOD
    #(
    parameter CH_BW      = 32,
    parameter N_CHANNELS = 4,
    parameter RNS_BW     = CH_BW * N_CHANNELS,                 //total RNS buswidth 
    parameter MOD        = 32'd2
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
    RNS_MULT_SINGLE_MOD #(CH_BW, N_CHANNELS, RNS_BW, MOD) m2(A, B, m_result); 
    RNS_ADD_SINGLE_MOD #(CH_BW, N_CHANNELS, RNS_BW, MOD) a1(m_result, m_result, Z);
    */
    
        genvar i;
    generate
        for (i = 0; i < N_CHANNELS; i = i + 1) begin
            integer L = CH_BW*i;
             MOD_MAC#(CH_BW, MOD) mac(CLK,A[L+:CH_BW],B[L+:CH_BW], reset, Z[L+:CH_BW]);
        end
    endgenerate
  /*  
    // Parallel generation
    genvar i;
    generate
        for (i = 0; i < N_CHANNELS; i = i + 1) begin
             integer L = CH_BW*i;     
            // Core MAC functions
            always @(posedge CLK) begin
                if (reset) begin
                    Z_out <= 0;
                end
                
                else begin   
                    Z_out[i*CH_BW+:CH_BW] <= Z_out[L+:CH_BW] + (A[L+:CH_BW] * B[L+:CH_BW]) % MOD;  
                end
             // end Core functions 
            
            end
        end
    endgenerate
    // end parallel generation
    assign Z = Z_out;    
    */          
endmodule


