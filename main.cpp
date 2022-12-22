#include <iostream>
#include <chrono>
#include <fstream>
#include "Parser.h"
#include "Interpreter.h"
#include "Lexical_Analyzer.h"
using namespace std;

string open_file(string fileName)
{
  ifstream in(fileName.data(), ios::in | ios::binary);

  if (in)
  {
    return string((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
  }

  throw runtime_error("Can't open file!");
}

int main() 
{
    cout << "Enter test number: ";
    string aa;
    string fileName = "test";
    cin >> aa;
    fileName = fileName + aa + ".txt";
    cout << endl;

    try 
    {
        //забираем все символы из файла
        string text = open_file(fileName);

        //создаем лексический анализатор
        Lexical_Analyzer LexicalA(text);

        //создаем синтаксический анализатор
        Parser parser(LexicalA);

        //генерируем ОПС
        auto RPN = parser.make_RPN();

        //создаем интерпретатор
        Interpreter interpreter(RPN);

        //выводим ОПС на экран
        cout << "Reverse Polish Notation:" << endl;
        for (const auto &item : RPN.items)
        {
            if (item.type == ITEM_TYPE::CONST)
            {
                cout << item.value << " ";
            }
            else
            {
                cout << item.word << " ";
            }
        }

        cout << endl << endl;

        //интерпретируем ОПС, выводим результат на экран
        cout << "Interpretation result:";
        interpreter.generate_commands();

        cout << endl;
    }
    catch(exception& e) 
    {
        cout << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
