#include <iostream>
#include <Windows.h>

#include "Memo.h"
#include "../tests/tester.h"
#include "Harmonographic.h"


int main()
{
    {
       // tester t;
    }

    Harmonographic app;

    if (app.init())
    {
        while (app.isRun())
        {
            app.broadcast();
        }
    }
    

    std::cout << "App terminate. \n";


    std::string s;
    std::getline(std::cin,s);

    std::cout << "\n\n";
      
    return 0;
}


