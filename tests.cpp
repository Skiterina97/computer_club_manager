#include <gtest/gtest.h>
#include "logic.h"

TEST(ValidationTest, PositiveNumbers)
{
  EXPECT_TRUE(IsPositiveInteger("123"));
  EXPECT_FALSE(IsPositiveInteger("0"));
  EXPECT_FALSE(IsPositiveInteger("-5"));
  EXPECT_FALSE(IsPositiveInteger("abc"));
  EXPECT_FALSE(IsPositiveInteger("7.457"));
}

TEST(ValidationTest, CorrectTimeBeginEnd)
{
  EXPECT_TRUE(IsCorrectTimeBeginEnd("11:59 19:00"));
  EXPECT_FALSE(IsCorrectTimeBeginEnd("0900 19:00"));
  EXPECT_FALSE(IsCorrectTimeBeginEnd("f3:00 16:30"));
  EXPECT_FALSE(IsCorrectTimeBeginEnd("1f:00 16:30"));
  EXPECT_FALSE(IsCorrectTimeBeginEnd("13:f0 16:30"));
  EXPECT_FALSE(IsCorrectTimeBeginEnd("13:0f 16:30"));
  EXPECT_FALSE(IsCorrectTimeBeginEnd("13:00 f6:30"));
  EXPECT_FALSE(IsCorrectTimeBeginEnd("13:00 1f:30"));
  EXPECT_FALSE(IsCorrectTimeBeginEnd("13:00 16:f0"));
  EXPECT_FALSE(IsCorrectTimeBeginEnd("13:00 16:3f"));
  EXPECT_FALSE(IsCorrectTimeBeginEnd("12:0023:57"));
  EXPECT_FALSE(IsCorrectTimeBeginEnd("12:00 23:573"));
  EXPECT_FALSE(IsCorrectTimeBeginEnd("12-00 23:00"));
  EXPECT_FALSE(IsCorrectTimeBeginEnd("12:00 23-00"));
  EXPECT_FALSE(IsCorrectTimeBeginEnd("12:00-23:00"));
}

TEST(ValidationTest, LowerAlpha)
{
  EXPECT_TRUE(isLower('a'));
  EXPECT_FALSE(isLower('A'));
  EXPECT_FALSE(isLower('0'));
  EXPECT_FALSE(isLower(':'));
}

TEST(TimeTest, Parsing)
{
  EXPECT_EQ(ParsTime("10:00"), 600);
  EXPECT_EQ(ParsTime("00:00"), 0);
}

TEST(ParserTest, InvalidLine)
{
  Event ev1 = ParsLine("15:00 1 cl5", 10);
  EXPECT_TRUE(ev1.valid);

  Event ev2 = ParsLine("15:00 2 cl5 7", 10);
  EXPECT_TRUE(ev2.valid);

  Event ev3 = ParsLine("a5:00 1 cl5", 10);
  EXPECT_FALSE(ev3.valid);

  Event ev4 = ParsLine("1a:00 1 cl5", 10);
  EXPECT_FALSE(ev4.valid);

  Event ev5 = ParsLine("15-00 1 cl5", 10);
  EXPECT_FALSE(ev5.valid);

  Event ev6 = ParsLine("15:a0 1 cl5", 10);
  EXPECT_FALSE(ev6.valid);

  Event ev7 = ParsLine("15:0a 1 cl5", 10);
  EXPECT_FALSE(ev7.valid);

  Event ev8 = ParsLine("15:00-1 cl5", 10);
  EXPECT_FALSE(ev8.valid);

  Event ev9 = ParsLine("à5:001 cl5", 10);
  EXPECT_FALSE(ev9.valid);

  Event ev10 = ParsLine("15:00 a cl5", 10);
  EXPECT_FALSE(ev10.valid);

  Event ev11 = ParsLine("15:00 7 cl5", 10);
  EXPECT_FALSE(ev11.valid);

  Event ev12 = ParsLine("15:00 0 cl5", 10);
  EXPECT_FALSE(ev12.valid);

  Event ev13 = ParsLine("15:00 15 cl5", 10);
  EXPECT_FALSE(ev13.valid);

  Event ev14 = ParsLine("15:00 1cl5", 10);
  EXPECT_FALSE(ev14.valid);

  Event ev15 = ParsLine("15:00 1:cl5", 10);
  EXPECT_FALSE(ev15.valid);

  Event ev16 = ParsLine("15:00 1 cP5", 10);
  EXPECT_FALSE(ev16.valid);

  Event ev17 = ParsLine("15:00 1 cl 5", 10);
  EXPECT_FALSE(ev17.valid);

  Event ev18 = ParsLine("15:00 1 cl5 ", 10);
  EXPECT_FALSE(ev18.valid);

  Event ev19 = ParsLine("15:00 1 c>l5", 10);
  EXPECT_FALSE(ev19.valid);

  Event ev20 = ParsLine("15:00 2 cl5", 10);
  EXPECT_FALSE(ev20.valid);

  Event ev21 = ParsLine("15:00 2 cl5 ", 10);
  EXPECT_FALSE(ev21.valid);

  Event ev22 = ParsLine("15:00 2 cl5 ab", 10);
  EXPECT_FALSE(ev22.valid);

  Event ev23 = ParsLine("15:00 2 cl5 0", 10);
  EXPECT_FALSE(ev23.valid);

  Event ev24 = ParsLine("15:00 2 cl5 17", 10);
  EXPECT_FALSE(ev24.valid);

  Event ev25 = ParsLine("invalid stuff", 10);
  EXPECT_FALSE(ev25.valid);
}

TEST(TableProcessingTest, BaseTest1)
{
  vector<pair<long long, int>> tables(5);
  for (auto el: tables)
  {
    el.first = 0;
    el.second = 0;
  }

  int prevTime = 60;
  int nextTime = 120;
  int price = 10;
  int numTable = 3;

  TableProcessing(tables, price, nextTime, prevTime, numTable);

  EXPECT_EQ(tables[3].first, 10);
  EXPECT_EQ(tables[3].second, 60);

  EXPECT_EQ(tables[1].first, 0);
  EXPECT_EQ(tables[1].second, 0);

  EXPECT_EQ(tables[2].first, 0);
  EXPECT_EQ(tables[2].second, 0);

  EXPECT_EQ(tables[4].first, 0);
  EXPECT_EQ(tables[4].second, 0);
}

TEST(TableProcessingTest, BaseTest2)
{
  vector<pair<long long, int>> tables(5);
  tables[1] = {100, 51};
  tables[2] = {50, 13};
  tables[3] = {20, 40};
  tables[4] = {50, 71};

  int prevTime = 60;
  int nextTime = 120;
  int price = 10;
  int numTable = 2;

  TableProcessing(tables, price, nextTime, prevTime, numTable);

  EXPECT_EQ(tables[1].first, 100);
  EXPECT_EQ(tables[1].second, 51);

  EXPECT_EQ(tables[2].first, 60);
  EXPECT_EQ(tables[2].second, 73);

  EXPECT_EQ(tables[3].first, 20);
  EXPECT_EQ(tables[3].second, 40);

  EXPECT_EQ(tables[4].first, 50);
  EXPECT_EQ(tables[4].second, 71);
}


TEST(TableProcessingTest, BaseTest3)
{
  vector<pair<long long, int>> tables(5);
  for (auto el: tables)
  {
    el.first = 0;
    el.second = 0;
  }

  int prevTime = 60;
  int nextTime = 125;
  int price = 10;
  int numTable = 3;

  TableProcessing(tables, price, nextTime, prevTime, numTable);

  EXPECT_EQ(tables[3].first, 20);
  EXPECT_EQ(tables[3].second, 65);
}

TEST(TableProcessingTest, Test1)
{
  vector<pair<long long, int>> tables(5);
  for (auto el: tables)
  {
    el.first = 0;
    el.second = 0;
  }

  int prevTime = 60;
  int nextTime = 120;
  int price = 10;
  int numTable = 3;

  TableProcessing(tables, price, nextTime, prevTime, numTable);

  EXPECT_EQ(tables[3].first, 10);
  EXPECT_EQ(tables[3].second, 60);

  prevTime = 210;
  nextTime = 230;

  TableProcessing(tables, price, nextTime, prevTime, numTable);

  EXPECT_EQ(tables[3].first, 20);
  EXPECT_EQ(tables[3].second, 80);
}

TEST(TableProcessingTest, Test2)
{
  vector<pair<long long, int>> tables(5);
  for (auto el: tables)
  {
    el.first = 0;
    el.second = 0;
  }

  int prevTime = 60;
  int nextTime = 120;
  int price = 10;
  int numTable = 3;

  TableProcessing(tables, price, nextTime, prevTime, numTable);

  EXPECT_EQ(tables[3].first, 10);
  EXPECT_EQ(tables[3].second, 60);

  prevTime = 210;
  nextTime = 230;
  numTable = 2;

  TableProcessing(tables, price, nextTime, prevTime, numTable);

  EXPECT_EQ(tables[3].first, 10);
  EXPECT_EQ(tables[3].second, 60);

  EXPECT_EQ(tables[2].first, 10);
  EXPECT_EQ(tables[2].second, 20);
}

//клиент успешно пришёл
TEST(ClientEnterTest, SuccessfulEntry)
{
  string testFile = "test1.txt";
  ofstream file(testFile);
  set<string> clients;
  int timeBegin = 9 * 60;
  int timeEnd = 19 * 60;

  Event event;
  event.time = "09:00";
  event.idCl = "client1";
  event.t = 9 * 60;

  ClientEnter(file, event, clients, timeBegin, timeEnd);
  file.close();


  ifstream in(testFile);
  string line;
  getline(in, line);
  in.close();

  EXPECT_EQ(line, "09:00 1 client1");
  EXPECT_TRUE(clients.find("client1") != clients.end());

  remove(testFile.c_str());
}

//клиент пришёл раньше открытия
TEST(ClientEnterTest, ClubClose1)
{
  string testFile = "test2.txt";
  ofstream file(testFile);
  set<string> clients;
  int timeBegin = 9 * 60;
  int timeEnd = 19 * 60;

  Event event;
  event.time = "08:59";
  event.idCl = "client1";
  event.t = 8 * 60 + 59;

  ClientEnter(file, event, clients, timeBegin, timeEnd);
  file.close();


  ifstream in(testFile);
  string line1, line2;
  getline(in, line1);
  getline(in, line2);
  in.close();

  EXPECT_EQ(line1, "08:59 1 client1");
  EXPECT_EQ(line2, "08:59 13 NotOpenYet");
  EXPECT_TRUE(clients.find("client1") == clients.end());

  remove(testFile.c_str());
}

//клиент пришёл после закрытия
TEST(ClientEnterTest, ClubClose2)
{
  string testFile = "test2.txt";
  ofstream file(testFile);
  set<string> clients;
  int timeBegin = 9 * 60;
  int timeEnd = 19 * 60;

  Event event;
  event.time = "19:01";
  event.idCl = "client1";
  event.t = 19 * 60 + 1;

  ClientEnter(file, event, clients, timeBegin, timeEnd);
  file.close();


  ifstream in(testFile);
  string line1, line2;
  getline(in, line1);
  getline(in, line2);
  in.close();

  EXPECT_EQ(line1, "19:01 1 client1");
  EXPECT_EQ(line2, "19:01 13 NotOpenYet");
  EXPECT_TRUE(clients.find("client1") == clients.end());

  remove(testFile.c_str());
}

//клиент уже в клубе
TEST(ClientEnterTest, ClientAlreadyInTheClub)
{
  string testFile = "test3.txt";
  ofstream file(testFile);
  set<string> clients;

  clients.insert("client1");
  int timeBegin = 9 * 60;
  int timeEnd = 19 * 60;

  Event event;
  event.time = "09:05";
  event.idCl = "client1";
  event.t = 9 * 60 + 5;

  ClientEnter(file, event, clients, timeBegin, timeEnd);
  file.close();


  ifstream in(testFile);
  string line1, line2;
  getline(in, line1);
  getline(in, line2);
  in.close();

  EXPECT_EQ(line1, "09:05 1 client1");
  EXPECT_EQ(line2, "09:05 13 YouShallNotPass");
  EXPECT_TRUE(clients.find("client1") != clients.end());

  remove(testFile.c_str());
}

//клиент уcпешно сел за стол
TEST(ClientSitTest, SuccessfulSitBase)
{
  string testFile = "test3.txt";
  ofstream file(testFile);
  set<string> clients;
  unordered_set<int> freeTable;
  unordered_map<string, pair<int, int>> tableClients;
  vector<pair<long long, int>> tables(4);

  clients.insert("client1");
  for (int i = 1; i <= 3; i++)
  {
    tables[i] = {0, 0};
  }
  for (int i = 1; i <= 3; i++)
  {
    freeTable.insert(i);
  }
  int price = 10;

  Event event;
  event.time = "09:05";
  event.idCl = "client1";
  event.t = 9 * 60 + 5;
  event.numTable = 1;

  ClientSit(file, event, clients, freeTable, tableClients, tables, price);
  file.close();


  ifstream in(testFile);
  string line1;
  getline(in, line1);
  in.close();

  EXPECT_EQ(line1, "09:05 2 client1 1");
  EXPECT_EQ(freeTable.size(), 2);
  EXPECT_EQ(tableClients["client1"].first, 1);
  EXPECT_EQ(tableClients["client1"].second, 9 * 60 + 5);

  remove(testFile.c_str());
}

//клиент не в клубе
TEST(ClientSitTest, ClientUnknown)
{
  string testFile = "test3.txt";
  ofstream file(testFile);
  set<string> clients;
  unordered_set<int> freeTable;
  unordered_map<string, pair<int, int>> tableClients;
  vector<pair<long long, int>> tables(4);

  for (int i = 1; i <= 3; i++)
  {
    tables[i] = {0, 0};
  }
  for (int i = 1; i <= 3; i++)
  {
    freeTable.insert(i);
  }
  int price = 100;

  Event event;
  event.time = "10:30";
  event.idCl = "client1";
  event.t = 10 * 60 + 30;
  event.numTable = 3;

  ClientSit(file, event, clients, freeTable, tableClients, tables, price);
  file.close();

  ifstream in(testFile);
  string line1, line2;
  getline(in, line1);
  getline(in, line2);
  in.close();

  EXPECT_EQ(line1, "10:30 2 client1 3");
  EXPECT_EQ(line2, "10:30 13 ClientUnknown");
  EXPECT_EQ(freeTable.size(), 3);
  EXPECT_EQ(tableClients.size(), 0);

  remove(testFile.c_str());
}

//стол занят
TEST(ClientSitTest, TableBusy)
{
  string testFile = "test3.txt";
  ofstream file(testFile);
  set<string> clients;
  unordered_set<int> freeTable;
  unordered_map<string, pair<int, int>> tableClients;
  vector<pair<long long, int>> tables(4);

  clients.insert("client1");
  clients.insert("client2");

  for (int i = 1; i <= 3; i++)
  {
    tables[i] = {0, 0};
  }
  for (int i = 2; i <= 3; i++)
  {
    freeTable.insert(i);
  }
  int price = 10;

  tableClients["client2"] = {1, 9 * 60 + 5};

  Event event;
  event.time = "09:06";
  event.idCl = "client1";
  event.t = 9 * 60 + 6;
  event.numTable = 1;

  ClientSit(file, event, clients, freeTable, tableClients, tables, price);
  file.close();

  ifstream in(testFile);
  string line1, line2;
  getline(in, line1);
  getline(in, line2);
  in.close();

  EXPECT_EQ(line1, "09:06 2 client1 1");
  EXPECT_EQ(line2, "09:06 13 PlaceIsBusy");
  EXPECT_EQ(freeTable.size(), 2);
  EXPECT_EQ(tableClients.size(), 1);
  EXPECT_EQ(tableClients["client2"].first, 1);
  EXPECT_EQ(tableClients["client2"].second, 9 * 60 + 5);

  remove(testFile.c_str());
}


//клиент пересел
TEST(ClientSitTest, ClientChangeTable)
{
  string testFile = "test3.txt";
  ofstream file(testFile);
  set<string> clients;
  unordered_set<int> freeTable;
  unordered_map<string, pair<int, int>> tableClients;
  vector<pair<long long, int>> tables(4);

  clients.insert("client1");
  for (int i = 1; i <= 3; i++)
  {
    tables[i] = {0, 0};
  }
  for (int i = 1; i <= 3; i++)
  {
    freeTable.insert(i);
  }
  int price = 100;

  Event event;
  event.time = "10:30";
  event.idCl = "client1";
  event.t = 10 * 60 + 30;
  event.numTable = 3;

  ClientSit(file, event, clients, freeTable, tableClients, tables, price);

  EXPECT_EQ(freeTable.size(), 2);
  EXPECT_EQ(tableClients["client1"].first, 3);
  EXPECT_EQ(tableClients["client1"].second, 10 * 60 + 30);

  event.time = "11:00";
  event.idCl = "client1";
  event.t = 11 * 60;
  event.numTable = 2;

  ClientSit(file, event, clients, freeTable, tableClients, tables, price);
  file.close();

  ifstream in(testFile);
  string line1, line2;
  getline(in, line1);
  getline(in, line2);
  in.close();

  EXPECT_EQ(line1, "10:30 2 client1 3");
  EXPECT_EQ(line2, "11:00 2 client1 2");
  EXPECT_EQ(freeTable.size(), 2);
  EXPECT_EQ(tableClients["client1"].first, 2);
  EXPECT_EQ(tableClients["client1"].second, 11 * 60);
  EXPECT_EQ(tables[3].first, 100);
  EXPECT_EQ(tables[3].second, 30);

  remove(testFile.c_str());
}

//клиент успешно попадает в очередь ожидания
TEST(ClientWaitTest, SuccessfulWait1)
{
  string testFile = "test3.txt";
  ofstream file(testFile);
  set<string> clients;
  queue<string> waiting;
  int n = 3;
  size_t k = 0;

  clients.insert("client1");

  Event event;
  event.time = "12:00";
  event.idCl = "client1";
  event.t = 12 * 60;

  ClientWait(file, event, clients, k, waiting, n);
  file.close();

  ifstream in(testFile);
  string line1;
  getline(in, line1);
  in.close();

  EXPECT_EQ(line1, "12:00 3 client1");
  EXPECT_EQ(waiting.front(), "client1");

  remove(testFile.c_str());
}

TEST(ClientWaitTest, SuccessfulWait2)
{
  string testFile = "test3.txt";
  ofstream file(testFile);
  set<string> clients;
  queue<string> waiting;

  int n = 3;
  size_t k = 0;

  clients.insert("client1");
  clients.insert("client1");

  Event event;
  event.time = "12:00";
  event.idCl = "client1";
  event.t = 12 * 60;

  ClientWait(file, event, clients, k, waiting, n);

  EXPECT_EQ(waiting.size(), 1);

  event.time = "13:00";
  event.idCl = "client2";
  event.t = 13 * 60;

  ClientWait(file, event, clients, k, waiting, n);
  file.close();

  ifstream in(testFile);
  string line1, line2;
  getline(in, line1);
  getline(in, line2);
  in.close();

  EXPECT_EQ(line1, "12:00 3 client1");
  EXPECT_EQ(line2, "13:00 3 client2");
  EXPECT_EQ(waiting.size(), 2);

  remove(testFile.c_str());
}

//есть свободные столы
TEST(ClientWaitTest, FreeTables)
{
  string testFile = "test3.txt";
  ofstream file(testFile);
  set<string> clients;
  queue<string> waiting;
  int n = 3;
  size_t k = 1;

  clients.insert("client1");

  Event event;
  event.time = "12:00";
  event.idCl = "client1";
  event.t = 12 * 60;

  ClientWait(file, event, clients, k, waiting, n);
  file.close();

  ifstream in(testFile);
  string line1, line2;
  getline(in, line1);
  getline(in, line2);
  in.close();

  EXPECT_EQ(line1, "12:00 3 client1");
  EXPECT_EQ(line2, "12:00 13 ICanWaitNoLonger!");
  EXPECT_EQ(waiting.size(), 0);

  remove(testFile.c_str());
}

//слишком длинная очередь
TEST(ClientWaitTest, ManyPeopleInTheQueue)
{
  string testFile = "test3.txt";
  ofstream file(testFile);
  set<string> clients;
  queue<string> waiting;
  int n = 3;
  size_t k = 0;
  waiting.push("client1");
  waiting.push("client2");
  waiting.push("client3");

  clients.insert("client4");

  Event event;
  event.time = "12:00";
  event.idCl = "client4";
  event.t = 12 * 60;

  ClientWait(file, event, clients, k, waiting, n);
  file.close();

  ifstream in(testFile);
  string line1, line2;
  getline(in, line1);
  getline(in, line2);
  in.close();

  EXPECT_EQ(line1, "12:00 3 client4");
  EXPECT_EQ(line2, "12:00 11 client4");
  EXPECT_EQ(waiting.size(), 3);

  remove(testFile.c_str());
}

//уход клиента, не занимавшего стол, в очереди ожидания никого нет
TEST(ClientExitTest, SuccessfulExit1)
{
  string testFile = "test3.txt";
  ofstream file(testFile);
  set<string> clients;
  unordered_map<string, pair<int, int>> tableClients;
  vector<pair<long long, int>> tables(4);
  unordered_set<int> freeTable;

  queue<string> waiting;

  int price = 100;
  clients.insert("client1");

  Event event;
  event.time = "15:00";
  event.idCl = "client1";
  event.t = 15 * 60;

  for (auto el: tables)
  {
    el = {0, 0};
  }
  for (int i = 1; i <= 3; i++)
  {
    freeTable.insert(i);
  }

  ClientExit(file, event, clients, tableClients, price, tables, waiting, freeTable);
  file.close();

  ifstream in(testFile);
  string line1;
  getline(in, line1);
  in.close();

  EXPECT_EQ(line1, "15:00 4 client1");
  EXPECT_EQ(waiting.size(), 0);
  EXPECT_EQ(freeTable.size(), 3);
  EXPECT_TRUE(clients.find("client1") == clients.end());
  EXPECT_TRUE(tableClients.find("client1") == tableClients.end());
  EXPECT_EQ(tables[1].first, 0);
  EXPECT_EQ(tables[1].second, 0);
  EXPECT_EQ(tables[2].first, 0);
  EXPECT_EQ(tables[2].second, 0);
  EXPECT_EQ(tables[3].first, 0);
  EXPECT_EQ(tables[3].second, 0);

  remove(testFile.c_str());
}

//уход клиента, занимавшего стол, в очереди ожидания никого нет
TEST(ClientExitTest, SuccessfulExit2)
{
  string testFile = "test3.txt";
  ofstream file(testFile);
  set<string> clients;
  unordered_map<string, pair<int, int>> tableClients;
  vector<pair<long long, int>> tables(4);
  unordered_set<int> freeTable;

  queue<string> waiting;

  int price = 100;
  clients.insert("client1");
  tableClients["client1"] = make_pair(2, 12 * 60);

  Event event;
  event.time = "15:00";
  event.idCl = "client1";
  event.t = 15 * 60;

  for (auto el: tables)
  {
    el = {0, 0};
  }
  freeTable.insert(1);
  freeTable.insert(3);

  ClientExit(file, event, clients, tableClients, price, tables, waiting, freeTable);
  file.close();

  ifstream in(testFile);
  string line1;
  getline(in, line1);
  in.close();

  EXPECT_EQ(line1, "15:00 4 client1");
  EXPECT_EQ(waiting.size(), 0);
  EXPECT_EQ(freeTable.size(), 3);
  EXPECT_TRUE(clients.find("client1") == clients.end());
  EXPECT_TRUE(tableClients.find("client1") == tableClients.end());
  EXPECT_EQ(tables[1].first, 0);
  EXPECT_EQ(tables[1].second, 0);
  EXPECT_EQ(tables[2].first, 300);
  EXPECT_EQ(tables[2].second, 180);
  EXPECT_EQ(tables[3].first, 0);
  EXPECT_EQ(tables[3].second, 0);

  remove(testFile.c_str());
}

//уход клиента, занимавшего стол, в очереди ожидания есть ещё клиенты
TEST(ClientExitTest, SuccessfulExit3)
{
  string testFile = "test3.txt";
  ofstream file(testFile);
  set<string> clients;
  unordered_map<string, pair<int, int>> tableClients;
  vector<pair<long long, int>> tables(4);
  unordered_set<int> freeTable;

  queue<string> waiting;

  int price = 100;
  clients.insert("client1");
  clients.insert("client2");
  clients.insert("client3");
  clients.insert("client4");

  tableClients["client1"] = make_pair(2, 12 * 60);
  tableClients["client2"] = make_pair(1, 11 * 60);
  tableClients["client3"] = make_pair(3, 10 * 60);

  waiting.push("client4");

  Event event;
  event.time = "15:00";
  event.idCl = "client1";
  event.t = 15 * 60;

  for (auto el: tables)
  {
    el = {0, 0};
  }

  ClientExit(file, event, clients, tableClients, price, tables, waiting, freeTable);
  file.close();

  ifstream in(testFile);
  string line1, line2;
  getline(in, line1);
  getline(in, line2);
  in.close();

  EXPECT_EQ(line1, "15:00 4 client1");
  EXPECT_EQ(line2, "15:00 12 client4 2");
  EXPECT_EQ(waiting.size(), 0);
  EXPECT_EQ(freeTable.size(), 0);
  EXPECT_TRUE(clients.find("client1") == clients.end());
  EXPECT_TRUE(tableClients.find("client1") == tableClients.end());
  EXPECT_EQ(tables[1].first, 0);
  EXPECT_EQ(tables[1].second, 0);
  EXPECT_EQ(tables[2].first, 300);
  EXPECT_EQ(tables[2].second, 180);
  EXPECT_EQ(tables[3].first, 0);
  EXPECT_EQ(tables[3].second, 0);

  EXPECT_TRUE(clients.find("client4") != clients.end());
  EXPECT_TRUE(tableClients.find("client4") != tableClients.end());

  EXPECT_EQ(tableClients["client4"].first, 2);
  EXPECT_EQ(tableClients["client4"].second, 15 * 60);

  EXPECT_EQ(tableClients["client2"].first, 1);
  EXPECT_EQ(tableClients["client2"].second, 11 * 60);

  EXPECT_EQ(tableClients["client3"].first, 3);
  EXPECT_EQ(tableClients["client3"].second, 10 * 60);

  remove(testFile.c_str());
}

//уход клиента, которого не было в клубе
TEST(ClientExitTest, ClientUnknown)
{
  string testFile = "test3.txt";
  ofstream file(testFile);
  set<string> clients;
  unordered_map<string, pair<int, int>> tableClients;
  vector<pair<long long, int>> tables(4);
  unordered_set<int> freeTable;

  queue<string> waiting;

  int price = 100;

  Event event;
  event.time = "15:00";
  event.idCl = "client1";
  event.t = 15 * 60;

  for (auto el: tables)
  {
    el = {0, 0};
  }
  for (int i = 1; i <= 3; i++)
  {
    freeTable.insert(i);
  }

  ClientExit(file, event, clients, tableClients, price, tables, waiting, freeTable);
  file.close();

  ifstream in(testFile);
  string line1, line2;
  getline(in, line1);
  getline(in, line2);
  in.close();

  EXPECT_EQ(line1, "15:00 4 client1");
  EXPECT_EQ(line2, "15:00 13 ClientUnknown");
  EXPECT_EQ(waiting.size(), 0);
  EXPECT_EQ(freeTable.size(), 3);
  EXPECT_EQ(clients.size(), 0);
  EXPECT_EQ(tableClients.size(), 0);
  EXPECT_EQ(tables[1].first, 0);
  EXPECT_EQ(tables[1].second, 0);
  EXPECT_EQ(tables[2].first, 0);
  EXPECT_EQ(tables[2].second, 0);
  EXPECT_EQ(tables[3].first, 0);
  EXPECT_EQ(tables[3].second, 0);

  remove(testFile.c_str());
}
