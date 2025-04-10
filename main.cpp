#define _CRT_SECURE_NO_WARNINGS
#include "Cheats.h"
#include "Offsets.h"
#include "Resources/Language.h"
#include "Utils/Initial/Init.h"
#include <chrono>
#include <filesystem>
#include <iomanip>
#include <KnownFolders.h>
#include <ShlObj.h>
#include <stdio.h>
#include "xor.hpp"
#include <Windows.h>
#include "auth.hpp"
#include <string>
#include "skStr.h"
#include <stdio.h>
#include <urlmon.h>
#include <thread>
#include <chrono>
#include <direct.h>
#include <iostream>
#include "skStr.h"
#include <conio.h>
#include <fstream>
#include <string>
#include "curl/curl.h"
#include <fstream>
#include <string>
#include "filestuff.h"
#include <direct.h>
#include "json.hpp"

using json = nlohmann::json;
std::string tm_to_readable_time(tm ctx);
static std::time_t string_to_timet(std::string timestamp);
static std::tm timet_to_tm(time_t timestamp);
const std::string compilation_date = (std::string)skCrypt(__DATE__);
const std::string compilation_time = (std::string)skCrypt(__TIME__);

using namespace std;

namespace fs = filesystem;
string fileName;

void Exit()
{
	system("pause");
	exit(0);
}

std::string getHiddenInput()
{
	std::string input;
	char ch;
	while (true)
	{
		ch = _getch();
		if (ch == '\r')
		{
			std::cout << std::endl;
			break;
		}
		else if (ch == '\b')
		{
			if (!input.empty())
			{
				input.pop_back();
				std::cout << "\b \b";
			}
		}
		else
		{
			input += ch;
			std::cout << '*';
		}
	}
	return input;
}

void logo3()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	SetConsoleTextAttribute(hConsole, 11);
	std::cout <<
		R"(
                          Premium CS2 External (Version: 3.4)


	  .,-::::: .::::::.   .:::.        .,-:::::   ::   .: .,::::::   :::. ::::::::::::
	,;;;'````';;;`    `  ,;'``;.     ,;;;'````'  ,;;   ;;,;;;;''''   ;;`;;;;;;;;;;''''
	[[[       '[==/[[[[, ''  ,[['    [[[        ,[[[,,,[[[ [[cccc   ,[[ '[[,   [[     
	$$$         '''    $ .c$$P'      $$$        "$$$"""$$$ $$""""  c$$$cc$$$c  $$     
	`88bo,__,o,88b    dPd88 _,oo,    `88bo,__,o, 888   "88o888oo,__ 888   888, 88,    
	  "YUMMMMMP""YMmMY" MMMUP*"^^      "YUMMMMMP"MMM    YMM""""YUMMMYMM   ""`  MMM    
 
        )";

	std::cout << skCrypt("\n Starting in 5 seconds. ");
	Sleep(5000);
}

void Cheat()
{
	if (Init::Verify::CheckWindowVersion())
	{
		Lang::GetCountry(MenuConfig::Country);
		MenuConfig::MaxFrameRate = Init::Client::getMaxFrameRate();
	}
		
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	//Gets a standard output device handle  
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN);	//Set the text color to green  
	logo3();
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

	auto ProcessStatus = ProcessMgr.Attach("cs2.exe");

	std::cout << skCrypt("\n ");

	char documentsPath[MAX_PATH];
	if (SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, 0, documentsPath) != S_OK) {
		cerr << "\n [Info] Error: Failed to get the Documents folder path." << endl;
		Exit();
	}
	MenuConfig::path = documentsPath;
	MenuConfig::path += "\\CS2";

	switch (ProcessStatus) {
	case 1:
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		cout << "\n [ERROR] Please launch the game first! \n" << endl;
		Exit();
	case 2:
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		cout << "\n [ERROR] Failed to hook process, please run the cheat as Administrator." << endl;
		Exit();
	case 3:
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		cout << "\n [ERROR] Failed to get module address." << endl;
		Exit();
	default:
		break;
	}

	if (!Offset::UpdateOffsets())
	{
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		cout << "\n [ERROR] Failed to update offsets." << endl;
		Exit();
	}

	if (!gGame.InitAddress())
	{
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		cout << "\n [ERROR] Failed to call InitAddress()." << endl;
		Exit();
	}

	cout << "\n [Game] Process ID: " << ProcessMgr.ProcessID << endl;
	cout << "\n [Game] Client Address: " << gGame.GetClientDLLAddress() << endl;

	if (fs::exists(MenuConfig::path))
	{
		cout << "\n [Info] Config folder connected: " << MenuConfig::path << endl;
	}
	else
	{
		if (fs::create_directory(MenuConfig::path))
		{
			cout << "[Info] Config folder created: " << MenuConfig::path << endl;
		}
		else
		{
			cerr << "[Info] Error: Failed to create the config directory." << endl;
			Exit();
		}
	}

	cout << endl;
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	cout << "Cheat running successfully!" << endl;
	cout << "Press [INS] to show or hide the menu." << endl;
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED);
	cout << "=======[ Offset List ]=======" << endl;
	cout << setw(23) << left << "EntityList:" << setiosflags(ios::uppercase) << hex << Offset::EntityList << endl;
	cout << setw(23) << left << "Matrix:" << setiosflags(ios::uppercase) << hex << Offset::Matrix << endl;
	cout << setw(23) << left << "LocalPlayerController:" << setiosflags(ios::uppercase) << hex << Offset::LocalPlayerController << endl;
	cout << setw(23) << left << "ViewAngles:" << setiosflags(ios::uppercase) << hex << Offset::ViewAngle << endl;
	cout << setw(23) << left << "LocalPlayerPawn:" << setiosflags(ios::uppercase) << hex << Offset::LocalPlayerPawn << endl;
	cout << setw(23) << left << "PlantedC4:" << setiosflags(ios::uppercase) << hex << Offset::PlantedC4 << endl;
	cout << setw(23) << left << "ForceJump:" << setiosflags(ios::uppercase) << hex << Offset::ForceJump << endl;
	cout << setw(23) << left << "Sensitivity:" << setiosflags(ios::uppercase) << hex << Offset::Sensitivity << endl;
	// cout << setw(23) << left << "ForceCrouch:" << setiosflags(ios::uppercase) << hex << Offset::ForceCrouch << endl;
	// cout << setw(23) << left << "ForceForward:" << setiosflags(ios::uppercase) << hex << Offset::ForceForward << endl;
	// cout << setw(23) << left << "ForceLeft:" << setiosflags(ios::uppercase) << hex << Offset::ForceLeft << endl;
	// cout << setw(23) << left << "ForceRight:" << setiosflags(ios::uppercase) << hex << Offset::ForceRight << endl;
	// cout << setw(23) << left << "TestPointer:" << setiosflags(ios::uppercase) << hex << Offset::Pointer << endl;
	cout << endl;
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

	try
	{
		Gui.AttachAnotherWindow("Counter-Strike 2", "SDL_app", Cheats::Run);
	}
	catch (OSImGui::OSException& e)
	{
		try
		{
			// Perfect World version
			Gui.AttachAnotherWindow("反恐精英：全球攻势", "SDL_app", Cheats::Run);
		}
		catch (OSImGui::OSException& e)
		{
			cout << e.what() << endl;
		}
	}
}

int main()
{
	std::thread titleThread(animateConsoleTitle);
		Cheat();
}
