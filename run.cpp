#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
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
void executeCommand(const std::wstring &path, const std::string &name, const std::wstring &type)
{
    std::wcout << L">>  Running:  " << utf8_to_wstring(name) << L"  (" << type << L")" << std::endl;
    std::wstring command = L"powershell -Command \"Start-Process -File '" + path + L"'\"";
    _wsystem(command.c_str());
}

// 列出所有条目
void listEntries(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        size_t pos = line.find('>');
        if (pos != std::string::npos)
        {
            std::cout << ">> " << line.substr(0, pos) << " "; // 输出指令
            size_t typePos = line.find('>', pos + 1);
            if (typePos != std::string::npos)
            {
                std::cout << line.substr(pos + 1, typePos - pos - 1) << std::endl; // 输出指令类型
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "please input command" << std::endl;
        return 1;
    }
    std::string command = argv[1];

    // 如果命令是 ls，则列出所有条目
    if (command == "ls")
    {
        listEntries("C:\\run\\paths.txt");
        return 0; // 直接返回
    }

    std::wstring input = utf8_to_wstring(command);
    std::ifstream file("C:\\run\\paths.txt");
    std::string line;
    bool found = false;

    // 使用unordered_map存储指令和路径
    std::unordered_map<std::string, std::pair<std::string, std::string>> commandMap;

    while (std::getline(file, line))
    {
        size_t pos = line.find('>');
        if (pos != std::string::npos)
        {
            std::string cmd = line.substr(0, pos);
            size_t typePos = line.find('>', pos + 1);
            if (typePos != std::string::npos)
            {
                std::string type = line.substr(pos + 1, typePos - pos - 1);
                size_t pathStart = line.find('"', typePos + 1);
                size_t pathEnd = line.find('"', pathStart + 1);
                if (pathStart != std::string::npos && pathEnd != std::string::npos)
                {
                    std::string path = line.substr(pathStart + 1, pathEnd - pathStart - 1);
                    commandMap[cmd] = {type, path};
                }
            }
        }
    }

    // 查找命令并执行
    auto it = commandMap.find(command);
    if (it != commandMap.end())
    {
        std::wstring wpath = utf8_to_wstring(it->second.second);
        std::wstring wtype = utf8_to_wstring(it->second.first);
        executeCommand(wpath, command, wtype);
        found = true;
    }

    if (!found)
    {
        std::wcerr << L"not found" << std::endl;
    }

    return 0;
}
