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
	static void f1(CDB * cdb){
		cdb->insCon.FetchAndPush(cdb);
	};
	static void f2(CDB * cdb){
		cdb->reorderBuffer.Commit(cdb);
	};
	static void f3(CDB * cdb){
		cdb->loadStoreBuffer.EXE(cdb);
	};
	static void f4(CDB * cdb){
		cdb->reservationStation.EXE(cdb);
	};
};
#endif //MAIN_CPP_CDB_CPP
