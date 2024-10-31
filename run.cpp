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

/*
新的要求

精简不必要的环节
比方说只有在使用powershell输出命令的时候才需要中文

现在重新设计路径规则
eric>f>"C:\Users\exqin\eric"
使用<作为分隔符
eric是指令，f是指令类型，代表文件夹，后面是路径
edge>p>"C:\Users\exqin\Desktop\eric\software\Tools\Microsoft Edge.lnk"
edge是指令，p为指令类型，代表程序，后面是路径

匹配的逻辑需要改变
现在的思路是把所有的txt文件的条目读取到动态数组中，这一步要求支持中文字符
每一个字符串通过>劈分成三部分 指令;指令类型;路径 ，这里只有路径会出现中文

使用map对终端中读取的指令在指令数组中进行搜索

ls仍旧代表输出所有的命令，但是现在要求输出指令类型在指令之前
比方说
p edge




*/