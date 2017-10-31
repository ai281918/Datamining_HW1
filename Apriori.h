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
#define mod 7
using namespace std;

struct Node{
    Node **children;
    vector<vector<int> > item_sets;
    vector<int> cnt;
};

map<string, int> id_m;
map<int, string> name_m;
vector<vector<int> > DB;
int id_count = 0;
