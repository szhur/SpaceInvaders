// ◦ Playrix ◦
#pragma once

#include <fstream>

enum {
	windowWidth = 1024,
	windowHeight = 768
};

template <typename _T>
void printLog(_T const & _data)
{
	std::ofstream fout;

	fout.open("output.txt", std::ios_base::app);

    fout << "TEST: " << _data << std::endl;

    fout.close();
}