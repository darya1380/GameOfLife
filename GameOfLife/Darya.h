//
// Created by darya on 7/13/2020.
//

#ifndef MYGAME_DARYA_H
#define MYGAME_DARYA_H

#endif //MYGAME_DARYA_H

#include <iostream>
#include <dirent.h>
#include <ctime>
#include <unistd.h>
using namespace std;
struct Style_Game{
    char gaming_char_true = '1';
    char gaming_char_false = '0';
    bool show_true_char = true;
    bool show_false_char = true;
    bool has_heal_option = true;
    string motivational_sentence = "Dream bigger. Do bigger.";
};

struct COLORS
{

    struct COLOR
    {
        int r;
        int g;
        int b;
    };
    COLOR unstable_born{
            255,255,0 //yellow
    };
    COLOR unstable_heal{
            83,255,26 //light green
    };
    COLOR unstable_death{
            204,0,0  //somehow red
    };
    COLOR stable_heal_dead{
            153,153,102 //light brown
    };
    COLOR stable_die{
            0,0,204   //dark blue
    };
    COLOR stable_born{
            255,153,0 //orange
    };
};

struct user
{
    COLORS * colors{};
    Style_Game * styleGame{};
    char* user_name;
    char *password;

    user()
    {
        colors = new COLORS();
        styleGame = new Style_Game();
        user_name = new char[20];
        password = new char[20];
    }

};

struct point
{
public:
    bool value ;
    bool stable;
    bool heal;
    bool born;
    point() //constructor
    {
        switch(rand() % 5)
        {
            case 0: value = true;
                break;
            case 1: value = false;
                break;
            case 2: value = false;
                break;
            case 3: value = true;
                break;
            case 4: value = false;
                break;
        }
        stable = true;
        heal  = false;
        born = false;
    }
};



void menu_users(user *_user);
void save_current_game(FILE *file, double diff_time, int sleep_time, point **world, user * user);
namespace Darya
{
    static void current_directories()
    {
        dirent * dirent;
        DIR * dir = opendir("..");
        while ((dirent = readdir(dir)) != nullptr)
            cout << dirent -> d_name << endl;
    }

    static bool scan_input() {
        char *dir_name = new char[30];
        cin >> dir_name;
        dirent * dirent;
        DIR * dir = opendir("..");
        set<string>dir_names;
        while ((dirent = readdir(dir)) != nullptr)
            dir_names.insert(dirent->d_name);
        if(dir_names.find(dir_name) != dir_names.end())
        {
//           we creat a set of names and if this name does exist we ask for password
//            if password was right we let user in(return true)
            cout << "good..now you should enter password for the current directory" << endl;
            chdir(dir_name);
            FILE * file_password = fopen("password.txt", "r");
            char *main_password = new char[30];
            fscanf(file_password,"%s",main_password);
            char user_pass[30];
            cin >> user_pass;
            bool res = true;
            for(int i = 0; i < strlen(main_password); i++)
                if(user_pass[i] != main_password[i])
                    res = false;

            return res;

        } else
        {
            scan_input();
        }
        return false;
    }



    //some functions


    void assign_username(user *new_user)
    {
        cout << "please enter your user name" << endl;
        cout << "a username must contain between 5 to 20 characters" << endl;
        char *username = (char*)malloc(20 * sizeof(char));
        cin >> username;
        for(int i = 0; i < strlen(username); i++)
            new_user -> user_name[i] = username[i];
        mkdir(username);
        chdir(username);
    }

    void assign_password(user *new_user)
    {
        cout << "now you should creat a password" << endl;
        cout << "a password must contain between 5 to 20 characters" << endl;
        cout << "including at least 1 lowercase letter" << endl;
        char * pass = (char*)malloc(20 * sizeof(char));
        cin >> pass;
        bool res = false;
        for(int i = 0; pass[i] != '\0'; i++)
            if(islower(pass[i]))
                res = true;

        if(res)
        {
            for(int i = 0; i < strlen(pass); i++)
                new_user -> password[i] = pass[i];
            FILE * fp =  fopen("password.txt", "w");
            fprintf(fp,"%s",new_user->password);
        }

        else
        {
            cout << "password validation failed" << endl;
            assign_password(new_user);
        }
    }

    FILE * creat_game_file(user * user)
    {
        cout << "enter your game file name" << endl;
        cout << "a filename must contain between 10-20 characters and it mustn't be used in this directory(a list of current directories is given)" << endl;
        dirent * dirent;
        DIR * dir = opendir(user->user_name);
        while ((dirent = readdir(dir)) != nullptr)
            cout << dirent->d_name << endl;
        char * game_dir_name = new char[20];
        cin >> game_dir_name;
        if(strlen(game_dir_name) < 10)
        {
            cout << "the directory name too short" << endl;
            creat_game_file(user);
        }
        else
        {
            cout << "good! now we creat your game directory and your game file" << endl;
            mkdir(game_dir_name);
            chdir(game_dir_name);
            FILE * fp = fopen("game.txt", "w");
            return fp;
        }
        return nullptr;
    }//fine

    void show_rules()
    {
        cout << "welcome to rules page" << endl;
        cout << "use s key to save current game" << endl;
        cout << "use + key to higher the gaming speed" << endl;
        cout << "use - key to lower the gaming speed" << endl;
        cout << "use from 1-9 key to go some levels further" << endl;
        cout << "use p to pause the current game" << endl;
        cout << "use h key to show the rule page again" << endl;
    }

    bool is_i_j_valid(int i, int j)
    {
        return((i >= 0 && i < get_window_rows() && j >= 0 && j < get_window_cols()));
    }

    int count_neighbors(int i, int j,point ** main_point)
    {
        int counter = 0;
        if(is_i_j_valid(i - 1, j) && main_point[i - 1][j].value)
            counter++;
        if(is_i_j_valid(i + 1, j) && main_point[i + 1][j].value)
            counter++;
        if(is_i_j_valid(i, j + 1) && main_point[i][j + 1].value)
            counter++;
        if(is_i_j_valid(i, j - 1) && main_point[i][j - 1].value)
            counter++;
        if(is_i_j_valid(i - 1, j - 1) && main_point[i - 1][j - 1].value)
            counter++;
        if(is_i_j_valid(i - 1, j + 1) && main_point[i - 1][j + 1].value)
            counter++;
        if(is_i_j_valid(i + 1, j - 1) && main_point[i + 1][j - 1].value)
            counter++;
        if(is_i_j_valid(i + 1, j + 1) && main_point[i + 1][j + 1].value)
            counter++;
        return counter;

    }

    void evaluate(point ** main_point,user * user, bool print_needed = true)
    {

/*
 * a problem was detected! what would happen if all of main point values are 0???!!
 *          0   0   0   0   0
 *          0   0   0   0   0----
 *          0   0   0   0   0
 *          |
 *          |
 *          |
 *          |
 * actually the game would be BORING! that's why we check and if all list contains only 0 (or 1:))...
 * we reinitialize it and make it so better! :)
 *
 * the Next question..where to store data?!
 * a set OR multiset OR dequeue OR map OR stack OR...
 * we don't care about the order! we need some structure with contain method! our problem is simple
 * we use set
 */

//define set and initialize it
        set <int> main_point_value;
        for(int i = 0; i < get_window_rows(); i++)
            for(int j = 0; j < get_window_cols(); j++)
                main_point_value.insert(main_point[i][j].value);


        if((main_point_value.find(1) == main_point_value.end()) || (main_point_value.find(0) == main_point_value.end()))
        {
            cout << " Reinitialize World.... " << endl;
            delete main_point;
            main_point = new point *[get_window_rows()];
            for(int i = 0; i < get_window_rows(); i++)
                main_point[i] = new point[get_window_cols()];
        }

        cout << user->styleGame->motivational_sentence << endl << endl;
        for(int i = 0; i < get_window_rows(); i++)
            for(int j = 0; j < get_window_cols(); j++) {

                if (user->styleGame->has_heal_option) {

                    {

                        if (!main_point[i][j].stable) {
                            if (main_point[i][j].born) {
                                //unstable born
                                if (main_point[i][j].value == 0) {
                                    change_color_rgb(user->colors->unstable_born.r, user->colors->unstable_born.g,
                                                     user->colors->unstable_born.b);
                                    main_point[i][j].value = true;
                                }
                                    //unstable_heal
                                else {
                                    change_color_rgb(user->colors->unstable_heal.r, user->colors->unstable_heal.g,
                                                     user->colors->unstable_heal.b);
                                    main_point[i][j].heal = true;
                                }
                                main_point[i][j].born = false;

                            } else {
                                //unstable_dead
                                change_color_rgb(user->colors->unstable_death.r, user->colors->unstable_death.g,
                                                 user->colors->unstable_death.b);
                                main_point[i][j].value = false;
                            }

                            main_point[i][j].stable = true;

                        } else if (count_neighbors(i, j, main_point) <= 2 || count_neighbors(i, j, main_point) >= 4) {

                            if (main_point[i][j].heal) {
                                change_color_rgb(user->colors->stable_heal_dead.r, user->colors->stable_heal_dead.g,
                                                 user->colors->stable_heal_dead.b);
                                main_point[i][j].heal = false;
                            } else {
                                change_color_rgb(user->colors->stable_die.r, user->colors->stable_die.g,
                                                 user->colors->stable_die.b);
                                main_point[i][j].stable = false;
                            }
                        } else if (count_neighbors(i, j, main_point) == 3) {
                            change_color_rgb(user->colors->stable_born.r, user->colors->stable_born.g,
                                             user->colors->stable_born.b);
                            main_point[i][j].stable = false;
                            main_point[i][j].born = true;
                        }
                    }
                }
                else
                    {

                    if (!main_point[i][j].stable) {
                        //unstable_born
                        if (main_point[i][j].born) {
                            main_point[i][j].born = false;
                            main_point[i][j].value = true;
                            change_color_rgb(user->colors->unstable_born.r, user->colors->unstable_born.g,
                                             user->colors->unstable_born.b);
                        }
                            //unstable_dead
                        else {
                            main_point[i][j].value = false;
                            change_color_rgb(user->colors->unstable_death.r, user->colors->unstable_death.g,
                                             user->colors->unstable_death.b);
                        }
                        main_point[i][j].stable = true;
                    } else if (count_neighbors(i, j, main_point) == 3) {
                        //stable born
                        main_point[i][j].born = true;
                        main_point[i][j].stable = false;
                        change_color_rgb(user->colors->stable_born.r, user->colors->stable_born.g,
                                         user->colors->stable_born.b);
                    } else if (count_neighbors(i, j, main_point) >= 4 || count_neighbors(i, j, main_point) < 2) {
                        //stable die
                        main_point[i][j].stable = false;
                        change_color_rgb(user->colors->stable_die.r, user->colors->stable_die.g,
                                         user->colors->stable_die.b);
                    }
                }


                if (print_needed) {
                    if (main_point[i][j].value) {
                        if (user->styleGame->show_true_char) {
                            cout << user->styleGame->gaming_char_true << " ";
                        }
                    } else {
                        if (user->styleGame->show_false_char) {
                            cout << user->styleGame->gaming_char_false << " ";
                        }
                    }
                    if (j == get_window_cols() - 1)
                        cout << endl;
                }
            }

    }




    void new_game(user *user, bool is_saved = false, FILE * reader = nullptr, bool can_save = true)
    {
        FILE * game = creat_game_file(user);
        show_rules();
        clear_screen();
        bool gaming_state = true;
        auto **world = (point**)malloc(get_window_rows() * sizeof(point*));
        for(int i = 0; i < get_window_rows(); i++)
            world[i] = (point*)malloc(get_window_cols() * sizeof(point));

        time_t start, end;
        int sleep_time = 10;
        long long int diff_time = 0;
        time(&start);
        if(is_saved)
        {
            fscanf(reader,"%lld", &diff_time);
            fscanf(reader,"%d" , &sleep_time);

            for(int i = 0; i < get_window_rows(); i++)
                for(int j = 0; j < get_window_cols(); j++)
                {
                    int val,stable,born,heal;
                    fscanf(reader, "%d\n%d\n%d\n%d",&val,&stable,&born,&heal);
                    world[i][j].value = val;
                    world[i][j].heal = heal;
                    world[i][j].born = born;
                    world[i][j].stable = stable;
                }

        }



        while(gaming_state)
        {
            clear_screen();
            time(&end);
            cout << "second passed is  " << difftime(end, start) << endl;
            cout << "sleep time is  " << sleep_time << endl;
            evaluate(world,user);
            if(is_keyboard_hit())
            {
                char input = getch();
                if(input == 's')
                {
                    if(can_save)
                    {
                        save_current_game(game, difftime(end, start), sleep_time, world,user);
                        gaming_state = false;
                    }
                    else
                    {
                        exit(0);
                    }
                }
                else if (input == '+')
                {
                    if(sleep_time > 5)
                        sleep_time -= 5;
                }
                else if (input == '-')
                {
                    sleep_time += 5;
                }
                else if (input >= '1' && input <= '9')
                {
                    int i = input - '0';
                    for(int j = 0; j < i; j++)
                        evaluate(world,user, false);
                }
            }
            cout << "putting app on sleep for a short sleep time" << endl;
            sleep(sleep_time);
        }
    }



    void saved_game(user *user) {
        clear_screen();
        cout << "a list of current directories would be given to you enter your username"<< endl;
        current_directories();
        scan_input();
        FILE * game_file = fopen("game.txt","r");
        new_game(user,true,game_file);

    }


    void change_true_char(Style_Game *pGame)
    {
        clear_screen();
        cout << "enter the character to be as a true one" << endl;
        char c;
        cin >> c;
        pGame -> gaming_char_true = c;
    }

    void change_false_char(Style_Game *pGame)
    {
        clear_screen();
        cout << "enter the character to be as a false one" << endl;
        char c;
        cin >> c;
        pGame -> gaming_char_false = c;
    }

    void showing_true_char(Style_Game *pGame)
    {
        cout << "would you prefer true character 1)be_shown or 2)not ?" << endl;
        int input;
        cin >> input;
        switch (input)
        {
            case 1: pGame -> show_true_char = true;
                break;
            case 2: pGame->show_true_char = false;
                break;
            default: showing_true_char(pGame);
        }
    }

    void showing_false_char(Style_Game *pGame)
    {
        clear_screen();
        cout << "would you prefer false character 1)be_shown or 2)not ?" << endl;
        int input;
        cin >> input;
        switch (input)
        {
            case 1: pGame -> show_false_char = true;
                break;
            case 2: pGame->show_false_char = false;
                break;
            default: showing_false_char(pGame);
                break;
        }
    }

    void change_motiv_sentence(Style_Game *pGame)
    {
        clear_screen();
        cout << "motivation has always been so important to humanity!" << endl;
        cout << "so you should always have it!" << endl;
        cout << "enter your new motivational sentence" << endl;
        getline(cin,pGame -> motivational_sentence);
    }

    void change_heal_option(Style_Game *pGame)
    {
        clear_screen();
        cout << "1) with heal" << endl;
        cout << "2) without heal" << endl;
        int input;
        cin >> input;
        switch (input)
        {
            case 1: pGame->has_heal_option = true;
                break;
            case 2: pGame->has_heal_option = false;
                break;
            default: change_heal_option(pGame);
                break;
        }
    }

    void CHANGE_GAME_STYLE(Style_Game *pGame)
    {
        clear_screen();
        Style_Game styleGame;
        cout << "creat your own style!" << endl;
        cout << "1)change true character" << endl;
        cout << "2) change false character" << endl;
        cout << "3) change the true character show" << endl;
        cout << "4) change the false character show" << endl;
        cout << "5) change motivational sentence" << endl;
        cout << "6) change the power! with or without heal?" << endl;

        int input;
        cin >> input;
        switch (input)
        {
            case 1:change_true_char(pGame);
                break;
            case 2:change_false_char(pGame);
                break;
            case 3:showing_true_char(pGame);
                break;
            case 4:showing_false_char(pGame);
               break;
            case 5:change_motiv_sentence(pGame);
                break;
            case 6:change_heal_option(pGame);
                break;
            default: CHANGE_GAME_STYLE(pGame);

        }

    }

    bool is_rgb_valid(int r)
    {
        return (r >= 0 && r <= 255);
    }

    void CHANGE_COLOR(COLORS::COLOR color)
    {
        cout << "enter the RGB you want to change current RGB to" << endl;
        int R,G,B;
        cin >> R >> G >> B;
        if(is_rgb_valid(R) && is_rgb_valid(G) && is_rgb_valid(B))
        {
            cout << "Great! your color will be simply changed" << endl;
        }

    }

    void CHANGE_GAME_COLOR(COLORS *pColors)
    {
        clear_screen();
        cout << "1) change color of stable born" << endl;
        cout << "2) change color of stable die" << endl;
        cout << "3) change color of the node who's heal is being damaged" << endl;
        cout << "4) change color of unstable born(born in this round)" << endl;
        cout << "5) change color of unstable death" << endl;
        cout << "6) change color of unstable heal" << endl;


        int input;
        cin >> input;
        switch (input)
        {
            case 1: CHANGE_COLOR(pColors->stable_born);
            break;
            case 2: CHANGE_COLOR(pColors->stable_die);
            break;
            case 3: CHANGE_COLOR(pColors->stable_heal_dead);
            break;
            case 5:CHANGE_COLOR(pColors->unstable_death);
            break;
            case 6:CHANGE_COLOR(pColors->unstable_heal);
            break;

        }
    }

    void change_game_style(Style_Game *pstyleGame, COLORS *pColors, user * puser)
    {
        puser->colors = pColors;
        puser->styleGame = pstyleGame;
        cout << "1)change style" << endl;
        cout <<"2)change color"  << endl;
        int input;
        cin >> input;
        switch (input)
        {
            case 1:
                CHANGE_GAME_STYLE(pstyleGame);
                break;
            case 2:
                CHANGE_GAME_COLOR(pColors);
                break;
            default:
                change_game_style(pstyleGame, pColors,puser);
                break;
        }
    }


    void change_password(user *pUser) {
        cout << "enter your previous password" << endl;//todo
        char *pre_pass = new char[20];
        cin >> pre_pass;
        bool result = true;
        for(int i = 0; i < strlen(pUser->password); i++)
            if(pre_pass[i] != pUser->password[i])
                result = false;
        if(result)
        {
            cout << "great! now enter your new password"<< endl;
            assign_password(pUser);
        } else
        {
            cout << "the password was incorrect" << endl;
            change_password(pUser);
        }
    }

}
void save_current_game(FILE *file, double diff_time, int sleep_time, point **world, user * user)
{
    clear_screen();
    cout << "hello this is the end of game" << endl;
    cout << "you will be directly lead to your user menu" << endl;
    file = fopen("game.txt", "w");
    fprintf(file,"%f\n",diff_time);
    fprintf(file, "%d\n", sleep_time);
    for(int i = 0; i < get_window_rows(); i++)
        for(int j = 0; j < get_window_cols(); j++)
            fprintf(file,"%d\n%d\n%d\n%d\n", world[i][j].value, world[i][j].stable, world[i][j].heal, world[i][j].born);
    menu_users(user);

}
