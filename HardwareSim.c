/* Purpose Statement	: Program implements a machine language interpreter for a specified architecture using an outline.
 * Names		: David Dalmatoff
 * File	Name	: proj1a.c
 * Class		: CSC-214-001
 * Semester/Year: Fall 2015
 * Date Started	: 10/20/15
 * Date Ended	: 11/03/15
 */
 
 
#include <stdio.h>
#include <string.h>
#include <math.h>

void decode(char *,int);
void toDecimal(char *);
void toBinary(int,int,int);
void calcOperand(char *,int);
void pickRegister(char *,int);
void memoryDump();

//Global variables
int i,in,j,jumpdest,c,r = 0; //Loop variables
int r0,r1,r2,r3 = 0;
int instrs[255][4]; //Largest number of instructions the PC can display with 8 bits
char instr[] = "0000000000000000";

int inst = 0;
int pc = 0; //The PC variable
char cchar; //Current char read from file/array
//int ac = 0; //Accumulator variable
char ir[4];
int cc = 0; //String representing the CC
int skip = 0; //Boolean to skip the next instruction or not
int halted = 0;//Boolean to stop reading in instructions. 0 to continue reading, 1 to stop 

int main(int argc, char *argv[]){
	FILE *input = fopen("proj1a.dat","r");
	if(input == NULL){//If the file doesnt exit or cant open it
		perror("Error opening the file");//Print error
		return(-1);//Return -1 instead of a positive number
	}
	else{
		while((cchar = fgetc(input)) != EOF){//while the file has unread lines
			//printf("%c",cchar);
			if(cchar == '\0'){ //If cchar is null, break out of the loop
				break;
			}
			else if(cchar != '\n'){
				instr[in] = cchar;
				in++;
			}
			if(cchar == '\n'){
				in = 0;
				decode(instr,inst);
				inst++;
			}
		}
		memoryDump();
	}
}

void decode(char *in,int n){
	if(halted != 1){
		char opcode[] = "00000";
		memcpy(opcode,in,5);
		//printf("The opcode of (%s) is %s ",in,opcode);
		if(strcmp(opcode,"00000") == 0){//Direct Load
			instrs[n][0] = 1;
			instrs[n][1] = 0;
			printf("%s = LOAD\n",opcode);
		}
		else if(strcmp(opcode,"00001") == 0){//Immediate Load
			instrs[n][0] = 1;
			instrs[n][1] = 1;
			printf("%s is LODI\n",opcode);
			//load(in,1);
		}
		else if(strcmp(opcode,"00010") == 0){ //Direct Store
			instrs[n][0] = 2;
			instrs[n][1] = 0;
			printf("%s is STO\n",opcode);
			//store(in,0);
		}
		else if(strcmp(opcode,"00011") == 0){//Immediate Store
			instrs[n][0] = 2;
			instrs[n][1] = 1;
			printf("%s is STOI\n",opcode);
			//store(in,1);
		}
		else if(strcmp(opcode,"00100") == 0||strcmp(opcode,"00101") == 0){//Add
			instrs[n][0] = 3;
			instrs[n][1] = 0;
			printf("%s is ADD\n",opcode);
			//add(in);
		}
		else if(strcmp(opcode,"00110") == 0||strcmp(opcode,"00111") == 0){//Sub
			instrs[n][0] = 4;
			instrs[n][1] = 0;
			printf("%s is SUB\n",opcode);
			//sub(in);
		}
		else if(strcmp(opcode,"01000") == 0||strcmp(opcode,"01001") == 0){//Adr
			instrs[n][0] = 5;
			instrs[n][1] = 0;
			printf("%s is ADR\n",opcode);
			//adr(in);
		}
		else if(strcmp(opcode,"01010") == 0 ||strcmp(opcode,"01011") == 0){//Sur
			instrs[n][0] = 6;
			instrs[n][1] = 0;
			printf("%s is SUR\n",opcode);
			//sur(in);
		}
		else if(strcmp(opcode,"01100") == 0){ //AND
			instrs[n][0] = 7;
			instrs[n][1] = 0;
			printf("%s is AND\n",opcode);
			//andproc(in);
		}
		else if(strcmp(opcode,"01110") == 0){ //Ior instruction
			instrs[n][0] = 8;
			instrs[n][1] = 0;
			printf("%s is IOR\n",opcode);
			//ior(in);
		}
		else if(strcmp(opcode, "10000") == 0){ //NOT instruction
			instrs[n][0] = 9;
			instrs[n][1] = 0;
			printf("%s is NOT\n",opcode);
			//notproc(in);
		}
		else if(strcmp(opcode, "10100") == 0||strcmp(opcode, "10010") == 0||strcmp(opcode, "10110") == 0||strcmp(opcode, "11000") == 0){
			instrs[n][0] = 10;
			instrs[n][1] = 0;
			printf("%s is JMP\n",opcode);
			//jumpTo(in);
		} 
		else if(strcmp(opcode, "11010") == 0){
			instrs[n][0] = 11;
			instrs[n][1] = 0;
			printf("%s is CMP\n",opcode);
			//compTo(in);
		} 
		else if(strcmp(opcode, "11100") == 0){ //Clr instruction
			instrs[n][0] = 12;
			instrs[n][1] = 0;
			printf("%s is CLR\n",opcode);
			//clrRegister(in);
		}
		else if(strcmp(opcode, "11110") == 0){ //Hlt instruction
			instrs[n][0] = 13;
			instrs[n][1] = 0;
			printf("%s is HLT\n",opcode);
			halted = 1;
		}
		if(instrs[n][0] == 10 ||(instrs[n][0] != 9 && instrs[n][0] != 11 && instrs[n][0] != 12) ){ //is Jump, Clear, Not, or Compare
			//calcOperand(in,n);
		}
		if(instrs[n][0] != 10){ //not jump operation
			pickRegister(in,n);
		}
	pc++;
	}
	else{
		instrs[n][0] = -1;
	}
}
void calcOperand(char *in,int row){
	
}
void pickRegister(char *in,int row){
	int rnum = 0;
	if(in[7] == '1'){
		rnum+= 1;
	}
	if(in[6] == '1'){
		rnum += 2;
	}
	instrs[row][2] = rnum;
}

void memoryDump(){
	for(i = 0; i < 255;i++){
		if(instrs[i][0] != 0){
			printf("(%d): ",i);
		}
		else{
			break;
		}
		for(j = 0;j <4;j++){
			if(instrs[i][0] != 0){
				printf("%d ",instrs[i][j]);
			}
		}
		printf("\n");
	}	
}
