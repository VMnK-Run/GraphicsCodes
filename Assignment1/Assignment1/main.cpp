//ͷ�ļ�
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include <iostream>
//-----------------------------------��������-------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void mouseButton(GLFWwindow* window, int button, int action, int mods);
//-------------------------------------ȫ�ֱ���-------------------------------------------
//������
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
//��������
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
// ʱ��
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool left_flag = false;
bool right_flag = false;

//������
int main()
{
	// glfw: ��ʼ��������
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw ���崴��
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ModelDemo", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "����GLFW����ʧ��" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouseButton);
	glfwSetScrollCallback(window, scroll_callback);

	// ��껬���ص�����
	//(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: ����OpenGL����ָ��
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "��ʼ��GLADʧ��" << std::endl;
		return -1;
	}

	// ����ȫ��opengl״̬
	glEnable(GL_DEPTH_TEST);//������Ȳ���

	//����������shader
	Shader ourShader("vertexSource.txt", "fragmentSource.txt");

	// ����ģ��
	//FileSystem::getPath("resources/objects/nanosuit/nanosuit.obj")
	//�޸�Ϊ���·��
	Model ourModel("../resources/nanosuit/nanosuit.obj");
	//Model ourModel("../Debug/model/warrior/arakkoa_warrior.obj");
	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//ѭ����Ⱦ
	while (!glfwWindowShouldClose(window))
	{
		//��ȡʱ��
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// ��������
		processInput(window);
		// ��Ⱦ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// ����uniformsǰʹ��Shader
		ourShader.use();
		// view/projection����
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);
		// ��Ⱦ���ص�3dģ��
		glm::mat4 model = glm::mat4(1.0f);
		//ʹ��λ�ڳ���������
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		//��С��
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);
		// glfw: ��������������ѯIO�¼�������/�ͷŰ������ƶ����ȣ�
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: ��ֹ�����������ǰ�����GLFW��Դ��
	glfwTerminate();
	return 0;
}
//���̰�������
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

// glfw: ���ڸı�ص�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// glfw: ��껬���ص�����
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
// glfw: �����ֻص�����
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

