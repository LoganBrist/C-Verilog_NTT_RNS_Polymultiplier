`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/04/2020 06:08:48 PM
// Design Name: 
// Module Name: delay_tb
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


module delay_tb(

    );
    
    parameter N_CYCLES  = 3;
    parameter BIT_WIDTH = 8;
    reg CLK = 1;
    reg [BIT_WIDTH-1:0] in = 0;
    wire [BIT_WIDTH-1:0] out;
    
    delay #(N_CYCLES,BIT_WIDTH) uut (
    .CLK(CLK),
    .in(in),
    .out(out)
    );    
    
    initial begin
    for (integer i = 0; i < 200; i = i+1) begin
        #1
        CLK <= ~CLK;
        #1
        CLK <= ~CLK;
        in <= in + 1;
    end
end
    
endmodule
