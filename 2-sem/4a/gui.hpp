#pragma once

#include <curses.h>
#include <vector>
#include <functional>
#include <utility>
#include <locale.h>
#include <string>
#include <io.h>

class GUI
{
private:
	std::string windowTitle = "Приложение";
	std::string headerStatus = "Статус: не указан";

	WINDOW *header;

	void init()
	{
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

public:
	GUI(const std::string &windowTitle)
		: windowTitle(windowTitle) 
	{
		init();
	}

	GUI()
	{
		init();
	}

	~GUI()
	{
		delwin(header);
		endwin();
	}

	void updateStatus(const std::string &status)
	{
		headerStatus = status;
		wclear(header);

		attron(COLOR_PAIR(1));
		mvwaddstr(header, 0, COLS - headerStatus.size() - 3, headerStatus.c_str());
		mvwaddstr(header, 0, 3, windowTitle.c_str());
		attroff(COLOR_PAIR(1));
		wrefresh(header);
	}

	void setWindowTitle(const std::string &title)
	{
		windowTitle = title;
		wclear(header);

		attron(COLOR_PAIR(1));
		mvwaddstr(header, 0, COLS - headerStatus.size() - 3, headerStatus.c_str());
		mvwaddstr(header, 0, 3, windowTitle.c_str());
		attroff(COLOR_PAIR(1));
		wrefresh(header);
	}

	size_t printMenu(const std::vector<std::string> &options)
	{
		WINDOW *menu = newwin(LINES - 1, COLS, 1, 0);
		size_t choise = 0;
		bool isChosen = false;

		while (!isChosen)
		{
			for (size_t i = 0; i < options.size(); i++)
			{
				if (choise == i)
				{
					wattron(menu, A_BOLD);
					mvwaddstr(menu, 1 + i, 1, "> ");
				}
				else
					mvwaddstr(menu, 1 + i, 1, "  ");
				wprintw(menu, options[i].c_str());
				wattroff(menu, A_BOLD);
			}
			wrefresh(menu);

			switch (getch())
			{
				case KEY_UP:
					if (choise > 0)
						choise--;
					else
						choise = options.size() - 1;
					break;
				case KEY_DOWN:
					if (choise < options.size() - 1)
						choise++;
					else
						choise = 0;
					break;
				case 10:
					isChosen = true;
					break;
			}
		}

		delwin(menu);
		return choise;
	}

	std::vector<std::pair<std::string, bool>> printInputMenu(const std::vector<std::pair<std::string, bool>> &options)
	{
		WINDOW *menu = newwin(LINES - 1, COLS, 1, 0);
		size_t choise = 0;
		bool isChosen = false;
		std::vector<std::pair<std::string, bool>> input(options.size());

		while (!isChosen)
		{
			for (size_t i = 0; i < options.size(); i++)
			{
				if (choise == i)
				{
					wattron(menu, A_BOLD);
					if (options[i].second)
						mvwaddstr(menu, 1 + i, 1, std::string("  " + options[i].first + ": " + input[i].first).c_str());
					else
						mvwaddstr(menu, 1 + i, 1, std::string("> " + options[i].first).c_str());
					wattroff(menu, A_BOLD);
				}
				else
				{
					if (options[i].second)
						mvwaddstr(menu, 1 + i, 1, std::string("  " + options[i].first + ": " + input[i].first).c_str());
					else
						mvwaddstr(menu, 1 + i, 1, std::string("  " + options[i].first).c_str());
				}
			}
			wrefresh(menu);

			if (options[choise].second)
			{
				curs_set(1);
				move(2 + choise, 5 + options[choise].first.size() + input[choise].first.size());
			}

			int ch = getch();
			switch (ch)
			{
				case KEY_UP:
					if (choise > 0)
						choise--;
					else
						choise = options.size() - 1;
					break;
				case KEY_DOWN:
					if (choise < options.size() - 1)
						choise++;
					else
						choise = 0;
					break;
				case 10: // Enter
					if (!options[choise].second)
						isChosen = true;
					break;
				case 8: // Backspace
					if (options[choise].second && !input[choise].first.empty())
						input[choise].first.pop_back();
					break;
				default:
					if (options[choise].second)
						input[choise].first.push_back(ch);
					break;
			}
			curs_set(0);
			wclear(menu);
		}

		delwin(menu);
		input[choise].second = true;
		return input;
	}

	void showPopup(const std::string &title, const std::vector<std::string> &text)
	{
		const int width = 42;
		const int height = 8 + text.size();
		WINDOW *popup = newwin(height, width, (LINES - height) / 2, (COLS - width) / 2);

		box(popup, 0, 0);
		wbkgd(popup, COLOR_PAIR(1));

		mvwaddstr(popup, 1, (width - title.length()) / 2, title.c_str());
		for (size_t i = 0; i < text.size(); i++)
		{
			mvwaddstr(popup, 4 + i, (width - text[i].length()) / 2, text[i].c_str());
		}
		mvwaddstr(popup, height - 2, 5, "Нажмите Enter, чтобы продолжить");

		wrefresh(popup);
		getch();
		delwin(popup);
	}

	void showPopup(const std::string &title, const std::string &text)
	{
		showPopup(title, std::vector<std::string>({ text }));
	}

	void showCustomWindow(std::function<void(WINDOW *win)> render)
	{
		WINDOW *win = newwin(LINES - 1, COLS, 1, 0);
		render(win);
		delwin(win);
	}

};