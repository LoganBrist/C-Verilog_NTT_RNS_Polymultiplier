`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 08/06/2020 05:11:40 PM
// Design Name: 
// Module Name: BASE_EXTENSION_SHENOY
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

module BASE_EXTENSION_SHENOY
    #(
    parameter CH_BW      = 32,
    parameter N_CHANNELS = 4,
    parameter RNS_BW     = CH_BW * N_CHANNELS,                 //total RNS buswidth 
    parameter EXT_BW     = CH_BW * (N_CHANNELS + 1),                     //total RNS_EXT buswidth + m_r
    parameter RNS        = {32'd4294967291, 32'd4294967279, 32'd4294967231, 32'd4294967197},
    parameter RNS_EXT    = {32'd4294967189, 32'd4294967161, 32'd4294967143, 32'd4294967111, 32'd4294967087},
    parameter [CH_BW-1:0] M_R = RNS_EXT[CH_BW*N_CHANNELS +: CH_BW]
    )
    (
    input wire [EXT_BW-1:0] A,
    output wire [RNS_BW-1:0] Z
    );
    
    //assign num_RNS_out = num_RNS;
    
    //Precomputed tables 
    reg [EXT_BW-1:0] D2_J_INV_RED_J =  'hC9C4644BEB441A2C8C96E0C1BE609F1A58CC8CA2;
    reg [EXT_BW-1:0] D2_J_INV_RED_R =  'h00018480000217800002C3A000067320000CD2C0;
    reg [CH_BW-1:0]  D2_INV_RED_R   =  'h6C7E999A; 
    
    reg [RNS_BW-1:0] D2_J0_RED_I0    =  'hFFCB27DBFFD6DC6FFFF4B83FFFFD728D;
    reg [RNS_BW-1:0] D2_J1_RED_I1    =  'hFFD6899BFFE09F6FFFF93B3FFFFF6E7D;
    reg [RNS_BW-1:0] D2_J2_RED_I2    =  'hFFDB948BFFE4C68FFFFA9D9FFFFF9EDD;
    reg [RNS_BW-1:0] D2_J3_RED_I3    =  'hFFE20E0BFFE9F60FFFFC0D1FFFFFC2DD;
    //reg [RNS_BW-1:0] D2_J4_RED_I4    =  'hFFE6562BFFED85AFFFFDC8BF000138C0;     //extra modulus, not used 
    
    reg [RNS_BW-1:0] D2_RED_I       =  'h150E1CC00E767B0001D9BB0000146880;
    
    

   
    //constant
    //wire [CH_BW-1:0] M_R = RNS_EXT[CH_BW*4 +: CH_BW];
    
    //Arithmetic block inputs
    wire [EXT_BW-1:0] E_j;
    wire [RNS_BW-1:0] int_result2;
    wire [CH_BW-1:0] t1, int_result, beta;
    
    // Shenoy Stages
    //Step 1 - Calculate E_j (1 cycle) 
    RNS_MULT #(CH_BW, N_CHANNELS+1, EXT_BW, RNS_EXT) m0(A, D2_J_INV_RED_J, E_j); 
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // Can be in parallel
    
    //step 2-4 - accumulate t with respect to m_r (j cycles) 
    RNS_MAC #(CH_BW, N_CHANNELS + 1, EXT_BW, RNS_EXT[CH_BW*4 +: CH_BW]) mac0( E_j, D2_J_INV_RED_R, t1);
     
    //step 5 - determine beta (1 cycle) 
    MOD_SUB  #(M_R,CH_BW)  sub1 (t1,RNS[CH_BW*(N_CHANNELS-1) +: CH_BW],int_result);
    MOD_MULT #(M_R,CH_BW) mult1 (D2_INV_RED_R, int_result, beta);
    //assign int_result = (t1 + M_R - RNS[CH_BW*(N_CHANNELS-1) +: CH_BW]) % M_R;
    //assign beta = (D2_INV_RED_R * int_result) % M_R;

///////////////////////////////////////////////////////////////////////////////////////////////////////
   // With this

    //step 6-8 - accumulate t (j additions, i channels) 
    wire [CH_BW-1:0] out0, out1, out2, out3;
    wire [RNS_BW-1:0] t_i;
    RNS_MAC #(CH_BW, N_CHANNELS + 1, EXT_BW, RNS[CH_BW*0 +: CH_BW]) mac1(E_j, D2_J0_RED_I0, out0);
    RNS_MAC #(CH_BW, N_CHANNELS + 1, EXT_BW, RNS[CH_BW*1 +: CH_BW]) mac2(E_j, D2_J1_RED_I1, out1);
    RNS_MAC #(CH_BW, N_CHANNELS + 1, EXT_BW, RNS[CH_BW*2 +: CH_BW]) mac3(E_j, D2_J2_RED_I2, out2);
    RNS_MAC #(CH_BW, N_CHANNELS + 1, EXT_BW, RNS[CH_BW*3 +: CH_BW]) mac4(E_j, D2_J3_RED_I3, out3);

    assign t_i = {out0, out1, out2, out3};
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    //step 9 - final output //D2_RED_I
    wire [RNS_BW-1:0] Z_out;
    RNS_MULT #(CH_BW, N_CHANNELS, RNS_BW, RNS) m2({beta,beta,beta,beta}, D2_RED_I, int_result2);   //and this can come immediately after beta is assigned
    RNS_SUB  #(CH_BW, N_CHANNELS, RNS_BW, RNS) s1(t_i, int_result2, Z_out);
    
    //return Z_out
    assign Z = Z_out;
    
endmodule