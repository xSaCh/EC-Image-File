#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#define MAX_SIZE 256

using namespace std;

map<string, int> codeT;
int codeC = 0;

string sm = "aababcabcdabcdeabcdefabcdefgabcdefgh";
// vector<int> o;
string o = "";
string s = "";

void printTable()
{
    int c = 0;
    for (auto i = codeT.cbegin(); i != codeT.cend(); i++)
    {

        cout << '[' << (*i).first << ']' << (*i).second << " | ";

        if (++c % 10 == 0)
            cout << '\n';
    }
}

int main()
{
    ifstream file("F:\\a.txt", ios::binary);
    cout << "Hello\n";
    for (int i = 0; i < 256; i++)
    {
        s = i;
        codeT[s] = i;
    }
    codeC = 256;

    string encodeStr = "";
    //step 2
    for (int i = 0; i < sm.size(); i++)
    {
        s = encodeStr + sm[i];

        if (codeT.count(s) > 0)
            encodeStr = s;
        else
        {
            char q = (char)codeT[encodeStr];
            o += q;

            if (codeC < MAX_SIZE)
            {
                cout << codeC << '\n';
                codeT[s] = codeC++;
            }

            encodeStr = sm[i];
        }
    }

    if (codeT.count(encodeStr) > 0)
        o += codeT[encodeStr];
    else
        o += codeC++;

    cout << sm.size() << " " << o.size() << " " << codeT.size() << '\n';

    cout << "=== Original ====\n";
    for (int i = 0; i < sm.size(); i++)
        cout << (int)sm[i] << " ";

    cout << "\n=== Compressed ====\n";
    for (int i = 0; i < o.size(); i++)
        cout << (int)o[i] << " ";

    return 0;
}