//
// Created by 14908 on 2023/6/24.
//
#include "ReservationStation.h"
#include "CDB.h"
void ReservationStation::Flush(){
	for(int i = 0; i < size; ++i){
		buffer[i] = tempbuffer[i];
	}
	for(int i = 0; i < alusize; ++i){
		alu[i].Flush();
	}
}

void ReservationStation::Push(ReservationStationEntry& entry){
	for(int i = 0; i < size; ++i){
		if(!buffer[i].used){
			entry.index = i;
			tempbuffer[i] = entry;
			tempbuffer[i].used = true;
			return;
		}
	}
}

void ReservationStation::EXE(CDB* cdb){
	for(int i = 0; i < alusize; ++i){
		if(alu[i].over){
			ReservationStationEntry tar = cdb->reservationStation.tempbuffer[alu[i].RSEntry];
			cdb->reorderBuffer.tempbuffer.datahead[tar.RoBEntry].value = alu[i].ans;
			cdb->reorderBuffer.tempbuffer.datahead[tar.RoBEntry].ready = true;
			alu[i].tempworking = false;
			alu[i].tempover = false;
			cdb->reservationStation.tempbuffer[alu[i].RSEntry].used = false;

		}else if(alu[i].working){
			alu[i].tempworking = false;
			alu[i].tempover = true;
		}
	}
	for(int i = 0; i < size; ++i){
		if(buffer[i].used and !buffer[i].exe and buffer[i].ready){
			switch (buffer[i].type) {
				case InstructionType::SUB:
				case InstructionType::ADDI:
				case InstructionType::ADD:
				{
					if(!alu[0].working and !alu[0].tempworking){
						alu[0].tempans = getans(buffer[i].value1, buffer[i].value2, buffer[i].type);
						alu[0].tempworking = true;
						alu[0].tempover = false;
						alu[0].tempRSEntry = i;
						tempbuffer[i].exe = true;
					}
					break;
				}
				case InstructionType::BEQ:
				case InstructionType::BNE:
				case InstructionType::BLT:
				case InstructionType::BGE:
				case InstructionType::BLTU:
				case InstructionType::BGEU:
				case InstructionType::SLT:
				case InstructionType::SLTU:
				case InstructionType::SLTI:
				case InstructionType::SLTIU:{
					if(!alu[1].working and !alu[1].tempworking){
						alu[1].tempans = getans(buffer[i].value1, buffer[i].value2, buffer[i].type);
						alu[1].tempworking = true;
						alu[1].tempover = false;
						alu[1].tempRSEntry = i;
						tempbuffer[i].exe = true;
					}
					break;
				}
				case InstructionType::XOR:
				case InstructionType::XORI:
				case InstructionType::OR:
				case InstructionType::ORI:
				case InstructionType::AND:
				case InstructionType::ANDI:
				{
					if(!alu[2].working and !alu[2].tempworking){
						alu[2].tempans = getans(buffer[i].value1, buffer[i].value2, buffer[i].type);
						alu[2].tempworking = true;
						alu[2].tempover = false;
						alu[2].tempRSEntry = i;
						tempbuffer[i].exe = true;
					}
					break;
				}
				case InstructionType::SLL:
				case InstructionType::SLLI:
				case InstructionType::SRL:
				case InstructionType::SRLI:
				case InstructionType::SRA:
				case InstructionType::SRAI:
				{
					if(!alu[3].working and !alu[3].tempworking){
						alu[3].tempans = getans(buffer[i].value1, buffer[i].value2, buffer[i].type);
						alu[3].tempworking = true;
						alu[3].tempover = false;
						alu[3].tempRSEntry = i;
						tempbuffer[i].exe = true;
					}
					break;
				}
				default:
					assert(false);
			}
		}
	}
	for(int i = 0; i < size; ++i){
		if(buffer[i].used and !buffer[i].ready){
			if(buffer[i].Q1need){
				if(cdb->reorderBuffer.tempbuffer.datahead[buffer[i].Q1].ready){
					tempbuffer[i].Q1need = false;
					tempbuffer[i].value1 = cdb->reorderBuffer.tempbuffer.datahead[buffer[i].Q1].value;
				}
			}
			if(buffer[i].Q2need){
				if(cdb->reorderBuffer.tempbuffer.datahead[buffer[i].Q2].ready){
					tempbuffer[i].Q2need = false;
					tempbuffer[i].value2 = cdb->reorderBuffer.tempbuffer.datahead[buffer[i].Q2].value;
				}
			}
			if((!tempbuffer[i].Q1need) and (!tempbuffer[i].Q2need))tempbuffer[i].ready = true;
		}
	}
}

int32_t ReservationStation::getans(int32_t value1, int32_t value2, InstructionType type){
	switch (type) {
		case InstructionType::ADD:
		case InstructionType::ADDI:
			return value1 + value2;
		case InstructionType::SUB:
			return value1 - value2;
		case InstructionType::BEQ:
			return value1 == value2;
		case InstructionType::BNE:
			return value1 != value2;
		case InstructionType::SLTI:
		case InstructionType::SLT:
		case InstructionType::BLT:
			return value1 < value2;
		case InstructionType::BGE:
			return value1 >= value2;
		case InstructionType::SLTU:
		case InstructionType::SLTIU:
		case InstructionType::BLTU:
			return static_cast<uint32_t>(value1) < static_cast<uint32_t>(value2);
		case InstructionType::BGEU:
			return static_cast<uint32_t>(value1) >= static_cast<uint32_t>(value2);
		case InstructionType::XOR:
		case InstructionType::XORI:
			return value1 ^ value2;
		case InstructionType::OR:
		case InstructionType::ORI:
			return value1 | value2;
		case InstructionType::AND:
		case InstructionType::ANDI:
			return value1 & value2;
		case InstructionType::SLL:
		case InstructionType::SLLI:
			return value1 << (value2 & 0b11111);
		case InstructionType::SRL:
		case InstructionType::SRLI:
			return static_cast<int32_t>(static_cast<uint32_t>(value1) >> (value2 & 0b11111));
		case InstructionType::SRA:
		case InstructionType::SRAI:
			return value1 >> (value2 & 0b11111);
		default:
			assert(false);
	}
}

void ReservationStation::Clear(){
	delete[] buffer;
	delete[] tempbuffer;
	delete[] alu;
	buffer = new ReservationStationEntry[size];
	tempbuffer = new ReservationStationEntry[size];
	alu = new ALU[4];
	alu[0].type = ALUTYPE::ADD;
	alu[1].type = ALUTYPE::BEQ;
	alu[2].type = ALUTYPE::XOR;
	alu[3].type = ALUTYPE::WEI;
}


