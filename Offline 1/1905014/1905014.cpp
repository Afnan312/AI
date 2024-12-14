#include<bits/stdc++.h>
using namespace std;

void printBoard(vector<int> matrix, int n)
{
    for(int i=0; i<matrix.size(); i++){
        cout<<matrix[i]<<" ";
        if((i+1)%n==0)
            cout<<endl;
    }
}


int inversionCount(vector<int> matrix, int n)
{
    int count=0;
    for (int i=0; i<n*n; i++)//////n*n is important!!!!!!!!!!!!!!
    {
        if(matrix[i]==0)
            continue;
        for (int j=i+1; j<n*n; j++)
        {
            if(matrix[j]==0)
                continue;
            if (matrix[i]> matrix[j])
            {
                count++;
            }
        }
    }
  return count;
}

bool solvable(vector<int> matrix, int n)//////n*n is important!!!!!!!!!!!!!!
{
    int inv=inversionCount(matrix, n);
    if(n%2!=0 && inv%2==0)
        return true;
    else if(n%2==0)
    {
        int pos;
        for(int i=0; i<n*n; i++)
        {
            if(matrix[i]==0)
            {
                pos=i;
                break;
            }
        }

        if( (inv+n-(pos/n + 1)) % 2 == 0)
            return true;
        return false;
    }
    else
        return false;
}

int hamming(vector<int> matrix, int n)
{

    int wrong=0;
    for(int i=0; i<n*n; i++)
    {
        if(matrix[i]==0)
            continue;

        if(matrix[i]!=i+1)
            wrong++;
    }
    return wrong;
}

int manhattan(vector<int> matrix, int n)
{
    int distance=0;
    for(int i=0; i<n*n; i++)
    {
        int x=matrix[i];
        if(x==0)
            continue;

        if(x!=i+1)
        {
            int row1=(x-1)/n;
            int col1=(x-1)-row1*n;

            int row2=i/n;
            int col2=i-row2*n;

            distance+=abs(row1-row2)+abs(col1-col2);
        }
    }
    return distance;
}

class searchNode
{
public:

    vector<int> current;
    searchNode* prev;
    int priority;
    int n;
    int moves;

    searchNode()
    {

    }
    searchNode(int n, vector<int>& current, int priority, int moves)
    {
        this->n=n;
        this->priority=priority;

        this->current.resize(n*n);
        for(int i=0; i<n*n; i++)
        {
            this->current[i]=current[i];
        }

        this->prev=nullptr;

        this->moves=moves;
    }

    searchNode(int n, vector<int>& current, searchNode* prev, int priority, int moves)
    {
        this->n=n;
        this->priority=priority;

        this->current=current;
        this->prev=prev;

        this->moves=moves;
    }
   /* ~searchNode(){
        this->prev=nullptr;
    }*/
};

struct compare
{
public:
    bool operator()(const searchNode* a, const searchNode* b)
    {
        if(a->priority>=b->priority)
            return true;
        else
            return false;
    }
};

void insertDesc(searchNode* node, vector<int> curr, priority_queue<searchNode*, vector<searchNode*>, compare>& open, vector<searchNode*>& close, bool hamm)
{
    for(searchNode* x:close)
    {
        if(curr==x->current)
            return;
    }
    int distance;
    if(hamm)
        distance=hamming(curr, node->n);
    else
        distance=manhattan(curr, node->n);
    int priority=node->moves+1+distance;

    searchNode* n=new searchNode(node->n, curr, node, priority, node->moves+1);
    open.push(n);
}

void findDesc(searchNode* node, priority_queue<searchNode*, vector<searchNode*>, compare>& open, vector<searchNode*>& close, bool hamm)
{
    int pos;
    vector<int> temcurr;
    temcurr.resize(node->n*node->n);

    for(int i=0; i<node->n*node->n; i++)
    {
        if(node->current[i]==0)
        {
            pos=i;
            break;
        }
    }
    int row=pos/node->n;
    int col=pos-row*node->n;


    if(row+1<(node->n))//down
    {
        temcurr=node->current;

        //printBoard(temcurr, node->n);
        temcurr[row*(node->n)+col]=temcurr[(row+1)*(node->n)+col];//put the lower cell in 0 cell
        temcurr[(row+1)*(node->n)+col]=0;
        if(node->prev==nullptr || !(temcurr==node->prev->current))
            insertDesc(node, temcurr, open, close, hamm);
    }
    if(row-1>=0)//up
    {
        temcurr=node->current;
        temcurr[row*(node->n)+col]=temcurr[(row-1)*(node->n)+col];//put the upper cell in 0 cell
        temcurr[(row-1)*(node->n)+col]=0;
        if(node->prev==nullptr || !(temcurr==node->prev->current))
            insertDesc(node, temcurr, open, close, hamm);
    }
    if(col+1<(node->n))//right
    {
        temcurr=node->current;
        temcurr[row*(node->n)+col]=temcurr[row*(node->n)+col+1];//put the right cell in 0 cell
        temcurr[row*(node->n)+col+1]=0;
        if(node->prev==nullptr || !(temcurr==node->prev->current))
            insertDesc(node, temcurr, open, close, hamm);
    }
    if(col-1>=0)//left
    {
        temcurr=node->current;
        temcurr[row*(node->n)+col]=temcurr[row*(node->n)+col-1];//put the left cell in 0 cell
        temcurr[row*(node->n)+col-1]=0;
        if(node->prev==nullptr || !(temcurr==node->prev->current))
            insertDesc(node, temcurr, open, close, hamm);
    }
}

void start(vector<int> matrix, int n, bool hamm)
{
    int stop=10;
    vector<int> goal;
    goal.resize(matrix.size());
    for(int i=0; i<matrix.size()-1; i++)
        goal[i]=i+1;
    goal[matrix.size()-1]=0;

    priority_queue<searchNode*, vector<searchNode*>, compare> open;
    vector<searchNode*> close;
    searchNode* first=new searchNode(n, matrix, 0, 0);
    open.push(first);
    while(true)
    {
        searchNode* currNode=open.top();
        open.pop();
        close.push_back(currNode);

        if(currNode->current==goal && !hamm)
        {
            cout<<"Minimum number of moves = "<<currNode->moves<<endl;
            vector<vector<int>> solution;
            while(currNode!=nullptr)
            {
                solution.push_back(currNode->current);
                currNode=currNode->prev;
            }
            for(int i=solution.size()-1; i>=0; i--){
                printBoard(solution[i], n);
                cout<<endl;
            }
            break;
        }
        else if(currNode->current==goal)
            break;
        //findDesc(currNode, open, close, true);
        findDesc(currNode, open, close, hamm);
    }
    if(hamm)
        cout<<"In hamming distance:"<<endl;
    else
        cout<<"In Manhattan distance:"<<endl;
    cout<<"Expanded nodes: "<<close.size()<<endl;
    cout<<"Explored nodes: "<<(close.size()+open.size())<<endl;
    for (int i = 0; i < close.size(); i++) {
        delete close[i];
    }
    close.clear();

    while(!open.empty()){
        searchNode* temp=open.top();
        open.pop();
        delete temp;
    }
}

int main()
{
    int n;
    cin>>n;

    vector<int> matrix;
    matrix.resize(n*n);//////n*n is important!!!!!!!!!!!!!!
    for(int i=0; i<n*n; i++)
    {
        cin>>matrix[i];
    }


    if(!(solvable(matrix, n)))
        cout<<"Unsolvable Puzzle"<<endl;
    else{
        start(matrix, n, false);//start by the manhattan distance
        start(matrix, n, true);//hamming distance
    }

    return 0;
}
