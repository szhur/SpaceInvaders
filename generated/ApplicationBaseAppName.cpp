// engine/src/Core/App/ApplicationBaseAppName.cpp.in используется чтобы генерировать ApplicationBaseAppName.cpp в CMakeLists.txt

// ◦ Playrix ◦

#include "Core/App/ApplicationBase.h"

namespace Core
{

std::string_view ApplicationBase::GetName() {
	return "SpaceInvaders";
}

}
