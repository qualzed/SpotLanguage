/*
    Spot v1.0.0
    Functions header, use CTRL+F
*/

#include <string>

template <typename... Args>
void out(const Args...  args) { // Like print() in Python
    (std::cout << ... << args) << std::endl;
}

void gchar(const std::string& prompt, auto& var) { // The same thing like input in Python
    std::cout << prompt;
    std::getline(std::cin, var); // There is a global var, just use that variable that u include in function
    // gchar("Something: ", TheVar); - After u can use out(TheVar);
}

// void calc(auto& a, char& operation, auto& b) {
//     int Temp;
//     switch(operation) {
//         case "+": Temp = a+b;
//         case "-": Temp = a-b;
//         case "/": Temp = a/b;
//         case "*": Temp = a*b;
//     }
//     std::cout << Temp << endl;
// }