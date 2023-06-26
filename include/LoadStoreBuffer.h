//
// Created by 14908 on 2023/6/22.
//
#ifndef MAIN_CPP_LSB_CPP
#define MAIN_CPP_LSB_CPP
#include "instruction.h"
#include "queue.h"
class CDB;
struct LoadStoreEntry{
	InstructionType type = InstructionType::END;
	uint32_t destination = 0;
	bool destinationneed = false;
	uint8_t destinationRob = 0;
	uint32_t value = 0;
	bool valueneed = false;
	uint8_t valueRoB = 0;
	uint8_t RoBEntry = 0;
	int32_t offset = 0;
	bool ready = true;
	bool empty = true;
	LoadStoreEntry(){}
};
class LoadStoreBuffer{
public:
	LoadStoreBuffer(){count_ = 0;}
	~LoadStoreBuffer(){}
	bool Full(){return buffer.Full();}
	bool Empty(){return buffer.Empty();}
	void EXE(CDB*);
	void ADD(LoadStoreEntry&);
	void Clear();
	queue<LoadStoreEntry, 5>buffer;
	int count_ = 0;
};

#endif //MAIN_CPP_LSB_CPP