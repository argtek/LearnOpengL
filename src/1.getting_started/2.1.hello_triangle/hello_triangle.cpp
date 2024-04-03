#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// glsl语法，直接以字符串的形式插入，在这里面进行编译
// 顶点着色器
// 程序语句，这个程序的编译需要opengl库的支持
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
// 片段着色器
// 片段着色器对图元进行天色填充
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
	// 创建一个顶点着色器，参数是着色器类型，返回值的着色器索引
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// 确定着色器源代码，绑定到着色器vertexShader上
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// 从vertexShader里面取得语句，将编译结果返回到这个对象
    glCompileShader(vertexShader);


    // check for shader compile errors
    int success;
    char infoLog[512];
	// iv：info verbose
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
	// 创建一个着色器小程序对象，这个小程序需要送到显卡的alu里面执行
    unsigned int shaderProgram = glCreateProgram();
	// 链接，将着色器实体和小程序框架绑定，编译好的填充小程序框架里面
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

	// 小程序已经完成，着色器就不需要了
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
	// 设置顶点数据,在图形学里面，最基础的就是顶点，所以的图形，3d都是顶点构成的，所以，设置顶点集是基础
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
    }; 
    /*VAO （ Vertex Array Object ）是OpenGL用来处理顶点数据的一个缓冲区对象，它不能单独使用，都是结合VBO来一起使用的。
	VAO是OpenGL CoreProfile 引入的一个特性。事实上在CoreProfile中做顶点数据传入时，必须使用VAO方式。
	
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
	第一个参数是要生成的缓冲对象的数量，第二个是要输入用来存储缓冲对象名称的数组
	n:
	Specifies the number of buffer object names to be generated.
	buffers:
	Specifies an array in which the generated buffer object names are stored.
	*/
	// 顶点缓冲空间对象
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).

	glBindVertexArray(VAO);
	// 第一个就是缓冲对象的类型，
    // 第二个参数就是要绑定的缓冲对象的名称，也就是我们在上一个函数里生成的名称，
	// 使用该函数将缓冲对象绑定到OpenGL上下文环境中以便使用
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
		// 背景颜色
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
		// 使用应景编译好的小程序
        glUseProgram(shaderProgram);
		// 使用到了顶点数组对象
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        // 这个是图元绘制，参数1是绘制什么东西，是简单的点，还是顶点连接成的线，还是三角形这个平面
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