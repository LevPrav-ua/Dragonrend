#include "Engine.h"
#include <iostream>

Engine::Engine()
{
    // Получаем разрешение экрана, создаем окно SFML и View
    Vector2f resolution;
    resolution.x = W_WIDTH;
    resolution.y = W_HEIGHT;

    for (int i = 0; i <10; i++)
    {
        enemys.push_back(new Enemy());
    }
    enemys[0]->born(W_WIDTH, W_HEIGHT);
    fails = 0;
    scores = 0;
    on_restart = false;

    m_Window.create(VideoMode(resolution.x, resolution.y),
        "Simple Game Engine");
    m_Window.setFramerateLimit(60);
    // Загружаем фон в текстуру
    // Подготовьте изображение под ваш размер экрана в редакторе
    m_BackgroundTexture.loadFromFile("source\\back\\forest.png");
    // Связываем спрайт и текстуру
    m_BackgroundSprite.setTexture(m_BackgroundTexture);

    kill_streak_texture.loadFromFile("source\\back\\dragon_head.png");
    kill_streak_sprite.setTexture(kill_streak_texture);
    fire_texture.loadFromFile("source\\back\\fire.png");
    fire_sprite.setTexture(fire_texture);

    kill_streak_sprite.setPosition(0.0, 0.0);
    fire_sprite.setPosition(kill_streak_texture.getSize().x + 150, 0);

    if (!font.loadFromFile("source\\font\\rampart_one.ttf")) {
        std::cerr << "Font wasn't load\n";
    }
    text.setStyle(sf::Text::Bold);
    text.setCharacterSize(50);
    text.setPosition(W_WIDTH - 150, 20);
    text.setFont(font);

    killed = lost = text;

    killed.setPosition(kill_streak_sprite.getPosition().x + kill_streak_texture.getSize().x + 10, 0);
    lost.setPosition(fire_sprite.getPosition().x + fire_texture.getSize().x + 10, 0);

}

Engine::~Engine() {
    for(Enemy* enemy : enemys ){
        delete enemy;
    }
}

void Engine::reset() {
    for (Enemy* enemy : enemys) {
        enemy->kill();
    }
    fails = 0;
    scores = 0;
}
void Engine::save() {
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "Scores: %d; Lost enemys: %d \n",
        scores, fails);
    FILE* ptrFile;
    fopen_s(&ptrFile, "save.txt", "a+");
    if (ptrFile != NULL)
    {
        fputs(buffer, ptrFile); // записать строку в файл
        fclose(ptrFile);
    }
    else {
        std::cerr << "Cannot open file to save\n";
    }
}

void Engine::gameOver() {
    int win_x = W_WIDTH/2, win_y = W_HEIGHT/2;

    RenderWindow window(VideoMode(win_x, win_y), "Game Over");
    Text over_text, scores_text, menu_text, exit_text;
    Font menu_font;

    if (!menu_font.loadFromFile("source\\font\\rampart_one.ttf")) {
        std::cerr << "Font wasn't load\n";
    }
    over_text.setStyle(sf::Text::Bold); over_text.setCharacterSize(60); over_text.setFont(font);
    scores_text.setStyle(sf::Text::Bold); scores_text.setCharacterSize(40); scores_text.setFont(font);
    menu_text.setStyle(sf::Text::Bold); menu_text.setCharacterSize(40); menu_text.setFont(font);
    exit_text.setStyle(sf::Text::Bold); exit_text.setCharacterSize(40); exit_text.setFont(font);

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "Scores: %d Lost enemys: %d \n",
        scores, fails);

    over_text.setString(sf::String("GAME OVER"));
    scores_text.setString(sf::String(buffer));
    menu_text.setString(sf::String("Menu"));
    exit_text.setString(sf::String("Exit"));

    menu_text.setPosition(200, 50);
    scores_text.setPosition(50, 150);
    menu_text.setPosition(50, 250);
    exit_text.setPosition(500, 250);

    int menuNum;

    while (window.isOpen())
    {
        window.clear(Color::Red);
        menuNum = 0;

        if (IntRect(50,250,100,60).contains(Mouse::getPosition(window))) { menu_text.setFillColor(Color::Blue); menuNum = 0; }
        else menu_text.setFillColor(Color::White);

        if (IntRect(500,250,100,60).contains(Mouse::getPosition(window))) { exit_text.setFillColor(Color::Blue); menuNum = 1; }
        else exit_text.setFillColor(Color::White);

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) { window.close(); }
        }
        if (Mouse::isButtonPressed(Mouse::Left))
        {
            //new game
            if (menuNum == 0) { window.close();}
            //exit
            if (menuNum == 1) { window.close(); m_Window.close(); }
        }

        window.draw(menu_text); window.draw(scores_text); window.draw(exit_text); window.draw(over_text);
        window.display();
    }
}

void Engine::start()
{
    // Расчет времени
    Clock clock;
    float born_timer = 0.0;
    while (m_Window.isOpen())
    {
        if (born_timer > 4.0) {
            for (Enemy* enemy : enemys) {
                if (enemy->outOfWindow()) {
                    fails++;
                    enemy->kill();
                }
                if (!enemy->isAlive()) {
                    enemy->born(W_WIDTH, W_HEIGHT);
                    break;
                }
            }
            born_timer = 0.0;
        }

        //выход из игры 
        if (fails > 9) {
            gameOver();
            reset();
            menu();
        }
        // Перезапускаем таймер и записываем отмеренное время в dt
        Time dt = clock.restart();

        float dtAsSeconds = dt.asSeconds();
        born_timer += dtAsSeconds;
        input();
        update(dtAsSeconds);
        draw();
    }
}

void Engine::input()
{
    sf::Event event;
    while (m_Window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed) m_Window.close();
        if (event.type == sf::Event::KeyPressed)
        {
            // Получаем нажатую клавишу - выполняем соответствующее действие
            if (event.key.code == sf::Keyboard::Escape) menu();
            if (event.key.code == sf::Keyboard::Up) gun.moveUp();
            if (event.key.code == sf::Keyboard::Down) gun.moveDown();
            if (event.key.code == sf::Keyboard::Space) {
                if (!gun.isOnFire()) {
                    gun.setFireAngle();
                    gun.setFireTime(getFlyTime());
                    gun.setFirePosition();
                    gun.shot();
                }
            }
        }
        if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::Up) gun.stopUp();
            if (event.key.code == sf::Keyboard::Down) gun.stopDown();
        }

    }
}

float Engine::calculate_and_scale_x(float t, float angle) {
    double MAX_DISTANCE = pow(gun.getBullet().getSpeed(), 2) / G;

    float x = gun.getBullet().getSpeed() * t * cos((PI * angle / 180));
    return x *
        ((float)W_WIDTH - gun.getSprite().getTexture()->getSize().x - gun.getPosition().x)
        / MAX_DISTANCE;
};
float Engine::calculate_and_scale_y(float t, float angle) {
    double MAX_HEIGHT = (pow(gun.getBullet().getSpeed(), 2) / G) / 2;

    float y = gun.getBullet().getSpeed() * t * sin((PI * angle / 180)) - (G * pow(t, 2) / 2);
    return -1 * y *
        (gun.getSprite().getTexture()->getSize().y + gun.getPosition().y)
        / MAX_HEIGHT;
};
float Engine::getFlyTime() {
    return (gun.getBullet().getSpeed() * 2.0 * sin((PI * gun.getAngle() / 180))) / G;
}

void Engine::menu() {    
    int menuNum = 0;

    Texture menu_texture;
    Sprite menu_sprite;
    menu_texture.loadFromFile("source\\menu\\back.jpg");
    menu_sprite.setTexture(menu_texture);

    int win_x = menu_texture.getSize().x, win_y = menu_texture.getSize().y;

    RenderWindow window(VideoMode(win_x, win_y), "Menu");
    Text menu_text, start_text, save_text, new_game_text, exit_text;
    Font menu_font;

    if (!menu_font.loadFromFile("source\\font\\rampart_one.ttf")) {
        std::cerr << "Font wasn't load\n";
    }
    menu_text.setStyle(sf::Text::Bold); menu_text.setCharacterSize(50); menu_text.setFont(font);
    start_text.setStyle(sf::Text::Bold); start_text.setCharacterSize(40); start_text.setFont(font);
    new_game_text.setStyle(sf::Text::Bold); new_game_text.setCharacterSize(40); new_game_text.setFont(font);
    exit_text.setStyle(sf::Text::Bold); exit_text.setCharacterSize(40); exit_text.setFont(font);
    save_text.setStyle(sf::Text::Bold); save_text.setCharacterSize(40); save_text.setFont(font);

    menu_text.setString(sf::String("Menu"));
    start_text.setString(sf::String("Play"));
    save_text.setString(sf::String("Save"));
    new_game_text.setString(sf::String("New Game"));
    exit_text.setString(sf::String("Exit"));
    RectangleShape menu(Vector2f(250.f, 60.f)), start(Vector2f(250.f, 50.f)),
        save(Vector2f(250.f, 50.f)), new_game(Vector2f(250.f, 50.f)), exit(Vector2f(250.f, 50.f));

    Color fon(255, 166, 166);

    menu_text.setPosition(float(win_x) / 2.0, 50);
    menu.move(float(win_x) / 2.0, 50);
    menu.setFillColor(fon);

    start_text.setPosition(float(win_x) / 2.0, 150);
    start.move(float(win_x) / 2.0, 150);
    start.setFillColor(fon);

    save_text.setPosition(float(win_x) / 2.0, 200);
    save.move(float(win_x) / 2.0, 200);
    save.setFillColor(fon);

    new_game_text.setPosition(float(win_x) / 2.0, 250);
    new_game.move(float(win_x) / 2.0, 250);
    new_game.setFillColor(fon);

    exit_text.setPosition(float(win_x) / 2.0, 300);
    exit.move(float(win_x) / 2.0, 300);
    exit.setFillColor(fon);
    
    //////////////////////////////МЕНЮ///////////////////
    while (window.isOpen())
    {
        window.clear(Color::White);
        menuNum = 0;

        if (IntRect(start.getPosition().x, start.getPosition().y, start.getSize().x, start.getSize().y).contains(Mouse::getPosition(window))) { start.setFillColor(Color::Blue); menuNum = 1; }
        else start.setFillColor(fon);

        if (IntRect(save.getPosition().x, save.getPosition().y, save.getSize().x, save.getSize().y).contains(Mouse::getPosition(window))) { save.setFillColor(Color::Blue); menuNum = 2; }
        else save.setFillColor(fon);

        if (IntRect(new_game.getPosition().x, new_game.getPosition().y, new_game.getSize().x, new_game.getSize().y).contains(Mouse::getPosition(window))) { new_game.setFillColor(Color::Blue); menuNum = 3; }
        else new_game.setFillColor(fon);

        if (IntRect(exit.getPosition().x, exit.getPosition().y, exit.getSize().x, exit.getSize().y).contains(Mouse::getPosition(window))) { exit.setFillColor(Color::Blue); menuNum = 4; }
        else exit.setFillColor(fon);

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) { window.close(); }
            if (event.type == sf::Event::KeyPressed)
            {
                // Получаем нажатую клавишу - выполняем соответствующее действие
                if (event.key.code == sf::Keyboard::Escape) { window.close(); };
            }
        }
        if (Mouse::isButtonPressed(Mouse::Left))
        {
            if (menuNum == 1) window.close();//если нажали первую кнопку, то выходим из меню 
            //save
            if (menuNum == 2) { 
                Engine::save();
                window.close();
            }
            //new game
            if (menuNum == 3) { window.close(); reset();}
            //exit
            if (menuNum == 4) { window.close(); m_Window.close(); }

        }

        window.draw(menu_sprite);

        window.draw(menu); window.draw(start); window.draw(save); window.draw(new_game); window.draw(exit);
        window.draw(menu_text); window.draw(start_text); window.draw(save_text); window.draw(new_game_text);
        window.draw(exit_text);

        window.display();
    }
    ////////////////////////////////////////////////////
}

void Engine::update(float dtAsSeconds)
{
    gun.update(dtAsSeconds);
    if (gun.isOnFire()) {
        static float t = 0;

        if (t < gun.getFireTime()) t += dt;
        else { 
            t = 0.0; 
            gun.stopShot();
        }
        Vector2f start = gun.getFirePosition();
        gun.getBullet().update(
            start.x + calculate_and_scale_x(t, gun.getFireAngle()),
            start.y + calculate_and_scale_y(t, gun.getFireAngle())
        );
    }

    for(Enemy* enemy : enemys)
    {
        if (enemy->isAlive() && enemy->hitCheck(gun.getBullet())) {
            enemy->kill();
            scores++;
        }
        enemy->update(dtAsSeconds);
    }
}

void Engine::draw()
{
    // Стираем предыдущий кадр
    m_Window.clear(Color::White);
    // Отрисовываем фон
    m_Window.draw(m_BackgroundSprite);
    // И Боба
    m_Window.draw(kill_streak_sprite);
    m_Window.draw(fire_sprite);

    m_Window.draw(gun.getSprite());

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%g*", gun.getAngle());
    text.setString(sf::String(buffer));
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), ": %d", scores);
    killed.setString(sf::String(buffer));
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), ": %d", fails);
    lost.setString(sf::String(buffer));

    m_Window.draw(text);
    m_Window.draw(killed);
    m_Window.draw(lost);

    double fly_time = getFlyTime();
    int count = 0;
    int freq = 20;
    std::vector<CircleShape*> trajectory(freq+1);
    for (float i = 0; i < fly_time; i += fly_time/(float)freq)
    {
        Vector2f start = gun.getStartPosition() +  gun.correctStartPosition();
        trajectory[count] = new CircleShape(20.f);
        trajectory[count]->setFillColor(Color(255, 255, 255, 255));
        trajectory[count]->setPosition(
            start.x + calculate_and_scale_x(i, gun.getAngle()), 
            start.y + calculate_and_scale_y(i, gun.getAngle())
        );
        m_Window.draw(*(trajectory[count++]));
    }

    if (gun.isOnFire()) m_Window.draw(gun.getBullet().getSprite());

    for (Enemy* enemy : enemys) {
        if (enemy->isAlive()) {
            m_Window.draw(enemy->getSprite());
        }
    }

    // Отображаем все, что нарисовали
    m_Window.display();

    for (CircleShape* circle : trajectory) {
        delete circle;
    }
}

float  Engine::getYofTrajectory(float x) {
    return ((double)x * tanh((PI * gun.getAngle() / 180))) -
        (G * pow(x, 2))/ 
        (2 * pow(gun.getBullet().getSpeed(), 2) * pow(cos((PI * gun.getAngle() / 180)), 2));
}