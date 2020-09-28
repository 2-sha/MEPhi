#ifdef _DEBUG
	#include <crtdbg.h>
	#define _CRTDBG_MAP_ALLOC
	#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#define _USE_MATH_DEFINES // ��� ������� � M_PI
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

	cxxopts::Options options("LinearStrophoid", "����� ��� ������ � ������ ����������");
	options.add_options()
		("a", "�������� ��������� a", cxxopts::value<int>())
		("d,descart", "Y �� X � ����������", cxxopts::value<int>())
		("p,polar", "������ �� ���� � �������� �������", cxxopts::value<int>())
		("v,volume", "����� ���� �������� �����")
		("c,curvature-radius", "������ �������� � ������� �����")
		("l,left-area", "������� ����� �����")
		("r,right-area", "������� �� �������� ������")
		("h,help", "����� ������� �� ��������");

	try
	{
		auto result = options.parse(argc, argv);

		if (result.count("help"))
		{
			std::cout << options.help() << std::endl;
			goto END;
		}

		if (!result.count("a"))
			throw std::exception("�� ������ �������� a");

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
			std::cout << "����: " << angle << " ����." << std::endl;
			std::cout << "����: " << angle * M_PI / 180 << " ���." << std::endl;
			std::cout << "������: " << ls.calcInPolar(angle * M_PI / 180) << std::endl;
		}
		else if (result.count("volume"))
			std::cout << "����� �������� ����� ������ �������: " << ls.getLoopVolume() << std::endl;
		else if (result.count("curvature-radius"))
			std::cout << "������ �������� � ������� �����: " << ls.getRadiusOfCurvature() << std::endl;
		else if (result.count("left-area"))
			std::cout << "������� ��� ����� ������ (�����): " << ls.getLoopArea() << std::endl;
		else if (result.count("right-area"))
			std::cout << "������� ��� ������ ������: " << ls.getAsymptoteArea() << std::endl;
	}
	catch (std::exception ex)
	{
		std::cout << "������: " << ex.what() << std::endl;
	}

	END:
#ifdef _DEBUG
	_CrtMemDumpAllObjectsSince(&_ms);
#endif
	return 0;
}