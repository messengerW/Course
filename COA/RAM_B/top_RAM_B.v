`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    23:15:31 05/04/2018 
// Design Name: 
// Module Name:    top_RAM_B 
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
module top_RAM_B(Mem_Addr,C,Mem_Write,Clk,LED);
input [7:2]Mem_Addr;//���ص�3��4��5��6��7��8λ
input Mem_Write,Clk;//clkΪ����C9����дʹ���ź�Ϊ����C4��0Ϊ����1Ϊд
input [1:0]C;//ѡ��д������+������ʱѡ����ʾ�ֽ�,Ϊ����1��2λ
output reg [7:0]LED;
wire [31:0]M_R_Data;//���ڴ洢�����32λ��������
reg [31:0]M_W_Data;//�Ĵ����ݴ�����32λд������
RAM_B test_ram (
  .clka(Clk), //����ʱ���ź�
  .wea(Mem_Write), //�����д�ź�
  .addra(Mem_Addr[7:2]), //����5λ��ַ�ź�
  .dina(M_W_Data), //д��32λ����
  .douta(M_R_Data) //����32λ����
);
always@(*)
begin
  LED=0;//��ʼ��
  M_W_Data=0;//��ʼ��
  if(!Mem_Write)//������
  begin
   case(C)
	 2'b00:LED=M_R_Data[7:0];//�������ݵ�0-7λ
	 2'b01:LED=M_R_Data[15:8];//�������ݵ�8-15λ
	 2'b10:LED=M_R_Data[23:16];//�������ݵ�16-23λ
	 2'b11:LED=M_R_Data[31:24];//�������ݵ�24-31λ
	 endcase
	 end
	 else
	 begin
	  case(C)
	   2'b00:M_W_Data=32'h0055_7523;//д���һ������
		2'b01:M_W_Data=32'h1234_5678;//д��ڶ�������
		2'b10:M_W_Data=32'h8765_4321;//д�����������
		2'b11:M_W_Data=32'hffff_ffff;//д����ĸ�����
		endcase
     end
	end
endmodule
