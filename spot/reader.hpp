#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include "..\functions\header.hpp"

std::vector<std::string> codeLines;
std::unordered_map<std::string, std::string> vars;  // variables

// trim whitespace
std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t");
    return s.substr(start, end - start + 1);
}

// remove ; and trim
std::string cleanLine(const std::string& s) {
    std::string line = trim(s);
    if (!line.empty() && line.back() == ';') line.pop_back();
    return trim(line);
}

// check prefix (C++17 compatible)
bool startsWith(const std::string& s, const std::string& prefix) {
    return s.size() >= prefix.size() && s.compare(0, prefix.size(), prefix) == 0;
}

// handle out(...)
void handleOut(const std::string& line) {
    size_t l = line.find('(') + 1;
    size_t r = line.find_last_of(')');
    std::string arg = trim(line.substr(l, r - l));

    if (!arg.empty() && arg.front() == '"' && arg.back() == '"') {
        out(arg.substr(1, arg.size() - 2));
        return;
    }

    bool isNum = !arg.empty();
    for (char c : arg) if (!isdigit(c)) { isNum = false; break; }
    if (isNum) {
        out(std::stoi(arg));
        return;
    }

    auto it = vars.find(arg);
    out(it != vars.end() ? it->second : "<undefined: " + arg + ">");
}

// handle gchar("prompt", var)
void handleGchar(const std::string& line) {
    size_t q1 = line.find('"');
    size_t q2 = line.find('"', q1 + 1);
    if (q1 == std::string::npos || q2 == std::string::npos) return;

    std::string prompt = line.substr(q1 + 1, q2 - q1 - 1);

    size_t comma = line.find(',', q2);
    if (comma == std::string::npos) return;

    size_t vstart = line.find_first_not_of(" \t,", comma + 1);
    size_t vend = line.rfind(')');
    if (vstart == std::string::npos || vend == std::string::npos) return;

    std::string varName = trim(line.substr(vstart, vend - vstart));

    std::string input;
    std::cout << prompt << std::flush;
    // std::cout << prompt << std::endl;
    std::getline(std::cin, input);

    vars[varName] = input;
}

// main interpreter function — just call it with file path
void Interpretate(const std::string& path) {
    std::ifstream file(path);
    codeLines.clear();
    vars.clear();

    if (!file.is_open()) {
        std::cout << "Error: cannot open file " << path << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        codeLines.push_back(line);
    }
    file.close();

    for (size_t i = 0; i < codeLines.size(); ++i) {
        std::string raw = codeLines[i];
        std::string cmd = cleanLine(raw);

        if (cmd.empty()) continue;

        // remove comments
        size_t hash = cmd.find('#');
        if (hash != std::string::npos) cmd = cmd.substr(0, hash);
        cmd = trim(cmd);
        if (cmd.empty()) continue;

        if (startsWith(cmd, "out(")) {
            handleOut(cmd);
        }
        else if (startsWith(cmd, "gchar(")) {
            handleGchar(cmd);
        }
        else {
            std::cout << "Unknown command (line " << (i + 1) << "): " << cmd << std::endl;
        }
    }
}