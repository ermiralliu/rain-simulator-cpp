#include <windows.h>
#include "windowManagement.hpp"
#include <iostream>
#include <fstream>

void showConsole(bool show){
	HWND consoleWindow = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(consoleWindow, show);
}
void showConsole(){
	showConsole(true);
}

void exeToForeground(){
	showConsole(false);
	HWND rainWindow = FindWindowA(NULL, "Rain Simulator" );
	SetActiveWindow(rainWindow);
}

void writeFile(const int& count){
	std::ofstream dropFile("Raindrops.txt");
	if(!dropFile){
		std::cerr<<"\nUnable to write to file";
		return;
	}
	dropFile<<"Number of fallen raindrops is:"<<count<<"\n";
	dropFile.close();
}

bool Quit(const int& count){
	showConsole();
	writeFile(count);
	std::cout<<"Number of fallen raindrops in the end is: "<<count<<"\n";
	return false;
}