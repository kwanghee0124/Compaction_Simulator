#include <iostream>
#include <algorithm>
#include "list.h"

using namespace std;

sstable::sstable() : sst_size(1), key_cnt(0)
{ 
    key.clear();
    next = prev = NULL;
    cpt_next = NULL;
}

sstable::sstable(int sst_size) : sst_size(sst_size), key_cnt(0)
{
    key.clear();
    next = prev = NULL;
    cpt_next = NULL;
}

int list::key_data(sstable *target, int index)
{
    return target->key[index];
}

int list::get_keycnt(sstable *target)
{
    return target->key_cnt;
}

int list::get_size()
{
    return size;
}

void list::create(int sst_size)
{
    sstable *newsst = new sstable(sst_size);

    if(head == NULL) {
        head = newsst;
    } else {
        tail->next = newsst;
        newsst->prev = tail;
    }
    tail = newsst;
    size++;
}

void list::key_push(sstable *target, int k)
{
    if(target->sst_size <= target->key_cnt) {
        cout << "KEY_SIZE FULL" << endl;
        return;
    }

    target->key.push_back(k);
    target->key_cnt++;
}

int list::key_pop(sstable *target)
{
    int tmp = 0;

    if(target->key_cnt == 0) {
        cout << "KEY_SIZE ZERO" << endl;
        return -1;
    }

    tmp = target->key.front();
    target->key.erase(target->key.begin());
    target->key_cnt--;

    return tmp;
}

void list::cpt_push(sstable *target)
{
    if(head == NULL) {
        head = target;
    } else {
        tail->cpt_next = target;
    }
    tail = target;
    size++;
}

void list::push(sstable *target)
{
    if(head == NULL) {
        head = target;
    } else {
        tail->next = target;
        target->prev = tail;
    }
    tail = target;
    size++;
}

sstable *list::pop()
{
    if(head != NULL) {
        sstable *target = head;

        head = head->next;

        if(head == NULL) tail = NULL;
        else head->prev = NULL;

        target->next = NULL;

        size--;
        return target;
    }
    cout << "POP : LIST NULL" << endl;
}

sstable *list::flush()
{
    if(head != NULL) {
        sstable *target = head;

        head = head->next;

        if(head == NULL) tail = NULL;
        else head->prev = NULL;

        target->next = NULL;

        sort(target->key.begin(), target->key.end());

        size--;
        return target;
    }
    cout << "FLUSH : LIST NULL" << endl;
}

void list::erase_back()
{
    if(head != NULL) {
        sstable *target = tail;

        tail = tail->prev;

        if(tail == NULL) head = NULL;
        else tail->next = NULL;

        delete target;
        size--;
    }
}

void list::erase(sstable *target)
{
    if(head != NULL) {
        if(size == 1) {
            head = NULL;
            tail = NULL;
        } else {
            if(target == head) {
                head = head->next;

                head->prev = NULL;
                target->next = NULL;
            } else if(target == tail) {
                tail = tail->prev;

                tail->next = NULL;
                target->prev = NULL;
            } else {
                target->prev->next = target->next;
                target->next->prev = target->prev;

                target->next = NULL;
                target->prev = NULL;
            }
        }

        delete target;
        size--;
    }
}

void list::init()
{
    head = tail = NULL;
    size = 0;
}

void list::cpt_init()
{
    sstable *scan = head;

    while(scan != NULL) {
        scan->cpt_next = NULL;

        scan = scan->next;
    }
}

void list::print()
{
    sstable *scan = head;

    while(scan != NULL) {
        cout << "[ ";
        for(int i=0; i<scan->key.size(); i++) {
            cout << scan->key[i] << " ";
        }
        cout << "] ";
        scan = scan->next;
    }
    cout << endl;
}

void list::cpt_print()
{
    sstable *scan = head;

    while(scan != NULL) {
        cout << "[ ";
        for(int i=0; i < scan->key.size(); i++) {
            cout << scan->key[i] << " ";
        }
        cout << "] ";
        scan = scan->cpt_next;
    }
    cout << endl;
}

list::list()
{
    head = tail = NULL;
    size = 0;
}

list::~list()
{
    while(head != NULL) {
        pop();
    }
}
