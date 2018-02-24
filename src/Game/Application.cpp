#include "Application.hpp"
#include "IState.hpp"
#include "Version.hpp"

#include <iostream>

Application::Application()
{
}

Application::~Application()
{
}

bool Application::Init()
{
    auto& ver = Version::GetVersion();
    std::cout << "Starting..." << std::endl
        << "Version: " << ver.Major << "." << ver.Minor << "." << ver.Patch << " (" << ver.Revision << ")" << std::endl;

    return true;
}

bool Application::Run()
{
    return true;
}
