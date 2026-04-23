#pragma once

#include <windows.h>
#include <cstdint>
#include <string>

//----------------------------------------//
//              ENUMERATIONS              //
//----------------------------------------//

enum class Command : uint8_t
{
    Unknown = 0,
    None,
    Stop,
    Start,
    Reverse,
    Finish,
    Reset,
    Toggle,
    Update
};

enum class State : uint8_t
{
    Begin,
    Complete
};

//-----------------------------------//
//              STRUCTS              //
//-----------------------------------//

class CommandHandler
{
private:
    Command command = Command::None;
    std::wstring action[2][2] = { L"" };

public:
    inline Command Get() const noexcept { return command; }
    inline void Set(Command value) noexcept { command = value; }
    inline bool Is(Command value) const noexcept { return command == value; }
    inline void Reset() noexcept { command = Command::None; }

public:
    inline LPCWSTR GetAction(Command command, State state) const noexcept { return action[static_cast<uint8_t>(command) - static_cast<uint8_t>(Command::Start)][static_cast<uint8_t>(state)].c_str(); }
    inline void SetAction(Command command, State state, LPCWSTR value) noexcept { action[static_cast<size_t>(command) - static_cast<uint8_t>(Command::Start)][static_cast<uint8_t>(state)] = value; }
};

//-------------------------------//
//              API              //
//-------------------------------//

Command ParseCommand(LPCWSTR name) noexcept;