#include "MusicQueue.h"
#include <iostream>
using namespace std;

MusicQueue::MusicQueue()
{
    this->head = nullptr;
    this->rear = nullptr;
}

MusicQueue::~MusicQueue()
{
    while (this->head)
    {
        MusicQueueNode *tmpNode = head;
        head = head->getNext();
        delete tmpNode;
    }
    rear = nullptr;
}

bool MusicQueue::empty()
{
    return this->head==nullptr;
}

bool MusicQueue::exist(string line)
{

    MusicQueueNode *curNode = head; // node for search
    while (curNode)
    {
        if (curNode->exist(line))
            return true;
        curNode = curNode->getNext();
    }
    return false;
}

void MusicQueue::push(string line)
{
    if (this->size == 100)
    {
        return;
    }
    // extract elements from line
    string artist, title, run_time;
    size_t idx1, idx2;
    idx1 = line.find('|');
    idx2 = line.find('|', idx1 + 1);
    artist = line.substr(0, idx1);
    title = line.substr(idx1 + 1, idx2 - idx1 - 1);
    run_time = line.substr(idx2+1);

    MusicQueueNode *tmpNode = new MusicQueueNode;
    tmpNode->insert(artist, title, run_time);
    if (!head)
    {
        this->head = tmpNode;
        this->rear = tmpNode;
    }
    else
    {
        this->rear->setNext(tmpNode);
        tmpNode->setPrev(rear);
        this->rear = tmpNode;
    }
    size++;
}

MusicQueueNode *MusicQueue::pop()
{
    if (!this->head)
    {
        return nullptr;
    }
    MusicQueueNode *tmpNode = this->head; // store head for return
    this->head = this->head->getNext();   // update head to head->next
    if(this->head){
        this->head->setPrev(nullptr);
    }else{
        this->rear=nullptr;
    }
    size--;
    return tmpNode;
}

MusicQueueNode *MusicQueue::front()
{
    if (!this->head)
        return nullptr;
    else
        return this->head;
}