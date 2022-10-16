//头文件
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include <iostream>
//-----------------------------------函数声明-------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void mouseButton(GLFWwindow* window, int button, int action, int mods);
//-------------------------------------全局变量-------------------------------------------
//窗体宽高
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
//摄像机相关
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
// 时间
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool left_flag = false;
bool right_flag = false;

//主函数
int main()
{
	// glfw: 初始化和配置
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw 窗体创建
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ModelDemo", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "创建GLFW窗体失败" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouseButton);
	glfwSetScrollCallback(window, scroll_callback);

	// 鼠标滑动回调函数
	//(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: 加载OpenGL函数指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "初始化GLAD失败" << std::endl;
		return -1;
	}

	// 设置全局opengl状态
	glEnable(GL_DEPTH_TEST);//开启深度测试

	//创建并编译shader
	Shader ourShader("vertexSource.txt", "fragmentSource.txt");

	// 加载模型
	//FileSystem::getPath("resources/objects/nanosuit/nanosuit.obj")
	//修改为相对路径
	Model ourModel("../resources/nanosuit/nanosuit.obj");
	//Model ourModel("../Debug/model/warrior/arakkoa_warrior.obj");
	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//循环渲染
	while (!glfwWindowShouldClose(window))
	{
		//获取时间
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// 键盘输入
		processInput(window);
		// 渲染
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// 设置uniforms前使用Shader
		ourShader.use();
		// view/projection矩阵
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);
		// 渲染加载的3d模型
		glm::mat4 model = glm::mat4(1.0f);
		//使其位于场景的中心
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		//缩小它
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);
		// glfw: 交换缓冲区和轮询IO事件（按下/释放按键，移动鼠标等）
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: 终止，清除所有先前分配的GLFW资源。
	glfwTerminate();
	return 0;
}
//键盘按键控制
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera::RIGHT, deltaTime);
}

void mouseButton(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		left_flag = true;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:

		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		right_flag = true;
		break;
	default:
		return;
	}
	else if (action == GLFW_RELEASE) {
		if (left_flag) left_flag = false;
		if (right_flag) right_flag = false;
	}
	return;
}

// glfw: 窗口改变回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// glfw: 鼠标滑动回调函数
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	//cout << xpos << " " << ypos << endl;
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	if (right_flag) {
		//cout << camera.Position.x << " " << camera.Position.y << " " << camera.Position.z << endl;
		camera.ProcessMouseMovement(xoffset, yoffset);
	}
	if (left_flag) {
		//cout << camera.Position.x << " " << camera.Position.y << " " << camera.Position.z << endl;
		camera.rool(xoffset, yoffset);
		//camera.update(glm::vec3(x, y, z), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f + afa / 3.1415926 * 180.0 + xoffset - 83.6598f, 0.0f + fi / 3.1415926 * 180.0 + yoffset - 69.7911f);
	}

}
// glfw: 鼠标滚轮回调函数
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

