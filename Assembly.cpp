// This is the code for Assembly Project 1 Summer 2020 supervised by Dr. Mohamed Shalan
// This is the work of Students: Amr Abdalla 900182041 and Mina Naguib 900184014

#include <iostream>
#include <fstream>
#include "stdlib.h"
#include <iomanip>
#include <string>

using namespace std;

unsigned char buffer[8 * 1024];	// Buffer to hold the machine code

void emitError(string s)
{
	cout << s;
	exit(0);
}

unsigned int GetBranchImm(unsigned int instWord)
{
	unsigned int D12, D10_5, D4_1, D11;
	D12 = (instWord >> 21) & 0x00080000;
	D10_5 = (instWord >> 20) & 0x000007E0;
	D4_1 = (instWord >> 8) & 0x0000000F;
	D11 = (instWord << 4) & 0x00000800;
	return D12 + D10_5 + D4_1 + D11;
}

unsigned int GetJ_Imm(unsigned int instWord)
{
	unsigned int D20, D10_1, D11, D19_12;
	D20 = (instWord >> 12) & 0x00100000;
	D10_1 = (instWord >> 21) & 0x000003FF;
	D11 = (instWord >> 10) & 0x00000400;
	D19_12 = instWord & 0x000FF000;
	return D20 + D10_1 + D11 + D19_12;
}

void instDecExec(unsigned int instWord)
{
	unsigned int rd, rs1, rs2, funct3, funct7, opcode;
	int I_imm, S_imm, B_imm, U_imm, J_imm;
	unsigned int address;
	unsigned int succ, pred;

	opcode = instWord & 0x0000007F;
	rd = (instWord >> 7) & 0x0000001F;
	funct3 = (instWord >> 12) & 0x00000007;
	rs1 = (instWord >> 15) & 0x0000001F;
	rs2 = (instWord >> 20) & 0x0000001F;
	I_imm = (instWord >> 20) & 0x00000FFF; 
	if (I_imm >= 2048)
	{
		I_imm = (I_imm xor 0xFFF) + 1;
		I_imm = - I_imm;
	}
	S_imm = (instWord >> 19) & 0x00000FE0; S_imm += rd;
	B_imm = GetBranchImm(instWord);
	U_imm = (instWord >> 11);
	J_imm = GetJ_Imm(instWord);
	succ = (instWord >> 20) & 0x00F00000;
	pred = (instWord >> 24) & 0x0F000000;

	if (opcode == 0x33) {		// R Instructions
		switch (funct3) {
		case 0: if (funct7 == 32) {
			cout << "\tSUB\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			//regs[rd] = regs[rs1] - regs[rs2];
		}
			  else {
			cout << "\tADD\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			//			regs[rd] = regs[rs1] + regs[rs2];
		}
			  break;
		case 5: if (funct7 == 32) {
			cout << "\tSRA\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			//regs[rd] = 
		}
			  else {
			cout << "\tSRL\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			//			regs[rd] = 
		}
			  break;
		case 1: 
			cout << "\tSLL\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			//regs[rd] =
			break;
		case 2:
			cout << "\tSLT\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			//regs[rd] =
			break;
		case 3:
			cout << "\tSLTU\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			//regs[rd] =
			break;
		case 4:
			cout << "\tXOR\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			//regs[rd] =
			break;
		case 6:
			cout << "\tOR\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			//regs[rd] =
			break;
		case 7:
			cout << "\tAND\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			//regs[rd] =
			break;
		default: cout << "\tUnkown R Instruction \n";
		}
	}
	else if (opcode == 0x03) // I load instructions
	{
		switch (funct3) {
		case 0: 
			cout << "\tLB\tx" << rd << ",\t" << I_imm << "(x" << rs1 << ")\n";
			//			LB rd, I_imm(rs1)
			  break;
		case 1:
			cout << "\tLH\tx" << rd << ",\t" << I_imm << "(x" << rs1 << ")\n";
			// LH rd, I_imm(rs1)
			  break;
		case 2: 
			cout << "\tLW\tx" << rd << ",\t" << I_imm << "(x" << rs1 << ")\n";
			// LW rd, I_imm(rs1)
			break;
		case 3:
			cout << "\tLBU\tx" << rd << ",\t" << I_imm << "(x" << rs1 << ")\n";
			// LBU rd, I_imm(rs1)
			break;
		case 4:
			cout << "\tLHU\tx" << rd << ",\t" << I_imm << "(x" << rs1 << ")\n";
			// LHU rd, I_imm(rs1)
			break;
		default: cout << "\tUnkown I Instruction \n";
		}
	}
	else if (opcode == 0x13) // I immediate instructions
	{
		switch (funct3) {
		case 0:
			cout << "\tADDI\tx" << rd << ", x" << rs1 << ", " << I_imm << "\n";
			// ADDI xd, x1, Imm
			break;
		case 1: 
			cout << "\tSLLI\tx" << rd << ", x" << rs1 << ", " << rs2 << "\n";
			// SLLI xd, x1, shamt
			break;
		case 2:
			cout << "\tSLTI\tx" << rd << ", x" << rs1 << ", " << I_imm << "\n";
			// SLTI xd, x1, Imm
			break;
		case 3: 
			cout << "\tSLTIU\tx" << rd << ", x" << rs1 << ", " << I_imm << "\n";
			// SLTIU xd, x1, Imm
			break;
		case 4: 
			cout << "\tXORI\tx" << rd << ", x" << rs1 << ", " << I_imm << "\n";
			// XORI xd, x1, Imm
			break;
		case 5:
			if (funct7 == 0)
			cout << "\tSRLI\tx" << rd << ", x" << rs1 << ", " << rs2 << "\n";
			// SRLI xd, x1, shamt
			else 
			cout << "\tSRAI\tx" << rd << ", x" << rs1 << ", " << rs2 << "\n";
			// SRAI xd, x1, shamt
			break;
		case 6:
			cout << "\tORI\tx" << rd << ", x" << rs1 << ", " << I_imm << "\n";
			// ORI xd, x1, Imm
			break;
		case 7:
			cout << "\tANDI\tx" << rd << ", x" << rs1 << ", " << I_imm << "\n";
			// ADDI xd, x1, Imm
			break;
		default: cout << "\tUnkown I Instruction \n";
		}
	}
	else if (opcode == 0x23) // S instructions
	{
	switch (funct3) {
	case 0:
		cout << "\tSB\tx" << rd << ",\t" << S_imm << "(x" << rs1 << ")\n";
		//			SB Xrd, imm(Xrs1)
		break;
	case 1:
		cout << "\tSH\tx" << rd << ",\t" << S_imm << "(x" << rs1 << ")\n";
		//			SH Xrd, imm(Xrs1)
		break;
	case 2:
		cout << "\tSW\tx" << rd << ",\t" << S_imm << "(x" << rs1 << ")\n";
		//			SW Xrd, imm(Xrs1)
		break;
	default: 
		cout << "\tUnkown S Instruction \n";
	}
	}
	else if (opcode == 0x63) // SB instructions
	{
	switch (funct3) {
	case 0:
		cout << "\tBEQ\tx" << rs1 << ", x" << rs2 << ", <" << B_imm << ">\n";
		//	BEQ Xrs1, Xrs2, <imm> 
		break;
	case 1:
		cout << "\tBNE\tx" << rs1 << ", x" << rs2 << ", <" << B_imm << ">\n";
		//	BNE Xrs1, Xrs2, <imm> 
		break;
	case 4:
		cout << "\tBLT\tx" << rs1 << ", x" << rs2 << ", <" << B_imm << ">\n";
		//	BLT Xrs1, Xrs2, <imm> 
		break;
	case 5:
		cout << "\tBGE\tx" << rs1 << ", x" << rs2 << ", <" << B_imm << ">\n";
		//	BGE Xrs1, Xrs2, <imm> 
		break;
	case 6:
		cout << "\tBLTU\tx" << rs1 << ", x" << rs2 << ", <" << B_imm << ">\n";
		//	BLTU Xrs1, Xrs2, <imm> 
		break;
	case 7:
		cout << "\tBGEU\tx" << rs1 << ", x" << rs2 << ", <" << B_imm << ">\n";
		//	BGEU Xrs1, Xrs2, <imm> 
		break;
	default:
		cout << "\tUnkown SB Instruction \n";
	}
	}
	else if (opcode == 0x17) // AUIPC instruction
	{
		cout << "\tAUIPC\tx" << rd << ",\t" << U_imm << "\n";
	// AUIPC xrd, imm
	}
	else if (opcode == 0x37) // LUI instruction
	{
	cout << "\tLUI\tx" << rd << ",\t<" << U_imm << ">\n";
	// LUI xrd, <imm>
	}
	else if (opcode == 0x6f) // JAL instruction
	{
	cout << "\tJAL\tx" << rd << ", <" << J_imm << ">\n";
	// JALL xrd, <imm>
	}
	else if (opcode == 0x76) //JALR instruction
	{
	cout << "\tJALR\tx" << rd << ", x" << rs1 << ",\t " << I_imm << "\n";
	// JALR Xrd, Xrs1, imm
	}
	else if (opcode == 0x73) //Environment calls and csr instructions
	{
	switch (funct3) {
	case 0:
		if (I_imm == 0)
			cout << "\tECALL\n";
		else if (I_imm == 1)
			cout << "\tEBREAK\n";
		else cout << "Unknown Environment call\n";
		break;
	case 1:
		cout << "\tCSRRW\tx" << rd << ", " << I_imm << ", x" << rs1 << "\n";
		// csrrw rd,offset,rs1
		break;
	case 2: 
		cout << "\tCSRRS\tx" << rd << ", " << I_imm << ", x" << rs1 << "\n";
		// csrrw rd,offset,rs1
		break;
	case 3:
		cout << "\tCSRRC\tx" << rd << ", " << I_imm << ", x" << rs1 << "\n";
		// csrrc rd,offset,rs1	
		break;
	case 5:
		cout << "\tCSRRWI\tx" << rd << ", " << I_imm << ", " << rs1 << "\n";
		// CSRRWI rd,offset,uimmm
		break;
	case 6:
		cout << "\tCSRRSI\tx" << rd << ", " << I_imm << ", " << rs1 << "\n";
		// CSRRSI rd,offset,uimmm
		break;
	case 7:
		cout << "\tCSRRCI\tx" << rd << ", " << I_imm << ", " << rs1 << "\n";
		// CSRRCI rd,offset,uimmm
		break;
	default: cout << "Unknown csr instruction\n";	
	} 
	}
	else if (opcode == 0xF) //fence instructions 
	{
		if (funct3 == 0)
			cout << "\tfence\t" << pred << ", " << succ << "\n";
			// fence pred, succ
		else if (funct3 == 1)
			cout << "\t fence.i\n";
			// fence.i
		else cout << "Unknown fence instruction\n";
	}
	else {
		cout << "\tUnkown Instruction \n";
	}

}

int main(int argc, char* argv[]) {

	unsigned int instWord = 0;
	ifstream inFile;
	ofstream outFile;
	unsigned int pc = 0;

	if (argc < 1) emitError("use: disasm <machine_code_file_name>\n");

	inFile.open(argv[1], ios::in | ios::binary | ios::ate);

	if (inFile.is_open())
	{
		int fsize = inFile.tellg();

		inFile.seekg(0, inFile.beg);
		if (!inFile.read((char*)buffer, fsize)) emitError("Cannot read from input file\n");

		while (true) {
			instWord = (unsigned char)buffer[pc] |
				(((unsigned char)buffer[pc + 1]) << 8) |
				(((unsigned char)buffer[pc + 2]) << 16) |
				(((unsigned char)buffer[pc + 3]) << 24);
			pc += 4;
			// remove the following line once you have a complete disassembler
			if (pc == fsize + 4 ) break;			// stop when PC reached end of file
			instDecExec(instWord);
		}


	}
	else emitError("Cannot access input file\n");
}