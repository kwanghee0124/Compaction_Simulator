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
    int cnt = 0;
    int target = lnum+1;
    bool check = false;
    bool cpt_check = false;
    
    do {
        while(scan != NULL) { 
            for(int i=key_init; i<sst_size; i++) {
                if((lnum == 0) && (level[lnum].key_data(level[lnum].head, 0) <= level[target].key_data(scan, i)) 
                        && (level[lnum].key_data(level[lnum].head, level[lnum].get_keycnt(level[lnum].head)-1) >= level[target].key_data(scan, i))) {
                    check = true;
                }
                if((level[lnum].key_data(level[lnum].head, i) >= level[target].key_data(scan, 0)) 
                        && (level[lnum].key_data(level[lnum].head, i) <= level[target].key_data(scan, level[target].get_keycnt(scan)-1))) {
                    if(lnum != 0) key_init++;
                    check = true;
                }
            }
            
            if(check) {
                cpt.cpt_push(scan);
                while(level[target].get_keycnt(scan) != 0)
                    vec.push_back(level[target].key_pop(scan));
                
                read_cnt++;
                check = false;
                cpt_check = true;
            }
            scan = scan->next; 
        }
        // L[0] compaction
        if(lnum == 0) {
            scan = level[lnum].head->next;
            target = lnum;
            key_init = 0;
            cnt++;
        }
    } while(lnum == 0 && cnt < 2);

    // L[0] compaction data push
    if(lnum == 0) {
        scan = level[lnum].head;
        
        while(scan != NULL) {
            if(level[lnum].get_keycnt(scan) == 0) {
                level[lnum+1].push(level[lnum].erase(scan));
                
                scan = level[lnum].head;
            }
            scan = scan->next;
        }
    }

    // head data push
    level[lnum+1].push(level[lnum].pop());
    if(!cpt_check) write_cnt++;
    read_cnt++;

    return cpt_check;
}

void compaction_exec(list *level, list &cpt, int lnum, vector<int> &vec, int sst_size)
{
    sstable *scan = cpt.head;

    // tail data compaction select
    cpt.cpt_push(level[lnum+1].tail);
    while(level[lnum+1].get_keycnt(level[lnum+1].tail) != 0)
        vec.push_back(level[lnum+1].key_pop(level[lnum+1].tail));

    // sort
    sort(vec.begin(), vec.end());

    // update key check
    vec.erase(unique(vec.begin(), vec.end()), vec.end());
    
    // compaction sort data input
    while(scan != NULL) {
        if(!vec.size()) {
            delete level[lnum+1].erase(scan);
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
}

