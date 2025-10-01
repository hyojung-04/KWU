#pragma once
#include "ArtistBST.h"
#include "MusicQueueNode.h"
#include <vector>

class ArtistBSTNode {
private:
	string artist;
	vector<string> title;
	string run_time;
	vector<int> rt; 
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