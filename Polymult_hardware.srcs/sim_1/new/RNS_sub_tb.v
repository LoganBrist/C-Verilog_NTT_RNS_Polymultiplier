`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/14/2020 05:14:39 PM
// Design Name: 
// Module Name: RNS_sub_tb
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


module RNS_sub_tb(

    );
      // RNS module parameters   
      parameter integer CH_BW        = 32;                                                                    //RNS channel bitwidth
      parameter integer N_CHANNELS   = 4;                                                                     //Number of RNS channels, stay constant for test
      parameter RNS_BW               = CH_BW * N_CHANNELS;                                                    //total RNS buswidth
      parameter RNS_MOD              = {32'd4294967291, 32'd4294967279, 32'd4294967231, 32'd4294967197};      //RNS channels
      
      // Test parameters
      parameter N_TESTS  = 2000;
      reg [$clog2(N_TESTS)-1:0] TOTAL_CORRECT = 0;
      reg [$clog2(N_TESTS)-1:0] TEST_NO       = -2;

          
      //one per channel
      reg CLK = 1;
      reg  [CH_BW-1:0] A0;
      reg  [CH_BW-1:0] B0;
      wire [CH_BW-1:0] M0;
      wire  [CH_BW-1:0] Z0;
      
      reg correct0 = 0;
        
      reg  [CH_BW-1:0] A1;
      reg  [CH_BW-1:0] B1;
      wire [CH_BW-1:0] M1;
      wire  [CH_BW-1:0] Z1;
      
      reg correct1 = 0;
        
      reg  [CH_BW-1:0] A2;
      reg  [CH_BW-1:0] B2;
      wire [CH_BW-1:0] M2;
      wire  [CH_BW-1:0] Z2;
      
      reg correct2 = 0;
        
      reg  [CH_BW-1:0] A3;
      reg  [CH_BW-1:0] B3;
      wire [CH_BW-1:0] M3;
      wire  [CH_BW-1:0] Z3;
    
      reg correct3 = 0;
    
      // if all four channels are are correct
      wire ALLCORRECT = correct0 & correct1 & correct2 & correct3;
      
       //To separate moduli
      assign {M0,M1,M2,M3} = RNS_MOD;
      
      // instantiate RNS module    
      RNS_SUB #(CH_BW, N_CHANNELS, RNS_BW, RNS_MOD)  uut (
        .A({A0,A1,A2,A3}),
        .B({B0,B1,B2,B3}),
        .Z({Z0,Z1,Z2,Z3})
       );
       
       
       wire [CH_BW:0] sub0, sub1, sub2, sub3;
       assign sub0 = A0 + M0 - B0; 
       assign sub1 = A1 + M1 - B1;
       assign sub2 = A2 + M2 - B2;
       assign sub3 = A3 + M3 - B3;
        
      // start clock 
      initial begin
        for (integer i = 0; i < N_TESTS; i = i+1) begin
         
         // Edge 1
            #1
            CLK <= ~CLK;
            TEST_NO <= TEST_NO + 1;
            
            // Get correct answers
            ////////////////////////////////////////
            if (Z0 == sub0 % M0) begin
                correct0 <= 1;
            end
            else begin
                correct0 <= 0;     
            end
            /////////////////////////////////////////
            if (Z1 == sub1 % M1) begin
                correct1 <= 1;
            end
            else begin
                correct1 <= 0;     
            end
            ////////////////////////////////////////
            if (Z2 == sub2 % M2) begin
                correct2 <= 1;
            end
            else begin
                correct2 <= 0;     
            end
            ////////////////////////////////////////
            if (Z3 == sub3 % M3) begin
                correct3 <= 1;
            end
            else begin
                correct3 <= 0;     
            end 
            //////////////////////////////////////// 
            if (ALLCORRECT) begin
                TOTAL_CORRECT <= TOTAL_CORRECT + 1;
            end     
        // Edge 2 
            #1
            
            // Get random inputs
            A0 <= $random % M0;  //random
            A1 <= $random % M1;  //random
            A2 <= $random % M2;  //random
            A3 <= $random % M3;  //random
            
            B0 <= $random % M0;  //random
            B1 <= $random % M1;  //random
            B2 <= $random % M2;  //random
            B3 <= $random % M3;  //random
            CLK <= ~CLK;
        end
      end
      
    endmodule
