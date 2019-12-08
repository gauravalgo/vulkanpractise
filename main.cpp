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
    vkCreateInstance(&instanceinfo,NULL,&instance);
    
    
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    
    std::cout << extensionCount << " extensions supported" << std::endl;
    /* today i will implement spherical polar co-ordinate system*/
    float r,x,y,z,phi,theta;
    float z = r*cos(theta);
    float x = r*sin(theta)*cos(phi);
    float y = r*sin(theta)*sin(phi);
    /* reverse conversion*/
    if(std::hypot(x,y)<std::numeric_limits<float>::min())
    {
        r=std::abs(z);
        theta=0;
        phi=0;
    }
    else
    {
        r=std::hypot(x,y,z);
        theta=std::arcsin(std::div(std::hypot(x,y),r));
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

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}
