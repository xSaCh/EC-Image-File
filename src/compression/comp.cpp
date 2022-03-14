#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#define MAX_SIZE 4096

using namespace std;

vector<uint32_t> compress(const string input)
{
    // LZW Dictionary
    map<string, uint32_t> codeTable;
    int codeC = 0; // Code Count for LZW Dict

    //Sample string
    // string sm = "ABBABBBABBA";

    //output data
    vector<uint32_t> output;

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
    for (int i = 0; i < input.size(); i++)
    {
        strPCode = encodeStr + input[i];

        if (codeTable.count(strPCode) > 0)
            encodeStr = strPCode;
        else
        {
            output.push_back(codeTable[encodeStr]);

            if (codeC < MAX_SIZE)
            {
                codeTable[strPCode] = codeC++;
            }

            encodeStr = input[i];
        }
    }

    if (codeTable.count(encodeStr) > 0)
        output.push_back(codeTable[encodeStr]);
    else
        output.push_back(codeC++);

    cout << "Original Size   : " << input.size() << "\n";
    cout << "Compressed Size : " << output.size() << "\n";
    cout << "Percentage      : " << 1 - ((double)output.size() / (double)input.size()) << "%\n";

    return output;
}

string decompress(const vector<uint32_t> input)
{
    // LZW Dictionary
    map<uint32_t, string> codeTable;
    int codeC = 0; // Code Count for LZW Dict

    //Encoded Data
    // vector<uint32_t> input = {65, 66, 66, 257, 258, 260, 65};

    //output data
    string output = "";

    string prevStr = "";

    // Initizalize Dictionary
    for (int i = 0; i < 256; i++)
        codeTable[i] = string(1, i);

    codeC = 257; // 256 is for EOF marker

    for (int i = 0; i < input.size(); i++)
    {
        output += codeTable[input[i]];

        if (prevStr.size() > 0)
            codeTable[codeC++] = prevStr + codeTable[input[i]][0];

        prevStr = codeTable[input[i]];
    }

    cout << "STR: " << output << '\n';

    return output;
}
int main()
{

    // ifstream file("F:\\a.txt", ios::binary);
    cout << "Hello😍\n";

    decompress(compress(a));
    return 0;
}