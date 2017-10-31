#include "Test.h"

void Initialize()
{
    root = new T_node();
}

int GetID(string name)
{
    if(id_m[name] == 0){
        id_m[name] = ++id_count;
        name_m[id_count] = name;
    }
    return id_m[name];
}

string GetName(int id)
{
    if(id <= 0 || id > id_count){
        return "QQ";
    }
    return name_m[id];
}

void printV(vector<int> &v)
{
    for(int i=0;i<v.size();++i){
        cout << v[i] << " ";
    }
    puts("");
}

void printVV(vector<vector<int> > &v)
{
    puts("=============================================================");
    for(int i=0;i<v.size();++i){
        for(int j=0;j<v[i].size();++j){
            cout << v[i][j] << " ";
        }
        puts("");
    }
    puts("=============================================================");
}

void PrintVV_V(vector<vector<int> > &sets, vector<int> &cnt, double mf)
{
    if(!sets.size()) return;

    puts("=============================================================");
    for(int i=0;i<sets.size();++i){
        for(int j=0;j<sets[i].size();++j){
            cout << GetName(sets[i][j]) << " ";
        }
        cout << ": " << cnt[i];
        puts("");
        puts("-------------------------------------------------------------");
        GenerateRule(sets[i], mf);
        puts("");
    }
    puts("=============================================================");
    puts("");
}

void PrintRule(vector<int> &set, vector<int> &l, double mf)
{
    if(double(GetCntFromTree(set, root)) / GetCntFromTree(l, root) < mf){
        return;
    }

    vector<int> r;
    for(int i=0, j=0;i<set.size();++i){
        //cout << i << " " << j << endl;
        if(j < l.size() && set[i] == l[j]){
            j++;
        }
        else{
            r.push_back(set[i]);
        }
    }

    cout << "{";
    for(int i=0;i<l.size();++i){
        if(i) cout << ", ";
        cout << GetName(l[i]);
    }
    cout << "} --> {";
    for(int i=0;i<r.size();++i){
        if(i) cout << ", ";
        cout << GetName(r[i]);
    }
    cout << "} : ";
    cout << double(GetCntFromTree(set, root)) / GetCntFromTree(l, root) << endl;;
}

void UpdateHashTree(vector<int> &set, T_node *r)
{
    T_node *current = r;
    for(int i=0;i<set.size();++i){
        if(current -> children[set[i]%mod] == NULL){
            current -> children[set[i]%mod] = new T_node();
        }
        current = current -> children[set[i]%mod];
    }
    
    bool flag;
    int i;
    for(i=0;i<(current -> item_sets).size();++i){
        flag = false;
        for(int j=0;j<(current -> item_sets[i]).size();++j){
            if(current -> item_sets[i][j] != set[j]){
                flag = true;
                break;
            }
        }
        if(!flag) break;
    }
    if((current -> item_sets).size() == 0){
        flag = true;
    }
    
    if(!flag){
        (current -> cnt)[i]++;
    }
    else{
        (current -> item_sets).push_back(set);
        (current -> cnt).push_back(1);
    }
}

void PrintHashTree(T_node *p)
{
    for(int i=0;i<(p -> item_sets).size();++i){
        for(int j=0;j<(p -> item_sets)[i].size();++j){
            cout << GetName((p -> item_sets)[i][j]) << " ";
        }
        cout << p -> cnt[i];
        puts("");
    }
    for(int i=0; i<mod;++i){
        if(p->children[i] != NULL){
            PrintHashTree(p->children[i]);
        }
    }
}

void EnumSubSet(vector<int> &set, vector<int> &sub_set, int cur, int cnt, int num)
{
    if(cnt == num){
        UpdateHashTree(sub_set, root);
    }
    if(set.size() - cur < num - cnt) return;

    for(int i=cur;i<set.size();++i){
        sub_set.push_back(set[i]);
        EnumSubSet(set, sub_set, i+1, cnt+1, num);
        sub_set.erase(sub_set.end()-1);
    }
}

int GetCntFromTree(vector<int> v, T_node *r)
{
    T_node *cur = r;
    for(int i=0;i<v.size();++i){
        if((cur -> children)[v[i] % mod] == NULL){
            return 0;
        }
        else{
            cur = (cur -> children)[v[i] % mod];
        }
    }

    bool flag;
    int i;
    for(i=0;i<(cur -> item_sets).size();++i){
        flag = false;
        for(int j=0;j<(cur -> item_sets[i]).size();++j){
            if(cur -> item_sets[i][j] != v[j]){
                flag = true;
                break;
            }
        }
        if(!flag) break;
    }
    if((cur -> item_sets).size() == 0){
        flag = true;
    }
    
    if(!flag){
        return (cur -> cnt)[i];
    }
    return 0;
}

bool IsInTree(T_node *r, vector<int> &v)
{
    T_node *cur = r;
    for(int i=0;i<v.size();++i){
        if((cur -> children)[v[i] % mod] == NULL){
            return false;
        }
        else{
            cur = (cur -> children)[v[i] % mod];
        }
    }
    
    bool flag;
    for(int i=0;i<(cur -> item_sets).size();++i){
        flag = false;
        for(int j=0;j<v.size();++j){
            if(v[j] != (cur -> item_sets)[i][j]){
                flag = true;
                break;
            }
        }
        if(flag == false) break;
    }

    if(flag == false && (cur -> item_sets).size()) return true;
    return false;
}

void GenerateCandidates(vector<vector<int> > &sets, vector<int> &cnt, int ms)
{
    T_node *r = new T_node();
    vector<vector<int> > res;
    cnt.clear();
    for(int i=0;i<sets.size();++i){
        for(int j=i+1;j<sets.size();++j){
            if(i == j) continue;
            vector<int> uni;
            set_union(sets[i].begin(), sets[i].end(), sets[j].begin(), sets[j].end(), back_inserter(uni));
            sort(uni.begin(), uni.end());
            if(uni.size() - sets[0].size() == 1 && GetCntFromTree(uni, root) >= ms && !IsInTree(r, uni)){
                res.push_back(uni);
                cnt.push_back(GetCntFromTree(uni, root));
                UpdateHashTree(uni, r);
            }
        }
    }

    sets = res;
}

void ComputeConf(vector<int> &set, vector<int> &l_set, int cur, int cnt, int num, double mf)
{
    if(cnt == num){
        PrintRule(set, l_set, mf);
    }
    if(set.size() - cur < num - cnt) return;

    for(int i=cur;i<set.size();++i){
        l_set.push_back(set[i]);
        ComputeConf(set, l_set, i+1, cnt+1, num, mf);
        l_set.erase(l_set.end()-1);
    }
}

void GenerateRule(vector<int> &ls, double mf)
{
    vector<int> l;
    for(int i=1;i<ls.size();++i){
        ComputeConf(ls, l, 0, 0, i, mf);
    }
}

void Apriori(int ms, double mf)
{
    vector<vector<int> > cur_set;
    vector<int> cur_cnt;
    T_node* cur;
    
    for(int i=0;i<mod;++i){
        cur = (root -> children)[i];
        for(int j=0;j<(cur -> item_sets).size();++j){
            if((cur -> cnt)[j] >= ms){
                cur_set.push_back((cur -> item_sets)[j]);
                cur_cnt.push_back((cur -> cnt)[j]);
            }
        }
    }
    PrintVV_V(cur_set, cur_cnt, mf);
    while(!cur_set.empty()){
        GenerateCandidates(cur_set, cur_cnt, ms);
        PrintVV_V(cur_set, cur_cnt, mf);
    }
}

int main()
{
	freopen("data.ntrans_1.nitems_0.1","r",stdin);
    Initialize();

    int tid, pre = -1, gg;
    string name;
    vector<int> set, sub_set;
    while(cin >> tid >> gg >> name){
        if(tid != pre){
            cout << pre << endl;
            if(pre != -1){
                sort(set.begin(), set.end());
                for(int i=0;i<set.size();++i){
                    EnumSubSet(set, sub_set, 0, 0, i+1);
                }
                set.clear();
            }
            pre = tid;
            if(tid >= 300) break;
        }
        set.push_back(GetID(name));
    }
    sort(set.begin(), set.end());
    for(int i=0;i<set.size();++i){
        EnumSubSet(set, sub_set, 0, 0, i+1);
    }
    Apriori(3, 0.6);
    //PrintHashTree(root);

	return 0;
}
