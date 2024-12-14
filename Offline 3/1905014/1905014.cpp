#include<bits/stdc++.h>
#include<cstdlib>
#include<time.h>
using namespace std;


struct edge
{
    int from;
    int to;
    int weight;
};
class Graph
{
public:
    vector<edge> edgeList;
    int adjMat[1001][1001]={0};
    int nodes;
    int maxNode1;
    int maxNode2;
    int maxWeightEdge;
    int minWeightEdge;

    Graph(int n);
    void addEdge(int first, int second, int weight);
};

Graph::Graph(int n)
{
    nodes=n;
    maxNode1=0;
    maxNode2=0;
    maxWeightEdge=INT_MIN;
    minWeightEdge=INT_MAX;
}

void Graph::addEdge(int first, int second, int weight)
{
    edgeList.push_back({first, second, weight});
    adjMat[first][second]=weight;
    adjMat[second][first]=weight;
    if(weight>maxWeightEdge)
    {
        maxWeightEdge=weight;
        maxNode1=first;
        maxNode2=second;
    }
    if(weight<minWeightEdge)
    {
        minWeightEdge=weight;
    }
}

class solution
{
public:
    list<int> sCut;
    list<int> tCut;
    int totalWeight=0;
    int iteration=0;
};

int findEdgeSum(int node, list<int> cut, Graph graph)
{
    int sum=0;
    for(int num:cut)
    {
        sum+=graph.adjMat[node][num];
    }
    return sum;
}

solution localSearch(Graph graph, solution x)
{
    bool change=true;
    while(change)
    {
        change=false;
        for(int i=1; i<=graph.nodes; i++)
        {
            if(change)
                break;
            bool s=false;
            for(int num:x.sCut)
            {
                if(num==i)
                {
                    s=true;
                    break;
                }
            }
            //find sum of distance from v to all u in tcut
            int sumScut=findEdgeSum(i, x.tCut, graph);

            //find sum of distance from v to all u in scut
            int sumTcut=findEdgeSum(i, x.sCut, graph);

            if(s && (sumTcut-sumScut)>0)
            {
                x.sCut.remove(i);
                x.tCut.push_back(i);
                x.totalWeight+=sumTcut-sumScut;
                x.iteration++;
                change=true;
            }
            else if(!s && (sumScut-sumTcut)>0){
                x.tCut.remove(i);
                x.sCut.push_back(i);
                x.totalWeight+=sumScut-sumTcut;
                x.iteration++;
                change=true;
            }
        }

    }
    return x;
}

solution semiGreedy(Graph graph, double alpha)
{
    solution sgsolution;
    int wmax=graph.maxWeightEdge;
    int wmin=graph.minWeightEdge;
    double cutoff=wmin+alpha*1.0*(wmax-wmin);

    vector<edge> rclE;
    for(int i=0; i<graph.edgeList.size(); i++)
    {
        if(graph.edgeList[i].weight>=cutoff)
            rclE.push_back({graph.edgeList[i].from, graph.edgeList[i].to, graph.edgeList[i].weight});
    }

    int chosen=rand()%rclE.size();
    sgsolution.sCut.push_back(rclE[chosen].from);
    sgsolution.tCut.push_back(rclE[chosen].to);
    sgsolution.totalWeight+=rclE[chosen].weight;
    int xSize=1, ySize=1;

    list<int> vertices;
    for(int i=1; i<=graph.nodes; i++)
    {
        vertices.push_back(i);
    }

    vertices.remove(rclE[chosen].from);
    vertices.remove(rclE[chosen].to);

    int xvalues[1001]={0};
    int yvalues[1001]={0};

    while(xSize+ySize!=graph.nodes)
    {
        int minX=INT_MAX, minY=INT_MAX, maxX=INT_MIN, maxY=INT_MIN;
        int sum1, sum2;
        for(int i:vertices)
        {
            //find sum of distance from i to nodes in tcut and store in xvalues
            sum1=findEdgeSum(i, sgsolution.tCut, graph);
            xvalues[i]=sum1;
            if(sum1<minX)
                minX=sum1;
            if(sum1>maxX)
                maxX=sum1;

            //find sum of distance from i to nodes in scut and store in yvalues
            sum2=findEdgeSum(i, sgsolution.sCut, graph);
            yvalues[i]=sum2;
            if(sum2<minY)
                minY=sum2;
            if(sum2>maxY)
                maxY=sum2;
        }
        wmin=min(minX, minY);
        wmax=max(maxX, maxY);
        cutoff=wmin+alpha*1.0*(wmax-wmin);

        vector<int> rclV;
        for(int i:vertices)
        {
            if(max(xvalues[i], yvalues[i])>=cutoff)
                rclV.push_back(i);
        }

        chosen=rand()%rclV.size();
        int selectedNode=rclV[chosen];


        if(xvalues[selectedNode]>yvalues[selectedNode]){
            sgsolution.sCut.push_back(selectedNode);
            sgsolution.totalWeight+=xvalues[selectedNode];
            xSize++;
        }
        else{
            sgsolution.tCut.push_back(selectedNode);
            sgsolution.totalWeight+=yvalues[selectedNode];
            ySize++;
        }
        vertices.remove(selectedNode);
    }
    return sgsolution;
}

solution grasp(Graph graph, int iterations, double alpha)
{
    int bestWeight=0;
    solution solve;
    for(int i=0; i<iterations; i++)
    {
        solution x=semiGreedy(graph, alpha);
        x=localSearch(graph, x);
        if(x.totalWeight>bestWeight)
        {
            solve=x;
            bestWeight=x.totalWeight;
        }
    }
    return solve;
}

solution randomized(Graph graph)
{
    solution randomsolution;
    for(int i=1; i<=graph.nodes; i++)
    {
        int chosen=rand()%2;
        if(chosen==0)
        {
            randomsolution.sCut.push_back(i);
            randomsolution.totalWeight+=findEdgeSum(i, randomsolution.tCut, graph);
        }
        else
        {
            randomsolution.tCut.push_back(i);
            randomsolution.totalWeight+=findEdgeSum(i, randomsolution.sCut, graph);
        }
    }
    return randomsolution;
}

int main()
{
    int n, m;
    int v1, v2, wt;
    cin>>n;
    cin>>m;
    Graph graph(n);
    for(int i=0; i<m; i++)
    {
        cin>>v1;
        cin>>v2;
        cin>>wt;
        graph.addEdge(v1, v2, wt);
    }

    solution first;
    int sumR=0;

    //randomized
    for(int i=0; i<=10; i++){
        first=randomized(graph);
        sumR+=first.totalWeight;
    }
    cout<<"Randomized: "<<sumR/10<<endl;

    //alpha=0.2
    first=semiGreedy(graph, 0.2);
    cout<<"Semi-greedy(0.2): "<<first.totalWeight<<endl;

    //alpha=0.5
    first=semiGreedy(graph, 0.5);
    cout<<"Semi-greedy(0.5): "<<first.totalWeight<<endl;

    //greedy
    first=semiGreedy(graph, 1.0);
    cout<<"Greedy: "<<first.totalWeight<<endl;

    //local search
    first=localSearch(graph, first);
    cout<<"Local search iterations: "<<first.iteration<<endl;
    cout<<"Local search result: "<<first.totalWeight<<endl;

    //grasp
    first=grasp(graph, 5, 0.8);
    cout<<"GRASP: "<<first.totalWeight<<endl;

    return 0;
}
