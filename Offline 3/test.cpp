#include<bits/stdc++.h>

using namespace std;

int main()
{
    ofstream myFile;
    myFile.open("report.csv", ios::app);
    myFile<<"1st,2nd,3rd\n";
    myFile<<"4th,5th,6th\n";
    myFile.close();
    return 0;
}
