#include <HelloWorldTriangleApplication.h>

namespace TriangleApplication
{
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger)
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator)
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            func(instance, debugMessenger, pAllocator);
        }
    }

#pragma region Static Funktions
    VKAPI_ATTR VkBool32 VKAPI_CALL HelloWorldTriangleApplication::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageSeverityFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData)
    {
        std::cerr << "Validation Layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }
    std::vector<char> HelloWorldTriangleApplication::readFile(const std::string& filename){
        std::ifstream file(filename, std::ios::ate | std::ios::binary);
        if(!file.is_open()){
            throw std::runtime_error(std::string("failed to open file: ") + filename);
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();
        std::cout << "Red file " << filename << " in buffer of " << (buffer.size()/sizeof(char)) << " byte" << std::endl;

        return buffer;
    }
#pragma endregion

#pragma region Dynamic Funktions
    bool HelloWorldTriangleApplication::checkDeviceExtensionSupport(VkPhysicalDevice device)
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
        for (const auto &extension : availableExtensions)
        {
            requiredExtensions.erase(extension.extensionName);
        }
        return requiredExtensions.empty();
    }
    bool HelloWorldTriangleApplication::checkValidationLayerSupport()
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const auto &layerName : validationLayers)
        {
            bool found = false;
            for (const auto &availableLayer : availableLayers)
            {
                if (std::strcmp(layerName, availableLayer.layerName) == 0)
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                return false;
            }
        }
        return true;
    }
    VkPresentModeKHR HelloWorldTriangleApplication::choosePresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
    {
        for (const auto &availablePresentMode : availablePresentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return availablePresentMode;
            }
        }
        // TODO: Log which PrensetMode was uesed..
        return VK_PRESENT_MODE_FIFO_KHR;
    }
    VkExtent2D HelloWorldTriangleApplication::chooseSwapExtend(const VkSurfaceCapabilitiesKHR &capabilities)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        }
        else
        {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            VkExtent2D actualExtend = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };
            actualExtend.width = std::clamp(actualExtend.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtend.height = std::clamp(actualExtend.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
            return actualExtend;
        }
    }
    VkSurfaceFormatKHR HelloWorldTriangleApplication::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
    {
        for (const auto &availableFormat : availableFormats)
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }
        // TODO: Log that the first ColorFormat was uesed..
        return availableFormats[0];
    }
    void HelloWorldTriangleApplication::cleanUp()
    {
        for(auto imageView : swapChainImageViews){
            vkDestroyImageView(logicalDevice, imageView, nullptr);
        }
        vkDestroySwapchainKHR(logicalDevice, swapChain, nullptr);
        vkDestroyDevice(logicalDevice, nullptr);
        if (enableValidationLayers)
        {
            DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        }
        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    void HelloWorldTriangleApplication::createGraphicsPipeline(){
        auto vertShaderCode = readFile(".shaders/vert.spv");
        auto fragShaderCode = readFile(".shaders/frag.spv");

        VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
        VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main";
        //pSpecializationInfo allows you to specify values for shader constants.        

        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName = "main";    

        VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};    

        vkDestroyShaderModule(logicalDevice,fragShaderModule, nullptr);
        vkDestroyShaderModule(logicalDevice,vertShaderModule, nullptr);
    }
    void HelloWorldTriangleApplication::createInstace()
    {
        if (enableValidationLayers && !checkValidationLayerSupport())
        {
            throw std::runtime_error("validation layers requested, but not available!");
        }

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 0);

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // By creating an additional debug messenger this way it will automatically be used during vkCreateInstance and vkDestroyInstance and cleaned up after that.
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (enableValidationLayers)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();

            populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
        }
        else
        {
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
        }

        auto extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create instance");
        };
    }    
    void HelloWorldTriangleApplication::createImageViews(){
        swapChainImageViews.reserve(swapChainImages.size());
        for(size_t i = 0; i<swapChainImages.size(); i++){
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = swapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = swapChainFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;
            /*
                If you were working on a stereographic 3D application, then you would create a swap chain with multiple layers. 
                You could then create multiple image views for each image representing the views for the left and right eyes 
                by accessing different layers.
            */
           if (vkCreateImageView(logicalDevice, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS){
                throw std::runtime_error("failed to create image views!");
           }
        }
    }
    void HelloWorldTriangleApplication::createLogicalDevice()
    {
        QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uinqueQueueFamailies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        float queuePriority = 1.0f;
        for (auto queueFamily : uinqueQueueFamailies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();
        if (enableValidationLayers)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create logical device!");
        }
        vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
        vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), 0, &presentQueue);
    }
    VkShaderModule HelloWorldTriangleApplication::createShaderModule(const std::vector<char>& code){
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;
        if(vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS){
            throw std::runtime_error("failed to create another module!");
        }

        return shaderModule;
    }
    void HelloWorldTriangleApplication::createSurface()
    {
        if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create window surface!");
        }
    }
    void HelloWorldTriangleApplication::createSwapChain(){
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = choosePresentMode(swapChainSupport.presentModes);
        VkExtent2D extend = chooseSwapExtend(swapChainSupport.capabilities);
        
        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount){
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extend;
        createInfo.imageArrayLayers = 1; //TODO: stereoscopic 3D application; set this to something larger than 1
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indicies = findQueueFamilies(physicalDevice);
        uint32_t queueFamilyIndices[] {indicies.graphicsFamily.value(), indicies.presentFamily.value()};
        
        if(indicies.graphicsFamily != indicies.graphicsFamily){
            //Images can be used across multiple queue families without explicit ownership transfers.
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }else{
            //An image is owned by one queue family at a time and ownership must be explicitly transferred before using it in another queue family
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;//optional
            createInfo.pQueueFamilyIndices = nullptr;//optional
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        //if the alpha channel should be used for blending with other windows in the window system
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if(vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &swapChain) != VK_SUCCESS){
            throw std::runtime_error("failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, nullptr);
        swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, swapChainImages.data());

        swapChainExtend = extend;
        swapChainFormat = surfaceFormat.format;
    }
    QueueFamilyIndices HelloWorldTriangleApplication::findQueueFamilies(VkPhysicalDevice device)
    {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        VkBool32 presentSupport = false;

        int i = 0;
        for (const auto &queueFamily : queueFamilies)
        {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
                if (presentSupport)
                {
                    indices.presentFamily = i;
                }
                indices.graphicsFamily = i;
                break;
            }
            i++;
        }

        return indices;
    }
    std::vector<const char *> HelloWorldTriangleApplication::getRequiredExtensions()
    {
        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions;
        uint32_t extensionCount = 0;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        std::vector<const char *> requiredExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> installedExtensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, installedExtensions.data());

        if (glfwExtensions != NULL)
        {
            for (const auto &requiredExtesionName : requiredExtensions)
            {
                bool found = false;
                for (const auto &installedExtension : installedExtensions)
                {
                    if (std::strcmp(requiredExtesionName, installedExtension.extensionName) == 0)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    throw std::runtime_error(std::string("Extension not found: \n%1", requiredExtesionName));
                }
            }
        }
        else
        {
            throw std::runtime_error("failed to find extension(s)");
        }

        if (enableValidationLayers)
        {
            requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

#ifndef NDEBUG
        std::cout << "available extensions:" << std::endl;

        for (const auto &extension : installedExtensions)
        {
            std::cout << '\t' << extension.extensionName << std::endl;
        }
#endif

        return requiredExtensions;
    }

    void HelloWorldTriangleApplication::initWindow()
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    }
    void HelloWorldTriangleApplication::initVulkan()
    {
        createInstace();
        setupDebugMessenger();
        createSurface();
        pickPhysicalDevice();
        createLogicalDevice();
        createSwapChain();
        createImageViews();
        createGraphicsPipeline();
    }
    bool HelloWorldTriangleApplication::isDeviceSuitable(VkPhysicalDevice device)
    {
        /*
        // Basic device properties like the name, type and supported Vulkan version
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        // Optional features like texture compression, 64 bit floats and multi viewport rendering
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
        */

        bool requiredExtensionsSupported = checkDeviceExtensionSupport(device);

        QueueFamilyIndices indices = findQueueFamilies(device);

        bool swapChainSupported = false;
        if (requiredExtensionsSupported)
        {
            SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
            swapChainSupported = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        return indices.isComplete() && requiredExtensionsSupported && swapChainSupported;
    }
    void HelloWorldTriangleApplication::mainLoop()
    {
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
        }
    }
    void HelloWorldTriangleApplication::pickPhysicalDevice()
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            throw std::runtime_error("failed to find GPU's with vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        for (const auto &device : devices)
        {
            if (isDeviceSuitable(device))
            {
                physicalDevice = device;
                break;
            }
        }
        if (physicalDevice == VK_NULL_HANDLE)
        {
            throw std::runtime_error("failed to find suitable GPU!");
        }
    }
    void HelloWorldTriangleApplication::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo)
    {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = this->debugCallback;
        createInfo.pUserData = nullptr; // Optional
    }
    SwapChainSupportDetails HelloWorldTriangleApplication::querySwapChainSupport(VkPhysicalDevice device)
    {
        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        uint32_t formatsCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatsCount, nullptr);
        if (formatsCount != 0)
        {
            details.formats.resize(formatsCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatsCount, details.formats.data());
        }

        uint32_t presentModesCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModesCount, nullptr);
        if (presentModesCount != 0)
        {
            details.presentModes.resize(presentModesCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModesCount, details.presentModes.data());
        }

        return details;
    }
    void HelloWorldTriangleApplication::run()
    {
        initWindow();
        initVulkan();
        mainLoop();
        cleanUp();
    }
    void HelloWorldTriangleApplication::setupDebugMessenger()
    {
        if (!enableValidationLayers)
        {
            return;
        }

        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        populateDebugMessengerCreateInfo(createInfo);

        if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to set up debug messenger!");
        }
    }
#pragma endregion
}