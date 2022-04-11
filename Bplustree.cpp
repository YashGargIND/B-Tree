#include<iostream>
#include<vector>
using namespace std;

class Bplustree
{
public:
    Bplustree(int D, int T){
        d = D;
        t = T;
        root = nullptr;
        tempRoot = new DataNode();
        numOfDataNodes = 1;
        numOfIndexNodes = 0;
    };
    ~Bplustree(){};
    void insert(int val){
        if (root == nullptr)
        {
            if (tempRoot->occupancy()<2*d)
            {
                tempRoot->insert(val);
                return;
            }
            else{
                tempRoot->insert(val);
                DataNode* tempDataNode = new DataNode();
                numOfDataNodes++;
                for (int i = d; i <= 2*d; i++)
                {
                    tempDataNode->values.push_back(tempRoot->values[i]);
                }
                for (int i = d; i <= 2*d; i++)
                {
                    tempRoot->values.pop_back();
                }
                root = new IndexNode(tempDataNode->values[0], tempRoot, tempDataNode);
                numOfIndexNodes++;
                tempRoot->next = tempDataNode;
                tempRoot->parent = root;
                tempDataNode->parent = root;
            }
        }
        else{
            IndexNode* cur;
            while(cur->isLevelone==false){
                bool flag=false;
                for (int i = 0; i < cur->occupancy(); i++)
                {
                    if (cur->values[i]<val)
                    {
                        cur = cur->IndexNodeAddress[i+1];
                        flag = true;
                        break;
                    }
                }
                if(flag == false){
                    cur = cur->IndexNodeAddress[0];
                }
            }
            DataNode* targetDataNode;
            bool flag=false;
            for (int i = 0; i < cur->occupancy(); i++)
                {
                    if (cur->values[i]<val)
                    {
                        targetDataNode = cur->DataNodeAddress[i+1];
                        flag = true;
                        break;
                    }
                }
            if(flag==false){
                targetDataNode = cur->DataNodeAddress[0];
            }
            targetDataNode->insert(val);
            if(targetDataNode->occupancy()>2*d){
                DataNode* tempDataNode = new DataNode();
                numOfDataNodes++;
                for (int i = d; i <= 2*d; i++)
                {
                    tempDataNode->values.push_back(targetDataNode->values[i]);
                }
                for (int i = d; i <= 2*d; i++)
                {
                    targetDataNode->values.pop_back();
                }
            tempDataNode->next = targetDataNode->next;
            targetDataNode->next = tempDataNode;
            tempDataNode->parent = targetDataNode->parent;
            tempDataNode->parent->insert(tempDataNode->values[0], tempDataNode);
            if(tempDataNode->parent->occupancy()>((2*t)+1)){
                cur = tempDataNode->parent;
                IndexNode* tempIndexNode = new IndexNode();
                numOfIndexNodes++;
                tempIndexNode->isLevelone=true;
                    for (int i = t+1; i <= 2*t+2; i++)
                    {
                        tempIndexNode->values.push_back(cur->values[i]);
                        tempIndexNode->DataNodeAddress.push_back(cur->DataNodeAddress[i]);
                    }
                    tempIndexNode->DataNodeAddress.push_back(cur->DataNodeAddress[((2*t)+2)]);
                    for (int i = t+1; i <= 2*t+2; i++)
                    {
                        cur->values.pop_back();
                        cur->DataNodeAddress.pop_back();
                    }
                    cur->DataNodeAddress.pop_back();
                    if(cur->parent==nullptr){
                        IndexNode* curParent  = new IndexNode(cur->values[t], cur, tempIndexNode);
                        numOfIndexNodes++;
                        return;
                    }
                    else{
                        cur->parent->insert(cur->values[t], cur);
                        if(cur->parent->occupancy()>((2*t)+1)){
                            while(true){
                            cur  = cur->parent;
                            IndexNode* tempIndexNode = new IndexNode();
                            numOfIndexNodes++;
                            for (int i = t+1; i <= 2*t+2; i++)
                            {
                                tempIndexNode->values.push_back(cur->values[i]);
                                tempIndexNode->IndexNodeAddress.push_back(cur->IndexNodeAddress[i]);
                            }
                            tempIndexNode->IndexNodeAddress.push_back(cur->IndexNodeAddress[((2*t)+2)]);
                            for (int i = t+1; i <= 2*t+2; i++)
                            {
                                cur->values.pop_back();
                                cur->IndexNodeAddress.pop_back();
                            }
                            cur->IndexNodeAddress.pop_back();
                            if(cur->parent==nullptr){
                                IndexNode* curParent  = new IndexNode(cur->values[t], cur, tempIndexNode);
                                numOfIndexNodes++;
                                return;
                            }
                            else{
                                cur->parent->insert(cur->values[t], cur);
                                if(cur->parent->occupancy()>((2*t)+1)){
                                    continue;
                                }
                                else{
                                    return;
                                }
                            }
                            }
                        }
                        else{
                            return;
                        }
                    }
                }
            }
        }  
    }
    // void printStatus(){
    //     cout<<numOfDataNodes<<" "<<numOfIndexNodes<<" ";
    //     for (auto root->)
    //     {s
    //         /* code */
    //     }
        
    // }
private:
    int d;
    int t;
    IndexNode* root;
    DataNode* tempRoot;
    int numOfDataNodes;
    int numOfIndexNodes;
    
};

class DataNode{
    public:
        DataNode(){
            next = nullptr;
            parent = nullptr;
        }
        DataNode(int val){
            values.push_back(val);
            next = nullptr;
            parent = nullptr;

        }
        DataNode(int val, DataNode* nextptr){
            values.push_back(val);
            next = nextptr;
            parent = nullptr;
        }
        ~DataNode(){
        }
        void insert(int val){
            for (int i = 0; i < values.size(); i++)
            {                
                if (values[i]<val)
                {
                    values.emplace(values.begin()+ i, val);
                    return;
                }
            }
            values.push_back(val);
            return;   
        }
        int occupancy(){
            return values.size();
        }
        vector<int> values;
        DataNode* next;
        IndexNode* parent;
    private:
};

class IndexNode{
    public:
        IndexNode(int val, DataNode* LDataNode, DataNode* RDataNode){
            values.push_back(val);
            DataNodeAddress.push_back(LDataNode);
            DataNodeAddress.push_back(RDataNode);
            isLevelone = true;
            parent = nullptr;
        }
        IndexNode(int val, IndexNode* LIndexNode, IndexNode* RIndexNode){
            values.push_back(val);
            IndexNodeAddress.push_back(LIndexNode);
            IndexNodeAddress.push_back(RIndexNode);
            isLevelone = false;
            parent = nullptr;
        }
        IndexNode(){
            parent = nullptr;
            isLevelone = false;
        }
        ~IndexNode(){
        }
        void insert(int val, IndexNode* node){
            for (int i = 0; i < values.size(); i++)
            {                
                if (values[i]<val)
                {
                    values.emplace(values.begin()+ i, val);
                    IndexNodeAddress.emplace(IndexNodeAddress.begin() + i, node);
                    return;
                }
            }
            values.push_back(val);
            IndexNodeAddress.push_back(node);
            return;   
        }
        void insert(int val, DataNode* node){
            for (int i = 0; i < values.size(); i++)
            {                
                if (values[i]<val)
                {
                    values.emplace(values.begin()+ i, val);
                    DataNodeAddress.emplace(DataNodeAddress.begin() + i, node);
                    return;
                }
            }
            values.push_back(val);
            DataNodeAddress.push_back(node);
            return;   
        }
        int occupancy(){
            return values.size();
        }
        vector<int> values;
        vector<DataNode*> DataNodeAddress;
        vector<IndexNode*> IndexNodeAddress;
        IndexNode* parent;
        int size;
        bool isLevelone;
    private:
};

int main()
{
    vector<int> x;
    for (int  i = 0; i < 3; i++)
    {   
        int y = 1;
        x.push_back(y++);
    }
    x.emplace(x.begin()+0, 10);
    for (auto val : x)
    {
        cout<<val<<" ";
    }
    
    return 0;
}