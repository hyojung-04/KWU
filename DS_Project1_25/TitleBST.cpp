#include "TitleBST.h"
#include "MusicQueueNode.h"
#include <iostream>
#include <string>
using namespace std;


TitleBST::TitleBST() {
    this->root = nullptr;
    this->parent = nullptr;
    this->target = nullptr;
}

TitleBST::~TitleBST() {
    clearTree(root);
    root=nullptr;
}

void TitleBST::clearTree(TitleBSTNode* node) {
    if (node == nullptr) return;
    clearTree(node->getLeft());
    clearTree(node->getRight());
    delete node;
}

void TitleBST::insert(MusicQueueNode* queNode) {
    // set data
    string artist = queNode->getArtist();
    string title = queNode->getTitle();
    string run_time = queNode->getRunTime();

    // search already exist title
    TitleBSTNode* found = search(title);
    if (found != nullptr) {
        // add song if title exist
        found->addSong(artist, run_time);
        return;
    }

    // create new node if title not exist
    TitleBSTNode* newNode = new TitleBSTNode();
    newNode->setTitle(title);
    newNode->addSong(artist, run_time);

    // case: empty tree
    if (root == nullptr) {
        root = newNode;
        return;
    }

    // find position to insert
    TitleBSTNode* cur = root;
    TitleBSTNode* par = nullptr;

    while (cur != nullptr) {
        par = cur;
        if (title < cur->getTitle()) cur = cur->getLeft();
        else cur = cur->getRight();
    }

    // link new node to parent
    if (title < par->getTitle()) par->setLeft(newNode);
    else par->setRight(newNode);
}

TitleBSTNode* TitleBST::search(string title) {
    parent = nullptr;
    target = root;

    while (target != nullptr) {
        if (title == target->getTitle())
            return target;
        parent = target;
        if (title < target->getTitle())
            target = target->getLeft();
        else
            target = target->getRight();
    }

    target = nullptr;
    return nullptr;
}

void TitleBST::print() {
    if (root == nullptr) {
        cout << "========ERROR========" << endl;
        cout << "600" << endl;
        cout << "=====================" << endl;
        return;
    }

    cout << "========PRINT========" << endl;
    cout << "TitleBST" << endl;
    printInOrder(root);
    cout << "=====================" << endl;
}

void TitleBST::printInOrder(TitleBSTNode* node) {
    if (node == nullptr) return;

    printInOrder(node->getLeft());

    const vector<string>& artists = node->getArtistList();
    const vector<int>& rt = node->getRtList();

    // print all songs in same title node
    for (size_t i = 0; i < artists.size(); ++i) {
        int min = rt[i] / 60;
        int sec = rt[i] % 60;
        cout << artists[i] << "/" << node->getTitle() << "/";
        if (sec < 10) cout << min << ":0" << sec << endl;
        else cout << min << ":" << sec << endl;
    }

    printInOrder(node->getRight());
}

void TitleBST::deleteNode(string title) {
    // case: tree empty
    if (root == nullptr) {
        cout << "========ERROR========" << endl;
        cout << "700" << endl;
        cout << "=====================" << endl;
        return;
    }

    // search target node
    TitleBSTNode* cur = search(title);
    if (cur == nullptr) {
        cout << "========ERROR========" << endl;
        cout << "700" << endl;
        cout << "=====================" << endl;
        return;
    }

    // case: no child
    if (cur->getLeft() == nullptr && cur->getRight() == nullptr) {
        if (cur == root) root = nullptr;
        else if (parent->getLeft() == cur) parent->setLeft(nullptr);
        else parent->setRight(nullptr);
        delete cur;
    }

    // case: one child
    else if (cur->getLeft() == nullptr || cur->getRight() == nullptr) {
        TitleBSTNode* child = (cur->getLeft() != nullptr) ? cur->getLeft() : cur->getRight();
        if (cur == root) root = child;
        else if (parent->getLeft() == cur) parent->setLeft(child);
        else parent->setRight(child);
        delete cur;
    }

    // case: two children
    else {
        TitleBSTNode* succParent = cur;
        TitleBSTNode* succ = cur->getRight();

        // find smallest node in right subtree
        while (succ->getLeft() != nullptr) {
            succParent = succ;
            succ = succ->getLeft();
        }

        // copy successor's data to current node
        cur->setTitle(succ->getTitle());
        cur->setArtistList(succ->getArtistList());
        cur->setRtList(succ->getRtList());
        cur->setCount(succ->getCount());

        // remove successor node
        if (succParent->getLeft() == succ)
            succParent->setLeft(succ->getRight());
        else
            succParent->setRight(succ->getRight());

        delete succ;
    }

    // print success message
    cout << "========DELETE========" << endl;
    cout << "Success" << endl;
    cout << "=====================" << endl;
}
