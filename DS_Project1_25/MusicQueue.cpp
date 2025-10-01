#include "MusicQueue.h"


MusicQueue::MusicQueue() {

}

MusicQueue::~MusicQueue() {

}

bool MusicQueue::empty() {
    if(size<1) return true;
    else return false;
}

bool MusicQueue::exist() {

}

void MusicQueue::push(string str) {
    MusicQueueNode* node=new MusicQueueNode;
    node->insert(str);
}

MusicQueueNode* MusicQueue::pop(){
    
}

MusicQueueNode* MusicQueue::front() {

}