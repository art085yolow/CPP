#include <iostream>
#include <Windows.h>

#include "./TreeNode/Memo.h"
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
            app.RunMessageLoop();
        }
    }
    

    std::cout << "App terminate. \n";


    // std::string s;
    // std::getline(std::cin,s);

    std::cout << "\n\n";
      
    return 0;
}


