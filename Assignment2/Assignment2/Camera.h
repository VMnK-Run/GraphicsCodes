#ifndef __CAMERA_H__
#define __CAMERA_H__
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>

//默认的相机值
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;       //速度
const float SENSITIVITY = 0.1f; //灵敏度
const float ZOOM = 45.0f;       //缩放级别

//一个抽象的相机类，它处理输入并计算相应的欧拉角度、矢量和矩阵，以便在OpenGL中使用
class Camera {
public:
	//为摄像机移动定义了几种可能的选项。 一种抽象的用来远离窗口系统的特定输入方法
	enum Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	//照相机属性
	glm::vec3 Position; //位置
	glm::vec3 Front;    //方向向量
	glm::vec3 Target;	//物体坐标
	glm::vec3 Distance; //摄像机到物体的向量
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	//欧拉角
	float Yaw;
	float Pitch;
	float lastAfa;
	float lastFi;
	bool fistMouse = true;

	//相机选项
	float MovementSpeed;    //键盘按键移动速度
	float MouseSensitivity; //鼠标灵敏度
	float Zoom;

	//用矢量的构造函数
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
		Position = position;
		
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
		Target = glm::vec3(0.0f, 0.0f, 0.0f);
		Distance = Target - Position;
		std::cout << "Distance:::::" << Distance.x << " " << Distance.y << " " << Distance.z << std::endl;
	}

	//返回使用欧拉角和LookAt矩阵计算的视图矩阵
	glm::mat4 GetViewMatrix() {
		return glm::lookAt(Position, Position + Front, Up);
	}

	//处理从任何类似键盘的输入系统接收的输入。 以摄像机定义的ENUM形式接受输入参数（从窗口系统中抽象出来）
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;
		Target = Position + Distance;
		//std::cout << "keyBoard!" << std::endl;
		//std::cout << "changed camera position: " << Position.x << " " << Position.y << " " << Position.z << std::endl;
		//std::cout << "changed Front:" << Front.x << " " << Front.y << " " << Front.z << std::endl;
		//std::cout << "target" << Target.x << " " << Target.y << " " << Target.z << std::endl;
		//std::cout << std::endl;
	}

	// 处理从鼠标输入系统接收的输入。 期待获取x和y方向的偏移值。
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;
		Yaw += xoffset;
		Pitch += yoffset;

		//确保当pitch超出范围时，屏幕不会翻转
		if (constrainPitch) {
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// 使用更新的欧拉角进而更新Front、Right和Up矢量
		updateCameraVectors();
	}

	// 处理从鼠标滚轮事件接收的输入。 只需要在垂直轮轴上的输入
	void ProcessMouseScroll(float yoffset)
	{
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
	}

	// 处理从鼠标输入的左键点击事件
	void ProcessMouseClickAndMove(float xoffset, float yoffset, GLboolean constrainPitch = true) {
		//std::cout << "rool==========" << std::endl;
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		float r = sqrt(Position.x * Position.x + Position.y * Position.y + Position.z * Position.z);
		//float r = sqrt((Position.x-Target.x) * (Position.x - Target.x) + (Position.y - Target.y) * (Position.y - Target.y) + (Position.z - Target.z) * (Position.z - Target.z));
		float x, y, z, afa, fi;
		if (Position.z > 0) {
			afa = atan(Position.x / Position.z);
		}
		else if (Position.z == 0) {
			afa = 0;
		}
		else {
			afa = atan(Position.x / Position.z) + 3.1415926;
		}

		float val = sqrt(Position.z * Position.z + Position.x * Position.x);
		// 防止屏幕翻转
		if (val != 0 && atan(Position.y / val) <= 3.1 / 2 && atan(Position.y / val) >= -3.1 / 2) {
			fi = atan(Position.y / val);
		}
		else {
			fi = lastFi;
		}
		lastFi = fi;
		lastAfa = afa;

		x = r * cos(fi + (-yoffset) / 180.0f * 3.1415926) * sin(afa + (-xoffset) / 180.0f * 3.1415926);
		y = r * sin(fi + (-yoffset) / 180.0f * 3.1415926);
		z = r * cos(fi + (-yoffset) / 180.0f * 3.1415926) * cos(afa + (-xoffset) / 180.0f * 3.1415926);
		Position = glm::vec3(x, y, z);
		//std::cout << "changed camera position: " << Position.x << " " << Position.y << " " << Position.z << std::endl;
		Front = glm::normalize(Target - Position);
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
		// 这里需要反向求解Yaw和Pitch
		Pitch = asin(Front.y) / 3.1415926 * 180;
		if (glm::radians(Pitch) != 0) {
			Yaw = asin(Front.z / cos(glm::radians(Pitch))) / 3.1415926 * 180;
			if (abs(Front.x - cos(glm::radians(Yaw)) * cos(glm::radians(Pitch))) > abs(0.000001)) {
				//std::cout << Front.x << " " << cos(glm::radians(Yaw)) * cos(glm::radians(Pitch)) << std::endl;
				if (Yaw > 0) {
					Yaw = 180 - Yaw;
				}
				else {
					Yaw = -180 - Yaw;
				}
				if (abs(Front.x - cos(glm::radians(Yaw)) * cos(glm::radians(Pitch))) > abs(0.000001));
			}
		}
		//std::cout << "changed Front:" << Front.x << " " << Front.y << " " << Front.z << std::endl;
		//std::cout << "target" << Target.x << " " << Target.y << " " << Target.z << std::endl;
		//std::cout << std::endl;
	}

private:
	// 从Camera（更新的）欧拉角计算Front向量
	void updateCameraVectors() {
		//std::cout << "noooo==========" << std::endl;
		//计算新的Front向量
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

		float k = sqrt(Distance.x*Distance.x +Distance.y*Distance.y + Distance.z*Distance.z) / sqrt(Front.x*Front.x+Front.y*Front.y+Front.z*Front.z);
		Front = glm::normalize(front);
		Distance = Front * k;  //更新该矩阵

		//同样，重新计算Right和Up向量
		
		//单位化矢量，因为它们的长度越接近0，你向上或向下看时时间越多，导致移动速度变慢。
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
		Target = Position + Distance;
		
		//std::cout << "changed camera position: " << Position.x << " " << Position.y << " " << Position.z << std::endl;
		//std::cout << "changed Front:" << Front.x << " " << Front.y << " " << Front.z << std::endl;
		//std::cout << "Distance" << Distance.x << " " << Distance.y << " " << Distance.z << std::endl;
		//std::cout << "target" << Target.x << " " << Target.y << " " << Target.z << std::endl;
		//std::cout << std::endl;
	}
};
#endif