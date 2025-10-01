#pragma once
#include "PlayList.h"

class PlayListNode {
private:
	string artist;
	string title;
	int runtime_sec;

	PlayListNode* prev;
	PlayListNode* next;

public:
	PlayListNode(){}
	~PlayListNode(){}
	
	void set() {

	}
	
};