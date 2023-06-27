//
// Created by 14908 on 2023/6/24.
//
#include "CDB.h"

void CDB::run(){
	while (true){
		clock++;
		loadStoreBuffer.EXE(this);
		insCon.FetchAndPush(this);
		reorderBuffer.Commit(this);
		reservationStation.EXE(this);
		Flush();
		//std::cout << clock << "MEM:" << MEM.mem[4096]-'\0' << ' ' << MEM.mem[4097]-'\0' << ' ' << MEM.mem[4098]-'\0' << ' ' << MEM.mem[4099]-'\0' << '\n';

	}
	/*
	 while(true){
		loadStoreBuffer.run()
		FetchAndPush()
		RS.exe(RoB)
		RoB.commit()
		broadcast()
		++clock()
		Flush()
	 }
	*/

}

void CDB::Flush(){
	reorderBuffer.Flush();
	registerFile.Flush();
	reservationStation.Flush();
}

