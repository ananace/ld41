#pragma once

struct Version
{
    static const Version& GetVersion();

    int Major, Minor, Patch;
    char Revision[13];
};
