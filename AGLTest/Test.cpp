// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// SOIL
#include <SOIL/SOIL.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// PA
#include <portaudio.h>

#include "Test.h"
#include "GLFWApplication.h"
#include "Renderable.h"
#include "Texture.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Sprite2D.h"
#include "Text.h"
#include "FBO.h"
#include "debug.h"
#include "sound/Mixer.h"
#include "sound/Reader.h"
#include "sound/Sound.h"

// Others
#include <iostream>
#include <memory>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#ifdef _MSC_VER
#pragma warning(disable: 4099)
#endif

class AGLTest;

class Boxes : agl::Renderable {
public:
	Boxes(AGLTest* a);
	void setUp();
	void tearDown() {
		delete container;
		delete awesome; // aww
		delete vbo;
		delete ebo;
		delete vao;
		delete program;
	}
	void tick();
	void update() {
	}
private:
	agl::Texture* container;
	agl::Texture* awesome;
	agl::VBO* vbo;
	agl::EBO* ebo;
	agl::VAO* vao;
	agl::ShaderProgram* program;
	AGLTest* app;
};

#define DIGIT_WIDTH 14.8f
#define DIGIT_HEIGHT 18
#define DIGIT_BASEX 16
#define DIGIT_BASEY 230

#define NUM_WORDS 7
const char* fnames[NUM_WORDS] = {
	"sounds/I.ogg", "sounds/will.ogg", "sounds/shank.ogg", "sounds/your.ogg",
	"sounds/fuck.ogg", "sounds/king.ogg", "sounds/mom.ogg"
};

class AGLTest : public agl::GLFWApplication {
public:
	using agl::GLFWApplication::GLFWApplication;
	void initialise() {
		std::cout << "hi\n";
		agl::FBOTexMS ft = agl::makeFBOForMeMS(800, 600);
		fboTex = ft.ss.texture;
		fboTexMS = ft.ms.texture;
		fboSS = ft.ss.fbo;
		fboMS = ft.ms.fbo;
		boxes = new Boxes(this);
		stex = std::make_shared<agl::Texture>(std::move(agl::Texture("textures/fuckyou.png")));
		sprites = new agl::Sprite2D(&*stex);
		sprites->setApp(this);
		sprites->addSprite({
			{0, 448, 512, 512},
			{0, 0, 512, 64}
		});
		sprites->addSprite({
			{DIGIT_BASEX, DIGIT_BASEY, DIGIT_BASEX + 4 * DIGIT_WIDTH, DIGIT_BASEY + DIGIT_HEIGHT},
			{50, 500, 50 + 4 * DIGIT_WIDTH, 500 + DIGIT_HEIGHT}
		});
		sprites->addSprite({
			{ DIGIT_BASEX + 14 * DIGIT_WIDTH, DIGIT_BASEY, DIGIT_BASEX + 15 * DIGIT_WIDTH, DIGIT_BASEY + DIGIT_HEIGHT },
			{50 + 7 * DIGIT_WIDTH, 500, 50 + 8 * DIGIT_WIDTH, 500 + DIGIT_HEIGHT}
		});
		for (int i = 0; i < 5; ++i) {
			GLfloat x = 50 + (4 + i + (i >= 3)) * DIGIT_WIDTH;
			sprites->addSprite({
				{0, DIGIT_BASEY, 0, DIGIT_BASEY + DIGIT_HEIGHT},
				{x, 500, x + DIGIT_WIDTH, 500 + DIGIT_HEIGHT}
			});
		}
		sprites->setUp();
		fy = new agl::Text();
		fy->setApp(this);
		fy->setMargin(240);
		fy->setFont("Segoe UI");
		fy->setSize(0.7);
		fy->setTopColour(glm::vec4(0.7f, 0.0f, 0.3f, 1.0f));
		fy->setBottomColour(glm::vec4(0.3f, 0.7f, 0.0f, 0.8f));
		fy->setRich(true);
		//fy->setText("Fuck you!");
		fy->setText(u8"<i>Bad</i> translation\n<u>Κακή μετάφραση</u>\nMala traducción\nплохой перевод\n下手な翻訳\n잘못된 번역\nתרגום גרוע\nترجمة سيئة\nD́ȉa͟c̈r̆ȉt̂ics\nThe Touhou Project (東方Project Tōhō Purojekuto, lit. Eastern Project), also known as Toho Project or Project Shrine Maiden, is a series of Japanese bullet hell shooter video games developed by the single-person Team Shanghai Alice. Team Shanghai Alice's sole member, <b>ZUN</b>, independently produces the games' graphics, music, and programming.\n東方Project（とうほうプロジェクト）とは、同人サークルの上海アリス幻樂団によって製作されている著作物である。弾幕系シューティングを中心としたゲーム、書籍、音楽CDなどから成る。東方Projectの作品を一括して東方、東方Projectシリーズなどと称することもある。狭義には、上海アリス幻樂団のメンバー「<b>ZUN</b>」が制作している同人作品の一連の作品をあらわす。");
		fy->setPosition(glm::vec2(530, 20));
		fy->setUp();
		vtex = std::make_shared<agl::Texture>(std::move(*fboTex));
		view = new agl::Sprite2D(&*vtex);
		view->setApp(this);
		view->addSprite({
			{0, 600, 800, 0},
			{0, 0, 800, 600}
		});
		view->addSprite({
			{0, 600, 800, 0},
			{640, 0, 800, 120}
		});
		view->setUp();
		for (int i = 0; i < NUM_WORDS; ++i) {
			mixer.addSound(i, std::move(agl::Sound(fnames[i])));
			wasPressed[i] = false;
		}
		PaError stat = Pa_Initialize();
		if (stat != paNoError) throw "Failed to initialise PortAudio.";
		mixer.regist();
		//setVSyncEnable(false);
	}
	bool ff = true;
	void tick() {
		fboMS->setActive();
		glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		++frame;
		int which = (frame >> 6) & 3;
		sprites->getLoc(0)->source.top = (GLfloat) (448 - (which << 6));
		sprites->getLoc(0)->source.bottom = (GLfloat) (512 - (which << 6));
		int rfps = (int) (getRollingFPS() * 100);
		for (int i = 4; i >= 0; --i) {
			setDigit(i, rfps % 10);
			rfps /= 10;
		}
		sprites->update();
		boxes->tick();
		sprites->tick();
		//for (int i = 0; i < 1; ++i) fy->relayout();
		fy->tick();
		fboMS->blitTo(*fboSS, 800, 600);
		agl::setDefaultFBOAsActive();
		glClearColor(1.0f, 0.5f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		view->tick();
		char ctitle[256];
		snprintf(ctitle, 255, "TestApp @ GL %s | FPS: %lf", glGetString(GL_VERSION), getRollingFPS());
		glfwSetWindowTitle(underlying(), ctitle);
	}
	void readKeys() {
		// Test if window should close
		if (testKey(GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(underlying(), GL_TRUE);
		// Camera controls
		GLfloat cameraSpeed = 5.0f * (GLfloat) getDelta();
		if (testKey(GLFW_KEY_W))
			cameraPos += cameraSpeed * cameraFront;
		if (testKey(GLFW_KEY_S))
			cameraPos -= cameraSpeed * cameraFront;
		if (testKey(GLFW_KEY_A))
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (testKey(GLFW_KEY_D))
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		// Slider controls
		if (testKey(GLFW_KEY_UP)) mix += 0.005f;
		if (testKey(GLFW_KEY_DOWN)) mix -= 0.005f;
		for (int i = 0; i < NUM_WORDS; ++i) {
			bool t = testKey(GLFW_KEY_1 + i);
			if (t && !wasPressed[i]) {
				mixer.playSound(i, 1);
				wasPressed[i] = true;
			} else if (!t) wasPressed[i] = false;
		}
	}
	void onMouse(double xpos, double ypos) {
		if (firstMouse) {
			lastX = (GLfloat) xpos;
			lastY = (GLfloat) ypos;
			firstMouse = false;
		}
		GLfloat xoffset = (GLfloat) xpos - lastX;
		GLfloat yoffset = lastY - (GLfloat) ypos;
		lastX = (GLfloat) xpos;
		lastY = (GLfloat) ypos;
		GLfloat sensitivity = 0.10f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		yaw += xoffset;
		pitch += yoffset;
		if (pitch > 89.0f) pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;
		glm::vec3 front;
		front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		cameraFront = glm::normalize(front);
	}
	void start() {
		GLFWApplication::start();
	}
	~AGLTest() {
		delete boxes;
		delete sprites;
		delete fy;
		delete view;
		mixer.stop();
		Pa_Terminate();
	}
	GLfloat getMix() { return mix; }
	GLfloat mix = 0.0f;
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
	GLfloat lastX = 400, lastY = 300;
	GLfloat yaw = 90, pitch = 0;
	bool firstMouse = true;
	GLfloat fov = 45.0f;
	Boxes* boxes;
	std::shared_ptr<agl::Texture> stex;
	std::shared_ptr<agl::Texture> vtex;
	agl::Sprite2D* sprites;
	agl::Text* fy;
	std::shared_ptr<agl::FBO> fboMS;
	std::shared_ptr<agl::FBO> fboSS;
	std::shared_ptr<agl::Texture> fboTex;
	std::shared_ptr<agl::Texture> fboTexMS;
	agl::Sprite2D* view;
	int frame = 0;
	void setDigit(int i, int v) {
		agl::Sprite2DInfo* spr = sprites->getLoc(3 + i);
		spr->source.left = DIGIT_BASEX + (4 + v) * DIGIT_WIDTH;
		spr->source.right = DIGIT_BASEX + (5 + v) * DIGIT_WIDTH;
	}
	agl::Mixer mixer;
	bool wasPressed[NUM_WORDS];
};

Boxes::Boxes(AGLTest* a) {
	app = a;
	setUp();
}
void Boxes::setUp() {
	container = new agl::Texture("textures/container.jpg");
	awesome = new agl::Texture("textures/awesomeface.png");
	vbo = new agl::VBO();
	ebo = new agl::EBO();
	agl::Shader* vertexShader = agl::openShaderFromFile("shader/vertex.glsl", GL_VERTEX_SHADER);
	agl::Shader* fragmentShader = agl::openShaderFromFile("shader/fragment.glsl", GL_FRAGMENT_SHADER);
	program = new agl::ShaderProgram();
	program->attach(*vertexShader);
	program->attach(*fragmentShader);
	program->link();
	program->use();
	delete vertexShader;
	delete fragmentShader;
	vao = new agl::VAO();
	vao->setActive();
	vbo->feedData(sizeof(vertices), vertices, GL_STATIC_DRAW);
	ebo->feedData(sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
}
void Boxes::tick() {
	glDisable(GL_BLEND);
	vao->setActive();
	program->use();
	glEnable(GL_DEPTH_TEST);
	container->bindTo(0);
	SETUNSP(*program, 1i, "ourTexture", 0);
	awesome->bindTo(1);
	SETUNSP(*program, 1i, "ourTexture2", 1);
	SETUNSP(*program, 1f, "m", app->mix);
	// GLfloat radius = 10.0f;
	// GLfloat camX = (GLfloat) sin(glfwGetTime()) * radius;
	// GLfloat camZ = (GLfloat) cos(glfwGetTime()) * radius;
	glm::mat4 view;
	view = glm::lookAt(
		app->cameraPos, // position
		app->cameraPos + app->cameraFront, // target
		app->cameraUp); // up
	glm::mat4 projection;
	GLfloat aspect = ((GLfloat) app->getWidth()) / app->getHeight();
	projection = glm::perspective(glm::radians(app->fov), aspect, 0.1f, 100.0f);
	for (int i = 0; i < 10; ++i) {
		glm::mat4 model;
		model = glm::translate(model, cubePositions[i]);
		model = glm::rotate(model, glm::radians((GLfloat) ((i % 3 == 0 ? glfwGetTime() * 50.0f : 0) + i * 20.0f)), glm::vec3(0.5f, 1.0f, 0.0f));
		SETUNSPM(*program, 4fv, "model", glm::value_ptr(model));
		SETUNSPM(*program, 4fv, "view", glm::value_ptr(view));
		SETUNSPM(*program, 4fv, "projection", glm::value_ptr(projection));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}
}

int main(int argc, char** argv) {
	(void) argc;
	(void) argv;
	glfwInit();
	try {
		// Kriët ė test wýndö
		AGLTest* a = new AGLTest(800, 600, 0, 0, u8"AGL Test App", 4, 5, true);
		// AGLTest* a = new AGLTest(800, 600, 0, 0, u8"AGL Test App", 3, 3, false);
		a->start();
	} catch (char const* s) {
		std::cout << u8"An error has Okuued!\n\n" << s << u8"\n\n";
		getchar();
	} catch (char* s) {
		std::cout << u8"An error has Okuued!\n\n" << s << u8"\n\n";
		getchar();
	}
	glfwTerminate();
}
