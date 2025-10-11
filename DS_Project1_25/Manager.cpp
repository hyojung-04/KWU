#include "Manager.h"
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include "MusicQueue.h"

using namespace std;

Manager::Manager()
{
    cout << "[DEBUG] Manager constructor called" << endl;
    this->flog.open("log.txt", ios::app);
    if (!this->flog.is_open())
    {
        cout << "[DEBUG] log open fail" << endl;
    }
    else
    {
        cout << "[DEBUG] log open success" << endl;
    }
}

Manager::~Manager()
{
    // close command file
    if (this->fcmd.is_open())
        this->fcmd.close();

    // close log file
    if (this->flog.is_open())
        this->flog.close();
}

void Manager::run(const char *command)
{
    cout << "[DEBUG] trying to open command file: " << command << endl;
    this->fcmd.open(command);

    if (!this->fcmd.is_open())
    {
        cout << "[DEBUG] failed to open command.txt" << endl;
        return;
    }
    cout << "[DEBUG] command.txt open success" << endl;

    // // open command file
    // this->fcmd.open(command);

    // // if fail to open
    // if (!this->fcmd.is_open()) {
    //     cout << "========ERROR========" << endl;
    //     cout << "100" << endl;
    //     cout << "=====================" << endl;

    //     flog << "========ERROR========" << endl;
    //     flog << "100" << endl;
    //     flog << "=====================" << endl;
    //     return;
    // }

    string line;
    // read each line
    while (getline(fcmd, line))
    {
        // skip empty line
        if (line.empty())
            continue;

        // split first word (command)
        stringstream ss(line);
        string cmd;
        ss >> cmd;

        // move stream back to start of args
        if (cmd.empty())
            continue;

        // check what command is
        if (cmd == "LOAD")
        {
            this->LOAD();
        }
        else if (cmd == "ADD")
        {
            this->ADD();
        }
        else if (cmd == "QPOP")
        {
            this->QPOP();
        }
        else if (cmd == "SEARCH")
        {
            this->SEARCH();
        }
        else if (cmd == "MAKEPL")
        {
            this->MAKEPL();
        }
        else if (cmd == "PRINT")
        {
            this->PRINT();
        }
        else if (cmd == "DELETE")
        {
            this->DELETE();
        }
        else if (cmd == "EXIT")
        {
            this->EXIT();
            break; // stop program
        }
        else
        {
            // wrong command
            cout << "========ERROR========" << endl;
            cout << "100" << endl;
            cout << "=====================" << endl;
            flog << "========ERROR========" << endl;
            flog << "100" << endl;
            flog << "=====================" << endl;
        }
    }

    // close command file
    this->fcmd.close();
}

void Manager::LOAD()
{
    // case: already loaded (queue not empty)
    if (!this->q.empty())
    {
        this->flog << "========ERROR========" << endl;
        this->flog << "100" << endl;
        this->flog << "=====================" << endl;
        return;
    }

    ifstream readFile;
    readFile.open("Music_List.txt");
    if (!readFile.is_open())
    {
        this->flog << "========ERROR========" << endl;
        this->flog << "100" << endl;
        this->flog << "=====================" << endl;
        return;
    }

    string line;
    bool hasData = false;
    while (getline(readFile, line))
    {
        if (line.empty())
            continue;
        this->q.push(line);
        hasData = true;
    }

    readFile.close();

    // case: file empty or no valid data
    if (!hasData)
    {
        this->flog << "========ERROR========" << endl;
        this->flog << "100" << endl;
        this->flog << "=====================" << endl;
        return;
    }

    // print loaded data
    this->flog << "========LOAD========" << endl;

    MusicQueueNode *cur = this->q.getHead();
    while (cur)
    {
        this->flog << cur->getArtist() << "/"
                   << cur->getTitle() << "/"
                   << cur->getRunTime() << endl;
        cur = cur->getNext();
    }

    this->flog << "=====================" << endl;
}

void Manager::ADD()
{
    // read one line from command file
    string line;
    getline(fcmd, line);

    // make log header
    flog << "========ADD========" << endl;

    // cut spaces after "ADD"
    if (line.size() <= 4)
    {
        // not enough info
        flog << "========ERROR========" << endl;
        flog << "200" << endl;
        flog << "=====================" << endl;
        cout << "========ERROR========" << endl;
        cout << "200" << endl;
        cout << "=====================" << endl;
        return;
    }

    string data = line.substr(4);
    if (!data.empty() && data[0] == ' ')
        data = data.substr(1);

    // find '|'
    size_t idx1 = data.find('|');
    size_t idx2 = data.find('|', idx1 + 1);

    // if missing '|'
    if (idx1 == string::npos || idx2 == string::npos)
    {
        flog << "========ERROR========" << endl;
        flog << "200" << endl;
        flog << "=====================" << endl;
        cout << "========ERROR========" << endl;
        cout << "200" << endl;
        cout << "=====================" << endl;
        return;
    }

    // split data
    string artist = data.substr(0, idx1);
    string title = data.substr(idx1 + 1, idx2 - idx1 - 1);
    string runtime = data.substr(idx2 + 1);

    // if any empty
    if (artist.empty() || title.empty() || runtime.empty())
    {
        flog << "========ERROR========" << endl;
        flog << "200" << endl;
        flog << "=====================" << endl;
        cout << "========ERROR========" << endl;
        cout << "200" << endl;
        cout << "=====================" << endl;
        return;
    }

    // check same song in queue
    if (this->q.exist(data))
    {
        flog << "========ERROR========" << endl;
        flog << "200" << endl;
        flog << "=====================" << endl;
        cout << "========ERROR========" << endl;
        cout << "200" << endl;
        cout << "=====================" << endl;
        return;
    }

    // add new song to queue
    this->q.push(data);

    // print success
    flog << artist << "/" << title << "/" << runtime << endl;
    flog << "=====================" << endl;

    cout << "========ADD========" << endl;
    cout << artist << "/" << title << "/" << runtime << endl;
    cout << "=====================" << endl;
}

void Manager::QPOP()
{
    // check if queue empty
    if (this->q.empty())
    {
        cout << "========ERROR========" << endl;
        cout << "300" << endl;
        cout << "=====================" << endl;
        flog << "========ERROR========" << endl;
        flog << "300" << endl;
        flog << "=====================" << endl;
        return;
    }

    // pop all songs from queue
    while (!this->q.empty())
    {
        // take one song
        MusicQueueNode *node = this->q.pop();
        if (node == nullptr)
            break;

        // make node for BST
        // insert into artist BST
        this->ab.insert(node);

        // insert into title BST
        this->tb.insert(node);

        // free node
        delete node;
    }

    // print success
    cout << "========QPOP========" << endl;
    cout << "Success" << endl;
    cout << "=====================" << endl;
    flog << "========QPOP========" << endl;
    flog << "Success" << endl;
    flog << "=====================" << endl;
}

void Manager::SEARCH()
{
    // read full command line
    string line;
    getline(fcmd, line);

    // cut spaces after "SEARCH"
    if (line.size() <= 6)
    {
        cout << "========ERROR========" << endl;
        cout << "400" << endl;
        cout << "=====================" << endl;
        flog << "========ERROR========" << endl;
        flog << "400" << endl;
        flog << "=====================" << endl;
        return;
    }

    string data = line.substr(6);
    if (!data.empty() && data[0] == ' ')
        data = data.substr(1);

    // split command parts
    stringstream ss(data);
    string option;
    ss >> option;

    // check if option missing
    if (option.empty())
    {
        cout << "========ERROR========" << endl;
        cout << "400" << endl;
        cout << "=====================" << endl;
        flog << "========ERROR========" << endl;
        flog << "400" << endl;
        flog << "=====================" << endl;
        return;
    }

    // case 1: ARTIST
    if (option == "ARTIST")
    {
        string artist;
        ss >> artist;
        if (artist.empty())
        {
            cout << "========ERROR========" << endl;
            cout << "400" << endl;
            cout << "=====================" << endl;
            flog << "========ERROR========" << endl;
            flog << "400" << endl;
            flog << "=====================" << endl;
            return;
        }

        ArtistBSTNode *found = this->ab.search(artist);
        if (!found)
        {
            cout << "========ERROR========" << endl;
            cout << "400" << endl;
            cout << "=====================" << endl;
            flog << "========ERROR========" << endl;
            flog << "400" << endl;
            flog << "=====================" << endl;
            return;
        }

        // print songs of artist
        cout << "========SEARCH========" << endl;
        flog << "========SEARCH========" << endl;

        vector<string> titles = found->getTitleList();
        vector<int> rt = found->getRtList();

        for (size_t i = 0; i < titles.size(); i++)
        {
            int min = rt[i] / 60;
            int sec = rt[i] % 60;

            cout << artist << "/" << titles[i] << "/";
            flog << artist << "/" << titles[i] << "/";

            if (sec < 10)
            {
                cout << min << ":0" << sec << endl;
                flog << min << ":0" << sec << endl;
            }
            else
            {
                cout << min << ":" << sec << endl;
                flog << min << ":" << sec << endl;
            }
        }

        cout << "=====================" << endl;
        flog << "=====================" << endl;
        return;
    }

    // case 2: TITLE
    else if (option == "TITLE")
    {
        string title;
        ss >> ws;
        getline(ss, title);
        if (title.empty())
        {
            cout << "========ERROR========" << endl;
            cout << "400" << endl;
            cout << "=====================" << endl;
            flog << "========ERROR========" << endl;
            flog << "400" << endl;
            flog << "=====================" << endl;
            return;
        }

        TitleBSTNode *found = this->tb.search(title);
        if (!found)
        {
            cout << "========ERROR========" << endl;
            cout << "400" << endl;
            cout << "=====================" << endl;
            flog << "========ERROR========" << endl;
            flog << "400" << endl;
            flog << "=====================" << endl;
            return;
        }

        cout << "========SEARCH========" << endl;
        flog << "========SEARCH========" << endl;

        const vector<string> &artists = found->getArtistList();
        const vector<int> &rt = found->getRtList();

        for (size_t i = 0; i < artists.size(); i++)
        {
            int min = rt[i] / 60;
            int sec = rt[i] % 60;

            cout << artists[i] << "/" << title << "/";
            flog << artists[i] << "/" << title << "/";

            if (sec < 10)
            {
                cout << min << ":0" << sec << endl;
                flog << min << ":0" << sec << endl;
            }
            else
            {
                cout << min << ":" << sec << endl;
                flog << min << ":" << sec << endl;
            }
        }

        cout << "=====================" << endl;
        flog << "=====================" << endl;
        return;
    }

    // case 3: SONG
    else if (option == "SONG")
    {
        string songData;
        ss >> ws;
        getline(ss, songData);
        if (songData.empty())
        {
            cout << "========ERROR========" << endl;
            cout << "400" << endl;
            cout << "=====================" << endl;
            flog << "========ERROR========" << endl;
            flog << "400" << endl;
            flog << "=====================" << endl;
            return;
        }

        // split artist|title
        size_t idx = songData.find('|');
        if (idx == string::npos)
        {
            cout << "========ERROR========" << endl;
            cout << "400" << endl;
            cout << "=====================" << endl;
            flog << "========ERROR========" << endl;
            flog << "400" << endl;
            flog << "=====================" << endl;
            return;
        }

        string artist = songData.substr(0, idx);
        string title = songData.substr(idx + 1);

        // find artist first
        ArtistBSTNode *aNode = this->ab.search(artist);
        bool found = false;

        if (aNode)
        {
            vector<string> titles = aNode->getTitleList();
            vector<int> rt = aNode->getRtList();
            for (size_t i = 0; i < titles.size(); i++)
            {
                if (titles[i] == title)
                {
                    int min = rt[i] / 60;
                    int sec = rt[i] % 60;

                    cout << "========SEARCH========" << endl;
                    flog << "========SEARCH========" << endl;

                    cout << artist << "/" << title << "/";
                    flog << artist << "/" << title << "/";

                    if (sec < 10)
                    {
                        cout << min << ":0" << sec << endl;
                        flog << min << ":0" << sec << endl;
                    }
                    else
                    {
                        cout << min << ":" << sec << endl;
                        flog << min << ":" << sec << endl;
                    }

                    cout << "=====================" << endl;
                    flog << "=====================" << endl;
                    found = true;
                    break;
                }
            }
        }

        if (!found)
        {
            cout << "========ERROR========" << endl;
            cout << "400" << endl;
            cout << "=====================" << endl;
            flog << "========ERROR========" << endl;
            flog << "400" << endl;
            flog << "=====================" << endl;
        }
        return;
    }

    // wrong option
    else
    {
        cout << "========ERROR========" << endl;
        cout << "400" << endl;
        cout << "=====================" << endl;
        flog << "========ERROR========" << endl;
        flog << "400" << endl;
        flog << "=====================" << endl;
        return;
    }
}

void Manager::MAKEPL()
{
    // read command line
    string line;
    getline(fcmd, line);

    // remove spaces
    if (line.size() <= 7)
    {
        cout << "========ERROR========" << endl;
        cout << "500" << endl;
        cout << "=====================" << endl;
        flog << "========ERROR========" << endl;
        flog << "500" << endl;
        flog << "=====================" << endl;
        return;
    }

    string data = line.substr(7);
    if (!data.empty() && data[0] == ' ')
        data = data.substr(1);

    // split first word (option)
    stringstream ss(data);
    string option;
    ss >> option;

    // no option
    if (option.empty())
    {
        cout << "========ERROR========" << endl;
        cout << "500" << endl;
        cout << "=====================" << endl;
        flog << "========ERROR========" << endl;
        flog << "500" << endl;
        flog << "=====================" << endl;
        return;
    }

    cout << "========MAKEPL========" << endl;
    flog << "========MAKEPL========" << endl;

    // case 1: ARTIST
    if (option == "ARTIST")
    {
        string artist;
        ss >> artist;
        if (artist.empty())
        {
            cout << "========ERROR========" << endl;
            cout << "500" << endl;
            cout << "=====================" << endl;
            flog << "========ERROR========" << endl;
            flog << "500" << endl;
            flog << "=====================" << endl;
            return;
        }

        ArtistBSTNode *found = this->ab.search(artist);
        if (!found)
        {
            cout << "========ERROR========" << endl;
            cout << "500" << endl;
            cout << "=====================" << endl;
            flog << "========ERROR========" << endl;
            flog << "500" << endl;
            flog << "=====================" << endl;
            return;
        }

        vector<string> titles = found->getTitleList();
        vector<int> rt = found->getRtList();

        // space check
        if (this->pl.full())
        {
            cout << "========ERROR========" << endl;
            cout << "500" << endl;
            cout << "=====================" << endl;
            flog << "========ERROR========" << endl;
            flog << "500" << endl;
            flog << "=====================" << endl;
            return;
        }

        // add songs to playlist
        for (size_t i = 0; i < titles.size(); i++)
        {
            stringstream s;
            int min = rt[i] / 60;
            int sec = rt[i] % 60;
            s << artist << "|" << titles[i] << "|";
            if (sec < 10)
                s << min << ":0" << sec;
            else
                s << min << ":" << sec;
            this->pl.insert_node(s.str());

            cout << artist << "/" << titles[i] << "/";
            flog << artist << "/" << titles[i] << "/";
            if (sec < 10)
            {
                cout << min << ":0" << sec << endl;
                flog << min << ":0" << sec << endl;
            }
            else
            {
                cout << min << ":" << sec << endl;
                flog << min << ":" << sec << endl;
            }
        }

        // print count and time
        string result = this->pl.print();
        cout << result;
        flog << result;
        cout << "=====================" << endl;
        flog << "=====================" << endl;
        return;
    }

    // case 2: TITLE
    else if (option == "TITLE")
    {
        string title;
        ss >> ws;
        getline(ss, title);
        if (title.empty())
        {
            cout << "========ERROR========" << endl;
            cout << "500" << endl;
            cout << "=====================" << endl;
            flog << "========ERROR========" << endl;
            flog << "500" << endl;
            flog << "=====================" << endl;
            return;
        }

        TitleBSTNode *found = this->tb.search(title);
        if (!found)
        {
            cout << "========ERROR========" << endl;
            cout << "500" << endl;
            cout << "=====================" << endl;
            flog << "========ERROR========" << endl;
            flog << "500" << endl;
            flog << "=====================" << endl;
            return;
        }

        const vector<string> &artists = found->getArtistList();
        const vector<int> &rt = found->getRtList();

        if (this->pl.full() || (this->pl.run_time() + rt.size()) > 10)
        {
            cout << "========ERROR========" << endl;
            cout << "500" << endl;
            cout << "=====================" << endl;
            flog << "========ERROR========" << endl;
            flog << "500" << endl;
            flog << "=====================" << endl;
            return;
        }

        for (size_t i = 0; i < artists.size(); i++)
        {
            stringstream s;
            int min = rt[i] / 60;
            int sec = rt[i] % 60;
            s << artists[i] << "|" << title << "|";
            if (sec < 10)
                s << min << ":0" << sec;
            else
                s << min << ":" << sec;
            this->pl.insert_node(s.str());

            cout << artists[i] << "/" << title << "/";
            flog << artists[i] << "/" << title << "/";
            if (sec < 10)
            {
                cout << min << ":0" << sec << endl;
                flog << min << ":0" << sec << endl;
            }
            else
            {
                cout << min << ":" << sec << endl;
                flog << min << ":" << sec << endl;
            }
        }

        string result = this->pl.print();
        cout << result;
        flog << result;
        cout << "=====================" << endl;
        flog << "=====================" << endl;
        return;

        
    }

    // case 3: SONG
    else if (option == "SONG")
    {
        string song;
        ss >> ws;
        getline(ss, song);
        if (song.empty())
        {
            cout << "========ERROR========" << endl;
            cout << "500" << endl;
            cout << "=====================" << endl;
            flog << "========ERROR========" << endl;
            flog << "500" << endl;
            flog << "=====================" << endl;
            return;
        }

        size_t pos = song.find('|');
        if (pos == string::npos)
        {
            cout << "========ERROR========" << endl;
            cout << "500" << endl;
            cout << "=====================" << endl;
            flog << "========ERROR========" << endl;
            flog << "500" << endl;
            flog << "=====================" << endl;
            return;
        }

        string artist = song.substr(0, pos);
        string title = song.substr(pos + 1);

        ArtistBSTNode *foundA = this->ab.search(artist);
        bool ok = false;

        if (foundA)
        {
            vector<string> titles = foundA->getTitleList();
            vector<int> rt = foundA->getRtList();
            for (size_t i = 0; i < titles.size(); i++)
            {
                if (titles[i] == title)
                {
                    int min = rt[i] / 60;
                    int sec = rt[i] % 60;
                    stringstream s;
                    s << artist << "|" << title << "|";
                    if (sec < 10)
                        s << min << ":0" << sec;
                    else
                        s << min << ":" << sec;
                    this->pl.insert_node(s.str());

                    cout << artist << "/" << title << "/";
                    flog << artist << "/" << title << "/";
                    if (sec < 10)
                    {
                        cout << min << ":0" << sec << endl;
                        flog << min << ":0" << sec << endl;
                    }
                    else
                    {
                        cout << min << ":" << sec << endl;
                        flog << min << ":" << sec << endl;
                    }
                    ok = true;
                    break;
                }
            }
        }

        if (!ok)
        {
            cout << "========ERROR========" << endl;
            cout << "500" << endl;
            cout << "=====================" << endl;
            flog << "========ERROR========" << endl;
            flog << "500" << endl;
            flog << "=====================" << endl;
            return;
        }

        string res = this->pl.print();
        cout << res;
        flog << res;
        cout << "=====================" << endl;
        flog << "=====================" << endl;
        return;
    }

    // wrong option
    else
    {
        cout << "========ERROR========" << endl;
        cout << "500" << endl;
        cout << "=====================" << endl;
        flog << "========ERROR========" << endl;
        flog << "500" << endl;
        flog << "=====================" << endl;
    }
}

void Manager::PRINT()
{
    // read one line from command file
    string line;
    getline(fcmd, line);

    // remove spaces
    if (line.size() <= 5)
    {
        cout << "========ERROR========" << endl;
        cout << "600" << endl;
        cout << "=====================" << endl;
        flog << "========ERROR========" << endl;
        flog << "600" << endl;
        flog << "=====================" << endl;
        return;
    }

    string data = line.substr(5);
    if (!data.empty() && data[0] == ' ')
        data = data.substr(1);

    // split option
    stringstream ss(data);
    string option;
    ss >> option;

    // no option
    if (option.empty())
    {
        cout << "========ERROR========" << endl;
        cout << "600" << endl;
        cout << "=====================" << endl;
        flog << "========ERROR========" << endl;
        flog << "600" << endl;
        flog << "=====================" << endl;
        return;
    }

    // case 1: ARTIST
    if (option == "ARTIST")
    {
        // check if empty
        // ArtistBST::print() already handles empty (prints error 600)
        this->ab.print();
        return;
    }

    // case 2: TITLE
    else if (option == "TITLE")
    {
        this->tb.print();
        return;
    }

    // case 3: LIST
    else if (option == "LIST")
    {
        if (this->pl.empty())
        {
            cout << "========ERROR========" << endl;
            cout << "600" << endl;
            cout << "=====================" << endl;
            flog << "========ERROR========" << endl;
            flog << "600" << endl;
            flog << "=====================" << endl;
            return;
        }

        cout << "========PRINT========" << endl;
        cout << this->pl.print();
        cout << "=====================" << endl;

        flog << "========PRINT========" << endl;
        flog << this->pl.print();
        flog << "=====================" << endl;
        return;
    }

    // wrong option
    else
    {
        cout << "========ERROR========" << endl;
        cout << "600" << endl;
        cout << "=====================" << endl;
        flog << "========ERROR========" << endl;
        flog << "600" << endl;
        flog << "=====================" << endl;
    }
}

void Manager::DELETE()
{
    // read command line
    string line;
    getline(fcmd, line);

    // cut spaces
    if (line.size() <= 6)
    {
        cout << "========ERROR========" << endl;
        cout << "700" << endl;
        cout << "=====================" << endl;
        flog << "========ERROR========" << endl;
        flog << "700" << endl;
        flog << "=====================" << endl;
        return;
    }

    string data = line.substr(6);
    if (!data.empty() && data[0] == ' ')
        data = data.substr(1);

    // split option
    stringstream ss(data);
    string option;
    ss >> option;

    // no option
    if (option.empty())
    {
        cout << "========ERROR========" << endl;
        cout << "700" << endl;
        cout << "=====================" << endl;
        flog << "========ERROR========" << endl;
        flog << "700" << endl;
        flog << "=====================" << endl;
        return;
    }

    // case 1: ARTIST
    if (option == "ARTIST")
    {
        string artist;
        ss >> artist;
        if (artist.empty())
        {
            cout << "========ERROR========" << endl;
            cout << "700" << endl;
            cout << "=====================" << endl;
            flog << "========ERROR========" << endl;
            flog << "700" << endl;
            flog << "=====================" << endl;
            return;
        }

        ArtistBSTNode *found = this->ab.search(artist);
        if (!found)
        {
            cout << "========ERROR========" << endl;
            cout << "700" << endl;
            cout << "=====================" << endl;
            flog << "========ERROR========" << endl;
            flog << "700" << endl;
            flog << "=====================" << endl;
            return;
        }

        // copy current data
        vector<string> titles = found->getTitleList();

        // delete from ArtistBST
        this->ab.delete_node(artist);

        // delete from PlayList and TitleBST
        for (auto &t : titles)
        {
            this->tb.deleteNode(t);
            string songLine = artist + "|" + t;
            this->pl.delete_node(songLine);
        }

        cout << "========DELETE========" << endl;
        cout << "Success" << endl;
        cout << "=====================" << endl;

        flog << "========DELETE========" << endl;
        flog << "Success" << endl;
        flog << "=====================" << endl;
        return;
    }

    // case 2: TITLE
    else if (option == "TITLE")
    {
        string title;
        ss >> ws;
        getline(ss, title);
        if (title.empty())
        {
            cout << "========ERROR========" << endl;
            cout << "700" << endl;
            cout << "=====================" << endl;
            flog << "========ERROR========" << endl;
            flog << "700" << endl;
            flog << "=====================" << endl;
            return;
        }

        TitleBSTNode *found = this->tb.search(title);
        if (!found)
        {
            cout << "========ERROR========" << endl;
            cout << "700" << endl;
            cout << "=====================" << endl;
            flog << "========ERROR========" << endl;
            flog << "700" << endl;
            flog << "=====================" << endl;
            return;
        }

        const vector<string> &artists = found->getArtistList();

        // delete from TitleBST
        this->tb.deleteNode(title);

        // delete from ArtistBST and PlayList
        for (auto &a : artists)
        {
            this->ab.delete_node(a);
            string songLine = a + "|" + title;
            this->pl.delete_node(songLine);
        }

        cout << "========DELETE========" << endl;
        cout << "Success" << endl;
        cout << "=====================" << endl;

        flog << "========DELETE========" << endl;
        flog << "Success" << endl;
        flog << "=====================" << endl;
        return;
    }

    // case 3: LIST
    else if (option == "LIST")
    {
        string song;
        ss >> ws;
        getline(ss, song);
        if (song.empty())
        {
            cout << "========ERROR========" << endl;
            cout << "700" << endl;
            cout << "=====================" << endl;
            flog << "========ERROR========" << endl;
            flog << "700" << endl;
            flog << "=====================" << endl;
            return;
        }

        // only delete from PlayList
        this->pl.delete_node(song);

        cout << "========DELETE========" << endl;
        cout << "Success" << endl;
        cout << "=====================" << endl;

        flog << "========DELETE========" << endl;
        flog << "Success" << endl;
        flog << "=====================" << endl;
        return;
    }

    // case 4: SONG
    else if (option == "SONG")
    {
        string song;
        ss >> ws;
        getline(ss, song);
        if (song.empty())
        {
            cout << "========ERROR========" << endl;
            cout << "700" << endl;
            cout << "=====================" << endl;
            flog << "========ERROR========" << endl;
            flog << "700" << endl;
            flog << "=====================" << endl;
            return;
        }

        size_t pos = song.find('|');
        if (pos == string::npos)
        {
            cout << "========ERROR========" << endl;
            cout << "700" << endl;
            cout << "=====================" << endl;
            flog << "========ERROR========" << endl;
            flog << "700" << endl;
            flog << "=====================" << endl;
            return;
        }

        string artist = song.substr(0, pos);
        string title = song.substr(pos + 1);

        ArtistBSTNode *foundA = this->ab.search(artist);
        TitleBSTNode *foundT = this->tb.search(title);

        if (!foundA && !foundT)
        {
            cout << "========ERROR========" << endl;
            cout << "700" << endl;
            cout << "=====================" << endl;
            flog << "========ERROR========" << endl;
            flog << "700" << endl;
            flog << "=====================" << endl;
            return;
        }

        // remove song title from that artist node
        if (foundA)
        {
            // simple remove from artist node
            vector<string> tlist = foundA->getTitleList();
            vector<int> rlist = foundA->getRtList();
            for (size_t i = 0; i < tlist.size(); i++)
            {
                if (tlist[i] == title)
                {
                    tlist.erase(tlist.begin() + i);
                    rlist.erase(rlist.begin() + i);
                    break;
                }
            }
            foundA->setTitleList(tlist);
            foundA->setRtList(rlist);
        }

        if (foundT)
        {
            // simple remove from title node
            vector<string> alist = foundT->getArtistList();
            vector<int> rlist = foundT->getRtList();
            for (size_t i = 0; i < alist.size(); i++)
            {
                if (alist[i] == artist)
                {
                    alist.erase(alist.begin() + i);
                    rlist.erase(rlist.begin() + i);
                    break;
                }
            }
            foundT->setArtistList(alist);
            foundT->setRtList(rlist);
        }

        this->pl.delete_node(song);

        cout << "========DELETE========" << endl;
        cout << "Success" << endl;
        cout << "=====================" << endl;

        flog << "========DELETE========" << endl;
        flog << "Success" << endl;
        flog << "=====================" << endl;
        return;
    }

    // wrong option
    else
    {
        cout << "========ERROR========" << endl;
        cout << "700" << endl;
        cout << "=====================" << endl;
        flog << "========ERROR========" << endl;
        flog << "700" << endl;
        flog << "=====================" << endl;
    }
}

void Manager::EXIT()
{
    // print success
    cout << "========EXIT========" << endl;
    cout << "Success" << endl;
    cout << "=====================" << endl;

    flog << "========EXIT========" << endl;
    flog << "Success" << endl;
    flog << "=====================" << endl;
}
