//
// Created by 14908 on 2023/6/24.
//
#include "ReorderBuffer.h"
#include "CDB.h"
#include <cassert>
void ReorderBuffer::Push(ReorderBufferEntry& robentry){
	tempbuffer.push(robentry);
	robentry.index = tempbuffer.gettail();
}

void ReorderBuffer::Flush(){
	buffer = tempbuffer;
}

void ReorderBuffer::Commit(CDB* cdb){
	if(Empty())return;
	if(!buffer.front()->ready)return;
	switch (buffer.front()->type) {
		case ReorderBufferType::RegisterWrite:{
			cdb->registerFile.Write(buffer.front()->destination, buffer.front()->value, buffer.gethead());
			break;
		}
		default:
			assert(false);
	}
	tempbuffer.popfront();
}
