#include <iostream>
#include "Base64.h"
#include <string>

int main() {
    std::string message;

    short choice;
    do {
        std::cout << "Выберите действие:" << std::endl
                  << "- Закодировать данные -> 1" << std::endl
                  << "- Декодировать данные -> 2" << std::endl;
        std::cin >> choice;
        std::cin.ignore();
    } while (choice != 1 && choice != 2);

    std::cout << "Введите данные для обработки:" << std::endl;
    getline(std::cin, message);

    if (choice == 1) {
        std::cout << encode(message) << std::endl;
    } else {
        std::cout << decode(message) << std::endl;
    }

    return 0;
}