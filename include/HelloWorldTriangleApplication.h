#ifndef AC79E733_933A_4E39_816C_CDA7F4FC8A53
#define AC79E733_933A_4E39_816C_CDA7F4FC8A53

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include <optional>

namespace TriangleApplication
{
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
    };

    class HelloWorldTriangleApplication
    {
    public:
        void run();

    private:
        void cleanUp();
        void createInstace();
        void initWindow();
        void initVulkan();
        void mainLoop();
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
        void pickPhysicalDevice();
        void setupDebugMessenger();

        bool checkValidationLayerSupport();
        bool isDeviceSuitable(VkPhysicalDevice device);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        std::vector<const char *> getRequiredExtensions();

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageSeverityFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);

        const uint32_t WIDTH = 800;
        const uint32_t HEIGHT = 600;
        const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
#ifdef NDEBUG
        const bool enableValidationLayers = false;
#else
        const bool enableValidationLayers = true;
#endif

        GLFWwindow *window;
        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    };
}
#endif /* AC79E733_933A_4E39_816C_CDA7F4FC8A53 */
