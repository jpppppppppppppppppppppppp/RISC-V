//
// Created by 14908 on 2023/6/22.
//

#ifndef MAIN_CPP_QUEUE_H
#define MAIN_CPP_QUEUE_H
template<class T, int L>
class queue {
public:
	T* datahead;
	int head, tail;
	int size;
	queue(){
		size = 0;
		datahead = new T[1 << L];
		head = tail = 0;
	}
	~queue(){
		delete[] datahead;
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

	void popfront(){
		if(Empty())return;
		head++;
		size--;
		if(head == (1 << L))head = 0;
	}
	void popback(){
		if(Empty())return;
		size--;
		if(tail == 0)tail = (1 << L) - 1;
		else tail--;
	}
	T* front(){
		return datahead + head;
	}
	T* back(){
		if(tail == 0)return datahead + ((1 << L)-1);
		return datahead + (tail-1);
	}
	queue& operator = (const queue& rhs){
		if(this == &rhs)return *this;
		head = rhs.head;
		tail = rhs.tail;
		size = rhs.size;
		delete[] datahead;
		datahead = new T[1 << L];
		for(int i = 0; i < (1<<L); ++i){
			datahead[i] = rhs.datahead[i];
		}
		return *this;
	}
	int gettail(){
		if(tail != 0)return tail-1;
		return (1<<L)-1;
	}
	int gethead(){
		return head;
	}
	void clear(){
		delete[] datahead;
		datahead = new T[1 << L];
		size = head = tail = 0;
	}
};
#endif //MAIN_CPP_QUEUE_H
