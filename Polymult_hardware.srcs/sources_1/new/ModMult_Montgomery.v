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
// this version represents values in both bases
#(
parameter CH_BW        = 32,                                 //RNS channel bitwidth
parameter N_CHANNELS   = 4,                                  //Number of RNS channels
parameter RNS_BW = CH_BW * N_CHANNELS,                       //total RNS buswidth
parameter EXT_BW = CH_BW * (N_CHANNELS+1) ,                     //total RNS_EXT buswidth + m_r
parameter RNS     = {32'd4294967291, 32'd4294967279, 32'd4294967231, 32'd4294967197},  //RNS channels
parameter RNS_EXT = {32'd4294967189, 32'd4294967161, 32'd4294967143, 32'd4294967111, 32'd4294967087},  //extended RNS channels with redundant channel 
parameter TOTAL_RNS = {32'd4294967291, 32'd4294967279, 32'd4294967231, 32'd4294967197,32'd4294967189, 32'd4294967161, 32'd4294967143, 32'd4294967111, 32'd4294967087},
parameter TOTAL_CH = N_CHANNELS*2 + 1,
parameter TOTAL_BW = RNS_BW + EXT_BW
)
(
input  wire CLK,
input  wire [TOTAL_BW-1:0] A, 
input  wire [TOTAL_BW-1:0] B,
output wire [TOTAL_BW-1:0] Z
);

    // IO delay
    wire [TOTAL_BW-1:0] A_ff, A_prime_ff, B_ff;
    delay #(1, TOTAL_BW) A_input_delay_1 (CLK, A, A_ff);
    delay #(2, TOTAL_BW) B_input_delay_2 (CLK, B, B_ff);
        
// This version is in both bases and is not pipelined  
  //constants 
    reg [RNS_BW-1:0] M_INV_RED_I  = 'h94708C202400771D532F537717C1BA40;
    reg [EXT_BW-1:0] M_RED_J      = 'h0002512300025123000251230002512300025123;
    reg [EXT_BW-1:0] D1_INV_RED_J = 'h9DE04F1F71F7DDBD4314D34AC67A7E125AF42B81;
    reg [TOTAL_BW-1:0] D1         = 'h9DE04F1F_71F7DDBD_4314D34A_C67A7E12_9DE04F1F_71F7DDBD_4314D34A_C67A7E12_5AF42B81;
    
// 0. Get rid of montgomery factor
   wire [TOTAL_BW-1:0] A_prime;
   RNS_MULT #(CH_BW,TOTAL_CH,TOTAL_BW, TOTAL_RNS) mul01(A_ff,D1,A_prime);
    
    //A mult delay
    delay #(1, TOTAL_BW) A_prime_delay_1 (CLK, A_prime, A_prime_ff);
    
// 1. Find X = A * B (in base i and j)
   wire[TOTAL_BW-1:0] X; 
   RNS_MULT #(CH_BW,TOTAL_CH,TOTAL_BW, TOTAL_RNS) mul0(A_prime_ff,B_ff,X);
   
   // X delay
   wire [TOTAL_BW-1:0] X_ff;
   delay #(1, TOTAL_BW) X_delay_1 (CLK, X, X_ff);
   
   wire [RNS_BW-1:0] X_i = X_ff[RNS_BW-1:0];
   wire [EXT_BW-1:0] X_j = X_ff[TOTAL_BW-1:RNS_BW];
   
   // parallel to bajard delay
   wire [EXT_BW-1:0] X_j_ff;
   delay #(6, EXT_BW) X_delay_6 (CLK, X_j, X_j_ff);

   
// 2. Find q in RNS base i
   wire [RNS_BW-1:0] int, Q_i;
   RNS_MULT #(CH_BW,N_CHANNELS,RNS_BW,RNS) mul(X_i, M_INV_RED_I, int);
   
      // int delay
   wire [RNS_BW-1:0] int_ff;
   delay #(1, RNS_BW) int_delay_1 (CLK, int, int_ff);
   
   RNS_SUB  #(CH_BW,N_CHANNELS,RNS_BW,RNS) sub(RNS,int_ff,Q_i);
   
   
// 3. Base extend q to q' in base j
   wire [EXT_BW-1:0] Q_j;
   BASE_EXTENSION_BAJARD #(CH_BW, N_CHANNELS, RNS_BW, EXT_BW, RNS, RNS_EXT) bex_delay_4 (Q_i, Q_j, CLK);

   
// 4. Find Z = (X_j + Q_j * M_RED_J) * D1_INV_RED_J in base j      //line 7 in masters report
  wire [EXT_BW-1:0] int0, int1, Z_j;
  RNS_MULT #(CH_BW, N_CHANNELS + 1, EXT_BW, RNS_EXT) m0( Q_j,   M_RED_J, int0); 
  
    // int0 delay
    wire [EXT_BW-1:0] int0_ff;
    delay #(1, EXT_BW) int0_delay_1 (CLK, int0, int0_ff);

  RNS_ADD  #(CH_BW, N_CHANNELS + 1, EXT_BW, RNS_EXT) a0(int0_ff,    X_j_ff, int1); 
  
  // int1 delay
  wire [EXT_BW-1:0] int1_ff;
  delay #(1, EXT_BW) int1_delay_1 (CLK, int1, int1_ff);
  
  RNS_MULT #(CH_BW, N_CHANNELS + 1, EXT_BW, RNS_EXT) m1(int1_ff, D1_INV_RED_J,  Z_j); 
  
   
// 5. Base extend Z to RNS base i 
    wire [RNS_BW-1:0] Z_i; 
    BASE_EXTENSION_SHENOY #(CH_BW, N_CHANNELS, RNS_BW, EXT_BW, RNS, RNS_EXT)  bex_delay_6 (Z_j, Z_i, CLK);
    
    //parallel to shenoy delay
    wire [EXT_BW-1:0] Z_j_ff;
    delay #(6, EXT_BW) Z_delay_6 (CLK, Z_j, Z_j_ff);
    
    assign Z = {Z_i,Z_j_ff};
endmodule

/* 
// This version is in both bases and is pipelined  
  //constants 
    reg [RNS_BW-1:0] M_INV_RED_I  = 'h94708C202400771D532F537717C1BA40;
    reg [EXT_BW-1:0] M_RED_J      = 'h0002512300025123000251230002512300025123;
    reg [EXT_BW-1:0] D1_INV_RED_J = 'h9DE04F1F71F7DDBD4314D34AC67A7E125AF42B81;
   
    
// 0. Get rid of montgomery factor
 //reg [TOTAL_BW-1:0] D1         = 'h
  // wire [TOTAL_BW-1:0] A_prime =

// 1. Find X = A * B (in base i and j)
   wire[TOTAL_BW-1:0] X; 
   RNS_MULT #(CH_BW,TOTAL_CH,TOTAL_BW, TOTAL_RNS) mul0(A,B,X);
   
   wire [RNS_BW-1:0] X_i = X[RNS_BW-1:0];
   wire [EXT_BW-1:0] X_j = X[TOTAL_BW-1:RNS_BW];
   
// 2. Find q in RNS base i
   wire [RNS_BW-1:0] int, Q_i;
   RNS_MULT #(CH_BW,N_CHANNELS,RNS_BW,RNS) mul(X_i, M_INV_RED_I, int);
   RNS_SUB  #(CH_BW,N_CHANNELS,RNS_BW,RNS) sub(RNS,int,Q_i);
   
   // delay before bajard
   wire [RNS_BW-1:0] Q_i_reg;
   delay #(1, RNS_BW) d0 (CLK, Q_i, Q_i_reg);
   
   // parallel aside bajard 
   wire [EXT_BW-1:0] X_j_reg;
   delay #(2, EXT_BW) d3 (CLK, X_j, X_j_reg);
   
// 3. Base extend q to q' in base j
   wire [EXT_BW-1:0] Q_j;
   BASE_EXTENSION_BAJARD #(CH_BW, N_CHANNELS, RNS_BW, EXT_BW, RNS, RNS_EXT) bex1 (Q_i_reg, Q_j);

   // delay after bajard
   wire [RNS_BW-1:0] Q_j_reg;
   delay #(1, RNS_BW) d1 (CLK, Q_j, Q_j_reg);
   
// 4. Find Z = (X_j + Q_j * M_RED_J) * D1_INV_RED_J in base j      //line 7 in masters report
  wire [EXT_BW-1:0] int0, int1, Z_j;
  RNS_MULT #(CH_BW, N_CHANNELS + 1, EXT_BW, RNS_EXT) m0( Q_j_reg,   M_RED_J, int0); 
  RNS_ADD  #(CH_BW, N_CHANNELS + 1, EXT_BW, RNS_EXT) a0(int0,         X_j_reg, int1); 
  RNS_MULT #(CH_BW, N_CHANNELS + 1, EXT_BW, RNS_EXT) m1(int1,D1_INV_RED_J,  Z_j); 

   // delay before shenoy
   wire [RNS_BW-1:0] Z_j_reg;
   delay #(1, RNS_BW) d2 (CLK, Z_j, Z_j_reg);
   
   
// 5. Base extend Z to RNS base i 
    wire [RNS_BW-1:0] Z_i; 
    BASE_EXTENSION_SHENOY #(CH_BW, N_CHANNELS, RNS_BW, EXT_BW, RNS, RNS_EXT)  bex4 (Z_j_reg, Z_i);
    
       // delay after shenoy
    wire [RNS_BW-1:0] Z_j_reg;
    delay #(1, RNS_BW) d4 (CLK, Z_i, Z_i_reg);

       // delay parrallel shenoy
    wire [RNS_BW-1:0] Z_j_par;
    delay #(2, RNS_BW) d5 (CLK, Z_j, Z_j_par);
        
    assign Z = {Z_i_reg,Z_j_par};
endmodule
*/



/* this version does a base conversion to get the second base of A and B
#(
parameter CH_BW        = 32,                                 //RNS channel bitwidth
parameter N_CHANNELS   = 4,                                  //Number of RNS channels
parameter RNS_BW = CH_BW * N_CHANNELS,                       //total RNS buswidth
parameter EXT_BW = CH_BW * (N_CHANNELS+1) ,                     //total RNS_EXT buswidth + m_r
parameter RNS     = {32'd4294967291, 32'd4294967279, 32'd4294967231, 32'd4294967197},  //RNS channels
parameter RNS_EXT = {32'd4294967189, 32'd4294967161, 32'd4294967143, 32'd4294967111, 32'd4294967087}  //extended RNS channels with redundant channel 
)
(
input  wire CLK,
input  wire [RNS_BW-1:0] A, //A & B are not input size 2n. They need base extended in parallel with q
input  wire [RNS_BW-1:0] B,
output wire [RNS_BW-1:0] Z
);
    

// 1. Find q in RNS base i
   wire [RNS_BW-1:0] Q_i;
   wire [EXT_BW-1:0] Q_j;
   RNS_MULT #(CH_BW,N_CHANNELS,RNS_BW,RNS) mul(A,B,Q_i);

// multiply by -n^-1
   
// 2. Base extend q to q' in base j

wire [EXT_BW-1:0] bajard_out;             //out
wire [EXT_BW-1:0] shenoy_in;              
wire [RNS_BW-1:0] shenoy_out;


BASE_EXTENSION_BAJARD #(CH_BW, N_CHANNELS, RNS_BW, EXT_BW, RNS, RNS_EXT) bex1 (Q_i, Q_j);

 //2b. Base Extend A and B
 wire [EXT_BW-1:0] A_ext, B_ext, E_j;
 wire bajard_done_flag2, bajard_done_flag3;
 
 BASE_EXTENSION_BAJARD #(CH_BW, N_CHANNELS, RNS_BW, EXT_BW, RNS, RNS_EXT) bex2 (A, A_ext);
      
 BASE_EXTENSION_BAJARD #(CH_BW, N_CHANNELS, RNS_BW, EXT_BW, RNS, RNS_EXT) bex3 (B, B_ext);
   
// 3. Find Z = (X + q'M)D^-1 in base j //line 7 in masters report
   RNS_MULT #(CH_BW, N_CHANNELS + 1, EXT_BW, RNS_EXT) m0(A_ext,B_ext,E_j); 
   
   //muly t,n_j
   //RNS_MULT #(CH_BW, N_CHANNELS + 1, EXT_BW, RNS_EXT) m0(Q_j, );
  
   //add ab + tn_j
   RNS_ADD #(CH_BW, N_CHANNELS + 1, EXT_BW, RNS_EXT) a0(E_j,Q_j, shenoy_in); 
      
   //multiply by M^-1 
    
// 4. Base extend Z to RNS base i  
  
    BASE_EXTENSION_SHENOY #(CH_BW, N_CHANNELS, RNS_BW, EXT_BW, RNS, RNS_EXT)  bex4 (shenoy_in, Z);
    
endmodule
*/