#ifndef LIST_H
#define LIST_H

#include <vector>

class sstable
{
private:
    std::vector<int> key;
    int sst_size;
    int key_cnt;

public:
    sstable();
    sstable(int sst_size);
    sstable *next;
    sstable *prev;
    sstable *cpt_next;
    friend class list;
};

class list
{
private:
    int size;
public:
    sstable *head;
    sstable *tail;

    list();
    ~list();

    int key_data(sstable *target, int index);
    int get_keycnt(sstable *target);
    int get_size();

    void key_push(sstable *target, int k);
    int key_pop(sstable *target);

    void create(int sst_size);
    void cpt_push(sstable *target);
    void push(sstable *target);
    sstable *pop();
    sstable *flush();
    sstable *erase(sstable *target);

    void init();
    void cpt_init();

    void print();
    void cpt_print();
};

#endif
