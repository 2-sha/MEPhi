#define IN_RAM

#include <vector>
#include <string>
#include <exception>
#include <algorithm>
#include <utility>
#include <functional>
#include <fstream>
#include "gui.hpp"
#include "hop_hash.hpp"
#include "sum_hash.hpp"
using namespace std;

GUI gui;

void openFromFile();
void saveToFile();
void printTable();
void fillTable();
void addItem();
void findItem();
void removeItem();

bool isNumber(const std::string& s)
{
	return !s.empty() && (s.find_first_not_of("0123456789") == s.npos);
}

vector<pair<string, function<void()>>> actions = {
	{ "Автоматически заполнить таблицу", fillTable },
	{ "Вывести таблицу", printTable },
	{ "Добавить элемент", addItem },
	{ "Найти элемент", findItem },
	{ "Удалить элемент", removeItem },
	{ "Выйти", []() { exit(0); }},
};

int main()
{
	setlocale(LC_ALL, "");
	gui.setWindowTitle("Хэш-таблица");

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
			catch (std::bad_alloc ex)
			{
				gui.showPopup("Ошибка!", "Повреждённый файл");
			}
			catch (std::exception ex)
			{
				gui.showPopup("Ошибка!", ex.what());
			}
		}
	}

	return 0;
}

void printTable()
{
	gui.showCustomWindow([](WINDOW *win) 
	{

		mvwaddstr(win, 1, 2, "+--------+--------+-----+--------------------------+");
		mvwaddstr(win, 2, 2, "| hopKey | sumKey | ver |          value           |");
		mvwaddstr(win, 3, 2, "+--------+--------+-----+--------------------------+");

		size_t offsetTop = 0;
		for (size_t i = 0; i < SIZE; i++)
		{
			if (hop_hash::table[i].item == nullptr)
				continue;

			Item *item = hop_hash::table[i].item;
			mvwaddstr(win, 4 + offsetTop, 2, "|        |        |     |                          |");
			mvwaddstr(win, 4 + offsetTop, 4, to_string(item->hopKey).c_str());
			mvwaddstr(win, 4 + offsetTop, 13, to_string(item->sumKey).c_str());

			bool isFirstTime = true;
			for (auto it : item->info)
			{
				if (!isFirstTime)
					mvwaddstr(win, 4 + offsetTop, 2, "|        |        |     |                          |");
				mvwaddstr(win, 4 + offsetTop, 22, to_string(it.ver).c_str());
				mvwaddstr(win, 4 + offsetTop, 28, it.info.c_str());
				offsetTop++;
				isFirstTime = false;
			}
			mvwaddstr(win, 4 + offsetTop, 2, "+--------+--------+-----+--------------------------+");
			offsetTop++;
		}

		mvwaddstr(win, LINES - 3, (COLS - 35) / 2, "Нажмите Enter, чтобы закрыть");
		wrefresh(win);
		getch();

	});
}

void fillTable()
{
	Item *item = new Item(1, 5, "A");
	hop_hash::add(1, item);
	sum_hash::add(5, item);
	item->add("B");
	item->add("C");

	item = new Item(2, 3, "D");
	hop_hash::add(2, item);
	sum_hash::add(3, item);

	item = new Item(5, 13, "LLL");
	hop_hash::add(5, item);
	sum_hash::add(13, item);

	for (size_t i = 0; i < actions.size(); i++)
	{
		if (actions[i].first == "Автоматически заполнить таблицу")
		{
			actions.erase(actions.begin() + i);
			break;
		}
	}
	gui.showPopup("Успешно!", "Таблица заполнена");
}

void addItem()
{
	std::vector<std::pair<std::string, bool>> input = gui.printInputMenu({
		make_pair("Назад", 0),
		make_pair("Ключ перемешивания hopscotch", 1),
		make_pair("Ключ перемешивания сложением", 1),
		make_pair("Значение", 1),
		make_pair("Добавить", 0),
	});

	if (input[0].second)
		return;
	if (input[1].first.empty() || input[2].first.empty() || input[3].first.empty())
		throw invalid_argument("Поля не заполнены");
	if (!isNumber(input[1].first) || !isNumber(input[2].first))
		throw invalid_argument("Ключ должен быть положительным числом");

	Item *hopTtem = hop_hash::find(stoi(input[1].first));
	Item *sumTtem = sum_hash::find(stoi(input[2].first));
	if (hopTtem != sumTtem)
		throw invalid_argument("Один из ключей уже использован");
	if (hopTtem == nullptr)
	{
		hopTtem = new Item(
			stoi(input[1].first), 
			stoi(input[2].first), 
			input[3].first
		);

		hop_hash::add(stoi(input[1].first), hopTtem);
		sum_hash::add(stoi(input[2].first), hopTtem);
	}
	else
	{
		hopTtem->add(input[3].first);
	}

	gui.showPopup("Успешно!", "Элемент добавлен");
}

void findItem()
{
	size_t type = gui.printMenu({
		"Назад",
		"Перемешивание hopscotch",
		"Перемешивание сложением",
	});

	if (type == 0)
		return;

	std::vector<std::pair<std::string, bool>> input = gui.printInputMenu({
		make_pair("Назад", 0),
		make_pair("Ключ", 1),
		make_pair("Версия (можно не заполнять)", 1),
		make_pair("Найти", 0),
	});

	if (input[0].second)
	{
		findItem();
		return;
	}
	if (input[1].first.empty())
		throw invalid_argument("Ключ не указан");
	if (!isNumber(input[1].first) || (!input[2].first.empty() && !isNumber(input[2].first)))
		throw invalid_argument("Ключ должен быть положительным числом");

	Item *res = nullptr;
	if (type == 1)
		res = hop_hash::find(stoi(input[1].first));
	else if (type == 2)
		res = sum_hash::find(stoi(input[1].first));
	if (res == nullptr)
		throw std::out_of_range("Указанный элемент не найден");
	if (input[2].first.empty())
	{
		vector<string> output = {
			"Ключ hopscotch: " + to_string(res->hopKey),
			"Ключ сложением: " + to_string(res->sumKey),
		};
		for (auto it : res->info)
			output.push_back("Версия " + to_string(it.ver) + ": " + it.info);
		gui.showPopup("Элемент найден!", output);
	}
	else
	{
		for (auto it : res->info)
		{
			if (it.ver == stoi(input[2].first))
			{
				gui.showPopup("Элемент найден!", "Значение: " + it.info);
				return;
			}
		}
		throw std::out_of_range("Указанный элемент не найден");
	}
}

void removeItem()
{
	size_t type = gui.printMenu({
		"Назад",
		"Перемешивание hopscotch",
		"Перемешивание сложением",
	});

	if (type == 0)
		return;

	std::vector<std::pair<std::string, bool>> input = gui.printInputMenu({
		make_pair("Назад", 0),
		make_pair("Ключ", 1),
		make_pair("Версия (можно не заполнять)", 1),
		make_pair("Удалить", 0),
		});

	if (input[0].second)
	{
		findItem();
		return;
	}
	if (input[1].first.empty())
		throw invalid_argument("Ключ не указан");
	if (!isNumber(input[1].first) || (!input[2].first.empty() && !isNumber(input[2].first)))
		throw invalid_argument("Ключ должен быть положительным числом");

	Item *res = nullptr;
	if (type == 1)
		res = hop_hash::find(stoi(input[1].first));
	else if (type == 2)
		res = sum_hash::find(stoi(input[1].first));
	if (res == nullptr)
		throw std::out_of_range("Указанный элемент не найден");
	if (input[2].first.empty())
	{
		hop_hash::remove(res->hopKey);
		sum_hash::remove(res->sumKey);
		delete res;
		// При вызове delete item в sum_hash элемент удаляется сам
	}
	else
	{
		bool isRemoved = false;
		size_t ver = stoi(input[2].first);
		res->info.remove_if([&ver, &isRemoved](Version elem) {
			if (elem.ver == ver)
			{
				isRemoved = true;
				return true;
			}
			return false;
		});
		if (!isRemoved)
			throw std::out_of_range("Указанная версия не найдена");
	}

	gui.showPopup("Успешно!", "Элемент удалён");
}
