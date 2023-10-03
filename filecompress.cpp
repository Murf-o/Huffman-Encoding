// Author - Sebastian Barroso
// NetIDs - sbarr9

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include "huffman.cpp"

using std::cin;
using std::cout;
using std::string;

void displayCommands()
{
  cout << "\nOperation are given by digits 1 through 5\n\n";
  cout << "  1 <filename> - create a new Huffman Information file from an original file\n";
  cout << "  2 <filename> - load a Huffman Information file \n";
  cout << "  3 <filename> - compress a file using the current Huffman Information file\n";
  cout << "  4 <filename> - decompress a file using the current Huffman Information file\n";
  cout << "  5            - quit the program\n\n";
}

int main(int argc, char **argv)
{

  cout << "Welcome to File Compression program\n";
  displayCommands();
  bool done = false;
  string line;
  string input;
  string fileSelected;
  map<string, string> hiFiles;
  bool commandTwo = false;

  do
  {
    cout << "cmd> ";

    // read a complete line
    std::getline(std::cin, line);

    // now create a "stringstream" on the line just read
    std::stringstream ss(line);

    // clear input from any previous value
    input = "";

    // get command character
    char command;
    ss >> command;
    // cout << "Debug command:" << command << "***\n";

    if (command == '1')
    {
      ss >> input;
      // build a new .hi file using the information in the file: input
      // check if file exists
      ifstream f(input);
      if (!f.good())
      {
        cout << "File doesn't exist: '" << input << "'" << endl;
        continue;
      } // check if file doesn't exists
      f.close();
      buildHIFile(input);
      cout << ".hi file built" << endl;
    }

    if (command == '2')
    {
      ss >> input;
      // load a .hi file to later perform compression and decompression
      // check if file in correct format
      if (input.substr(input.size() - 3, input.size()) != ".hi")
      {
        cout << "File is not .hi: '" << input << "'" << endl;
        continue;
      }
      // check if file exists
      ifstream f(input);
      if (!f.good())
      {
        cout << "File doesn't exist: '" << input << "'" << endl;
        continue;
      } // check if file doesn't exists
      f.close();
      fileSelected = input;
      commandTwo = true;
      buildTree(input);
      cout << "loaded .hi file" << endl;
    }

    if (command == '3')
    {
      ss >> input;
      // use the current .hi file to compress the file in input
      if (!commandTwo)
      {
        cout << "Select a file first (command 2)" << endl;
        continue;
      }
      // check if file exists
      ifstream f(input);
      if (!f.good())
      {
        cout << "File doesn't exist: '" << input << "'" << endl;
        continue;
      }
      f.close();
      compress(input, fileSelected);
    }

    if (command == '4')
    {
      ss >> input;
      if (!commandTwo)
      {
        cout << "Select a file first (command 2)" << endl;
        continue;
      }
      // check if file in correct format
      if (input.substr(input.size() - 3, input.size()) != ".hc")
      {
        cout << "File is not .hc: '" << input << "'" << endl;
        continue;
      }
      // check if file exists
      ifstream f(input);
      if (!f.good())
      {
        cout << "File doesn't exist: '" << input << "'" << endl;
        continue;
      } // check if file doesn't exists
      f.close();
      decompress(input, fileSelected);
      cout << "decompressed" << endl;
    }

    if (command == '5' || command == 'q')
    {
      done = true;
    }

  } while (!done && !cin.eof());

  return 0;
}
