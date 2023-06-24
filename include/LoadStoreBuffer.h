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
	uint32_t destination;
	uint32_t source;
	bool ready = true;
	LoadStoreEntry(){}
};
class LoadStoreBuffer{
public:
	LoadStoreBuffer(){count_ = 0;}
	~LoadStoreBuffer(){}
	bool Full(){return buffer.Full();}
	bool Empty(){return buffer.Empty();}
	void EXE(CDB*);
	void ADD(instructionNormalized&);
private:
	queue<LoadStoreEntry, 5>buffer;
	int count_ = 0;
};

#endif //MAIN_CPP_LSB_CPP