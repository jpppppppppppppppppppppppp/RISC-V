//
// Created by 14908 on 2023/6/21.
//
#ifndef MAIN_CPP_MEM_CPP
#define MAIN_CPP_MEM_CPP
#include <iostream>
#include "instruction.h"
class Memory{
public:
	uint8_t* mem;
	int size_;
	void init();
	void test();
	Memory(int size){
		mem = new uint8_t[size];
		size_ = size;
		init();
	}
	~Memory(){
		delete []mem;
	}
	int32_t LB(uint32_t ind);
	int32_t LH(uint32_t ind);
	int32_t LW(uint32_t ind);
	int32_t LBU(uint32_t ind);
	int32_t LHU(uint32_t ind);
};
#endif //MAIN_CPP_MEM_CPP