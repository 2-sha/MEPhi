#include "gui.h"
using namespace cursesgui;


void GUI::init()
{
	setlocale(LC_ALL, "");

	initscr();
	start_color();
	keypad(stdscr, true);
	noecho();
	curs_set(0);
	refresh();

	init_pair(1, COLOR_BLACK, COLOR_WHITE);

	header = newwin(1, COLS, 0, 0);
	wbkgd(header, COLOR_PAIR(1));
	attron(COLOR_PAIR(1));
	mvwaddstr(header, 0, 3, windowTitle.c_str());
	attroff(COLOR_PAIR(1));
	wrefresh(header);
}

GUI::GUI(const std::string&& windowTitle, const std::string&& headerStatus)
{
	setWindowTitle(windowTitle);
	updateStatus(headerStatus);
	init();
}

GUI::~GUI()
{
	delwin(header);
	endwin();
}

void GUI::quit(int status)
{
	returnStatus = 0;
	isRuning = false;
}

void GUI::addActions(const std::vector<ActionField>&& actions)
{
	for (auto action : actions)
		this->actions[action.id] = action;
}

void GUI::removeAction(const int&& id)
{
	actions.erase(id);
}

int GUI::run()
{
	if (isRuning)
		throw std::runtime_error("Already runing");

	isRuning = true;
	while (isRuning)
	{
		std::vector<choiceField> actionsList;
		for (const auto& item : actions)
			actionsList.push_back({ item.second.id, item.second.name });

		try
		{
			actions[getChoice(actionsList)].func(*this);
		}
		catch (std::exception& ex)
		{
			showPopup("Error", ex.what());
		}
	}

	return returnStatus;
}

void GUI::updateStatus(const std::string& status)
{
	headerStatus = status;
	wclear(header);

	attron(COLOR_PAIR(1));
	mvwaddstr(header, 0, COLS - headerStatus.size() - 3, headerStatus.c_str());
	mvwaddstr(header, 0, 3, windowTitle.c_str());
	attroff(COLOR_PAIR(1));
	wrefresh(header);
}

void GUI::setWindowTitle(const std::string& title)
{
	windowTitle = title;
	wclear(header);

	attron(COLOR_PAIR(1));
	mvwaddstr(header, 0, COLS - headerStatus.size() - 3, headerStatus.c_str());
	mvwaddstr(header, 0, 3, windowTitle.c_str());
	attroff(COLOR_PAIR(1));
	wrefresh(header);
}

int GUI::getChoice(const std::vector<choiceField>& options)
{
	WINDOW* menu = newwin(LINES - 1, COLS, 1, 0);
	int choice = 0;

	bool isChosen = false;
	while (!isChosen)
	{
		for (int i = 0; i < options.size(); i++)
		{
			if (choice == i)
			{
				wattron(menu, A_BOLD);
				mvwaddstr(menu, 1 + i, 1, "> ");
			}
			else
				mvwaddstr(menu, 1 + i, 1, "  ");
			wprintw(menu, options[i].name.c_str());
			wattroff(menu, A_BOLD);
		}
		wrefresh(menu);

		switch (getch())
		{
		case KEY_UP:
			if (choice > 0)
				choice--;
			else
				choice = options.size() - 1;
			break;
		case KEY_DOWN:
			if (choice < options.size() - 1)
				choice++;
			else
				choice = 0;
			break;
			// Enter
		case 10:
			isChosen = true;
			break;
		}
	}

	delwin(menu);
	return options[choice].id;
}

GUI::InputData GUI::getInputs(const std::vector<InputChoiceField>& options,
							  std::function<std::pair<bool, std::string>(InputData)> validator)
{
	WINDOW* menu = newwin(LINES - 1, COLS, 1, 0);
	size_t choice = 0;
	bool isChosen = false;
	InputData input;

	while (!isChosen)
	{
		for (size_t i = 0; i < options.size(); i++)
		{
			if (choice == i)
			{
				wattron(menu, A_BOLD);
				if (options[i].isInput)
					mvwaddstr(menu, 1 + i, 1, std::string("  " + options[i].name + ": " + input[options[i].id].value).c_str());
				else
					mvwaddstr(menu, 1 + i, 1, std::string("> " + options[i].name).c_str());
				wattroff(menu, A_BOLD);
			}
			else
			{
				if (options[i].isInput)
					mvwaddstr(menu, 1 + i, 1, std::string("  " + options[i].name + ": " + input[options[i].id].value).c_str());
				else
					mvwaddstr(menu, 1 + i, 1, std::string("  " + options[i].name).c_str());
			}
		}
		wrefresh(menu);

		// Сдвигаем курсор в конец написанной строки
		if (options[choice].isInput)
		{
			curs_set(1);
			move(2 + choice, 5 + options[choice].name.size() + input[options[choice].id].value.size());
		}

		int ch = getch();
		switch (ch)
		{
		case KEY_UP:
			if (choice > 0)
				choice--;
			else
				choice = options.size() - 1;
			break;
		case KEY_DOWN:
			if (choice < options.size() - 1)
				choice++;
			else
				choice = 0;
			break;
		case 10: // Enter
			if (!options[choice].isInput)
			{
				input[options[choice].id].isChosen = true;
				auto res = validator(input);
				if (res.first)
					isChosen = true;
				else
				{
					input[options[choice].id].isChosen = false;
					showPopup("Ошибка", res.second);
				}
			}
			break;
		case 8: // Backspace
			if (options[choice].isInput && !input[options[choice].id].value.empty())
				input[options[choice].id].value.pop_back();
			break;
		default:
			if (options[choice].isInput)
				input[options[choice].id].value.push_back(ch);
			break;
		}
		curs_set(0);
		wclear(menu);
	}

	delwin(menu);
	return input;
}

void GUI::printTable(const Table& table)
{
	WINDOW* win = newwin(LINES - 1, COLS, 1, 0);

	std::string divider = "+";
	std::string rowTemplate = "|";
	std::vector<int> pos(table.cols);
	int prev_pos = 2;
	for (int i = 0; i < table.cols; i++)
	{
		divider += std::string(table.headers[i].widht + 2, '-') + "+";
		rowTemplate += std::string(table.headers[i].widht + 2, ' ') + "|";
		pos[i] = prev_pos + 2;
		prev_pos += table.headers[i].widht + 3;
	}

	mvwaddstr(win, 1, 2, divider.c_str());
	mvwaddstr(win, 2, 2, rowTemplate.c_str());
	mvwaddstr(win, 3, 2, divider.c_str());
	for (int i = 0; i < table.cols; i++)
		mvwaddstr(win, 2, pos[i], table.headers[i].title.c_str());

	int offsetTop = 0;
	for (const auto& row : table.rows)
	{
		mvwaddstr(win, 4 + offsetTop, 2, rowTemplate.c_str());
		for (int i = 0; i < table.cols; i++)
			mvwaddstr(win, 4 + offsetTop, pos[i], row[i].c_str());
		mvwaddstr(win, 4 + offsetTop + 1, 2, divider.c_str());
		offsetTop += 2;
	}


	mvwaddstr(win, LINES - 3, (COLS - 35) / 2, "Нажмите Enter, чтобы закрыть");
	wrefresh(win);
	getch();
	delwin(win);
}

void GUI::showPopup(const std::string& title, const std::vector<std::string>& text)
{
	const int width = 42;
	const int height = 8 + text.size();
	WINDOW* popup = newwin(height, width, (LINES - height) / 2, (COLS - width) / 2);

	box(popup, 0, 0);
	wbkgd(popup, COLOR_PAIR(1));

	mvwaddstr(popup, 1, (width - title.length()) / 2, title.c_str());
	for (size_t i = 0; i < text.size(); i++)
		mvwaddstr(popup, 4 + i, (width - text[i].length()) / 2, text[i].c_str());
	mvwaddstr(popup, height - 2, 5, "Нажмите Enter, чтобы продолжить");

	wrefresh(popup);
	getch();
	delwin(popup);
}

void GUI::showPopup(const std::string& title, const std::string& text)
{
	showPopup(title, std::vector<std::string>({ text }));
}

void GUI::showCustomWindow(std::function<void(WINDOW* win)> render)
{
	WINDOW* win = newwin(LINES - 1, COLS, 1, 0);
	render(win);
	delwin(win);
}

Table::Table(const std::vector<std::string>&& headers)
	: cols(headers.size())
{
	for (const auto& it : headers)
		this->headers.push_back({ it, (int)it.size() });
}

void Table::addRow(const std::vector<std::string>&& row)
{
	if (row.size() != cols)
		throw std::invalid_argument("Указанный вектор не совпадает с кол-вом столбцов");
	for (int i = 0; i < row.size(); i++)
		headers[i].widht = std::max((int)row[i].size(), headers[i].widht);
	rows.push_back(row);
}
