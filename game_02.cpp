#include <chrono>
#include <thread>
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <iostream>
#include <vector>
    //GameSnakePushka Перед використанням програми переконайтесь , що у Вас стоїть англ. клавіатура , без caps lock.

    //Для управління в меню використовуйте : W - вгору , S - вниз , ENTER - наступна опція, ESC - попередня опція.

    //Для управління в ігровому полі використовуйте : W - вгору , S - вниз , A - вліво , S - вправо.

    // --- глобальні змінні та константи --- //
constexpr auto MAGICNUM = -32;

constexpr auto ENTER = 13;            constexpr auto ESC = 27;
constexpr auto UP = 119;              constexpr auto DOWN = 115;

constexpr auto ColorStandart = 2;     constexpr auto ColorStandartActive = 10;
constexpr auto ColorUkrainian = 1;    constexpr auto ColorUkrainianActive = 6;
constexpr auto ColorPushka = 228;     constexpr auto ColorPushkaActive = 237;
constexpr auto ColorImpossible = 155; constexpr auto ColorImpossibleActive = 153;

int Color = ColorUkrainian;           int ColorActive = ColorUkrainianActive;
const int x_max = 100;                const int y_max = 25;
int gameResult = 0;

HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

std::vector<int> records{};

    // --- глобальні змінніта константи --- //

    // --- об'явлення функцій для керування ігровим середовищем --- //
void startGame();
void goToXY(int x, int y);
void CursorVisible(bool visible);
    // --- об'явлення функцій для керування ігровим середовищем --- //
    // --- клас меню --- //
class Menu {
    char ch = ' ';
    std::string MenuNames[4] = { "New Game","Records","Settings","Out" };
    std::string SettingsNames[5] = { "Standart Theme","Ukrainian Theme","Pushka Game Theme","Impossible Game Theme (pls, do not use, if you are not true Hardcore Gamer)" };
    int activeMenu = 0;
    enum MenuItems {
        NEWGAME,
        RECORDS,
        SETTINGS,
        EXITGAME
    };
private:
public:
   
    void showRecords() {
        while (true)
        {
            system("CLS");
            int x = 50;int y = 10;
            goToXY(x + 1, y - 2);
            std::cout << "Your records :  " << std::endl;
            goToXY(x, y);
            for (int i = 0; i < size(records); i++)
                for (int j = i; j < size(records); j++)
                    if (records[j] > records[i])
                        std::swap(records[j], records[i]);
            int i = 1;
            for (int n : records)
            {
                goToXY(x, y++);
                std::cout << i << ". Game score : " << n << std::endl;
                i++;
            }
            goToXY(x - 4, y + 4);
            std::cout << "to return to the main menu press ESC" << std::endl;
            ch = _getch();
            if (ch == ESC) {
                system("CLS");
                mainMenuLoop();
            }
        }
    };
    void showSettings() {
        while(true){
        system("CLS");
        int x = 50;int y = 10;
        goToXY(x, y);
        for (int i = 0; i < size(SettingsNames); i++)
        {
            if (i == activeMenu)SetConsoleTextAttribute(handle, Color);
            else SetConsoleTextAttribute(handle, ColorActive);
            goToXY(x, y++);
            std::cout << SettingsNames[i] << std::endl;
        }
        goToXY(x - 3, y + 1);
        std::cout << "to return to the main menu press ESC"<< std::endl;
        ch = _getch();
        if (ch == MAGICNUM) ch = _getch();
        switch (ch) {
        case ESC:
            system("CLS");
            mainMenuLoop();
        case UP:
            if (activeMenu > 0)
                --activeMenu;
            break;
        case DOWN:
            if (activeMenu < size(SettingsNames) - 1)
                ++activeMenu;
            break;
        case ENTER:
            switch (activeMenu)
            {
            case 0:
                ColorActive = ColorStandartActive;
                Color = ColorStandart;
                break;
            case 1:
                ColorActive = ColorUkrainianActive;
                Color = ColorUkrainian;
                break;
            case 2:
                ColorActive = ColorPushka;
                Color = ColorPushkaActive;
                break;
            case 3:
                ColorActive = ColorImpossibleActive;
                Color = ColorImpossible;
                break;
            default:
                break;
            }
            break;
        }

    }
}
    void mainMenuLoop() {
    while (true)
    {
        SetConsoleTextAttribute(handle, Color);SetConsoleTextAttribute(handle, ColorActive);
        int x = 50;int y = 10;
        goToXY(x - 10, y - 2);
        std::cout << "Click Enter to new game, last Score : " << gameResult << std::endl;
        goToXY(x + 1, y);
        for (int i = 0; i < size(MenuNames); i++)
        {
            if (i == activeMenu)SetConsoleTextAttribute(handle, Color);
            else SetConsoleTextAttribute(handle, ColorActive);
            goToXY(x, y++);
            std::cout << MenuNames[i] << std::endl;
        }
        goToXY(x - 3, y + 1);
        ch = _getch();
        if (ch == MAGICNUM) ch = _getch();
        switch (ch)
        {
        case ESC:
            exit(0);
        case UP:
            if (activeMenu > 0)
                --activeMenu;
            break;
        case DOWN:
            if (activeMenu < size(MenuNames) - 1)
                ++activeMenu;
            break;
        case ENTER:
            switch (activeMenu)
            {
            case NEWGAME:
                system("CLS");
                gameResult = 0;
                startGame();
                system("CLS");
                break;
            case RECORDS:
                showRecords();
                break;
            case SETTINGS:
                showSettings();
                break;
            case EXITGAME:
                exit(0);
            }
            break;

        }
    }

}
};
    // --- клас меню --- //
    // --- класи Field та Snake та додатквоі структури --- //
enum Rotation
{
    left,
    right,
    up,
    down
};
struct Position
{
    int x;
    int y;
};
class Field
{
public:
    char field[y_max][x_max + 1];
    void CreateBoundaries(char ch);
    void Show();
    void SpownPrice();
    void ShowScore();
};
class Snake
{
private:
    int length;
    Position snakePos[100];
    Rotation snakeRot;

public:
    Snake(int length, Rotation snakeRot) :
        length(length), snakeRot(snakeRot) {};

    void startValues();
    void move(char field[y_max][x_max + 1]);
    void addTail();
    bool isGameOver();
};

    // --- класи Field та Snake та додатквоі структури --- //
    // --- main --- //
int main()
{
    srand(time(NULL));
    CursorVisible(false);
    Menu menu;
    menu.mainMenuLoop();
}
    // --- main --- //
    // --- реалізація методів Field  --- // 
void Field::CreateBoundaries(char ch)
{
    for (int y = 0; y < y_max; y++)
    {
        for (int x = 0; x < x_max; x++)
        {
            if (y == 0 || y == y_max - 1 ||
                x == 0 || x == x_max - 1)
            {
                field[y][x] = ch;
            }
            else
            {
                field[y][x] = ' ';
            }
        }
        field[y][x_max] = '\0';
    }
}
void Field::Show()
{
    goToXY(0, 0);
    for (int y = 0; y < y_max; y++)
        std::cout << field[y] << std::endl;
}
void Field::ShowScore()
{
  
   
   
     goToXY(103, 5);
     std::cout << "Game by Igor Y.";
     goToXY(103, 6);
     std::cout << "Game Score : " << gameResult;
     goToXY(103, 8);
     std::cout << "You must pick up - @";
     goToXY(103,9);
     std::cout << "Avoid boundaries and";
     goToXY(103, 10);
     std::cout << "pick up all apples to win";
}
void Field::SpownPrice()
{
    int x = rand() % (x_max - 2) + 1;
    int y = rand() % (y_max - 2) + 1;
    bool isSpown = false;
    while (true)
    {
        if (x % 2 == 0) isSpown = true;
        if (field[y][x] == 'O' || field[y][x] == '*') isSpown = false;
        if (isSpown) break;

        x = rand() % (x_max - 2) + 1;
    }

    field[y][x] = '@';
}
    // --- реалізація методів Field  --- // 
    // --- реалізація методів Snake  --- // 
void Snake::startValues()
{
    for (int i = 0; i < length; i++)
    {
        snakePos[i].x = 0;
        snakePos[i].y = 0;
    }
    length = 1;
    snakePos[0] = { x_max / 2, y_max / 2 };
    goToXY(snakePos[0].x, snakePos[0].y);
    std::cout<<'O';
}
void Snake::move(char field[y_max][x_max + 1])
{
    if (GetAsyncKeyState('W'))
    {
        snakeRot = up;
    }
    else if (GetAsyncKeyState('S'))
    {
        snakeRot = down;
    }
    else if (GetAsyncKeyState('A'))
    {
        snakeRot = left;
    }
    else if (GetAsyncKeyState('D'))
    {
        snakeRot = right;
    }

    field[snakePos[length - 1].y][snakePos[length - 1].x] = ' ';

    for (int i = length; i > 0; i--)
    {
        snakePos[i] = snakePos[i - 1];
    }

    switch (snakeRot)
    {
    case up:
        snakePos[0].y = snakePos[0].y - 1;
        break;

    case down:
        snakePos[0].y = snakePos[0].y + 1;
        break;

    case left:
        snakePos[0].x = snakePos[0].x - 2;
        break;

    case right:
        snakePos[0].x = snakePos[0].x + 2;
        break;
    }

    if (field[snakePos[0].y][snakePos[0].x] == '@') addTail();

    field[snakePos[0].y][snakePos[0].x] = 'o';
    if (length > 1)
       field[snakePos[1].y][snakePos[1].x] = '*';
    
}
void Snake::addTail()
{
    snakePos[length].x = snakePos[length - 1].x;
    snakePos[length].y = snakePos[length - 1].y;
    length++;
    gameResult++;
}
bool Snake::isGameOver()
{
    bool isGameOver = false;
    if (snakePos[0].x <= 0 || snakePos[0].x >= x_max - 1 ||
        snakePos[0].y <= 0 || snakePos[0].y >= y_max - 1)
    {
        isGameOver = true;
    }
    for (int i = 2; i < length; i++)
    {
        if (snakePos[0].x == snakePos[i].x &&
            snakePos[0].y == snakePos[i].y)
        {
            isGameOver = true;
        }
    }
    return isGameOver;
}
    // --- реалізація методів Snake  --- // 
    // --- реалізація функцій для керування ігровим середовищем  --- //
void startGame()
{
    Field gameField;
    Snake snake(1, down);
    int charBoundar = 219;
    int speedGame = 150;
    int countApple = 30;
    gameField.CreateBoundaries(charBoundar);
    snake.startValues();
    for (int i = 0; i < countApple; i++)
        gameField.SpownPrice();
    while (true)
    {
        if (snake.isGameOver()) break;
        snake.move(gameField.field);

        gameField.Show();
        std::thread th1([&]() {
            gameField.ShowScore();
        }
        );


        th1.join();
        Sleep(speedGame);
    }
    records.push_back(gameResult);
}
void goToXY(int x, int y)
{
    COORD pos = { x, y };
    SetConsoleCursorPosition(handle, pos);
}
void CursorVisible(bool visible)
{
    CONSOLE_CURSOR_INFO structCursorInfo;
    GetConsoleCursorInfo(handle, &structCursorInfo);
    structCursorInfo.bVisible = visible;
    SetConsoleCursorInfo(handle, &structCursorInfo);
}
    // --- реалізація функцій для керування ігровим середовищем  --- //
