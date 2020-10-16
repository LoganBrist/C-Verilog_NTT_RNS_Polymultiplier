`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/15/2020 11:24:58 PM
// Design Name: 
// Module Name: RNS_MAC_tb
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


module RNS_MAC_tb(

    );
  
    // RNS module parameters   
    parameter integer CH_BW        = 32;                                                                    //RNS channel bitwidth
    parameter integer N_CHANNELS   = 4;                                                                     //Number of RNS channels, stay constant for test
    parameter RNS_BW               = CH_BW * N_CHANNELS;                                                    //total RNS buswidth
    parameter MOD              = 32'd4294967291;                                                            //RNS channels
    
    // Test parameters
    parameter N_TESTS  = 2000;
    reg [$clog2(N_TESTS)-1:0] TOTAL_CORRECT = 0;
    reg [$clog2(N_TESTS)-1:0] TEST_NO       = -1;
  
  
    //one per channel
    reg CLK = 1;
    reg  [CH_BW-1:0] A0;
    reg  [CH_BW-1:0] B0;
    reg  [CH_BW-1:0] A1;
    reg  [CH_BW-1:0] B1;
    reg  [CH_BW-1:0] A2;
    reg  [CH_BW-1:0] B2;
    reg  [CH_BW-1:0] A3;
    reg  [CH_BW-1:0] B3;
    
    wire  [CH_BW-1:0] Z;
    
  
    // instantiate RNS module    
    RNS_MAC #(CH_BW, N_CHANNELS, RNS_BW, MOD)  uut (
      .A({A0,A1,A2,A3}),
      .B({B0,B1,B2,B3}),
      .Z(Z)
     );
    
    //Answer 
    wire [CH_BW*2:0] mul0, mul1, mul2, mul3;
    assign mul0 = A0*B0; 
    assign mul1 = A1*B1;
    assign mul2 = A2*B2;
    assign mul3 = A3*B3;
    
    wire [CH_BW+N_CHANNELS:0] sum;
    assign sum = (mul0 % MOD) + (mul1 % MOD) + (mul2 % MOD) + (mul3 % MOD);
       
    // start clock 
    initial begin
      for (integer i = 0; i < N_TESTS; i = i+1) begin
       
       // Edge 1
          #1
          CLK <= ~CLK;
          TEST_NO <= TEST_NO + 1;
          
          // Get correct answers
          if (Z == sum % MOD) begin
              TOTAL_CORRECT <= TOTAL_CORRECT + 1;
          end
     
      // Edge 2 
          #1
          
          // Get random inputs
          A0 <= $random % MOD;  //random
          A1 <= $random % MOD;  //random
          A2 <= $random % MOD;  //random
          A3 <= $random % MOD;  //random
          
          B0 <= $random % MOD;  //random
          B1 <= $random % MOD;  //random
          B2 <= $random % MOD;  //random
          B3 <= $random % MOD;  //random
          CLK <= ~CLK;
      end
    end
    
  endmodule