#include "MusicQueue.h"
#include <iostream>
using namespace std;

MusicQueue::MusicQueue()
{
    this->head = nullptr;
    this->rear = nullptr;
}

MusicQueue::~MusicQueue()
{
    while (this->head)
    {
        MusicQueueNode *tmpNode = head;
        head = head->getNext();
        delete tmpNode;
    }
    rear = nullptr;
}

bool MusicQueue::empty()
{
    if (this->size < 1)
        return true;
    else
        return false;
}

bool MusicQueue::exist(string line)
{

    MusicQueueNode *curNode = head; // node for search
    while (curNode)
    {
        if (curNode->exist(line))
            return true;
        curNode = curNode->getNext();
    }
    return false;
}

void MusicQueue::push(string line)
{
    if (this->size == 100)
    {
        std::cout << "큐 꽉" << endl
                  << "프로그램 종료" << endl;
        return;
    }
    // extract elements from line
    string artist, title, run_time;
    size_t idx1, idx2;
    idx1 = line.find('|');
    idx2 = line.find('|', idx1 + 1);
    artist = line.substr(0, idx1);
    title = line.substr(idx1 + 1, idx2 - idx1 - 1);
    run_time = line.substr(idx2, line.size() - idx2);

    MusicQueueNode *tmpNode = new MusicQueueNode;
    tmpNode->insert(artist, title, run_time);
    if (!head)
    {
        this->head = tmpNode;
        this->rear = tmpNode;
    }
    else
    {
        this->rear->setNext(tmpNode);
        tmpNode->setPrev(rear);
        this->rear = tmpNode;
    }
}

MusicQueueNode *MusicQueue::pop()
{
    if (!this->head)
    {
        cout << "큐 텅" << endl
             << "프로그램 종료" << endl;
        return nullptr;
    }
    MusicQueueNode *tmpNode = this->head; // return을 위한 pop할 head 노드 저장
    this->head = this->head->getNext();   // 뮤직큐의 헤드를 head의 next로 바꾸기
    if (this->head == nullptr)
        this->rear = nullptr; // 만약 head==rear여서 head==nullptr이 되면 rear도 바꿔야함
    size--;
    return tmpNode;
}

MusicQueueNode *MusicQueue::front()
{
    if (!this->head)
        return nullptr;
    else
        return this->head;
}

int main()
{
    {
        MusicQueue q; // 여기서 생성자 호출됨

        // 노드 3개 정도 추가해보기
        q.push("N.Flying|blue moon|3:36");
        q.push("PSY|gangnam style|3:25");
        q.push("Maroon 5|sugar|3:55");

        cout << "큐에 노드 3개 삽입 완료" << endl;

    } // 여기서 q 소멸자 자동 호출됨

    cout << "프로그램 끝!" << endl;
    return 0;
}