//
// Created by 14908 on 2023/6/24.
//

#ifndef MAIN_CPP_REORDERBUFFER_H
#define MAIN_CPP_REORDERBUFFER_H
#include "queue.h"
#include <cstdint>
enum class ReorderBufferType{
	RegisterWrite,
	MemoryWrite,
	Branch,
	END
};
struct ReorderBufferEntry{
	ReorderBufferType type = ReorderBufferType::END;
	uint32_t clock = 0;
	bool ready = false;
	bool predict = false;
	uint8_t index = 0;
	int32_t value = 0;
	uint32_t destination = 0;
	ReorderBufferEntry(){clock = 0;}
};
class ReorderBuffer{
public:
	queue<ReorderBufferEntry, 5>buffer;
	queue<ReorderBufferEntry, 5>tempbuffer;
	void Commit();
	void Push(ReorderBufferEntry&);
	void PredictWrongClear();
	void Flush();
	bool Full(){return buffer.Full();}
};
#endif //MAIN_CPP_REORDERBUFFER_H
