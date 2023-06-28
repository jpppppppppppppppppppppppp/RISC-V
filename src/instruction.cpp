//
// Created by 14908 on 2023/6/24.
//
#include "ReorderBuffer.h"
#include "ReservationStation.h"
#include "instruction.h"
#include "CDB.h"
void instructionController::FetchAndPush(CDB* cdb){
	if(end)return;
	if(wait){
		if(cdb->reorderBuffer.buffer.datahead[RS1].ready){
			PC = (cdb->reorderBuffer.buffer.datahead[RS1].value + offset) & (~1);
			wait = false;
		}
		else return;
	}
	if(cdb->reorderBuffer.Full())return;
	uint32_t line = getcode(cdb->MEM.mem[PC], cdb->MEM.mem[PC + 1], cdb->MEM.mem[PC + 2], cdb->MEM.mem[PC + 3]);
	instructionNormalized ins = parser(line);
	//std::cout << PC << "->" << (int)ins.type << '\n';
	switch (ins.type) {
		case InstructionType::LUI: {
			ReorderBufferEntry entry;
			entry.ins = PC;
			entry.ready = true;
			entry.type = ReorderBufferType::RegisterWrite;
			entry.value = ins.imm1;
			entry.destination = ins.rd;
			cdb->reorderBuffer.Push(entry);
			cdb->registerFile.ADDRegisterWrite(ins.rd, entry.index);
			PC += 4;
			break;
		}
		case InstructionType::AUIPC: {
			ReorderBufferEntry entry;
			entry.ins = PC;
			entry.ready = true;
			entry.type = ReorderBufferType::RegisterWrite;
			entry.value = PC + ins.imm1;
			entry.destination = ins.rd;
			cdb->reorderBuffer.Push(entry);
			cdb->registerFile.ADDRegisterWrite(ins.rd, entry.index);
			PC += 4;
			break;
		}
		case InstructionType::JAL: {
			ReorderBufferEntry entry;
			entry.ins = PC;
			entry.ready = true;
			entry.type = ReorderBufferType::RegisterWrite;
			entry.value = PC + 4;
			entry.destination = ins.rd;
			cdb->reorderBuffer.Push(entry);
			cdb->registerFile.ADDRegisterWrite(ins.rd, entry.index);
			PC += ins.imm1;
			break;
		}
		case InstructionType::JALR: {
			ReorderBufferEntry entry;
			entry.ins = PC;
			entry.ready = true;
			entry.type = ReorderBufferType::RegisterWrite;
			entry.value = PC + 4;
			entry.destination = ins.rd;
			cdb->reorderBuffer.Push(entry);
			cdb->registerFile.ADDRegisterWrite(ins.rd, entry.index);
			if(cdb->registerFile.dirty[ins.rs1]){
				if(cdb->reorderBuffer.buffer.datahead[cdb->registerFile.RobEntry[ins.rs1]].ready){
					PC = (cdb->reorderBuffer.buffer.datahead[cdb->registerFile.RobEntry[ins.rs1]].value + ins.imm1) & (~1);
				}else{
					wait = true;
					RS1 = cdb->registerFile.RobEntry[ins.rs1];
					offset = ins.imm1;
				}
			}else{
				PC = (cdb->registerFile.reg[ins.rs1] + ins.imm1) & (~1);
			}
			break;
		}
		case InstructionType::END: {
			ReorderBufferEntry entry;
			entry.ins = PC;
			entry.type = ReorderBufferType::END;
			entry.ready = true;
			cdb->reorderBuffer.Push(entry);
			end = true;
			break;
		}
		case InstructionType::BEQ:
		case InstructionType::BNE:
        case InstructionType::BLT:
		case InstructionType::BGE:
		case InstructionType::BLTU:
		case InstructionType::BGEU: {
			ReorderBufferEntry entry;
			entry.ins = PC;
			entry.type = ReorderBufferType::Branch;
			entry.predict = cdb->predictor.GetPredict(line);
			entry.ldbindex = (line >> 7) & 0b1111111111;
			entry.ready = false;
			if(entry.predict)entry.destination = PC + 4;
			else entry.destination = PC + ins.imm1;
			ReservationStationEntry Rentry;
			Rentry.type = ins.type;
			if(cdb->registerFile.dirty[ins.rs1]){
				Rentry.Q1 = cdb->registerFile.RobEntry[ins.rs1];
				if(cdb->reorderBuffer.buffer.datahead[Rentry.Q1].ready){
					Rentry.value1 = cdb->reorderBuffer.buffer.datahead[Rentry.Q1].value;
				}
				else{
					Rentry.Q1need = true;
				}
			}
			else{
				Rentry.value1 = cdb->registerFile.reg[ins.rs1];
			}
			if(cdb->registerFile.dirty[ins.rs2]){
				Rentry.Q2 = cdb->registerFile.RobEntry[ins.rs2];
				if(cdb->reorderBuffer.buffer.datahead[Rentry.Q2].ready){
					Rentry.value2 = cdb->reorderBuffer.buffer.datahead[Rentry.Q2].value;
				}
				else{
					Rentry.Q2need = true;
				}
			}
			else{
				Rentry.value2 = cdb->registerFile.reg[ins.rs2];
			}
			if(Rentry.Q1need or Rentry.Q2need)Rentry.ready = false;
			else Rentry.ready = true;
			cdb->reorderBuffer.Push(entry);
			Rentry.RoBEntry = entry.index;
			cdb->reservationStation.Push(Rentry);
			if(entry.predict)PC += ins.imm1;
			else PC += 4;
			break;
		}
		case InstructionType::ADDI:
		case InstructionType::SLTI:
		case InstructionType::SLTIU:
		case InstructionType::XORI:
		case InstructionType::ORI:
		case InstructionType::ANDI:
		case InstructionType::SLLI:
		case InstructionType::SRLI:
		case InstructionType::SRAI: {
			ReorderBufferEntry entry;
			entry.ins = PC;
			entry.type = ReorderBufferType::RegisterWrite;
			entry.ready = false;
			entry.destination = ins.rd;
			ReservationStationEntry Rentry;
			Rentry.type = ins.type;
			Rentry.value2 = ins.imm1;
			if(cdb->registerFile.dirty[ins.rs1]){
				Rentry.Q1 = cdb->registerFile.RobEntry[ins.rs1];
				if(cdb->reorderBuffer.buffer.datahead[Rentry.Q1].ready){
					Rentry.value1 = cdb->reorderBuffer.buffer.datahead[Rentry.Q1].value;
					Rentry.ready = true;
				}
				else{
					Rentry.Q1need = true;
					Rentry.ready = false;
				}
			}
			else{
				Rentry.value1 = cdb->registerFile.reg[ins.rs1];
				Rentry.ready = true;
			}
			cdb->reorderBuffer.Push(entry);
			cdb->registerFile.ADDRegisterWrite(ins.rd, entry.index);
			Rentry.RoBEntry = entry.index;
			cdb->reservationStation.Push(Rentry);
			PC += 4;
			break;
		}
		case InstructionType::SH:
		case InstructionType::SB:
		case InstructionType::SW: {
			ReorderBufferEntry entry;
			entry.ins = PC;
			entry.type = ReorderBufferType::MemoryWrite;
			entry.ready = true;
			LoadStoreEntry lsbentry;
			lsbentry.ready = false;
			lsbentry.type = ins.type;
			lsbentry.offset = ins.imm1;
			if(cdb->registerFile.dirty[ins.rs1]){
				lsbentry.destinationRob = cdb->registerFile.RobEntry[ins.rs1];
				if(cdb->reorderBuffer.buffer.datahead[lsbentry.destinationRob].ready){
					lsbentry.destination = cdb->reorderBuffer.buffer.datahead[lsbentry.destinationRob].value + lsbentry.offset;
					lsbentry.destinationneed = false;
				}
				else{
					lsbentry.destination = ins.rs1;
					lsbentry.destinationneed = true;
				}
			}
			else{
				lsbentry.destination = cdb->registerFile.reg[ins.rs1] + lsbentry.offset;
				lsbentry.destinationneed = false;
			}
			if(cdb->registerFile.dirty[ins.rs2]){
				lsbentry.valueRoB = cdb->registerFile.RobEntry[ins.rs2];
				if(cdb->reorderBuffer.buffer.datahead[lsbentry.valueRoB].ready){
					lsbentry.value = cdb->reorderBuffer.buffer.datahead[lsbentry.valueRoB].value;
					lsbentry.valueneed = false;
				}
				else{
					lsbentry.value = ins.rs2;
					lsbentry.valueneed = true;
				}
			}
			else{
				lsbentry.value = cdb->registerFile.reg[ins.rs2];
				lsbentry.valueneed = false;
			}
			entry.ldbindex = cdb->loadStoreBuffer.buffer.tail;
			cdb->reorderBuffer.Push(entry);
			lsbentry.RoBEntry = entry.index;
			cdb->loadStoreBuffer.ADD(lsbentry);
			PC += 4;
			break;
		}
		case InstructionType::LB:
		case InstructionType::LH:
		case InstructionType::LW:
		case InstructionType::LBU:
		case InstructionType::LHU:{
			ReorderBufferEntry entry;
			entry.ins = PC;
			entry.type = ReorderBufferType::RegisterWrite;
			entry.ready = false;
			LoadStoreEntry lsbentry;
			lsbentry.ready = true;
			lsbentry.type = ins.type;
			lsbentry.offset = ins.imm1;
			entry.destination = lsbentry.destination = ins.rd;
			lsbentry.destinationneed = false;
			if(cdb->registerFile.dirty[ins.rs1]){
				lsbentry.valueRoB = cdb->registerFile.RobEntry[ins.rs1];
				if(cdb->reorderBuffer.buffer.datahead[lsbentry.valueRoB].ready){
					lsbentry.value = cdb->reorderBuffer.buffer.datahead[lsbentry.valueRoB].value + lsbentry.offset;
					lsbentry.valueneed = false;
				}
				else{
					lsbentry.value = ins.rs1;
					lsbentry.valueneed = true;
				}
			}
			else{
				lsbentry.value = cdb->registerFile.reg[ins.rs1] + lsbentry.offset;
				lsbentry.valueneed = false;
			}
			if(lsbentry.valueneed or lsbentry.destinationneed)lsbentry.ready = false;
			else lsbentry.ready = true;
			entry.ldbindex = cdb->loadStoreBuffer.buffer.tail;
			cdb->reorderBuffer.Push(entry);
			cdb->registerFile.ADDRegisterWrite(ins.rd, entry.index);
			lsbentry.RoBEntry = entry.index;
			cdb->loadStoreBuffer.ADD(lsbentry);
			PC += 4;
			break;
		}
		case InstructionType::ADD:
		case InstructionType::SUB:
		case InstructionType::SLL:
		case InstructionType::SLT:
		case InstructionType::SLTU:
		case InstructionType::XOR:
		case InstructionType::OR:
		case InstructionType::AND:{
			ReorderBufferEntry entry;
			entry.ins = PC;
			entry.type = ReorderBufferType::RegisterWrite;
			entry.ready = false;
			entry.destination = ins.rd;
			ReservationStationEntry Rentry;
			Rentry.type = ins.type;
			if(cdb->registerFile.dirty[ins.rs1]){
				Rentry.Q1 = cdb->registerFile.RobEntry[ins.rs1];
				if(cdb->reorderBuffer.buffer.datahead[Rentry.Q1].ready){
					Rentry.value1 = cdb->reorderBuffer.buffer.datahead[Rentry.Q1].value;
				}
				else{
					Rentry.Q1need = true;
				}
			}
			else{
				Rentry.value1 = cdb->registerFile.reg[ins.rs1];
			}
			if(cdb->registerFile.dirty[ins.rs2]){
				Rentry.Q2 = cdb->registerFile.RobEntry[ins.rs2];
				if(cdb->reorderBuffer.buffer.datahead[Rentry.Q2].ready){
					Rentry.value2 = cdb->reorderBuffer.buffer.datahead[Rentry.Q2].value;
				}
				else{
					Rentry.Q2need = true;
				}
			}
			else{
				Rentry.value2 = cdb->registerFile.reg[ins.rs2];
			}
			if(Rentry.Q1need or Rentry.Q2need)Rentry.ready = false;
			else Rentry.ready = true;
			cdb->reorderBuffer.Push(entry);
			cdb->registerFile.ADDRegisterWrite(ins.rd, entry.index);
			Rentry.RoBEntry = entry.index;
			cdb->reservationStation.Push(Rentry);
			PC += 4;
			break;
		}
		default:
			assert(false);
	}

}

int32_t str2int32(std::string str, int sysofnum){
	uint32_t ans = 0;
	for(int i = 0; i < str.length(); i++){
		char ch = str[i];
		if(ch >= '0' and ch <= '9')
			ans = ans * sysofnum + ch - '0';
		else if(ch >= 'A' and ch <= 'F')
			ans = ans * sysofnum + ch - 'A' + 10;
	}
	return static_cast<int32_t>(ans);
}

int8_t str2int8(std::string str, int sysofnum){
	uint8_t ans = 0;
	for(int i = 0; i < str.length(); i++){
		char ch = str[i];
		if(ch >= '0' and ch <= '9')
			ans = ans * sysofnum + ch - '0';
		else if(ch >= 'A' and ch <= 'F')
			ans = ans * sysofnum + ch - 'A' + 10;
	}
	return static_cast<int8_t>(ans);
}

uint32_t instructionController::getcode(uint8_t num1, uint8_t num2, uint8_t num3, uint8_t num4){
	uint32_t ans = num4;
	ans <<= 8;
	ans += num3;
	ans <<= 8;
	ans += num2;
	ans <<= 8;
	ans += num1;
	return ans;
}

instructionNormalized instructionController::parser(uint32_t line){
	uint8_t instype = line & 0b1111111;
	instructionNormalized ans;
	if(line == 0x0ff00513){
		ans.type = InstructionType::END;
		return ans;
	}
	switch (instype) {
		case 0b0110111 ://LUI
		{
			ans.type = InstructionType::LUI;
			line >>= 7;
			ans.rd = line & 0b11111;
			line >>= 5;
			if(line >> 19){
				line = (line - 1) ^ 0b11111111111111111111;
				ans.imm1 = line << 12;
				ans.imm1 = -ans.imm1;
			}
			else
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
				line = (line - 1) ^ 0b11111111111111111111;
				ans.imm1 = line << 12;
				ans.imm1 = -ans.imm1;
			}
			else
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
				ans.imm1 = (ans.imm1 ^ 0b111111111111111111111) + 1;
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
				case 0b000:
					ans.type = InstructionType::BEQ;
					break;
				case 0b001:
					ans.type = InstructionType::BNE;
					break;
				case 0b100:
					ans.type = InstructionType::BLT;
					break;
				case 0b101:
					ans.type = InstructionType::BGE;
					break;
				case 0b110:
					ans.type = InstructionType::BLTU;
					break;
				case 0b111:
					ans.type = InstructionType::BGEU;
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
				ans.imm1 = -ans.imm1;
			}
			break;
		}
		case 0b0000011://LB/LH/LW/LBU/LHU
		{
			line >>= 7;
			ans.rd = line & 0b11111;
			line >>= 5;
			switch (line & 0b111) {
				case 0b000:
					ans.type = InstructionType::LB;
					break;
				case 0b001:
					ans.type = InstructionType::LH;
					break;
				case 0b010:
					ans.type = InstructionType::LW;
					break;
				case 0b100:
					ans.type = InstructionType::LBU;
					break;
				case 0b101:
					ans.type = InstructionType::LHU;
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
				ans.imm1 = -ans.imm1;
			}
			break;
		}
		case 0b0100011 ://SB/SH/SW
		{
			line >>= 7;
			ans.imm1 = line & 0b11111;
			line >>= 5;
			switch (line & 0b111) {
				case 0b000:
					ans.type = InstructionType::SB;
					break;
				case 0b001:
					ans.type = InstructionType::SH;
					break;
				case 0b010:
					ans.type = InstructionType::SW;
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
				ans.imm1 = -ans.imm1;
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
				case 0b001: {
					ans.type = InstructionType::SLLI;
					ans.imm1 = line & 0b11111;
					line >>= 5;
					if(line)assert(false);
					break;
				}
				case 0b101: {
					ans.imm1 = line & 0b11111;
					line >>= 5;
					switch (line) {
						case 0:
							ans.type = InstructionType::SRLI;
							break;
						case 0b0100000:
							ans.type = InstructionType::SRAI;
							break;
						default:
							assert(false);
					}
					break;
				}
				case 0b000:
					ans.type = InstructionType::ADDI;
					break;
				case 0b010:
					ans.type = InstructionType::SLTI;
					break;
				case 0b011:
					ans.type = InstructionType::SLTIU;
					break;
				case 0b100:
					ans.type = InstructionType::XORI;
					break;
				case 0b110:
					ans.type = InstructionType::ORI;
					break;
				case 0b111:
					ans.type = InstructionType::ANDI;
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
				case 0b000: {
					switch (line) {
						case 0:
							ans.type = InstructionType::ADD;
							break;
						case 0b0100000 :
							ans.type = InstructionType::SUB;
							break;
						default:
							assert(false);
					}
					break;
				}
				case 0b001:
					ans.type = InstructionType::SLL;
					break;
				case 0b010:
					ans.type = InstructionType::SLT;
					break;
				case 0b011:
					ans.type = InstructionType::SLTU;
					break;
				case 0b100:
					ans.type = InstructionType::XOR;
					break;
				case 0b101: {
					switch (line) {
						case 0:
							ans.type = InstructionType::SRL;
							break;
						case 0b0100000:
							ans.type = InstructionType::SRA;
							break;
						default:
							assert(false);
					}
					break;
				}
				case 0b110:
					ans.type = InstructionType::OR;
					break;
				case 0b111:
					ans.type = InstructionType::AND;
					break;
				default:
					ans.type = InstructionType::END;
			}
			break;
		}
		default:
			assert(false);
	}

	return ans;
}
