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
    parameter N_CHANNELS = 5,
    parameter RNS_BW     = CH_BW * N_CHANNELS,                 //total RNS buswidth 
    parameter MOD        = 32'd4294967291
    )
    (
    //input  wire CLK,
    input  wire [RNS_BW-1:0]  A,
    input  wire [RNS_BW-1:0]  B,
    output wire  [CH_BW-1:0]  Z,
    input wire CLK
    );
    

//REDUCE-AT-END VERSION WITH PIPELINE:
   //multiply   
    wire [RNS_BW-1:0] int_val; 
    MOD_MULT #(MOD)  mult[RNS_BW/CH_BW-1:0] (.A(A), .B(B), .Z(int_val));  //written this way -> RNS_BW/CH_BW <- because Shenoy MAC requires summing over 4 channels but on EXT_BW data. This ensures modmult is correct 

// Delay
wire [RNS_BW-1:0] int_val_ff;
delay #(1, RNS_BW) d0 (CLK, int_val, int_val_ff); 


//accumulate
    reg [CH_BW+N_CHANNELS-1:0] sum;
    integer idx;
    always @* begin
           sum = {CH_BW{1'b0}};
            for( idx = 0; idx < N_CHANNELS; idx = idx + 1) begin
              sum = sum + int_val_ff[CH_BW*idx +:CH_BW];
            end
     end
     
// Reduce sum to output Z          
    REDUCE #(CH_BW+N_CHANNELS,MOD) red (sum, Z);


/*
//REDUCE-AT-ADDS VERSION: (SLOWER)
   //multiply   
    wire [RNS_BW-1:0] int_val; 
    MOD_MULT #(MOD)  mult[N_CHANNELS-1:0] (.A(A), .B(B), .Z(int_val));

//accumulate and reduce
    wire [CH_BW*(N_CHANNELS+1):0] sum;
    assign sum[0 +:CH_BW] = int_val[0 +:CH_BW]; //initialize first val
      genvar i;
      generate
          for (i = 0; i < N_CHANNELS-1; i = i + 1) begin                                                 
            MOD_ADD #(MOD) add (sum[CH_BW*i +:CH_BW], int_val[CH_BW*(i+1) +:CH_BW], sum[CH_BW*(i+1) +:CH_BW]);       
           end
      endgenerate   
        
// Assign to output
assign Z = sum[CH_BW*(N_CHANNELS-1) +:CH_BW];

*/

/*
//NON_MODULE VERSION:
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
*/
     
endmodule

