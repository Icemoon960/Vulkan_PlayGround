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
#include <set>
#include <limits>
#include <algorithm>
#include <cstdint>
#include <fstream>

namespace TriangleApplication
{
    struct SwapChainSupportDetails{
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;
        bool isComplete()
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    class HelloWorldTriangleApplication
    {
    public:
        void run();

    private:
        void cleanUp();
        void createGraphicsPipeline();
        void createInstace();
        void createImageViews();
        void createLogicalDevice();
        void createSurface();
        void createSwapChain();
        void initWindow();
        void initVulkan();
        void mainLoop();
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
        void pickPhysicalDevice();
        void setupDebugMessenger();

        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        bool checkValidationLayerSupport();
        bool isDeviceSuitable(VkPhysicalDevice device);
        VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtend(const VkSurfaceCapabilitiesKHR& capabilities);
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        std::vector<const char *> getRequiredExtensions();
        
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageSeverityFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);
        static std::vector<char> readFile(const std::string& filename);

        const uint32_t WIDTH = 800;
        const uint32_t HEIGHT = 600;
        const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
        const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
#ifdef NDEBUG
        const bool enableValidationLayers = false;
#else
        const bool enableValidationLayers = true;
#endif
        VkDebugUtilsMessengerEXT debugMessenger;
        VkQueue graphicsQueue;
        VkInstance instance;
        VkDevice logicalDevice;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkQueue presentQueue;
        VkSurfaceKHR surface;
        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;
        VkFormat swapChainFormat;
        VkExtent2D swapChainExtend;
        GLFWwindow *window;
    };
}
#endif /* AC79E733_933A_4E39_816C_CDA7F4FC8A53 */
