#pragma once
#include <string>
#include <iostream>

class PlayListNode {
private:
	std::string artist;
	std::string title;
	int runtime_sec=0;

	PlayListNode* prev;
	PlayListNode* next;

public:
	PlayListNode() {
		this->prev = nullptr;
		this->next = nullptr;
	}

	~PlayListNode() {}

	// Getter function
	std::string getArtist() const { return artist; }
	std::string getTitle() const { return title; }
	int getRunTime() const { return runtime_sec; }
	PlayListNode* getPrev() const { return prev; }
	PlayListNode* getNext() const { return next; }

	// Setter function
	void insert(std::string art, std::string tit, int rt) {
		this->artist = art;
		this->title = tit;
		this->runtime_sec = rt;
	}

	void linkPrev(PlayListNode* node) { this->prev = node; }
	void linkNext(PlayListNode* node) { this->next = node; }
};