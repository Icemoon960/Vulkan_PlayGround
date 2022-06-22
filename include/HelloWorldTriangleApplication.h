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
        void Run();

    private:
        void cleanUp();
        void cleanupSwapChain();
        void createCommandBuffers();
        void createCommandPool();
        void createFramebuffers();
        void createGraphicsPipeline();
        void createInstace();
        void createImageViews();
        void createLogicalDevice();
        void createRenderPass();
        void createSurface();        
        void createSwapChain();
        void createSyncObjects();
        void drawFrame();
        void initWindow();
        void initVulkan();
        void mainLoop();
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
        void pickPhysicalDevice();
        void recordCommandBuffer(VkCommandBuffer cmdBuffer, uint32_t imageIndex);
        void recreateSwapChain();
        void setupDebugMessenger();

        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        bool checkValidationLayerSupport();
        bool isDeviceSuitable(VkPhysicalDevice device);
        VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkShaderModule createShaderModule(const std::vector<char>& code);
        VkExtent2D chooseSwapExtend(const VkSurfaceCapabilitiesKHR& capabilities);
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        std::vector<const char *> getRequiredExtensions();
        
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageSeverityFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);
        static void frameBufferResizedCallback(GLFWwindow* window, int width, int height);
        static std::vector<char> readFile(const std::string& filename);

        const uint32_t WIDTH = 800;
        const uint32_t HEIGHT = 600;
        const int MAX_FRAMES_IN_FLIGHT = 2;
        const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
        const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
#ifdef NDEBUG
        const bool enableValidationLayers = false;
#else
        const bool enableValidationLayers = true;
#endif
        bool frameBufferResized = false;
        VkCommandPool commandPool;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkQueue graphicsQueue;        
        VkPipeline graphicsPipeline;
        VkInstance instance;
        VkDevice logicalDevice;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkQueue presentQueue;
        VkPipelineLayout pipelineLayout;
        VkRenderPass renderPass;
        VkSurfaceKHR surface;
        VkSwapchainKHR swapChain;
        std::vector<VkCommandBuffer> commandBuffers;
        std::vector<VkFence> inFlightFences;
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFramebuffer> swapChainFramebuffers;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtend;
        GLFWwindow *window;
    };
}
#endif /* AC79E733_933A_4E39_816C_CDA7F4FC8A53 */
