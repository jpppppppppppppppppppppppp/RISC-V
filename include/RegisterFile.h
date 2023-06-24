//
// Created by 14908 on 2023/6/24.
//

#ifndef MAIN_CPP_REGISTERFILE_H
#define MAIN_CPP_REGISTERFILE_H
#include "instruction.h"
class RegisterFile{
public:
	int32_t* reg;
	bool * ready;
	int8_t* ALU;
	int8_t* LSB;
	RegisterFile(){
		reg = new int32_t[32];
		ready = new bool[32];
		ALU = new int8_t[32];
		LSB = new int8_t[32];
		for(int i = 0; i < 32; ++i){
			ready[i] = true;
		}
	}
	~RegisterFile(){
		delete[] reg;
		delete[] ready;
		delete[] ALU;
		delete[] LSB;
	}
};
#endif //MAIN_CPP_REGISTERFILE_H
