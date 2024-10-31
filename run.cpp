#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

// 将UTF-8编码转换为UTF-16编码
std::wstring utf8_to_wstring(const std::string &str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), nullptr, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), &wstr[0], size_needed);
    return wstr;
}

// 执行命令
void executeCommand(const std::wstring &path, char *name)
{
    std::wcout << ">> " << L"Running: " << name << std::endl;
    std::wstring command = L"powershell -Command \"Start-Process -File '" + path + L"'\"";
    _wsystem(command.c_str());
}

void listEntries(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        // 找到并输出第一个字符串
        size_t end = line.find(' ');
        if (end != std::string::npos)
        {
            std::cout << ">> " << line.substr(0, end) << std::endl; // 输出第一个字符串
        }
        else
        {
            std::cout << "invalid path" << std::endl; // 如果没有空格，输出整行
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "please input the name of software" << std::endl;
        return 1;
    }
    std::string command = argv[1];

    // 如果命令是 ls，则列出所有条目
    if (command == "ls")
    {
        listEntries("C:\\run\\paths.txt");
        return 0; // 直接返回
    }

    std::wstring input = utf8_to_wstring(argv[1]);

    std::ifstream file("C:\\run\\paths.txt");
    std::string line;
    bool found = false;

    while (std::getline(file, line))
    {
        std::wstring wline = utf8_to_wstring(line);
        if (wline.substr(0, input.length()) == input && wline[input.length()] == L' ')
        {
            size_t start = line.find('"');
            size_t end = line.find('"', start + 1);
            if (start != std::string::npos && end != std::string::npos)
            {
                std::string path = line.substr(start + 1, end - start - 1);
                std::wstring wpath = utf8_to_wstring(path);
                executeCommand(wpath, argv[1]);
                found = true;
                break;
            }
        }
    }

    if (!found)
    {
        std::wcerr << L"not found" << std::endl;
    }

    return 0;
}
