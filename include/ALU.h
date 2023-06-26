//
// Created by 14908 on 2023/6/25.
//

#ifndef MAIN_CPP_ALU_H
#define MAIN_CPP_ALU_H
#include <cstdint>
enum class ALUTYPE{
	ADD,
	BEQ,
	XOR,
	WEI
};
class ALU{
public:
	ALUTYPE type;
	bool over = false;
	bool tempover = false;
	bool working= false;
	bool tempworking= false;
	int32_t ans = 0;
	int32_t tempans = 0;
	uint8_t RSEntry = 0;
	uint8_t tempRSEntry = 0;
	ALU(){};
	~ALU(){};
	void Flush(){
		over = tempover;
		working = tempworking;
		ans = tempans;
		RSEntry = tempRSEntry;
	}
};
#endif //MAIN_CPP_ALU_H
