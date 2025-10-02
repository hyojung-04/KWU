#include "ArtistBST.h"


ArtistBST::ArtistBST() {
    this->root=nullptr;
    this->parent=nullptr;
    this->data="";
    this->target;
}
ArtistBST::~ArtistBST() {

}

void ArtistBST::insert(MusicQueueNode* queNode) {
    if(this->root==nullptr){
        ArtistBSTNode* treeNode=new ArtistBSTNode;
        root=treeNode;
    }
    else{

    }
}

void ArtistBST::search() {

}

void ArtistBST::print() {

}

void ArtistBST::delete_node() {

}