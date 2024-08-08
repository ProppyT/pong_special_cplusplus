// 2024_PongSpecial_ProppyT.cpp :

/*
	the headers of the program game.
*/
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <string>
#include <windows.h>
#include <fstream> // For loading the font into memory
#include <vector>
#include <cstdlib>

#ifdef SFML_SYSTEM_IOS
#include <SFML/Main.hpp>
#endif

static std::string resourcesDir()
{
#ifdef SFML_SYSTEM_IOS
	return "";
#else
	return "resources/";
#endif
}

// prototypes of the funcions used in this program
void ball_update(sf::CircleShape&, float&, float&, int&, int&);
void text_update(sf::Text&, int, int);
void paddle_up(sf::RectangleShape&, float, float);
void paddle_down(sf::RectangleShape&, float, float);


int main() {
	// the score
	int score_1 = 0;
	int score_2 = 0;


	sf::RenderWindow window(sf::VideoMode(800, 600), "Pong based C++, made by ProppyT 2024");


	sf::Font font;
	if (!font.loadFromFile(resourcesDir() + "tuffy.ttf")) {
		return EXIT_FAILURE;
	}
	
	// set up the text for the scoring
	sf::Text scoreDisplay;
	scoreDisplay.setFont(font);
	scoreDisplay.setPosition(350, 50); // Set an initial position
	

	// Ball declaration
	sf::CircleShape ballShape(15.f); // 15 pixel radius
	ballShape.setFillColor(sf::Color::White);
	
	// ..initialize ball position and velocity
	ballShape.setPosition(400.f, 300.f);
	float dx = (float)0.1; // ball velocity horizontal
	float dy = (float)0.1; // ball veloctiy vertical
	

	// Paddle declarations
	sf::RectangleShape leftPaddleShape(sf::Vector2f(10.f, 50.f));
	sf::RectangleShape rightPaddleShape(sf::Vector2f(10.f, 50.f));

	// ..initialize paddle positions
	leftPaddleShape.setPosition(50.f, 300.f);
	rightPaddleShape.setPosition(750.f, 300.f);

	// Game Loop
	while (window.isOpen()) {
		sf::Event event;

		window.clear(sf::Color::Blue);
		text_update(scoreDisplay, score_1, score_2);
		window.draw(scoreDisplay);
		window.draw(ballShape);
		window.draw(leftPaddleShape);
		window.draw(rightPaddleShape);

		while (window.pollEvent(event)) {
			// when event is closing the window is to closed
			if (event.type == sf::Event::Closed)
				window.close();

			// The letter "w" key was pressed, to get the left paddle up
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::W))
				paddle_up(leftPaddleShape, 20.f, 600.f);
			// The letter "s" key was pressed, to get the left paddle down 
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::S))
				paddle_down(leftPaddleShape, 20.f, 600.f);
			// The arrow up key was pressed, to get the right paddle up
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Up))
				paddle_up(rightPaddleShape, 20.f, 600.f);
			// The arrow down key was pressed, to get the right paddle down
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Down))
				paddle_down(rightPaddleShape, 20.f, 600.f);
		}
		
		
		// Update (move ball, checkcollisions, score text)
		ball_update(ballShape, dx, dy, score_1, score_2);
		
		window.display();
	}

	return 0;
}


std::wstring ExePath() { // to discover the working directory where everything works for Visual Studio
	TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
	return std::wstring(buffer).substr(0, pos);
}


void text_update(sf::Text& text, int score_1, int score_2) {
	std::string scoreString = std::to_string(score_1) + " : " + std::to_string(score_2);
	text.setString(std::to_string(score_1) + " : " + std::to_string(score_2));
}

void ball_update(sf::CircleShape& ball, float& dx, float& dy, int& score_1, int& score_2) {

	// move the ball
	ball.setPosition((ball.getPosition().x + dx), (ball.getPosition().y + dy));

	// boundry checks with the top and bottom walls
	if (ball.getPosition().y <= 0)
		dy *= -1; // reverse the vertical direction
	if (ball.getPosition().y >= 570.f)
		dy *= -1; // reverse the vertical direction

	// score left and right
	if (ball.getPosition().x <= 0) {
		dx *= -1; // reverse the horizontal direction
		score_2 += 1;
	}
	if (ball.getPosition().x >= 770.f) {
		dx *= -1; // reverse the horizontal direction
		score_1 += 1;
	}
}

void paddle_up(sf::RectangleShape& paddle, float movementSpeed, float windowHeight) {
	float y = paddle.getPosition().y;
	y -= movementSpeed; // move upward

	// Boundary check top
	if (y > 0 && y + paddle.getSize().y < windowHeight) {
		paddle.setPosition(paddle.getPosition().x, y);
	}
}

void paddle_down(sf::RectangleShape& paddle, float movementSpeed, float windowHeight) {
	float y = paddle.getPosition().y;
	y += movementSpeed; // Move downward

	// Boundary check bottom
	if (y + paddle.getSize().y < windowHeight) {
		paddle.setPosition(paddle.getPosition().x, y);
	}
}