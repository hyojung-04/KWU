#include "PlayList.h"
#include "PlayListNode.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

// convert "mm:ss" to seconds
static int toSeconds(const string& str) {
    size_t pos = str.find(':');
    if (pos == string::npos) return 0;
    int m = stoi(str.substr(0, pos));
    int s = stoi(str.substr(pos + 1));
    return m * 60 + s;
}

// convert seconds to "m:ss"
static string toMMSS(int sec) {
    int m = sec / 60;
    int s = sec % 60;
    stringstream ss;
    ss << m << ":";
    if (s < 10) ss << "0";
    ss << s;
    return ss.str();
}

PlayList::PlayList() {
    this->head = nullptr;
    this->cursor = nullptr;
    this->count = 0;
    this->time = 0;
}

PlayList::~PlayList() {
    if (!this->head) return;

    PlayListNode* cur = this->head->getNext();
    while (cur != this->head) {
        PlayListNode* tmp = cur->getNext();
        delete cur;
        cur = tmp;
    }
    delete this->head;

    this->head = nullptr;
    this->cursor = nullptr;
}

void PlayList::insert_node(const string& line) {
    if (this->full() || line.empty()) return;

    // Split line into artist, title, runtime
    size_t idx1 = line.find('|');
    size_t idx2 = line.find('|', idx1 + 1);
    if (idx1 == string::npos || idx2 == string::npos) return;

    string artist = line.substr(0, idx1);
    string title = line.substr(idx1 + 1, idx2 - idx1 - 1);
    string runtime = line.substr(idx2 + 1);
    int runtime_sec = toSeconds(runtime);

    // Skip if song already exists
    if (this->exist(line)) return;

    // Create new node
    PlayListNode* node = new PlayListNode();
    node->insert(artist, title, runtime_sec);

    // Case 1: empty list
    if (!this->head) {
        this->head = node;
        this->head->linkPrev(this->head);
        this->head->linkNext(this->head);
        this->cursor = this->head;
    } 
    // Case 2: non-empty list, add to end
    else {
        PlayListNode* tail = this->head->getPrev();
        tail->linkNext(node);
        node->linkPrev(tail);
        node->linkNext(this->head);
        this->head->linkPrev(node);
        this->cursor = node;
    }

    this->count++;
    this->time += runtime_sec; // update total runtime
}

void PlayList::delete_node(const string& line) {
    if (this->empty() || line.empty()) return;

    // Split line into artist and title
    size_t idx1 = line.find('|');
    size_t idx2 = line.find('|', idx1 + 1);
    if (idx1 == string::npos || idx2 == string::npos) return;

    string artist = line.substr(0, idx1);
    string title = line.substr(idx1 + 1, idx2 - idx1 - 1);

    PlayListNode* cur = this->head;
    for (int i = 0; i < this->count; i++) {
        if (cur->getArtist() == artist && cur->getTitle() == title) {
            int runtime_sec = cur->getRunTime();
            
            // Case 1: only one node in list
            if (this->count == 1) {
                delete cur;
                this->head = nullptr;
                this->cursor = nullptr;
                this->count = 0;
                this->time = 0;
                return;
            }
            // Case 2: multiple nodes
			else {
				PlayListNode* prev = cur->getPrev();
				PlayListNode* next = cur->getNext();
				prev->linkNext(next);
				next->linkPrev(prev);
				if (cur == this->head) this->head = next;
				if (cur == this->cursor) this->cursor = next;
				delete cur;
				this->count--;
				this->time -= runtime_sec; // update total runtime
				return;
			}
        }
        cur = cur->getNext();
    }

    if (this->empty()) {
    this->head = nullptr;
    this->cursor = nullptr;
    return;
}

}

bool PlayList::exist(const string& line) {
    if (this->empty() || line.empty()) return false;

    size_t idx1 = line.find('|');
    size_t idx2 = line.find('|', idx1 + 1);
    if (idx1 == string::npos || idx2 == string::npos) return false;

    string artist = line.substr(0, idx1);
    string title = line.substr(idx1 + 1, idx2 - idx1 - 1);

    PlayListNode* cur = this->head;
    for (int i = 0; i < this->count; i++) {
        if (cur->getArtist() == artist && cur->getTitle() == title)
            return true;
        cur = cur->getNext();
    }
    return false;
}

bool PlayList::empty() {
    return this->count == 0;
}

bool PlayList::full() {
    return this->count == PLAYLIST_MAX_SIZE;
}

string PlayList::print() {
    if (this->empty()) {
    return "========ERROR========\n600\n=====================\n";
}

    stringstream ss;
    PlayListNode* cur = this->head;
    for (int i = 0; i < this->count; i++) {
        ss << cur->getArtist() << "/" << cur->getTitle() << "/"
           << toMMSS(cur->getRunTime()) << "\n";
        cur = cur->getNext();
    }
    ss << "Count : " << this->count << " / 10\n";
    ss << "Time : " << (this->time / 60) << "min ";
    if (this->time % 60 < 10) ss << "0";
    ss << (this->time % 60) << "sec\n";
    return ss.str();
}

int PlayList::run_time() {
    return this->time;
}
