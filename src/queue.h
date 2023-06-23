//
// Created by 14908 on 2023/6/22.
//

#ifndef MAIN_CPP_QUEUE_H
#define MAIN_CPP_QUEUE_H
template<class T, int L>
class queue {
private:
	T* datahead;
	int head, tail;
	int size;
public:
	queue(){
		size = 0;
		datahead = new T[1 << L];
		head = tail = 0;
	}

	bool Full(){ return size == (1 << L); }

	bool Empty(){ return size == 0; }

	void push(T data){
		if(Full())return;
		datahead[tail] = data;
		tail++;
		size++;
		if(tail == (1 << L))tail = 0;
	}

	void pop(){
		if(Empty())return;
		head++;
		size--;
		if(head == (1 << L))head = 0;
	}

	T* front(){
		return datahead + head;
	}
	T* back(){
		if(tail == 0)return datahead + ((1 << L)-1);
		return datahead + (tail-1);
	}
};
#endif //MAIN_CPP_QUEUE_H
