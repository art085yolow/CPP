#include "UserInterface.h"

UserInterface::UserInterface()
{
    this->m_console = GetConsoleWindow();

    GetWindowRect(this->m_console, &this->m_r);

    MoveWindow(this->m_console, 50, 50, this->m_width_window, this->m_height_window, TRUE);

    this->m_lines.resize( Titles_Names::f_max_line + 1 );

    // width of line is 90, REMEMBER system count from 0!!! 0->89 = 90
    //int width_char = 89;
    char c = 205; // frame characters
    std::vector<std::string> s_frame (2);
    s_frame[0].resize(Titles_Names::f_width_char+1, ' ');
    s_frame[1].resize(Titles_Names::f_width_char+1, c);
    
    c = 186;
    
    for (auto& s : s_frame)
    {
       // s.replace(0, 1, "|");
       // s.replace(Titles_Names::f_width_char, 1, "|");

        
        s[0] = c;
        s[Titles_Names::f_width_char] = c;
        
    }

    for (size_t i = 0; i < this->m_lines.size(); i++)
    {

        switch (i) // line numbers // del after
        {
        case Titles_Names::f_path_tree:
            c = 201;
            s_frame[1][0] = c;
            c = 187;
            s_frame[1][Titles_Names::f_width_char] = c;
            this->m_lines[i] = s_frame[1];
            break;
        case Titles_Names::f_title:
        case Titles_Names::f_tag:
        case Titles_Names::f_description:
            c = 204;
            s_frame[1][0] = c;
            c = 185;
            s_frame[1][Titles_Names::f_width_char] = c;
            this->m_lines[i] = s_frame[1];
            break;
        case Titles_Names::f_max_line:
            c = 200;
            s_frame[1][0] = c;
            c = 188;
            s_frame[1][Titles_Names::f_width_char] = c;
            this->m_lines[i] = s_frame[1];
            break;

        default:
            this->m_lines[i] = s_frame[0];
            break;
        };

    }

    // for interating enum
    std::initializer_list<Titles_Names> Titles_Names_list = { f_path_tree, f_title, f_tag , f_description };


    for (auto e : Titles_Names_list)
    {

        std::string sub_title{ "" };

        switch (e) // titles
        {
        case Titles_Names::f_path_tree:
            sub_title = " Path Tree ";
            break;
        case Titles_Names::f_title:
            sub_title = " Title ";
            break;
        case Titles_Names::f_tag:
            sub_title = " TAGS ";
            break;
        case Titles_Names::f_description:
            sub_title = " Description ";
            break;
        }

        this->m_lines[e].replace(Titles_Names::f_width_char / 2 - sub_title.length() / 2, sub_title.length(), sub_title);
        
    }

    for (auto e : Titles_Names_list) 
    {
        switch (e)
        {
        case f_path_tree:
            c = 30;
            this->m_lines[e+1][Titles_Names::f_sign_char_updown_position] = c;
            c = 31;
            this->m_lines[Titles_Names::f_title-1][Titles_Names::f_sign_char_updown_position] = c;
            break;
        case f_title:
            c = 30;
            this->m_lines[e + 1][Titles_Names::f_sign_char_updown_position] = c;
            c = 31;
            this->m_lines[Titles_Names::f_tag - 1][Titles_Names::f_sign_char_updown_position] = c;
            break;
        case f_tag:
            c = 30;
            this->m_lines[e + 1][Titles_Names::f_sign_char_updown_position] = c;
            c = 31;
            this->m_lines[Titles_Names::f_description - 1][Titles_Names::f_sign_char_updown_position] = c;
            break;
        case f_description:
            c = 30;
            this->m_lines[e + 1][Titles_Names::f_sign_char_updown_position] = c;
            c = 31;
            this->m_lines[Titles_Names::f_max_line - 1][Titles_Names::f_sign_char_updown_position] = c;
            break;
        }
    }
    
   // for (size_t i = 0; i < this->m_lines.size(); i++)
   // {
   //
   //     switch (i) // line numbers // del after
   //     {
   //     case Titles_Names::f_path_tree:
   //     case Titles_Names::f_title:
   //     case Titles_Names::f_tag:
   //     case Titles_Names::f_description:
   //     case Titles_Names::f_max_line:
   //         break;
   //
   //     default:
   //         this->m_lines[i].replace(Titles_Names::f_width_char - std::to_string(i).length()- 1 , std::to_string(i).length(), std::to_string(i));
   //         break;
   //     };
   // }
}

void UserInterface::print()
{
    // system("cls");

    for (auto l : this->m_lines)
    {
        std::cout << l;
    }
    std::cout << "\n Write command:\n";
}
