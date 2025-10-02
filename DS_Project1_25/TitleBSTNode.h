#pragma once
#include "TitleBST.h"
#include "MusicQueueNode.h"
#include <vector>

class TitleBSTNode {
private:
	std::string title;
	std::vector<std::string> artist;
	std::string run_time;
	std::vector<int> rt;
	int count;
	TitleBSTNode* left;
	TitleBSTNode* right;
public:
	TitleBSTNode() {}
	~TitleBSTNode() {}

	void set() {

	}
	void search() {
	
	}
};