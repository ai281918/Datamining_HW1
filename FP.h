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
using namespace std;

struct T_node{
    T_node **children = NULL;
    int cnt = 0;
    int value = 0;
};

T_node* root;
vector<vector<int> > DB;
vector<int> ID_cnt;
map<string, int> id_m;
map<int, string> name_m;
int id_count = 0;