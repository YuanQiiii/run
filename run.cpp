#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <iconv.h>
#include <cstring>

// 将UTF-8编码转换为UTF-16编码
std::wstring utf8_to_wstring(const std::string &str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), nullptr, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), &wstr[0], size_needed);
    return wstr;
}

void executeCommand(const std::wstring &path, char *argv)
{
    // 输出执行命令
    std::wcout << L"Running: " << argv << std::endl;

    // 使用 PowerShell 执行命令
    std::wstring command = L"powershell -Command \"Start-Process -File '" + path + L"'\"";
    _wsystem(command.c_str());
}

int main(int argc, char *argv[])
{
    // 检查命令行参数
    if (argc < 2)
    {
        std::cerr << "please input the name of software" << std::endl;
        return 1;
    }

    std::string input = argv[1];
    std::ifstream file("C:\\run\\paths.txt");
    std::string line;
    bool found = false;

    // 设置控制台输出为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8); // 设置输入代码页

    // 查找对应的文件路径
    while (std::getline(file, line))
    {
        if (line.find(input) != std::string::npos)
        {
            // 找到匹配的行，提取路径
            size_t start = line.find('"');
            size_t end = line.find('"', start + 1);
            if (start != std::string::npos && end != std::string::npos)
            {
                std::string path = line.substr(start + 1, end - start - 1);

                // 将路径转换为宽字符字符串
                std::wstring wpath = utf8_to_wstring(path);
                executeCommand(wpath, argv[1]); // 直接执行路径
                found = true;
                break;
            }
        }
    }

    if (!found)
    {
        std::cerr << "not found" << std::endl;
    }

    return 0;
}
