#pragma once
//#include "MusicQueue.h"
#include <string>
#include <cstring>
#include <sstream>

using namespace std;

class MusicQueueNode {
private:
	string artist;
	string title;
	string run_time;

	MusicQueueNode* next;
	MusicQueueNode* prev;

public:
	MusicQueueNode() {}
	~MusicQueueNode() {}

	void insert(string art, string tit, string rt) {
		artist=art;
		title=tit;
		run_time=rt;
	}

	bool exist() {

	}
};