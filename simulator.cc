#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string.h>
#include <vector>
#include <algorithm>
#include "list.h"
#include "compaction.h"

using namespace std;

int read_cnt = 0;
int write_cnt = 0;

void usage(char *program)
{
    cout << "====================" << endl;
    cout << "usage : " << program << " [-t type] [-l level_num] [-s level_sst_size] [-k input_key] [-n sst_size]" << endl;
    cout << "-t     type            :   input type sequential(s), random(r) Ex) r" << endl;
    cout << "-l     level_num       :   # of Level Ex) 3" << endl;
    cout << "-s     level_sst_size  :   Size of SSTable Ex) 2,4,8 (%)" << endl;
    cout << "-k     input_key       :   input keys Ex) 1~32" << endl;
    cout << "-n     sst_size        :   # of keys in SSTable Ex) 4" << endl;
    cout << "====================" << endl;
}

void level_print(list memtable, list *level, int level_num)
{
    cout << "----------" << endl;
    cout << "memtable" << endl;
    memtable.print();
    cout << "----------" << endl;
    for(int i=0;i<level_num;i++) {
        cout << "level " << i << endl;
        level[i].print();
    }
    cout << "----------" << endl;
}

int* level_sst_size_tok(int level_num, char *lss_str)
{
    int *level_sst_size = new int(level_num);
    char* tok = strtok(lss_str,",");
    for(int i=0;i<level_num;i++) {
        level_sst_size[i] = atoi(tok);
        tok = strtok(NULL,",");
    }

    return level_sst_size;
}

void random_arr(int *input_arr, int num)
{
    int temp = 0;
    int n = 0;

    srand((unsigned int)time(0));

    for(int i=0; i<num; i++) {
        n = rand()%num;

        temp = input_arr[n];
        input_arr[n] = input_arr[num-1];
        input_arr[num-1] = temp;
    }
}

int main(int argc, char **argv)
{
    int c = 1;
    int level_num = 0; // # of Level ex) 3 / l
    int *level_sst_size; // Size of SSTable ex) 2,4,8 / s
    char *lss_str;
    int input_key = 0; // input keys ex) 1~32 / k
    int sst_size = 0; // # of keys in SSTable ex) 4 / n
    char *type; // type ex) r / t

    while((c = getopt(argc, argv, "t:l:s:k:n:h")) != -1) {
        switch(c) {
            case 't':
                type = optarg;
                break;
            case 'l':
                level_num = atoi(optarg);
                break;
            case 's':
                lss_str = optarg;
                break;
            case 'k':
                input_key = atoi(optarg);
                break;
            case 'n':
                sst_size = atoi(optarg);
                break;
            case 'h':
                usage(argv[0]);
                exit(0);
                break;
            default:
                usage(argv[0]);
                exit(0);
                break;
        }
    }
    
    // type check
    int *input_arr = new int[input_key+1];
    for(int i=0; i<input_key; i++)
        input_arr[i] = i+1;

    if(strcmp(type, "r") == 0)
        random_arr(input_arr, input_key);

    input_arr[input_key] = 0;

    // level_sst_size tok
    level_sst_size = level_sst_size_tok(level_num, lss_str);

    // level create 
    list memtable;
    list level[level_num];
    list cpt;

    // result value
    int all_cpt_sst_cnt = 0;

    // input key range
    for(int i=0; i<=input_key; i++) {
        if(memtable.head == NULL || memtable.get_keycnt(memtable.tail) == sst_size) {
            // flush, key data sort
            if(memtable.get_size() == 1) {
                level[0].push(memtable.flush());
            }

            // compaction
            for(int j=0; j<level_num-1; j++) {
                if(level[j].get_size() > level_sst_size[j]) {
                    if(level[j+1].head != NULL) {

                        vector<int> vec;
                        bool cpt_check;
                       
                        // compaction check
                        cpt_check = compaction_check(level, cpt, j, vec, sst_size);

                        // compaction level push
                        level[j+1].push(level[j].pop());
                        read_cnt++;
                        write_cnt++;

                        // compaction check true / merge sort compaction
                        if(cpt_check) {
                            // compaction execution
                            compaction_exec(level, cpt, j, vec, sst_size);

                            // list init
                            cpt.init();
                            level[j+1].cpt_init();
                        }
                    } else {
                        level[j+1].push(level[j].pop());
                    }
                }
            }

            // last level limit
            //if(level[level_num-1].get_size() > level_sst_size[level_num-1])
            //    level[level_num-1].pop();

            // memtable input
            memtable.create(sst_size);
        }

        // key insert
        memtable.key_push(memtable.head, input_arr[i]);
        //level_print(memtable, level, level_num);
    } 

    level_print(memtable, level, level_num);

    cout << "I/O Count - Read : " << read_cnt << endl;
    cout << "I/O Count - Write : " << write_cnt << endl;

    delete input_arr;
    delete level_sst_size;

    return 0;
} 
