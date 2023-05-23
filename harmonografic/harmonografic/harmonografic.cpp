#include <iostream>
#include <Windows.h>

#include "Memo.h"
#include "tests/tester.h"
#include "UserInterface.h"

int main()
{
    {
       // tester t;
    }

    UserInterface interface_window;

    interface_window.print();

    std::string s;
    std::getline(std::cin,s);

    for (size_t i = 185; i < 189; i++)
    {
        char c = i;
        std::cout << "[ i = " << i << " c = " << c << " ]" << std::endl;
    }

    for (size_t i = 200; i < 205; i++)
    {
        char c = i;
        std::cout << "[ i = " << i << " c = " << c << " ]" << std::endl;
    }

    std::cout << "\n\n\n";

    /*
    for (size_t i = 0; i < 255; i+=3)
    {
        char c = i;
        std::cout << "[ i = " << i << " c = " << c << " ]\t";
        c = i+1;
        std::cout << "[ i = " << i+1 << " c = " << c << " ]\t";
        c = i+2;
        std::cout << "[ i = " << i+2 << " c = " << c << " ]" << std::endl;
    }
    */

    return 0;
}

