#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <windows.h>
#include "employee.h"

void isSen(DWORD& writeByte, bool& isSend, HANDLE hPipe, char cmd[10]) {
    bool isSent = WriteFile(hPipe, cmd, 10, &writeByte, NULL);
    if (!isSent) {
        std::cerr << "error" << std::endl;
        return;
    }
}
bool isRea(bool& isSend, employee& e, DWORD& readBytes, HANDLE hPipe) {
    bool isRead = ReadFile(hPipe, &e, sizeof(e), &readBytes, NULL);
    if (!isRead) {
        std::cerr << "Error in receiving answer" << std::endl;
    }
}

int main(int argc, char** argv) {
    HANDLE hReadyEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, LPWSTR(argv[1]));
    HANDLE hStartEvent = OpenEvent(SYNCHRONIZE, FALSE, L"START_ALL");
    if (NULL == hStartEvent || NULL == hReadyEvent) {
        std::cerr << "error" << std::endl;
        return GetLastError();
    }
    SetEvent(hReadyEvent);
    std::cout << "Process is ready" << std::endl;
    WaitForSingleObject(hStartEvent, INFINITE);
    std::cout << "Process is started" << std::endl;

    HANDLE hPipe;
    while (true) {
        const char pipeName[30] = "\\\\.\\pipe\\pipe_name";
        hPipe = CreateFile(LPWSTR(pipeName), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL,
            OPEN_EXISTING, 0, NULL);
        if (INVALID_HANDLE_VALUE != hPipe) {
            break;
        }
        if (!WaitNamedPipe(LPWSTR(pipeName), 5000)) {
            std::cout << "5 second wait timed out." << std::endl;
            return 0;
        }
    }
    std::cout << "Connected to pipe" << std::endl;

    while (true) {
        employee emp;//employee.h
        char command[10];
        std::cout << "Enter r or w to read / write " << std::endl;
        std::cin.getline(command, 10, '\n');
        if (std::cin.eof()) {
            return;
        }
        DWORD bytesWritten;
        bool isSent;
        isSen(bytesWritten, isSent, hPipe, command);
        DWORD readBytes;
        if (isRea(isSent, emp, readBytes, hPipe));
        else {
            if (emp.num < 0) {
                std::cerr << "error" << std::endl;
                continue;
            }
            else {
                //if correct
                emp.print(std::cout);
                if ('w' == command[0]) {
                    std::cout << "Enter ID, name and hours" << std::endl << std::flush;
                    std::cin >> emp.num >> emp.name >> emp.hours;
                    std::cin.ignore(2, '\n');
                    isSent = WriteFile(hPipe, &emp, sizeof(emp), &bytesWritten, NULL);
                    if (isSent)
                        std::cout << "new record" << std::endl;
                    else {
                        std::cerr << "error" << std::endl;
                        break;
                    }
                }
            }
        }
    }
    return 0;
}