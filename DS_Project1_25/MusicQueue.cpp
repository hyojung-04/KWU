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
    string artist, title, run_time;
    //extract elements from line
    int idx1, idx2;
		idx1=str.find('|');
		idx2=str.find('|',idx1+1);
		artist=str.substr(0,idx1);
		title=str.substr(idx1+1, idx2-idx1-1);
		run_time=str.substr(idx2,str.size()-idx2);


    MusicQueueNode* node=new MusicQueueNode;
    node->insert(artist, title, run_time);
}

MusicQueueNode* MusicQueue::pop(){
    
}

MusicQueueNode* MusicQueue::front() {

}