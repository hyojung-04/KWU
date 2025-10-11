#pragma once
#include <string>
#include "PlayListNode.h"
const int PLAYLIST_MAX_SIZE = 10;

class PlayList {
private:
	PlayListNode* head;
	PlayListNode* cursor;
	int count=0;
	int time=0;

public:
	PlayList();
	~PlayList();
	void insert_node(const std::string& line);
    void delete_node(const std::string& line);
    bool exist(const std::string& line);
	bool empty();
	bool full();
	std::string print();
	int run_time();
};