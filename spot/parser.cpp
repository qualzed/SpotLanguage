#include <fstream>
#include <iostream>
#include <termcolor.hpp>
#include <vector>
#include <string>
#include <conio.h>
#include <windows.h>
#include "reader.hpp"

#define RED     termcolor::red
#define GREEN     termcolor::green
#define BLUE     termcolor::blue
#define RESET     termcolor::reset

const int lineNumberPadding = 1;
const int lineNumberWidth = lineNumberPadding + 2;

void SetCursorPosition(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void ClearScreen() {
    system("cls");
}

void LiveCode() {
    std::vector<std::string> lines(1, "");
    int currentLine = 0, cursorPos = 0;
    bool exit = false, saved = false;
    const int offsetX = lineNumberWidth;
    while (!exit) {
        ClearScreen();
        for (int i = 0; i < lines.size(); ++i) {
            std::cout << (i + 1) << ": " << lines[i] << "\n";
        }
        if (saved) {
            std::cout << RED << "\n> " << RESET << "Parsing your code...\n";
            saved = false;
        }

        int cursorScreenX = offsetX + cursorPos;
        int cursorScreenY = currentLine;
        SetCursorPosition(cursorScreenX, cursorScreenY);
        HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cci;
        GetConsoleCursorInfo(hStd, &cci);
        cci.bVisible = true;
        SetConsoleCursorInfo(hStd, &cci); 

        int ch = _getch();
        if (ch == 0 || ch == 224) {
            ch = _getch();
            if (ch == 72) {
                if (currentLine > 0) currentLine--;
                cursorPos = std::min(cursorPos, (int)lines[currentLine].size());
            } else if (ch == 80) {
                if (currentLine < (int)lines.size() - 1) currentLine++;
                cursorPos = std::min(cursorPos, (int)lines[currentLine].size());
            } else if (ch == 75) {
                if (cursorPos > 0) cursorPos--;
                else if (currentLine > 0) {
                    currentLine--;
                    cursorPos = lines[currentLine].size();
                }
            } else if (ch == 77) {
                if (cursorPos < (int)lines[currentLine].size()) cursorPos++;
                else if (currentLine < (int)lines.size() - 1) {
                    currentLine++;
                    cursorPos = 0;
                }
            }
        } else if (ch == 13) {
            std::string newLine = lines[currentLine].substr(cursorPos);
            lines[currentLine] = lines[currentLine].substr(0, cursorPos);
            lines.insert(lines.begin() + currentLine + 1, newLine);
            currentLine++;
            cursorPos = 0;
        } else if (ch == 8) {
            if (cursorPos > 0) {
                lines[currentLine].erase(cursorPos - 1, 1);
                cursorPos--;
            } else if (currentLine > 0) {
                cursorPos = lines[currentLine - 1].size();
                lines[currentLine - 1] += lines[currentLine];
                lines.erase(lines.begin() + currentLine);
                currentLine--;
            }
        } else if (ch == 3) {
            exit = true;
        } else if (ch == 19) {
            saved = true;
        } else if (ch == 9) {
            lines[currentLine].insert(lines[currentLine].begin() + cursorPos, ' ');
            lines[currentLine].insert(lines[currentLine].begin() + cursorPos + 1, ' ');
            lines[currentLine].insert(lines[currentLine].begin() + cursorPos + 2, ' ');
            lines[currentLine].insert(lines[currentLine].begin() + cursorPos + 3, ' ');
            cursorPos += 4;
        } else if (ch >= 32 && ch <= 126) {
            lines[currentLine].insert(lines[currentLine].begin() + cursorPos, ch);
            cursorPos++;
        }
    }
    std::string TempCode;
    for (const auto& l : lines) {
        TempCode += l + "\n";
    }
}

void Menu() {
    ClearScreen();
    int choice;
    std::cout << RED << ">" << RESET << " Spot Language v1.0.0\n" << RED << ">" << RESET << " Parser.\n";
    std::cout << RED << "\n\n\n1. " << RESET << "Use Spot code editor and parser.\n"; 
    std::cout << RED << "2. " << RESET << "Choose path to parse.\n"; 
    std::cout << RED << "3. " << RESET << "Close the parser.\n"; 
    std::cin >> choice;
    switch(choice) {
        case 1: LiveCode();
        case 2: {
            ClearScreen();
            std::cout << GREEN << "> " << RESET << "Type the path to .spot file.\n> ";
            std::string PathToFile;
            std::cin >> PathToFile;
            std::cin.ignore(10000, '\n');
            Interpretate(PathToFile);
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
            Menu();
            break;
        }
        case 3: {
            ClearScreen(); 
            exit(0);
            break;
        }
    }
}

int main() {
    Menu();
    return 1;
}