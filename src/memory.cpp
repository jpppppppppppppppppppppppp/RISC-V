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

int32_t Memory::LB(uint32_t ind){
	return static_cast<int32_t>(static_cast<uint8_t>(mem[ind]));
}

int32_t Memory::LH(uint32_t ind){
	return static_cast<int32_t>(static_cast<uint16_t>(mem[ind]) + (static_cast<uint16_t>(mem[ind+1]) << 8));
}

int32_t Memory::LW(uint32_t ind){
	return static_cast<int32_t>((static_cast<uint32_t>(mem[ind])) + (static_cast<uint32_t>(mem[ind + 1]) << 8) +
	                            (static_cast<uint32_t>(mem[ind + 2]) << 16) +
	                            (static_cast<uint32_t>(mem[ind + 3]) << 24));
}

int32_t Memory::LBU(uint32_t ind){
	return static_cast<int32_t>(static_cast<uint32_t>(static_cast<uint8_t>(mem[ind])));
}

int32_t Memory::LHU(uint32_t ind){
	return static_cast<int32_t>(static_cast<uint32_t>(static_cast<uint16_t>((mem[ind]) + (mem[ind+1] << 8))));
}
