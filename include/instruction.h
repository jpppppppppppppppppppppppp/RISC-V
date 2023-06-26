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
	LUI,
	AUIPC,
	JAL,
	JALR,
	BEQ,
	BNE,
	BLT,
	BGE,
	BLTU,
	BGEU,
	LB,
	LH,
	LW,
	LBU,
	LHU,
	SB,
	SH,
	SW,
	ADDI,
	SLTI,
	SLTIU,
	XORI,
	ORI,
	ANDI,
	SLLI,
	SRLI,
	SRAI,
	ADD,
	SUB,
	SLL,
	SLT,
	SLTU,
	XOR,
	SRL,
	SRA,
	OR,
	AND,
	END
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
