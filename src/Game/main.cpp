#include <Version.hpp>
#include <iostream>

int main(int argc, char** argv)
{
    auto& ver = Version::GetVersion();
    std::cout << "Version: " << ver.Major << "." << ver.Minor << "." << ver.Patch << " (" << ver.Revision << ")" << std::endl;

    return 0;
}
