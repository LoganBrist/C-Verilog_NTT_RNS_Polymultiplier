`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/24/2020 09:33:42 PM
// Design Name: 
// Module Name: NTT
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


module NTT 
#(parameter W_N = 2,
  parameter MOD = 12289,
  parameter N_NTT_MODULES = 4,
  parameter BIT_WIDTH = 64,
  parameter N_COEFF    = 1024,
  parameter ADDR_WIDTH = $clog2(N_COEFF),
  parameter PHI_FILENAME = ""
)
  (
    input   wire CLK,   
    input   wire [BIT_WIDTH-1:0]  data_in1,
    input   wire [BIT_WIDTH-1:0]  data_in2,
    
    output  wire [BIT_WIDTH-1:0]  data_out1,
    output  wire [BIT_WIDTH-1:0]  data_out2,

    input   wire [BIT_WIDTH-1:0] phi
   );   
    
    //NTT butterfly
    butterfly #(BIT_WIDTH,MOD, ADDR_WIDTH) bf (CLK, data_in1, data_in2, data_out1, data_out2, phi);   

endmodule
