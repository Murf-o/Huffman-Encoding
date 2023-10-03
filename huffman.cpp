// Author - Sebastian Barroso

#include <map>
#include <unordered_map>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstring>
#include "mypq.cpp"
#include <bitset>
#include <algorithm>
using namespace std;

unordered_map<int, string> hE;
unordered_map<string, string> hiFileMap;

const int NOT_A_CHAR = 257;

// returns true if char 'c' is already in the vector
bool vectorContains(vector<int> &v, char c)
{
  for (int i : v)
  {
    if (i == c)
    {
      return true;
    }
  }
  return false;
}

// build huffman tree, using my priority queue
HuffmanNode *buildEncodingTree(map<int, int> &map, vector<int> &insertedOrder)
{

  mypq p; // my priorityQueue
  HuffmanNode *huff;
  vector<int> keys = insertedOrder;
  for (auto c : keys)
  {
    huff = new HuffmanNode;
    huff->character = c;
    huff->count = map[c];
    huff->zero = nullptr;
    huff->one = nullptr;
    p.push(huff);
  }
  while (p.size() > 1)
  {
    huff = new HuffmanNode;
    huff->character = NOT_A_CHAR;
    huff->zero = p.top();
    huff->count = huff->zero->count;
    p.pop();
    huff->one = p.top();
    huff->count += huff->one->count;
    p.pop();
    p.push(huff);
  }
  return p.top();
}

// find helper to building the encoding map
void buildEMapHelper(HuffmanNode *cur, unordered_map<int, string> &mapE, string str,
                     HuffmanNode *prev)
{
  if (cur == nullptr)
  {
    return;
  };
  if (cur->character != NOT_A_CHAR)
  {
    mapE[cur->character] = str;
    return;
  }
  str += '0';
  buildEMapHelper(cur->zero, mapE, str, cur);
  str.pop_back();
  str += '1';
  buildEMapHelper(cur->one, mapE, str, cur);
}

// build encoding map
unordered_map<int, string> buildEncodingMap(HuffmanNode *tree)
{
  unordered_map<int, string> encodingMap; // int, string
  if (tree == nullptr)
  {
    return encodingMap;
  }
  string str = "";
  buildEMapHelper(tree, encodingMap, str, nullptr);
  return encodingMap;
}

// builds hiMap
// allows us to look for which code corresponds to which letter in O(1) time
void buildHiMap(string hiFile)
{
  hiFileMap.clear();
  ifstream file(hiFile);
  stringstream ss;
  string line; // line read
  string c;    // letter the code represents
  string code; // the code for the letter itself

  while (getline(file, line))
  {
    ss << line;
    ss >> c;
    ss >> code;
    // map code to letter
    hiFileMap[code] = c;
    ss.clear();
  }
  return;
}

// build a .hi file using filename
void buildHIFile(string filename)
{

  unordered_map<int, int> um;
  vector<int> insertedOrder;
  ifstream file(filename);
  char c;
  map<int, int> freqM;
  vector<int> letters;
  HuffmanNode *eRoot;

  while (file.get(c))
  {
    um[c] += 1;
    freqM[c] = um[c];
    if (!vectorContains(insertedOrder, c))
    {
      insertedOrder.push_back(c);
    }
    letters.push_back(c);
  }
  // add rest of letters with freq of zero
  for (int i = 0; i < 128; ++i)
  {
    if (um.find(i) == um.end())
    {
      freqM[i] = 0;
      insertedOrder.push_back(i);
    }
  }

  // start building .hi file

  filename = filename + ".hi";
  ofstream hiFile(filename);

  eRoot = buildEncodingTree(freqM, insertedOrder);
  hE = buildEncodingMap(eRoot);

  // write to .hi file
  for (int i = 0; i < 128; ++i)
  {
    if (hE.find(i) != hE.end()) // if exists
      hiFile << i << "    " << hE[i] << endl;
    else
    {
      hiFile << i << "    " << endl;
    }
  }
  hiFile.close();
  return;
}

// compress filename file, using .hi file 'hiFile'
void compress(string filename, string hiFile)
{
  // curFile = files[hiFile];
  string f = filename + ".hc";
  ifstream input(filename);
  ofstream outHc(f, ios::binary);
  double numBits = 0;
  char c;
  int origSize = 0;
  string fullCode = "";
  // put all codes into one string
  while (input.get(c))
  {
    fullCode.append(hE[c]);
    ++origSize;
  }
  unsigned char garbage = 0;
  for (int i = 0; i < fullCode.size(); i += 8)
  {
    unsigned char byte = 0;
    string str8 = "";
    if (i + 8 < fullCode.length())
      str8 = fullCode.substr(i, i + 8);
    else
      str8 = fullCode.substr(i, fullCode.length());
    for (unsigned b = 0; b < 8; ++b)
    {
      if (b < str8.length())
        byte |= (str8[b] & 1) << b; // add bit
      else
      {
        byte |= 1 << b; // pad with garbage
        ++garbage;
      }
    }
    outHc.put(byte);
  }
  outHc.put(garbage);

  double numBytes = outHc.tellp();
  outHc.close();

  cout << endl;
  cout << fixed;
  cout << setprecision(0);
  cout << "Number of characters in original file: " << origSize << endl;
  cout << "Number of bytes in compressed file: " << numBytes << endl;
  cout << fixed;
  cout << setprecision(5);
  cout << "Compression Ratio: " << origSize / numBytes << endl;
  cout << "Space Saving (%): " << (1.0 - (numBytes / origSize)) * 100.00000 << endl;
  cout << endl;
}

// IMPORTANT: ONLY NEEDED WHEN YOU DON'T WANT TO USE A hiFileMap
// Scans entire .hi File -- current implementation uses map instead
// find character needed using .hi file
void findCharHI(string hiFile, string findStr, string &charFound)
{
  ifstream file(hiFile);
  string line;
  stringstream ss;
  string tmp;
  string c;
  string code;

  while (getline(file, line))
  {
    ss << line;
    ss >> c;
    ss >> code;
    if (code == findStr)
    {
      charFound = c;
      return;
    }
    ss.clear();
  }
  return;
}

// decompress .hc file, 'fileName', using .hi file 'hiFile'
// decompresses file with same fileName as "'fileName' - .hc" (last three letters)
void decompress(string fileName, string hiFile)
{
  ifstream input(fileName); //.hc file
  string newFile = fileName.substr(0, fileName.size() - 3);
  ofstream out(newFile);

  // decode
  char c;
  bitset<8> setByte;
  string charFound = "";
  string bitCode = "";

  string code = "";
  unsigned char garbageCount = 0;
  // get all bits
  while (input.get(c))
  {
    setByte = c;
    string bitStr = setByte.to_string();
    reverse(bitStr.begin(), bitStr.end());
    code.append(bitStr);
    garbageCount = c;
  }

  // remove last x garbage bits!
  int count = (int)garbageCount;

  // don't count garbage, plus extra byte which tells us how much garbage there is
  for (int i = 0; i < code.size() - count - 8; ++i)
  {
    // go through each 'bit' and check if it matches a character in the .hi file!
    bitCode.push_back(code[i]);

    // findCharHI(hiFile, bitCode, charFound);
    //  if bitcode matches a character in the .hi file, write that character to the file
    if (hiFileMap.find(bitCode) != hiFileMap.end())
    {
      string charFound = hiFileMap[bitCode];
      int num = stoi(charFound);
      char letter = (char)num;
      out << letter;
      bitCode = "";
    }
  }
}

// build tree from .hi file
void buildTree(string filename)
{
  ifstream file(filename);
  string f = filename.substr(0, filename.size() - 3);

  // parse .hi file
  string line;
  stringstream ss;
  string ch;
  string code;
  while (getline(file, line))
  {
    ss << line;
    ss >> ch;
    ss >> code;
    hE[stoi(ch)] = code;
    ss.clear();
  }
}