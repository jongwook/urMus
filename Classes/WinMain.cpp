// GLRect.cpp
// OpenGL SuperBible, 
// Program by Richard S. Wright Jr.
// This program shows a very simple OpenGL program using
// the standard Win32 API function calls.

#include <windows.h>

static LPCTSTR lpszAppName = L"urMus Simulator";

#if (defined _CONSOLE) || (!defined _WIN32)

int main(int argc, char ** argv) {
	printf("urMus\n");
	return WinMain((HINSTANCE)GetModuleHandle(NULL), NULL, "", 0);
}

#endif

//////////////////////////////////////////////////////////////////
// Entry point of all Windows programs
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	return 0;
}

