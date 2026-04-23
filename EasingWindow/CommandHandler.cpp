#include "CommandHandler.h"

//---------------------------------//
//              TABLE              //
//---------------------------------//

//static const std::unordered_map<std::wstring, Command> CommandMap =
//{
//    {L"stop",       Command::Stop},
//    {L"start",      Command::Start},
//    {L"reverse",    Command::Reverse},
//    {L"finish",     Command::Finish},
//    {L"reset",      Command::Reset},
//    {L"toggle",     Command::Toggle},
//    {L"update",     Command::Update},
//};

struct CommandEntry
{
    LPCWSTR name;
    Command value;
};

static constexpr CommandEntry CommandTable[] =
{
    { L"stop",    Command::Stop    },
    { L"start",   Command::Start   },
    { L"reverse", Command::Reverse },
    { L"finish",  Command::Finish  },
    { L"reset",   Command::Reset   },
    { L"toggle",  Command::Toggle  },
    { L"update",  Command::Update  },
};

//-------------------------------//
//              API              //
//-------------------------------//

//Command ParseCommand(const std::wstring& name) noexcept
//{
//    auto it = CommandMap.find(name);
//    return (it != CommandMap.end()) ? it->second : Command::Unknown;
//}

Command ParseCommand(LPCWSTR name) noexcept
{
    if (!name || !*name)
        return Command::Unknown;

    for (const auto& entry : CommandTable)
    {
        if (_wcsicmp(entry.name, name) == 0)
            return entry.value;
    }

    return Command::Unknown;
}