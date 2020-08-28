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
    parameter CH_BW      = 5,
    parameter N_CHANNELS = 4,
    parameter RNS_BW     = CH_BW * N_CHANNELS,                 //total RNS buswidth 
    parameter EXT_BW     = CH_BW * (N_CHANNELS+1),             //total RNS_EXT buswidth + m_r
    parameter RNS     = {5'd1, 5'd0, 5'd0, 5'd2},
    parameter RNS_EXT = {5'd1, 5'd0, 5'd0, 5'd2, 5'd10},
    parameter TABLE_D1_I_INV_RED_I = "" /*"TABLE_D1_I_INV_RED_I.txt"*/,
    parameter M_R = 32'd10
    )
    (
    input wire CLK,
    input wire [EXT_BW-1:0] num_RNS,
    output wire [RNS_BW-1:0] num_RNS_out
    );
    
    //assign num_RNS_out = num_RNS;
    
    //Precomputed tables 
    reg [EXT_BW-1:0] D2_J_INV_RED_J [0:0]; 
    reg [EXT_BW-1:0] D2_J_RED_R [0:0];

    initial begin 
        if(TABLE_D1_I_INV_RED_I != "")
            $readmemh(TABLE_D1_I_INV_RED_I, D1_I_INV_RED_I);
   end
   
    //Arithmetic block inputs
    wire [EXT_BW-1:0] E_j, t1, int_result, beta, t2;
    wire [RNS_BW-1:0] int_result2;
    
    // Shenoy Stages
    //Step 1 - Calculate E_j (1 cycle) //D2_J_INV_RED_J[0]
    RNS_MULT #(CH_BW, N_CHANNELS+1, EXT_BW, RNS_EXT) m0(num_RNS, num_RNS, E_j); 
    
    //step 2-4 - accumulate t with respect to m_r (j cycles) //D2_J_RED_R[0]
    RNS_MAC_SINGLE_MOD #(CH_BW, N_CHANNELS+1, EXT_BW, M_R) mac0(CLK, E_j, E_j, reset, t1);
     
    //step 5 - determine beta (1 cycle) //D2_INV_RED_R
    RNS_SUB_SINGLE_MOD  #(CH_BW, N_CHANNELS+1, EXT_BW, M_R) s0(t1, num_RNS, int_result);
    RNS_MULT_SINGLE_MOD #(CH_BW, N_CHANNELS+1, EXT_BW, M_R) m1(int_result, int_result, beta); 

    //step 6-8 - accumulate t with respect to m_i (i cycles, parallel in j/r) //D2_J_RED_I
     RNS_MAC #(CH_BW, N_CHANNELS+1, EXT_BW, RNS_EXT) mac1(CLK,E_j, E_j, reset2, t2);
    
    //step 9 - final output //D2_RED_I
    RNS_MULT #(CH_BW, N_CHANNELS, RNS_BW, RNS) m2(beta, beta, int_result2);
    RNS_SUB  #(CH_BW, N_CHANNELS, RNS_BW, RNS) s1(t2, int_result2, Z_out);
    
    //return Z_out
    assign num_RNS_out = Z_out;
    
endmodule