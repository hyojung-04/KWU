#pragma once
#include "TitleBSTNode.h"
#include "MusicQueueNode.h"
#include <string>

class TitleBST {
private:
    TitleBSTNode* root;    
    TitleBSTNode* parent;  
    TitleBSTNode* target; 

public:
    TitleBST();
    ~TitleBST();

    void clearTree(TitleBSTNode* node);
    void insert(MusicQueueNode* queNode);
    TitleBSTNode* search(std::string title);

    void print();
    void printInOrder(TitleBSTNode* node);

    void deleteNode(std::string title);
};
