#include "Application.hpp"

int main(int argc, char** argv)
{
    Application app;

    (void)argc;
    (void)argv;

    app.Init();
    return app.Run() ? 0 : 1;
}
