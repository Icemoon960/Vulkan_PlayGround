#ifndef AC79E733_933A_4E39_816C_CDA7F4FC8A53
#define AC79E733_933A_4E39_816C_CDA7F4FC8A53

#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <limits>
#include <optional>
#include <vector>

namespace TriangleApplication
{
    struct UniformBufferObject
    {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };
    

    struct Vertex
    {
        glm::vec2 pos;
        glm::vec3 color;
        static VkVertexInputBindingDescription getBindingDescription();
        static std::array<VkVertexInputAttributeDescription, 2>getAttributeDescriptions();
    };

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;
        std::optional<uint32_t> transferFamily;
        bool isComplete();
    };

    class KnobyApp
    {
    public:
        void Run();

    private:
        void cleanUp();
        void cleanupSwapChain();
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags propertyFlags, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        void createCommandBuffers();
        void createCommandPools();
        void createDescriptorSetLayout();
        void createFramebuffers();
        void createGraphicsPipeline();
        void createIndexBuffer();
        void createInstace();
        void createImageViews();
        void createLogicalDevice();
        void createRenderPass();
        void createSurface();
        void createSwapChain();
        void createSyncObjects();
        void createUniformBuffers();
        void createVertexBuffer();
        void drawFrame();
        void initWindow();
        void initVulkan();
        void mainLoop();
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
        void pickPhysicalDevice();
        void recordCommandBuffer(VkCommandBuffer cmdBuffer, uint32_t imageIndex);
        void recreateSwapChain();
        void setupDebugMessenger();
        void updateUniformBuffer(uint32_t currentImage);

        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        bool checkValidationLayerSupport();
        bool isDeviceSuitable(VkPhysicalDevice device);

        VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
        VkShaderModule createShaderModule(const std::vector<char> &code);
        VkExtent2D chooseSwapExtend(const VkSurfaceCapabilitiesKHR &capabilities);
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        std::vector<const char *> getRequiredExtensions();

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageSeverityFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);
        static void frameBufferResizedCallback(GLFWwindow *window, int width, int height);
        static std::vector<char> readFile(const std::string &filename);

        const uint32_t WIDTH = 800;
        const uint32_t HEIGHT = 600;
        const int MAX_FRAMES_IN_FLIGHT = 2;
        const std::vector<uint16_t> indicies = {0, 1, 2, 2, 3, 0};
        const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
        // Color and position combined into one array of vertices, known as interleaving vertex
        const std::vector<Vertex> vertices = {
            {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
            {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
        };
        const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
#ifdef NDEBUG
        const bool enableValidationLayers = false;
#else
        const bool enableValidationLayers = true;        
#endif
        bool frameBufferResized = false;
        VkBuffer indexBuffer;
        VkBuffer vertexBuffer;
        VkDeviceMemory indexBufferMemory; 
        VkDeviceMemory vertexBufferMemory;        
        VkCommandPool commandPool;
        VkCommandPool transferCommandPool;
        VkQueue graphicsQueue;
        VkQueue presentQueue;
        VkQueue transferQueue;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkDescriptorSetLayout descriptionSetLayout;
        VkPipeline graphicsPipeline;
        VkInstance instance;
        VkDevice logicalDevice;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
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
        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBufferMemory;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtend;
        GLFWwindow *window;
    };
}
#endif /* AC79E733_933A_4E39_816C_CDA7F4FC8A53 */
