#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <string>
#include <iostream>
#include <queue>
#include <climits>
#include <stack>
#include <map>
#include <windows.h>
using namespace std;

struct Node{
    map<int, Node*> children;
    Node *parent = NULL;
    int cnt = 0;
    int value = 0;
    bool visit = false;
};

int ID_cnt[1000];
map<string, int> id_m;
map<int, string> name_m;
int id_count = 0;

void FPTreeRetrieval(int id, Node *candidate_root, vector<vector<Node*> > &nodes, int ms, float mf, vector<int> &cond);
void FPTreeMining(vector<vector<Node*> > &nodes, int ms, float mf, vector<int> &cond, Node *candidate_root);