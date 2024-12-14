#include<bits/stdc++.h>
#include<time.h>

using namespace std;

class Node{
public:
    string attributeValue;
    string label;
    bool leaf;
    int depth;
    string attribute;
    //vector<int> children;
    //vector<Node*> children;
    map<string, Node*> children;
    Node()
    {
        leaf=false;
        depth=0;
    }
    ~Node()
    {
        children.clear();
    }
};

class DecisionTree
{
public:
    Node* Treeroot;
    int depth;
    DecisionTree()
    {
        this->Treeroot=NULL;
    }
    ~DecisionTree()
    {
        if (Treeroot==NULL)
        {
            return;
        }

        queue<Node*> queue;
        queue.push(Treeroot);

        Node* front = nullptr;

        while (!queue.empty())
        {
            front = queue.front();
            queue.pop();

            if(Treeroot->children.size()!=0)
            {
                for(auto i:Treeroot->children)
                {
                    queue.push(i.second);
                }
            }

            delete front;
        }
        Treeroot=nullptr;
    }
    double entropy(const vector<string>& labels);
    int getIndex(vector<string> v, string val);
    string findBestAttribute(const vector<vector<string>>& data, const vector<string>& forAttIndex, vector<string> pickedAttr);
    Node* createDecisionTree(const vector<vector<string>>& data, const vector<vector<string>>& parent_data, map<string, vector<string>>& attributes, const vector<string>& forAttIndex, vector<string> pickedAttr);
    string classify(Node* root, const vector<string>& data, const vector<string>& forAttIndex);
    string pluralityValue(const vector<string>& labels);
};

double DecisionTree::entropy(const vector<string>& labels) {
    map<string, int> labelCount;
    for(const string& label : labels)
    {
        labelCount[label]++;
    }

    double entropy=0.0;
    for (const auto& entry : labelCount)
    {
        double q=1.0*(entry.second)/labels.size();
        entropy-=q*log2(q);
    }

    return entropy;
}

int DecisionTree::getIndex(vector<string> v, string val)
{
    auto it = find(v.begin(), v.end(), val);

    if (it != v.end())
    {
        int index = it - v.begin();
        return index;
    }
    else
    {
        return -1;
    }
}

string DecisionTree::findBestAttribute(const vector<vector<string>>& data, const vector<string>& forAttIndex, vector<string> pickedAttr)
{
    vector<string> labels;
    for (const vector<string>& entry : data)
    {
        labels.push_back(entry.back());
    }
    double parentEntropy=entropy(labels);
    double maxGain=0.0;
    string bestAttribute;
    //cout<<"in find best attr"<<endl;
    //cout<<"parent entropy: "<<parentEntropy<<endl;

    for (int i=0; i<forAttIndex.size(); i++) {
        string attribute=forAttIndex[i];
        if(pickedAttr.size()!=0 && count(pickedAttr.begin(), pickedAttr.end(), attribute))//if the attribute is already picked
        {
            //cout<<"attr "<<attribute<<" is already picked"<<endl;
            continue;
        }

        map<string, vector<string>> attributeLabels;//get all labels corresponding to an attribute value
        for (int j=0; j<data.size(); j++)
        {
            attributeLabels[data[j][i]].push_back(data[j][forAttIndex.size()]);
        }

        double attributeEntropy=0.0;
        for(const auto& entry : attributeLabels)
        {
            double q=1.0*(entry.second.size())/data.size();
            attributeEntropy+=q*entropy(entry.second);
        }

        double gain=parentEntropy-attributeEntropy;
        //cout<<"gain of "<<attribute<<": "<<gain<<endl;
        if (gain>=maxGain) {
            maxGain=gain;
            bestAttribute=attribute;
        }
    }
    //cout<<"bestattribute in findBest attr: "<<bestAttribute<<endl;
    return bestAttribute;
}

string DecisionTree::pluralityValue(const vector<string>& labels)
{
    map<string, int> labelCount;
    for (const string& label : labels)
    {
        labelCount[label]++;
    }

    int max=0;
    string label;
    for(auto i:labelCount)
    {
        if(i.second>max)
        {
            max=i.second;
            label=i.first;
        }
    }
    return label;
}

Node* DecisionTree::createDecisionTree(const vector<vector<string>>& data, const vector<vector<string>>& parent_data, map<string, vector<string>>& attributes, const vector<string>& forAttIndex, vector<string> pickedAttr) {
    Node* root=new Node;
    vector<string> labels;
    for (const vector<string>& entry : data)
    {
        labels.push_back(entry.back());
    }

    if(data.size()==0)
    {
        //the label will be the plurality value of parent labels
        vector<string> parent_labels;
        for (const vector<string>& entry : parent_data)
        {
            parent_labels.push_back(entry.back());
        }
        root->label=pluralityValue(parent_labels);
        //cout<<"label when data size is 0: "<<root->label<<endl;
    }

    else if(pickedAttr.size()==attributes.size())//all picked or no data for the attribute value
    {
        //the label will be the plurality value of data labels

        root->label=pluralityValue(labels);
        //cout<<"label at the end: "<<root->label<<endl;
    }



    else if(adjacent_find(labels.begin(), labels.end(), std::not_equal_to<>())==labels.end())//all labels are same
    {
        root->label=labels[0];
        //cout<<"label when all the labels are same: "<<root->label<<endl;
    }

    else
    {
        string bestAttribute=findBestAttribute(data, forAttIndex, pickedAttr);
        pickedAttr.push_back(bestAttribute);

        if(this->Treeroot==NULL)
            this->Treeroot=root;
        root->attribute=bestAttribute;

        map<string, vector<vector<string>>> datasets;//divide the dataset on the basis of attribute values
        for (size_t i = 0; i < data.size(); ++i)
        {
            int index=getIndex(forAttIndex, bestAttribute);
            //cout<<"data set present: "<<data[i][index]<<" for attribute "<<bestAttribute<<endl;
            datasets[data[i][index]].push_back(data[i]);
        }

        vector<string>& entry=attributes[bestAttribute];
        for(int i=0; i<entry.size(); i++)
        {
            string attributeValue=entry[i];
            //cout<<"bestattr: "<<bestAttribute<<" curr value: "<<attributeValue<<endl;
            const vector<vector<string>>& subsetData=datasets[attributeValue];
            root->children[attributeValue]=createDecisionTree(subsetData, data, attributes, forAttIndex, pickedAttr);
        }
    }
    return root;
}

double findMean(vector<double>& accuracy)
{
    double sum=0.0;
    for(int i=0; i<accuracy.size(); i++)
        sum+=accuracy[i];
    return sum*1.0/accuracy.size();
}

double findStandardDeviation(vector<double>& accuracy, double mean)
{
    double sum = 0.0, sDeviation = 0.0;

    for(int i = 0; i<accuracy.size(); i++)
    {
        sDeviation+=pow(accuracy[i]-mean, 2);
    }

    return sqrt(sDeviation*1.0/accuracy.size());
}

string DecisionTree::classify(Node* root, const vector<string>& data, const vector<string>& forAttIndex) {

    if (root->children.empty())//the node is a leaf
    {
        return root->label;
    }

    int index=getIndex(forAttIndex, root->attribute);
    string att=data[index];

    return classify(root->children[att], data, forAttIndex);
}

int main() {
    vector<vector<string>> data;

    string line;
    while(getline(cin,line))
    {
        if (line.empty())
           break;
        vector<string> v;

        stringstream ss(line);

        while (ss.good()) {
            string substr;
            getline(ss, substr, ',');
            v.push_back(substr);
        }
        data.push_back(v);
    }

    map<string, vector<string>> attributes;
    vector<string> forAttIndex;
//    buying:   vhigh, high, med, low.
//    maint:    vhigh, high, med, low.
//    doors:    2, 3, 4, 5more.
//    persons:  2, 4, more.
//    lug_boot: small, med, big.
//    safety:   low, med, high.
    forAttIndex={"buying", "maint", "doors", "persons", "lug_boot", "safety"};
    attributes["buying"]={"vhigh", "high", "med", "low"};
    attributes["maint"]={"vhigh", "high", "med", "low"};
    attributes["doors"]={"2", "3", "4", "5more"};
    attributes["persons"]={"2", "4", "more"};
    attributes["lug_boot"]={"small", "med", "big"};
    attributes["safety"]={"low", "med", "high"};
    vector<double> accuracy;

    for(int i=1; i<=20; i++)
    {

        int seed=time(0);
        shuffle(data.begin(), data.end(), default_random_engine(seed));

        vector<vector<string>> training, test;
        int trainSize=data.size()*0.8;
        int testSize=data.size()-trainSize;

        for(int i=0; i<trainSize; i++)
        {
            training.push_back(data[i]);
        }
        for(int i=0; i<testSize; i++)
        {
            test.push_back(data[trainSize+i]);
        }
        vector<string> pickedAttr;
        DecisionTree dt;

        Node* decisionTree = dt.createDecisionTree(training, training, attributes, forAttIndex, pickedAttr);

        int correct=0;
        for(int i=0; i<test.size(); i++)
        {
            string result=dt.classify(decisionTree, test[i], forAttIndex);
            //cout<<"result: "<<result<<endl;
            //cout<<test[i].back()<<endl;
            if(result==test[i].back())
            {
                correct++;
            }
        }
        cout<<"For i="<<i<<endl;
        cout<<"correct: "<<correct<<endl;
        cout<<"total: "<<test.size()<<endl;
        cout<<"test accuracy="<<correct*100.0/test.size()<<"%"<<endl;
        accuracy.push_back(correct*100.0/test.size());
    }
    double mean=findMean(accuracy);
    cout<<"Mean accuracy= "<<mean<<"%"<<endl;
    double sDeviation=findStandardDeviation(accuracy, mean);
    cout<<"Standard Deviation= "<<sDeviation<<endl;

    return 0;
}
