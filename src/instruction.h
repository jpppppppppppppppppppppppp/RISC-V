//
// Created by 14908 on 2023/6/21.
//
#ifndef MAIN_CPP_INS_CPP
#define MAIN_CPP_INS_CPP
#include "parser.h"
#include "CDB.h"
class CDB;
class instructionController{
public:
	uint32_t PC;
	instructionController(){PC = 0;}
	bool FetchAndPush(CDB* cdb){
	}
};
#endif //MAIN_CPP_INS_CPP