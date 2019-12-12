#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <vulkan/vulkan.h>
#include <cmath>
#include <memory>
#include <vector>
#include <limits>
#include <iostream>
VkInstance instance;
VkSurfaceKHR surface;
VkDevice dev;
GLFWwindow* window;
void printstats(VkPhysicalDevice &device)
{
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(device,&properties);
    std::cout<<"NAME OF DEVICE:"<<properties.deviceName<<std::endl;
    uint32_t apiVersion=properties.apiVersion;
    std::cout<<"API VERSION :"<< VK_VERSION_MAJOR(apiVersion)<<"."<<VK_VERSION_MINOR(apiVersion)<<"."<<VK_VERSION_PATCH(apiVersion)<<std::endl;
    std::cout<<"DRIVER VERSION"<<properties.driverVersion<<std::endl;
    std::cout<<"discrete queue properties"<<properties.limits.discreteQueuePriorities<<std::endl;
    
    std::cout<< std::endl;
    
}
void startGLfw()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);
    window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

}
void stopGLfw()
{
  
    
    glfwDestroyWindow(window);
    glfwTerminate();
} 
void gameloop()
{
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }  
}
void startVulkan()
{
    VkApplicationInfo appInfo;
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext=nullptr;
    appInfo.pApplicationName="Vulkan Tutorial";
    appInfo.applicationVersion=VK_MAKE_VERSION(0,0,0);
    appInfo.pEngineName = "my first vulkan engine";
    appInfo.engineVersion=VK_MAKE_VERSION(1,0,0);
    appInfo.apiVersion=VK_API_VERSION_1_0;
    
    uint32_t amountofGlfwExtensions=0;
    auto glfwExtensions=glfwGetRequiredInstanceExtensions(&amountofGlfwExtensions);
    
    VkInstanceCreateInfo instanceinfo;
    instanceinfo.sType= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceinfo.pNext=NULL;
    instanceinfo.flags=0;
    instanceinfo.pApplicationInfo=&appInfo;
    instanceinfo.enabledLayerCount=0;
    instanceinfo.ppEnabledLayerNames=NULL;
    instanceinfo.enabledExtensionCount=amountofGlfwExtensions;
    instanceinfo.ppEnabledExtensionNames=glfwExtensions;
    VkResult result=vkCreateInstance(&instanceinfo,NULL,&instance);
    if(result!=VK_SUCCESS)
    {
        std::cout<<"VK_FAILURE"<<std::endl;
    }
    
    result = glfwCreateWindowSurface(instance,window,nullptr,&surface);
    if(result!=VK_SUCCESS)
    {
        std::cout<<"VK_FAILURE"<<std::endl;
    }
 
    uint32_t amountofphysicaldevices=0;
    result=vkEnumeratePhysicalDevices(instance,&amountofphysicaldevices,NULL);
    if(result!=VK_SUCCESS)
    {
            std::cout<<"VK_FAILURE"<<std::endl;
    }
    std::cout<<"how many graphics card do I have "<<amountofphysicaldevices<<std::endl;
    
    
    
    std::vector<VkPhysicalDevice> physicaldevices;
    physicaldevices.resize(amountofphysicaldevices);
    result=vkEnumeratePhysicalDevices(instance,&amountofphysicaldevices,physicaldevices.data());
    if(result!=VK_SUCCESS)
    {
            std::cout<<"VK_FAILURE"<<std::endl;
    }
    for(int i=0 ; i < amountofphysicaldevices ;i++)
    {
        printstats(physicaldevices[i]);
    }
    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(physicaldevices[0],&features);
    
    
    std::cout<<"Is geometry shader active "<<features.geometryShader<<std::endl;
    
    
    VkPhysicalDeviceMemoryProperties memprop; 
    vkGetPhysicalDeviceMemoryProperties(physicaldevices[0],&memprop);
    std::cout<<"count of heap memory in bytes "<<memprop.memoryHeapCount<<std::endl;
    uint32_t amountofqueueFamilies=0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicaldevices[0],&amountofqueueFamilies,NULL);
    if(result!=VK_SUCCESS)
    {
            std::cout<<"VK_FAILURE"<<std::endl;
    }
    std::vector<VkQueueFamilyProperties> queuefamilyProperties; 
     queuefamilyProperties.resize(amountofqueueFamilies);
    vkGetPhysicalDeviceQueueFamilyProperties(physicaldevices[0],&amountofqueueFamilies,queuefamilyProperties.data());
    if(result!=VK_SUCCESS)
    {
            std::cout<<"VK_FAILURE"<<std::endl;
    }
    
    std::cout<<"Amount of Queue Families"<<amountofqueueFamilies<<std::endl;    
    for(int i=0;i<amountofqueueFamilies;i++)
    {
        std::cout<<std::endl;
        std::cout<<"Queue FAMILY #"<<i<<std::endl;
        std::cout<<"VK_QUEUE_GRAPHICS_BIT "<<(queuefamilyProperties[i].queueFlags&VK_QUEUE_GRAPHICS_BIT);
    }
    
    
    VkSurfaceCapabilitiesKHR surfacecapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicaldevices[0],surface,&surfacecapabilities);
    std::cout<<surfacecapabilities.minImageCount<<std::endl;
    std::cout<<surfacecapabilities.maxImageCount<<std::endl;
    std::cout<<surfacecapabilities.currentExtent.width<<surfacecapabilities.currentExtent.height<<std::endl;
    std::cout<<surfacecapabilities.minImageExtent.width<<surfacecapabilities.minImageExtent.height<<std::endl;
    std::cout<<surfacecapabilities.maxImageExtent.width<<surfacecapabilities.maxImageExtent.height<<std::endl;
    std::cout<<surfacecapabilities.maxImageArrayLayers<<std::endl;
    std::cout<<surfacecapabilities.supportedTransforms<<std::endl;
    std::cout<<surfacecapabilities.currentTransform<<std::endl;
    std::cout<<surfacecapabilities.supportedCompositeAlpha<<std::endl;
    std::cout<<surfacecapabilities.supportedUsageFlags<<std::endl;
    
    uint32_t amountsofformats=0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicaldevices[0],surface,&amountsofformats,nullptr);
    std::vector<VkSurfaceFormatKHR> surfaceformat;
    surfaceformat.resize(amountsofformats);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicaldevices[0],surface,&amountsofformats,surfaceformat.data());
    
    for(auto &r : surfaceformat)
    {
        std::cout<<"surface formats "<< r.format <<std::endl;
    }
    const std::vector<const char *> validationLayers={"VK_LAYER_<<std::endlLUNAG_standrad_validation"};
    float qprioriorities[]={1.0f,1.0f,1.0f,1.0f};
    VkDeviceQueueCreateInfo devicequeuecreateinfo;
    devicequeuecreateinfo.sType=VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    devicequeuecreateinfo.pNext=NULL;
    devicequeuecreateinfo.flags= 0;
    devicequeuecreateinfo.queueFamilyIndex=0;
    devicequeuecreateinfo.queueCount=4;
    devicequeuecreateinfo.pQueuePriorities=qprioriorities;
    
    VkPhysicalDeviceFeatures usedfeatures={};
    
    VkDeviceCreateInfo information;
    information.sType=VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    information.pNext=NULL;
    information.flags=0;
    information.queueCreateInfoCount=1;
    information.pQueueCreateInfos=&devicequeuecreateinfo;
    information.enabledLayerCount=validationLayers.size();
    information.ppEnabledLayerNames=validationLayers.data();
    information.enabledExtensionCount=0;
    information.ppEnabledExtensionNames=NULL;
    information.pEnabledFeatures=&usedfeatures;
    
    
    
    result=vkCreateDevice(physicaldevices[0],&information,NULL,&dev);
    if(result!=VK_SUCCESS)
    {
        
    }
   VkQueue queue;
   vkGetDeviceQueue(dev,0,0,&queue);
   
    uint amountofLayers=0;
    vkEnumerateInstanceLayerProperties(&amountofLayers,NULL);
    std::unique_ptr<VkLayerProperties[]> layers{new VkLayerProperties[amountofLayers]};
    vkEnumerateInstanceLayerProperties(&amountofLayers,layers.get());
    
    
    std::cout<<"amount of layers"<< amountofLayers<<std::endl;
    for(int i=0 ;i< amountofLayers;i++)
    {
        std::cout<< "name of the layer"<<layers[i].layerName<<std::endl;
        std::cout<< "specification version"<<layers[i].specVersion<<std::endl;
        std::cout<< "implementation version"<<layers[i].implementationVersion<<std::endl;
        std::cout<< "description"<<layers[i].description<<std::endl;
    }
    
    uint32_t amountOfExtensions = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &amountOfExtensions , nullptr);
    std::unique_ptr<VkExtensionProperties[]> extensions{new VkExtensionProperties[amountOfExtensions]};
    vkEnumerateInstanceExtensionProperties(nullptr, &amountOfExtensions , extensions.get());
    
    std::cout << amountOfExtensions << " extensions supported" << std::endl;
    for(int i=0;i<amountOfExtensions;i++)
    {
        std::cout<<std::endl;
        std::cout<<"NAME: "<<extensions[i].extensionName<<std::endl;
    }
    
}
void stopVulkan()
{
    vkDeviceWaitIdle(dev);
    vkDestroyDevice(dev,nullptr);
    vkDestroySurfaceKHR(instance,surface,nullptr);
    vkDestroyInstance(instance,nullptr);
}

int main() {
    startGLfw();
    startVulkan();
    gameloop();
    stopVulkan();
    stopGLfw();
    
    

    
    /* today i will implement spherical polar co-ordinate system*/
    float r,x,y,z,phi,theta;
    z = r*cos(theta);
     x = r*sin(theta)*cos(phi);
     y = r*sin(theta)*sin(phi);
    /* reverse conversion*/
    if(std::hypot(x,y)<std::numeric_limits<float>::min())
    {
        r=std::abs(z);
        theta=0;
        phi=0;
    }
    else
    {
        /*r=std::hypot(x,y,z);
        theta=std::arcsin(std::div(std::hypot(x,y),r));*/
        if(y<0)
        {
            phi=2*180-std::atan2(y,x);
        }
        else
        {
            phi=std::atan2(y,x);
        }
    }
    glm::mat4 matrix;
    glm::vec4 vec;
    auto test = matrix * vec;
    
    return 0;
}
