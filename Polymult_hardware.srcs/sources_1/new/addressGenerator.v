`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/28/2020 05:52:29 PM
// Design Name: 
// Module Name: addressGenerator
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



module addressGenerator 
    #(  parameter integer N_COEFF    = 8,
        parameter integer ADDR_WIDTH = $clog2(N_COEFF),
        parameter integer BUTTERFLY_DELAY = 3)
    ( input wire CLK,
      output reg [ADDR_WIDTH-1:0] readIndex0,
      output reg [ADDR_WIDTH-1:0] readIndex1,
      output reg [ADDR_WIDTH-1:0] writeIndex0,
      output reg [ADDR_WIDTH-1:0] writeIndex1,
      output wire writeEn0,
      output wire writeEn1,
      output reg [ADDR_WIDTH-2:0] phi_index 
     );
    

    //Loop registers and increments
    reg [ADDR_WIDTH:0] idx              = 0;        //not -1               //loops through indexes (inner)
    reg [ADDR_WIDTH-1:0] block          = 0;                       //loops through blocks (middle)
    reg [ADDR_WIDTH-1:0] stage          = 0;                       //loops through stages (outer)
    reg [ADDR_WIDTH:0] n_idx            = 2;      //not -1                 //number of indexes in a block. Multiply by 2 each loop. (inner)
    reg [ADDR_WIDTH-1:0] n_blocks       = N_COEFF / 2;    //number of blocks in a stage. Divides by 2 each loop    (middle)
    reg [ADDR_WIDTH-1:0] n_stages       = ADDR_WIDTH;  //number of stages in the NTT. (outer)
    wire [ADDR_WIDTH-1:0] idx_next, block_next, stage_next, halfstep;
    wire [ADDR_WIDTH-2:0] phi_index_wire;
    wire last_idx, last_block, last_stage;
    
    
    //Delay for the butterfly output write address
    //delay #(BUTTERFLY_DELAY,ADDR_WIDTH) d1(CLK,readIndex0, writeIndex0);
    //delay #(BUTTERFLY_DELAY,ADDR_WIDTH) d2(CLK,readIndex1, writeIndex1);
    always @(*) begin
        writeIndex0 = readIndex0;
        writeIndex1 = readIndex1;
    end 
    
    assign halfstep = n_idx / 2 ;
    
    //writeEn if write indexes are defined and not finished with cycle
    wire writeEnable, writeEnableDelay;
    assign writeEnable = !((writeIndex0 == (N_COEFF/2 - 1))
                        && (writeIndex1 == (N_COEFF - 1)));
    assign writeEn0 = writeEnable;// || writeEnableDelay;
    assign writeEn1 = writeEnable;// || writeEnableDelay;
    //delay #(1,1) d3(CLK, writeEnable, writeEnableDelay);
           
    //index increments
    assign idx_next   = idx + 1;
    assign block_next = block + 1;
    assign stage_next = stage + 1;
    
    //End of loop flags
    assign last_idx   = (idx_next == halfstep);
    assign last_block = (block_next == n_blocks);
    assign last_stage = (stage_next == n_stages);  
 
    //phi index is a function of n_blocks and idx
    assign phi_index_wire = n_blocks * idx;
    
    always @(posedge CLK) begin
    readIndex0 <= idx + n_idx * block;             //index  
    readIndex1 <= idx + n_idx * block + halfstep; //index + half step  
    
    casez ({last_idx,last_block,last_stage}) 
        //increment idx
        3'b0ZZ: begin
        idx <= idx_next; block <= block; stage <= stage; n_idx <= n_idx; 
        n_blocks <= n_blocks; phi_index <= phi_index_wire;
        end
        
        //increment block
        3'b10Z: begin
        idx <= 0; block <= block_next; stage <= stage; n_idx <= n_idx; 
        n_blocks <= n_blocks; phi_index <= phi_index_wire;
        end
        
        //increment stage. Change n_blocks and n_idx 
        3'b110: begin
        idx <= 0; block <= 0; stage <= stage_next; n_idx <= n_idx * 2; 
        n_blocks <= n_blocks / 2; phi_index <= phi_index_wire;
        end
        
        //idle
        'b111:   begin
        idx   <= idx; block <= block; stage <= stage; n_idx <= n_idx;
         n_blocks <= n_blocks; phi_index <= phi_index_wire;
        end
              
        //default: increment index
        default: begin
        //increment idx
        idx <= idx_next; block <= block; stage <= stage; n_idx <= n_idx;
         n_blocks <= n_blocks; phi_index <= phi_index_wire;
        end        
    endcase
    
    end 
              
endmodule
