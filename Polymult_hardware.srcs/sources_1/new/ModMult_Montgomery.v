`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 07/09/2020 10:19:28 AM
// Design Name: 
// Module Name: ModMult_Montgomery
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


module RNS_modmult
#(
parameter CH_BW        = 32,                                 //RNS channel bitwidth
parameter N_CHANNELS   = 4,                                  //Number of RNS channels
parameter RNS_BW = CH_BW * N_CHANNELS,                       //total RNS buswidth
parameter EXT_BW = CH_BW * (N_CHANNELS+1) ,                     //total RNS_EXT buswidth + m_r
parameter RNS     = {32'd4294967291, 32'd4294967279, 
                     32'd4294967231, 32'd4294967197},  //RNS channels
parameter RNS_EXT = {32'd4294967189, 32'd4294967161,
                     32'd4294967143, 32'd4294967111, 
                                     32'd4294967087},  //extended RNS channels with redundant channel )
//Table names
parameter TABLE_D1_I_INV_RED_I = ""/*parameter = "TABLE_D1_I_INV_RED_I.txt"*/,
parameter TABLE_D1_I_RED_J     = ""/*parameter = "TABLE_D1_I_RED_J.txt"*/
)
(
input wire CLK,
input wire [RNS_BW-1:0] A, //A & B are not input size 2n. They need base extended in parallel with q
input wire [RNS_BW-1:0] B,
output wire [RNS_BW-1:0] Z
);
    

// 1. Find q in RNS base i
   wire [RNS_BW-1:0] Q_i;
   wire [EXT_BW-1:0] Q_j;
   RNS_MULT #(CH_BW,N_CHANNELS,RNS_BW,RNS) mul(A,B,Q_i);

// multiply by -n^-1
   
// 2. Base extend q to q' in base j
reg               run_bajard_flag = 0, run_shenoy_flag = 0;    //in
wire              bajard_done_flag, shenoy_done_flag;       //out

wire [EXT_BW-1:0] bajard_out;             //out
wire [EXT_BW-1:0] shenoy_in;              
wire [RNS_BW-1:0] shenoy_out;


BASE_EXTENSION_BAJARD
     #(CH_BW, N_CHANNELS, RNS_BW, EXT_BW, RNS, RNS_EXT, TABLE_D1_I_INV_RED_I, TABLE_D1_I_RED_J)
      bex1 (CLK, Q_i, Q_j);

 //2b. Base Extend A and B
 wire [EXT_BW-1:0] A_ext, B_ext, E_j;
 wire bajard_done_flag2, bajard_done_flag3;
 
 BASE_EXTENSION_BAJARD
     #(CH_BW, N_CHANNELS, RNS_BW, EXT_BW, RNS, RNS_EXT, TABLE_D1_I_INV_RED_I, TABLE_D1_I_RED_J)
      bex2 (CLK, A, A_ext);
      
 BASE_EXTENSION_BAJARD
     #(CH_BW, N_CHANNELS, RNS_BW, EXT_BW, RNS, RNS_EXT, TABLE_D1_I_INV_RED_I, TABLE_D1_I_RED_J)
      bex3 (CLK, B, B_ext);
   
// 3. Find Z = (X + q'M)D^-1 in base j //line 7 in masters report
   RNS_MULT #(CH_BW, N_CHANNELS + 1, EXT_BW, RNS_EXT) m0(A_ext,B_ext,E_j); 
   
   //muly t,n_j
   //RNS_MULT #(CH_BW, N_CHANNELS + 1, EXT_BW, RNS_EXT) m0(Q_j, );
  
   //add ab + tn_j
   RNS_ADD #(CH_BW, N_CHANNELS + 1, EXT_BW, RNS_EXT) a0(E_j,Q_j, shenoy_in); 
      
   //multiply by M^-1 
    
// 4. Base extend Z to RNS base i  
  
    BASE_EXTENSION_SHENOY      
    #(CH_BW, N_CHANNELS, RNS_BW, EXT_BW, RNS, RNS_EXT)
      bex4 (CLK, shenoy_in, Z);
    
endmodule
