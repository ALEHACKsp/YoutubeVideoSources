#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
using namespace std;

int playerBase = 0xD8B2BC;

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, modName))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}


int main()
{
	cout << "PlayerBase: " << playerBase << endl;
	DWORD pID;
	HWND hWnd = FindWindowA(0, "Counter-Strike: Global Offensive");
	GetWindowThreadProcessId(hWnd, &pID);
	cout << "ProcessID: " << pID << endl;
	HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pID);
	cout << "ProcessHANDLE: " << pHandle << endl;
	DWORD clientdllBaseAddress = GetModuleBaseAddress(pID, L"client.dll");
	if (!clientdllBaseAddress) 
	{
		cout << "Error\n";
	}
	cout << "Client.dll adress: " << clientdllBaseAddress << endl;
	DWORD LocalPlayer;
	int health;
	ReadProcessMemory(pHandle, (LPVOID)(clientdllBaseAddress + playerBase), &LocalPlayer, sizeof(DWORD), 0);
	cout << "Localplayer: " << LocalPlayer << endl;
	//0x100
	ReadProcessMemory(pHandle, (LPVOID)(LocalPlayer + 0x100), &health, sizeof(int), 0);
	cout << "Health: " << health << endl;
	Sleep(440);
	system("cls");
	main();


}

