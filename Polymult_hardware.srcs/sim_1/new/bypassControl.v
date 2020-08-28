`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 07/03/2020 10:55:54 AM
// Design Name: 
// Module Name: bypassControl
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


module bypassControl(
    input wire CLK,
    output reg [2:0] bypassEN = {3'b101}
    );
    
    always @(posedge CLK) begin
        bypassEN[0] <= ~bypassEN[0];
    end 
 
    always @(negedge bypassEN[0]) begin
        bypassEN[1] <= ~bypassEN[1];
    end
 
    always @(negedge bypassEN[1]) begin
        bypassEN[2] <= ~bypassEN[2];
    end
endmodule
