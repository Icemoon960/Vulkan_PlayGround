#include <vulkan/vulkan.h>

#include <iostream>
#include <HelloWorldTriangleApplication.h>

int main(int argc, char **argv)
{
    TriangleApplication::HelloWorldTriangleApplication app;

    try
    {
        app.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}