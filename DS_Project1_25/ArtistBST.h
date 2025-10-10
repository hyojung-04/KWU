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
	void clearTree(ArtistBSTNode* node);
	void insert(MusicQueueNode* queNode);
	ArtistBSTNode* search(const std::string& artist);
	void print();
	void printInOrder(ArtistBSTNode* node);
	void delete_node(const std::string& artist);

};