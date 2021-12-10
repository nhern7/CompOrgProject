/* 
Class Project: The logical conclusion (v1.1)
CSCI-2500 Fall 2021
*/

/******************************************************************************/
/* Usual suspects to include  */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define BIT type as a char (i.e., one byte)
typedef char BIT;
#define TRUE 1
#define FALSE 0
#define UNDEF -1

// useful constants
BIT ONE[32] = {TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, 
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};
BIT ZERO[32] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};
BIT REG_THIRTY_ONE[5] = {TRUE, TRUE, TRUE, TRUE, TRUE};
BIT THIRTY_TWO[32] = {FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, 
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};


/******************************************************************************/
/* Function prototypes */
/******************************************************************************/
BIT not_gate(BIT A);
BIT or_gate(BIT A, BIT B);
BIT or_gate3(BIT A, BIT B, BIT C);
BIT or_gate4(BIT A, BIT B, BIT C, BIT D);
BIT and_gate(BIT A, BIT B);
BIT and_gate3(BIT A, BIT B, BIT C);
BIT and_gate4(BIT A, BIT B, BIT C, BIT D);
BIT xor_gate(BIT A, BIT B);
BIT nor_gate(BIT A, BIT B);
BIT nand_gate(BIT A, BIT B);

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3);
BIT multiplexor2(BIT S, BIT I0, BIT I1);
void multiplexor2_32(BIT S, BIT* I0, BIT* I1, BIT* Output);
BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3);

void copy_bits(BIT* A, BIT* B);
void print_binary(BIT* A);
void print_binary_general(BIT* A, int length);
void convert_to_binary(int a, BIT* A, int length);
void convert_to_binary_char(int a, char* A, int length);
int binary_to_integer(BIT* A);
void register_map(char* register_name, BIT register_field[][5]);
void hex_to_binary_string(char* address, char address_to_write_to[32]);
void string_splice(char* source, int start_pos, int end_pos, char* destination);
void address_getter(char* address_string, char* address_reformed);

void add_convert(char* input_line, BIT Instructions[32]);
void or_convert(char* input_line, BIT Instructions[32]);
void and_convert(char* input_line, BIT Instructions[32]);
void sub_convert(char* input_line, BIT Instructions[32]);
void slt_convert(char* input_line, BIT Instructions[32]);
void lw_convert(char* input_line, BIT Instructions[32]);
void sw_convert(char* input_line, BIT Instructions[32]);
void addi_convert(char* input_line, BIT Instructions[32]);
void beq_convert(char* input_line, BIT Instructions[32]);
void j_convert(char* input_line, BIT Instructions[32]);
void jal_convert(char* input_line, BIT Instructions[32]);
void jr_convert(char* input_line, BIT Instructions[32]);

int get_instructions(BIT Instructions[][32]);

void Instruction_Memory(BIT* ReadAddress, BIT* Instruction);
void Control(BIT* OpCode,
  BIT* RegDst, BIT* Jump, BIT* Branch, BIT* MemRead, BIT* MemToReg,
  BIT* ALUOp, BIT* MemWrite, BIT* ALUSrc, BIT* RegWrite);
void Read_Register(BIT* ReadRegister1, BIT* ReadRegister2,
  BIT* ReadData1, BIT* ReadData2);
void Write_Register(BIT RegWrite, BIT* WriteRegister, BIT* WriteData);
void ALU_Control(BIT* ALUOp, BIT* funct, BIT* ALUControl);
void ALU(BIT* ALUControl, BIT* Input1, BIT* Input2, BIT* Zero, BIT* Result);
void Data_Memory(BIT MemWrite, BIT MemRead, 
  BIT* Address, BIT* WriteData, BIT* ReadData);
void Extend_Sign16(BIT* Input, BIT* Output);
void updateState();
  
/******************************************************************************/
/* Functions provided for your convenience */
/******************************************************************************/
BIT not_gate(BIT A)
{
  if (A)
    return FALSE;
  else
    return TRUE;
}

BIT or_gate(BIT A, BIT B)
{
  if (A || B)
    return TRUE;
  else
    return FALSE;
}

BIT or_gate3(BIT A, BIT B, BIT C)
{
  return or_gate(A, or_gate(B, C));
}

BIT or_gate4(BIT A, BIT B, BIT C, BIT D)
{
  return or_gate(A, or_gate3(B, C, D));
}

BIT and_gate(BIT A, BIT B)
{
  if (A && B)
    return TRUE;
  else
    return FALSE;
}

BIT and_gate3(BIT A, BIT B, BIT C)
{
  return and_gate(A, and_gate(B, C));
}

BIT and_gate4(BIT A, BIT B, BIT C, BIT D)
{
  return and_gate(A, and_gate3(B, C, D));
}

BIT xor_gate(BIT A, BIT B)
{
  if (A ^ B)
    return TRUE;
  else
    return FALSE;
}

BIT nor_gate(BIT A, BIT B)
{
  return not_gate(or_gate(A, B));
}

BIT nand_gate(BIT A, BIT B)
{
  return not_gate(and_gate(A, B));
}

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3)
{
  // Note: The input -> output mapping is slightly modified from before
  *O0 = and_gate(not_gate(I1), not_gate(I0));
  *O1 = and_gate(not_gate(I1), I0);
  *O2 = and_gate(I1, not_gate(I0));
  *O3 = and_gate(I1, I0);
  
  return;
}

BIT multiplexor2(BIT S, BIT I0, BIT I1)
{
  BIT x0 = and_gate(not_gate(S), I0);
  BIT x1 = and_gate(S, I1);
  return or_gate(x0, x1);  
}

void multiplexor2_32(BIT S, BIT* I0, BIT* I1, BIT* Output)
{
  for (int i = 0; i < 32; ++i) {
    BIT x0 = and_gate(not_gate(S), I0[i]);
    BIT x1 = and_gate(S, I1[i]);
    Output[i] = or_gate(x0, x1);
  }
}

BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3)
{
  BIT x0, x1, x2, x3 = FALSE;
  decoder2(S0, S1, &x0, &x1, &x2, &x3);
  
  BIT y0 = and_gate(x0, I0);
  BIT y1 = and_gate(x1, I1);
  BIT y2 = and_gate(x2, I2);
  BIT y3 = and_gate(x3, I3);
  
  BIT z0 = or_gate(y0, y1);
  BIT z1 = or_gate(y2, y3);
  
  return or_gate(z0, z1);  
}

void multiplexor8(BIT S0, BIT S1, BIT S2, BIT I0, BIT I1, BIT I2,
                  BIT I3, BIT I4, BIT I5, BIT I6, BIT I7)
{
  return multiplexor2(S0,multiplexor4(S1,S2,I0,I1,I2,I3),multiplexor4(S2,S1,I4,I5,I6,I7));
}


/******************************************************************************/
/* Helper functions */
/******************************************************************************/
void copy_bits(BIT* A, BIT* B)
{
  /*copy the bit sequence of A unto B*/
  for (int i = 0; i < 32; ++i)
    B[i] = A[i]; 
}

void print_binary(BIT* A) //expecting length of 32
{
  for (int i = 31; i >= 0; --i)
    printf("%d", A[i]); 
}

void print_binary_general(BIT* A, int length) //expecting any length, should be 1 less than the actual
{
  for (int i = length; i >= 0; --i)
    printf("%d", A[i]); 
}

void convert_to_binary(int a, BIT* A, int length)
{
  /* convert INTEGER to BIT REPRESENTATION of the given length (in 2's complement) */
  if (a >= 0) {
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 1 ? TRUE : FALSE);
      a /= 2;
    }
  } 
  else {
    a += 1;
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 0 ? TRUE : FALSE);
      a /= 2;
    }
  }
}

void convert_to_binary_char(int a, char* A, int length)
{
  /* convert INTEGER to BINARY STRING REPRESENTATION of the given length (in 2's complement)*/
  if (a >= 0) {
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 1 ? '1' : '0');
      a /= 2;
    }
  } 
  else {
    a += 1;
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 0 ? '1' : '0');
      a /= 2;
    }
  }
}
  
int binary_to_integer(BIT* A)
{
  unsigned a = 0;
  unsigned mult = 1;
  
  for (int i = 0; i < 32; ++i) {
    a += A[i]*mult;
    mult *= 2;
  }
  
  return (int)a;
}


/******************************************************************************/
/* Parsing functions */
/******************************************************************************/
void register_map(char* register_name, BIT register_field[][5]){
  if (!strcmp(register_name, "t0")){
    convert_to_binary(8, *register_field, 5); 
  }
  else if (!strcmp(register_name, "s0")){
    convert_to_binary(16, *register_field, 5); 
  }
  else if (!strcmp(register_name, "zero")){
    convert_to_binary(0, *register_field, 5); 
  }  
  else if (!strcmp(register_name, "v0")){
    convert_to_binary(2, *register_field, 5); 
  }  
  else if (!strcmp(register_name, "a0")){
    convert_to_binary(4, *register_field, 5); 
  }  
  else if (!strcmp(register_name, "t1")){
    convert_to_binary(9, *register_field, 5); 
  }  
  else if (!strcmp(register_name, "s1")){
    convert_to_binary(17, *register_field, 5); 
  }  
  else if (!strcmp(register_name, "sp")){
    convert_to_binary(29, *register_field, 5); 
  }  
  else if (!strcmp(register_name, "ra")){
    convert_to_binary(31, *register_field, 5); 
  }  
}

void hex_to_binary_string(char* address, char address_to_write_to[32]){
  int i;
  int j = 31;
  for (i = 0; i < strlen(address); i++){
    if (address[i] == '0'){
      address_to_write_to[j] = '0';
      address_to_write_to[j-1] = '0';
      address_to_write_to[j-2] = '0';
      address_to_write_to[j-3] = '0';
      j -= 4;
    }
    else if (address[i] == '1'){
      address_to_write_to[j] = '0';
      address_to_write_to[j-1] = '0';
      address_to_write_to[j-2] = '0';
      address_to_write_to[j-3] = '1';
      j -= 4;
    }
    else if (address[i] == '1'){
      address_to_write_to[j] = '0';
      address_to_write_to[j-1] = '0';
      address_to_write_to[j-2] = '0';
      address_to_write_to[j-3] = '1';
      j -= 4;
    }    
    else if (address[i] == '2'){
      address_to_write_to[j] = '0';
      address_to_write_to[j-1] = '0';
      address_to_write_to[j-2] = '1';
      address_to_write_to[j-3] = '0';
      j -= 4;
    }    
    else if (address[i] == '3'){
      address_to_write_to[j] = '0';
      address_to_write_to[j-1] = '0';
      address_to_write_to[j-2] = '1';
      address_to_write_to[j-3] = '1';
      j -= 4;
    }    
    else if (address[i] == '4'){
      address_to_write_to[j] = '0';
      address_to_write_to[j-1] = '1';
      address_to_write_to[j-2] = '0';
      address_to_write_to[j-3] = '0';
      j -= 4;
    }    
    else if (address[i] == '5'){
      address_to_write_to[j] = '0';
      address_to_write_to[j-1] = '1';
      address_to_write_to[j-2] = '0';
      address_to_write_to[j-3] = '1';
      j -= 4;
    }    
    else if (address[i] == '6'){
      address_to_write_to[j] = '0';
      address_to_write_to[j-1] = '1';
      address_to_write_to[j-2] = '1';
      address_to_write_to[j-3] = '0';
      j -= 4;
    }   
    else if (address[i] == '7'){
      address_to_write_to[j] = '0';
      address_to_write_to[j-1] = '1';
      address_to_write_to[j-2] = '1';
      address_to_write_to[j-3] = '1';
      j -= 4;
    }     
    else if (address[i] == '8'){
      address_to_write_to[j] = '1';
      address_to_write_to[j-1] = '0';
      address_to_write_to[j-2] = '0';
      address_to_write_to[j-3] = '0';
      j -= 4;
    }   
    else if (address[i] == '9'){
      address_to_write_to[j] = '1';
      address_to_write_to[j-1] = '0';
      address_to_write_to[j-2] = '0';
      address_to_write_to[j-3] = '1';
      j -= 4;
    }     
    else if (address[i] == 'a'){
      address_to_write_to[j] = '1';
      address_to_write_to[j-1] = '0';
      address_to_write_to[j-2] = '1';
      address_to_write_to[j-3] = '0';
      j -= 4;
    }    
    else if (address[i] == 'b'){
      address_to_write_to[j] = '1';
      address_to_write_to[j-1] = '0';
      address_to_write_to[j-2] = '1';
      address_to_write_to[j-3] = '1';
      j -= 4;
    }       
    else if (address[i] == 'c'){
      address_to_write_to[j] = '1';
      address_to_write_to[j-1] = '1';
      address_to_write_to[j-2] = '0';
      address_to_write_to[j-3] = '0';
      j -= 4;
    }
    else if (address[i] == 'd'){
      address_to_write_to[j] = '1';
      address_to_write_to[j-1] = '1';
      address_to_write_to[j-2] = '0';
      address_to_write_to[j-3] = '1';
      j -= 4;
    }  
    else if (address[i] == 'e'){
      address_to_write_to[j] = '1';
      address_to_write_to[j-1] = '1';
      address_to_write_to[j-2] = '1';
      address_to_write_to[j-3] = '0';
      j -= 4;
    }         
    else if (address[i] == 'f'){
      address_to_write_to[j] = '1';
      address_to_write_to[j-1] = '1';
      address_to_write_to[j-2] = '1';
      address_to_write_to[j-3] = '1';
      j -= 4;
    }                 
  }
}

void string_splice(char* source, int start_pos, int end_pos, char* destination){
  int i;
  int j;
  for (j = 0, i = start_pos; i < end_pos; i++, j++){
    destination[j] = source[i];
  }
}

void address_getter(char* address_string, char* address_reformed){
  char address_bin[33] = {0};
  hex_to_binary_string(address_string, address_bin);  //convert address_string from hex (string) to binary string
  string_splice(address_bin, 2, 28, address_reformed);  //extract the string from index 28 to 2
}

void add_convert(char* input_line, BIT Instructions[32]){
  char reg1[3];  
  char reg2[3];
  char reg3[3];
  char instruction[5]; 
  BIT opcode[6] = {0};
  BIT funct[5] = {0};  
  BIT shamt[5] = {0};  
  BIT rd[5] = {0};  
  BIT rs[5] = {0};  
  BIT rt[5] = {0}; 
  int i; 
  int j;
  sscanf(input_line, "%s %s %s %s", instruction, reg1, reg2, reg3);
  
  convert_to_binary(0, opcode, 6);  //opcode
  for (j = 26, i = 0; j < 32; j++,i++){
    Instructions[j] = opcode[i];
  }

  convert_to_binary(32, funct, 6);  //funct
  for (j = 0, i = 0; j < 6; j++,i++){
    Instructions[j] = funct[i];
  }     

  convert_to_binary(0, shamt, 5);   //shamt
  for (j = 6, i = 0; j < 11; j++,i++){
    Instructions[i] = shamt[i];
  } 

  //reg1 is essentially rd, reg2 is essentially rs, reg3 is essentially rt
  register_map(reg1, &rd);
  for (j = 11, i = 0; j < 16; j++,i++){
    Instructions[j] = rd[i];
  }

  register_map(reg2, &rs);
  for (j = 21, i = 0; j < 26; j++,i++){
    Instructions[j] = rs[i];
  }   

  register_map(reg3, &rt);
  for (j = 16, i = 0; j < 21; j++,i++){
    Instructions[j] = rt[i];
  }  
}

void or_convert(char* input_line, BIT Instructions[32]){
  char reg1[3];  
  char reg2[3];
  char reg3[3];
  char instruction[5]; 
  BIT opcode[6] = {0};
  BIT funct[5] = {0};  
  BIT shamt[5] = {0};  
  BIT rd[5] = {0};  
  BIT rs[5] = {0};  
  BIT rt[5] = {0}; 
  int i; 
  int j;
  sscanf(input_line, "%s %s %s %s", instruction, reg1, reg2, reg3);
  
  convert_to_binary(0, opcode, 6);  //opcode
  for (j = 26, i = 0; j < 32; j++,i++){
    Instructions[j] = opcode[i];
  }

  convert_to_binary(37, funct, 6);  //funct
  for (j = 0, i = 0; j < 6; j++,i++){
    Instructions[j] = funct[i];
  }     

  convert_to_binary(0, shamt, 5);   //shamt
  for (j = 6, i = 0; j < 11; j++,i++){
    Instructions[i] = shamt[i];
  } 

  //reg1 is essentially rd, reg2 is essentially rs, reg3 is essentially rt
  register_map(reg1, &rd);
  for (j = 11, i = 0; j < 16; j++,i++){
    Instructions[j] = rd[i];
  }

  register_map(reg2, &rs);
  for (j = 21, i = 0; j < 26; j++,i++){
    Instructions[j] = rs[i];
  }   

  register_map(reg3, &rt);
  for (j = 16, i = 0; j < 21; j++,i++){
    Instructions[j] = rt[i];
  }  
}

void and_convert(char* input_line, BIT Instructions[32]){
  char reg1[3];  
  char reg2[3];
  char reg3[3];
  char instruction[5]; 
  BIT opcode[6] = {0};
  BIT funct[5] = {0};  
  BIT shamt[5] = {0};  
  BIT rd[5] = {0};  
  BIT rs[5] = {0};  
  BIT rt[5] = {0}; 
  int i; 
  int j;
  sscanf(input_line, "%s %s %s %s", instruction, reg1, reg2, reg3);
  
  convert_to_binary(0, opcode, 6);  //opcode
  for (j = 26, i = 0; j < 32; j++,i++){
    Instructions[j] = opcode[i];
  }

  convert_to_binary(36, funct, 6);  //funct
  for (j = 0, i = 0; j < 6; j++,i++){
    Instructions[j] = funct[i];
  }     

  convert_to_binary(0, shamt, 5);   //shamt
  for (j = 6, i = 0; j < 11; j++,i++){
    Instructions[i] = shamt[i];
  } 

  //reg1 is essentially rd, reg2 is essentially rs, reg3 is essentially rt
  register_map(reg1, &rd);
  for (j = 11, i = 0; j < 16; j++,i++){
    Instructions[j] = rd[i];
  }

  register_map(reg2, &rs);
  for (j = 21, i = 0; j < 26; j++,i++){
    Instructions[j] = rs[i];
  }   

  register_map(reg3, &rt);
  for (j = 16, i = 0; j < 21; j++,i++){
    Instructions[j] = rt[i];
  }  
}

void sub_convert(char* input_line, BIT Instructions[32]){
  char reg1[3];  
  char reg2[3];
  char reg3[3];
  char instruction[5]; 
  BIT opcode[6] = {0};
  BIT funct[5] = {0};  
  BIT shamt[5] = {0};  
  BIT rd[5] = {0};  
  BIT rs[5] = {0};  
  BIT rt[5] = {0}; 
  int i; 
  int j;
  sscanf(input_line, "%s %s %s %s", instruction, reg1, reg2, reg3);
  
  convert_to_binary(0, opcode, 6);  //opcode
  for (j = 26, i = 0; j < 32; j++,i++){
    Instructions[j] = opcode[i];
  }

  convert_to_binary(34, funct, 6);  //funct
  for (j = 0, i = 0; j < 6; j++,i++){
    Instructions[j] = funct[i];
  }     

  convert_to_binary(0, shamt, 5);   //shamt
  for (j = 6, i = 0; j < 11; j++,i++){
    Instructions[i] = shamt[i];
  } 

  //reg1 is essentially rd, reg2 is essentially rs, reg3 is essentially rt
  register_map(reg1, &rd);
  for (j = 11, i = 0; j < 16; j++,i++){
    Instructions[j] = rd[i];
  }

  register_map(reg2, &rs);
  for (j = 21, i = 0; j < 26; j++,i++){
    Instructions[j] = rs[i];
  }   

  register_map(reg3, &rt);
  for (j = 16, i = 0; j < 21; j++,i++){
    Instructions[j] = rt[i];
  }  
}

void slt_convert(char* input_line, BIT Instructions[32]){
  char reg1[3];  
  char reg2[3];
  char reg3[3];
  char instruction[5]; 
  BIT opcode[6] = {0};
  BIT funct[5] = {0};  
  BIT shamt[5] = {0};  
  BIT rd[5] = {0};  
  BIT rs[5] = {0};  
  BIT rt[5] = {0}; 
  int i; 
  int j;
  sscanf(input_line, "%s %s %s %s", instruction, reg1, reg2, reg3);
  
  convert_to_binary(0, opcode, 6);  //opcode
  for (j = 26, i = 0; j < 32; j++,i++){
    Instructions[j] = opcode[i];
  }

  convert_to_binary(42, funct, 6);  //funct
  for (j = 0, i = 0; j < 6; j++,i++){
    Instructions[j] = funct[i];
  }       
  convert_to_binary(0, shamt, 5);   //shamt
  for (j = 6, i = 0; j < 11; j++,i++){
    Instructions[j] = shamt[i];  
  } 

  //reg1 is essentially rd, reg2 is essentially rs, reg3 is essentially rt
  register_map(reg1, &rd);
  for (j = 11, i = 0; j < 16; j++,i++){
    Instructions[j] = rd[i];
  }

  register_map(reg2, &rs);
  for (j = 21, i = 0; j < 26; j++,i++){
    Instructions[j] = rs[i];
  }   

  register_map(reg3, &rt);
  for (j = 16, i = 0; j < 21; j++,i++){
    Instructions[j] = rt[i];
  }  
}

void lw_convert(char* input_line, BIT Instructions[32]){
  char reg1[3];  
  char reg2[3];
  char instruction[5]; 
  BIT opcode[6] = {0};
  BIT rs[5] = {0};  
  BIT rt[5] = {0}; 
  BIT immediate[16] = {0}; //aka offset
  int immediate_int = 0;
  int i; 
  int j;
  sscanf(input_line, "%s %s %s %d", instruction, reg1, reg2, &immediate_int);
  
  convert_to_binary(35, opcode, 6);  //opcode
  for (j = 26, i = 0; j < 32; j++,i++){
    Instructions[j] = opcode[i];
  }  

  convert_to_binary(immediate_int, immediate, 16);  //immediate
  for (j = 0, i = 0; j < 16; j++,i++){
    Instructions[j] = immediate[i];
  }    

  //reg1 is essentially rs, reg2 is essentially rt
  register_map(reg1, &rs);
  for (j = 21, i = 0; j < 26; j++,i++){
    Instructions[j] = rs[i];
  }   

  register_map(reg2, &rt);
  for (j = 16, i = 0; j < 21; j++,i++){
    Instructions[j] = rt[i];
  }  
}

void sw_convert(char* input_line, BIT Instructions[32]){
  char reg1[3];  
  char reg2[3];
  char instruction[5]; 
  BIT opcode[6] = {0};
  BIT rs[5] = {0};  
  BIT rt[5] = {0}; 
  BIT immediate[16] = {0}; //aka offset
  int immediate_int = 0;
  int i; 
  int j;
  sscanf(input_line, "%s %s %s %d", instruction, reg1, reg2, &immediate_int);
  
  convert_to_binary(43, opcode, 6);  //opcode
  for (j = 26, i = 0; j < 32; j++,i++){
    Instructions[j] = opcode[i];
  }  

  convert_to_binary(immediate_int, immediate, 16);  //immediate
  for (j = 0, i = 0; j < 16; j++,i++){
    Instructions[j] = immediate[i];
  }    

  //reg1 is essentially rs, reg2 is essentially rt
  register_map(reg1, &rs);
  for (j = 21, i = 0; j < 26; j++,i++){
    Instructions[j] = rs[i];
  }   

  register_map(reg2, &rt);
  for (j = 16, i = 0; j < 21; j++,i++){
    Instructions[j] = rt[i];
  }  
}

void addi_convert(char* input_line, BIT Instructions[32]){
  char reg1[3];  
  char reg2[3];
  char instruction[5]; 
  BIT opcode[6] = {0};
  BIT rs[5] = {0};  
  BIT rt[5] = {0}; 
  BIT immediate[16] = {0}; //aka offset
  int immediate_int = 0;
  int i; 
  int j;
  sscanf(input_line, "%s %s %s %d", instruction, reg1, reg2, &immediate_int);
  
  convert_to_binary(8, opcode, 6);  //opcode
  for (j = 26, i = 0; j < 32; j++,i++){
    Instructions[j] = opcode[i];
  }  

  convert_to_binary(immediate_int, immediate, 16);  //immediate
  for (j = 0, i = 0; j < 16; j++,i++){
    Instructions[j] = immediate[i];
  }    

  //reg1 is essentially rt, reg2 is essentially rs
  register_map(reg2, &rs);
  for (j = 21, i = 0; j < 26; j++,i++){
    Instructions[j] = rs[i];
  }   

  register_map(reg1, &rt);
  for (j = 16, i = 0; j < 21; j++,i++){
    Instructions[j] = rt[i];
  }  
}

void beq_convert(char* input_line, BIT Instructions[32]){
  char reg1[3];  
  char reg2[3];
  char instruction[5]; 
  BIT opcode[6] = {0};
  BIT rs[5] = {0};  
  BIT rt[5] = {0}; 
  BIT immediate[16] = {0}; //aka offset
  int immediate_int = 0;
  int i; 
  int j;
  sscanf(input_line, "%s %s %s %d", instruction, reg1, reg2, &immediate_int);
  
  convert_to_binary(4, opcode, 6);  //opcode
  for (j = 26, i = 0; j < 32; j++,i++){
    Instructions[j] = opcode[i];
  }  

  convert_to_binary(immediate_int, immediate, 16);  //immediate
  for (j = 0, i = 0; j < 16; j++,i++){
    Instructions[j] = immediate[i];
  }    

  //reg1 is essentially rs, reg2 is essentially rt
  register_map(reg1, &rs);
  for (j = 21, i = 0; j < 26; j++,i++){
    Instructions[j] = rs[i];
  }   

  register_map(reg2, &rt);
  for (j = 16, i = 0; j < 21; j++,i++){
    Instructions[j] = rt[i];
  }  
}

void j_convert(char* input_line, BIT Instructions[32]){
  char instruction[5];   
  char address_string[27];
  char address_bin_spliced[26] = {'\0'};
  BIT opcode[6] = {0};
  BIT address[26] = {0}; 
  int i; 
  int j;
  sscanf(input_line, "%s %s", instruction, address_string);
  
  convert_to_binary(2, opcode, 6);  //opcode
  for (j = 26, i = 0; j < 32; j++,i++){
    Instructions[j] = opcode[i];
  }  

  address_getter(address_string, address_bin_spliced);  //address
  for (i = 0; i < 26; i++){
    if (address_bin_spliced[i] == '0'){
      address[i] = FALSE;
    }
    else if (address_bin_spliced[i] == '1'){
      address[i] = TRUE;  
    }
  }
  
  for (j = 0, i = 0; j < 26; j++,i++){ 
    Instructions[j] = address[i];
  }      
}

void jal_convert(char* input_line, BIT Instructions[32]){
  char instruction[5];   
  char address_string[27];
  char address_bin_spliced[26] = {'\0'};
  BIT opcode[6] = {0};
  BIT address[26] = {0}; 
  int i; 
  int j;
  sscanf(input_line, "%s %s", instruction, address_string);
  
  convert_to_binary(3, opcode, 6);  //opcode
  for (j = 26, i = 0; j < 32; j++,i++){
    Instructions[j] = opcode[i];
  }  

  address_getter(address_string, address_bin_spliced);  //address
  for (i = 0; i < 26; i++){
    if (address_bin_spliced[i] == '0'){
      address[i] = FALSE;
    }
    else if (address_bin_spliced[i] == '1'){
      address[i] = TRUE;  
    }
  }
  
  for (j = 0, i = 0; j < 26; j++,i++){ 
    Instructions[j] = address[i];
  }      
}

void jr_convert(char* input_line, BIT Instructions[32]){
  char reg1[3];  
  char instruction[5]; 
  BIT opcode[6] = {0};
  BIT funct[5] = {0};  
  BIT rs[5] = {0};   
  int i; 
  int j;
  sscanf(input_line, "%s %s", instruction, reg1);
  
  convert_to_binary(0, opcode, 6);  //opcode
  for (j = 26, i = 0; j < 32; j++,i++){
    Instructions[j] = opcode[i];
  }

  convert_to_binary(8, funct, 6);  //funct
  for (j = 0, i = 0; j < 6; j++,i++){
    Instructions[j] = funct[i];
  }       

  for (j = 6, i = 0; j < 11; j++,i++){
    Instructions[j] = FALSE;  
  } 

  //reg1 is essentially rs
  register_map(reg1, &rs);
  for (j = 21, i = 0; j < 26; j++,i++){
    Instructions[j] = rs[i];
  }   

  for (j = 11, i = 0; j < 16; j++,i++){
    Instructions[j] = FALSE;
  }

  for (j = 16, i = 0; j < 21; j++,i++){
    Instructions[j] = FALSE;
  }  
    
}

int get_instructions(BIT Instructions[][32])
{
  char line[256] = {0};
  int instruction_count = 0;
  char instruction[5]; 

  while (fgets(line, 256, stdin) != NULL) {     
    sscanf(line, "%s", instruction);

    if (!strcmp(instruction, "add") ){
      add_convert(line, Instructions[instruction_count]);
      instruction_count++;  
    }
    else if (!strcmp(instruction, "or") ){
      or_convert(line, Instructions[instruction_count]);   
      instruction_count++;      
    }
    else if (!strcmp(instruction, "and") ){
      and_convert(line, Instructions[instruction_count]);   
      instruction_count++;     
    }   
    else if (!strcmp(instruction, "sub") ){
      sub_convert(line, Instructions[instruction_count]);   
      instruction_count++;      
    }   
    else if (!strcmp(instruction, "slt") ){
      slt_convert(line, Instructions[instruction_count]);            
      instruction_count++;     
    }      
    else if (!strcmp(instruction, "lw") ){
      lw_convert(line, Instructions[instruction_count]);
      instruction_count++;
    }
    else if (!strcmp(instruction, "sw") ){
      sw_convert(line, Instructions[instruction_count]);      
      instruction_count++;
    }    
    else if (!strcmp(instruction, "addi") ){
      addi_convert(line, Instructions[instruction_count]);   
      instruction_count++;
    }        
    else if (!strcmp(instruction, "beq") ){
      beq_convert(line, Instructions[instruction_count]);       
      instruction_count++;
    }
    else if (!strcmp(instruction, "j") ){
      j_convert(line, Instructions[instruction_count]);
      instruction_count++;
    }        
    else if (!strcmp(instruction, "jal") ){
      jal_convert(line, Instructions[instruction_count]);
      instruction_count++;
    } 
    else if (!strcmp(instruction, "jr") ){
      jr_convert(line, Instructions[instruction_count]);
      instruction_count++;
    }        
    // TODO: perform conversion of instructions to binary
    // Input: coming from stdin via: ./a.out < input.txt
    // Output: Convert instructions to binary in the instruction memory
    // Return: Total number of instructions
    // Note: you are free to use if-else and external libraries here
    // Note: you don't need to implement circuits for saving to inst. mem.
    // My approach:
    // - Use sscanf on line to get strings for instruction and registers
    // - Use instructions to determine op code, funct, and shamt fields
    // - Convert immediate field and jump address field to binary
    // - Use registers to get rt, rd, rs fields
    // Note: I parse everything as strings, then convert to BIT array at end
  }
  
  return instruction_count;
}


/******************************************************************************/
/* Program state - memory spaces, PC, and control */
/******************************************************************************/
BIT PC[32]                  = {FALSE};
BIT MEM_Instruction[32][32] = {FALSE};
BIT MEM_Data[32][32]        = {FALSE};
BIT MEM_Register[32][32]    = {FALSE};

BIT RegDst    = FALSE;
BIT Jump      = FALSE;
BIT Branch    = FALSE;
BIT MemRead   = FALSE;
BIT MemToReg  = FALSE;
BIT ALUOp[2]  = {FALSE};
BIT MemWrite  = FALSE;
BIT ALUSrc    = FALSE;
BIT RegWrite  = FALSE;
BIT Zero      = FALSE;
BIT ALUControl[4] = {FALSE};

void print_instruction()
{
  unsigned pc = binary_to_integer(PC);
  printf("PC: %d\n", pc);
  printf("Instruction: ");
  print_binary(MEM_Instruction[pc]);
  printf("\n");
}

void print_state()
{
  printf("Data: ");
  for (int i = 0; i < 32; ++i) {
    printf("%d ", binary_to_integer(MEM_Data[i]));
  } 
  printf("\n");  
  
  printf("Register: ");
  for (int i = 0; i < 32; ++i) {
    printf("%d ", binary_to_integer(MEM_Register[i]));
  } 
  printf("\n");
}


/******************************************************************************/
/* Functions that you will implement */
/******************************************************************************/
void Instruction_Memory(BIT* ReadAddress, BIT* Instruction)
{
  // TODO: Implement instruction memory
  // Input: 32-bit instruction address
  // Output: 32-bit binary instruction
  // Note: Useful to use a 5-to-32 decoder here
  
}

void Control(BIT* OpCode,
  BIT* RegDst, BIT* Jump, BIT* Branch, BIT* MemRead, BIT* MemToReg,
  BIT* ALUOp, BIT* MemWrite, BIT* ALUSrc, BIT* RegWrite)
{
  //RegDst... = 0 for lw, 1 for R format, dont care for antyhing else
  //POS
  BIT lw_or_gate1 = or_gate3( not_gate(OpCode[1]), not_gate(OpCode[0]), OpCode[2] );
  BIT lw_or_gate2 = or_gate3( OpCode[3], OpCode[4], not_gate(OpCode[5]) );
  BIT lw_or_gate_final = or_gate(lw_or_gate1,lw_or_gate2);  
  *RegDst = lw_or_gate_final; 

  //Branch... = 1 for branch instruction, 0 otherwise
  BIT beq_and_gate1 = and_gate3( not_gate(OpCode[0]), not_gate(OpCode[1]), OpCode[2] );
  BIT beq_and_gate2 = and_gate3( not_gate(OpCode[3]), not_gate(OpCode[4]), not_gate(OpCode[5]) );
  BIT beq_and_gate_final = and_gate(beq_and_gate1,beq_and_gate2);  
  *Branch = beq_and_gate_final; 

  //ALUOp... ALUOp[0] = 1 for addi and beq, 0 otherwise. ALUOp[1] = 1 for R format and addi
  BIT addi_and_gate1 = and_gate3( not_gate(OpCode[0]), not_gate(OpCode[1]), not_gate(OpCode[2]) );
  BIT addi_and_gate2 = and_gate3( not_gate(OpCode[5]), not_gate(OpCode[4]), OpCode[3] );
  BIT addi_and_gate_final = and_gate(addi_and_gate1, addi_and_gate2);

  BIT addi_beq_or_gate = or_gate(beq_and_gate_final, addi_and_gate_final);
  ALUOp[0] = addi_beq_or_gate;  

  BIT rformat_and_gate1 = and_gate3( not_gate(OpCode[0]), not_gate(OpCode[1]), not_gate(OpCode[2]) );
  BIT rformat_and_gate2 = and_gate3( not_gate(OpCode[5]), not_gate(OpCode[4]), not_gate(OpCode[3]) );
  BIT rformat_and_gate_final = and_gate(rformat_and_gate1, rformat_and_gate2);

  BIT rformat_addi_or_gate = or_gate(rformat_and_gate_final, addi_and_gate_final);
  ALUOp[1] = rformat_addi_or_gate; 

  //Jump... = 1 for j type (including jr), 0 otherwise
  //SOP
  BIT j_and_gate1 = and_gate3( not_gate(OpCode[0]), OpCode[1], not_gate(OpCode[2]) );
  BIT j_and_gate2 = and_gate3( not_gate(OpCode[3]), not_gate(OpCode[4]), not_gate(OpCode[5]) );
  BIT j_and_gate_final = and_gate(j_and_gate1,j_and_gate2);

  BIT jal_and_gate1 = and_gate3( OpCode[0], OpCode[1], not_gate(OpCode[2]) );
  BIT jal_and_gate2 = and_gate3( not_gate(OpCode[3]), not_gate(OpCode[4]), OpCode[5] );
  BIT jal_and_final = and_gate(jal_and_gate1,jal_and_gate2);

  BIT jr_and_gate1 = and_gate3( not_gate(OpCode[0]),not_gate(OpCode[1]),not_gate(OpCode[2]) );
  BIT jr_and_gate2 = and_gate3( not_gate(OpCode[3]),not_gate(OpCode[4]),not_gate(OpCode[5]) );
  BIT jr_and_gate_final = and_gate(jr_and_gate1,jr_and_gate2);

  *Jump = or_gate3( j_and_gate_final, jal_and_final, jr_and_gate_final );  
  //problem being, all r format ones get jump = 1 but this is bad. if they all got 0, jal would be messed up
  //possible solution, make them get 0. implement a new control signal in update_datapath or somewhere
  //cant be here(dont think you can alter the signature) but somewhere where the pc stuff really comes into play

  //MemRead... = 1 for lw, 0 otherwise
  BIT lw_and_gate1 = and_gate3( OpCode[1], OpCode[0], not_gate(OpCode[2]) );
  BIT lw_and_gate2 = and_gate3( not_gate(OpCode[3]), not_gate(OpCode[4]), OpCode[5] );
  BIT lw_and_gate_final = and_gate(lw_and_gate1,lw_and_gate2);  
  *MemRead = lw_and_gate_final;

  //MemWrite... = 1 for sw, 0 otherwise
  BIT sw_and_gate1 = and_gate3( OpCode[1], OpCode[0], not_gate(OpCode[2]) );
  BIT sw_and_gate2 = and_gate3( OpCode[3], not_gate(OpCode[4]), OpCode[5] );
  BIT sw_and_gate_final = and_gate(sw_and_gate1,sw_and_gate2); 
  *MemWrite = sw_and_gate_final;

  //MemToReg... = 1 for lw, 0 otherwise
  *MemToReg = lw_and_gate_final;

  //RegWrite... = 1 for load and R format and addi, 0 otherwise
  BIT lw_addi_rformat_final = or_gate3(lw_and_gate_final, addi_and_gate_final, rformat_and_gate_final);
  *RegWrite = lw_addi_rformat_final;

  //ALUSrc... = mux that returns 1 for I format and R format, 0 otherwise
  BIT j_or_gate1 = or_gate3( OpCode[0], not_gate(OpCode[1]), OpCode[2] );
  BIT j_or_gate2 = or_gate3( OpCode[3], OpCode[4], OpCode[5] );
  BIT J_or_gate_final = or_gate(j_or_gate1,j_or_gate2);

  BIT jal_or_gate1 = or_gate3( not_gate(OpCode[0]), not_gate(OpCode[1]), OpCode[2] );
  BIT jal_or_gate2 = or_gate3( OpCode[3], OpCode[4], OpCode[5] );
  BIT jal_or_gate_final = or_gate(jal_or_gate1,jal_or_gate2);
  
  *ALUSrc = and_gate(J_or_gate_final, jal_or_gate_final);

}

void Read_Register(BIT* ReadRegister1, BIT* ReadRegister2,
  BIT* ReadData1, BIT* ReadData2)
{
  // TODO: Implement register read functionality
  // Input: two 5-bit register addresses
  // Output: the values of the specified registers in ReadData1 and ReadData2
  // Note: Implementation will be very similar to instruction memory circuit
  
}

void Write_Register(BIT RegWrite, BIT* WriteRegister, BIT* WriteData)
{
  // TODO: Implement register write functionality
  // Input: one 5-bit register address, data to write, and control bit
  // Output: None, but will modify register file
  // Note: Implementation will again be similar to those above
  
}

void ALU_Control(BIT* ALUOp, BIT* funct, BIT* ALUControl)
{
  ALUControl[3] = FALSE;
  
  //= 1 for beq, funct (funct0 = 0, funct1 = 1, funct2 = 0, funct3 = 0), funct (funct0 = 0, funct1 = 1, funct2 = 0, funct3 = 1)
  BIT beq_and_gate_final = and_gate( not_gate(ALUOp[1]), ALUOp[0] );
  
  BIT sub_and_gate1 = and_gate3( not_gate(funct[0]), funct[1], not_gate(funct[2]) );
  BIT sub_and_gate2 = and_gate3( not_gate(funct[3]), funct[5], not_gate(funct[4]) );
  BIT sub_and_gate_final = and_gate(sub_and_gate1,sub_and_gate2);

  BIT slt_and_gate1 = and_gate3( not_gate(funct[0]), funct[1], not_gate(funct[2]) );
  BIT slt_and_gate2 = and_gate3( not_gate(funct[4]), funct[3], funct[5] );
  BIT slt_and_gate_final = and_gate(slt_and_gate1,slt_and_gate2);

  ALUControl[2] = or_gate3(beq_and_gate_final, sub_and_gate_final, slt_and_gate_final);
  
  //= 0 for and, or, 1 otherwise
  BIT and_or_gate1 = or_gate3( funct[0], funct[1], not_gate(funct[2]) );
  BIT and_or_gate2 = or_gate3( funct[3], funct[4], not_gate(funct[5]) );
  BIT and_or_gate_final = or_gate(and_or_gate1,and_or_gate2);

  BIT or_or_gate1 = or_gate3( not_gate(funct[2]), funct[1], not_gate(funct[0]) );
  BIT or_or_gate2 = and_or_gate2;
  BIT or_or_gate_final = or_gate(or_or_gate1,or_or_gate2);
  
  ALUControl[1] = and_gate(and_or_gate_final,or_or_gate_final);

  //= 1 for slt, or, 0 otherwise
  BIT or_and_gate1 = and_gate3( not_gate(funct[0]), funct[1], not_gate(funct[2]) );
  BIT or_and_gate2 = and_gate3( not_gate(funct[3]), not_gate(funct[4]), funct[5] );
  BIT or_and_gate_final = and_gate(or_and_gate1,or_and_gate2);
  ALUControl[0] = or_gate(slt_and_gate_final,or_and_gate_final);
}

void adder1(BIT A, BIT B, BIT CarryIn, BIT* CarryOut, BIT* Sum)
{
  // TODO: implement a 1-bit adder
  BIT x0 = xor_gate(A, B);
  *Sum = xor_gate(CarryIn, x0);
  
  BIT y0 = and_gate(x0, CarryIn);
  BIT y1 = and_gate(A, B);
  *CarryOut = or_gate(y0, y1);
}

void ALU1(BIT A, BIT B, BIT LSB1, BIT LSB2, BIT Less,
BIT MSB1, BIT MSB2, BIT * Result, BIT * CarryOut, BIT * Set)
{
  BIT Binvert = xor_gate(MSB1,MSB2);
  BIT x0 = multiplexor2(Binvert, B, not_gate(B));

  BIT y0 = and_gate(A,x0);
  BIT y1 = or_gate(A,x0);
  
  BIT y2 = FALSE;
  adder1(A, x0, CarryIn, CarryOut, &y2);
  *Set = y2;
  BIT y3 = Less;

  *Result = multiplexor4(LSB1,LSB2,y0,y1,y2,y3);
}
void ALU(BIT* ALUControl, BIT* Input1, BIT* Input2, BIT* Zero, BIT* Result)
{   
  // TODO: Implement 32-bit ALU
  // Input: 4-bit ALUControl, two 32-bit inputs
  // Output: 32-bit result, and zero flag big
  // Note: Can re-use prior implementations (but need new circuitry for zero)
  BIT Less = FALSE;
  BIT Set = FALSE;
  BIT CarryIn;
  BIT CarryOut;
  ALU1(Input1[0], Input2[0], ALUControl[3], ALUControl[2], Less, 
    ALUControl[1],ALUControl[0], &Result[0],CarryOut,&Set);
  for (int i = 1; i < 32; i++){
    ALU1(Input1[i],Input2[i],ALUControl[3],ALUControl[2],Less,
    ALUControl[1],ALUControl[0],&Result[i],CarryOut,&Set);
  }
  Less = Set;
  ALU1(Input1[0], Input2[0], ALUControl[3],ALUControl[2],Less, 
    ALUControl[1],ALUControl[0],&Result[0],CarryOut, &Set);  
}


void Data_Memory(BIT MemWrite, BIT MemRead, 
  BIT* Address, BIT* WriteData, BIT* ReadData)
{
  // TODO: Implement data memory
  // Input: 32-bit address, control flags for read/write, and data to write
  // Output: data read if processing a lw instruction
  // Note: Implementation similar as above
  
}

void Extend_Sign16(BIT* Input, BIT* Output)
{
  // TODO: Implement 16-bit to 32-bit sign extender
  // Copy Input to Output, then extend 16th Input bit to 17-32 bits in Output
  for (int i = 0; i < 16; i++){
    Output[i] = TRUE;
  }
  int j = 0;
  for (int i = 16; i < 32; i++){
    Output[i] = Input[j];
    j++;
  }
}

void updateState()
{
  /*note from working on part4: define a new local signal called JumpReg (determined by funct). 
  "You just need a multiplexor that determines whether the new value of PC is taken from ReadData1 
  (if it is a jr instruction) or something else (PC+4, target address of j, etc.)
  So, for this purpose, yes, you would need a bit like the JumpReg that you mentioned to control this multiplexor."
  */

  // TODO: Implement the full datapath here
  // Essentially, you'll be figuring out the order in which to process each of 
  // the sub-circuits comprising the entire processor circuit. It makes it 
  // easier to consider the pipelined version of the process, and handle things
  // in order of the pipeline. The stages and operations are:
  // Fetch - load instruction from instruction memory
  // Decode - set control bits and read from the register file
  // Execute - process ALU
  // Memory - read/write data memory
  // Write Back - write to the register file
  // Update PC - determine the final PC value for the next instruction
  Instruction_Memory(MEM_Instruction)
  
}


/******************************************************************************/
/* Main */
/******************************************************************************/

int main()
{
  setbuf(stdout, NULL);
    
  // parse instructions into binary format
  int counter = get_instructions(MEM_Instruction);

  /*
  // load program and run
  copy_bits(ZERO, PC);
  copy_bits(THIRTY_TWO, MEM_Register[29]);
  
  while (binary_to_integer(PC) < counter) {
    print_instruction();
    updateState();
    print_state();
  }
  */

  return 0;
}

