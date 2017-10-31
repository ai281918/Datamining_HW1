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

struct T_node{
    T_node *children[mod] = {0};
    vector<vector<int> > item_sets;
    vector<int> cnt;
};

T_node *root;
map<string, int> id_m;
map<int, string> name_m;
int id_count = 0;

void Initialize();
int GetID(string name);
string GetName(int id);
void printV(vector<int> &v);
void printVV(vector<int> &v);
void PrintVV_V(vector<vector<int> > &sets, vector<int> &cnt, double mf);
void PrintRule(vector<int> &set, vector<int> &l, double mf);
void UpdateHashTree(vector<int> &set, T_node *r);
void PrintHashTree(T_node *p);
void EnumSubSet(vector<int> &set, vector<int> &sub_set, int cur, int cnt, int num);
int GetCntFromTree(vector<int> v, T_node *r);
bool IsInTree(T_node *r, vector<int> &v);
void GenerateCandidates(vector<vector<int> > &sets, vector<int> &cnt, int ms);
void ComputeConf(vector<int> &set, vector<int> &l_set, int cur, int cnt, int num, double mf);
void GenerateRule(vector<int> &ls, double mf);
void Apriori(int ms, double mf);