#include <bits/stdc++.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <conio.h>
#include <windows.h>

using namespace std;

//macro for maximum length of snake
#define MAX_LENGTH 1000

const char DIR_UP = 'U';
const char DIR_DOWN = 'D';
const char DIR_LEFT = 'L';
const char DIR_RIGHT = 'R';

int consoleWidth, consoleHeight;

struct Point{
    int xCoord;
    int yCoord;
    Point(){}
    Point(int x, int y){
        xCoord = x;
        yCoord = y;
    }
};


//to get the initial width and height of the console
void initialScreen(){
    HANDLE hConSole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConSole, &csbi);
    consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top;
    consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left;
}


class Snake{
    int length; //current length of snake
    char direction; //current direction of snake
    public:

    Point body[MAX_LENGTH];

    Snake(int x, int y){
        length = 1;
        body[0] = Point(x,y);
        direction = DIR_RIGHT; //default staring direction
    }

    ~Snake(){
        delete[] body;
    }

    int getLength(){
        return length;
    }

    //& condition is applied because the snake can't just reverse its direction
    void changeDirection(char newDirection){
        if(newDirection == DIR_UP && direction != DIR_DOWN){
            direction = newDirection;
        }else if(newDirection == DIR_DOWN && direction != DIR_UP){
            direction = newDirection;
        }else if(newDirection == DIR_LEFT && direction != DIR_RIGHT){
            direction = newDirection;
        }else if(newDirection == DIR_RIGHT && direction != DIR_LEFT){
            direction = newDirection;
        }
    }

    //moving the snake 1 element towards the selected location
    bool move(Point food){
        for(int i = length-1; i>0; i--){
            body[i] = body[i-1];
        }

        //defining/changing axis on which snake will move when the direction is changed
        switch(direction){
            int val;
            case DIR_UP:
                val = body[0].yCoord;
                body[0].yCoord = val - 1;
                break;
            case DIR_DOWN:
                val = body[0].yCoord;
                body[0].yCoord = val + 1;
                break;
            case DIR_RIGHT:
                val = body[0].xCoord;
                body[0].xCoord = val + 1;
                break;
            case DIR_LEFT:
                val = body[0].xCoord;
                body[0].xCoord = val - 1;
                break;
        }


    //Checks if snake's head has colided with any part of its body
        for(int i = 1; i < length; i++){
            if(body[0].xCoord == body[i].xCoord && body[0].yCoord == body[i].yCoord){
                return false;
            }
        }

    //increasing length of snake when it eats food!
        if(food.xCoord == body[0].xCoord && food.yCoord == body[0].yCoord){
            body[length] = Point{body[length-1].xCoord, body[length-1].yCoord};
            length++;
        }

        return true;
    }
};


class Board{
    Snake *snake;
    const char snakeBody = 'O';
    Point food;
    const char Food = 'f';
    int score;

    public:
        Board(){
            spawnFood();
            snake = new Snake(10,10);
            score = 0;
        }

        ~Board(){
            delete snake;
        }

        int getScore(){
            return score;
        }

        void spawnFood(){
            int x = rand() % consoleWidth;
            int y = rand() % consoleHeight;
            food = Point(x,y);
        }

        void gotoxy(int x, int y){

            COORD coord; //windowsAPI windows.h structure
            coord.X = x;
            coord.Y = y;

            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); //to draw a point on a console screen/window
        }

        void draw(){
            system("cls"); //refreshes screen/frame
            for(int i = 0; i < snake->getLength(); i++){
                gotoxy(snake->body[i].xCoord, snake->body[i].yCoord); //drawing snake body
                cout<<snakeBody;
            }

            gotoxy(food.xCoord, food.yCoord); //drawing food at random points
            cout<<Food;
        }

        bool update(){
            bool isAlive = snake->move(food);
            if(isAlive == false){
                return false;
            }
            return true;

            if(food.xCoord == snake->body[0].xCoord && food.yCoord == snake->body[0].yCoord){
                score++;
                spawnFood();
            }
        }

        //detecting key presses
        void detectKeys(){
            if(kbhit()){
                int key = getch();
                if(key == 'w'){
                    snake->changeDirection(DIR_UP);
                }
                if(key == 'a'){
                    snake->changeDirection(DIR_LEFT);
                }
                if(key == 's'){
                    snake->changeDirection(DIR_DOWN);
                }
                if(key == 'd'){
                    snake->changeDirection(DIR_RIGHT);
                }
            }
        }
};


int main(){
    srand(time(0));
    initialScreen();
    Board *board = new Board{};
    while(board->update()){
        board->detectKeys();
        board->draw();
        Sleep(200); //speed of snake
    }
    cout<<"GAME OVER!!";
    return 0;
}
