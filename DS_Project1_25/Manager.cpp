#include "Manager.h"
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>
#include <functional>
#include "MusicQueue.h"

using namespace std;

static const char *TAIL = "====================\n";

Manager::Manager()
{
    // open log in append mode
    this->flog.open("log.txt", ios::app);
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
    // DO NOT re-open or truncate flog here

    // open command file
    this->fcmd.open(command);

    // if fail to open
    if (!this->fcmd.is_open())
    {
        cout << "========ERROR========" << endl;
        cout << "100" << endl;
        cout << "====================" << endl;

        this->flog << "========ERROR========" << endl;
        this->flog << "100" << endl;
        this->flog << "====================" << endl;
        return;
    }

    std::string line;
    while (std::getline(this->fcmd, line))
    {
        if (line.empty())
            continue;

        // split command and rest (ex: "ADD KDA|pop stars|3:23")
        size_t sp = line.find(' ');
        string cmd = (sp == std::string::npos) ? line : line.substr(0, sp);
        string rest = (sp == std::string::npos) ? "" : line.substr(sp + 1);

        if (cmd == "LOAD")
        {
            this->LOAD();
        }
        else if (cmd == "ADD")
        {
            this->ADD(rest);
        }
        else if (cmd == "QPOP")
        {
            this->QPOP();
        }
        else if (cmd == "SEARCH")
        {
            this->SEARCH(rest);
        }
        else if (cmd == "MAKEPL")
        {
            this->MAKEPL(rest);
        }
        else if (cmd == "PRINT")
        {
            this->PRINT(rest);
        }
        else if (cmd == "DELETE")
        {
            this->DELETE(rest);
        }
        else if (cmd == "EXIT")
        {
            this->EXIT();
            break;
        }
        else
        {
            std::cout << "========ERROR========\n1000\n"
                      << TAIL;
            this->flog << "========ERROR========\n1000\n"
                       << TAIL;
        }
    }
    this->fcmd.close();
}

void Manager::LOAD()
{
    if (this->loaded_once)
    {
        std::cout << "========ERROR========\n100\n====================\n";
        this->flog << "========ERROR========\n100\n====================\n";
        return;
    }

    // case: already loaded (queue not empty)
    if (!this->q.empty())
    {
        cout << "========ERROR========" << endl;
        cout << "100" << endl;
        cout << "====================" << endl;

        this->flog << "========ERROR========" << endl;
        this->flog << "100" << endl;
        this->flog << "====================" << endl;
        return;
    }

    ifstream readFile;
    readFile.open("Music_List.txt");
    if (!readFile.is_open())
    {
        cout << "========ERROR========" << endl;
        cout << "100" << endl;
        cout << "====================" << endl;

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
        cout << "========ERROR========" << endl;
        cout << "100" << endl;
        cout << "====================" << endl;

        this->flog << "========ERROR========" << endl;
        this->flog << "100" << endl;
        this->flog << "====================" << endl;
        return;
    }

    // print loaded data (both cout and flog)
    cout << "========LOAD========" << endl;
    this->flog << "========LOAD========" << endl;

    MusicQueueNode *cur = this->q.getHead();
    while (cur)
    {
        cout << cur->getArtist() << "/"
             << cur->getTitle() << "/"
             << cur->getRunTime() << endl;

        this->flog << cur->getArtist() << "/"
                   << cur->getTitle() << "/"
                   << cur->getRunTime() << endl;

        cur = cur->getNext();
    }

    cout << "====================" << endl;
    this->flog << "====================" << endl;

    this->loaded_once=true;
}

void Manager::ADD(const std::string &data_in)
{
    // 1) basic check
    if (data_in.empty())
    {
        std::cout << "========ERROR========\n200\n"
                  << TAIL;
        this->flog << "========ERROR========\n200\n"
                   << TAIL;
        return;
    }

    // 2) trim one leading space if exists
    std::string data = data_in;
    if (!data.empty() && data[0] == ' ')
        data = data.substr(1);

    // 3) split by '|'
    size_t p1 = data.find('|');
    size_t p2 = (p1 == std::string::npos) ? std::string::npos : data.find('|', p1 + 1);
    if (p1 == std::string::npos || p2 == std::string::npos)
    {
        std::cout << "========ERROR========\n200\n"
                  << TAIL;
        this->flog << "========ERROR========\n200\n"
                   << TAIL;
        return;
    }

    std::string artist = data.substr(0, p1);
    std::string title = data.substr(p1 + 1, p2 - p1 - 1);
    std::string runtime = data.substr(p2 + 1);

    if (artist.empty() || title.empty() || runtime.empty())
    {
        std::cout << "========ERROR========\n200\n"
                  << TAIL;
        this->flog << "========ERROR========\n200\n"
                   << TAIL;
        return;
    }

    // 4) duplicate check in queue (compare full record in queue context)
    if (this->q.exist(artist + "|" + title + "|" + runtime))
    {
        std::cout << "========ERROR========\n200\n"
                  << TAIL;
        this->flog << "========ERROR========\n200\n"
                   << TAIL;
        return;
    }

    // 5) push to queue
    this->q.push(artist + "|" + title + "|" + runtime);

    // 6) success print (both)
    std::cout << "========ADD========\n"
              << artist << "/" << title << "/" << runtime << "\n"
              << TAIL;

    this->flog << "========ADD========\n"
               << artist << "/" << title << "/" << runtime << "\n"
               << TAIL;
}

void Manager::QPOP()
{
    // check if queue empty
    if (this->q.empty())
    {
        cout << "========ERROR========" << endl;
        cout << "300" << endl;
        cout << "====================" << endl;
        this->flog << "========ERROR========" << endl;
        this->flog << "300" << endl;
        this->flog << "====================" << endl;
        return;
    }

    // pop all songs from queue
    while (!this->q.empty())
    {
        MusicQueueNode *node = this->q.pop();
        if (node == nullptr)
            break;

        // insert into BSTs
        this->ab.insert(node);
        this->tb.insert(node);

        delete node;
    }

    // print success
    cout << "========QPOP========" << endl;
    cout << "Success" << endl;
    cout << "====================" << endl;

    this->flog << "========QPOP========" << endl;
    this->flog << "Success" << endl;
    this->flog << "====================" << endl;
}

void Manager::SEARCH(const std::string &rest)
{
    if (rest.empty())
    {
        std::cout << "========ERROR========\n400\n"
                  << TAIL;
        this->flog << "========ERROR========\n400\n"
                   << TAIL;
        return;
    }

    // split option and arg
    std::string option, arg;
    size_t sp = rest.find(' ');
    option = (sp == std::string::npos) ? rest : rest.substr(0, sp);
    arg = (sp == std::string::npos) ? "" : rest.substr(sp + 1);
    if (!arg.empty() && arg[0] == ' ')
        arg.erase(0, 1);
    if (option.empty() || arg.empty())
    {
        std::cout << "========ERROR========\n400\n"
                  << TAIL;
        this->flog << "========ERROR========\n400\n"
                   << TAIL;
        return;
    }

    // helper: rt -> "m:ss"
    auto mmss = [](int sec)
    {
        int m = sec / 60, s = sec % 60;
        std::ostringstream os;
        os << m << ":" << (s < 10 ? "0" : "") << s;
        return os.str();
    };

    if (option == "ARTIST")
    {
        std::string artist = arg;
        ArtistBSTNode *found = this->ab.search(artist);
        if (!found)
        {
            std::cout << "========ERROR========\n400\n"
                      << TAIL;
            this->flog << "========ERROR========\n400\n"
                       << TAIL;
            return;
        }

        std::cout << "========SEARCH========\n";
        this->flog << "========SEARCH========\n";

        std::vector<std::string> titles = found->getTitleList();
        std::vector<int> rt = found->getRtList();
        for (size_t i = 0; i < titles.size(); ++i)
        {
            std::string t = artist + "/" + titles[i] + "/" + mmss(rt[i]) + "\n";
            std::cout << t;
            this->flog << t;
        }

        std::cout << TAIL;
        this->flog << TAIL;
        return;
    }

    if (option == "TITLE")
    {
        std::string title = arg;
        TitleBSTNode *found = this->tb.search(title);
        if (!found)
        {
            std::cout << "========ERROR========\n400\n"
                      << TAIL;
            this->flog << "========ERROR========\n400\n"
                       << TAIL;
            return;
        }

        std::cout << "========SEARCH========\n";
        this->flog << "========SEARCH========\n";

        const std::vector<std::string> &artists = found->getArtistList();
        const std::vector<int> &rt = found->getRtList();
        for (size_t i = 0; i < artists.size(); ++i)
        {
            std::string t = artists[i] + "/" + title + "/" + mmss(rt[i]) + "\n";
            std::cout << t;
            this->flog << t;
        }

        std::cout << TAIL;
        this->flog << TAIL;
        return;
    }

    if (option == "SONG")
    {
        std::string song = arg; // "artist|title"
        size_t bar = song.find('|');
        if (bar == std::string::npos)
        {
            std::cout << "========ERROR========\n400\n"
                      << TAIL;
            this->flog << "========ERROR========\n400\n"
                       << TAIL;
            return;
        }
        std::string artist = song.substr(0, bar);
        std::string title = song.substr(bar + 1);

        ArtistBSTNode *aNode = this->ab.search(artist);
        if (!aNode)
        {
            std::cout << "========ERROR========\n400\n"
                      << TAIL;
            this->flog << "========ERROR========\n400\n"
                       << TAIL;
            return;
        }

        const std::vector<std::string> titles = aNode->getTitleList();
        const std::vector<int> rt = aNode->getRtList();

        for (size_t i = 0; i < titles.size(); ++i)
        {
            if (titles[i] == title)
            {
                std::string t = artist + "/" + title + "/" + mmss(rt[i]) + "\n";
                std::cout << "========SEARCH========\n";
                this->flog << "========SEARCH========\n";
                std::cout << t;
                this->flog << t;
                std::cout << TAIL;
                this->flog << TAIL;
                return;
            }
        }

        // not found
        std::cout << "========ERROR========\n400\n"
                  << TAIL;
        this->flog << "========ERROR========\n400\n"
                   << TAIL;
        return;
    }

    // wrong option
    std::cout << "========ERROR========\n400\n"
              << TAIL;
    this->flog << "========ERROR========\n400\n"
               << TAIL;
}

void Manager::MAKEPL(const std::string &rest)
{
    // simple tail macro assumed: TAIL == "=====================\n"

    // 0) basic arg check
    if (rest.empty())
    {
        std::cout << "========ERROR========\n500\n"
                  << TAIL;
        this->flog << "========ERROR========\n500\n"
                   << TAIL;
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
        std::cout << "========ERROR========\n500\n"
                  << TAIL;
        this->flog << "========ERROR========\n500\n"
                   << TAIL;
        return;
    }

    // helper: seconds -> "m:ss"
    auto mmss = [](int sec) -> std::string
    {
        int m = sec / 60, s = sec % 60;
        std::ostringstream os;
        os << m << ":" << (s < 10 ? "0" : "") << s;
        return os.str();
    };

    struct Song
    {
        std::string artist, title, rtStr;
    };
    std::vector<Song> toAdd;

    // 2) build toAdd from BSTs
    if (option == "ARTIST")
    {
        std::string artist = arg;
        ArtistBSTNode *node = this->ab.search(artist);
        if (!node)
        {
            std::cout << "========ERROR========\n500\n"
                      << TAIL;
            this->flog << "========ERROR========\n500\n"
                       << TAIL;
            return;
        }
        std::vector<std::string> titles = node->getTitleList();
        std::vector<int> rts = node->getRtList();
        for (size_t i = 0; i < titles.size(); ++i)
            toAdd.push_back({artist, titles[i], mmss(rts[i])});
    }
    else if (option == "TITLE")
    {
        std::string title = arg;
        TitleBSTNode *node = this->tb.search(title);
        if (!node)
        {
            std::cout << "========ERROR========\n500\n"
                      << TAIL;
            this->flog << "========ERROR========\n500\n"
                       << TAIL;
            return;
        }
        const std::vector<std::string> &artists = node->getArtistList();
        const std::vector<int> &rts = node->getRtList();
        for (size_t i = 0; i < artists.size(); ++i)
            toAdd.push_back({artists[i], title, mmss(rts[i])});
    }
    else if (option == "SONG")
    {
        size_t bar = arg.find('|');
        if (bar == std::string::npos)
        {
            std::cout << "========ERROR========\n500\n"
                      << TAIL;
            this->flog << "========ERROR========\n500\n"
                       << TAIL;
            return;
        }
        std::string artist = arg.substr(0, bar);
        std::string title = arg.substr(bar + 1);

        ArtistBSTNode *node = this->ab.search(artist);
        if (!node)
        {
            std::cout << "========ERROR========\n500\n"
                      << TAIL;
            this->flog << "========ERROR========\n500\n"
                       << TAIL;
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
            std::cout << "========ERROR========\n500\n"
                      << TAIL;
            this->flog << "========ERROR========\n500\n"
                       << TAIL;
            return;
        }
    }
    else
    {
        std::cout << "========ERROR========\n500\n"
                  << TAIL;
        this->flog << "========ERROR========\n500\n"
                   << TAIL;
        return;
    }

    if (toAdd.empty())
    {
        std::cout << "========ERROR========\n500\n"
                  << TAIL;
        this->flog << "========ERROR========\n500\n"
                   << TAIL;
        return;
    }

    // helper: get current Count by parsing pl.print()
    auto get_current_count = [&]() -> int
    {
        std::string all = this->pl.print(); // may be ""
        if (all.empty())
            return 0;
        std::istringstream iss(all);
        std::string line, lastCountLine;
        while (std::getline(iss, line))
        {
            if (line.rfind("Count :", 0) == 0)
                lastCountLine = line;
        }
        // format: "Count : X / 10"
        int cnt = 0;
        if (!lastCountLine.empty())
        {
            std::istringstream p(lastCountLine);
            std::string dummy;
            char colon;
            p >> dummy >> colon >> cnt; // read X
        }
        return cnt;
    };

    // 3) pre-check: duplicates (must pass "artist|title|m:ss")
    for (const auto &s : toAdd)
    {
        std::string key = s.artist + "|" + s.title + "|" + s.rtStr; // exist() needs two pipes
        if (this->pl.exist(key))
        {
            std::cout << "========ERROR========\n500\n"
                      << TAIL;
            this->flog << "========ERROR========\n500\n"
                       << TAIL;
            return;
        }
    }

    // 4) pre-check: capacity
    int curCount = get_current_count();
    if (curCount + (int)toAdd.size() > PLAYLIST_MAX_SIZE)
    {
        std::cout << "========ERROR========\n500\n"
                  << TAIL;
        this->flog << "========ERROR========\n500\n"
                   << TAIL;
        return;
    }

    // 5) header
    std::cout << "========MAKEPL========\n";
    this->flog << "========MAKEPL========\n";

    // 6) insert all (no partial success)
    for (const auto &s : toAdd)
    {
        std::ostringstream rec;
        rec << s.artist << "|" << s.title << "|" << s.rtStr;
        this->pl.insert_node(rec.str());

        // // echo each added song
        // std::cout << s.artist << "/" << s.title << "/" << s.rtStr << "\n";
        // this->flog  << s.artist << "/" << s.title << "/" << s.rtStr << "\n";
    }

    // 7) print only Count/Time from pl.print()
    std::string all = this->pl.print();
    std::cout << all;
    this->flog << all;
    std::cout << TAIL;
    this->flog << TAIL;
}

void Manager::PRINT(const std::string &rest)
{
    std::string data = rest;
    if (!data.empty() && data[0] == ' ')
        data = data.substr(1);

    if (data.empty())
    {
        std::cout << "========ERROR========\n600\n"
                  << TAIL;
        this->flog << "========ERROR========\n600\n"
                   << TAIL;
        return;
    }

    std::stringstream ss(data);
    std::string option;
    ss >> option;

    // helper: capture cout during a function call, then write to cout + flog
    auto capture_and_dual_write = [&](const std::function<void()> &fn_call)
    {
        std::ostringstream cap;
        std::streambuf *old = std::cout.rdbuf(cap.rdbuf());
        fn_call();            // function prints to cout
        std::cout.rdbuf(old); // restore

        const std::string body = cap.str();

        // ensure header/footer (avoid duplication if BST already printed them)
        bool hasHeader = (body.rfind("========", 0) == 0);
        bool hasTail = (body.find("====================") != std::string::npos);

        if (!hasHeader)
        {
            std::cout << "========PRINT========\n";
            this->flog << "========PRINT========\n";
        }

        std::cout << body;
        this->flog << body;

        if (!hasTail)
        {
            std::cout << TAIL;
            this->flog << TAIL;
        }
        this->flog.flush();
    };

    if (option == "ARTIST")
    {
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
            std::cout << "========ERROR========\n600\n"
                      << TAIL;
            this->flog << "========ERROR========\n600\n"
                       << TAIL;
            return;
        }

        std::cout << "========PRINT========\n";
        std::cout << this->pl.print();
        std::cout << TAIL;

        this->flog << "========PRINT========\n";
        this->flog << this->pl.print();
        this->flog << TAIL;
        this->flog.flush();
        return;
    }

    // wrong option
    std::cout << "========ERROR========\n600\n"
              << TAIL;
    this->flog << "========ERROR========\n600\n"
               << TAIL;
}

void Manager::DELETE(const std::string &rest)
{
    // basic arg check
    if (rest.empty())
    {
        std::cout << "========ERROR========\n700\n"
                  << TAIL;
        this->flog << "========ERROR========\n700\n"
                   << TAIL;
        return;
    }

    // split option + argument
    std::string option, arg;
    size_t sp = rest.find(' ');
    option = (sp == std::string::npos) ? rest : rest.substr(0, sp);
    arg = (sp == std::string::npos) ? "" : rest.substr(sp + 1);
    if (!arg.empty() && arg[0] == ' ')
        arg.erase(0, 1);

    // validate tokens
    if (option.empty() || arg.empty())
    {
        std::cout << "========ERROR========\n700\n"
                  << TAIL;
        this->flog << "========ERROR========\n700\n"
                   << TAIL;
        return;
    }

    // success printer (only here, not inside BST/PL)
    auto print_ok = [&]()
    {
        std::cout << "========DELETE========\nSuccess\n"
                  << TAIL;
        this->flog << "========DELETE========\nSuccess\n"
                   << TAIL;
    };

    // remove all matches of (title) inside an Artist node
    // returns {removed_any, became_empty}
    auto remove_from_artist_node =
        [&](ArtistBSTNode *anode, const std::string &title) -> std::pair<bool, bool>
    {
        std::vector<std::string> titles = anode->getTitleList();
        std::vector<int> rts = anode->getRtList();

        bool removed = false;
        std::vector<std::string> newTitles;
        std::vector<int> newRts;
        newTitles.reserve(titles.size());
        newRts.reserve(rts.size());

        for (size_t i = 0; i < titles.size(); ++i)
        {
            if (titles[i] == title)
            {
                removed = true;
                continue;
            }
            newTitles.push_back(titles[i]);
            newRts.push_back(rts[i]);
        }

        if (!removed)
            return {false, false};

        anode->setTitleList(newTitles);
        anode->setRtList(newRts);
        anode->setCount(static_cast<int>(newTitles.size()));
        return {true, newTitles.empty()};
    };

    // remove all matches of (artist) inside a Title node
    // returns {removed_any, became_empty}
    auto remove_from_title_node =
        [&](TitleBSTNode *tnode, const std::string &artist) -> std::pair<bool, bool>
    {
        std::vector<std::string> artists = tnode->getArtistList();
        std::vector<int> rts = tnode->getRtList();

        bool removed = false;
        std::vector<std::string> newArtists;
        std::vector<int> newRts;
        newArtists.reserve(artists.size());
        newRts.reserve(rts.size());

        for (size_t i = 0; i < artists.size(); ++i)
        {
            if (artists[i] == artist)
            {
                removed = true;
                continue;
            }
            newArtists.push_back(artists[i]);
            newRts.push_back(rts[i]);
        }

        if (!removed)
            return {false, false};

        tnode->setArtistList(newArtists);
        tnode->setRtList(newRts);
        tnode->setCount(static_cast<int>(newArtists.size()));
        return {true, newArtists.empty()};
    };

    // option: ARTIST <artist>
    if (option == "ARTIST")
    {
        const std::string artist = arg;
        ArtistBSTNode *anode = this->ab.search(artist);
        if (!anode)
        {
            std::cout << "========ERROR========\n700\n"
                      << TAIL;
            this->flog << "========ERROR========\n700\n"
                       << TAIL;
            return;
        }

        // snapshot titles
        std::vector<std::string> titles = anode->getTitleList();

        // clean TitleBST and PlayList for every title
        for (const auto &t : titles)
        {
            if (TitleBSTNode *tnode = this->tb.search(t))
            {
                auto [removed, empty] = remove_from_title_node(tnode, artist);
                if (empty)
                    this->tb.deleteNode(t);
                // removed flag not needed here because whole artist will be deleted anyway
            }
            // remove all duplicates in playlist
            const std::string key = artist + "|" + t;
            while (this->pl.exist(key))
                this->pl.delete_node(key);
        }

        // finally remove artist node
        this->ab.delete_node(artist);

        print_ok();
        return;
    }

    // option: TITLE <title>
    if (option == "TITLE")
    {
        const std::string title = arg;
        TitleBSTNode *tnode = this->tb.search(title);
        if (!tnode)
        {
            std::cout << "========ERROR========\n700\n"
                      << TAIL;
            this->flog << "========ERROR========\n700\n"
                       << TAIL;
            return;
        }

        // snapshot artists
        std::vector<std::string> artists = tnode->getArtistList();

        // clean ArtistBST and PlayList for every artist
        for (const auto &a : artists)
        {
            if (ArtistBSTNode *anode = this->ab.search(a))
            {
                auto [removed, empty] = remove_from_artist_node(anode, title);
                if (empty)
                    this->ab.delete_node(a);
            }
            // remove all duplicates in playlist
            const std::string key = a + "|" + title;
            while (this->pl.exist(key))
                this->pl.delete_node(key);
        }

        // finally remove title node
        this->tb.deleteNode(title);

        print_ok();
        return;
    }

    // option: LIST <artist|title> (playlist only)
    if (option == "LIST")
    {
        const std::string key = arg;
        if (!this->pl.exist(key))
        {
            std::cout << "========ERROR========\n700\n"
                      << TAIL;
            this->flog << "========ERROR========\n700\n"
                       << TAIL;
            return;
        }
        // remove all duplicates in playlist
        while (this->pl.exist(key))
            this->pl.delete_node(key);
        print_ok();
        return;
    }

    // option: SONG <artist|title> (every structure)
    if (option == "SONG")
    {
        size_t bar = arg.find('|');
        if (bar == std::string::npos)
        {
            std::cout << "========ERROR========\n700\n"
                      << TAIL;
            this->flog << "========ERROR========\n700\n"
                       << TAIL;
            return;
        }
        const std::string artist = arg.substr(0, bar);
        const std::string title = arg.substr(bar + 1);

        bool touched = false;

        // ArtistBST side
        if (ArtistBSTNode *anode = this->ab.search(artist))
        {
            auto [removed, empty] = remove_from_artist_node(anode, title);
            if (removed)
                touched = true;
            if (empty)
                this->ab.delete_node(artist);
        }

        // TitleBST side
        if (TitleBSTNode *tnode = this->tb.search(title))
        {
            auto [removed, empty] = remove_from_title_node(tnode, artist);
            if (removed)
                touched = true;
            if (empty)
                this->tb.deleteNode(title);
        }

        // PlayList side
        const std::string key = artist + "|" + title;
        bool plRemoved = false;
        while (this->pl.exist(key))
        {
            this->pl.delete_node(key);
            plRemoved = true;
        }
        if (plRemoved)
            touched = true;

        if (!touched)
        {
            std::cout << "========ERROR========\n700\n"
                      << TAIL;
            this->flog << "========ERROR========\n700\n"
                       << TAIL;
            return;
        }

        print_ok();
        return;
    }

    // wrong option
    std::cout << "========ERROR========\n700\n"
              << TAIL;
    this->flog << "========ERROR========\n700\n"
               << TAIL;
}

void Manager::EXIT()
{
    cout << "========EXIT========" << endl;
    cout << "Success" << endl;
    cout << "====================" << endl;

    this->flog << "========EXIT========" << endl;
    this->flog << "Success" << endl;
    this->flog << "====================" << endl;
}
