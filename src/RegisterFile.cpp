//
// Created by 14908 on 2023/6/24.
//
#include "RegisterFile.h"

void RegisterFile::Flush(){
	for(int i = 0; i < 32; ++i){
		dirty[i] = tempdirty[i];
		RobEntry[i] = tempRobEntry[i];
		reg[i] = tempreg[i];
	}
}

void RegisterFile::ADDRegisterWrite(uint8_t Register, uint8_t RobEntryIndex){
	if(Register == 0)return;
	tempRobEntry[Register] = RobEntryIndex;
	tempdirty[Register] = true;
}

void RegisterFile::Write(uint8_t Register, int32_t value, uint8_t fromwhere){
	if(Register != 0){
		tempreg[Register] = value;
		if(tempRobEntry[Register] == fromwhere)tempdirty[Register] = false;
	}
}
