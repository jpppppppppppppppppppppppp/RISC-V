//
// Created by 14908 on 2023/6/21.
//
#ifndef MAIN_CPP_MEM_CPP
#define MAIN_CPP_MEM_CPP
#include "tool.h"
#include <iostream>
class Memory{
public:
	uint8_t* mem;
	int size_;
	void init();
	void test();
	Memory(int size){
		mem = new uint8_t[size];
		size_ = size;
		//init();
	}
	~Memory(){
		delete []mem;
	}
	void




};
#endif //MAIN_CPP_MEM_CPP