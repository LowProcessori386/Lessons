#include<stdio.h>
#include<Windows.h>
#include<TlHelp32.h>
#pragma warning(disable:4996)
#define dwLocalPlayer 0xDB65DC
#define dwClientState_ViewAngles 0x4D90
#define dwClientState  0x58CFC4
#define m_iHealth 0x100
typedef struct
{
	float xyz[3];
}Vector;
void Cheat()
{
	if (AllocConsole())
	{
		freopen("CONIN$", "rb", stdin);
		freopen("CONOUT$", "wb", stdout);
		freopen("CONOUT$", "wb", stderr);
	}
	DWORD modules = (DWORD)GetModuleHandle("client.dll");
	DWORD modules_engine = (DWORD)GetModuleHandle("engine.dll");
	DWORD* localplayer = (DWORD*)(modules + dwLocalPlayer);

	DWORD lp = *localplayer;
	DWORD* clientstate_ptr = (DWORD*)(modules_engine + dwClientState);
	DWORD clientstate = *clientstate_ptr;
	Vector angle_xyz;
	
	while (1)
	{

		
		int* hp = (int*)(lp + m_iHealth);
		memcpy(&angle_xyz, (PBYTE*)(clientstate + dwClientState_ViewAngles), sizeof(angle_xyz));//читаем знаения viewangleclientstate + dwClientState_ViewAngles
		angle_xyz.xyz[1] += 90;//каждый цикл прибавляем 90 градусов
		*(Vector*)(clientstate + dwClientState_ViewAngles) = angle_xyz;//запись в память игры
		printf("%f %f %f \n", angle_xyz.xyz[0], angle_xyz.xyz[1], angle_xyz.xyz[2]);
		printf("%d \n", *hp);
	}

}
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Cheat), hModule, 0, 0);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}
