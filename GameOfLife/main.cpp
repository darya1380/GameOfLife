#include <iostream>
#include <set>
#include "clui.h"
#include "Darya.h"
using namespace Darya;
void menu_users(user *_user) {
    Style_Game styleGame;
    COLORS colors;
    cout << "1) new game" << endl;
    cout << "2) saved game" << endl;
    cout << "3) change game style" << endl;
    cout << "4) change password" << endl;

    int input;
    cin >> input;

    switch (input)
    {
        case 1: new_game(_user);
            break;
        case 2: saved_game(_user);
            break;
        case 3: change_game_style(&styleGame, &colors,_user);
            break;
        case 4: change_password(_user);
            break;
        default: menu_users(_user);
            break;
    }
}

//user kinds
void new_user()
{
    user * new_user = new user();
    assign_username(new_user);
    assign_password(new_user);
    menu_users(new_user);
}

bool ask_password() {
    cout << "please enter your username" << endl;
    char user_name[20];
    cin >> user_name;

    cout << "please enter your password" << endl;
    char password[20];
    cin >> password;

    chdir(user_name);
    char main_pass[20];
    FILE * fp = fopen("password.txt","r");
    fscanf(fp,"%s",main_pass);
    bool res = true;
    for(int i = 0; i < strlen(main_pass); i++)
        if(main_pass[i] != password[i])
        {
            chdir("..");
            res = false;
        }

    return res;
}

void old_user()
{
    user * old_user_ = new user();
    bool result = ask_password();
    if(result)
        menu_users(old_user_);
    else
    {
        cout << "your password was incorrect" << endl;
        clear_screen();
        old_user();
    }
}

void guest_user()
{
    /*
     * a guest user doesn't have the opportunity to save a data
     * he or she can just play this game for once
     * and if press s will be directly moved to the main menu
     * So no ability to change style-color-save-.. :-(
     * ANd the game will start immediately
     */
    user * guest = new user();
    new_game(guest, false, nullptr, false);
}


void main_menu()
{
    change_color_rgb(204, 0, 153);
    cout << "Hello Welcome to Game Of Life" << endl;
    cout << "1) New User" << endl;
    cout << "2) Old User" << endl;
    cout << "3) Guest User"<< endl;
    reset_color();

    int input = 0;
    cin >> input;
    switch(input)
    {
        case 1: new_user();
            break;
        case 2: old_user();
            break;
        case 3: guest_user();
            break;
        default: main_menu();
            break;
    }


}

int main()
{
    init_clui();
    main_menu();
}

