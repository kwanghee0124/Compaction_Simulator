#ifndef COMPACTION_H
#define COMPACTION_H

#include "list.h"

extern int read_cnt;
extern int write_cnt;

bool compaction_check(list *level, list &cpt, int lnum, std::vector<int> &vec, int sst_size);
void compaction_exec(list *level, list &cpt, int lnum, std::vector<int> &vec, int sst_size);

#endif
