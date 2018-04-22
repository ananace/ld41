#include "Application.hpp"

int main(int argc, char** argv)
{
    Application app;

    app.Init(argc, argv);
    return app.Run() ? 0 : 1;
}
