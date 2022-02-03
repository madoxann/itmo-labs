#include <bits/stdc++.h>
using namespace std;
 
/*
void print(int** horn, int disj, int vars)
{
    cout << endl;
    for(int i = 0; i < disj; i++)
    {
       for(int j = 0; j < vars + 1; j++)
       {
           cout << horn[i][j] << " ";
       }
       cout << endl;
    }
}
*/
int main(int argc, char const *argv[])
{
    int vars, disj, currNum;
    cin >> vars >> disj;
    int** horn = new int*[disj];
    for(int i = 0; i < disj; i++)
    {
        int currSize = vars;
        int* disjs = new int[vars + 1];
        for(int j = 0; j < vars; j++)
        {
            cin >> currNum;
            if(currNum == -1)
                currSize--;
            disjs[j] = currNum;
        }
        disjs[vars] = currSize;
        horn[i] = disjs;
    }
    for(int i = 0; i < vars; i++)
    {
        for(int j = 0; j < disj; j++)
        {
            if(horn[j][vars] == 1)
            {
                int position;
                for(int k = 0; k < vars; k++)
                {
                    if(horn[j][k] != -1)
                    {
                        position = k;
                        break;
                    }
                }
                for(int k = 0; k < disj; k++)
                {
                    if((j != k) && (horn[k][position] == horn[j][position]))
                    {
                        for(int c = 0; c < vars; c++)
                        {
                            horn[k][c] = -1;
                        }
                        horn[k][vars] = 0;
                    }
                }
                for(int k = 0; k < disj; k++)
                {
                    if((j != k) && (horn[k][position] == (horn[j][position] + 1) % 2) && (horn[k][vars] == 1))
                    {
                        cout << "YES" << endl;
                        return 0;
                    }
                }
                //print(horn, disj, vars);
                for(int k = 0; k < disj; k++)
                {
                    if(horn[k][position] != -1)
                    {
                        horn[k][position] = -1;
                        horn[k][vars]--;
                    }
                }
            }
        }
    }
    cout << "NO" << endl;
    return 0;
}