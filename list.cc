#include <iostream>
#include <algorithm>
#include "list.h"

using namespace std;

sstable::sstable() : sst_size(1), key_cnt(0)
{
    key = new int[sst_size];
    
    for(int i=0; i<sst_size; i++)
        key[i] = 0;

    next = NULL;
    cpt_next = NULL;
}

sstable::sstable(int sst_size) : sst_size(sst_size), key_cnt(0)
{
    key = new int[sst_size];
    
    for(int i=0; i<sst_size; i++)
        key[i] = 0;

    next = NULL;
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

   target->key[target->key_cnt] = k;
   target->key_cnt++;
}

int list::key_pop(sstable *target)
{
    int tmp = 0;

    if(target->key_cnt == 0) {
        cout << "KEY_SIZE ZERO" << endl;
        return -1;
    }
    
    tmp = target->key[0];
    target->key_cnt--;
    for(int i=1;i<=target->key_cnt;i++)
        target->key[i-1] = target->key[i];
    target->key[target->key_cnt+1] = 0;

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
       target->next = NULL;

       size--;
       return target;
    }
}

sstable *list::flush()
{
    if(head != NULL) {
       sstable *target = head;

       head = head->next;
       if(head == NULL) tail = NULL;
       target->next = NULL;

       sort(target->key, target->key+target->sst_size);

       size--;
       return target;
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
        for(int i=0; i < scan->sst_size; i++) {
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
        for(int i=0; i < scan->sst_size; i++) {
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
