#include "logic.h"

using namespace std;

//проверка на то, является ли число целым и положительным
bool IsPositiveInteger(const string &str)
{
  for (auto c: str)
  {
    if (!isdigit(c))
    {
      return false;
    }
  }

  if (str.length() == 1 && str[0] == '0')
  {
    return false;
  }

  return true;
}

//проверка корректности второй строки со временем начала и конца работы клубы (хх:хх хх:хх)
bool IsCorrectTimeBeginEnd(const string &str)
{
  //наличие лишних символов
  if (str.length() != 11)
  {
    return false;
  }

  //не числовые значения
  if (str[2] != ':' || str[5] != ' ' || str[8] != ':')
  {
    return false;
  }

  //цифры
  if (!isdigit(str[0]) || !isdigit(str[1]) || !isdigit(str[3]) || !isdigit(str[4]) || !isdigit(str[6]) ||
      !isdigit(str[7]) || !isdigit(str[9]) || !isdigit(str[10]))
  {
    return false;
  }

  return true;
}

//функция перевода времени из строкового формата хх:хх в числовой в минутах от 00:00
int ParsTime(const string &str)
{
  int t;
  t = stoi(str.substr(0, 2)) * 60 + stoi(str.substr(3, 2));
  return t;
}

//проверка, что символ являктся строчной английской буквой
bool isLower(char c)
{
  return (c >= 'a' && c <= 'z');
}

//функция перевода строки с событием в структуру и проверки корректности ввода данных
Event ParsLine(const string &str, const int &n)
{
  Event evTemp;
  evTemp;

  //в строке должно быть минимум 9 символов:
  //время хх:хх (5 символов)
  //пробел (1 символ)
  //номер события 1, 2, 3 или 4 (1 символ)
  //пробел (1 символ)
  //имя клиента (минимум 1 символ)
  // для операции 2 символов больше, так как ещё указывается номер стола
  if (str.length() < 9)
  {
    evTemp.valid = false;
    return evTemp;
  }

  //если время задано в неправильном формате, делаем структуру невалидной и выходим из функции
  evTemp.time = str.substr(0, 5);
  if (!isdigit(str[0]) || !isdigit(str[1]) || str[2] != ':' || !isdigit(str[3]) || !isdigit(str[4]))
  {
    evTemp.valid = false;
    return evTemp;
  }

  //переводим время в целочисленый формат и проверяем корректно ли оно по значениям (часов не больше 23, минут не больше 59)
  evTemp.t = ParsTime(evTemp.time);
  if (stoi(evTemp.time.substr(0, 2)) > 23 || stoi(evTemp.time.substr(3, 2)) > 59)
  {
    evTemp.valid = false;
    return evTemp;
  }

  size_t pos = 5;

  //проверяем, что после времени идёт разделитель
  if (str[pos] != ' ')
  {
    evTemp.valid = false;
    return evTemp;
  }

  //проверяем номер операции - 6-ой символ в строке
  pos++;

  //проверяем, что это число
  if (!isdigit(str[pos]))
  {
    evTemp.valid = false;
    return evTemp;
  }
  evTemp.idOp = stoi(str.substr(pos, 1));

  //проверяем, что оно от 1 до 4
  if (evTemp.idOp < 1 || evTemp.idOp > 4)
  {
    evTemp.valid = false;
    return evTemp;
  }

  //проверяем, что после номера операции идёт разделитель
  pos++;
  if (str[pos] != ' ')
  {
    evTemp.valid = false;
    return evTemp;
  }

  //проверяем имя клиента
  pos++;

  //если у нас 1, 3 или 4 событие, то имя клиента идёт до конца строки
  if (evTemp.idOp != 2)
  {
    evTemp.idCl = str.substr(pos);
  }
  //если событие 2, то до следующего пробела
  else
  {
    size_t nextPos = str.find(' ', pos);
    //если пробелов больше нет, то событие 2 задано некорректно
    if (nextPos == string::npos)
    {
      evTemp.valid = false;
      return evTemp;
    }
    evTemp.idCl = str.substr(pos, nextPos - pos);
    pos = nextPos;
  }

  //проверяем имя клиента на корректность (только разрешённые символы)
  for (auto c: evTemp.idCl)
  {
    if (c != '-' && c != '_' && !isdigit(c) && !isLower(c))
    {
      evTemp.valid = false;
      break;
    }
  }

  if (!evTemp.valid)
  {
    return evTemp;
  }

  //для события 2 проверяем номер стола, он должен быть до конца строки и состоять из цифр
  if (evTemp.idOp == 2)
  {
    pos++;
    //если пробел был последним и после него нет номера стола
    if (pos == str.length())
    {
      evTemp.valid = false;
      return evTemp;
    }
    string number = str.substr(pos);
    for (auto c: number)
    {
      if (!isdigit(c))
      {
        evTemp.valid = false;
        break;
      }
    }

    if (!evTemp.valid)
    {
      return evTemp;
    }

    evTemp.numTable = stoi(number);
    //проверка, что номер стола корректен
    if (evTemp.numTable > n || evTemp.numTable == 0)
    {
      evTemp.valid = false;
      return evTemp;
    }
  }

  return evTemp;
}

//функция увеличения времени работы стола и его выручки
void TableProcessing(vector<pair<long long, int>> &tables, int &price, int &nextTime, int &prevTime, int &numTable)
{
  int timeThisTable = nextTime - prevTime;
  tables[numTable].first += price * (timeThisTable / 60);
  if (timeThisTable % 60 != 0)
  {
    tables[numTable].first += price;
  }
  tables[numTable].second += timeThisTable;
  return;
}

//функция обработки события 1 (клиент пришёл)
void ClientEnter(ofstream& newFile, Event &ev, set<string> &clients, int &timeBegin, int &timeEnd)
{
  //делаем запись входящего события
  newFile << ev.time << " " << 1 << " " << ev.idCl << endl;

  //проверяем не находится ли клиент уже в клубе
  if (clients.find(ev.idCl) != clients.end())
  {
    newFile << ev.time << " " << 13 << " YouShallNotPass" << endl;
    return;
  }

  //проверяем открыт ли клуб
  if (ev.t < timeBegin || ev.t >= timeEnd)
  {
    newFile << ev.time << " " << 13 << " NotOpenYet" << endl;
    return;
  }

  //если всё в порядке, то заносим в список клиентов, находящихся в клубе
  clients.insert(ev.idCl);
  return;
}

//функция обработки события 2 (клиент сел за стол)
void ClientSit(ofstream& newFile, Event &ev, const set<string> &clients, unordered_set<int> &freeTable,
               unordered_map<string, pair<int, int>> &tableClients, vector<pair<long long, int>> &tables, int &price)
{
  //делаем запись входящего события
  newFile << ev.time << " " << 2 << " " << ev.idCl << " " << ev.numTable << endl;

  //проверяем, находится ли клиент в клубе
  if (clients.find(ev.idCl) == clients.end())
  {
    newFile << ev.time << " " << 13 << " ClientUnknown" << endl;
    return;
  }

  //проверяем свободен ли стол
  if (freeTable.find(ev.numTable) == freeTable.end())
  {
    newFile << ev.time << " " << 13 << " PlaceIsBusy" << endl;
    return;
  }

  //если всё в порядке, проверяем не сидит ли клиент на данный момент за другим столом
  //если да, то находим время, которое он был за другим столом и увеличиваем выручку и время работы этого стола
  if (tableClients.find(ev.idCl) != tableClients.end())
  {
    TableProcessing(tables, price, ev.t, tableClients[ev.idCl].second, tableClients[ev.idCl].first);

    //освобождаем предыдущий стол
    freeTable.insert(tableClients[ev.idCl].first);
  }

  //занимаем новый стол, фиксируем время, в которое клиент за него сел
  tableClients[ev.idCl] = make_pair(ev.numTable, ev.t);
  freeTable.erase(ev.numTable);

  return;
}

//функция обработки события 3 (клиент ожидает)
void ClientWait(ofstream& newFile, Event &ev, set<string> &clients, size_t &k, queue<string> &waiting, int &n)
{
  //делаем запись входящего события
  newFile << ev.time << " " << 3 << " " << ev.idCl << endl;

  //проверяем, находится ли клиент в клубе
  if (clients.find(ev.idCl) == clients.end())
  {
    newFile << ev.time << " " << 13 << " ClientUnknown" << endl;
    return;
  }

  //проверяем, нет ли свободных столов
  if (k > 0)
  {
    newFile << ev.time << " " << 13 << " ICanWaitNoLonger!" << endl;
    return;
  }

  //проверяем, что очередь не больше общего количества столов
  if (waiting.size() >= n)
  {
    newFile << ev.time << " " << 11 << " " << ev.idCl << endl;
    clients.erase(ev.idCl);
    return;
  }

  //если всё в порядке, добавляем клиента в очередь ожидания
  waiting.push(ev.idCl);
  return;
}

//функция обработки события 4 (клиент ушёл)
void ClientExit(ofstream& newFile, Event &ev, set<string> &clients, unordered_map<string, pair<int, int>> &tableClients,
                int &price, vector<pair<long long, int>> &tables, queue<string> &waiting, unordered_set<int> &freeTable)
{
  //делаем запись входящего события
  newFile << ev.time << " " << 4 << " " << ev.idCl << endl;

  //проверяем, был ли клиент в клубе
  if (clients.find(ev.idCl) == clients.end())
  {
    newFile << ev.time << " " << 13 << " ClientUnknown" << endl;
    return;
  }

  //если всё в порядке, проверяем сидел ли он за столом
  //если да, то рассчитываем время, которое клиент провёл за ним, увеличиваем выручку и время работы этого стола
  if (tableClients.find(ev.idCl) != tableClients.end())
  {
    TableProcessing(tables, price, ev.t, tableClients[ev.idCl].second, tableClients[ev.idCl].first);

    //проверяем есть ли в очереди ожидающие клиенты
    //если да, то сажаем их за освободившийся стол
    //если нет, то добавляем этот стол в свободные
    if (!waiting.empty())
    {
      tableClients[waiting.front()] = make_pair(tableClients[ev.idCl].first, ev.t);
      newFile << ev.time << " " << 12 << " " << waiting.front() << " " << tableClients[ev.idCl].first << endl;
      waiting.pop();
    }
    else
    {
      freeTable.insert(tableClients[ev.idCl].first);
    }
    tableClients.erase(ev.idCl);
  }

  clients.erase(ev.idCl);
  return;
}
