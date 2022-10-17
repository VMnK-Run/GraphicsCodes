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
void mouse_click_callback(GLFWwindow* window, int button, int action, int mods);
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

bool leftFlag = false;
bool rightFlag = false;
glm::mat4 trans = glm::mat4(1.0f);

//主函数
int main() {
	// glfw: 初始化和配置
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw 窗体创建
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LoadObjModel", NULL, NULL);
	if (window == NULL) {
		std::cout << "创建GLFW窗体失败" << std::endl;
		glfwTerminate();
		return -1;
	}

	// 注册回调函数
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_click_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// glad: 加载OpenGL函数指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "初始化GLAD失败" << std::endl;
		return -1;
	}

	// 设置全局opengl状态
	glEnable(GL_DEPTH_TEST);//开启深度测试
	glDepthFunc(GL_LEQUAL);

	//创建并编译shader
	Shader ourShader("vertexSource.txt", "fragmentSource.txt");

	// 加载模型
	Model ourModel("../resources/nanosuit/nanosuit.obj");
	// draw in wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//循环渲染
	while (!glfwWindowShouldClose(window)) {
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
		ourShader.setMat4("transform", trans);


		// 渲染加载的3d模型
		glm::mat4 model = glm::mat4(1.0f);
		//使其位于场景的中心
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		//缩小它
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
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
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera::FORWARD, deltaTime);
		cout << "Press Key W\n";
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera::BACKWARD, deltaTime);
		cout << "Press Key S\n";
	}
		
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera::LEFT, deltaTime);
		cout << "Press Key A\n";
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera::RIGHT, deltaTime);
		cout << "Press Key D\n";
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		camera.ProcessMouseClickAndMove(0.5, 0);
		cout << "Press Key J\n";
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		camera.ProcessMouseClickAndMove(-0.5, 0);
		cout << "Press Key K\n";
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		trans = glm::translate(trans, glm::vec3(-(float)glfwGetTime() / 5000, 0.0f, 0.0f));
		cout << "Press Key Left\n";
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		trans = glm::translate(trans, glm::vec3((float)glfwGetTime() / 5000, 0.0f, 0.0f));
		cout << "Press Key Right\n";
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		trans = glm::translate(trans, glm::vec3(0.0f, (float)glfwGetTime() / 5000, 0.0f));
		cout << "Press Key Up\n";
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		trans = glm::translate(trans, glm::vec3(0.0f, -(float)glfwGetTime() / 5000, 0.0f));
		cout << "Press Key Down\n";
	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		trans = glm::rotate(trans, (float)glfwGetTime() / 5000, glm::vec3(0.0f, 0.0f, 1.0f));
		cout << "Press Key U\n";
		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		cout << "Press Key I\n";
		trans = glm::rotate(trans, (float)glfwGetTime() / 5000, glm::vec3(0.0f, 0.0f, -1.0f));
	}
}

void mouse_click_callback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		cout << "Click Left\n";
		leftFlag = true;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:

		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		cout << "Click Right\n";
		rightFlag = true;
		break;
	default:
		return;
	}
	else if (action == GLFW_RELEASE) {
		if (leftFlag) leftFlag = false;
		if (rightFlag) rightFlag = false;
	}
	return;
}

// glfw: 窗口改变回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// glfw: 鼠标滑动回调函数
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	cout << xpos << " " << ypos << endl;
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	if (rightFlag) {
		camera.ProcessMouseMovement(xoffset, yoffset);
	}
	if (leftFlag) {
		//cout << camera.Position.x << " " << camera.Position.y << " " << camera.Position.z << endl;
		camera.ProcessMouseClickAndMove(xoffset, yoffset);
	}

}
// glfw: 鼠标滚轮回调函数
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}

