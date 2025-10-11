#pragma once
#include "MusicQueueNode.h"
#include <vector>
#include <string>

class TitleBSTNode {
private:
    std::string title;                // key
    std::vector<std::string> artists; // same title by different artists
    std::string run_time;             // last inserted mm:ss (optional keep)
    std::vector<int> rt;              // each song's runtime in seconds
    int count=0;                        // number of songs with this title
    TitleBSTNode* left;
    TitleBSTNode* right;

public:
    TitleBSTNode(){
        this->left=nullptr;
        this->right=nullptr;
    }
    ~TitleBSTNode() {}

    // Add a (artist, runtime) pair into this title node
    void addSong(const std::string& artist, const std::string& rt_str) {
        artists.push_back(artist);
        run_time = rt_str;

        int min = std::stoi(rt_str.substr(0, rt_str.find(':')));
        int sec = std::stoi(rt_str.substr(rt_str.find(':') + 1));
        rt.push_back(min * 60 + sec);

        ++count;
    }

    // getter function
    std::string getTitle() const { return title; }
    const std::vector<std::string>& getArtistList() const { return artists; }
    const std::vector<int>& getRtList() const { return rt; }
    int getCount() const { return count; }
    TitleBSTNode* getLeft() const { return left; }
    TitleBSTNode* getRight() const { return right; }

    // setter function
    void setTitle(const std::string& t) { title = t; }
    void setLeft(TitleBSTNode* node) { left = node; }
    void setRight(TitleBSTNode* node) { right = node; }
    void setArtistList(const std::vector<std::string>& a) { artists = a; }
    void setRtList(const std::vector<int>& r) { rt = r; }
    void setCount(int c) { count = c; }
};
