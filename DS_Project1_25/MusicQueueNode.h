#pragma once
//#include "MusicQueue.h"
#include <string>
#include <cstring>
#include <sstream>

// using namespace std;

class MusicQueueNode {
private:
	std::string artist;
	std::string title;
	std::string run_time;

	MusicQueueNode* next;
	MusicQueueNode* prev;

public:
	MusicQueueNode() {
		next = nullptr;
		prev = nullptr;
	}
	~MusicQueueNode() {}

	void insert(std::string art, std::string tit, std::string rt ) {
		artist = art;
		title = tit;
		run_time = rt;
	}

	bool exist(std::string line) {
	//extract elements from line
    std::string art, tit;
    size_t idx1, idx2;
    idx1 = line.find('|');
    idx2 = line.find('|', idx1 + 1);
    art = line.substr(0, idx1);
    tit = line.substr(idx1 + 1, idx2 - idx1 - 1);

	if(this->artist==art&&this->title==tit) return true;
	else return false;

	}


	//setter function
	void setNext(MusicQueueNode* nxt) {
		this->next = nxt;
	}

	void setPrev(MusicQueueNode* prv){
		this->prev=prv;
	}

	//getter function
	MusicQueueNode* getNext() {
		if(!this->next){
			return nullptr;
		}
		else{
			return this->next;
		}
	}

	MusicQueueNode* getPrev(){
		if(!this->prev){
			return nullptr;
		}
		else{
			return this->prev;
		}
	}

	std::string  getArtist(){
		return this->artist;
	}

	std::string  getTitle(){
		return this->title;
	}
	
	std::string getRunTime(){
		return this->run_time;
	}
};