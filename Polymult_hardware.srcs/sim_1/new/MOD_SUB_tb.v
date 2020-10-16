`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/15/2020 04:31:53 PM
// Design Name: 
// Module Name: MOD_SUB_tb
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


module MOD_SUB_tb(

    );
                                                            
    parameter           MOD        = 32'd4294967291;
    parameter integer CH_BW        = $clog2(MOD);  
    
    // Test parameters
    parameter N_TESTS  = 2000;
  
  
    //one per channel
    reg CLK = 1;
    reg  [CH_BW-1:0] A;
    reg  [CH_BW-1:0] B;
    reg  [CH_BW-1:0] M = MOD;
    wire [CH_BW-1:0] Z;
    
    reg correct = 0;
    reg [$clog2(N_TESTS)-1:0] TOTAL_CORRECT = 0;
    reg [$clog2(N_TESTS)-1:0] TEST_NO       = -1;
    
    // instantiate RNS module    
    MOD_SUB #(CH_BW)  uut (
      .A(A),
      .B(B),
      .M(M),
      .Z(Z)
     );
     
     wire [CH_BW:0] SUB = A + M - B;
     
    // start clock 
    initial begin
      for (integer i = 0; i < N_TESTS; i = i+1) begin
       
       // Edge 1
          #1
          CLK <= ~CLK;
          TEST_NO <= TEST_NO + 1;
          // Get correct answers
          if (Z == (SUB) % M) begin
              correct <= 1;
              TOTAL_CORRECT <= TOTAL_CORRECT + 1;
          end
          else begin
              correct <= 0;     
          end
         
          ////////////////////////////////////////      
      // Edge 2 
          #1
          // Get random inputs
          A <= $random % MOD;  //random
          B <= $random % MOD;  //random
          CLK <= ~CLK;
      end
    end
endmodule
