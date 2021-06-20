#include <iostream>
#include <iostream>
#include <windows.h>
#include <locale>
#include <conio.h>
#include <limits>
#include <stack>
#include "functions.h"
/* Отрисовка опоры */
void draw_balk(int x, int y, int delta = 1) {
    int x_new = x; 
    int y_new = y;
    for (int i = 0; i < 10; i++) {
        console_gotoxy(x_new, y_new);
        std::cout << '|';
        y_new = y_new + delta;
    }
}
//template <typename T> 
class Disk {
public:
    int x_;
    int y_;
    int length_;
    Disk(int length, int x, int y) {
        draw(length, x, y);
    };
    Disk* previous_;
    /* Отрисовка диска */
    void draw(int length, int x, int y, int delta = 1) {
        char ch = 220;
        int x_new = x+1, y_new = y;
        length_ = length;
          
        for (int i = 0; i < length/2; i++) {
            console_gotoxy(x_new, y_new);
            std::cout << ch;
            x_new = x_new + delta;
        }
        x_new = x;
        for (int i = 0; i < length/2; i++) {
            x_new = x_new - delta;
            console_gotoxy(x_new, y_new);
            std::cout << ch;
        }
        x_ = x; y_ = y;         // Координаты диска 
        
    };
};

/* Класс стека */
class Stack {
    int max_size = 8;     // Максимальное кол-во элементов в стеке
    Disk* tos_ = nullptr;
    int count_;

public:
    int count_;
    Stack() {
        count_ = 0;
    };
@@ -108,12 +107,15 @@ class Game {
public:
    int x_ = 12;            // Координаты начала игры
    int y_;
    int size_;
    Stack* new_tower;

    Disk* d_ptr = nullptr;
    Stack* s_ptr = nullptr;
    Game(Stack *steck1) {
        s_ptr = steck1;
        s_ptr = steck1;         // Делаем так, чтобы игра начиналась с первой пирамидки
        DrawPointer(x_);
        size_ = s_ptr->count_;
    };

    void Move(Disk *disk_ch, int x, int y) {          // disk_ch - указатель на выбранный диск
@@ -140,25 +142,26 @@ class Game {

    }

    /* Указатель на выбранную башню */
    /* Создаем стрелочку, указывающая на выбранную башню */
    void DrawPointer(int x, int y = 17) {
        char ptr = 30;
        console_gotoxy(x, y);
        std::cout << ptr;
    }

        /* Убираем стрелочку */
    void RemovePointer(int x, int y = 17) {
        console_gotoxy(x, y);
        std::cout << ' ';
    }

    void Start(Stack *steck1, Stack *steck2, Stack *steck3) {
        new_tower = steck3;

        console_hide_cursor();
        WORD vk;
        bool isFinish = false;
        bool isUp = false;
        bool isDown = true;
        bool isUp = false;          // isUp означает, что диск подняли и его можно только опустить или перенести
        bool isDown = true;         // isUp - опустили 

        while (!isFinish) {
            if (_kbhit()) {
@@ -189,14 +192,14 @@ class Game {
                case VK_DOWN:
                    if (isDown)
                        break;

                    /* Опускаем вниз диск */
                    if (s_ptr != nullptr && d_ptr != nullptr) {
                        Disk *prev_disk = s_ptr->peek();
                        if (prev_disk == nullptr) {
                            int len = d_ptr->length_;
                            int x_new = d_ptr->x_; int y_new = d_ptr->y_;
                            ReMove(d_ptr, x_, y_);
                            s_ptr->push(len, x_, 15);       // Опускаем в самый низ
                            s_ptr->push(len, x_, 15);       
                            //s_ptr = nullptr;
                            d_ptr = nullptr;
                            isDown = true;
@@ -213,7 +216,7 @@ class Game {
                                //std::cout << "DOWN";
                                int x_new = prev_disk->x_; int y_new = prev_disk->y_;
                                ReMove(d_ptr, x_, y_);
                                x_ = x_new; y_ = y_new - 1;             // говницо получается
                                x_ = x_new; y_ = y_new - 1;             
                                Move(d_ptr, x_, y_);
                                s_ptr->push(len, x_, y_);
                                d_ptr = nullptr;
@@ -305,14 +308,17 @@ class Game {
                    console_gotoxy(12, 4);
                    std::cout << " \t\t\t\tGAME OVER ";
                    isFinish = true;
                    break;
                break;
                }
            }
            int new_size = new_tower->count_;
            if (size_ == new_size) {
                isFinish = true;
                console_gotoxy(12, 4);
                std::cout << " \t\t\t\tYOU WIN ";
            }
        } 
    }



};

int main()
{    
    int disks_num;
    system("cls");
    std::cout << "Enter number of disks 3-8: ";
    std::cin >> disks_num;
    if (disks_num < 3) {
        std::cout << "Enter more disks";
        return 0;
    }
    if (disks_num > 8) {
        std::cout << "Too much";
        return 0;
    }
    
    /* Отрисовка балок */
    draw_balk(12, 6);
    draw_balk(32, 6);
    draw_balk(52, 6);
    /* создание стека для первой балки, пирамиды*/
     Stack *s1 = new Stack;
    Tower t_1 (disks_num, s1);       // Помещаем в первый стек исходную пирамиду
    /* создание двух других стеков для оставшихся балок*/
    Stack* s2 = new Stack;
    Stack* s3 = new Stack;
    
    std::cout << std::endl;
    Game game(s1);
    game.Start(s1, s2, s3);
}
