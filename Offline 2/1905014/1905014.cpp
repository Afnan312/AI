#include<bits/stdc++.h>

#define w1 1
#define w2 1
#define w3 1
#define w4 1
#define winA 100000//player a wins
#define loseA -100000//player b wins
#define draw 0//neutral
#define MAX_DEPTH 9

using namespace std;

const int PITS=6;
const int MARBLES=4;


class Player
{
public:
    bool max;
    int hrst;
    int additional;
    int captured;
    int depth;

    Player(){}

    Player(bool max, int hrst)
    {
        this->max=max;
        this->hrst=hrst;
        this->additional=0;
        this->captured=0;
        this->depth=0;
    }

};

// Game node
class GameNode {
public:
    vector<int> marbles;
    int depth;
    Player player;//current player

    GameNode()//initialization of the game node
    {
        marbles.resize(14);
        for(int i=0; i<2*PITS+2; i++)//6 pits and 2 storages
        {
            if(i==PITS || i==2*PITS+1)//initialize storage to 0 marbles
                marbles[i]=0;
            else
                marbles[i]=4;//initialize pits to 4 marbles
        }
        depth = 0;
    }

    int gameOver() const {//storage more than half......or if one has more in storage than the other in the final pits
        if(marbles[PITS]>PITS*MARBLES)//player A wins
            return 1;

        if(marbles[2*PITS+1]>PITS*MARBLES)//player B wins
            return 2;

        int p1=0, p2=0;
        int p1all=0, p2all=0;

        for(int i=0; i<6; i++)//count marbles in pits only
        {
            p1+=marbles[i];
        }
        if(p1==0)//no more moves for player a
        {
            for(int i=7; i<=13; i++)//count pits+storage
            {
                p2all+=marbles[i];
            }
            p1all=marbles[6];//only the storage
            if(p1all>p2all)
                return 1;//a wins
            else if(p1all<p2all)
                return 2;//b wins
            else
                return 3;//tie
        }

        for(int i=7; i<13 ; i++)//count marbles in pits only
        {
            p2+=marbles[i];
        }

        if(p2==0)//no more moves for player b
        {
            for(int i=0; i<=6; i++)//count pits+storage
            {
                p1all+=marbles[i];
            }
            p2all=marbles[13];//only the storage
            if(p1all>p2all)
                return 1;//a wins
            else if(p1all<p2all)
                return 2;//b wins
            else
                return 3;//tie
        }
        return 0; //game is not over
    }

    void printBoard()
    {
        cout<<"Player A: \t";
        cout<<"Storage: "<<marbles[6]<<" |\t";
        for(int i=PITS-1; i>=0; i--){
            cout<<i<<": "<<marbles[i]<<"\t";
        }
        cout<<endl;

        cout << "Player B: \t\t\t";
        for(int i=PITS+1; i<2*PITS+1; i++){
            cout<<i<<": "<<marbles[i]<<"\t";
        }
        cout<<"| Storage: "<<marbles[13];
        cout<<endl;
        cout<<endl;
    }

    int hrst1() const
    {
        return marbles[6]-marbles[13];//playerA-playerB always to calculate max for A and min for B
    }

    int hrst2() const
    {
        int hrs1=marbles[6]-marbles[13];
        int stonesA=0, stonesB=0;
        for (int i=0; i<6; i++) {
            stonesA+=marbles[i];
        }
        for(int i=7; i<13; i++)
        {
            stonesB+=marbles[i];
        }
        return w1*hrs1+w2*(stonesA-stonesB);//calculate max for A and min for B
    }

    int hrst3()const
    {
        int hrs1=marbles[6]-marbles[13];
        int stonesA=0, stonesB=0;
        for (int i=0; i<6; i++) {
            stonesA+=marbles[i];
        }
        for(int i=7; i<13; i++)
        {
            stonesB+=marbles[i];
        }
        if(!player.max)
            return w1*hrs1+w2*(stonesA-stonesB)-w3*player.additional;//min value for min player
        return w1*hrs1+w2*(stonesA-stonesB)+w3*player.additional;//max value for max player
    }

    int hrst4() const
    {
        int hrs1=marbles[6]-marbles[13];
        int stonesA=0, stonesB=0;
        for (int i=0; i<6; i++) {
            stonesA+=marbles[i];
        }
        for(int i=7; i<13; i++)
        {
            stonesB+=marbles[i];
        }
        if(!player.max)
            return w1*hrs1+w2*(stonesA-stonesB)-w3*player.additional-w4*player.captured;//min value for min player
        return w1*hrs1+w2*(stonesA-stonesB)+w3*player.additional+w4*player.captured;//max value for max player
    }
};

int minmaxAlgo(GameNode& node, Player& plA, Player& plB, int depth, int alpha, int beta, int maxDepth)
{
    int done=node.gameOver();
    if(done!=0)//out of moves
    {
        if(done==1)//player a won
            return winA;
        else if(done==2)//player a lost
            return loseA;
        else if(done==3)
            return draw;
    }

    if (node.depth==maxDepth)//stop at depth
    {
        ////evaluate according to the player and heuristics
        //cout<<"At depth "<<node.depth;
        int value;
        switch(node.player.hrst)
        {
        case 1:
            //cout<<" hrst 1"<<endl;
            value=node.hrst1();
            //cout<<value<<endl;
            return value;
        case 2:
            //cout<<"hrst 2"<<endl;
            value=node.hrst2();
            return value;
        case 3:
            //cout<<"hrst 3"<<endl;
            value=node.hrst3();
            return value;
        case 4:
            //cout<<"hrst 4"<<endl;
            value=node.hrst4();
            return value;
        }
    }

    if (node.player.max)//for player a
    {
        int maxEval=INT_MIN;
        for (int i=0; i<PITS; i++)
        {
            //cout<<"Player A At depth :"<<node.depth<<", i= ";
            //cout<<i<<endl;
            if (node.marbles[i]==0){//there are no marbles in current pit
                //cout<<"skipped"<<endl;
                continue;
            }

            GameNode newNode=node;
            Player pl1=plA;
            int marbles=newNode.marbles[i];
            newNode.marbles[i]=0;//remove all from current pit
            int k=i;

            while (marbles>0)
            {
                k=(k+1)%(2*PITS+2);
                if (k==2*PITS+1)
                    continue;  //Skip opponent storage
                newNode.marbles[k]++;//sow the marbles in pits
                marbles--;
            }

            if (k==PITS)//last marble in storage, additional move
            {
                pl1.additional++;
                newNode.depth=depth;//no change in depth as it is in same move
                maxEval=max(maxEval, minmaxAlgo(newNode, pl1, plB, depth, alpha, beta, maxDepth));
            }
            else
            {
                if (k>=0 && k<PITS && newNode.marbles[k]==1)//capture opponent marbles
                {
                    newNode.marbles[PITS]+=newNode.marbles[k]+newNode.marbles[2*PITS-k];//move to storage
                    pl1.captured+=newNode.marbles[k]+newNode.marbles[2*PITS-k];
                    //cout<<"captured for A: "<<pl1.captured<<endl;
                    newNode.marbles[k]=newNode.marbles[2*PITS-k]=0;//set value to 0
                }
                //cout<<"player A switching at i="<<i<<endl;
                newNode.player=plB;//change current player of game
                newNode.depth=depth+1;
                maxEval=max(maxEval, minmaxAlgo(newNode, pl1, plB, depth+1, alpha, beta, maxDepth));
            }

            alpha = max(alpha, maxEval);
            //cout<<"At depth "<<node.depth;
            //cout<<", The value of alpha for A: "<<alpha<<endl;

            if (beta<=alpha){
                //cout<<"In player A, alpha: "<<alpha<<" beta: "<<beta<<" so cutoff"<<endl;
                break;
            }
        }
        return maxEval;
    }

    else//player b
    {
        int minEval=INT_MAX;
        for (int i=PITS+1; i<2*PITS+1; i++)
        {
            //cout<<"player B at depth: "<<node.depth<<" , i= ";
            //cout<<i<<endl;
            if (node.marbles[i]==0){
                //cout<<"skipped"<<endl;
                continue;
            }

            GameNode newNode=node;
            Player pl2=plB;
            int marbles = newNode.marbles[i];
            newNode.marbles[i]=0;
            int k=i;

            while (marbles>0)
            {
                k=(k+1)%(2*PITS+2);
                if (k==PITS)
                    continue;  // Skip opponent storage
                newNode.marbles[k]++;
                marbles--;
            }

            if (k==2*PITS+1)//last one in storage, additional
            {
                pl2.additional++;
                newNode.depth=depth;
                //cout<<"Additional move for B: "<<pl2.additional<<endl;
                minEval = min(minEval, minmaxAlgo(newNode, plA, pl2, depth, alpha, beta, maxDepth));
            }

            else
            {
                if (k>=PITS+1 && k<2*PITS+2 && newNode.marbles[k]==1)//capture the marbles into storage
                {
                    newNode.marbles[2*PITS+1]+=newNode.marbles[k]+newNode.marbles[2*PITS-k];
                    pl2.captured+=newNode.marbles[k]+newNode.marbles[2*PITS-k];
                    //cout<<"captured for B: "<<pl2.captured<<endl;
                    newNode.marbles[k]=newNode.marbles[2*PITS-k]=0;
                }
                //cout<<"player b switching at i="<<i<<endl;
                newNode.player=plA;//change current player
                newNode.depth=depth+1;
                minEval=min(minEval, minmaxAlgo(newNode, plA, pl2, depth+1, alpha, beta, maxDepth));
            }

            beta=min(beta, minEval);
            //cout<<"At depth "<<node.depth;
            //cout<<", The value of beta for B: "<<beta<<endl;
            if(beta<=alpha){
                //cout<<"In player B, alpha: "<<alpha<<" beta: "<<beta<<" so cutoff"<<endl;
                break;
            }
        }
        return minEval;
    }
}

// Get the best move using the minimax algorithm
int getBestMove(GameNode& node, Player& plA, Player& plB)
{
    int bestMaxEval=INT_MIN;
    int bestMinEval=INT_MAX;
    int bestMove=-1;//the pit to move the marbles

    int alpha=INT_MIN;
    int beta=INT_MAX;

    //cout<<"in getbest move"<<endl;
    if(node.player.max)
    {
        for (int i=0; i<PITS; i++)//start branching from here
        {
            if (node.marbles[i]==0)
                continue;

            GameNode newNode=node;
            Player pl1=plA;
            int marbles=newNode.marbles[i];
            newNode.marbles[i]=0;
            int k=i;

            while(marbles>0)
            {
                k=(k+1)%(2*PITS+2);
                if (i==2*PITS+1)
                    continue;  // Skip opponent storage
                newNode.marbles[k]++;
                marbles--;
            }

            if (k==PITS)
            {//additional move
                newNode.depth=0;//do not change current player or depth
                pl1.additional++;
                bestMaxEval=max(bestMaxEval, minmaxAlgo(newNode, pl1, plB, 0, alpha, beta, pl1.depth));//for further branching
            }

            else
            {
                if(k>=0 && k<PITS && newNode.marbles[k]==1)//capture opponent marbles
                {
                    newNode.marbles[PITS]+=newNode.marbles[k]+newNode.marbles[2*PITS-k];//move to storage
                    pl1.captured+=newNode.marbles[k]+newNode.marbles[2*PITS-k];
                    newNode.marbles[k]=newNode.marbles[2*PITS-k]=0;//set value to 0
                }

                newNode.player=plB;//change current player of game
                newNode.depth=1;
                bestMaxEval=max(bestMaxEval, minmaxAlgo(newNode, pl1, plB, 1, alpha, beta, pl1.depth));
            }
            if(bestMaxEval>alpha)
            {
                alpha=bestMaxEval;
                bestMove=i;
            }
        }
    }

    else
    {
        for (int i=PITS+1; i<2*PITS+1; i++)
        {
            if (node.marbles[i]==0)
                continue;

            GameNode newNode=node;
            Player pl2=plB;
            int marbles = newNode.marbles[i];
            newNode.marbles[i]=0;
            int k=i;

            while (marbles>0)
            {
                k=(k+1)%(2*PITS+2);
                if (k==PITS)
                    continue;  // Skip opponent storage
                newNode.marbles[k]++;
                marbles--;
            }

            if (k==2*PITS+1)//last one in storage, additional
            {
                newNode.depth=0;//do not change current player or depth
                pl2.additional++;
                bestMinEval=min(bestMinEval, minmaxAlgo(newNode, plA, pl2, 0, alpha, beta, pl2.depth));//for further branching
            }

            else
            {
                if (k>=PITS+1 && k<2*PITS+2 && newNode.marbles[k]==1)//capture the marbles into storage
                {
                    newNode.marbles[2*PITS+1]+=newNode.marbles[k]+newNode.marbles[2*PITS-k];
                    pl2.captured+=newNode.marbles[k]+newNode.marbles[2*PITS-k];
                    newNode.marbles[k]=newNode.marbles[2*PITS-k]=0;
                }

                newNode.player=plA;//change current player
                newNode.depth=1;
                bestMinEval=min(bestMinEval, minmaxAlgo(newNode, plA, pl2, 1, alpha, beta, pl2.depth));
            }
            if(bestMinEval<beta)
            {
                beta=bestMinEval;
                bestMove=i;
            }
        }
    }
    //cout<<"bestval: "<<bestMaxEval<<endl;
    return bestMove;
}

// Main function
int main()
{
    /*Player playerA(true, 2);
    Player playerB(false, 2);
    GameNode node;
    playerA.depth=6;
    playerB.depth=6;
    node.player=playerA;*/
    int done;
    int winForA=0;
    int winForB=0;
    int tie=0;
    int h1;
    int h2;
    cout<<"Select heuristic for Player A:(1/2/3/4)"<<endl;
    cin>>h1;
    cout<<"Select heuristic for Player B:(1/2/3/4)"<<endl;
    cin>>h2;
    cout<<"Start"<<endl;

    for(int i=2; i<=10; i++){
        for(int j=2; j<=10; j++){
            Player playerA(true, h1);
            Player playerB(false, h2);
            GameNode node;
            playerA.depth=i;
            playerB.depth=j;
            node.player=playerA;
            while(true)
            {
                done=node.gameOver();
                if(done!=0)
                    break;

                int bestMove=getBestMove(node, playerA, playerB);

                int marbles=node.marbles[bestMove];
                node.marbles[bestMove]=0;

                while (marbles>0)
                {
                    bestMove=(bestMove+1)%(2*PITS+2);
                    if(node.player.max && bestMove==2*PITS+1)
                        continue;    //Player a skips player b's storage
                    if(!node.player.max && bestMove==PITS)
                        continue;  //Player b skips player a's storage
                    node.marbles[bestMove]++;
                    marbles--;
                }

                if(!((node.player.max && bestMove==PITS) || (!node.player.max && bestMove==2*PITS+1)))
                {
                    if(node.player.max)
                    {
                        if(bestMove>=0 && bestMove<PITS && node.marbles[bestMove]==1)
                        {
                            node.marbles[PITS]+=node.marbles[bestMove]+node.marbles[2*PITS-bestMove];
                            node.marbles[bestMove]=node.marbles[2*PITS-bestMove]=0;

                        }
                        node.player=playerB;
                    }
                    else
                    {
                        if(bestMove>PITS && bestMove<2*PITS+1 && node.marbles[bestMove]==1)
                        {
                            node.marbles[2*PITS+1]+=node.marbles[bestMove]+node.marbles[2*PITS-bestMove];
                            node.marbles[bestMove]=node.marbles[2*PITS-bestMove]=0;

                        }
                        node.player=playerA;
                    }
                }
                playerA.additional=0;
                playerA.captured=0;
                playerB.additional=0;
                playerB.captured=0;
            }

            cout << "Final:" << endl;
            node.printBoard();
            if(done==1){
                cout<<"\t\tPlayer A wins for depth "<<playerA.depth<<" for A and depth "<<playerB.depth<<" for B."<<endl;
                winForA++;
            }
            else if(done==2){
                cout<<"\t\tPlayer B wins for depth "<<playerA.depth<<" for A and depth "<<playerB.depth<<" for B."<<endl;
                winForB++;
            }
            else{
                cout<<"\t\tIt's a tie for depth "<<playerA.depth<<" for A and depth "<<playerB.depth<<" for B."<<endl;
                tie++;
            }
        }
    }
    /*while(true)
    {
        done=node.gameOver();
        if(done!=0)
            break;
        cout<<"Current Board:"<<endl;
        node.printBoard();

        if (node.player.max)
            cout<<"Player A's Turn"<<endl;
        else
            cout<<"Player B's Turn"<<endl;

        int bestMove=getBestMove(node, playerA, playerB);
        cout<<endl;
        cout<<endl;
        cout<<"The move is from pit no. : " <<bestMove<< endl;

        int marbles=node.marbles[bestMove];
        node.marbles[bestMove]=0;

        while (marbles>0)
        {
            bestMove=(bestMove+1)%(2*PITS+2);
            if(node.player.max && bestMove==2*PITS+1)
                continue;    //Player a skips player b's storage
            if(!node.player.max && bestMove==PITS)
                continue;  //Player b skips player a's storage
            node.marbles[bestMove]++;
            marbles--;
        }

        if(node.player.max && bestMove==PITS)
            cout<<"Player A gets an additional move"<<endl;

        else if(!node.player.max && bestMove==2*PITS+1)
            cout<<"Player B gets an additional move"<<endl;

        else
        {
            if(node.player.max)
            {
                if(bestMove>=0 && bestMove<PITS && node.marbles[bestMove]==1)
                {
                    node.marbles[PITS]+=node.marbles[bestMove]+node.marbles[2*PITS-bestMove];
                    cout<<"Player A captured "<<node.marbles[2*PITS-bestMove]<<" stones from player B's "<<2*PITS-bestMove<<" pit"<<endl;
                    node.marbles[bestMove]=node.marbles[2*PITS-bestMove]=0;

                }
                node.player=playerB;
            }
            else
            {
                if(bestMove>PITS && bestMove<2*PITS+1 && node.marbles[bestMove]==1)
                {
                    node.marbles[2*PITS+1]+=node.marbles[bestMove]+node.marbles[2*PITS-bestMove];
                    cout<<"Player B captured "<<node.marbles[2*PITS-bestMove]<<" stones from player A's "<<2*PITS-bestMove<<" pit"<<endl;
                    node.marbles[bestMove]=node.marbles[2*PITS-bestMove]=0;

                }
                node.player=playerA;
            }
        }
        playerA.additional=0;
        playerA.captured=0;
        playerB.additional=0;
        playerB.captured=0;
        cout<<endl;
        cout<<endl;
    }

    cout << "Final:" << endl;
    node.printBoard();
    if(done==1){
        cout<<"\t\tPlayer A wins for depth "<<playerA.depth<<" for A and depth "<<playerB.depth<<" for B."<<endl;
    }
    else if(done==2)
        cout<<"\t\tPlayer B wins for depth "<<playerA.depth<<" for A and depth "<<playerB.depth<<" for B."<<endl;
    else
        cout<<"\t\tIt's a tie for depth "<<playerA.depth<<" for A and depth "<<playerB.depth<<" for B."<<endl;*/

    cout<<"A wins: "<<winForA<<", B wins: "<<winForB<<", TIE: "<<tie<<endl;
    if(winForA>winForB)
        cout<<"Heuristic "<<h1<<" is better."<<endl;
    else if(winForA<winForB)
        cout<<"Heuristic "<<h2<<" is better."<<endl;
    else
        cout<<"Inconclusive"<<endl;


    return 0;
}
