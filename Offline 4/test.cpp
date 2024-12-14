#include<bits/stdc++.h>
#include<windows.h>
using namespace std;

//vector<int> findXOR(vector<int>& r1, vector<int>& r2)
//{
//    vector<int> rem;
//    bool first=false;
//    for(int i=0; i<r1.size(); i++)
//    {
//        if(r1[i]!=r2[i])
//        {
//            if(!first)
//                first=true;
//            rem.push_back(1);
//        }
//        else{
//            if(first)
//                rem.push_back(0);
//        }
//    }
//    return rem;
//}
//
//vector<int> crc(vector<int>& dividend, vector<int>& generator)
//{
//    int taken=0;
//    int dvdLen=dividend.size();
//    int gnrLen=generator.size();
//    vector<int> result;
//    while(true)
//    {
//        vector<int> sub;
//        sub=result;
//        int total=gnrLen-result.size();
//        for(int i=0; i<total; i++)
//        {
//            sub.push_back(dividend[taken]);
//            taken++;
//        }
//        result=findXOR(sub, generator);
//        if((taken+gnrLen-result.size())>dvdLen)
//            break;
//    }
//    for(int i=taken; i<dvdLen; i++)
//        result.push_back(dividend[taken]);
//    return result;
//}
//
//int main()
//{
//    vector<int> dividend={1,1,0,1,1,1,0,1,0,0,0,1};
//    vector<int> check=dividend;
//
//    vector<int> generator={1,0,1};
//    for(int i=0; i<generator.size()-1; i++)
//        check.push_back(0);
//
//    vector<int> result=crc(check, generator);
//    cout<<result.size()<<endl;
//    for(int i=0; i<result.size(); i++)
//        cout<<result[i];
//    cout<<endl;
//
//    int extra=generator.size()-1-result.size();
//    for(int i=0; i<extra; i++)
//        dividend.push_back(0);
//    for(int i=0; i<result.size(); i++)
//        dividend.push_back(result[i]);
//    cout<<endl;
//    for(int i=0; i<dividend.size(); i++)
//        cout<<dividend[i];
//    cout<<endl;
//
//    cout<<endl;
//    vector<int> result2=crc(dividend, generator);
//    cout<<result2.size()<<endl;
//    for(int i=0; i<result2.size(); i++)
//        cout<<result2[i];
//    return 0;
//}
int main()
{
    string gnr="0100";
    vector<int> gnr2;
    for(int i=0; i<gnr.size(); i++)
    {
        if(gnr[i]=='0')
            gnr2.push_back(0);
        else if(gnr[i]=='1')
            gnr2.push_back(1);
    }
    for(int i=0; i<gnr2.size(); i++)
        cout<<gnr2[i];
    cout<<endl;
    return 0;
}
