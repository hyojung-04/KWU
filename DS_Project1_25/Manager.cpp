#include "Manager.h"
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>
#include <functional>
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
    // reset log at start (once per run)
    if (this->flog.is_open())
        this->flog.close();
    this->flog.open("log.txt");
    // open command file
    this->fcmd.open(command);

    // if fail to open
    if (!this->fcmd.is_open())
    {
        cout << "========ERROR========" << endl;
        cout << "100" << endl;
        cout << "====================" << endl;

        flog << "========ERROR========" << endl;
        flog << "100" << endl;
        flog << "====================" << endl;
        return;
    }

    std::string line;
    while (std::getline(this->fcmd, line))
    {
        if (line.empty())
            continue;

        // split command and rest
        // ex) "ADD KDA|pop stars|3:23"
        size_t sp = line.find(' ');
        string cmd = (sp == std::string::npos) ? line : line.substr(0, sp);
        string rest = (sp == std::string::npos) ? "" : line.substr(sp + 1);

        if (cmd == "LOAD")
        {
            this->LOAD();
        }
        else if (cmd == "ADD")
        {
            this->ADD(rest); // pass whole "KDA|pop stars|3:23"
        }
        else if (cmd == "QPOP")
        {
            this->QPOP();
        }
        else if (cmd == "SEARCH")
        {
            this->SEARCH(rest); // pass search key/options
        }
        else if (cmd == "MAKEPL")
        {
            this->MAKEPL(rest); // pass makepl key/options
        }
        else if (cmd == "PRINT")
        {
            this->PRINT(rest);
        }
        else if (cmd == "DELETE")
        {
            this->DELETE(rest); // pass delete target/options
        }
        else if (cmd == "EXIT")
        {
            this->EXIT();
            break;
        }
        else
        {
            std::cout << "========ERROR========\n1000\n====================\n";
            this->flog << "========ERROR========\n1000\n====================\n";
        }
    }
    this->fcmd.close();
}

void Manager::LOAD()
{
    // case: already loaded (queue not empty)
    if (!this->q.empty())
    {
        this->flog << "========ERROR========" << endl;
        this->flog << "100" << endl;
        this->flog << "====================" << endl;
        return;
    }

    ifstream readFile;
    readFile.open("Music_List.txt");
    if (!readFile.is_open())
    {
        this->flog << "========ERROR========" << endl;
        this->flog << "100" << endl;
        this->flog << "====================" << endl;
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
        this->flog << "====================" << endl;
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

    this->flog << "====================" << endl;
}

void Manager::ADD(const std::string &data_in)
{
    // do not read file here
    // do not print ADD header before validation

    // 1) basic check
    if (data_in.empty())
    {
        std::cout << "========ERROR========\n200\n====================\n";
        this->flog << "========ERROR========\n200\n====================\n";
        return;
    }

    // 2) trim one leading space if exists (from "ADD <data>")
    std::string data = data_in;
    if (!data.empty() && data[0] == ' ')
        data = data.substr(1);

    // 3) split by '|'
    size_t p1 = data.find('|');
    size_t p2 = (p1 == std::string::npos) ? std::string::npos : data.find('|', p1 + 1);
    if (p1 == std::string::npos || p2 == std::string::npos)
    {
        std::cout << "========ERROR========\n200\n====================\n";
        this->flog << "========ERROR========\n200\n====================\n";
        return;
    }

    std::string artist = data.substr(0, p1);
    std::string title = data.substr(p1 + 1, p2 - p1 - 1);
    std::string runtime = data.substr(p2 + 1);

    if (artist.empty() || title.empty() || runtime.empty())
    {
        std::cout << "========ERROR========\n200\n====================\n";
        this->flog << "========ERROR========\n200\n====================\n";
        return;
    }

    // 4) duplicate check (your exist() compares artist+title)
    if (this->q.exist(artist + "|" + title + "|" + runtime))
    {
        std::cout << "========ERROR========\n200\n====================\n";
        this->flog << "========ERROR========\n200\n====================\n";
        return;
    }

    // 5) push to queue
    this->q.push(artist + "|" + title + "|" + runtime);

    // 6) success print (both)
    std::cout << "========ADD========\n"
              << artist << "/" << title << "/" << runtime << "\n"
              << "====================\n";

    this->flog << "========ADD========\n"
               << artist << "/" << title << "/" << runtime << "\n"
               << "====================\n";
}

void Manager::QPOP()
{
    // check if queue empty
    if (this->q.empty())
    {
        cout << "========ERROR========" << endl;
        cout << "300" << endl;
        cout << "====================" << endl;
        flog << "========ERROR========" << endl;
        flog << "300" << endl;
        flog << "====================" << endl;
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
    cout << "====================" << endl;
    flog << "========QPOP========" << endl;
    flog << "Success" << endl;
    flog << "====================" << endl;
}

void Manager::SEARCH(const std::string &rest)
{
    // 0) basic check
    if (rest.empty())
    {
        std::cout << "========ERROR========\n400\n====================\n";
        this->flog << "========ERROR========\n400\n====================\n";
        return;
    }

    // 1) split option and arg (keep the whole arg as-is)
    std::string option, arg;
    size_t sp = rest.find(' ');
    option = (sp == std::string::npos) ? rest : rest.substr(0, sp);
    arg = (sp == std::string::npos) ? "" : rest.substr(sp + 1);
    if (!arg.empty() && arg[0] == ' ')
        arg.erase(0, 1);
    if (option.empty() || arg.empty())
    {
        std::cout << "========ERROR========\n400\n====================\n";
        this->flog << "========ERROR========\n400\n====================\n";
        return;
    }

    // 2) ARTIST
    if (option == "ARTIST")
    {
        std::string artist = arg; // allow spaces
        ArtistBSTNode *found = this->ab.search(artist);
        if (!found)
        {
            std::cout << "========ERROR========\n400\n====================\n";
            this->flog << "========ERROR========\n400\n====================\n";
            return;
        }

        std::cout << "========SEARCH========\n";
        this->flog << "========SEARCH========\n";

        std::vector<std::string> titles = found->getTitleList();
        std::vector<int> rt = found->getRtList();
        for (size_t i = 0; i < titles.size(); ++i)
        {
            int m = rt[i] / 60, s = rt[i] % 60;
            std::cout << artist << "/" << titles[i] << "/" << m << ":" << (s < 10 ? "0" : "") << s << "\n";
            this->flog << artist << "/" << titles[i] << "/" << m << ":" << (s < 10 ? "0" : "") << s << "\n";
        }

        std::cout << "====================\n";
        this->flog << "====================\n";
        return;
    }

    // 3) TITLE
    if (option == "TITLE")
    {
        std::string title = arg; // allow spaces
        TitleBSTNode *found = this->tb.search(title);
        if (!found)
        {
            std::cout << "========ERROR========\n400\n====================\n";
            this->flog << "========ERROR========\n400\n====================\n";
            return;
        }

        std::cout << "========SEARCH========\n";
        this->flog << "========SEARCH========\n";

        const std::vector<std::string> &artists = found->getArtistList();
        const std::vector<int> &rt = found->getRtList();
        for (size_t i = 0; i < artists.size(); ++i)
        {
            int m = rt[i] / 60, s = rt[i] % 60;
            std::cout << artists[i] << "/" << title << "/" << m << ":" << (s < 10 ? "0" : "") << s << "\n";
            this->flog << artists[i] << "/" << title << "/" << m << ":" << (s < 10 ? "0" : "") << s << "\n";
        }

        std::cout << "====================\n";
        this->flog << "====================\n";
        return;
    }

    // 4) SONG
    if (option == "SONG")
    {
        std::string song = arg; // format: artist|title
        size_t bar = song.find('|');
        if (bar == std::string::npos)
        {
            std::cout << "========ERROR========\n400\n====================\n";
            this->flog << "========ERROR========\n400\n====================\n";
            return;
        }
        std::string artist = song.substr(0, bar);
        std::string title = song.substr(bar + 1);

        ArtistBSTNode *aNode = this->ab.search(artist);
        if (!aNode)
        {
            std::cout << "========ERROR========\n400\n====================\n";
            this->flog << "========ERROR========\n400\n====================\n";
            return;
        }

        const std::vector<std::string> titles = aNode->getTitleList();
        const std::vector<int> rt = aNode->getRtList();

        for (size_t i = 0; i < titles.size(); ++i)
        {
            if (titles[i] == title)
            {
                int m = rt[i] / 60, s = rt[i] % 60;
                std::cout << "========SEARCH========\n";
                this->flog << "========SEARCH========\n";
                std::cout << artist << "/" << title << "/" << m << ":" << (s < 10 ? "0" : "") << s << "\n";
                this->flog << artist << "/" << title << "/" << m << ":" << (s < 10 ? "0" : "") << s << "\n";
                std::cout << "====================\n";
                this->flog << "====================\n";
                return;
            }
        }

        // not found in this artist
        std::cout << "========ERROR========\n400\n====================\n";
        this->flog << "========ERROR========\n400\n====================\n";
        return;
    }

    // 5) wrong option
    std::cout << "========ERROR========\n400\n====================\n";
    this->flog << "========ERROR========\n400\n====================\n";
}

void Manager::MAKEPL(const std::string &rest)
{
    // 0) basic validation
    if (rest.empty())
    {
        std::cout << "========ERROR========\n500\n====================\n";
        this->flog << "========ERROR========\n500\n====================\n";
        return;
    }

    // 1) split option and arg
    std::string option, arg;
    size_t sp = rest.find(' ');
    option = (sp == std::string::npos) ? rest : rest.substr(0, sp);
    arg = (sp == std::string::npos) ? "" : rest.substr(sp + 1);
    if (!arg.empty() && arg[0] == ' ')
        arg.erase(0, 1);
    if (option.empty() || arg.empty())
    {
        std::cout << "========ERROR========\n500\n====================\n";
        this->flog << "========ERROR========\n500\n====================\n";
        return;
    }

    // 2) helper: seconds -> "m:ss"
    auto mmss = [](int sec) -> std::string
    {
        int m = sec / 60, s = sec % 60;
        std::ostringstream os;
        os << m << ":" << (s < 10 ? "0" : "") << s;
        return os.str();
    };

    // 3) gather candidates from BSTs
    struct Song
    {
        std::string artist;
        std::string title;
        std::string rtStr;
    };
    std::vector<Song> toAdd;

    if (option == "ARTIST")
    {
        std::string artist = arg;
        ArtistBSTNode *node = this->ab.search(artist);
        if (!node)
        {
            std::cout << "========ERROR========\n500\n====================\n";
            this->flog << "========ERROR========\n500\n====================\n";
            return;
        }
        std::vector<std::string> titles = node->getTitleList();
        std::vector<int> rts = node->getRtList();
        for (size_t i = 0; i < titles.size(); ++i)
        {
            toAdd.push_back({artist, titles[i], mmss(rts[i])});
        }
    }
    else if (option == "TITLE")
    {
        std::string title = arg;
        TitleBSTNode *node = this->tb.search(title);
        if (!node)
        {
            std::cout << "========ERROR========\n500\n====================\n";
            this->flog << "========ERROR========\n500\n====================\n";
            return;
        }
        const std::vector<std::string> &artists = node->getArtistList();
        const std::vector<int> &rts = node->getRtList();
        for (size_t i = 0; i < artists.size(); ++i)
        {
            toAdd.push_back({artists[i], title, mmss(rts[i])});
        }
    }
    else if (option == "SONG")
    {
        size_t bar = arg.find('|');
        if (bar == std::string::npos)
        {
            std::cout << "========ERROR========\n500\n====================\n";
            this->flog << "========ERROR========\n500\n====================\n";
            return;
        }
        std::string artist = arg.substr(0, bar);
        std::string title = arg.substr(bar + 1);

        ArtistBSTNode *node = this->ab.search(artist);
        if (!node)
        {
            std::cout << "========ERROR========\n500\n====================\n";
            this->flog << "========ERROR========\n500\n====================\n";
            return;
        }
        std::vector<std::string> titles = node->getTitleList();
        std::vector<int> rts = node->getRtList();

        bool found = false;
        for (size_t i = 0; i < titles.size(); ++i)
        {
            if (titles[i] == title)
            {
                toAdd.push_back({artist, title, mmss(rts[i])});
                found = true;
                break;
            }
        }
        if (!found)
        {
            std::cout << "========ERROR========\n500\n====================\n";
            this->flog << "========ERROR========\n500\n====================\n";
            return;
        }
    }
    else
    {
        std::cout << "========ERROR========\n500\n====================\n";
        this->flog << "========ERROR========\n500\n====================\n";
        return;
    }

    if (toAdd.empty())
    {
        std::cout << "========ERROR========\n500\n====================\n";
        this->flog << "========ERROR========\n500\n====================\n";
        return;
    }

    // 4) strict duplicate policy (if any exists -> fail all)
    for (const auto &s : toAdd)
    {
        // must pass "artist|title|something"
        std::ostringstream key;
        key << s.artist << "|" << s.title << "|" << s.rtStr; // same as insert format
        if (this->pl.exist(key.str()))
        {
            std::cout << "========ERROR========\n500\n====================\n";
            this->flog << "========ERROR========\n500\n====================\n";
            return;
        }
    }

    // 5) header
    std::cout << "========MAKEPL========\n";
    this->flog << "========MAKEPL========\n";

    // 6) insert each, echo only the added lines (NO playlist reprint)
    for (const auto &s : toAdd)
    {
        if (this->pl.full())
        {
            // if full in the middle, still show only Count/Time (no list duplication)
            std::string all = this->pl.print(); // contains whole list + summary
            std::istringstream iss(all);
            std::string line, last1, last2;
            while (std::getline(iss, line))
            { // keep last two lines
                last2 = last1;
                last1 = line;
            }
            std::cout << last2 << "\n"
                      << last1 << "\n";
            this->flog << last2 << "\n"
                       << last1 << "\n";
            std::cout << "====================\n";
            this->flog << "====================\n";
            return;
        }

        std::ostringstream rec; // "artist|title|m:ss"
        rec << s.artist << "|" << s.title << "|" << s.rtStr;
        this->pl.insert_node(rec.str());

        // echo just-added song
        std::cout << s.artist << "/" << s.title << "/" << s.rtStr << "\n";
        this->flog << s.artist << "/" << s.title << "/" << s.rtStr << "\n";
    }

    // 7) print ONLY Count/Time from pl.print()
    std::string all = this->pl.print(); // full text (list + summary)
    std::istringstream iss(all);
    std::string line, last1, last2;
    while (std::getline(iss, line))
    {
        last2 = last1;
        last1 = line;
    }
    std::cout << last2 << "\n"
              << last1 << "\n";
    this->flog << last2 << "\n"
               << last1 << "\n";
    std::cout << "====================\n";
    this->flog << "====================\n";
}

void Manager::PRINT(const std::string &rest)
{
    std::string data = rest;
    if (!data.empty() && data[0] == ' ')
        data = data.substr(1);

    if (data.empty())
    {
        std::cout << "========ERROR========\n600\n====================\n";
        this->flog << "========ERROR========\n600\n====================\n";
        return;
    }

    std::stringstream ss(data);
    std::string option;
    ss >> option;

    // helper: capture cout during a function call, then write to cout + flog
    auto capture_and_dual_write = [&](const std::function<void()> &fn_call)
    {
        std::ostringstream cap;
        std::streambuf *old = std::cout.rdbuf(cap.rdbuf()); // redirect cout to cap
        fn_call();                                          // call function that prints to cout
        std::cout.rdbuf(old);                               // restore cout

        const std::string out = cap.str(); // same text captured
        std::cout << out;
        this->flog << out;
        this->flog.flush();
    };

    if (option == "ARTIST")
    {
        // ArtistBST::print() prints to cout; we mirror it to flog as well
        capture_and_dual_write([&]
                               { this->ab.print(); });
        return;
    }
    else if (option == "TITLE")
    {
        capture_and_dual_write([&]
                               { this->tb.print(); });
        return;
    }
    else if (option == "LIST")
    {
        if (this->pl.empty())
        {
            std::cout << "========ERROR========\n600\n====================\n";
            this->flog << "========ERROR========\n600\n====================\n";
            return;
        }

        std::cout << "========PRINT========\n";
        std::cout << this->pl.print();
        std::cout << "====================\n";

        this->flog << "========PRINT========\n";
        this->flog << this->pl.print();
        this->flog << "====================\n";
        this->flog.flush();
        return;
    }

    // wrong option
    std::cout << "========ERROR========\n600\n====================\n";
    this->flog << "========ERROR========\n600\n====================\n";
}

void Manager::DELETE(const std::string &rest)
{
    // 0) validate rest
    if (rest.empty())
    {
        std::cout << "========ERROR========\n700\n====================\n";
        this->flog << "========ERROR========\n700\n====================\n";
        return;
    }

    // 1) split option and arg
    std::string option, arg;
    size_t sp = rest.find(' ');
    option = (sp == std::string::npos) ? rest : rest.substr(0, sp);
    arg = (sp == std::string::npos) ? "" : rest.substr(sp + 1);
    if (!arg.empty() && arg[0] == ' ')
        arg.erase(0, 1);
    if (option.empty() || arg.empty())
    {
        std::cout << "========ERROR========\n700\n====================\n";
        this->flog << "========ERROR========\n700\n====================\n";
        return;
    }

    auto print_ok = [&]()
    {
        std::cout << "========DELETE========\nSuccess\n====================\n";
        this->flog << "========DELETE========\nSuccess\n====================\n";
    };

    // helper: remove (artist,title) from ArtistBST node
    auto remove_from_artist_node = [&](ArtistBSTNode *anode, const std::string &title) -> bool
    {
        // return true if node becomes empty
        std::vector<std::string> titles = anode->getTitleList();
        std::vector<int> rts = anode->getRtList();
        for (size_t i = 0; i < titles.size(); ++i)
        {
            if (titles[i] == title)
            {
                titles.erase(titles.begin() + i);
                rts.erase(rts.begin() + i);
                anode->setTitleList(titles);
                anode->setRtList(rts);
                break;
            }
        }
        return titles.empty();
    };

    // helper: remove (artist,title) from TitleBST node
    auto remove_from_title_node = [&](TitleBSTNode *tnode, const std::string &artist) -> bool
    {
        // return true if node becomes empty
        std::vector<std::string> artists = tnode->getArtistList();
        std::vector<int> rts = tnode->getRtList();
        for (size_t i = 0; i < artists.size(); ++i)
        {
            if (artists[i] == artist)
            {
                artists.erase(artists.begin() + i);
                rts.erase(rts.begin() + i);
                tnode->setArtistList(artists);
                tnode->setRtList(rts);
                break;
            }
        }
        return artists.empty();
    };

    // 2) cases
    if (option == "ARTIST")
    {
        std::string artist = arg; // allow spaces
        ArtistBSTNode *anode = this->ab.search(artist);
        if (!anode)
        {
            std::cout << "========ERROR========\n700\n====================\n";
            this->flog << "========ERROR========\n700\n====================\n";
            return;
        }

        // copy titles first
        std::vector<std::string> titles = anode->getTitleList();

        // update TitleBST and PlayList
        for (const auto &t : titles)
        {
            TitleBSTNode *tnode = this->tb.search(t);
            if (tnode)
            {
                bool empty = remove_from_title_node(tnode, artist);
                if (empty)
                    this->tb.deleteNode(t);
            }
            // delete only this song in playlist
            this->pl.delete_node(artist + "|" + t);
        }

        // remove artist node from ArtistBST
        this->ab.delete_node(artist);

        print_ok();
        return;
    }
    else if (option == "TITLE")
    {
        std::string title = arg; // allow spaces
        TitleBSTNode *tnode = this->tb.search(title);
        if (!tnode)
        {
            std::cout << "========ERROR========\n700\n====================\n";
            this->flog << "========ERROR========\n700\n====================\n";
            return;
        }

        // copy artists first
        std::vector<std::string> artists = tnode->getArtistList();

        // update ArtistBST and PlayList
        for (const auto &a : artists)
        {
            ArtistBSTNode *anode = this->ab.search(a);
            if (anode)
            {
                bool empty = remove_from_artist_node(anode, title);
                if (empty)
                    this->ab.delete_node(a);
            }
            // delete only this song in playlist
            this->pl.delete_node(a + "|" + title);
        }

        // remove title node from TitleBST
        this->tb.deleteNode(title);

        print_ok();
        return;
    }
    else if (option == "LIST")
    {
        // arg: "artist|title" only playlist touched
        if (!this->pl.exist(arg))
        {
            std::cout << "========ERROR========\n700\n====================\n";
            this->flog << "========ERROR========\n700\n====================\n";
            return;
        }
        this->pl.delete_node(arg);
        print_ok();
        return;
    }
    else if (option == "SONG")
    {
        // arg: "artist|title" remove across structures
        size_t bar = arg.find('|');
        if (bar == std::string::npos)
        {
            std::cout << "========ERROR========\n700\n====================\n";
            this->flog << "========ERROR========\n700\n====================\n";
            return;
        }
        std::string artist = arg.substr(0, bar);
        std::string title = arg.substr(bar + 1);

        bool touched = false;

        // ArtistBST
        if (ArtistBSTNode *anode = this->ab.search(artist))
        {
            bool empty = remove_from_artist_node(anode, title);
            if (empty)
                this->ab.delete_node(artist);
            touched = true;
        }

        // TitleBST
        if (TitleBSTNode *tnode = this->tb.search(title))
        {
            bool empty = remove_from_title_node(tnode, artist);
            if (empty)
                this->tb.deleteNode(title);
            touched = true;
        }

        // PlayList
        if (this->pl.exist(artist + "|" + title))
        {
            this->pl.delete_node(artist + "|" + title);
            touched = true;
        }

        if (!touched)
        {
            std::cout << "========ERROR========\n700\n====================\n";
            this->flog << "========ERROR========\n700\n====================\n";
            return;
        }

        print_ok();
        return;
    }

    // wrong option
    std::cout << "========ERROR========\n700\n====================\n";
    this->flog << "========ERROR========\n700\n====================\n";
}

void Manager::EXIT()
{
    // print success
    cout << "========EXIT========" << endl;
    cout << "Success" << endl;
    cout << "====================" << endl;

    flog << "========EXIT========" << endl;
    flog << "Success" << endl;
    flog << "====================" << endl;
}
