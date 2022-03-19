#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>

#define MAX_SIZE 2 << 15
#define EOF_CODE 256

using namespace std;

vector<uint32_t> compress(const string fileName)
{
    // LZW Dictionary
    unordered_map<string, uint32_t> codeTable;
    int codeC = 0; // Code Count for LZW Dict

    //output data
    vector<uint32_t> output;

    string encodeStr = "";

    // Encoded String + next char
    string strPCode = "";

    // Input File Handler
    ifstream inpFile(fileName, ios::binary);

    char c;
    size_t fileSize = 0;

    // Initizalize Dictionary
    for (int i = 0; i < 256; i++)
    {
        codeTable[string(1, (char)i)] = i;
    }
    codeC = 257; // 256 is for EOF marker

    //step 2
    while (inpFile.get(c))
    {
        fileSize++;
        strPCode = encodeStr + c;

        if (codeTable.count(strPCode) > 0)
            encodeStr = strPCode;
        else
        {
            output.push_back(codeTable[encodeStr]);

            if (codeC < MAX_SIZE)
            {
                codeTable[strPCode] = codeC++;
            }

            encodeStr = c;
        }
    }

    inpFile.close();
    if (codeTable.count(encodeStr) > 0)
        output.push_back(codeTable[encodeStr]);
    else
        output.push_back(codeC++);

    cout << "\nOriginal Size   : " << fileSize << "\n";
    cout << "Compressed Size : " << output.size() << "\n";
    cout << "LAST: " << codeC << codeTable[encodeStr.substr(0, encodeStr.size() - 1)] << " " << encodeStr.substr(0, encodeStr.size() - 1) << '\n';
    cout << "Percentage      : " << 1 - ((double)output.size() / (double)fileSize) << "%\n";

    return output;
}

bool getCode(ifstream &input, uint32_t &code)
{
    char c;
    if (!input.get(c))
        return false;
    code = c & 0xff;

    if (!input.get(c))
        return false;

    code |= (c & 0xff) << 8;
    return code != EOF_CODE;
}

string decompress(const string fileName)
{
    // LZW Dictionary
    unordered_map<uint32_t, string> codeTable;
    int codeC = 0; // Code Count for LZW Dict

    //Encoded Data
    // vector<uint32_t> input = {65, 66, 66, 257, 258, 260, 65};

    // Input File Handler
    ifstream inpFile(fileName, ios::binary);
    ofstream outFile("F:\\aa.bin", ios::binary);

    char c;
    size_t fileSize = 0;

    //output data
    string output = "";

    string prevStr = "";

    // Initizalize Dictionary
    for (int i = 0; i < 256; i++)
        codeTable[i] = string(1, i);

    codeC = 257; // 256 is for EOF marker
    uint32_t code = 0;

    while (getCode(inpFile, code))
    {
        if (codeTable.count(code) <= 0)
            codeTable[codeC] = prevStr + prevStr[0];
        output += codeTable[code];
        outFile << codeTable[code];
        if ((prevStr.size() > 0) && codeC < MAX_SIZE)
            codeTable[codeC++] = prevStr + codeTable[code][0];

        prevStr = codeTable[code];
    }

    inpFile.close();
    outFile.close();
    cout << "STR: " << output.size() << '\n';

    return output;
}
int main()
{
    vector<uint32_t> a = compress("F:\\a.bin");

    ofstream ofi("F:\\b.bin", ios::binary);

    for (int i = 0; i < a.size(); i++)
    {
        ofi.put(a[i] & 0xff);
        ofi.put((a[i] >> 8) & 0xff);
    }
    ofi.close();
    // decompress("F:\\b.bin");
    return 0;
}