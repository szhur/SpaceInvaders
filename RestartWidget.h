// ◦ Playrix ◦
#pragma once

#include <PlayrixEngine.h>

namespace SpaceInvaders {

using namespace Core;

class RestartWidget : public GUI::Widget
{
public:
	RestartWidget(const std::string& name, pugi::xml_node elem)
        : GUI::Widget(name, elem)
    {}

	void AcceptMessage(const Message& message) override
    {}
};

} // namespace SpaceInvaders
