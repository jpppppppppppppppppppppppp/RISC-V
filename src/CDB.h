//
// Created by 14908 on 2023/6/20.
//
#ifndef MAIN_CPP_CDB_CPP
#define MAIN_CPP_CDB_CPP
#include "memory.h"
#include "instruction.h"
#include "parser.h"
#include "LoadStoreBuffer.h"
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
		std::string line1,line2,line3,line4;
		while (true){
			clock ++;
			std::cin >> line1 >> line2 >> line3 >> line4;
			parser(getcode(str2int8(line1,16),str2int8(line2,16),str2int8(line3,16),str2int8(line4,16)));
			loadStoreBuffer.EXE(this);
			insCon.FetchAndPush();
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
