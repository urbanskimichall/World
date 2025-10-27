#include "World.hpp"
#include "Logger.hpp"

int main()
{
    Logger::instance().enableConsole(true);
    
    World world;
    world.create();

    return 0;
}