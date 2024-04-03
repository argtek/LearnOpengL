#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// glsl�﷨��ֱ�����ַ�������ʽ���룬����������б���
// ������ɫ��
// ������䣬�������ı�����Ҫopengl���֧��
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
// Ƭ����ɫ��
// Ƭ����ɫ����ͼԪ������ɫ���
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.5f, 0.5f, 0.9f, 1.0f);\n"
    "}\n\0";

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
	// ����һ��������ɫ������������ɫ�����ͣ�����ֵ����ɫ������
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// ȷ����ɫ��Դ���룬�󶨵���ɫ��vertexShader��
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// ��vertexShader����ȡ����䣬�����������ص��������
    glCompileShader(vertexShader);


    // check for shader compile errors
    int success;
    char infoLog[512];
	// iv��info verbose
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    // link shaders
	// ����һ����ɫ��С����������С������Ҫ�͵��Կ���alu����ִ��
    unsigned int shaderProgram = glCreateProgram();
	// ���ӣ�����ɫ��ʵ���С�����ܰ󶨣�����õ����С����������
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

	// С�����Ѿ���ɣ���ɫ���Ͳ���Ҫ��
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
	// ���ö�������,��ͼ��ѧ���棬������ľ��Ƕ��㣬���Ե�ͼ�Σ�3d���Ƕ��㹹�ɵģ����ԣ����ö��㼯�ǻ���
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
    }; 
    /*VAO �� Vertex Array Object ����OpenGL�������������ݵ�һ�����������������ܵ���ʹ�ã����ǽ��VBO��һ��ʹ�õġ�
	VAO��OpenGL CoreProfile �����һ�����ԡ���ʵ����CoreProfile�����������ݴ���ʱ������ʹ��VAO��ʽ��
	
	pram1:n
    Specifies the number of vertex array object names to generate.
    pram2:arrays
    Specifies an array in which the generated vertex array object names are stored.	\

	Description:
    glGenVertexArrays returns n vertex array object names in arrays. There is no guarantee that the names form a contiguous set of integers; however, it is guaranteed that none of the returned names was in use immediately before the call to glGenVertexArrays.
    Vertex array object names returned by a call to glGenVertexArrays are not returned by subsequent calls, unless they are first deleted with glDeleteVertexArrays.
    The names returned in arrays are marked as used, for the purposes of glGenVertexArrays only, but they acquire state and type only when they are first bound.*/
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
	/*
	generate buffer object names
	��һ��������Ҫ���ɵĻ��������������ڶ�����Ҫ���������洢����������Ƶ�����
	n:
	Specifies the number of buffer object names to be generated.
	buffers:
	Specifies an array in which the generated buffer object names are stored.
	*/
	// ���㻺��ռ����
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).

	glBindVertexArray(VAO);
	// ��һ�����ǻ����������ͣ�
    // �ڶ�����������Ҫ�󶨵Ļ����������ƣ�Ҳ������������һ�����������ɵ����ƣ�
	// ʹ�øú������������󶨵�OpenGL�����Ļ������Ա�ʹ��
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GLfloat r = 0;
	GLfloat g = 0;
	GLfloat b = 0;

    // render loop
    // -----------
	int i = 0;
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
		// ������ɫ
        //glClearColor(0.5f, 0.6f, 0.3f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);
				//for (int i = 0; i < 100; i++)
		{
			if ((1 - r) < 1e-5 && (1 - g) < 1e-5 && (1 - b) < 1e-5)
			{
				r = 0;
				g = 0;
				b = 0;

			}
			r += 0.0001;
			g += 0.0001;
			b += 0.0001;
			glClearColor(r, g, b, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			//Sleep(1);
		}

		//std::cout << i++ << std::endl;

        // draw our first triangle
		// ʹ��Ӧ������õ�С����
        glUseProgram(shaderProgram);
		// ʹ�õ��˶����������
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        // �����ͼԪ���ƣ�����1�ǻ���ʲô�������Ǽ򵥵ĵ㣬���Ƕ������ӳɵ��ߣ��������������ƽ��
		glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); // no need to unbind it every time 
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
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