#include "ArtistBST.h"
#include "MusicQueueNode.h"
#include "ArtistBSTNode.h"
#include <iostream>
using namespace std;


ArtistBST::ArtistBST() {
    this->root=nullptr;
}

ArtistBST::~ArtistBST() {
    clearTree(root);
}

void ArtistBST::clearTree(ArtistBSTNode* node) {
    if (node == nullptr) return;
    clearTree(node->getLeft());
    clearTree(node->getRight());
    delete node;
}

void ArtistBST::insert(MusicQueueNode* queNode) {
    // set data
    string artist = queNode->getArtist();
    string title = queNode->getTitle();
    string run_time = queNode->getRunTime();

    // search already exist artist
    ArtistBSTNode* found = search(artist);
    if (found != nullptr) {
        // add song if artist exist
        found->addSong(title, run_time);
        return;
    }

    // Create new node if artist is not found
    ArtistBSTNode* newNode = new ArtistBSTNode();
    newNode->setArtist(artist);
    newNode->addSong(title, run_time);

    // root is empty
    if (root == nullptr) {
        root = newNode;
        return;
    }

    // search for right place
    ArtistBSTNode* cur = root;
    ArtistBSTNode* parent = nullptr;

    while (cur != nullptr) {
        parent = cur;
        if (artist < cur->getArtist()) cur = cur->getLeft();
        else cur = cur->getRight();
    }

    // link the newNode
    if (artist < parent->getArtist())
        parent->setLeft(newNode);
    else
        parent->setRight(newNode);
}

ArtistBSTNode* ArtistBST::search(const std::string& artist) {
    ArtistBSTNode* cur = root;
    while (cur) {
        if (artist == cur->getArtist()) return cur;   // search target
        else if (artist < cur->getArtist()) cur = cur->getLeft();
        else cur = cur->getRight();
    }
    return nullptr; // do not search target
}

void ArtistBST::print() {
    // case: root is empty
    if (this->root == nullptr) {
        cout << "========ERROR========" << endl;
        cout << "600" << endl;
        cout << "=====================" << endl;
        return;
    }

    // case: root exist
    cout << "========PRINT========" << endl;
    cout << "ArtistBST" << endl;
    printInOrder(this->root);
    cout << "=====================" << endl;
}

// print data 
void ArtistBST::printInOrder(ArtistBSTNode* node) {
    if (node == nullptr) return;

    printInOrder(node->getLeft());

    // print all song in node
    vector<string> titles = node->getTitleList();
    vector<int> rt = node->getRtList();
    for (size_t i = 0; i < titles.size(); i++) {
        int min = rt[i] / 60;
        int sec = rt[i] % 60;

        cout << node->getArtist() << "/" 
             << titles[i] << "/"
             << min << ":" ;
        if (sec < 10) cout << "0";
        cout << sec << endl;
    }

    printInOrder(node->getRight());
}

void ArtistBST::delete_node(const std::string& artist) {
    // case: tree empty
    if (root == nullptr) {
        cout << "========ERROR========" << endl;
        cout << "700" << endl;
        cout << "=====================" << endl;
        return;
    }

    ArtistBSTNode* cur = root;
    ArtistBSTNode* parent = nullptr;

    // find target node
    while (cur != nullptr && cur->getArtist() != artist) {
        parent = cur;
        if (artist < cur->getArtist())
            cur = cur->getLeft();
        else
            cur = cur->getRight();
    }

    // not found
    if (cur == nullptr) {
        cout << "========ERROR========" << endl;
        cout << "700" << endl;
        cout << "=====================" << endl;
        return;
    }

    // case: leaf node
    if (cur->getLeft() == nullptr && cur->getRight() == nullptr) {
        if (parent == nullptr) root = nullptr;
        else if (parent->getLeft() == cur) parent->setLeft(nullptr);
        else parent->setRight(nullptr);
        delete cur;
    }

    // case: one child
    else if (cur->getLeft() == nullptr || cur->getRight() == nullptr) {
        ArtistBSTNode* child = (cur->getLeft() != nullptr) ? cur->getLeft() : cur->getRight();
        if (parent == nullptr) root = child;
        else if (parent->getLeft() == cur) parent->setLeft(child);
        else parent->setRight(child);
        delete cur;
    }

    // case: two children
    else {
        ArtistBSTNode* succParent = cur; // succParent is new parent
        ArtistBSTNode* succ = cur->getRight();
        while (succ->getLeft() != nullptr) {
            succParent = succ;
            succ = succ->getLeft();
        }

        // Copy the successor's data to the current node.
        cur->setArtist(succ->getArtist());
        cur->setTitleList(succ->getTitleList());
        cur->setRtList(succ->getRtList());
        cur->setCount(succ->getCount());

        // remove successor
        if (succParent->getLeft() == succ)
            succParent->setLeft(succ->getRight());
        else
            succParent->setRight(succ->getRight());
        delete succ;
    }

    cout << "========DELETE========" << endl;
    cout << "Success" << endl;
    cout << "=====================" << endl;
}
