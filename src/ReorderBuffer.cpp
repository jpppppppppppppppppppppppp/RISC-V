//
// Created by 14908 on 2023/6/24.
//
#include "ReorderBuffer.h"
#include "CDB.h"
#include <cassert>
void ReorderBuffer::Push(ReorderBufferEntry& robentry){
	robentry.index = tempbuffer.tail;
	tempbuffer.push(robentry);
}

void ReorderBuffer::Flush(){
	buffer = tempbuffer;
}

void ReorderBuffer::Commit(CDB* cdb){
	if(Empty())return;
	if(!buffer.front()->ready)return;
//	if(buffer.front()->ins == 4104)	std::cout << cdb->clock << "->" << buffer.front()->value << '\n';
//	if(buffer.front()->ins == 4108)	std::cout << cdb->clock << "->" << buffer.front()->value << '\n';
//	std::cout << std::hex <<buffer.front()->ins << std::dec << ':';
//	for(int i = 0; i < 32; ++i){
//		std::cout << (uint32_t)cdb->registerFile.reg[i] << ' ';
//	}
//	std::cout << std::endl;
	switch (buffer.front()->type) {
		case ReorderBufferType::RegisterWrite:{
			cdb->registerFile.Write(buffer.front()->destination, buffer.front()->value, buffer.gethead());
			for(int i = 0; i < cdb->reservationStation.size; ++i){
				if(cdb->reservationStation.buffer[i].used){
					if(cdb->reservationStation.buffer[i].Q1need and cdb->reservationStation.buffer[i].Q1 == buffer.front()->index){
						cdb->reservationStation.tempbuffer[i].Q1need = false;
						cdb->reservationStation.tempbuffer[i].value1 = buffer.front()->value;
					}
					if(cdb->reservationStation.buffer[i].Q2need and cdb->reservationStation.buffer[i].Q2 == buffer.front()->index){
						cdb->reservationStation.tempbuffer[i].Q2need = false;
						cdb->reservationStation.tempbuffer[i].value2 = buffer.front()->value;
					}
					if(!cdb->reservationStation.tempbuffer[i].Q1need and !cdb->reservationStation.tempbuffer[i].Q2need)cdb->reservationStation.tempbuffer[i].ready = true;
				}
			}
			break;
		}
		case ReorderBufferType::MemoryWrite: {
			cdb->loadStoreBuffer.buffer.datahead[buffer.front()->ldbindex].ready = true;
			break;
		}
		case ReorderBufferType::END: {
			std::cout << (static_cast<uint8_t>(cdb->registerFile.reg[10] & 255u)) - '\0' << std::endl;
			exit(0);
		}
		case ReorderBufferType::Branch: {
			cdb->predictor.update(buffer.front()->ldbindex, buffer.front()->value);
			if(buffer.front()->predict != buffer.front()->value){
				int tail = tempbuffer.gettail();
				while (tail != tempbuffer.gethead()){
					tempbuffer.popback();
					tail = tempbuffer.gettail();
				}
				cdb->reservationStation.Clear();
				cdb->loadStoreBuffer.Clear();
				cdb->registerFile.Clear();
				cdb->insCon.PC = tempbuffer.front()->destination;
				cdb->insCon.end = false;
				cdb->insCon.wait = false;
			}
			break;
		}
		default:
			assert(false);
	}
	tempbuffer.popfront();
}
