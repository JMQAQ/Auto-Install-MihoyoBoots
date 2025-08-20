#include <windows.h>  
#include <wininet.h>  
#include <stdio.h>  

#pragma comment(lib, "wininet.lib")  
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) 

BOOL ClickButton(const char* windowTitle, int x, int y) {
	HWND hwnd = FindWindowA(NULL, windowTitle);
	if (hwnd == NULL) {
		return -4;
	}

	SetForegroundWindow(hwnd);
	Sleep(100);

	RECT rect;
	GetWindowRect(hwnd, &rect);

	int absoluteX = rect.left + x;
	int absoluteY = rect.top + y;

	SetCursorPos(absoluteX, absoluteY);

	mouse_event(MOUSEEVENTF_LEFTDOWN, absoluteX, absoluteY, 0, 0);
	Sleep(50);
	mouse_event(MOUSEEVENTF_LEFTUP, absoluteX, absoluteY, 0, 0);

	return TRUE;
}

int main() {
	HINTERNET hInternet, hFile;
	DWORD bytesRead;
	char buffer[4096];

	hInternet = InternetOpen("Dowloder/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternet == NULL) {
		return -1;
	}

	hFile = InternetOpenUrl(hInternet,"https://ys-api.mihoyo.com/event/download_porter/link/ys_cn/official/pc_backup317", NULL, 0, INTERNET_FLAG_RELOAD, 0);
	if (hFile == NULL) {
		InternetCloseHandle(hInternet);
		return -2;
	}

	FILE* fp = fopen("1.exe", "wb");
	if (fp == NULL) {
		InternetCloseHandle(hFile);
		InternetCloseHandle(hInternet);
		return -3;
	}

	while (InternetReadFile(hFile, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
		buffer[bytesRead] = '\0';
		fwrite(buffer, sizeof(char), bytesRead, fp);
	}

	fclose(fp);
	InternetCloseHandle(hFile);
	InternetCloseHandle(hInternet);

	BlockInput(TRUE);

	ShellExecute(
		NULL,           
		"open",         
		"1.exe",        
		NULL,           
		NULL,           
		SW_RESTORE         
	);

	for (int i = 0; i < 10; i++) {
		Sleep(500);
		HWND hwnd = FindWindow(NULL, "米哈游启动器 安装程序");
		if (hwnd != NULL) break;
	}

	ClickButton("米哈游启动器 安装程序", 400, 300);
	ClickButton("米哈游启动器 安装程序", 500, 300);


	BlockInput(FALSE);
	return 0;
}