#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <vulkan/vulkan.h>
#include <cmath>
#include <limits>
#include <iostream>
VkInstance instance;
void printstats(VkPhysicalDevice &device)
{
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(device,&properties);
    std::cout<<"NAME OF DEVICE:"<<properties.deviceName<<std::endl;
    uint32_t apiVersion=properties.apiVersion;
    std::cout<<"API VERSION :"<< VK_VERSION_MAJOR(apiVersion)<<"."<<VK_VERSION_MINOR(apiVersion)<<"."<<VK_VERSION_PATCH(apiVersion)<<std::endl;
    std::cout<<"DRIVER VERSION"<<properties.driverVersion<<std::endl;
    std::cout<< std::endl;
}
int main() {
    glfwInit();
    VkApplicationInfo appInfo;
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext=NULL;
    appInfo.pApplicationName="Vulkan Tutorial";
    appInfo.applicationVersion=VK_MAKE_VERSION(0,0,0);
    appInfo.pEngineName = "my first vulkan engine";
    appInfo.engineVersion=VK_MAKE_VERSION(1,0,0);
    appInfo.apiVersion=VK_API_VERSION_1_0;
    
    VkInstanceCreateInfo instanceinfo;
    
    instanceinfo.sType= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceinfo.pNext=NULL;
    instanceinfo.flags=0;
    instanceinfo.pApplicationInfo=&appInfo;
    instanceinfo.enabledLayerCount=0;
    instanceinfo.ppEnabledLayerNames=NULL;
    instanceinfo.enabledExtensionCount=0;
    instanceinfo.ppEnabledExtensionNames=NULL;
    VkResult result=vkCreateInstance(&instanceinfo,NULL,&instance);
    
    
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
    VkPhysicalDevice* physicaldevices = new VkPhysicalDevice[amountofphysicaldevices];
    result=vkEnumeratePhysicalDevices(instance,&amountofphysicaldevices,physicaldevices);
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
    VkQueueFamilyProperties *queuefamilyProperties = new VkQueueFamilyProperties[amountofqueueFamilies];
    vkGetPhysicalDeviceQueueFamilyProperties(physicaldevices[0],&amountofqueueFamilies,queuefamilyProperties);
    if(result!=VK_SUCCESS)
    {
            std::cout<<"VK_FAILURE"<<std::endl;
    }
    
    std::cout<<"Amount of Queue Families"<<amountofqueueFamilies<<std::endl;    
    for(int i=0;i<amountofqueueFamilies;i++)
    {
        std::cout<<std::endl;
        std::cout<<"Queue FAMILY #"<<i<<std::endl;
        std::cout<<"VK_QUEUE_GRAPHICS_BIT "<<(queuefamilyProperties[i].queueFlags  & VK_QUEUE_GRAPHICS_BIT);
    }
    
    
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    
    std::cout << extensionCount << " extensions supported" << std::endl;
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

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
    delete []queuefamilyProperties;
    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}
