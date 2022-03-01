#include <iostream>
#include <unistd.h>

int main()
{
    sleep(10);
    std::cout << "Enter an echo: ";
    std::string echo;
    std::cin >> echo;
    std::cout << echo << std::endl;
    std::cin >> echo;

    return 0;
}