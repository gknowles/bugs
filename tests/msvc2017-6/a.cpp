// Copyright Glen Knowles 2018.
// Distributed under the Boost Software License, Version 1.0.
//
// a.cpp - bugs msvc2017-6
//
// Compile with: Visual C++ 2017 Version 15.7.4
// Tested on: Windows 10 Version 1709 (OS Build 16299.492)
//
// When a console application is run in a new console there is an ownership
// conflict of the standard OS in/out/err handles between the console subsystem
// and the CRT. Calls to FreeConsole and _close (directly or indirectly via
// _dup) both close the handles, and doing both of those calls is required to
// reattach to a new console.
//
// All of which ends badly when some other code manages to reuse a handle
// value immediate after the first close and gets it closed unexpectedly
// by the second.

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <sstream>

#include <Windows.h>

using namespace std;

int launch(const char prog[], bool newConsole) {
    auto pid = GetCurrentProcessId();
    ostringstream ocmdline;
    ocmdline << '"' << prog << "\" " << pid;
    STARTUPINFO si = {};
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi;
    if (!CreateProcess(
        NULL,
        ocmdline.str().data(),
        NULL,   // proc attrs
        NULL,   // thread attrs
        false,  // inherit handles
        newConsole ? CREATE_NEW_CONSOLE : 0,
        NULL,   // environment
        NULL,   // current dir
        &si,
        &pi
    )) {
        cout << "ERROR " << GetLastError() << ": starting child." << endl;
        return 71; // EX_OSERR
    }
    if (!WaitForSingleObject(pi.hProcess, INFINITE))
        return 71; // EX_OSERR
    return 0;
}

int attach(int pid) {
    // Save OS console handles for later display
    intptr_t fd0 = _get_osfhandle(0);
    intptr_t fd1 = _get_osfhandle(1);
    intptr_t fd2 = _get_osfhandle(2);

    // Detach from current console. Closes the fd* handles above if launched
    // in a new console, otherwise leaves them alone.
    FreeConsole();

    // Open some handles, this is only done to demonstrate how some of them are
    // wrongfully closed.
    HANDLE prochandles[10];
    for (auto && proc : prochandles) {
        proc = OpenProcess(
            PROCESS_QUERY_LIMITED_INFORMATION,
            false,
            GetCurrentProcessId()
        );
        assert(proc && proc != INVALID_HANDLE_VALUE);
    }

    // Attach to new console and connect it to stdio
    if (!AttachConsole(pid)) {
        auto err = GetLastError();
        ostringstream os;
        os << "ERROR " << err << ": attaching to console of process"
            << ", pid = " << pid;
        AttachConsole(ATTACH_PARENT_PROCESS);
        WriteConsole(
            GetStdHandle(STD_OUTPUT_HANDLE),
            os.str().c_str(),
            (DWORD) os.str().size(),
            nullptr,
            nullptr
        );
        return 66; // EX_NOINPUT
    }
    struct { DWORD nstd; int fd; } streams[] = {
        { STD_INPUT_HANDLE, 0 },
        { STD_OUTPUT_HANDLE, 1 },
        { STD_ERROR_HANDLE, 2 },
    };
    for (auto && s : streams) {
        auto fdNew = _open_osfhandle((intptr_t) GetStdHandle(s.nstd), _O_TEXT);
        _dup2(fdNew, s.fd);
        _close(fdNew);
    }

    cout << "INFO: old stdin/out/err = " << fd0
        << ' ' << fd1
        << ' ' << fd2
        << endl;
    cout << "INFO: new stdin/out/err = " << _get_osfhandle(0)
        << ' ' << _get_osfhandle(1)
        << ' ' << _get_osfhandle(2)
        << endl;

    cout << "INFO: proc handles = ";
    for (auto && proc : prochandles) {
        cout << (intptr_t) proc << ' ';
    }
    cout << endl;

    // Test opened handles for ones that been closed or closed and reopened
    // as a different (i.e. stdio) type.
    for (auto && proc : prochandles) {
        if (!GetProcessId(proc)) {
            cout << "ERROR " << GetLastError() << ": using handle "
                << (intptr_t) proc << endl;
        } else if (!CloseHandle(proc)) {
            cout << "ERROR " << GetLastError() << ": closing handle "
                << (intptr_t) proc << endl;
        }
    }

    // Display "Hello." by all means available
    auto msg = "Hello via WriteConsole.\n"s;
    WriteConsole(
        GetStdHandle(STD_OUTPUT_HANDLE),
        msg.c_str(),
        (DWORD) msg.size(),
        nullptr,
        nullptr
    );
    msg = "Hello via _write.\n";
    _write(1, msg.c_str(), (unsigned) msg.size());
    printf("Hello via printf.\n");
    cout << "Hello via cout." << endl;

    return 0;
}

int main(int argc, char * argv[]) {
    if (argc != 2) {
        printf(1 + R"(
Runs as either a parent process that launches a child process or as the child
process that displays text (using 4 different methods) on the console of the
parent that launched it.

usage: PROGRAM {same|new|<pid>}
  same      run as parent, launch child process in parents console (works)
  new       run as parent, launch in new console (demonstrates error)
  <pid>     run as child, attaches to console of target process
)");
        return 64; // EX_USAGE
    }

    if (!strcmp(argv[1], "same")) {
        return launch(argv[0], false);
    } else if (!strcmp(argv[1], "new")) {
        return launch(argv[0], true);
    } else {
        auto pid = strtol(argv[1], nullptr, 10);
        return attach(pid);
    }
}
