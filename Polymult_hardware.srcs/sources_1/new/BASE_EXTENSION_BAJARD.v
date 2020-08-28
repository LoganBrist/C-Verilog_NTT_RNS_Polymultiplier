`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 08/06/2020 05:11:40 PM
// Design Name: 
// Module Name: BASE_EXTENSION_BAJARD
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


module BASE_EXTENSION_BAJARD
    #(
    parameter CH_BW      = 32,
    parameter N_CHANNELS = 4,
    parameter RNS_BW     = CH_BW * N_CHANNELS,                 //total RNS buswidth 
    parameter EXT_BW     = CH_BW * (N_CHANNELS + 1),                     //total RNS_EXT buswidth + m_r
    parameter RNS     = {32'd4294967291, 32'd4294967279, 32'd4294967231, 32'd4294967197},
    parameter RNS_EXT = {32'd4294967189, 32'd4294967161, 32'd4294967143, 32'd4294967111, 32'd4294967087},
    parameter TABLE_D1_I_INV_RED_I = ""/*"TABLE_D1_I_INV_RED_I.txt"*/,
    parameter TABLE_D1_I_RED_J     = ""/*"TABLE_D1_I_RED_J.txt"*/
    )
    (
    input wire CLK,
    input wire [RNS_BW-1:0] num_RNS,
    output wire[EXT_BW-1:0] num_RNS_out
    );
    
    //assign num_RNS_out = num_RNS;
    
    
    //Precomputed tables 
    reg [RNS_BW-1:0] D1_I_INV_RED_I [0:0]; //Holds one line, with channels (D1/i)^-1 mod m_i
    reg [EXT_BW-1:0] D1_I_RED_J [N_CHANNELS-1:0]; //Holds i lines, with channels (D1/i) mod m_j/m_r


    initial begin 
        if(TABLE_D1_I_INV_RED_I != "")
            $readmemh(TABLE_D1_I_INV_RED_I, D1_I_INV_RED_I);
        if(TABLE_D1_I_RED_J != "")
            $readmemh(TABLE_D1_I_RED_J, D1_I_RED_J);
   end
  
    //Step 1 - Calculate sigma (1 cycles)
    wire [RNS_BW-1:0] sigma;
    RNS_MULT #(CH_BW, N_CHANNELS, RNS_BW, RNS) m(num_RNS, RNS, sigma); 
    
    //Step 2 - Accumulate in base 2 (i cycles, j channels
    wire [RNS_BW-1:0] out;
    RNS_MAC #(CH_BW, N_CHANNELS, RNS_BW, RNS) mac1(CLK, sigma, RNS, reset, out);
    
    assign num_RNS_out = out;
   
    /*Old- has control logic for feeding variables. Worry about later
    //Arithmetic block inputs
    reg [RNS_BW-1:0] mult_in1, mult_in2, MAC_in1, MAC_in2;
    wire [RNS_BW-1:0] sigma, MAC_out;
    
    // Bajard Stages
    reg [31:0] stage = 0;
    always @(posedge CLK) begin
        if (do_not_run) begin
           stage    <= 0;
           mult_in1 <= 0;
           mult_in2 <= 0;
           MAC_in1  <= 0;
           MAC_in2  <= 0;
           result_ready <= 0;  
           num_RNS_out <= num_RNS_out;            
        end
        
        else if (run && stage == 0) begin
           stage    <= 1;
           mult_in1 <= num_RNS;
           mult_in2 <= D1_I_INV_RED_I[0];
           MAC_in1  <= 0;
           MAC_in2  <= 0;
           num_RNS_out <= num_RNS_out;            
        
        end
        
        else if (run && stage <= N_CHANNELS) begin
           stage    <= stage + 1;
           mult_in1 <= 0;
           mult_in2 <= 0;
           MAC_in1  <= sigma [(stage-2)*CH_BW +:CH_BW]; //goes through i channels
           MAC_in2  <= D1_I_RED_J[stage-1]; //goes through i channels of j + m_r width
           result_ready <= 0; 
           num_RNS_out <= num_RNS_out; 
        end
        
        else if (run && stage == N_CHANNELS + 1) begin
           stage    <= stage + 1;
           mult_in1 <= 0;
           mult_in2 <= 0;
           MAC_in1  <= sigma[(stage-2)*CH_BW +: CH_BW];  //j wide data  (+m_r)
           MAC_in2  <= D1_I_RED_J[stage-1];           //j wide data (+m_r)
           result_ready <= 0; 
           num_RNS_out <= num_RNS_out;         
        end
        
        else begin
           stage    <= stage;
           mult_in1 <= 0;
           mult_in2 <= 0;
           MAC_in1  <= 0;
           MAC_in2  <= 0;
           result_ready <= 1;
           num_RNS_out <= MAC_out;            
        end
            
    end
    
    //Step 1 - Calculate sigma (1 cycles)
    RNS_MULT #(CH_BW, N_CHANNELS, RNS_BW, RNS) m(mult_in1, mult_in2, sigma); 
    
    //Step 2 - Accumulate in base 2 (i cycles, j channels)
    RNS_MAC #(CH_BW, N_CHANNELS+1, EXT_BW, RNS_EXT) mac1(CLK,MAC_in1, MAC_in2, reset, MAC_out);

    //return Z_out
    */
endmodule
