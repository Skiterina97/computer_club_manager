#include "logic.h"

using namespace std;

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    cerr << "This program must run with 1 argument (input filename)";
    return 1;
  }

  //открываем файл для чтения данных
  ifstream file(argv[1]);

  //проверяем, что он открылся
  if (!file.is_open())
  {
    cerr << "File not open" << endl;
    return 1;
  }

  //cчитываем количество столов
  string number;
  getline(file, number);
  if (!IsPositiveInteger(number))
  {
    file.close();
    ofstream newFile("output.txt");
    if (!newFile.is_open())
    {
      cerr << "File not open" << endl;
      return 1;
    }
    newFile << number << endl;
    newFile.close();
    return 0;
  }
  int n;
  n = stoi(number);

  //считываем время начала и конца, проверяем корректную запись самой строки
  string timeBeginEnd;
  getline(file, timeBeginEnd);
  if (!IsCorrectTimeBeginEnd(timeBeginEnd))
  {
    file.close();
    ofstream newFile("output.txt");
    if (!newFile.is_open())
    {
      cerr << "File not open" << endl;
      return 1;
    }
    newFile << timeBeginEnd;
    newFile.close();
    return 0;
  }

  //проверяем, что заданное время работы корректно
  int timeBegin, timeEnd;
  string tb = timeBeginEnd.substr(0, 5);
  string te = timeBeginEnd.substr(6, 5);
  timeBegin = ParsTime(tb);
  timeEnd = ParsTime(te);
  if (timeEnd == 0)
  {
    timeEnd = 1440;
  }

  if (stoi(tb.substr(0, 2)) > 23 || stoi(tb.substr(3, 2)) > 59 || stoi(te.substr(0, 2)) > 23 || stoi(te.substr(3, 2)) > 59 ||
      timeBegin > timeEnd)
  {
    file.close();
    ofstream newFile("output.txt");
    if (!newFile.is_open())
    {
      cerr << "File not open" << endl;
      return 1;
    }
    newFile << timeBeginEnd;
    newFile.close();
    return 0;
  }

  //считываем цену одного часа и проверяем её корректность
  string pr;
  getline(file, pr);
  if (!IsPositiveInteger(pr))
  {
    file.close();
    ofstream newFile("output.txt");
    if (!newFile.is_open())
    {
      cerr << "File not open" << endl;
      return 1;
    }
    newFile << pr << endl;
    newFile.close();
    return 0;
  }
  int price;
  price = stoi(pr);

  //создаём вектор для хранения структур событий
  vector<Event> events;
  string line;
  while(getline(file, line))
  {
    Event ev;
    ev = ParsLine(line, n);
    //если запись некорректна, завершаем работу
    if (!ev.valid)
    {
      file.close();
      ofstream newFile("output.txt");
      if (!newFile.is_open())
      {
        cerr << "File not open" << endl;
        return 1;
      }
      newFile << line;
      newFile.close();
      return 0;
    }

    //если время текущего события меньше времени предыдущего, то некорректно и выводим
    int temp = events.size();
    temp--;
    if (temp >= 0 && ev.t < events[temp].t)
    {
      file.close();
      ofstream newFile("output.txt");
      if (!newFile.is_open())
      {
        cerr << "File not open" << endl;
        return 1;
      }
      newFile << line;
      newFile.close();
      return 0;
    }

    events.push_back(ev);
  }

  file.close();

  //вектор для хранения информации о столах (выручка и количество минут, которое отработал стол за день)
  vector<pair<long long, int>> tables(n + 1, {0, 0});

  //номера свободных столов в текущий момент времени (в начале все столы свободны)
  unordered_set<int> freeTable;
  for (int i = 1; i <= n; i++)
  {
    freeTable.insert(i);
  }

  //список клиентов, находящихся в клубе в данный момент времени
  set<string> clients;

  //список клиентов, находящихся за столами в данный момени времени
  //для каждого клиента храним номер стола и время, в которое он за него сел
  unordered_map<string, pair<int, int>> tableClients;

  //список ожидающих клиентов
  queue<string> waiting;

  //открываем файл для записи
  ofstream newFile("output.txt");
  if (!newFile.is_open())
  {
    cerr << "File not open" << endl;
    return 1;
  }

  //записываем время открытия
  newFile << tb << endl;

  //флаг для отслеживания конца рабочего дня
  bool flagClose = false;

  //идём по событиям
  for (int i = 0; i < events.size(); i++)
  {
    //если конец рабочего дня уже наступил
    if (!flagClose && events[i].t > timeEnd)
    {
      for (auto el: clients)
      {
        newFile << te << " " << 11 << " " << el << endl;
        if (tableClients.find(el) != tableClients.end())
        {
          TableProcessing(tables, price, timeEnd, tableClients[el].second, tableClients[el].first);
        }
      }
      clients.clear();
      flagClose = true;
    }

    //событие типа 1 (Клиент пришёл)
    if (events[i].idOp == 1)
    {
      ClientEnter(newFile, events[i], clients, timeBegin, timeEnd);
      continue;
    }

    //событие типа 2 (клиент сел за стол)
    if (events[i].idOp == 2)
    {
      ClientSit(newFile, events[i], clients, freeTable, tableClients, tables, price);
      continue;
    }

    //событие типа 3 (клиент ожидает)
    if (events[i].idOp == 3)
    {
      size_t k = freeTable.size();
      ClientWait(newFile, events[i], clients, k, waiting, n);
      continue;
    }

    //событие типа 4 (клиент ушёл)
    if (events[i].idOp == 4)
    {
      ClientExit(newFile, events[i], clients, tableClients, price, tables, waiting, freeTable);
    }
  }

  //по окончанию рабочего дня для всех клиентов, оставшихся в клубе делаем проверку на то, сидели ли они за каким-то столом
  //если да, то рассчитываем выручку и время работы этого стола
  //делаем записи событий типа 11
  //если не сделали этого в процессе обработки событий
  if (!flagClose)
  {
    for (auto el: clients)
    {
      newFile << te << " " << 11 << " " << el << endl;
      if (tableClients.find(el) != tableClients.end())
      {
        TableProcessing(tables, price, timeEnd, tableClients[el].second, tableClients[el].first);
      }
    }
    clients.clear();
  }

  //делаем запись времени конца работы
  newFile << te << endl;

  //для каждого стола выводим выручку и время его работы за день
  for (int i = 1; i <= n; i++)
  {
    newFile << i << " " << tables[i].first << " ";
    if (tables[i].second / 60 < 10)
    {
      newFile << "0" << tables[i].second / 60 << ":";
    }
    else
    {
      newFile << tables[i].second / 60 << ":";
    }

    if (tables[i].second % 60 < 10)
    {
      newFile << "0" << tables[i].second % 60 << endl;
    }
    else
    {
      newFile << tables[i].second % 60 << endl;
    }
  }

  newFile.close();
  return 0;
}
