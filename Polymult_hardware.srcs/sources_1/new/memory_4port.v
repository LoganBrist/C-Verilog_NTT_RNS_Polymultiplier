`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/31/2020 07:32:49 AM
// Design Name: 
// Module Name: memory
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

// This has two reads and two write per cycle. Needs to read and write two for the butterfly
module memory_4port
  #(
     parameter integer BIT_WIDTH = 4, //r g b
     parameter integer N_COEFF = 64,
     parameter INITIALIZATION_FILENAME = "",
     parameter integer ADDR_WIDTH = 6
     )
   (
    input  wire [ADDR_WIDTH-1:0] readAddr,
    input  wire [ADDR_WIDTH-1:0] writeAddr,
    input  wire [BIT_WIDTH-1:0]  writeData,
    input  wire                  writeEn,
    output reg [BIT_WIDTH-1:0]  readData = 0,
    
    input  wire          CLK,
    
    input  wire [ADDR_WIDTH-1:0] readAddr2,
    input  wire [ADDR_WIDTH-1:0] writeAddr2,
    input  wire [BIT_WIDTH-1:0]  writeData2,
    input  wire                  writeEn2,
    output reg [BIT_WIDTH-1:0]  readData2 = 0
    );
   reg [BIT_WIDTH-1:0]         mem[N_COEFF-1:0];

   //load given filename
   initial begin 
    if(INITIALIZATION_FILENAME != "")
        $readmemh(INITIALIZATION_FILENAME, mem);
    for (integer i = 0; i < N_COEFF; i = i + 1) begin
        $display(mem[i]);
    end
   end
    
   always @(*) begin 
    readData  = mem[readAddr];
    readData2 = mem[readAddr2];
   end
   
   always @(posedge CLK) begin
     if( writeEn )
       mem[writeAddr]  <= writeData;
     if(writeEn2)
       mem[writeAddr2] <= writeData2;   
    end
        

  /* Parity based memory (not working)    
  
 //even and odd inputs to parity memories 
   reg [ADDR_WIDTH-1:0] even_readAddr,  odd_readAddr;
   reg [ADDR_WIDTH-1:0] even_writeAddr, odd_writeAddr;
   reg [BIT_WIDTH-1:0]  even_writeData, odd_writeData;
   reg                  even_writeEn,   odd_writeEn;
   wire [BIT_WIDTH-1:0]  even_readData,  odd_readData;

 //parity check to choose memory   
    reg readAddrParityIsEven;   
    always @(*) begin
        readAddrParityIsEven = ^readAddr;  //(~^ is reduction XOR)
        
        if (readAddrParityIsEven == 0) begin
            even_readAddr  = readAddr;// / 2;
            even_writeAddr = writeAddr;// / 2;
            even_writeData = writeData; 
            even_writeEn   = writeEn;
            readData       = even_readData;
            
            odd_readAddr   = (readAddr2);// - 1) / 2;
            odd_writeAddr  = (writeAddr2);// - 1) / 2;
            odd_writeData  = writeData2;
            odd_writeEn    = writeEn2;
            readData2      = odd_readData;
            
        end 
        
        else begin 
            odd_readAddr  = (readAddr);// - 1) / 2 ;
            odd_writeAddr = (writeAddr);// - 1) / 2;
            odd_writeData = writeData; 
            odd_writeEn   = writeEn;
            readData      = odd_readData;
            
            even_readAddr   = readAddr2; // / 2;
            even_writeAddr  = writeAddr2; // / 2;
            even_writeData  = writeData2;
            even_writeEn    = writeEn2;
            readData2       = even_readData;
        end
    
    end
 
   //Initialize memory
   reg [BIT_WIDTH-1:0]         mem_even[N_COEFF/2 -1:0];  
   reg [BIT_WIDTH-1:0]         mem_odd[N_COEFF/2 -1:0];
   
   integer even_idx = 0;
   integer odd_idx  = 0;
   integer file; 
   integer i;
   
   
   initial begin 
    if(INITIALIZATION_FILENAME != "") begin
        file = $fopen(INITIALIZATION_FILENAME, "r");
        if (file)  $display("File was opened successfully : %0d", file);
        else       $display("File was NOT opened successfully : %0d", file);
        
        for (i = 0; i < N_COEFF; i = i + 1) begin
            if(^i == 0) begin
                mem_even[i] = i;
                //mem_even[even_idx] = i;//$fscanf(file,"%d",i);
                //even_idx = even_idx + 1;
                $display("Even");
                $display(i);
            end
            
            if(^i == 1) begin
                mem_odd[i] = i;
                //mem_odd[odd_idx] = i; //$fscanf(file,"%d",i + 1);
                //odd_idx = odd_idx + 1;
                $display("odd");
                $display(i);
            end            

        end 
        $fclose(file);
    end 
   end
   
   // Operate memory   
   assign even_readData  = mem_even[even_readAddr];
   assign odd_readData   = mem_odd[odd_readAddr];
   
   always @(posedge CLK) begin
     if(even_writeEn)
       mem_even[even_writeAddr]  <= even_writeData; 
     if(odd_writeEn)
       mem_odd[odd_writeAddr]    <= odd_writeData; 
   end
  */ 
        
endmodule
