#include <windows.h>  
#include <wininet.h>  
#include <stdio.h>  

#pragma comment(lib, "wininet.lib")  
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) 

int ClickButton(int x, int y) {
	HWND hwnd = FindWindowA("Qt51517QWindowIcon", NULL);

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
	return 0;
}

int DownLoad(const char* Url) {
	HINTERNET hInternet, hFile;
	DWORD bytesRead;
	char buffer[4096];

	hInternet = InternetOpen("Downloder/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternet == NULL) {
		exit(-1);
	}

	hFile = InternetOpenUrl(hInternet, Url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
	if (hFile == NULL) {
		InternetCloseHandle(hInternet);
		exit(-2);
	}

	FILE* fp = fopen("1.exe", "wb");
	if (fp == NULL) {
		InternetCloseHandle(hFile);
		InternetCloseHandle(hInternet);
		exit(-3);
	}

	while (InternetReadFile(hFile, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
		buffer[bytesRead] = '\0';
		fwrite(buffer, sizeof(char), bytesRead, fp);
	}

	fclose(fp);
	InternetCloseHandle(hFile);
	InternetCloseHandle(hInternet);
	return 0;
}

int main() {
	DownLoad("https://ys-api.mihoyo.com/event/download_porter/link/ys_cn/official/pc_backup317");

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
		HWND hwnd = FindWindow("Qt51517QWindowIcon", NULL);
		if (hwnd != NULL) break;
	}

	ClickButton(400, 300);
	ClickButton(325, 220);

	BlockInput(FALSE);

	return 0;
}