#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// ������������Ϊ�������ı��룬�����ǰ���˳�����������ݿ�ģ�����ʹ�ú����������ͻ�����ڷ������ݿ������Ҳ�����Ӧ����
/*!  GLFWwindow @brief Opaque window object.
 *
 *  Opaque ����͸���� window object.
 *
 *  @ingroup window
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// ���봦��
void processInput(GLFWwindow *window);

// settings
// ���ڴ�С
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
	// ��ʼ�����gl�������
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
	// 	glfwCreateWindow��glfw��ܴ��룬�����Ǵ������ڣ�glfw���Ƕ�opengl�İ�װ����opengl�Ľӿڽ�����װ���򻯳����ܽӿ�
	// ʵ�־��峣�õĹ���
	// ������ǽ����opengl API������ϣ���ϳ����ڴ�������
	// ��ʱ�����������Ѿ���ʾһ�����ڣ����ǲ�����ʾ
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ͨ��glfw�ĸ�����������", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "���ڴ���ʧ��" << std::endl;
		// ��ֹ����opengl�����ͷ���Դ
        glfwTerminate();
        return -1;
    }

	// ����ʱ�Ѿ��ڴ����д����Ĵ���
	// ������������þ���ȷ����������ڵ��߳�
    glfwMakeContextCurrent(window);

	// ���ûص�����������ص����������þ��Ǹ���opengl֡�����Ƕ���
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
	// opengl��API���Ժ���ָ����ڵģ�������Ǽ������еĺ���ָ��
    // ---------------------------------------
	// typedef void* (* GLADloadproc)(const char *name);
	// ��glfwGetProcAddress���غ�����ָ��ת���� GLADloadproc���ͺ���
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    // render loop
	// ��Ⱦ����Ҫѭ������Ϊ��ʾ��ʵ��ˢ�µ�
    // -----------
	// ShouldClose���رմ���
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
		// �������룬�����Ǵ��ڣ�window��������������淢�����¼�
		// GLFWwindow *window
		// GLFWwindow��һ���ṹ�壬�������������ڷ������¼�
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
