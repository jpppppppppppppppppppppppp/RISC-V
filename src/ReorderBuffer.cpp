//
// Created by 14908 on 2023/6/24.
//
#include "ReorderBuffer.h"
void ReorderBuffer::Push(ReorderBufferEntry& robentry){
	tempbuffer.push(robentry);
	robentry.index = tempbuffer.gettail();
}

void ReorderBuffer::Flush(){
	buffer = tempbuffer;
}
