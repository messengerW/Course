`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    10:42:05 05/01/2018 
// Design Name: 
// Module Name:    Top_Register_file 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module Top_Register_file(Addr,Write_Reg,C1,C2,Clk,Reset,LED);
input [4:0]Addr;//��д��ַ������ĩ��λ
input [1:0]C1;//C1ѡ��32λ��������İ�λ�ֽڣ�����23λ
input Write_Reg,C2,Clk,Reset;//C2ѡ���A/B�˿ڵ����ݣ�����C4��0��A��1��B�� ���ص�һλΪ��д�źţ�1Ϊд��clkΪ����C9
output reg [7:0]LED;
wire [31:0]R_Data_A,R_Data_B;
reg [31:0]W_Data;
reg[4:0] A,B;
Register_file reg1(A,B,Addr,Write_Reg,W_Data,Clk,Reset,R_Data_A,R_Data_B);
always@(Addr or Write_Reg or C1 or C2 or R_Data_A or R_Data_B)
begin
 A=0;
 B=0;
 LED=0;
 W_Data=0;
 if(!Write_Reg)//������Write_Reg=0
 begin
  if(!C2)
  begin
  A=Addr;
   case(C1)
    2'b00:LED=R_Data_A[7:0];
	 2'b01:LED=R_Data_A[15:8];
	 2'b10:LED=R_Data_A[23:16];
	 2'b11:LED=R_Data_A[31:24];
	endcase
  end
else
 begin
 B=Addr;
	case(C1)
	 2'b00:LED=R_Data_B[7:0];
	 2'b01:LED=R_Data_B[15:8];
	 2'b10:LED=R_Data_B[23:16];
	 2'b11:LED=R_Data_B[31:24];
	endcase
	end
 end
 else//д����
	 begin
	 case(C1)
	 2'b00:W_Data=32'h0000_0003;
	 2'b01:W_Data=32'h0000_0607;
	 2'b10:W_Data=32'hFFFF_FFFF;
	 2'b11:W_Data=32'h1111_1234;
	endcase
  end
  end
endmodule

