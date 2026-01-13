#include "Engine.h"

void myupdateloop(GLFWwindow* window, double tDelta);
void mykeyboardhandler(GLFWwindow* window, int key, int scancode, int action, int mods);
void playercontrol(double tDelta);
void updateAsteroid(GameObject2D* asteroid, double TDelta, float speed);


// Function prototypes

bool uparrow, leftarrow, rightarrow, downarrow, spacebar;
static float playerspeed = 0.0f;
const float acceleration = 0.01f; 
const float bulletspeed = 0.002f; 
GameObject2D* player;
GameObject2D* astroid1, *asteroid2, *asteroid3, *asteroid4; 
float maxspeed = 0.05f;

GameObject2D* asteroids = new GameObject2D[10];

// Global Variables
float moveamount;
float bulletmoveamount;

int main(void) {

	// Initialise the engine (create window, setup OpenGL backend)
	int initResult = engineInit("GDV4002 - Applied Maths for Games", 1024, 1024);

	// If the engine initialisation failed report error and exit
	if (initResult != 0) {

		printf("Cannot setup game window!!!\n");
		return initResult; // exit if setup failed
	}

	// background remove for the assets, allowing transparency 
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_ALWAYS);

	//
	// Setup game scene objects here
	//

	//the next section is the veriable initialization
	
	float pi = 3.14159265f;

	// const char* name, glm::vec2 initPosition, float initOrientation, glm::vec2 initSize, const char* texturePath, TextureProperties texProperties
		addObject("player", glm::vec2(0.0f, 0.0f), glm::radians(90.0f), glm::vec2(1.0f, 1.0f), "Resources\\Textures\\player1_ship.png", TextureProperties::NearestFilterTexture());
	addObject("asteroid1", glm::vec2(1.0f, 1.0f), 0.0f, glm::vec2(1.0f, 1.0f), "Resources\\Textures\\mcblock01.png", TextureProperties::NearestFilterTexture());
	addObject("asteroid2", glm::vec2(1.0f, -1.0f), 0.0f, glm::vec2(1.0f, 1.0f), "Resources\\Textures\\mcblock01.png", TextureProperties::NearestFilterTexture());
	addObject("asteroid3", glm::vec2(-1.0f, 1.0f), 0.0f, glm::vec2(1.0f, 1.0f), "Resources\\Textures\\mcblock01.png", TextureProperties::NearestFilterTexture());
	addObject("asteroid4", glm::vec2(-1.0f, -1.0f), 0.0f, glm::vec2(1.0f, 1.0f), "Resources\\Textures\\mcblock01.png", TextureProperties::NearestFilterTexture());
	addObject("bullet", glm::vec2(-20.0f, -20.0f), 0.0f, glm::vec2(0.2f, 0.2f), "Resources\\Textures\\bullet.png", TextureProperties::NearestFilterTexture());
	player = getObject("player");
	asteroid1 = getObject("asteroid1");
	asteroid2 = getObject("asteroid2");
	asteroid3 = getObject("asteroid3");
	asteroid4 = getObject("asteroid4");

	// Asteroid Control

	asteroid1->orientation = glm::radians(45.0f);
	asteroid2->orientation = glm::radians(90.0f);
	asteroid3->orientation = glm::radians(135.0f);
	asteroid4->orientation = glm::radians(180.0f);


	//-------------------------------------------------------
	

	setkeyboardhandler(mykeyboardhandler);
	setUpdateFunction(myupdateloop);
	// Enter main loop - this handles update and render calls
	engineMainLoop();

	// When we quit (close window for example), clean up engine resources
	engineShutdown();

	// return success :)
	return 0;
}






void myupdateloop(GLFWwindow* window, double tDelta)
{
	// Player update 
	playercontrol(tDelta);

	// Asteroid movement
	updateAsteroid(asteroid1, tDelta, 2.0f);
	updateAsteroid(asteroid2, tDelta, 1.015f);
	updateAsteroid(asteroid3, tDelta, 3.02f);
	updateAsteroid(asteroid4, tDelta, 4.025f);

	// Bullet logic
	GameObject2D* bullet = getObject("bullet");
	float theta = player->orientation;
	glm::vec2 forward = glm::vec2(std::cos(theta), std::sin(theta));
	glm::vec2 vel;
	bulletmoveamount = bulletspeed * static_cast<float>(tDelta);
	glm::vec2 bulletvel = forward * bulletmoveamount;
	bullet->position += forward * 1.0f;


	void playercontrol(double tdelta){

		// get the forward vector from the player's orientation
		float theta = player->orientation;
		glm::vec2 forward = glm::vec2(std::cos(theta), std::sin(theta));
		glm::vec2 vel;

		// this chunk controls the players velocity

		if (upArrow)
			playerSpeed += acceleration;

		if (downArrow)
			playerSpeed -= acceleration;

		// this chunk controls the players rotation

		if (leftArrow)
			player->orientation += glm::radians(90.0f) * static_cast<float>(tDelta);

		if (rightArrow)
			player->orientation -= glm::radians(90.0f) * static_cast<float>(tDelta);

		// bullet shooting 
		if (spacebar){
			GameObject2D* bullet = getObject("bullet");
			bullet->position = player->position; // spawn bullet at player position
			bullet->orientation = player->orientation;

		}
		// calculate velocity vector
		moveAmount = playerSpeed * static_cast<float>(tDelta);
		vel = forward * moveAmount;
		float normalizedSpeed = glm::length(vel);

		// stops speed going to high so the player doesn't fly so fast they're hard to notice
		if (normalizedSpeed > maxSpeed) {
			vel = glm::normalize(vel);
			vel = vel * maxSpeed;
		}

		// debug lines that output the current speed to console
		// commented out to reduce spam in the game this is important becauseit allows for the game to be more stableised 
		normalizedSpeed = glm::length(vel);
		// std::cout << "Speed: " << normalizedSpeed << std::endl;
		player->position += vel;


		// this chunk wraps around the edges of the screen 
		if (player->position.y > getViewplaneHeight() / 2)
			player->position.y = -getViewplaneHeight() / 2;
		if (player->position.y < -getViewplaneHeight() / 2)
			player->position.y = getViewplaneHeight() / 2;

		if (player->position.x > getViewplaneHeight() / 2)
			player->position.x = -getViewplaneHeight() / 2;
		if (player->position.x < -getViewplaneHeight() / 2)
			player->position.x = getViewplaneHeight() / 2;

	}
	void myKeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
		// Check if the key was just pressed
		if (action == GLFW_PRESS) {

			// now check which key was pressed...
			switch (key)
			{
			case GLFW_KEY_ESCAPE:
				// If escape is pressed tell GLFW we want to close the 
				// window(and quit)
				glfwSetWindowShouldClose(window, true);
				break;
			case GLFW_KEY_UP:
				upArrow = true;
				break;

			case GLFW_KEY_DOWN:
				downArrow = true;
				break;

			case GLFW_KEY_LEFT:
				leftArrow = true;
				break;

			case GLFW_KEY_RIGHT:
				rightArrow = true;
				break;

			case GLFW_KEY_SPACE:
				spaceBar = true;
				break;

			}
			void updateAsteroid(GameObject2D* asteroid, double tDelta, float speed) {

				float theta = asteroid->orientation;
				glm::vec2 forward = glm::vec2(std::cos(theta), std::sin(theta));

				glm::vec2 vel;
				float moveAmount = speed * (float)tDelta;
				vel = forward * moveAmount;
				asteroid->position += vel;

				// more screen wrapping for asteroids
				if (asteroid->position.y > getViewplaneHeight() / 2)
					asteroid->position.y = -getViewplaneHeight() / 2;
				if (asteroid->position.y < -getViewplaneHeight() / 2)
					asteroid->position.y = getViewplaneHeight() / 2;
				if (asteroid->position.x > getViewplaneHeight() / 2)
					asteroid->position.x = -getViewplaneHeight() / 2;
				if (asteroid->position.x < -getViewplaneHeight() / 2)
					asteroid->position.x = getViewplaneHeight() / 2;
			}
