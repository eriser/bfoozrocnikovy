// reSounder.cpp : main project file.

#include "stdafx.h"
#include "ReSounder.h"

using namespace reSounder;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew ReSounder());
	return 0;
}
