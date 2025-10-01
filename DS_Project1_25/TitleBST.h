#pragma once
#include "TitleBSTNode.h"

class TitleBST {
private:
	TitleBSTNode* root;
	TitleBSTNode* parent;
	string data;
	TitleBSTNode* target;

public:
	TitleBST();
	~TitleBST();

	void insert();
	void search();
	void print();
	void delete_node();
};