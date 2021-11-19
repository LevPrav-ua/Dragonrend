#pragma once
#include <SFML/Graphics.hpp>;
#include <vector>;
#include "gun.h";
#include "enemy.h";


using namespace sf;

class Engine
{
private:
    RenderWindow m_Window;

    // Объявляем спрайт и текстуру для фона
    Sprite m_BackgroundSprite;
    Texture m_BackgroundTexture;
    Texture kill_streak_texture, fire_texture;
    Sprite kill_streak_sprite, fire_sprite;
    Text text, killed, lost;
    Font font;
    int fails;
    int scores;
    bool on_restart;

    // Экземпляр пушки
    Gun gun;
    std::vector<Enemy*> enemys;

    int const W_WIDTH = 1280;
    int const W_HEIGHT = 720;
    double const G = 9.8;
    double const PI = 3.14159265;
    float const dt = 0.1;

    void input();
    void update(float dtAsSeconds);
    float getYofTrajectory(float x);
    float calculate_and_scale_x(float t, float angle);
    float calculate_and_scale_y(float t, float angle);
    float getFlyTime();

    void reset();
    void draw();
    void gameOver();
    void save();

public:
    // Конструктор движка
    Engine();
    ~Engine();
    // Функция старт вызовет все приватные функции
    void start();
    void menu();

};