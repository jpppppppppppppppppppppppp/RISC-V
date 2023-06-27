//
// Created by 14908 on 2023/6/24.
//

#ifndef MAIN_CPP_INSTRUCTION_H
#define MAIN_CPP_INSTRUCTION_H
#include <cstdint>
#include <cassert>
#include <string>
class CDB;
int32_t str2int32(std::string str, int sysofnum = 16);
int8_t str2int8(std::string str, int sysofnum = 16);

enum class InstructionType{
	LUI,//0
	AUIPC,//1
	JAL,//2
	JALR,//3
	BEQ,//4
	BNE,//5
	BLT,//6
	BGE,//7
	BLTU,//8
	BGEU,//9
	LB,//10
	LH,//11
	LW,//12
	LBU,//13
	LHU,//14
	SB,//15
	SH,//16
	SW,//17
	ADDI,//18
	SLTI,//19
	SLTIU,//20
	XORI,//21
	ORI,//22
	ANDI,//23
	SLLI,//24
	SRLI,//25
	SRAI,//26
	ADD,//27
	SUB,//28
	SLL,//29
	SLT,//30
	SLTU,//31
	XOR,//32
	SRL,//33
	SRA,//34
	OR,//35
	AND,//36
	END//37
};
struct instructionNormalized{
	InstructionType type = InstructionType::END;
	uint8_t rd = 0;
	uint8_t rs1 = 0;
	uint8_t rs2 = 0;
	int32_t imm1 = 0;
};
class instructionController{
public:
	bool wait = false;
	uint8_t RS1 = 0;
	int32_t offset = 0;
	bool end = false;
	int32_t PC = 0;
	instructionController(){PC = 0;}
	void FetchAndPush(CDB* cdb);
	uint32_t getcode(uint8_t num1, uint8_t num2, uint8_t num3, uint8_t num4);
	instructionNormalized parser(uint32_t line);
};


#endif //MAIN_CPP_INSTRUCTION_H
