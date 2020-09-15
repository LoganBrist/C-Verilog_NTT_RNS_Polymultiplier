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
    parameter MOD        = 32'd4294967291
    )
    (
    input  wire CLK,
    input  wire [RNS_BW-1:0]  A,
    input  wire [RNS_BW-1:0]  B,
    output wire  [CH_BW-1:0]  Z
    );
    

             //repeat this loop j times for different Mi_j
   //multiply   
    genvar i;
    wire [RNS_BW-1:0] int_val;  
     generate
         for (i = 0; i < N_CHANNELS; i = i + 1) begin
             integer L = CH_BW*i;                                                          //for each i channel, multiply sigma_i times M_i (jth M_i) and sum
             assign int_val[CH_BW*i +:CH_BW] = (A[L+:CH_BW] * B[L+:CH_BW]) % MOD;        
          end
     endgenerate


//accumulate
    reg [CH_BW+N_CHANNELS-1:0] sum;
    integer idx;
    always @* begin
           sum = {CH_BW{1'b0}};
            for( idx = 0; idx < N_CHANNELS; idx = idx + 1) begin
              sum = sum + int_val[CH_BW*idx +:CH_BW];
            end
     end
           
     assign Z = sum % MOD;
     
endmodule
