#pragma once
#include "ArtistBSTNode.h"

class ArtistBST {
private:
	ArtistBSTNode* root;
	ArtistBSTNode* parent;
	std::string data;
	ArtistBSTNode* target;


public:
	ArtistBST();
	~ArtistBST();

	void insert(MusicQueueNode* queNode);
	void search();
	void print();
	void delete_node();

};