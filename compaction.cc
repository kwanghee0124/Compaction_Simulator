#include <iostream>
#include <vector>
#include <algorithm>
#include "compaction.h"
#include "list.h"

using namespace std;

bool compaction_check(list *level, list &cpt, int lnum, vector<int> &vec, int sst_size)
{
    sstable *scan = level[lnum+1].head;
    int key_init = 0;
    bool check = false;
    bool cpt_check = false;

    while(scan != NULL) {
        for(int i=key_init; i<sst_size; i++) {
            if((level[lnum].key_data(level[lnum].head, i) >= level[lnum+1].key_data(scan, 0))
                    && (level[lnum].key_data(level[lnum].head, i) <= level[lnum+1].key_data(scan, level[lnum+1].get_keycnt(scan)-1))) {
                key_init++;
                check = true;
            }

        }
        if(check) {
            cpt.cpt_push(scan);
            while(level[lnum+1].get_keycnt(scan) != 0)
                vec.push_back(level[lnum+1].key_pop(scan));
            
            read_cnt++;
            check = false;
            cpt_check = true;
        }
        scan = scan->next;
    }

    return cpt_check;
}

void compaction_exec(list *level, list &cpt, int lnum, vector<int> &vec, int sst_size)
{
    sstable *scan = cpt.head;

    cpt.cpt_push(level[lnum+1].tail);

    // merge
    while(level[lnum+1].get_keycnt(level[lnum+1].tail) != 0)
        vec.push_back(level[lnum+1].key_pop(level[lnum+1].tail));

    // sort
    sort(vec.begin(), vec.end());

    // update key check
    vec.erase(unique(vec.begin(), vec.end()), vec.end());
    
    // compaction sort data input
    while(scan != NULL) {
        if(!vec.size()) {
            level[lnum+1].erase(scan);
        } else {
            int size = 0;

            if(vec.size() > sst_size) size = sst_size;
            else size = vec.size();

            for(int i=0; i<size; i++) {
                cpt.key_push(scan, vec.front());
                vec.erase(vec.begin());
            }
            write_cnt++;
        }
        scan = scan->cpt_next;
    }
    write_cnt--;
}

