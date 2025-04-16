#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <time.h>
#include <sstream>
#include <iostream>
#include <fstream>
using namespace std;
using namespace sf;

int X = 30, Y = 20;
int interface = 16;
int W = interface * X;
int H = interface * Y;
int dir, lenght = 4;
int hscore = 0;

bool gameEnd(int &score, int &hscore);

struct Snake
{
    int x, y;
} snake[100];
struct Fruit
{
    int x, y;
} food;

int Tick(int &score, int &gameover)
{
    ifstream hiScore("Hi.txt");
    hiScore >> hscore;
    hiScore.close();

    for (int i = lenght; i > 0; --i)
    {
        snake[i].x = snake[i - 1].x;
        snake[i].y = snake[i - 1].y;
    }

    if (dir == 0)
        snake[0].y += 1;
    if (dir == 1)
        snake[0].x -= 1;
    if (dir == 2)
        snake[0].x += 1;
    if (dir == 3)
        snake[0].y -= 1;

    if ((snake[0].x == food.x) && (snake[0].y == food.y))
    {
        ofstream hiScore("Hi.txt");
        lenght++;
        score++;
        if (score > hscore)
            hscore = score;
        hiScore << hscore;
        hiScore.close();
        food.x = rand() % X;
        food.y = rand() % Y;
    }

    if (snake[0].x > X - 1)
        snake[0].x = 0;
    if (snake[0].x < 0)
        snake[0].x = X - 1;
    if (snake[0].y > Y - 1)
        snake[0].y = 0;
    if (snake[0].y < 0)
        snake[0].y = Y - 1;

    for (int i = 1; i < lenght; i++)
    {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
        {
            gameover = 1;
            bool b = gameEnd(score, hscore);
            gameover = b ? 0 : 1;
        }
    }
    return gameover;
}

int main()
{
    srand(time(0));
    int score = 0;
    RenderWindow window(VideoMode(W, H), "Legends of Snake!");

    int gameover = 0;
    Clock clock;
    float timer = 0, speed = 0.1;

    food.x = 10;
    food.y = 10;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Font font;
        font.loadFromFile("C:/Windows/Fonts/Arial.ttf");

        ostringstream scoreStr;
        scoreStr << "Score: " << score;
        Text scoreText(scoreStr.str(), font, 20);
        scoreText.setFillColor(Color::Red);

        ostringstream hScoreStr;
        hScoreStr << "High Score: " << hscore;
        Text hScoreText(hScoreStr.str(), font, 20);
        hScoreText.setFillColor(Color::Red);
        hScoreText.setPosition(300, 0);

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
            else if (Keyboard::isKeyPressed(Keyboard::Escape))
                window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Left))
            dir = 1;
        if (Keyboard::isKeyPressed(Keyboard::Right))
            dir = 2;
        if (Keyboard::isKeyPressed(Keyboard::Up))
            dir = 3;
        if (Keyboard::isKeyPressed(Keyboard::Down))
            dir = 0;

        if (timer > speed)
        {
            timer = 0;
            gameover = Tick(score, gameover);
        }

        window.clear(Color::White);

        // Draw food
        CircleShape foodShape(interface / 2);
        foodShape.setFillColor(Color::Red);
        foodShape.setPosition(food.x * interface, food.y * interface);
        window.draw(foodShape);

        // Draw snake
        for (int i = 0; i < lenght; i++)
        {
            RectangleShape segment(Vector2f(interface, interface));
            segment.setPosition(snake[i].x * interface, snake[i].y * interface);

            if (i == 0) // Head
                segment.setFillColor(Color::Blue);
            else if (i == lenght - 1) // Tail
                segment.setFillColor(Color::Cyan);
            else // Body
                segment.setFillColor(Color::Green);

            window.draw(segment);
        }

        window.draw(scoreText);
        window.draw(hScoreText);
        window.display();
    }
    return 0;
}

bool gameEnd(int &score, int &hscore)
{
    RenderWindow w(VideoMode(W, H), "Game Over");
    w.setFramerateLimit(60);

    Font font;
    font.loadFromFile("C:/Windows/Fonts/Arial.ttf");

    Text gameOverText("Game Over!", font, 50);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setPosition(W / 2 - 120, H / 2 - 50);

    Text restartText("Press Enter to Restart", font, 30);
    restartText.setFillColor(Color::White);
    restartText.setPosition(W / 2 - 140, H / 2 + 20);

    Text exitText("Press Escape to Exit", font, 30);
    exitText.setFillColor(Color::White);
    exitText.setPosition(W / 2 - 120, H / 2 + 60);

    while (w.isOpen())
    {
        Event event;
        while (w.pollEvent(event))
        {
            if (event.type == Event::Closed)
                w.close();
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Enter)
                {
                    score = 0;
                    lenght = 4;
                    snake[0].x = 0;
                    snake[0].y = 0;
                    return true;
                }
                if (event.key.code == Keyboard::Escape)
                    return false;
            }
        }

        w.clear(Color::Black);
        w.draw(gameOverText);
        w.draw(restartText);
        w.draw(exitText);
        w.display();
    }
    return false;
}