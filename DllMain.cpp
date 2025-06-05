#include "Entry/Entry.h"

DWORD WINAPI dwMainThread(LPVOID lpArguments)
{
	G::ModuleEntry.Load();
	//DoMenuStuff(lpArguments);
	while (!GetAsyncKeyState(VK_END))
		std::this_thread::sleep_for(std::chrono::milliseconds(350));

	//InputSys::Get().Uninit(); 
	G::Hooks.Uninit();
	FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(lpArguments), EXIT_SUCCESS);
}

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
		if (const HANDLE hMain = CreateThread(0, 0, dwMainThread, hInstance, 0, 0))
			CloseHandle(hMain);

	return true;
}