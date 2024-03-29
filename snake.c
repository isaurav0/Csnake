#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h> 

bool gameOver;
int bodyX[100], bodyY[100];
int x, y;
const int width = 40;
const int height = 20;
int fruitX, fruitY, score;
int body_len;
int speed = 100000;
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
            // else if(i==height-1 && j>0 && j<width-1)
            //     printw("_");
            else
            {
                bool print = false;
                for(int k=0; k<body_len;k++){
                    if(bodyX[k]==j && bodyY[k]==i)
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
        printw("‾");
    }
    printw("\n");
    printw("Score: %d \n", score);
    printw("X -> %d : fruitX -> %d\n speed-> %d",x,fruitX, speed);
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
    int prevX = bodyX[0];
    int prevY = bodyY[0];
    int prev2X, prev2Y;
    bodyX[0] = x;
    bodyY[0] = y;
    for (int i = 1; i < body_len; i++)
    {
        prev2X = bodyX[i];
        prev2Y = bodyY[i];
        bodyX[i] = prevX;
        bodyY[i] = prevY;
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
        fruitX = rand() % width+1;
        fruitY = rand() % height+1;
        body_len++;
    }

    for (int i = 0; i < body_len; i++)
        if (bodyX[i] == x && bodyY[i] == y)
            gameOver = true;

}

int main()
{
    initscr();
    int factor;
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    Setup();
    while (!gameOver)
    {
        Draw();
        Input();
        Logic();
        usleep(speed);
        factor = score/30;
        if(score%30==0)
        {
            speed = abs(100000-factor*9000);
        }
            
    }
    nodelay(stdscr, FALSE);
    Draw();
    mvprintw(10,13,"Game Over");
    getch();
    endwin();
}