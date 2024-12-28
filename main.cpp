//Then I heard the voice of the Lord saying "Whom shall I send? And who shall go for us?" And I said, "Here am I. send me!"
//Lord give me one more chance;
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>

constexpr auto numVAOs = 1;

GLuint renderingProgram;
GLuint vao[numVAOs];

using namespace std; //why do so many people avoid this one line of code and write std 100 times in there project?

void printShaderLog(GLuint shader)
{
    int len = 0;
    int cWrittn = 0;
    char* log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0)
    {
        log = (char*)malloc(len);
        glGetShaderInfoLog(shader, len, &cWrittn, log);
        cout << "Shader Info Log: " << log << endl;
        free(log);
    }
}

void printProgramLog(int prog)
{
    int len = 0;
    int chWrittn = 0;
    char* log;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
    if (len > 0)
    {
        log = (char*)malloc(len);
        glGetProgramInfoLog(prog, len, &chWrittn, log);
        cout << "Program Info Log: " << log << endl;
        free(log);
    }
}


bool checkOpenGLError()
{
    bool foundError = 0; // now stay that way!!
    int glErr = glGetError();
    while (glErr != GL_NO_ERROR) // loop through all the error and print them out.
    {
        cout << "glError: " << glErr << endl;
        foundError = 1;
        glErr = glGetError();
    }
    return foundError;
}

string readShaderSource(const char* filePath)
{
    string content;
    ifstream fileStream(filePath, ios::in);
    string line = "";
    while (!fileStream.eof())
    {
        getline(fileStream, line);
        content.append(line + "\n");
    }
    return content;
}

GLuint createShaderProgram() 
{
    GLint vertCompiled;
    GLint fragCompiled;
    GLint linked;
    
    string vertShaderStr = readShaderSource("vertShader.glsl");
    string fragShaderStr = readShaderSource("fragShader.glsl");

    const char* vShaderSource = vertShaderStr.c_str();
    const char* fShaderSource = fragShaderStr.c_str();

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vShader, 1, &vShaderSource, NULL);
    glShaderSource(fShader, 1, &fShaderSource, NULL);

    // error checking & compiling the shaders
    glCompileShader(vShader);
    checkOpenGLError();
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
    if (vertCompiled != 1)
    {
        cout << "vertex compilation Failed" << endl;
        printShaderLog(vShader);
    }
    glCompileShader(fShader);
    checkOpenGLError();
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
    if (fragCompiled != 1)
    {
        cout << "frag compilation Failed" << endl;
        printShaderLog(fShader);
    }

    //error checking & linking the shaders
    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);
    checkOpenGLError();
    glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
    if (linked != 1)
    {
        cout << "Linking failed :c" << endl;
        printProgramLog(vfProgram);
    }

    return vfProgram;
}

void init(GLFWwindow* window) 
{
    renderingProgram = createShaderProgram();
    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]); 
}


void display(GLFWwindow* window, double currentTime)
{
    glUseProgram(renderingProgram);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(void)
{
    if (!glfwInit()) //if the GLFWinit workie it returns true :)
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //setting GLFW to version 4.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //width,height, title, isFullscreen, isResorceSharring; window is made a pointer because its much more efficent to just pass the address the the entire window object each cycle;
    GLFWwindow* window = glfwCreateWindow(1200, 800, "OpenGl Test0; Init(C01E);", NULL, NULL);
    glfwMakeContextCurrent(window); //linking GLFW to open GL

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSwapInterval(1); //this is required for Vsync
    init(window);

    while (!glfwWindowShouldClose(window))
    {
        display(window, glfwGetTime());
        glfwSwapBuffers(window); //this is also required for Vsync
        glfwPollEvents(); // this handles window events like keys being pressed;
    }
    //clear the allocated memory like a good programmer do :)
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
    return 0;
}