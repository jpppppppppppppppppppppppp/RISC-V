//
// Created by 14908 on 2023/6/24.
//

#ifndef MAIN_CPP_REGISTERFILE_H
#define MAIN_CPP_REGISTERFILE_H
#include "instruction.h"
class RegisterFile{
public:
	int32_t* reg;
	int32_t* tempreg;
	int8_t* RobEntry;
	int8_t* tempRobEntry;
	bool * dirty;
	bool * tempdirty;
	RegisterFile(){
		reg = new int32_t[32];
		RobEntry = new int8_t[32];
		tempRobEntry = new int8_t[32];
		tempreg = new int32_t[32];
		dirty = new bool[32];
		tempdirty = new bool[32];
	}
	~RegisterFile(){
		delete[] reg;
		delete[] tempreg;
		delete[] dirty;
		delete[] tempdirty;
		delete[] RobEntry;
		delete[] tempRobEntry;
	}
	void Flush();
	void ADDRegisterWrite(uint8_t Register, uint8_t RobEntry);
};
#endif //MAIN_CPP_REGISTERFILE_H
