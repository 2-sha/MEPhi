#pragma once

#include <curses.h>
#include <vector>
#include <array>
#include <functional>
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <locale.h>
#include <string>
#include <map>
#include <io.h>

namespace cursesgui
{

	class Table
	{
	private:
		struct Column
		{
			std::string title;
			int widht;
		};

		int cols;
		std::vector<Column> headers;
		std::vector<std::vector<std::string>> rows;

	public:
		Table(const std::vector<std::string>&&);

		void addRow(const std::vector<std::string>&&);

		friend class GUI;
	};

	class GUI
	{
	public:
		typedef std::function<void(GUI&)> ActionFunc;

		struct ActionField {
			int id;
			std::string name;
			ActionFunc func;
		};

		struct choiceField {
			int id;
			std::string name;
		};

		struct InputChoiceField {
			int id;
			std::string name;
			bool isInput;
		};

		struct InputRes {
			std::string value;
			bool isChosen;
		};

		typedef std::map<int, InputRes> InputData;

	private:
		std::string windowTitle;
		std::string headerStatus;
		bool isRuning = false;
		int returnStatus = 0;
		std::map<int, ActionField> actions;

		WINDOW* header;

		void init();

	public:

		GUI(const std::string&& = "Progrann", const std::string&& = "");
		~GUI();

		void addActions(const std::vector<ActionField>&&);
		void removeAction(const int&&);
		void updateStatus(const std::string&);
		void setWindowTitle(const std::string&);

		void quit(int = 0);
		int run();

		int getChoice(const std::vector<choiceField>&);
		InputData getInputs(const std::vector<InputChoiceField>&,
							std::function<std::pair<bool, std::string>(InputData)> = [](InputData) { return std::make_pair(true, ""); });
		void printTable(const Table& table);
		void showPopup(const std::string&, const std::vector<std::string>&);
		void showPopup(const std::string&, const std::string&);
		void showCustomWindow(std::function<void(WINDOW* win)>);
	};

}