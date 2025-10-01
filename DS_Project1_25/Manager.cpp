#include "Manager.h"
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include "MusicQueue.h"

using namespace std;

Manager::Manager() {

}

Manager::~Manager() {

}

void Manager::run(const char* command) {
	
}

void Manager::LOAD() {
    MusicQueue que;//insert music list in que

	std::ifstream readFile;
    readFile.open("Music_List.txt");
    if(!readFile.is_open()){
        std::cout<<"파일을 열 수 없습니다."<<endl;
    }

    string line;
    while(getline(readFile, line)){
        que.push(line);
    }


    readFile.close();
}

void Manager::ADD() {
	
}

void Manager::QPOP() {
	
}

void Manager::SEARCH() {
	
}

void Manager::MAKEPL() {
	
}

void Manager::PRINT() {
	
}

void Manager::DELETE() {
	
}

void Manager::EXIT() {

}