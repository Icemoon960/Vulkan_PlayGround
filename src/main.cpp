#include <vulkan/vulkan.h>

#include <iostream>
#include <KnobyApp.h>

int main(int argc, char **argv)
{
    TriangleApplication::KnobyApp app;

    try
    {
        app.Run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}