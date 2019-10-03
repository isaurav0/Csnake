#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h> 

bool gameOver;
const int width = 40;
const int height = 20;
int fruitX, fruitY, score;
float x, y;
int tailX[100], tailY[100];
int ntail;
enum direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum direction dir;


void Setup()
{
    gameOver = false;
    dir = LEFT;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
}

void Draw()
{
    clear();
    for (int i = 0; i < width; i++)
    {
        printw("_");
    }
    printw("\n");
    
    //draws boundaries, fruits and head
    for (int i = 0; i < height; i++)
    {
        for (float j = 0; j < width; j++)
        {
            if (j == 0 | j == width - 1)
                printw("|");
            else if (i == y && j == x)
                printw("@");
            else if (i == fruitY && j == fruitX)
            {
                printw("*");
            }
            else
            {
                bool print = false;
                for(int k=0; k<ntail;k++){
                    if(tailX[k]==j && tailY[k]==i)
                    {
                        printw("o");
                        print = true;
                    }   
                }
                if(!print){
                    printw(" ");
                }   
            }
                
        }
        printw("\n");
    }

    for (int i = 0; i < width; i++)
    {
        printw("-");
    }
    printw("\n");
    printw("Score: %d", score);
}

void Input()
{
    int c = 0;
    switch ((c = getch()))
    {
    case KEY_UP:
        dir = UP;
        break;
    case KEY_DOWN:
        dir = DOWN;
        break;
    case KEY_LEFT:
        dir = LEFT;
        break;
    case KEY_RIGHT:
        dir = RIGHT; 
        break;
    case 'q':
        gameOver = true;
    default:
        // dir = LEFT;
        break;
    }
}

void Logic()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < ntail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    
    switch(dir){
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        default:
            // gameOver = true;
            break;
    }

    if (x >= width-1 || x <= 0 || y >= height || y < 0)
        gameOver = true;
    
 
    if (x == fruitX && y == fruitY)
    {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        ntail++;
    }

    for (int i = 0; i < ntail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;

}

int main()
{
    initscr();
    float speed = 100000;
    if(score>20)
        speed*=1.10;
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    Setup();
    while (!gameOver)
    {
        Draw();
        Input();
        Logic();
        usleep(speed);
    }
    nodelay(stdscr, FALSE);
    Draw();
    mvprintw(10,13,"Game Over");
    getch();
    endwin();
}