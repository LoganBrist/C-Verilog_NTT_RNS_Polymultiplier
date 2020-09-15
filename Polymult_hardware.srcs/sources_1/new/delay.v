`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/04/2020 05:56:17 PM
// Design Name: 
// Module Name: delay
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


module delay #(parameter N_CYCLES = 1, parameter BIT_WIDTH = 8) (
    input wire CLK,
    input  wire [BIT_WIDTH-1:0] in,
    output reg [BIT_WIDTH-1:0] out
    );
   
    if (N_CYCLES == 1) begin
        always @(posedge CLK) begin
            out <= in;
        end
    end
            
    else begin
        reg [BIT_WIDTH-1:0] buffer [N_CYCLES-2:0];
        integer i = 1;
        always @(posedge CLK) begin
            buffer[0] <= in;
            for (i = 1; i < N_CYCLES-1; i = i + 1) begin
                buffer [i] <= buffer[i-1];
            end
            out <= buffer[N_CYCLES-2]; 
       end 
   end
endmodule
