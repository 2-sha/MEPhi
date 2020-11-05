#include <iostream>
#include <sstream>
#include <string>
#include "gui.h"
#include "Card.h"
#include "Deck.h"
using namespace laba3;
using namespace std;
using namespace cursesgui;

/*
 - Инициализация
	+ Задать значения каждой карты
	+ Указать кол-во карт
 - Вывод
	+ Всю колоду
	+ Вывести информацию о карте по номеру
	+ Показать карты заданной масти
 - Добавить
	+ Задать значения карты
	+ Сгенерировать
 - Отсортировать
 - Выход
*/

inline bool isInteger(const std::string&);

void init(GUI&);
void show(GUI&);
void input(GUI&);
void sort(GUI&);
void quit(GUI&);

Deck deck;

int main()
{
	GUI gui("Колода карт");

	gui.addActions({
		{ 1, "Инициализация", init },
		{ 2, "Вывод", show },
		{ 3, "Добавление", input },
		{ 4, "Сортировка", sort },
		{ 5, "Выход", quit },
		});

	return gui.run();
}

inline bool isInteger(const std::string& s)
{
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

	char* p;
	strtol(s.c_str(), &p, 10);

	return (*p == 0);
}

void init(GUI& gui)
{
	auto input = gui.getInputs({
		{ 1, "Размер", true },
		{ 2, "Указать значения", false },
		{ 3, "Сгенерировать колоду", false },
		{ 4, "Назад", false },
		}, [](GUI::InputData data) {
			if (data[4].isChosen)
				return std::make_pair(true, "");
			if (!isInteger(data[1].value) || stoi(data[1].value) < 1 || stoi(data[1].value) > deck.get_MAX_LEN())
				return std::make_pair(false, "Некорректный размер");
			return std::make_pair(true, "");
		});

	if (input[4].isChosen)
		return;
	int size = stoi(input[1].value);;

	if (input[2].isChosen)
	{
		Deck newDeck;
		for (int i = 0; i < size; i++)
		{
			auto input = gui.getInputs({
				{ 1, "Масть (S, H, C, D)", true },
				{ 2, "Ранг (2 .. 10, 11 - J, 12 - Q, 13 - K, 14 - A)", true },
				{ 3, "Дальше (" + to_string(i) + "/" + to_string(size) + ")", false },
				{ 4, "Отменить", false },
				}, [&newDeck](GUI::InputData data) {
					if (data[4].isChosen)
						return std::make_pair(true, "");
					char suit = data[1].value.c_str()[0];
					if (suit != 'S' && suit != 'H' && suit != 'C' && suit != 'D')
						return std::make_pair(false, "Некорректная масть");
					if (!isInteger(data[2].value) || stoi(data[2].value) < 2 || stoi(data[2].value) > 14)
						return std::make_pair(false, "Некорректный ранг");
					if (newDeck.find_card(Card(suit, stoi(data[2].value))) != -1)
						return std::make_pair(false, "Карточка уже существует");
					return std::make_pair(true, "");
				});
			if (input[4].isChosen)
			{
				init(gui);
				return;
			}
			newDeck.add_new_card(Card(input[1].value.c_str()[0], stoi(input[2].value)));
		}
		deck = newDeck;
	}
	else
	{
		deck = Deck(size);
	}
	gui.removeAction(1);
	if (deck.get_len() == deck.get_MAX_LEN())
		gui.removeAction(3);
	gui.showPopup("Успешно!", "Колода инициализирована");
}

void show(GUI& gui)
{
	int choice = gui.getChoice({
		{ 1, "Вся колода" },
		{ 2, "Карта по номеру" },
		{ 3, "Карты указанной масти" },
		{ 4, "Назад" },
		});

	if (choice == 1)
	{
		Table table({ "ID", "SUIT", "RANG" });
		for (int i = 0; i < deck.get_len(); i++)
			table.addRow({ to_string(i),  string(1, deck.get_card(i).get_suit_ch()), deck.get_card(i).get_rang_str() });
		gui.printTable(table);
	}
	else if (choice == 2)
	{
		auto input = gui.getInputs({
			{ 1, "Индекс", true },
			{ 2, "Найти", false },
			{ 3, "Назад", false },
			}, [](GUI::InputData data) {
				if (data[3].isChosen)
					return std::make_pair(true, "");
				if (!isInteger(data[1].value) || stoi(data[1].value) < 0 || stoi(data[1].value) > deck.get_len())
					return std::make_pair(false, "Некорректный индекс");
				return std::make_pair(true, "");
			});
		if (input[3].isChosen)
		{
			show(gui);
			return;
		}

		int index = stoi(input[1].value);
		gui.showPopup("Карта №" + to_string(index), {
			"Масть: " + string(1, deck.get_card(index).get_suit_ch()),
			"Ранг: " + deck.get_card(index).get_rang_str(),
			});
	}
	else if (choice == 3)
	{
		auto input = gui.getInputs({
			{ 1, "Масть (S, H, C, D)", true },
			{ 2, "Найти", false },
			{ 3, "Назад", false },
			}, [](GUI::InputData data) {
				if (data[3].isChosen)
					return std::make_pair(true, "");
				char suit = data[1].value.c_str()[0];
				if (suit != 'S' && suit != 'H' && suit != 'C' && suit != 'D')
					return std::make_pair(false, "Некорректная масть");
				return std::make_pair(true, "");
			});
		if (input[3].isChosen)
		{
			show(gui);
			return;
		}

		char suit = input[1].value.c_str()[0];
		Deck newDeck;
		for (int i = 0; i < deck.get_len(); i++)
		{
			Card card = deck.get_card(i);
			if (card.get_suit_ch() == suit)
				newDeck.add_new_card(card);
		}
		Table table({ "ID", "SUIT", "RANG" });
		for (int i = 0; i < newDeck.get_len(); i++)
			table.addRow({ to_string(i),  string(1, newDeck.get_card(i).get_suit_ch()), newDeck.get_card(i).get_rang_str() });
		gui.printTable(table);
	}
}

void input(GUI& gui)
{
	int choice = gui.getChoice({
		{ 1, "Указать" },
		{ 2, "Сгенерировать" },
		{ 3, "Назад" },
		});
	if (choice == 3)
		return;

	Card card;
	if (choice == 1)
	{
		auto data = gui.getInputs({
				{ 1, "Масть (S, H, C, D)", true },
				{ 2, "Ранг (2 .. 10, 11 - J, 12 - Q, 13 - K, 14 - A)", true },
				{ 3, "Отменить", false },
			}, [](GUI::InputData data) {
				if (data[3].isChosen)
					return std::make_pair(true, "");
				char suit = data[1].value.c_str()[0];
				if (suit != 'S' && suit != 'H' && suit != 'C' && suit != 'D')
					return std::make_pair(false, "Некорректная масть");
				if (!isInteger(data[2].value) || stoi(data[2].value) < 2 || stoi(data[2].value) > 14)
					return std::make_pair(false, "Некорректный ранг");
				if (deck.find_card(Card(suit, stoi(data[2].value))) != -1)
					return std::make_pair(false, "Карточка уже существует");
				return std::make_pair(true, "");
			});
		if (data[3].isChosen)
		{
			input(gui);
			return;
		}

		card = Card(data[1].value.c_str()[0], stoi(data[2].value));
	}
	else if (choice == 2)
	{
		card = Card(1);
	}

	deck.add_new_card(card);
	gui.showPopup("Новая карта", {
		"Масть: " + string(1, card.get_suit_ch()),
		"Ранг: " + card.get_rang_str(),
		});
}

void sort(GUI& gui)
{
	deck.sort();
	gui.showPopup("Успешно!", "Колода отсортирована");
}

void quit(GUI& gui)
{
	gui.quit();
}