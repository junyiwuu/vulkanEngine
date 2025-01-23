# Study Log
[Vulkan Engine tutorial](https://youtube.com/playlist?list=PL8327DO66nu9qYVKLDmdLW_84-yE4auCR&si=RmYj1OuZB_rt8zqs)


### Overview notes:
FirstApp : initialize the window, initialize the graphicPipeline(read SPRV files into buffers)


### Compile notes:
**when conflict happen**:   
like cmakecache doesnt match: remove build file , start again

    rm -rf build

**add_executable how to include all cpp file**:   
1.  `file(GLOB SOURCES "*.cpp")` searching for current folder, adding all cpp file into SOURCES variable and then `add_executable(vulkanTest ${SOURCES})`. Therefore collect all cpp under current directory
    > but if add more cpp files later, need to cmake again

2. manually add all cpp files name here


## Chapter 1 : create windows
[code](https://github.com/junyiwuu/vulkanEngine/tree/00a8953686fdff02c80504df77a1088ce2ec82bf)
### **glfwWindowHint (int hint, int value) :**

[glfwWindowHint link](https://www.glfw.org/docs/3.3/group__window.html#ga7d9c8c62384b1e2821c4dc48952d2033)

[hint](https://www.glfw.org/docs/3.3/window_guide.html#window_hints)
- Hints also including soft constraint and hard constraint
- GLFW_CLIENT_API by default set to openGL

    Hard Constraint: 

        
    >A strict requirement that must be satisfied. If a hard constraint cannot be fulfilled, the associated operation will fail. In the context of the image, specific window hints like GLFW_STEREO or GLFW_CLIENT_API are examples of hard constraints. These must align exactly with the system's capabilities. 

    Soft Constraint: 

    >A flexible preference. The system will attempt to match the soft constraint as closely as possible, but it's not mandatory for success. If the exact condition isn't met, the operation can still proceed with the closest alternative available. 

    Hint: 

    >A configuration setting or guideline provided by the developer to influence how the system behaves. In the GLFW context, hints are parameters used to define the properties of a window or OpenGL context, such as whether double buffering is enabled (GLFW_DOUBLEBUFFER). These hints can be hard or soft constraints, depending on the specific functionality and context. 
        
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




## Chapter 2 : read spv and build pipeline

gl_VertextIndex: contains the index of the current vertext for each time our main function run


**`layout(location=0) out vec4 outColor;`**

> layout(location=0):  
layout qualifier. Specifies the location index of this output variable. currently return 0, meaning this variable will be linked to the first framebuffer

> out: 
this variable is an output of the fragment shader. fragment shader produce color values for each pixel, this is where the color will be stored

>outColor: 
the name of output variable

OVERALL:
a output variable , name is "outColor"-- fragment shader, location is 0, vec4



----------------------------
### compile GLSL to SPIR-V
we need to compile the shader code into intermediate binary formate (SPIR-V). like compile in c++ 



-o specifiy our compiled output file's name, int his caser we use the same name

**compile.sh**   
.sh file is the shell file, we create compile.sh:

`/usr/bin/glslc shaders/simple_shader.vert -o shaders/simple_shader.vert.spv`

`/usr/bin/glslc shaders/simple_shader.frag -o shaders/simple_shader.frag.spv`

`chmod +x compile.sh` make this file executable


----
pipeline build

std::vector    
dynamic sized array

`static std::vector<char> readFile(const std::string& filepath);` read file content and return with formate std::vector\<char>

### **How to read the file**   
`std::ifstream` input stream, for reading files   
`file()` file function, ( filepath ,bit flags(open mode))   
`std::ios::ate` at the end, this mean once the file open we seek the end immediately   
`std::ios::binary` open the file in binary mode, raw stream of bytes, avoid any unwanted text transformations  
`std::ios::ate | std::ios::binary` we want both gpointer o to end the file (so can use tellg()) , and read as binary

why {} instead of (): 
1. uniform initialization. so we tell this is object definition, not function declaration.
2. strict type checking, prevent implicit type conversion


**Open file error check**
```cpp
    if (!file.is_open()){
        throw std::runtime_error("fail to open file: " + filepath)
        //filepath is incorrect or no permission to open the file
    }
```

**File size**   
***the last of file***    
file.tellg() return `std::streampos`daata, means the pointer's location in the file
```cpp
size_t filesize = static_cast<size_t>(file.tellg());
```
> other example: 
> ```cpp
>double value = 3.14;
>int intValue = static_cast<int>(value); // transfer to >integer
>std::cout << intValue; // output: 3 
>```
***the first of the file***
```cpp
file.seekg(0);  //move the pointer to the beginning
```
**Create buffer**

    file.read(buffer.data(), fileSize);
        file.close();
        return buffer;

**createGraphicPipeline**

    auto vertCode = readFile(vertFilepath);
    auto fragCode = readFile(fragFilepath);
we read files into buffers.

**lvePipeline constructor**   
call createGraphicPiline function --> create buffers --> read files for buffers, store data into them.



**back to FirstApp**: 
```cpp
namespace lve{
    class FirstApp{
      public:
        static constexpr int width = 800;
        static constexpr int height = 600;

        void run();

      private:
        LveWindow LveWindow_app{width , height, "hello vulkan"} ;

        lvePipeline lvePipeline_app{"../shaders/simple_shader.vert.spv", "../shaders/simple_shader.frag.spv"};
    };
}
```
add the lvePipeline in FirstApp private, so when initialize the object, it also get called


````````````````