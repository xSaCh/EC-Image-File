#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#define MAX_SIZE 4096

using namespace std;

void compress()
{
    // LZW Dictionary
    map<string, int> codeTable;
    int codeC = 0; // Code Count for LZW Dict

    //Sample string
    string sm = "AABAHSJCABABABA";

    //output data
    vector<uint16_t> output;

    string encodeStr = "";

    // Encoded String + next char
    string strPCode = "";

    // Initizalize Dictionary
    for (int i = 0; i < 256; i++)
    {
        codeTable[string(1, (char)i)] = i;
    }
    codeC = 257; // 256 is for EOF marker

    //step 2
    for (int i = 0; i < sm.size(); i++)
    {
        strPCode = encodeStr + sm[i];

        if (codeTable.count(strPCode) > 0)
            encodeStr = strPCode;
        else
        {
            output.push_back(codeTable[encodeStr]);

            if (codeC < MAX_SIZE)
            {
                codeTable[strPCode] = codeC++;
            }

            encodeStr = sm[i];
        }
    }

    if (codeTable.count(encodeStr) > 0)
        output.push_back(codeTable[encodeStr]);
    else
        output.push_back(codeC++);

    cout << "Original Size   : " << sm.size() << "\n";
    cout << "Compressed Size : " << output.size() << "\n";
    cout << "Percentage      : " << 1 - ((double)output.size() / (double)sm.size()) << "%\n";
}

int main()
{

    // ifstream file("F:\\a.txt", ios::binary);
    cout << "Hello😍\n";

    compress();
    return 0;
}