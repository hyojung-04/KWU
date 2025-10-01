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

	void insert(string str) {
		int idx1, idx2;
		idx1=str.find('|');
		idx2=str.find('|',idx1+1);
		artist=str.substr(0,idx1);
		title=str.substr(idx1+1, idx2-idx1-1);
		run_time=str.substr(idx2,str.size()-idx2);
	}

	bool exist() {

	}
};