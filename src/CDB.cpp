//
// Created by 14908 on 2023/6/24.
//
#include "CDB.h"
#include <algorithm>
#include <vector>
void CDB::run(){
	std::vector<void (*)(CDB *)>p;
	p.push_back(f1);
	p.push_back(f2);
	p.push_back(f3);
	p.push_back(f4);
	while (true){
		clock++;
		srand(time(NULL));
		std::random_shuffle(p.begin(),p.end());
		for(int i = 0; i < 4; ++i){
			p[i](this);
		}

//		f1(this);
//		f4(this);
//		f3(this);
//		f2(this);
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

