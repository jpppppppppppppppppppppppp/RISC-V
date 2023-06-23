//
// Created by 14908 on 2023/6/21.
//
#ifndef MAIN_CPP_PAR_CPP
#define MAIN_CPP_PAR_CPP
#include <cassert>
#include <cstdint>
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
	uint32_t uimm1 = 0;
};

instructionNormalized parser(uint32_t line){
	uint8_t instype = line & 0b1111111;
	instructionNormalized ans;
	switch (instype) {
		case 0b0110111 ://LUI
		{
			ans.type = InstructionType::LUI;
			line >>= 7;
			ans.rd = line & 0b11111;
			line >>= 5;
			if(line >> 19){
				line = (line-1)^0b11111111111111111111;
				ans.imm1 = line << 12;
				ans.imm1 = -ans.imm1;
			}else
				ans.imm1 = line << 12;
			break;
		}
		case 0b0010111 ://AUIPC
		{
			ans.type = InstructionType::AUIPC;
			line >>= 7;
			ans.rd = line & 0b11111;
			line >>= 5;
			if(line >> 19){
				line = (line-1) ^ 0b11111111111111111111;
				ans.imm1 = line << 12;
				ans.imm1 = -ans.imm1;
			}else
				ans.imm1 = line << 12;
			break;
		}
		case 0b1101111 ://JAL
		{
			ans.type = InstructionType::JAL;
			line >>= 7;
			ans.rd = line & 0b11111;
			line >>= 5;
			ans.imm1 = (line & 0b11111111) << 12;
			line >>= 8;
			ans.imm1 += (line & 0b1) << 11;
			line >>= 1;
			ans.imm1 += (line & 0b1111111111) << 1;
			line >>= 10;
			ans.imm1 += line << 20;
			if(line){
				ans.imm1 = (ans.imm1^0b111111111111111111111)+1;
				ans.imm1 = -ans.imm1;
			}
			break;
		}
		case 0b1100111://JALR
		{
			ans.type = InstructionType::JALR;
			line >>= 7;
			ans.rd = line & 0b11111;
			line >>= 8;
			ans.rs1 = line & 0b11111;
			line >>= 5;
			ans.imm1 = line;
			if(line >> 11){
				ans.imm1 = (ans.imm1 ^ 0b111111111111) + 1;
				ans.imm1 = -ans.imm1;
			}
			break;
		}
		case 0b1100011://BEQ/BNE/BLT/GBE/BLTU/BGEU
		{
			line >>= 7;
			ans.imm1 = (line & 0b1) << 11;
			line >>= 1;
			ans.imm1 += (line & 0b1111) << 1;
			line >>= 4;
			switch (line & 0b111) {
				case 0b000:ans.type = InstructionType::BEQ;
					break;
				case 0b001:ans.type = InstructionType::BNE;
					break;
				case 0b100:ans.type = InstructionType::BLT;
					break;
				case 0b101:ans.type = InstructionType::BGE;
					break;
				case 0b110:ans.type = InstructionType::BLTU;
					break;
				case 0b111:ans.type = InstructionType::BGEU;
					break;
				default:
					assert(false);
			}
			line >>= 3;
			ans.rs1 = line & 0b11111;
			line >>= 5;
			ans.rs2 = line & 0b11111;
			line >>= 5;
			ans.imm1 += (line & 0b111111) << 5;
			line >>= 6;
			ans.imm1 += line << 12;
			if(line){
				ans.imm1 = (ans.imm1 ^ 0b1111111111111) + 1;
				ans.imm1 = - ans.imm1;
			}
			break;
		}
		case 0b0000011://LB/LH/LW/LBU/LHU
		{
			line >>= 7;
			ans.rd = line & 0b11111;
			line >>= 5;
			switch (line & 0b111) {
				case 0b000:ans.type = InstructionType::LB;
					break;
				case 0b001:ans.type = InstructionType::LH;
					break;
				case 0b010:ans.type = InstructionType::LW;
					break;
				case 0b100:ans.type = InstructionType::LBU;
					break;
				case 0b101:ans.type = InstructionType::LHU;
					break;
				default:
					assert(false);
			}
			line >>= 3;
			ans.rs1 = line & 0b11111;
			line >>= 5;
			ans.imm1 = line;
			if(line >> 11){
				ans.imm1 = (ans.imm1 ^ 0b111111111111) + 1;
				ans.imm1 = - ans.imm1;
			}
			break;
		}
		case 0b0100011 ://SB/SH/SW
		{
			line >>= 7;
			ans.imm1 = line & 0b11111;
			line >>= 5;
			switch (line & 0b111) {
				case 0b000:ans.type = InstructionType::SB;
					break;
				case 0b001:ans.type = InstructionType::SH;
					break;
				case 0b010:ans.type = InstructionType::SW;
					break;
				default:
					assert(false);
			}
			line >>= 3;
			ans.rs1 = line & 0b11111;
			line >>= 5;
			ans.rs2 = line & 0b11111;
			line >>= 5;
			ans.imm1 += line << 5;
			if(line >> 6){
				ans.imm1 = (ans.imm1 ^ 0b111111111111) + 1;
				ans.imm1 = - ans.imm1;
			}
			break;
		}
		case 0b0010011 ://ADDI/SLTI/SLTIU/XORI/ORI/ANDI/SLLI/SRLI/SRAI
		{
			line >>= 7;
			ans.rd = line & 0b11111;
			line >>= 5;
			int opt = line & 0b111;
			line >>= 3;
			ans.rs1 = line & 0b11111;
			line >>= 5;
			switch (opt) {
				case 0b001:
				{
					ans.type = InstructionType::SLLI;
					ans.imm1 = line & 0b11111;
					line >>= 5;
					if(line)assert(false);
					break;
				}
				case 0b101:
				{
					ans.imm1 = line & 0b11111;
					line >>= 5;
					switch (line) {
						case 0:ans.type = InstructionType::SRLI;
							break;
						case 0b0100000:ans.type = InstructionType::SRAI;
							break;
						default:
							assert(false);
					}
					break;
				}
				case 0b000:ans.type = InstructionType::ADDI;
					break;
				case 0b010:ans.type = InstructionType::SLTI;
					break;
				case 0b011:ans.type = InstructionType::SLTIU;
					break;
				case 0b100:ans.type = InstructionType::XORI;
					break;
				case 0b110:ans.type = InstructionType::ORI;
					break;
				case 0b111:ans.type = InstructionType::ANDI;
					break;
				default:
					assert(false);
			}
			if(opt != 0b001 and opt != 0b101){
				ans.imm1 = line;
				if(line >> 11){
					ans.imm1 = (ans.imm1 ^ 0b111111111111) + 1;
					ans.imm1 = -ans.imm1;
				}
			}
			break;
		}
		case 0b0110011 ://ADD/SUB/SLL/SLT/SLTU/XOR/SRL/SRA/OR/AND
		{
			line >>= 7;
			ans.rd = line & 0b11111;
			line >>= 5;
			int opt = line & 0b111;
			line >>= 3;
			ans.rs1 = line & 0b11111;
			line >>= 5;
			ans.rs2 = line & 0b11111;
			line >>= 5;
			switch (opt) {
				case 0b000:
				{
					switch (line) {
						case 0:ans.type = InstructionType::ADD;
							break;
						case 0b0100000 :ans.type = InstructionType::SUB;
							break;
						default:
							assert(false);
					}
					break;
				}
				case 0b001:ans.type = InstructionType::SLL;
					break;
				case 0b010:ans.type = InstructionType::SLT;
					break;
				case 0b011:ans.type = InstructionType::SLTU;
					break;
				case 0b100:ans.type = InstructionType::XOR;
					break;
				case 0b101:
				{
					switch (line) {
						case 0:ans.type = InstructionType::SRL;
							break;
						case 0b0100000:ans.type = InstructionType::SRA;
							break;
						default:
							assert(false);
					}
					break;
				}
				case 0b110:ans.type = InstructionType::OR;
					break;
				case 0b111:ans.type = InstructionType::AND;
					break;
				default:
					assert(false);
			}
			break;
		}
		default:
			assert(false);
	}
	if(line == 0x0ff00513)ans.type = InstructionType::END;
	return ans;
}
#endif //MAIN_CPP_PAR_CPP