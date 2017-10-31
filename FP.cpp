#include "FP.h"

void Initialize()
{
    root = new T_node();
    ID_cnt.push_back(0);
}

bool cmp(int a, int b)
{
    return ID_cnt[a] > ID_cnt[b];
}

int GetID(string name)
{
    if(id_m[name] == 0){
        id_m[name] = ++id_count;
        name_m[id_count] = name;
        ID_cnt.push_back(1);
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

T_node* GetNodeChild(T_node *node, int id)
{
    id--;
    if(node -> children == NULL){
        node -> children = (T_node**)malloc(24);
        for(int i=0;i<id_count;++i){
            (node -> children)[i] = NULL;
        }
    }
    if((node -> children)[id] == NULL){
        (node -> children)[id] = new T_node();
        (node -> children)[id] -> value = id + 1;
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
            cout << GetName(v[i][j]) << " ";
        }
        puts("");
    }
    puts("=============================================================");
}

void PrintFPTree(T_node* current)
{
    cout << GetName(current -> value) << " " << current -> cnt << endl;
    if((current -> children) != NULL){
        for(int i=0;i<id_count;++i){
            if((current -> children)[i] != NULL){
                PrintFPTree((current -> children)[i]);
            }
        }
    }
}

void UpdateFPTree(vector<int> set, T_node* root)
{
    T_node* current = root;
    for(int i=0;i<set.size();++i){
        current = GetNodeChild(current, set[i]);
        (current -> cnt)++;
    }
}

int main()
{
	freopen("in.txt","r",stdin);
    Initialize();

    int tid, pre = -1, cnt=0;
    string name;
    vector<int> v;

    while(cin >> tid >> name){
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
    for(int i=0;i<DB.size();++i){
        sort(DB[i].begin(), DB[i].end(), cmp);
        UpdateFPTree(DB[i], root);
    }
    printVV(DB);
    PrintFPTree(root);

	return 0;
}