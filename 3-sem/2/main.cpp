#ifdef _DEBUG
	#include <crtdbg.h>
	#define _CRTDBG_MAP_ALLOC
	#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#define _USE_MATH_DEFINES // Для доступа к M_PI
#include <iostream>
#include <math.h>
#include "cxxopts.hpp"
#include "utils.h"

int main(int argc, char** argv)
{
#ifdef _DEBUG
	_CrtMemState _ms;
	_CrtMemCheckpoint(&_ms);
	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	//_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
#endif
	setlocale(0, "");

	cxxopts::Options options("LinearStrophoid", "Класс для работы с прямой строфоидой");
	options.add_options()
		("a", "Значение параметра a", cxxopts::value<int>())
		("d,descart", "Y по X в декартовой", cxxopts::value<int>())
		("p,polar", "Радиус по углу в полярной системе", cxxopts::value<int>())
		("v,volume", "Объём тела вращения петли")
		("c,curvature-radius", "Радиус кривизны в узловой точке")
		("l,left-area", "Плоащдь петли слева")
		("r,right-area", "Плоащдь до асимптот справа")
		("h,help", "Вывод справки по командам");

	try
	{
		auto result = options.parse(argc, argv);

		if (result.count("help"))
		{
			std::cout << options.help() << std::endl;
			goto END;
		}

		if (!result.count("a"))
			throw std::exception("Не указан параметр a");

		int a = result["a"].as<int>();
		LinearStrophoid ls(a);
		std::cout << "a: " << a << std::endl << std::endl;
		if (result.count("descart"))
		{
			int x = result["descart"].as<int>();
			auto values = ls.calcInCartesian(x);
			std::cout << "X: " << x << std::endl;
			std::cout << "Y: " << values.first << std::endl;
			std::cout << "Y: " << values.second << std::endl;
		}
		else if (result.count("polar"))
		{
			int angle = result["polar"].as<int>();
			std::cout << "Угол: " << angle << " град." << std::endl;
			std::cout << "Угол: " << angle * M_PI / 180 << " рад." << std::endl;
			std::cout << "Радиус: " << ls.calcInPolar(angle * M_PI / 180) << std::endl;
		}
		else if (result.count("volume"))
			std::cout << "Объём вращения петли вокруг абсцисс: " << ls.getLoopVolume() << std::endl;
		else if (result.count("curvature-radius"))
			std::cout << "Радиус кривизны в узловой точке: " << ls.getRadiusOfCurvature() << std::endl;
		else if (result.count("left-area"))
			std::cout << "Площадь под левой частью (петлёй): " << ls.getLoopArea() << std::endl;
		else if (result.count("right-area"))
			std::cout << "Площадь под правой частью: " << ls.getAsymptoteArea() << std::endl;
	}
	catch (std::exception ex)
	{
		std::cout << "Ошибка: " << ex.what() << std::endl;
	}

	END:
#ifdef _DEBUG
	_CrtMemDumpAllObjectsSince(&_ms);
#endif
	return 0;
}