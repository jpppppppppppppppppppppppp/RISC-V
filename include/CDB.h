//
// Created by 14908 on 2023/6/20.
//
#ifndef MAIN_CPP_CDB_CPP
#define MAIN_CPP_CDB_CPP
#include "memory.h"
#include "instruction.h"
#include "LoadStoreBuffer.h"
#include "RegisterFile.h"
#include "ReorderBuffer.h"
#include "ReservationStation.h"
#include "predictor.h"
class CDB{
public:
	Memory MEM;
	instructionController insCon;
	RegisterFile registerFile;
	ReorderBuffer reorderBuffer;
	ReservationStation reservationStation;
	LoadStoreBuffer loadStoreBuffer;
	uint clock = 0;
	Predictor predictor;
	CDB():MEM(2048000), insCon(){
		clock = 0;
	}
	~CDB(){}
	void run();
	void Flush();
};
#endif //MAIN_CPP_CDB_CPP
