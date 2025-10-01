#pragma once
#include "PlayListNode.h"

class PlayList {
private:
	PlayListNode* head;
	PlayListNode* cursor;
	int count;
	int time;
	string data;

public:
	PlayList() {}
	~PlayList(){}
	void insert_node();
	void delete_node();
	bool empty();
	bool full();
	bool exist();
	string print();
	int run_time();
};