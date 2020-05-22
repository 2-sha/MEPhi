#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <queue>
#include <fstream>
#include <Windows.h>
#include <math.h>
#include <ctime>
#include "chart.hpp"
#include "gui.hpp";
#include "treap.hpp";
using namespace std;

GUI gui;
Treap<string> tree;

void printTree();
void fillTree();
void addToTree();
void removeFromTree();
void findInTree();
void saveTree();
void loadTree();
void treeTiming();

void calcTiming(function<void(ofstream&)>);
void printOrderedTree(const vector<Treap<string>::Node>& nodes);
void drawTree();
bool isNumber(const std::string& s);

vector<pair<string, function<void()>>> actions = {
    { "Загрузить из файла", loadTree },
    { "Вывести дерево", printTree },
    { "Добавить элемент", addToTree },
    { "Найти элемент", findInTree },
    { "Удалить элемент", removeFromTree },
    { "Таймирование", treeTiming },
    { "Сохранить", saveTree },
    { "Выйти", []() { exit(0); }},
};

int main()
{
    setlocale(LC_ALL, "");
    gui.setWindowTitle("Декартово дерево");

    bool isRunning = true;
    bool flag = false;
    while (isRunning)
    {
        if (flag && tree.getSize() != 0)
        {
            for (int i = 0; i < actions.size(); i++)
            {
                bool isFillTree = (actions[i].first == "Автоматически заполнить дерево");
                bool loadTree = (actions[i].first == "Загрузить из файла");
                if (isFillTree || loadTree)
                {
                    actions.erase(actions.begin() + i);
                    flag = false;
                }
            }
        }
        else if (!flag)
        {
            actions.insert(actions.begin(), { "Автоматически заполнить дерево", fillTree });
            flag = true;
        }

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

bool isNumber(const std::string& s)
{
    return !s.empty() && (s.find_first_not_of("0123456789") == s.npos);
}

void printTree()
{
    if (tree.getSize() == 0)
        throw invalid_argument("Дерево пустое");

    int type = gui.printMenu({
        "Назад",
        "Вывод ключей по порядку",
        "Прямой обход дерева",
        "Сгенерировать картинку",
        });
    if (type == 0)
        return;

    if (type == 3)
    {
        drawTree();
        return;
    }

    vector<Treap<string>::Node> nodes;
    if (type == 1)
        tree.map([&nodes](Treap<string>::Node* node) { nodes.push_back(*node); });
    else
        tree.nlr([&nodes](Treap<string>::Node* node) { nodes.push_back(*node); });

    printOrderedTree(nodes);
}
// писка

void printOrderedTree(const vector<Treap<string>::Node>& nodes)
{
    gui.showCustomWindow([&nodes](WINDOW* win)
        {
            mvwaddstr(win, 1, 2, "+-----+-----------+------------------------+");
            mvwaddstr(win, 2, 2, "|  x  |     y     |          data          |");
            mvwaddstr(win, 3, 2, "+-----+-----------+------------------------+");

            int offsetTop = 0, indexPos = 0;
            for (auto node : nodes)
            {
                mvwaddstr(win, 4 + offsetTop, 2, "|     |           |                        |");
                mvwaddstr(win, 4 + offsetTop, 6, to_string(node.x).c_str());
                mvwaddstr(win, 4 + offsetTop, 10, to_string(node.y).c_str());
                mvwaddstr(win, 4 + offsetTop, 22, node.data.c_str());
                mvwaddstr(win, 4 + offsetTop + 1, 2, "+-----+-----------+------------------------+");
                offsetTop += 2;
            }

            mvwaddstr(win, LINES - 3, (COLS - 35) / 2, "Нажмите Enter, чтобы закрыть");
            wrefresh(win);
            getch();
        });
}

void drawTree()
{
    tree.generateImage("tree.png", "png");

    gui.showPopup("Успешно!", "Файл сгенерирован");

    ShellExecute(0, "open", "tree.png", NULL, NULL, SW_SHOW);
}

void fillTree()
{
    std::vector<std::pair<std::string, bool>> input = gui.printInputMenu({
        make_pair("Назад", 0),
        make_pair("Кол-во элементов", 1),
        make_pair("Сгенерировать", 0),
        });

    if (input[0].second)
        return;
    if (input[1].first.empty())
        throw invalid_argument("Поля не заполнены");
    if (!isNumber(input[1].first))
        throw invalid_argument("Ожидается число");

    vector<pair<long, string>> data;
    srand(time(0));
    for (int i = 0; i < stoi(input[1].first); i++)
    {
        string str = "---";
        str[0] = 65 + rand() % 25;
        str[1] = 65 + rand() % 25;
        str[2] = 65 + rand() % 25;
        data.push_back({ i, str });
    }
    tree.buildTree(data);

    gui.showPopup("Успешно!", "Дерево заполнено");
}

void addToTree()
{
    std::vector<std::pair<std::string, bool>> input = gui.printInputMenu({
        make_pair("Назад", 0),
        make_pair("Ключ", 1),
        make_pair("Значение", 1),
        make_pair("Добавить", 0),
        });

    if (input[0].second)
        return;
    if (input[1].first.empty() || input[2].first.empty())
        throw invalid_argument("Поля не заполнены");
    if (!isNumber(input[1].first))
        throw invalid_argument("Ключ должен быть числом");
    if (tree.find(stoi(input[1].first)) != nullptr)
        throw invalid_argument("Ключ уже существует");

    tree.add(stoi(input[1].first), input[2].first);

    gui.showPopup("Успешно!", "Значение добавлено");
}

void removeFromTree()
{
    std::vector<std::pair<std::string, bool>> input = gui.printInputMenu({
        make_pair("Назад", 0),
        make_pair("Ключ", 1),
        make_pair("Удалить", 0),
        });

    if (input[0].second)
        return;
    if (input[1].first.empty())
        throw invalid_argument("Поля не заполнены");
    if (!isNumber(input[1].first))
        throw invalid_argument("Ключ должен быть числом");

    // Если элемент не найден, то кинет иисключение
    tree.find(stoi(input[1].first));
    tree.remove(stoi(input[1].first));
    gui.showPopup("Успешно!", "Значение удалено");
}

void findInTree()
{
    std::vector<std::pair<std::string, bool>> input = gui.printInputMenu({
        make_pair("Назад", 0),
        make_pair("Ключ", 1),
        make_pair("Найти", 0),
        });

    if (input[0].second)
        return;
    if (input[1].first.empty())
        throw invalid_argument("Поля не заполнены");
    if (!isNumber(input[1].first))
        throw invalid_argument("Ключ должен быть числом");

    Treap<string>::Node* node = tree.find(stoi(input[1].first));
    if (node == nullptr)
        throw invalid_argument("Ключ не найден");
    vector<string> output = {
        "Значение найдено",
        "x: " + to_string(node->x),
        "y: " + to_string(node->y),
        "data: " + node->data,
    };
    gui.showPopup("Успешно!", output);
}

void saveTree()
{
    if (tree.getSize() == 0)
        throw invalid_argument("Дерево пустое");

    std::vector<std::pair<std::string, bool>> input = gui.printInputMenu({
        make_pair("Назад", 0),
        make_pair("Файл", 1),
        make_pair("Сохранить", 0),
        });

    if (input[0].second)
        return;
    if (input[1].first.empty())
        throw invalid_argument("Поля не заполнены");

    ofstream file(input[1].first);
    if (!file.is_open())
        throw invalid_argument("Невозможно создать файл");
    tree.map([&file](Treap<string>::Node* node)
        {
            file << node->x << endl;
            file << node->data << endl;
        });
    file.close();

    gui.showPopup("Успешно!", "Дерево сохранено");
}

void loadTree()
{
    std::vector<std::pair<std::string, bool>> input = gui.printInputMenu({
        make_pair("Назад", 0),
        make_pair("Файл", 1),
        make_pair("Загрузить", 0),
        });

    if (input[0].second)
        return;
    if (input[1].first.empty())
        throw invalid_argument("Поля не заполнены");

    ifstream file(input[1].first);
    if (!file.is_open())
        throw invalid_argument("Невозможно открыть файл");
    string str;
    bool isIndex = true;
    vector<pair<long, string>> data;
    auto index = data.begin();
    while (getline(file, str))
    {
        if (isIndex)
        {
            if (!isNumber(str))
                continue;
            data.push_back(make_pair(stoi(str), ""));
            index = data.end() - 1;
        }
        else
        {
            (*index).second = str;
        }
        isIndex = !isIndex;
    }
    file.close();

    tree.buildTree(data);
    gui.showPopup("Успешно!", "Дерево загружено");
}

void treeTiming()
{
    std::vector<std::pair<std::string, bool>> input = gui.printInputMenu({
        make_pair("Назад", 0),
        make_pair("Кол-во элементов", 1),
        make_pair("Кол-во измерений", 1),
        make_pair("Имя csv файла", 1),
        make_pair("Начать", 0),
    });

    if (input[1].second)
        return;
    if (input[1].first.empty() || input[2].first.empty() || input[3].first.empty())
        throw invalid_argument("Поля не заполнены");
    if (!isNumber(input[1].first) || !isNumber(input[2].first))
        throw invalid_argument("Ожидалось число");

    int num = stoi(input[1].first);
    int step = 1;
    if (num > 10)
        step = num / stoi(input[2].first);
    ofstream file(input[3].first);
    if (!file.is_open())
        throw invalid_argument("Не удаётся открыть файл");

    file << ";0;";
    for (int i = step; i <= num; i += step)
        file << to_string(i) + ';';
    file << endl;

    gui.showCustomWindow([&file, &step, &num](WINDOW* win)
        {
            Treap<int> tree;

            int loaderWidth = (num / step) * 2 + 2;
            int j = 3;
            string loaderLine = "[";
            for (int i = 0; i < loaderWidth; i++)
                loaderLine += "-";
            loaderLine += "]";

            // Добавление и высота
            mvwaddstr(win, 1, 2, "Добавление и высота");
            mvwaddstr(win, 2, 2, loaderLine.c_str());

            string addBuf = "add;";
            string depthBuf = "depth;";

            int addTime = 0;
            unsigned int start;
            for (int i = 0; i <= num; i++)
            {
                start = clock();
                tree.add(i, 0);
                addTime += clock() - start;

                if (i % step == 0)
                {
                    addBuf += to_string(addTime) + ";";

                    int maxDepth = 0;
                    queue<pair<Treap<int>::Node*, int>> nodes;
                    nodes.push({ tree.getRoot(), 0 });
                    while (!nodes.empty())
                    {
                        Treap<int>::Node* p = nodes.front().first;
                        int depth = nodes.front().second;
                        if (p->left != nullptr)
                            nodes.push({ p->left, depth + 1 });
                        if (p->right != nullptr)
                            nodes.push({ p->right, depth + 1 });
                        maxDepth = max(depth, maxDepth);
                        nodes.pop();
                    }
                    depthBuf += to_string(maxDepth) + ";";

                    mvwaddstr(win, 2, j, "##");
                    wrefresh(win);
                    j += 2;
                }
            }
            file << addBuf << endl;
            file << depthBuf << endl;
            mvwaddstr(win, 3, 2, string("Время: " + to_string(addTime) + "мс").c_str());

            mvwaddstr(win, LINES - 3, (COLS - 35) / 2, "Нажмите Enter, чтобы закрыть");
            wrefresh(win);
            getch();
        });

    file.close();
}