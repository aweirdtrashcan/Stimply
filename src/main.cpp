#include "core/Engine.h"

int main(void)
{
    printf("Initializing Stimply Engine...\n");

    Engine engine("Stimply Engine", 1280, 720);

    engine.Run();

    return 0;
}