//
// Created by 14908 on 2023/6/20.
//
#ifndef MAIN_CPP_CDB_CPP
#define MAIN_CPP_CDB_CPP
#include "memory.h"
#include "instruction.h"
#include "parser.h"
#include "LoadStoreBuffer.cpp"
class CDB{
public:
	Memory MEM;
	instructionController insCon;
	//ReorderBuffer reorderBuffer;
	//ReservationStation reservationStation;
	LoadStoreBuffer loadStoreBuffer;
	uint clock = 0;
	//Predictor predictor;
	CDB():MEM(100000), insCon(){
		clock = 0;
	}
	void run(){
		while (true){
			clock ++;
			loadStoreBuffer.EXE(this);
			bool flag = insCon.FetchAndPush(this);//TODO
		}
		/*
		 while(true){
		    loadStoreBuffer.run()
		    FetchAndPush()
		    RS.exe(RoB)
		    RoB.commit()
		    broadcast()
		    ++clock()

		 }
		*/
	}
};
#endif //MAIN_CPP_CDB_CPP
