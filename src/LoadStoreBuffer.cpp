//
// Created by 14908 on 2023/6/23.
//
#include "LoadStoreBuffer.h"
#include "CDB.h"
void LoadStoreBuffer::EXE(CDB* cdb){
	if(count_ > 0){
		count_ --;
		if(count_ == 0){
			LoadStoreEntry* entry = buffer.front();
			buffer.popfront();
			switch (entry->type) {//TODO
				case InstructionType::LB: {

					break;
				}
				case InstructionType::LH: {
					break;
				}
				case InstructionType::LW: {
					break;
				}
				case InstructionType::LBU: {
					break;
				}
				case InstructionType::LHU: {
					break;
				}
				case InstructionType::SH: {
					break;
				}
				case InstructionType::SW: {
					break;
				}
				default:
					assert(false);
			}
		}
	}else if(!Empty() and buffer.front()->ready){
		count_ = 2;
	}
}
void LoadStoreBuffer::ADD(LoadStoreEntry& Entry){
	if(!buffer.Full()){
		buffer.push(Entry);
	}
}