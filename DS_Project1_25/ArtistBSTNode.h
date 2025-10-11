#pragma once
#include "MusicQueueNode.h"
#include <vector>

class ArtistBSTNode {
private:
	std::string artist;
	std::vector<std::string> title;
	std::string run_time;
	std::vector<int> rt; 
	int count=0; 
	ArtistBSTNode* left;
	ArtistBSTNode* right;

public:
	ArtistBSTNode() {
		this->left=nullptr;
		this->right=nullptr;
	}
	~ArtistBSTNode(){}

	void set() {
	
	}
	void search() {

	}

	void addSong(const std::string& title, const std::string& rt_str) {
        this->title.push_back(title);
        this->run_time = rt_str;

        int min = stoi(rt_str.substr(0, rt_str.find(':')));
        int sec = stoi(rt_str.substr(rt_str.find(':') + 1));
        rt.push_back(min * 60 + sec);

        count++;
    }

	//getter function
	std::string getArtist(){ return this->artist; }
	std::vector<std::string> getTitleList() const { return title; }
    int getCount() const { return count; }
    ArtistBSTNode* getLeft() const { return left; }
    ArtistBSTNode* getRight() const { return right; }
	std::vector<int> getRtList() const {return rt; }

	//setter function
	void setArtist(std::string name){ this->artist=name; }
	void setLeft(ArtistBSTNode* node) { this->left = node; }
    void setRight(ArtistBSTNode* node) { this->right = node; }
	void setTitleList(const std::vector<std::string>& t) { this->title = t; }
	void setRtList(const std::vector<int>& r) { this->rt = r; }
	void setCount(int c) { this->count = c; }
};