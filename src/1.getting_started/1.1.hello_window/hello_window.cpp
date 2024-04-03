#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// 函数声明，因为编译器的编译，符号是按照顺序加入符号数据库的，当在使用后面声明，就会造成在符号数据库里面找不到对应符号
/*!  GLFWwindow @brief Opaque window object.
 *
 *  Opaque （不透明） window object.
 *
 *  @ingroup window
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// 输入处理
void processInput(GLFWwindow *window);

// settings
// 窗口大小
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
	// 初始化这个gl辅助框架
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
	// 	glfwCreateWindow：glfw框架代码，作用是创建窗口，glfw就是对opengl的包装，将opengl的接口进行组装，简化出功能接口
	// 实现具体常用的功能
	// 这个就是将多个opengl API进行组合，组合出窗口创建功能
	// 此时，在任务栏已经显示一个窗口，但是不能显示
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "通过glfw的辅助创建窗口", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "窗口创建失败" << std::endl;
		// 终止整个opengl程序，释放资源
        glfwTerminate();
        return -1;
    }

	// 参数时已经在窗口中创建的窗口
	// 这个函数的作用就是确定绑定这个窗口的线程
    glfwMakeContextCurrent(window);

	// 设置回调函数，这个回调函数的作用就是告诉opengl帧缓冲是多少
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
	// opengl的API是以函数指针存在的，这个就是加载所有的函数指针
    // ---------------------------------------
	// typedef void* (* GLADloadproc)(const char *name);
	// 将glfwGetProcAddress返回函数的指针转换成 GLADloadproc类型函数
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    // render loop
	// 渲染，需要循环，因为显示器实在刷新的
    // -----------
	// ShouldClose：关闭窗口
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
		// 处理输入，参数是窗口，window里面包含窗口里面发生的事件
		// GLFWwindow *window
		// GLFWwindow是一个结构体，里面包含这个窗口发生的事件
        processInput(window);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
