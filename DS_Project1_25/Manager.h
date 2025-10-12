#pragma once
#include "MusicQueue.h"
#include "TitleBST.h"
#include "ArtistBST.h"
#include "PlayList.h"
#include <fstream>

using namespace std;

class Manager {
private:
	MusicQueue q;
	ArtistBST ab;
	TitleBST tb;
	PlayList pl;
	ifstream fcmd;
	ofstream flog;
public:
	Manager();
	~Manager();

	void run(const char* command);

	void LOAD();
	void ADD(const std::string& data_in);
	void QPOP();
	void SEARCH(const std::string& rest);
	void MAKEPL(const std::string& rest);
	void PRINT(const std::string& rest);
	void DELETE(const std::string& rest);
	void EXIT();

};