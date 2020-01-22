#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
void render();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// shader


// camera
Camera camera(glm::vec3(0.0f, 0.0f, 8.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

int main(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for MacOS
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "getstarted", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	int window_width = mode->width;
	int window_height = mode->height;
	glfwSetWindowPos(window, (window_width - SCR_WIDTH) / 2, (window_height - SCR_HEIGHT) / 2);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	Shader objectShader = Shader("assets/object.vert", "assets/object.frag");
	Shader lampShader = Shader("assets/lamp.vert", "assets/lamp.frag");
	Shader materialShader = Shader("assets/material.vert", "assets/material.frag");

	// =================================================================================================  cube

	float verticesCube[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
	};
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// normal vector attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	// we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// normal vector attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// =================================================================================================  texture

	unsigned int texture1, texture2;
	// texture 1
	// ---------
	texture1 = loadTexture("assets/metal.jpg");
	texture2 = loadTexture("assets/pokeball.png");

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	objectShader.use();
	objectShader.setInt("uTexture1", 0);
	objectShader.setInt("uTexture2", 1);

	// =================================================================================================  light maps
	unsigned int diffuseMap, specularMap, emissionMap;
	diffuseMap = loadTexture("assets/container2.png");
	specularMap = loadTexture("assets/container2_specular.png");
	emissionMap = loadTexture("assets//matrix.jpg");
	materialShader.use();
	materialShader.setInt("material.diffuse", 0);
	materialShader.setInt("material.specular", 1);
	materialShader.setInt("material.emission", 2);
	// =================================================================================================  matrices

	glm::vec3 cubePositions[20] = {
	  glm::vec3(-2.0f,  0.3f,  -4.0f),
	  glm::vec3(2.0f,  2.1f, -3.0f),
	  glm::vec3(-1.5f, -2.2f, 1.5f),
	  glm::vec3(-3.8f, -2.0f, 2.3f),
	  glm::vec3(2.4f, -0.4f, -2.5f),
	  glm::vec3(-1.7f,  3.0f, 3.5f),
	  glm::vec3(1.3f, -2.0f, -2.5f),
	  glm::vec3(1.5f,  2.0f, 1.5f),
	  glm::vec3(1.5f,  0.2f, -1.3f),
	  glm::vec3(-1.3f,  1.0f, 2.8f)
	};

	glm::mat4 models[10];

	for (unsigned int i = 0; i < 10; i++)
	{
		models[i] = glm::mat4(1.0f);
		models[i] = glm::translate(models[i], cubePositions[i]);
	}

	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};
	
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	glm::mat4 materialModel = glm::mat4(1.0f);
	//materialModel = glm::translate(materialModel, glm::vec3(0.f, -0.3f, -4.2f));
	materialModel = glm::scale(materialModel, glm::vec3(1.2f));
	// =================================================================================================  render
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		//render();
		glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float timeValue = glfwGetTime();
		
		lightPos.x = 1.0f + cos(glfwGetTime()) * 2.0f;
		lightPos.z = sin(glfwGetTime() / 2.0f) * 1.0f;
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		// material shading cube
		materialShader.use();
		materialShader.setFloat("uTime", timeValue);
		materialShader.setFloat("material.shininess", 64.0f);
		materialShader.setVec3("viewPos", camera.Position);
		// -------------------------------------------------------------------------------------------------------- Lights
		// directional light
		materialShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		materialShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		materialShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		materialShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

		// point light 1
		materialShader.setVec3("pointLights[0].position", lightPos);
		materialShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		materialShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		materialShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		materialShader.setFloat("pointLights[0].constant", 1.0f);
		materialShader.setFloat("pointLights[0].linear", 0.09);
		materialShader.setFloat("pointLights[0].quadratic", 0.032);
		// point light 2
		materialShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		materialShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		materialShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		materialShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		materialShader.setFloat("pointLights[1].constant", 1.0f);
		materialShader.setFloat("pointLights[1].linear", 0.09);
		materialShader.setFloat("pointLights[1].quadratic", 0.032);
		// point light 3
		materialShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		materialShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		materialShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		materialShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		materialShader.setFloat("pointLights[2].constant", 1.0f);
		materialShader.setFloat("pointLights[2].linear", 0.09);
		materialShader.setFloat("pointLights[2].quadratic", 0.032);
		// point light 4
		materialShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		materialShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		materialShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		materialShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		materialShader.setFloat("pointLights[3].constant", 1.0f);
		materialShader.setFloat("pointLights[3].linear", 0.09);
		materialShader.setFloat("pointLights[3].quadratic", 0.032);

		// spotLight
		materialShader.setVec3("spotLight.position", camera.Position);
		materialShader.setVec3("spotLight.direction", camera.Front);
		materialShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		materialShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		materialShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		materialShader.setFloat("spotLight.constant", 1.0f);
		materialShader.setFloat("spotLight.linear", 0.09);
		materialShader.setFloat("spotLight.quadratic", 0.032);
		materialShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		materialShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		//materialShader.setMat4("model", materialModel);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, emissionMap);

		materialShader.setMat4("projection", projection);
		materialShader.setMat4("view", view);
		for (unsigned int i = 0; i < 10; i++)
		{
			models[i] = glm::rotate(models[i], deltaTime * cubePositions[i].y, glm::vec3(1.0f, 0.3f, 0.5f));
			materialShader.setMat4("model", models[i]);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// also draw the lamp object
		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		lampShader.setMat4("model", model);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		for (unsigned int i = 1; i < 4; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
			lampShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		        
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//glDeleteVertexArrays(1, &VAORect);
	//glDeleteBuffers(1, &VBORect);
	//glDeleteBuffers(1, &EBORect);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(SPACE, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

void render() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
