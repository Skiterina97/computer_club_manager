#ifndef LOGIC_H_INCLUDED
#define LOGIC_H_INCLUDED

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <fstream>
#include <unordered_set>
#include <queue>
#include <set>

using namespace std;

//струкура события
struct Event
{
  string time;           //времяв формате xx:xx
  int t;                 //время в минутах, отсчитано от 00:00
  string idCl;           //ник клиента
  int idOp;              //вид события (1, 2, 3, 4)
  int numTable = 0;      //номер стола для события типа 2
  bool valid = true;     //проверка на корректность записи
};

//проверка на то, является ли число целым и положительным
bool IsPositiveInteger(const string &str);

//проверка корректности второй строки со временем начала и конца работы клубы (хх:хх хх:хх)
bool IsCorrectTimeBeginEnd(const string &str);

//функция перевода времени из строкового формата хх:хх в числовой в минутах от 00:00
int ParsTime(const string &str);

//проверка, что символ являктся строчной английской буквой
bool isLower(char c);

//функция перевода строки с событием в структуру и проверки корректности ввода данных
Event ParsLine(const string &str, const int &n);

//функция увеличения времени работы стола и его выручки
void TableProcessing(vector<pair<long long, int>> &tables, int &price, int &nextTime, int &prevTime, int &numTable);

//функция обработки события 1 (клиент пришёл)
void ClientEnter(ofstream& newFile, Event &ev, set<string> &clients, int &timeBegin, int &timeEnd);

//функция обработки события 2 (клиент сел за стол)
void ClientSit(ofstream& newFile, Event &ev, const set<string> &clients, unordered_set<int> &freeTable,
               unordered_map<string, pair<int, int>> &tableClients, vector<pair<long long, int>> &tables, int &price);

//функция обработки события 3 (клиент ожидает)
void ClientWait(ofstream& newFile, Event &ev, set<string> &clients, size_t &k, queue<string> &waiting, int &n);

//функция обработки события 4 (клиент ушёл)
void ClientExit(ofstream& newFile, Event &ev, set<string> &clients, unordered_map<string, pair<int, int>> &tableClients,
                int &price, vector<pair<long long, int>> &tables, queue<string> &waiting, unordered_set<int> &freeTable);

#endif // LOGIC_H_INCLUDED
