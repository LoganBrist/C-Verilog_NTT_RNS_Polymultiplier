`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/14/2020 04:56:43 PM
// Design Name: 
// Module Name: mem_4to2_output
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


module mem_4to2_output
// Multiplexes 2port ram into 4 port ram
  #(
     parameter integer BIT_WIDTH = 4, 
     parameter integer ADDR_WIDTH = 6
     )
   (
   //single port inputs
    input  wire [BIT_WIDTH-1:0]  Data_in,

    //double port outputs
    output  reg [BIT_WIDTH-1:0]   Data1_out,
    output  reg [BIT_WIDTH-1:0]  Data2_out,
    
    input  wire          CLK,
    input  wire          DOUBLE_CLK
    );
    
    /*
    assign Data2_out = Data_in;
    //when CLK == 0, save 
    always @(posedge DOUBLE_CLK) begin
    Data1_out <= Data_in;
    end
   */
    always @(*) begin
        Data2_out <= Data_in;
        if(CLK) begin
            Data1_out <= Data_in;
        end
    end    
        


    endmodule