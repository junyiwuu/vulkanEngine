# Study Log
[Vulkan Engine tutorial](https://youtube.com/playlist?list=PL8327DO66nu9qYVKLDmdLW_84-yE4auCR&si=RmYj1OuZB_rt8zqs)
## Chapter 1 : create windows

### **glfwWindowHint (int hint, int value) :**

[glfwWindowHint link](https://www.glfw.org/docs/3.3/group__window.html#ga7d9c8c62384b1e2821c4dc48952d2033)

[hint](https://www.glfw.org/docs/3.3/window_guide.html#window_hints)
- Hints also including soft constraint and hard constraint
- GLFW_CLIENT_API by default set to openGL

    Hard Constraint: 

        A strict requirement that must be satisfied. If a hard constraint cannot be fulfilled, the associated operation will fail. In the context of the image, specific window hints like GLFW_STEREO or GLFW_CLIENT_API are examples of hard constraints. These must align exactly with the system's capabilities. 

    Soft Constraint: 

        A flexible preference. The system will attempt to match the soft constraint as closely as possible, but it's not mandatory for success. If the exact condition isn't met, the operation can still proceed with the closest alternative available. 

    Hint: 

        A configuration setting or guideline provided by the developer to influence how the system behaves. In the GLFW context, hints are parameters used to define the properties of a window or OpenGL context, such as whether double buffering is enabled (GLFW_DOUBLEBUFFER). These hints can be hard or soft constraints, depending on the specific functionality and context. 
        
### **GLFWwindow:**

[window object](https://www.glfw.org/docs/3.3/window_guide.html#window_object)
- GLFWwindow object encapsulates both a window and a context : glfwCreateWIndow, glfwDestroyWindow, glfwTerminate.
- GLFWmonitor: spoecify whichi monitor (here not specify so using nullptr)
- GLFWwindow: share resource in the context, here is nullptr, means no share context with any window.






### **glfwWindowShouldClose:**

[glfwWindowShouldClose](https://www.glfw.org/docs/3.3/group__window.html#ga24e02fbfefbb81fc45320989f8140ab5)

What does it do: check if GLFW window required to close. 
> - when the user close the window (press the close button) return **True**
> - when use glfwSetWindowShouldClose()
> - shutdown the computer, means os require to close the window, also return True

parameter: THE window

return: the value of close flag







### what we do in this chaper:

goal: create a window

steps:

    define "lveWindow" class
        |
        |PRIVATE:
        |--initWindow()
        |--width, height, windowName
        |--pointer: GLFWwindow *window (the pointer for the window)
        |
        |PUBLIC:
        |--constructor and deconstructor
        |--dont allow copy 
        |--check if window close
        
    define "FirstApp" class
        |
        |PUBLIC:
        |--width, hright
        |--run function: when window is open, do something
        |
        |PRIVATE:
        |initialize a LveWindow(create a glfw window)



>First app (class): 
.
> - LveWindow_app(LveWindow class) is a member of FirstApp class **--->** when FirstApp object create (default constructor, here not explicit write down) **--->** LveWindow app automatically created(initialize)
.
> -  put LveWindow_app in private is **Encapsulation**, no other access from outside


lveWindow_app(object) (basic) --> been used in FirstApp class, part of initialize. --> 