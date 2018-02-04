#define GLEW_STATIC
#include "shprogram.h"
#include "Cylinder.h"
#include "Cuboid.h"
#include "LightSource.h"
#include "Saw.h"
#include "Skybox.h"
#include "HalfCylinder.h"
using namespace std;

GLfloat rot_y = 0.0f;
GLfloat rot_x = 0.0f;
GLfloat fov = 45.0f;
GLfloat deltaY = 0.0001f;
GLfloat sinMagnitude = 1.0f;
GLfloat lightX = 0.0f;
GLfloat lightY = 20.0f;
GLfloat lightSensitivity = 1.0f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_LEFT)
		rot_y -= 2.0f;
	else if (key == GLFW_KEY_RIGHT)
		rot_y += 2.0f;
	else if (key == GLFW_KEY_UP)
		rot_x += 0.5f;
	else if (key == GLFW_KEY_DOWN)
		rot_x -= 0.5f;
	else if (key == GLFW_KEY_W)
		deltaY += 0.0001f;
	else if (key == GLFW_KEY_S)
		deltaY -= 0.0001f;
	else if (key == GLFW_KEY_A)
		sinMagnitude -= 0.01f;
	else if (key == GLFW_KEY_D)
		sinMagnitude += 0.01f;
	else if (key == GLFW_KEY_1)
		lightX += 0.1f;
	else if (key == GLFW_KEY_2)
		lightX -= 0.1f;
	else if (key == GLFW_KEY_0)
		lightY += 0.1f;
	else if (key == GLFW_KEY_9)
		lightY -= 0.1f;
	else if (key == GLFW_KEY_C)
		lightSensitivity += 0.01f;
	else if (key == GLFW_KEY_Z)
		lightSensitivity -= 0.01f;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float) yoffset;
}

GLuint LoadMipmapTexture(GLuint texId, const char* fname)
{
	int width, height;
	unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGB);
	if (image == nullptr)
		throw exception("Failed to load texture file");

	GLuint texture;
	glGenTextures(1, &texture);

	glActiveTexture(texId);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

int main()
{
	if (glfwInit() != GL_TRUE)
	{
		cout << "GLFW initialization failed" << endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	const GLuint WIDTH = 1280, HEIGHT = 720;
	try
	{
		GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GKOM - OpenGL 05", nullptr, nullptr);
		if (window == nullptr)
			throw exception("GLFW window not created");
		glfwMakeContextCurrent(window);
		glfwSetKeyCallback(window, key_callback);
		glfwSetScrollCallback(window, scroll_callback);

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
			throw exception("GLEW Initialization failed");

		glViewport(0, 0, WIDTH, HEIGHT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		ShaderProgram textureProgram("texture.vert", "texture.frag");
		ShaderProgram lightSourceProgram("light_source.vert", "light_source.frag");
		ShaderProgram skyboxProgram("skybox.vert", "skybox.frag");

		GLfloat woodRadius = 2.0f;
		GLfloat woodLength = 20.0f;
		GLuint slices = 50;
		Cylinder leg;
		HalfCylinder half(slices / 2, woodRadius, woodLength);

		GLuint numberOfKnives = 15;
		GLfloat sawWidth = 0.1f;
		GLfloat sawHeight = woodRadius;
		GLfloat sawLength = 1.2f * woodLength;
		Saw saw(numberOfKnives, sawWidth, sawHeight, sawLength, 10);

		GLfloat tableWidth = 8.0f * woodRadius;
		GLfloat tableLength = sawLength;
		GLfloat tableHeight = 1.0f;
		Cuboid table(tableWidth, tableHeight, tableLength);
		GLfloat pileVerWidth = woodRadius / 2;
		GLfloat pileVerHeight = woodRadius * 4;
		GLfloat pileVerLength = woodRadius / 2;
		Cuboid pileVer(pileVerWidth, pileVerHeight, pileVerLength, 5);
		GLfloat pileHorWidth = woodRadius / 2;
		GLfloat pileHorLength = woodRadius / 2;
		GLfloat pileHorHeight = woodRadius * 5 + pileVerWidth;
		Cuboid pileHor(pileHorWidth, pileHorHeight, pileHorLength, 5);

		glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 lightLocation = glm::vec3(lightX, lightY, -30.0f);
		LightSource ls (lightColor, lightLocation);

		Cuboid bottom(100.0f, 0.0f, 100.0f, 1);

		Skybox skybox;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLuint texture0 = LoadMipmapTexture(GL_TEXTURE0, "tree.png");
		GLuint texture1 = LoadMipmapTexture(GL_TEXTURE1, "metal.jpg");
		GLuint texture2 = LoadMipmapTexture(GL_TEXTURE2, "wood_cuboid.jpg");
		GLfloat y = pileVerHeight - 2 * pileVerLength;
		bool stillRotate = true;
		float time = (float) glfwGetTime();

		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();

			glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			textureProgram.Use();
			GLuint transformLoc = glGetUniformLocation(textureProgram.get_programID(), "transform");
			GLint viewLoc = glGetUniformLocation(textureProgram.get_programID(), "view");
			GLint projLoc = glGetUniformLocation(textureProgram.get_programID(), "projection");
			GLint lightColorLoc = glGetUniformLocation(textureProgram.get_programID(), "lightColor");
			GLint lightPositionLoc = glGetUniformLocation(textureProgram.get_programID(), "lightPosition");
			glm::mat4 trans;
			glm::mat4 camRot;
			glm::mat4 view;
			glm::mat4 projection;
			glm::mat4 skyView;
			glm::vec3 cameraPos;
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture0);
			glUniform1i(glGetUniformLocation(textureProgram.get_programID(), "Tex"), 0);

			if (rot_y >= 360.0f)
				rot_y -= 360.0f;
			else if (rot_y <= -360.0f)
				rot_y += 360.0f;

			if (rot_x >= 89.0f)
				rot_x = 89.0f;
			else if (rot_x <= 0.0f)
				rot_x = 0.0f;

			if (deltaY <= 0.0f)
				deltaY = 0.0f;

			if (sinMagnitude <= 0.0f)
				sinMagnitude = 0.0f;
			else if (sinMagnitude >= 3.0f)
				sinMagnitude = 3.0f;

			if (fov >= 45.0f)
				fov = 45.0f;
			else if (fov <= 22.5f)
				fov = 22.5f;

			if (lightX >= 30.0f)
				lightX = 30.0f;
			else if (lightX <= -30.0f)
				lightX = -30.0f;

			if (lightY >= 40.0f)
				lightY = 40.0f;
			else if (lightY <= 0.0f)
				lightY = 0.0f;

			if (lightSensitivity >= 1.0f)
				lightSensitivity = 1.0f;
			else if (lightSensitivity <= 0.0f)
				lightSensitivity = 0.0f;

			camRot = glm::rotate(camRot, glm::radians(rot_y), glm::vec3(0.0, 1.0, 0.0));
			camRot = glm::rotate(camRot, glm::radians(rot_x), glm::vec3(1.0f, 0.0f, 0.0f));
			cameraPos = glm::vec3(camRot * glm::vec4(0.0f, 0.0f, -40.0f, 1.0f));
			view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, tableLength / 2), glm::vec3(0.0f, 1.0f, 0.0f));
			projection = glm::perspective(glm::radians(fov), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
			glUniform3f(lightColorLoc, ls.getLightColor().x, ls.getLightColor().y, ls.getLightColor().z);
			glUniform3f(lightPositionLoc, ls.getLightPosition().x, ls.getLightPosition().y, ls.getLightPosition().z);
			if (y > 0)
			{
				float toTrans = y <= 2 * woodRadius ? y / (2 * woodRadius) : 1.0f;
				trans = glm::translate(trans, glm::vec3((1 - toTrans) * sawWidth / 2, 0.0f, 0.0f));
				glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
				half.draw();
				trans = glm::mat4(1.0f);
				trans = glm::translate(trans, glm::vec3(-(1 - toTrans) * sawWidth / 2, 0.0f, 0.0f));
				trans = glm::rotate(trans, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
				half.draw();
				time = (float) glfwGetTime();
			}
			else
			{
				float deg = glm::radians((float)glfwGetTime() - time);
				float toCheck = 50000 * glm::sin(deg) * glm::radians(deg);
				float theta = toCheck < glm::radians(90.0f) && stillRotate ? toCheck : glm::radians(90.0f);
				if (theta == glm::radians(90.0f))
					stillRotate = false;
				float toTrans = (woodRadius) * glm::sin(theta) + sawWidth / 2;
				trans = glm::translate(trans, glm::vec3(toTrans, 0, 0));
				trans = glm::rotate(trans, -theta, glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
				half.draw();
				trans = glm::mat4(1.0f);
				trans = glm::translate(trans, glm::vec3(-toTrans, 0, 0));
				trans = glm::rotate(trans, glm::radians(180.0f) + theta, glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
				half.draw();
			}

			trans = glm::mat4(1.0f);
			trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
			trans = glm::translate(trans, glm::vec3(-2 * woodRadius, 0.0, woodRadius + 0.005f));
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			leg.draw();
			trans = glm::translate(trans, glm::vec3(4 * woodRadius, 0.0, 0.0));
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			leg.draw();
			trans = glm::translate(trans, glm::vec3(0.0, woodLength, 0.0));
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			leg.draw();
			trans = glm::translate(trans, glm::vec3(-4 * woodRadius, 0.0, 0.0));
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			leg.draw();

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, texture2);
			glUniform1i(glGetUniformLocation(textureProgram.get_programID(), "Tex"), 2);
			trans = glm::mat4(1.0f);
			trans = glm::translate(trans, glm::vec3(0.0, -woodRadius, -woodRadius));
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			table.draw();

			trans = glm::mat4(1.0f);
			trans = glm::translate(trans, glm::vec3(2.5 * woodRadius, pileVerHeight - woodRadius, - woodRadius / 2));
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			pileVer.draw();
			trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, tableLength - woodRadius));
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			pileVer.draw();
			trans = glm::translate(trans, glm::vec3(-5 * woodRadius, 0.0f, 0.0f));
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			pileVer.draw();
			trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, woodRadius - tableLength));
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			pileVer.draw();

			trans = glm::mat4(1.0f);
			trans = glm::translate(trans, glm::vec3(- 2.5 * woodRadius - pileVerWidth / 2, pileVerHeight - woodRadius + pileVerLength / 2, -pileVerWidth));
			trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			pileHor.draw();
			trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, tableLength - woodRadius));
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			pileHor.draw();
			
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture1);
			glUniform1i(glGetUniformLocation(textureProgram.get_programID(), "Tex"), 1);
			trans = glm::mat4(1.0f);
			y = y - deltaY > 0.0f ? y - deltaY : 0.0f;
			trans = glm::translate(trans, glm::vec3(0.0, y, glm::sin(sinMagnitude * (glfwGetTime()) - woodRadius)));
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			saw.draw();

			lightSourceProgram.Use();
			lightLocation.x = lightX;
			lightLocation.y = lightY;
			ls.setLightColor(glm::vec3(lightSensitivity, lightSensitivity, lightSensitivity));
			ls.setLightPosition(lightLocation);
			trans = glm::mat4(1.0f);
			trans = glm::translate(trans, ls.getLightPosition());
			transformLoc = glGetUniformLocation(lightSourceProgram.get_programID(), "transform");
			viewLoc = glGetUniformLocation(lightSourceProgram.get_programID(), "view");
			projLoc = glGetUniformLocation(lightSourceProgram.get_programID(), "projection");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
			ls.draw();

			skyboxProgram.Use();
			trans = glm::mat4(1.0f);
			skyView = glm::mat4(glm::mat3(view));
			transformLoc = glGetUniformLocation(skyboxProgram.get_programID(), "transform");
			viewLoc = glGetUniformLocation(skyboxProgram.get_programID(), "view");
			projLoc = glGetUniformLocation(skyboxProgram.get_programID(), "projection");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(skyView));
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
			skybox.draw();

			glfwSwapBuffers(window);
		}
	}
	catch (exception ex)
	{
		cout << ex.what() << endl;
		system("pause");
	}
	glfwTerminate();

	return 0;
}
