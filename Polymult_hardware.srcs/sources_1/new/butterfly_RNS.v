`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 07/10/2020 10:59:27 AM
// Design Name: 
// Module Name: butterfly_RNS
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


module butterfly_RNS
// this version keeps numbers in both bases
#(
parameter CH_BW        = 32,                           //RNS channel bitwidth
parameter N_CHANNELS   = 4,                            //Number of RNS channels
parameter RNS_BW = CH_BW * N_CHANNELS,                 //total RNS buswidth
parameter EXT_BW = CH_BW * (N_CHANNELS+1),                     //total RNS_EXT buswidth + m_r
parameter RNS     = {32'd4294967291, 32'd4294967279, 32'd4294967231, 32'd4294967197},  //RNS channels
parameter RNS_EXT = {32'd4294967189, 32'd4294967161, 32'd4294967143, 32'd4294967111, 32'd4294967087},  //extended RNS channels with redundant channel 

parameter TOTAL_RNS = {32'd4294967291, 32'd4294967279, 32'd4294967231, 32'd4294967197,32'd4294967189, 32'd4294967161, 32'd4294967143, 32'd4294967111, 32'd4294967087},
parameter TOTAL_CH = N_CHANNELS*2 + 1,
parameter TOTAL_BW = RNS_BW + EXT_BW
)
(
input wire CLK,
input  wire [TOTAL_BW-1:0]  A,
input  wire [TOTAL_BW-1:0]  B,
input  wire [1:0] phi_select, 
output wire [TOTAL_BW-1:0]  Y,
output wire [TOTAL_BW-1:0]  Z
    );

// This version keeps numbers in both bases and is not pipelined
   reg [TOTAL_BW-1:0] phitable [1:0]; //this is the phi table, specific per stage
   
   initial begin 
       phitable[0] = 'h429496724294967242949672429496724294967242949672429496724294967242949672;
       phitable[1] = 'h429496724294967242949672429496724294967242949672429496724294967242949672;
       phitable[2] = 'h429496724294967242949672429496724294967242949672429496724294967242949672;
       phitable[3] = 'h429496724294967242949672429496724294967242949672429496724294967242949672;

   end
   
   //assign phi
    wire [TOTAL_BW-1:0] phi;
    assign phi = phitable[phi_select];
     
   wire [TOTAL_BW-1:0] mult_result;
    
   
   //butterfly mod multiply by phi
   RNS_modmult #(CH_BW,N_CHANNELS,RNS_BW,EXT_BW,RNS,RNS_EXT) mm (CLK, B, phi, mult_result);
    
 
  //buterfly add and subtract
  RNS_ADD #(CH_BW,TOTAL_CH,TOTAL_BW,TOTAL_RNS) ra(A,mult_result,Y);
  RNS_SUB #(CH_BW,TOTAL_CH,TOTAL_BW,TOTAL_RNS) rs(A,mult_result,Z);

endmodule
/*
// This version keeps numbers in both bases and is pipelined
   reg [TOTAL_BW-1:0] phitable [1:0]; //this is the phi table, specific per stage
   
   initial begin 
       phitable[0] = 'h429496724294967242949672429496724294967242949672429496724294967242949672;
       phitable[1] = 'h429496724294967242949672429496724294967242949672429496724294967242949672;
       phitable[2] = 'h429496724294967242949672429496724294967242949672429496724294967242949672;
       phitable[3] = 'h429496724294967242949672429496724294967242949672429496724294967242949672;

   end
   
   //assign phi
    wire [TOTAL_BW-1:0] phi;
    assign phi = phitable[phi_select];
     
   wire [TOTAL_BW-1:0] mult_result;
   
    //Delay at modmult inputs
   wire [TOTAL_BW-1:0] B_reg;
   wire [TOTAL_BW-1:0] phi_reg; 
    
   delay #(1, TOTAL_BW) d1 (CLK,B,B_reg);
   delay #(1, TOTAL_BW) d2 (CLK,phi,phi_reg);
    
   //Delay at modmult parallel path
   parameter MODMULT_DELAY = 4;
   wire [TOTAL_BW-1:0] A_reg; 
   delay #(MODMULT_DELAY, TOTAL_BW) d0 (CLK,A,A_reg);
   
   //butterfly mod multiply by phi
   RNS_modmult #(CH_BW,N_CHANNELS,RNS_BW,EXT_BW,RNS,RNS_EXT) mm (CLK, B_reg, phi_reg, mult_result);
   


 // Delay before add/subtract   
 wire [TOTAL_BW-1:0] mult_result_reg;
 wire [TOTAL_BW-1:0] A_reg_2;
 
 delay #(1, TOTAL_BW) d3 (CLK, mult_result, mult_result_reg);
 delay #(1, TOTAL_BW) d4 (CLK, A_reg, A_reg_2);
 
 
  //buterfly add and subtract
  RNS_ADD #(CH_BW,TOTAL_CH,TOTAL_BW,TOTAL_RNS) ra(A_reg_2,mult_result_reg,Y);
  RNS_SUB #(CH_BW,TOTAL_CH,TOTAL_BW,TOTAL_RNS) rs(A_reg_2,mult_result_reg,Z);

endmodule
*/
/* this version does not keep numbers in both bases
#(
parameter CH_BW        = 32,                           //RNS channel bitwidth
parameter N_CHANNELS   = 4,                            //Number of RNS channels
parameter RNS_BW = CH_BW * N_CHANNELS,                 //total RNS buswidth
parameter EXT_BW = CH_BW * (N_CHANNELS+1),                     //total RNS_EXT buswidth + m_r
parameter RNS     = {32'd4294967291, 32'd4294967279, 32'd4294967231, 32'd4294967197},  //RNS channels
parameter RNS_EXT = {32'd4294967189, 32'd4294967161, 32'd4294967143, 32'd4294967111, 32'd4294967087}  //extended RNS channels with redundant channel 
)
(
input wire CLK,
input  wire [RNS_BW-1:0]  A,
input  wire [RNS_BW-1:0]  B,
output wire [RNS_BW-1:0]  Y,
output wire [RNS_BW-1:0]  Z
    );

   reg [RNS_BW-1:0] phitable [4:0]; //this is the phi table, specific per stage
   reg [7:0] phi_idx [5:0];    //this is repeated address order for the phi table
   
   initial begin 
       phitable[0] = 2;
       phitable[1] = 4;
       phitable[2] = 6;
       phitable[3] = 8;
       
       phi_idx[0] = 0;
       phi_idx[1] = 4;
       phi_idx[2] = 3;
   end
   
   //assign phi
    //Phi is stored in a ROM. Make a list of the binary addr vals in order of their call and hopefully there is
    //a pattern with this serial butterflies. Each stage alternates between several set addresses. Just have those saved locally as well
    wire [RNS_BW-1:0] phi;
    assign phi = 32'd4294967291; //phitable[0];
     
   wire [RNS_BW-1:0] mult_result;
   
   //butterfly mod multiply by phi
   RNS_modmult #(CH_BW,N_CHANNELS,RNS_BW,EXT_BW,RNS,RNS_EXT) mm (CLK,B, phi, mult_result);

  //buterfly add and subtract
  RNS_ADD #(CH_BW,N_CHANNELS,RNS_BW,RNS) ra(A,mult_result,Y);
  RNS_SUB #(CH_BW,N_CHANNELS,RNS_BW,RNS) rs(A,mult_result,Z);


endmodule
*/