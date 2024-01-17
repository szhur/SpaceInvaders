// engine/src/Core/App/Win_ApplicationWindowName.cpp.in используется чтобы генерировать Win_ApplicationWindowName.cpp в CMakeLists.txt

// ◦ Playrix ◦

#include "Core/Application.h"

namespace Core
{

std::string_view Application::GetWindowName() {
	return "PlrExampleApp";
}

}
