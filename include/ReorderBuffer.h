//
// Created by 14908 on 2023/6/24.
//

#ifndef MAIN_CPP_REORDERBUFFER_H
#define MAIN_CPP_REORDERBUFFER_H
#include "queue.h"
#include <cstdint>
class CDB;
enum class ReorderBufferType{
	RegisterWrite,
	MemoryWrite,
	Branch,
	END
};
struct ReorderBufferEntry{
	ReorderBufferType type = ReorderBufferType::END;
	bool ready = false;
	bool predict = false;
	uint8_t index = 0;
	uint8_t ldbindex = 0;
	int32_t value = 0;
	uint32_t destination = 0;
	ReorderBufferEntry(){}
};
class ReorderBuffer{
public:
	queue<ReorderBufferEntry, 5>buffer;
	queue<ReorderBufferEntry, 5>tempbuffer;
	void Commit(CDB*);
	void Push(ReorderBufferEntry&);
	void PredictWrongClear();
	void Flush();
	bool Full(){return buffer.Full();}
	bool Empty(){return buffer.Empty();}
	ReorderBuffer(){}
	~ReorderBuffer(){}
};
#endif //MAIN_CPP_REORDERBUFFER_H
