#include "FP.h"

void Initialize()
{
    memset(ID_cnt, 0, sizeof(ID_cnt));
}

bool cmp(int a, int b)
{
    return ID_cnt[a] > ID_cnt[b];
}

bool cmp2(int a, int b)
{
    return ID_cnt[a] < ID_cnt[b];
}

int GetID(string name)
{
    if(id_m[name] == 0){
        id_m[name] = ++id_count;
        name_m[id_count] = name;
    }
    return id_m[name];
}

void UpdataCnt(string name)
{
    ID_cnt[GetID(name)]++;
}

string GetName(int id)
{
    if(id <= 0 || id > id_count){
        return "QQ";
    }
    return name_m[id];
}

void UpdateNodes(vector<vector<Node*> > &nodes, int id, Node *node)
{
    while(id >= nodes.size()){
        vector<Node*> v;
        nodes.push_back(v);
    }
    nodes[id].push_back(node);
}

Node* GetFPNodeChild(Node *node, int id, vector<vector<Node*> > &nodes)
{
    if((node -> children)[id] == NULL){
        (node -> children)[id] = new Node();
        (node -> children)[id] -> value = id;
        (node -> children)[id] -> parent = node;
        UpdateNodes(nodes, id, (node -> children)[id]);
    }
    return (node -> children)[id];
}

Node* GetCandidateNodeChild(Node *node, int id)
{
    if((node -> children)[id] == NULL){
        (node -> children)[id] = new Node();
        (node -> children)[id] -> value = id;
        (node -> children)[id] -> parent = node;
    }
    return (node -> children)[id];
}

void printV(vector<int> &v)
{
    for(int i=0;i<v.size();++i){
        cout << GetName(v[i]) << " ";
    }
    puts("");
}

void printVV(vector<vector<int> > &v)
{
    puts("=============================================================");
    for(int i=0;i<v.size();++i){
        for(int j=0;j<v[i].size();++j){
            cout << GetName(v[i][j]) << " ";
        }
        puts("");
    }
    puts("=============================================================");
}

void PrintFPTree(Node* current)
{
    cout << GetName(current -> value) << " " << current -> cnt << endl;
    map<int, Node*>::iterator iter;

    for(iter = (current -> children).begin(); iter != (current -> children).end(); iter++){
        PrintFPTree(iter -> second);
    }
}

void UpdateFPTree(vector<int> &set, Node* root, int ms, vector<vector<Node*> > &nodes, int n)
{
    Node* current = root;
    for(int i=0;i<set.size();++i){
        if(ID_cnt[set[i]] < ms) break;
        current = GetFPNodeChild(current, set[i], nodes);
        (current -> cnt) += n;
    }
}

int GetCntFromTree(vector<int> &set, Node *root)
{
    Node *current = root;
    for(int i=0;i<set.size();++i){
        current = GetCandidateNodeChild(current, set[i]);
    }
    return current -> cnt;
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

void GenerateRule(vector<int> &candidate, int cnt, double mf, Node* root)
{
    vector<int> l;
    // puts("=============================================================");
    for(int i=0;i<candidate.size();++i){
        cout << GetName(candidate[i]) << " ";
    }
    cout << ": " << cnt << endl;
    // puts("-------------------------------------------------------------");
    // for(int i=1;i<candidate.size();++i){
    //     ComputeConf(candidate, l, 0, 0, i, mf, root);
    // }
    // puts("=============================================================");
}

void PrintCandidateTree(Node *root, int ms, float mf)
{
    queue<vector<int> > vq;
    queue<Node*> nq;
    vector<int> s;
    Node *cur, *next;
    vq.push(s);
    nq.push(root);

    while(!nq.empty()){
        cur = nq.front();nq.pop();
        s = vq.front();vq.pop();

        GenerateRule(s, cur -> cnt, mf, root);

        map<int, Node*>::iterator iter;
        for(iter = (cur -> children).begin(); iter != (cur -> children).end(); iter++){
            next = iter -> second;
            s.push_back(next -> value);
            nq.push(next);
            vq.push(s);
            s.erase(s.end()-1);
        }
    }
}

void UpdateCandidateTree(Node *root, vector<int> s, int m)
{
    Node *current = root;
    sort(s.begin(), s.end());
    for(int i=0;i<s.size();++i){
        current = GetCandidateNodeChild(current, s[i]);
    }
    (current -> cnt) += m;
}

vector<int> FindPath(Node *node, int n)
{
    vector<int> path;
    node = node -> parent;
    while(node -> value){
        path.push_back(node -> value);
        ID_cnt[node -> value] += n;
        node = node -> parent;
    }
    return path;
}

void FPTreeRetrieval(int id, Node *candidate_root, vector<vector<Node*> > &nodes, int ms, float mf, vector<int> &cond)
{
    vector<vector<int> > paths;
    vector<vector<Node*> > sub_nodes;

    memset(ID_cnt, 0, sizeof(ID_cnt));
    Node* root = new Node();

    for(int i=0;i<nodes[id].size();++i){
        UpdateCandidateTree(candidate_root, cond, nodes[id][i] -> cnt);
        paths.push_back(FindPath(nodes[id][i], nodes[id][i] -> cnt));
    }
    for(int i=0;i<paths.size();++i){
        sort(paths[i].begin(), paths[i].end(), cmp);
        UpdateFPTree(paths[i], root, ms, sub_nodes, nodes[id][i] -> cnt);
    }

    FPTreeMining(sub_nodes, ms, mf, cond, candidate_root);
}

void FPTreeMining(vector<vector<Node*> > &nodes, int ms, float mf, vector<int> &cond, Node *candidate_root)
{
    vector<int> items;
    for(int i=1;i<=id_count;++i){
        if(ID_cnt[i] >= ms){
            items.push_back(i);
        }
    }
    sort(items.begin(), items.end(), cmp2);

    for(int i=0;i<items.size();++i){
        cond.push_back(items[i]);
        FPTreeRetrieval(items[i], candidate_root, nodes, ms, mf, cond);
        cond.erase(cond.end()-1);
    }
}

int main(int argc, char **argv)
{
    freopen("data.ntrans_1.nitems_0.1","r",stdin);
    //freopen("in2.txt","r",stdin);
    Initialize();

    int tid, pre = -1, cnt=0, gg, ms;
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
        mf = 0.5;
    }

    string name;
    vector<int> v;
    Node* root = new Node();
    Node *candidate_root = new Node();
    vector<vector<int> > DB;
    vector<vector<Node*> > nodes;

    while(cin >> tid >> gg >> name){
        if(tid != pre){
            if(pre != -1){
                DB.push_back(v);
            }
            v.clear();
            pre = tid;
        }
        v.push_back(GetID(name));
        UpdataCnt(name);
    }
    DB.push_back(v);
    for(int i=0;i<DB.size();++i){
        sort(DB[i].begin(), DB[i].end(), cmp);
        UpdateFPTree(DB[i], root, ms, nodes, 1);
    }

    int t = GetTickCount();
    vector<int> cond;
    FPTreeMining(nodes, ms, mf, cond, candidate_root);
    PrintCandidateTree(candidate_root, ms, mf);
    cout << "Time : " << (double)(GetTickCount() - t) / 1000 << endl;

	return 0;
}