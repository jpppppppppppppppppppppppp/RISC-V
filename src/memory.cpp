//
// Created by 14908 on 2023/6/23.
//
#include "memory.h"
void Memory::init(){
	std::string buffer;
	uint32_t address = 0;
	while (std::cin >> buffer){
		if(buffer[0] == '@'){
			std::string ad = buffer.substr(1);
			address = str2int32(ad, 16);
		}
		else{
			mem[address] = str2int8(buffer, 16);
			address ++;
		}
	}
}

void Memory::test(){
	for(int i = 0; i < size_; ++i){
		std::cout << mem[i]-'\0' << ' ';
	}
	puts("");
}