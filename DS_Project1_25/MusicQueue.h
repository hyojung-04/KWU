#pragma once
#include "MusicQueueNode.h"
#include <string>

class MusicQueue {
private:
	MusicQueueNode* head;
	MusicQueueNode* rear;
	int size = 0;
public:
	MusicQueue();
	~MusicQueue();

	bool empty();
	bool exist(std::string line);
	void push(std::string str);
	MusicQueueNode* pop();
	MusicQueueNode* front();
};