//
// Created by 14908 on 2023/6/24.
//

#ifndef MAIN_CPP_RESERVATIONSTATION_H
#define MAIN_CPP_RESERVATIONSTATION_H
#include "queue.h"
#include "instruction.h"
#include "ALU.h"
class CDB;
struct ReservationStationEntry{
	uint8_t index = 0;
	InstructionType type = InstructionType::END;
	bool ready = false;
	bool used = false;
	bool exe = false;
	int32_t value1 = 0;
	int32_t value2 = 0;
	uint8_t Q1 = 0;
	bool Q1need = false;
	uint8_t Q2 = 0;
	bool Q2need = false;
	uint8_t RoBEntry = 0;
};
class ReservationStation{
public:
	ALU *alu;
	ReservationStationEntry *buffer;
	ReservationStationEntry *tempbuffer;
	int size = 1 << 5;
	int alusize = 4;
	ReservationStation(){
		buffer = new ReservationStationEntry[size];
		tempbuffer = new ReservationStationEntry[size];
		alu = new ALU[4];
		alu[0].type = ALUTYPE::ADD;
		alu[1].type = ALUTYPE::BEQ;
		alu[2].type = ALUTYPE::XOR;
		alu[3].type = ALUTYPE::WEI;
	}
	~ReservationStation(){
		delete[] buffer;
		delete[] tempbuffer;
		delete[] alu;
	}
	void Flush();
	void Push(ReservationStationEntry&);
	void EXE(CDB*);
	int32_t getans(int32_t value1, int32_t value2, InstructionType type);
};
#endif //MAIN_CPP_RESERVATIONSTATION_H
