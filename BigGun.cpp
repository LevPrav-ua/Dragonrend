#include "Engine.h"

int main()
{
    // Объявляем экземпляр класса Engine
    Engine engine;
    engine.menu();
    // Вызываем функцию start
    engine.start();
    // Останавливаем программу программу, когда движок остановлен
    return 0;
}