#include "Apriori.h"

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

Node* GetNodeChild(Node *node, int id)
{
    id %= mod;
    if(node -> children == NULL){
        node -> children = (Node**)malloc(sizeof(Node*) * mod);
        for(int i=0;i<mod;++i){
            (node -> children)[i] = NULL;
        }
    }
    if((node -> children)[id] == NULL){
        (node -> children)[id] = new Node();
    }
    return (node -> children)[id];
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
        // puts("-------------------------------------------------------------");
        // GenerateRule(sets[i], mf);
        // puts("");
    }
    puts("=============================================================");
    puts("");
}

void PrintCandidateTree(Node *current)
{
    puts("=============================================================");
    for(int i=0;i<(current -> item_sets).size();++i){
        for(int j=0;j<(current -> item_sets)[i].size();++j){
            cout << (current -> item_sets)[i][j] << " ";
        }
        cout << (current -> cnt)[i] << endl;
    }
    puts("=============================================================");
    if(current -> children != NULL){
        for(int i=0;i<mod;++i){
            if((current -> children)[i] != NULL){
                PrintCandidateTree((current -> children)[i]);
            }
        }
    }
}

void AddCandidateToTree(vector<int> &set, Node *root)
{
    Node *current = root;
    for(int i=0;i<set.size();++i){
        current = GetNodeChild(current, set[i]);
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
    
    if(flag){
        (current -> item_sets).push_back(set);
        (current -> cnt).push_back(0);
    }
}

Node* BuildCandidateTree(vector<vector<int> > &candidates)
{
    Node* root = new Node();
    for(int i=0;i<candidates.size();++i){
        AddCandidateToTree(candidates[i], root);
    }
    return root;
}

void UpdateCandidateTreeCount(vector<int> &set, Node *root)
{
    Node *current = root;
    for(int i=0;i<set.size();++i){
        current = GetNodeChild(current, set[i]);
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
}

void EnumSubsetAndUpdateCandidateCnt(vector<int> &set, vector<int> &sub_set, int cur, int cnt, int num, Node *root)
{
    if(cnt == num){
        UpdateCandidateTreeCount(sub_set, root);
    }
    if(set.size() - cur < num - cnt) return;

    for(int i=cur;i<set.size();++i){
        sub_set.push_back(set[i]);
        EnumSubsetAndUpdateCandidateCnt(set, sub_set, i+1, cnt+1, num, root);
        sub_set.erase(sub_set.end()-1);
    }
}

void CountCandidate(Node *root, int num)
{
    vector<int> subset;

    for(int i=0;i<DB.size();++i){
        EnumSubsetAndUpdateCandidateCnt(DB[i], subset, 0, 0, num, root);
    }
}

void DeleteTree(Node *current)
{
    if(current == NULL) return;

    if(current -> children != NULL){
        for(int i=0;i<mod;++i){
            if((current -> children)[i] != NULL){
                DeleteTree((current -> children)[i]);
            }
        }
    }
    
    delete current;
}

void RetriveCandidate(Node *current, vector<vector<int> > &candidates, vector<int> &cnt, int ms, int cur, int num)
{
    if(cur <= num && (current -> children) != NULL){
        for(int i=0;i<mod;++i){
            if((current -> children)[i] != NULL){
                RetriveCandidate((current -> children)[i], candidates, cnt, ms, cur + 1, num);
            }
        }
    }
    else if(cur == num){
        for(int i=0;i<(current -> item_sets).size();++i){
            if((current -> cnt)[i] >= ms){
                candidates.push_back((current -> item_sets)[i]);
                cnt.push_back((current -> cnt)[i]);
            }
        }
    }
}

void BuildNextCandidateTree(vector<vector<int> > &sets, Node *root)
{
    vector<int> uni;
    int cnt;

    for(int i=0;i<sets.size();++i){
        for(int j=i+1;j<sets.size();++j){
            if(i == j) continue;
            cnt = 0;
            uni.clear();
            for(int k=0;k<sets[i].size();++k){
                if(sets[i][k] != sets[j][k]){
                    if(++cnt == 2) break;
                    if(sets[i][k] > sets[j][k]){
                        uni.push_back(sets[j][k]);
                        uni.push_back(sets[i][k]);
                    }
                    else{
                        uni.push_back(sets[i][k]);
                        uni.push_back(sets[j][k]);
                    }
                }
                else{
                    uni.push_back(sets[i][k]);
                }
            }

            if(cnt == 1){
                AddCandidateToTree(uni, root);
            }
        }
    }
}

int GetCntFromTree(vector<int> &set, Node *root)
{
    Node *current = root;
    for(int i=0;i<set.size();++i){
        current = GetNodeChild(current, set[i]);
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
        return (current -> cnt)[i];
    }
    else{
        return 0;
    }
}

void PrintRule(vector<int> &set, vector<int> &l, double mf, Node *root)
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

void ComputeConf(vector<int> &set, vector<int> l, int cur, int cnt, int num, double mf, Node *root)
{
    if(cnt == num){
        PrintRule(set, l, mf, root);
    }
    if(set.size() - cur < num - cnt) return;

    for(int i=cur;i<set.size();++i){
        l.push_back(set[i]);
        ComputeConf(set, l, i+1, cnt+1, num, mf, root);
        l.erase(l.end()-1);
    }
}

void GenerateRule(vector<vector<int> > &candidates, vector<int> &cnt, double mf, Node* root)
{
    if(!candidates.size()) return;

    vector<int> l;
    puts("=============================================================");
    for(int i=0;i<candidates.size();++i){
        for(int j=0;j<candidates[i].size();++j){
            cout << GetName(candidates[i][j]) << " ";
        }
        cout << ": " << cnt[i];
        puts("");
        puts("-------------------------------------------------------------");
        for(int j=1;j<candidates[i].size();++j){
            ComputeConf(candidates[i], l, 0, 0, j, mf, root);
        }
        puts("-------------------------------------------------------------");
    }
    puts("=============================================================");
    puts("");
}

void Apriori(int ms, double mf)
{
    vector<vector<int> > candidates;
    vector<int> set, cnt;
    Node *root = NULL;
    
    for(int i=1;i<=id_count;++i){
        set.push_back(i);
        candidates.push_back(set);
        set.clear();
    }
    root = BuildCandidateTree(candidates);
    
    int num = 0;
    while(candidates.size()){
        CountCandidate(root, ++num);
        candidates.clear();
        cnt.clear();
        RetriveCandidate(root, candidates, cnt, ms, 0, num);
        GenerateRule(candidates, cnt, mf, root);
        BuildNextCandidateTree(candidates, root);
    }
    DeleteTree(root);
}

int main(int argc, char **argv)
{
    freopen("data.ntrans_1.nitems_0.1","r",stdin);
    // freopen("out.txt","w",stdout);

    int tid, pre = -1, gg, ms;
    float mf;
    if(argc == 2){
        ms = atoi(argv[1]);
    }
    else if(argc == 3){
        ms = atoi(argv[1]);
        mf = atof(argv[2]);
    }
    else{
        ms = 5;
        mf = 0.2;
    }

    string name;
    vector<int> set;
    while(cin >> tid >> gg >> name){
        if(tid != pre){
            //cout << pre << endl;
            if(pre != -1){
                sort(set.begin(), set.end());
                DB.push_back(set);
                set.clear();
            }
            pre = tid;
            // if(tid >= 100) break;
        }
        set.push_back(GetID(name));
    }
    sort(set.begin(), set.end());
    DB.push_back(set);
    Apriori(ms, mf);

	return 0;
}
