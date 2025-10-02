#pragma once
#include "ArtistBST.h"
#include "MusicQueueNode.h"
#include <vector>

class ArtistBSTNode {
private:
	std::string artist;
	std::vector<std::string> title;
	std::string run_time;
	std::vector<int> rt; 
	int count; 
	ArtistBSTNode* left;
	ArtistBSTNode* right;

public:
	ArtistBSTNode() {}
	~ArtistBSTNode(){}

	void set() {

	}
	void search() {

	}
	
};