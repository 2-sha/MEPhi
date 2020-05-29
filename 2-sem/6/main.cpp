#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <map>
#include <regex>
#include <ctime>
#include <string>
#include <math.h>
#include <Windows.h>
#include "gui.hpp"
#include "graph.hpp"
using namespace std;

GUI gui;
Graph graph;

void load();
void generate();
void save();
void print();
void addElem();
void findWay();
void removeElem();
void calcTiming();

bool isNumber(const string& s);
bool hasSpaces(const string& s);
vector<string> split(const string& s, const string& regex);
void addNode();
void addEdge();

vector<pair<string, function<void()>>> actions = {
    { "Загрузить из файла", load },
    { "Заполинть", generate },
    { "Вывести граф", print },
    { "Добавить", addElem },
    { "Найти путь", findWay },
    { "Удалить вершину", removeElem },
    //{ "Таймирование", calcTiming },
    { "Сохранить", save },
    { "Выйти", []() { exit(0); }},
};

int main()
{
    setlocale(LC_ALL, "");
    gui.setWindowTitle("Взвешенный граф");

    bool isRunning = true;
    while (isRunning)
    {

        vector<string> actionsNames;
        for (auto it : actions)
            actionsNames.push_back(it.first);
        size_t choise = gui.printMenu(actionsNames);

        if (choise == actions.size())
        {
            isRunning = false;
        }
        else
        {
            try
            {
                actions[choise].second();
            }
            catch (std::exception ex)
            {
                gui.showPopup("Ошибка!", ex.what());
            }
        }
    }

    return 0;
}

void load()
{
    std::vector<std::pair<std::string, bool>> input = gui.printInputMenu({
        { "Назад", 0 },
        { "Имя файла", 1 },
        { "Загрузить", 0 },
    });

    if (input[0].second)
        return;
    if (input[1].first.empty())
        throw invalid_argument("Поля не заполнены");

    fstream file(input[1].first, ios::binary | ios::in);
    if (!file.is_open())
        throw invalid_argument("Не удаётся открыть файл");
    graph.read(file);
    file.close();

    gui.showPopup("Успешно!", "Граф загружен");
}

void save()
{
    std::vector<std::pair<std::string, bool>> input = gui.printInputMenu({
        { "Назад", 0 },
        { "Имя файла", 1 },
        { "Сохранить", 0 },
    });

    if (input[0].second)
        return;
    if (input[1].first.empty())
        throw invalid_argument("Поля не заполнены");

    fstream file(input[1].first, ios::binary | ios::out);
    if (!file.is_open())
        throw invalid_argument("Не удаётся открыть файл");
    graph.save(file);
    file.close();

    gui.showPopup("Успешно!", "Граф сохранён");
}

void generate()
{
    graph.clear();

    graph.addNode(5, 5, "A");
    graph.addNode(6, 4, "C");
    graph.addNode(7, 5, "B");
    graph.addNode(7, 6, "E");
    graph.addNode(8, 3, "F");
    graph.addNode(5, 10, "D");

    graph.addEdgees("A", { "C", "D" });
    graph.addEdgees("D", { "C", "E" });
    graph.addEdgees("C", { "B", "E" });
    graph.addEdgees("B", { "E" });
    graph.addEdgees("E", { "F" });

    gui.showPopup("Успешно!", "Граф сгенерирован");

    //std::vector<std::pair<std::string, bool>> input = gui.printInputMenu({
    //    { "Назад", 0 },
    //    { "Кол-во вершин", 1 },
    //    { "Сгенерировать", 0 },
    //});

    //if (input[0].second)
    //    return;
    //if (input[1].first.empty())
    //    throw invalid_argument("Поля не заполнены");
    //if (!isNumber(input[1].first))
    //    throw invalid_argument("Ожидается число");
    //int size = stoi(input[1].first);

    //srand(time(0));
    //vector<string> names;
    //for (int i = 0; i < size; i++)
    //{
    //    string str = "---";
    //    str[0] = 65 + i;
    //    str[1] = 65 + i / 25;
    //    str[2] = 65 + i / 625;
    //    names.push_back(str);
    //    graph.addNode(rand() % 100, rand() % 100, str);
    //}

    //for (int i = 0; i < rand() % size; i++)
    //{
    //    vector<bool> isRepeat(size, false);
    //    int start = rand() % size;
    //    isRepeat[start] = true;
    //    vector<string> nodes;
    //    for (int j = 0; j < rand() % size; j++)
    //    {
    //        int id = rand() % size;
    //        while (isRepeat[id])
    //            id = rand() % size;
    //        nodes.push_back(names[id]);
    //    }
    //    graph.addEdgees(names[start], nodes);
    //}

    //gui.showPopup("Успешно!", "Граф сгенерирован");
}

void print()
{
    if (graph.isEmpty())
        throw invalid_argument("Граф пуст");

    int type = gui.printMenu({
        "Назад",
        "Вывод матрицы смежности",
        "Сгенерировать картинку",
    });

    switch (type)
    {
    case 0:
        return;
    case 1:
        gui.showCustomWindow([](WINDOW* win)
            {
                std::map<std::string, int> names = graph.getNames();
                std::vector<std::vector<double>> matrix = graph.getMatrix();
                int size = names.size();

                string delimetr = "+-----";
                string line = "|     ";
                for (int i = 0; i < size; i++)
                {
                    delimetr += "+-----";
                    line += "|     ";
                }
                delimetr += "+";
                line += "|";

                int i = 0;
                mvwaddstr(win, 1, 2, delimetr.c_str());
                mvwaddstr(win, 2, 2, line.c_str());
                for (auto name : names)
                {
                    mvwaddstr(win, 2, 10 + i * 6, name.first.c_str());
                    i++;
                }
                mvwaddstr(win, 3, 2, delimetr.c_str());

                i = 0;
                for (auto tableLine : names)
                {
                    mvwaddstr(win, 4 + i, 2, line.c_str());
                    mvwaddstr(win, 4 + i, 4, tableLine.first.c_str());
                    int j = 0;
                    for (auto tableRow : names)
                    {
                        double val = matrix[tableLine.second][tableRow.second];
                        if (val == DBL_MAX)
                            mvwaddstr(win, 4 + i, 10 + j * 6, "-");
                        else
                            mvwaddstr(win, 4 + i, 10 + j * 6, to_string((int)round(val)).c_str());
                        j++;
                    }
                    mvwaddstr(win, 5 + i, 2, delimetr.c_str());
                    i += 2;
                }

                mvwaddstr(win, LINES - 3, (COLS - 35) / 2, "Нажмите Enter, чтобы закрыть");
                wrefresh(win);
                getch();
            });
        break;
    case 2:
        graph.draw("graph.png", "png");
        gui.showPopup("Успешно!", "Файл сгенерирован");
        ShellExecute(0, "open", "graph.png", NULL, NULL, SW_SHOW);
        break;
    }
}

void addElem()
{
    int type = gui.printMenu({
        "Назад",
        "Добавить вершину",
        "Добавить ребро",
    });

    switch(type)
    {
    case 0:
        return;
    case 1:
        addNode();
        break;
    case 2:
        addEdge();
        break;
    }
}

void addNode()
{
    std::vector<std::pair<std::string, bool>> input = gui.printInputMenu({
        make_pair("Назад", 0),
        make_pair("Имя", 1),
        make_pair("x", 1),
        make_pair("y", 1),
        make_pair("Добавить", 0),
    });

    if (input[0].second)
    {
        addElem();
        return;
    }
    if (input[1].first.empty() || input[2].first.empty() || input[3].first.empty())
        throw invalid_argument("Поля не заполнены");
    if (hasSpaces(input[1].first))
        throw invalid_argument("Имя не должно содержать пробелы");
    if (!isNumber(input[2].first) || !isNumber(input[3].first))
        throw invalid_argument("Координаты должны быть числами");

    graph.addNode(stoi(input[2].first), stoi(input[3].first), input[1].first);
    gui.showPopup("Успешно!", "Вершина добавлена");
}

void addEdge()
{
    std::vector<std::pair<std::string, bool>> input = gui.printInputMenu({
        make_pair("Назад", 0),
        make_pair("Вершина", 1),
        make_pair("Смежные вершины через пробел", 1),
        make_pair("Добавить", 0),
    });

    if (input[0].second)
    {
        addElem();
        return;
    }
    if (input[1].first.empty() || input[2].first.empty())
        throw invalid_argument("Поля не заполнены");
    if (hasSpaces(input[1].first))
        throw invalid_argument("Имя не должно содержать пробелы");
    vector<string> nodes = split(input[2].first, " ");

    graph.addEdgees(input[1].first, nodes);
    gui.showPopup("Успешно!", "Рёбра добавлены");
}

void findWay()
{
    std::vector<std::pair<std::string, bool>> input = gui.printInputMenu({
        make_pair("Назад", 0),
        make_pair("Из", 1),
        make_pair("В", 1),
        make_pair("Найти", 0),
    });

    if (input[0].second)
        return;
    if (input[1].first.empty() || input[2].first.empty())
        throw invalid_argument("Поля не заполнены");
    if (hasSpaces(input[1].first) || hasSpaces(input[2].first))
        throw invalid_argument("Названия не должны содержать пробелы");

    map<string, int> names = graph.getNames();
    if (names.find(input[1].first) == names.end() || names.find(input[2].first) == names.end())
        throw invalid_argument("Указанных точек не существует");

    int idA = names[input[1].first];
    int idB = names[input[2].first];
    vector<vector<pair<int, double>>> ways = graph.YenKSP(names[input[1].first], names[input[2].first], 3);

    vector<string> output;
    for (int i = 0; i < ways.size(); i++)
    {
        string str = to_string(i) + ": ";
        if (ways[i].back().second != DBL_MAX)
            str += to_string(ways[i].back().second);
        else
            str += "-";
        output.push_back(str);
    }

    gui.showPopup("Выполнено!", output);
}

void removeElem()
{
    std::vector<std::pair<std::string, bool>> input = gui.printInputMenu({
        make_pair("Назад", 0),
        make_pair("Имя", 1),
        make_pair("Удалить", 0),
    });

    if (input[0].second)
        return;
    if (input[1].first.empty())
        throw invalid_argument("Поля не заполнены");
    if (hasSpaces(input[1].first))
        throw invalid_argument("Имя не должно содержать пробелы");

    graph.removeNode(input[1].first);
    gui.showPopup("Успешно!", "Вершина удалена");
}

void calcTiming()
{
    std::vector<std::pair<std::string, bool>> input = gui.printInputMenu({
        make_pair("Назад", 0),
        make_pair("Кол-во вершин", 1),
        make_pair("Кол-во рёбер", 1),
        make_pair("Кол-во измерений", 1),
        make_pair("Имя csv файла", 1),
        make_pair("Начать", 0),
    });

    if (input[1].second)
        return;
    if (input[1].first.empty() || input[2].first.empty() || input[3].first.empty() || input[4].first.empty())
        throw invalid_argument("Поля не заполнены");
    if (!isNumber(input[1].first) || !isNumber(input[2].first) || !isNumber(input[3].first))
        throw invalid_argument("Ожидалось число");

    int numV = stoi(input[1].first);
    int numE = stoi(input[2].first);
    int stepV = 1, stepE = 1;
    int EperV = numE / numV;
    if (numV > 10)
        stepV = numV / stoi(input[3].first);
    if (numE > 10)
        stepE = numE / stoi(input[3].first);

    ofstream file(input[4].first);
    if (!file.is_open())
        throw invalid_argument("Не удаётся открыть файл");

    file << "V;0;";
    for (int i = stepV; i <= numV; i += stepV)
        file << to_string(i) + ';';
    file << endl;
    file << "E;0;";
    for (int i = stepE; i <= numE; i += stepE)
        file << to_string(i) + ';';
    file << endl;

    string addBuf = "add;";
    string findBuf = "find;";

    int addTime = 0;
    unsigned int start;
    gui.showCustomWindow([&](WINDOW* win)
        {
            Graph graph;

            int loaderWidth = stoi(input[3].first) * 2 + 2;
            int j = 3;
            string loaderLine = "[";
            for (int i = 0; i < loaderWidth; i++)
                loaderLine += "-";
            loaderLine += "]";

            mvwaddstr(win, 1, 2, "Поиск кратчайшего пути");
            mvwaddstr(win, 2, 2, loaderLine.c_str());

            srand(time(0));
            for (int i = 0; i <= numV; i++)
            {
                string str = "-----";
                str[0] = 65 + i;
                str[1] = 65 + i / 25;
                str[2] = 65 + i / 625;
                str[3] = 65 + i / 15625;
                str[4] = 65 + i / 390625;
                start = clock();
                graph.addNode(rand() % numV, rand() % numV, str);
                addTime += clock() - start;
                vector<string> nodes;
                for (int j = 0; j < stepE; j++)
                {
                    string str = "-----";
                    int val = rand() % numV;
                    str[0] = 65 + val;
                    str[1] = 65 + val / 25;
                    str[2] = 65 + val / 625;
                    str[3] = 65 + val / 15625;
                    str[4] = 65 + val / 390625;
                    nodes.push_back(str);
                }
                graph.addEdgees(str, nodes);

                if (i % stepV == 0)
                {
                    addBuf += to_string(addTime) + ";";

                    start = clock();
                    //graph.YenKSP();
                    findBuf += to_string(clock() - start) + ";";

                    mvwaddstr(win, 2, j, "##");
                    wrefresh(win);
                    j += 2;
                }
            }

            file << addBuf << endl;
            file << findBuf << endl;

            mvwaddstr(win, LINES - 3, (COLS - 35) / 2, "Нажмите Enter, чтобы закрыть");
            wrefresh(win);
            getch();
        });

    file.close();
}

bool isNumber(const string& s)
{
    return !s.empty() && (s.find_first_not_of("0123456789") == s.npos);
}

bool hasSpaces(const string& s)
{
    return !s.empty() && (s.find(" ") != s.npos);
}

std::vector<std::string> split(const string& s, const string& regex) {
    // passing -1 as the submatch index parameter performs splitting
    std::regex re(regex);
    std::sregex_token_iterator first{ s.begin(), s.end(), re, -1 }, last;
    return { first, last };
}