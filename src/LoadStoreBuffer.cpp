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
			buffer.front()->empty = true;
			buffer.popfront();
			switch (entry->type) {
				case InstructionType::LB: {
					cdb->reorderBuffer.tempbuffer.datahead[entry->RoBEntry].value = cdb->MEM.LB(entry->value);
					cdb->reorderBuffer.tempbuffer.datahead[entry->RoBEntry].ready = true;
					break;
				}
				case InstructionType::LH: {
					cdb->reorderBuffer.tempbuffer.datahead[entry->RoBEntry].value = cdb->MEM.LH(entry->value);
					cdb->reorderBuffer.tempbuffer.datahead[entry->RoBEntry].ready = true;
					break;
				}
				case InstructionType::LW: {
					cdb->reorderBuffer.tempbuffer.datahead[entry->RoBEntry].value = cdb->MEM.LW(entry->value);
					cdb->reorderBuffer.tempbuffer.datahead[entry->RoBEntry].ready = true;
					break;
				}
				case InstructionType::LBU: {
					cdb->reorderBuffer.tempbuffer.datahead[entry->RoBEntry].value = cdb->MEM.LBU(entry->value);
					cdb->reorderBuffer.tempbuffer.datahead[entry->RoBEntry].ready = true;
					break;
				}
				case InstructionType::LHU: {
					cdb->reorderBuffer.tempbuffer.datahead[entry->RoBEntry].value = cdb->MEM.LHU(entry->value);
					cdb->reorderBuffer.tempbuffer.datahead[entry->RoBEntry].ready = true;
					break;
				}
				case InstructionType::SH: {
					cdb->MEM.mem[entry->destination + 1] = static_cast<int8_t>(entry->value >> 8);
					cdb->MEM.mem[entry->destination] = static_cast<int8_t>(entry->value);
					break;
				}
				case InstructionType::SW: {
					cdb->MEM.mem[entry->destination + 3] = static_cast<int8_t>(entry->value >> 24);
					cdb->MEM.mem[entry->destination + 2] = static_cast<int8_t>(entry->value >> 16);
					cdb->MEM.mem[entry->destination + 1] = static_cast<int8_t>(entry->value >> 8);
					cdb->MEM.mem[entry->destination] = static_cast<int8_t>(entry->value);
					break;
				}
				case InstructionType::SB: {
					cdb->MEM.mem[entry->destination] = static_cast<int8_t>(entry->value);
					break;
				}
				default:
					assert(false);
			}
		}
	}else if(!Empty() and buffer.front()->ready){
		count_ = 2;
	}
	for(int i = 0; i < (1<<5); ++i){
		if(buffer.datahead[i].empty)continue;
		if(buffer.datahead[i].ready)continue;
		switch (buffer.datahead[i].type) {
			case InstructionType::LB:
			case InstructionType::LH:
			case InstructionType::LW:
			case InstructionType::LBU:
			case InstructionType::LHU: {
				if(buffer.datahead[i].valueneed){
					if(cdb->registerFile.dirty[buffer.datahead[i].value]){
						if(cdb->reorderBuffer.buffer.datahead[buffer.datahead[i].valueRoB].ready){
							buffer.datahead[i].value = cdb->reorderBuffer.buffer.datahead[buffer.datahead[i].valueRoB].value + buffer.datahead[i].offset;
							buffer.datahead[i].valueneed = false;
						}else continue;
					}else{
						buffer.datahead[i].valueneed = false;
						buffer.datahead[i].value = cdb->registerFile.reg[buffer.datahead[i].value] + buffer.datahead[i].offset;
					}
				}else
					assert(false);
				if(!buffer.datahead[i].valueneed and !buffer.datahead[i].destinationneed)buffer.datahead[i].ready = true;
				break;
			}
			case InstructionType::SB:
			case InstructionType::SH:
			case InstructionType::SW: {
				if(buffer.datahead[i].valueneed){
					if(cdb->registerFile.dirty[buffer.datahead[i].value]){
						if(cdb->reorderBuffer.buffer.datahead[buffer.datahead[i].valueRoB].ready){
							buffer.datahead[i].value = cdb->reorderBuffer.buffer.datahead[buffer.datahead[i].valueRoB].value;
							buffer.datahead[i].valueneed = false;
						}else buffer.datahead[i].valueneed = true;
					}else{
						buffer.datahead[i].valueneed = false;
						buffer.datahead[i].value = cdb->registerFile.reg[buffer.datahead[i].value];
					}
				}
				if(buffer.datahead[i].destinationneed){
					if(cdb->registerFile.dirty[buffer.datahead[i].destination]){
						if(cdb->reorderBuffer.buffer.datahead[buffer.datahead[i].destinationRob].ready){
							buffer.datahead[i].destination = cdb->reorderBuffer.buffer.datahead[buffer.datahead[i].destinationRob].value + buffer.datahead[i].offset;
							buffer.datahead[i].destinationneed = false;
						}else buffer.datahead[i].destinationneed = true;
					}else{
						buffer.datahead[i].destinationneed = false;
						buffer.datahead[i].destination = cdb->registerFile.reg[buffer.datahead[i].destination] + buffer.datahead[i].offset;
					}
				}
				if(!buffer.datahead[i].valueneed and !buffer.datahead[i].destinationneed)buffer.datahead[i].ready = true;
				break;
			}
			default:
				assert(false);
		}
	}



}
void LoadStoreBuffer::ADD(LoadStoreEntry& Entry){
	if(!buffer.Full()){
		Entry.empty = false;
		buffer.push(Entry);
	}
}

void LoadStoreBuffer::Clear(){
	buffer.clear();
	count_ = 0;
}
