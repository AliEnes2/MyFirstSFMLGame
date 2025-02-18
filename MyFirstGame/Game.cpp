#include "Game.h"

void Game::initVaribles()
{
	this->window = nullptr;

	//Game logic
	this->endGame = false;
	this->points = 0;
	this->health = 25;
	this->enemySpawnTimerMax = 20.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 8;
	this->mouseHeld = false;
}

void Game::initWindow()
{
	this->videoMode.height = 768;
	this->videoMode.width = 1024;

	this->window = new sf::RenderWindow(this->videoMode, "Test Game", sf::Style::Titlebar | sf::Style::Close);

	this->window->setFramerateLimit(60);
}

void Game::initFonts()
{
	if (this->font.loadFromFile("Fonts/Aller_BdIt.ttf"))
	{
		std::cout << "ERROR::GAME::INITFONTS::Failed to load font!" << "\n";
	}
}

void Game::initText()
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(24);
	this->uiText.setFillColor(sf::Color::White);
	this->uiText.setString("NONE");
}

void Game::initEnemies()
{
	this->enemy.setPosition(30.f, 50.f);
	this->enemy.setSize(sf::Vector2f(100.f, 100.f));
	this->enemy.setFillColor(sf::Color::Magenta);
	/*this->enemy.setOutlineColor(sf::Color::Green);
	this->enemy.setOutlineThickness(1.f);*/
}

Game::Game()
{
	this->initVaribles();
	this->initWindow();
	this->initFonts();
	this->initText();
	this->initEnemies();
}

Game::~Game()
{
	delete this->window;
}

//Accessors

const bool Game::running() const
{
	return this->window->isOpen();
}

const bool Game::getEndGame() const
{
	return this->endGame;
}

void Game::spawnEnemy()
{
	this->enemy.setPosition(static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)), 0.f);

	//Randomize enemy type
	int type = rand() % 5;

	switch (type)
	{
	case 0:
		this->enemy.setSize(sf::Vector2f(15.f, 15.f));
		this->enemy.setFillColor(sf::Color::Red);
		break;
	case 1:
		this->enemy.setSize(sf::Vector2f(35.f, 35.f));
		this->enemy.setFillColor(sf::Color::Magenta);
		break;
	case 2:
		this->enemy.setSize(sf::Vector2f(50.f, 50.f));
		this->enemy.setFillColor(sf::Color::Blue);
		break;
	case 3:
		this->enemy.setSize(sf::Vector2f(70.f, 70.f));
		this->enemy.setFillColor(sf::Color::Green);
		break;
	case 4:
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
		this->enemy.setFillColor(sf::Color::White);
		break;
	default:
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
		this->enemy.setFillColor(sf::Color::Yellow);
		break;
		break;
	}

	//Spawn the enemy
	this->enemies.push_back(this->enemy);

	

}

//Functions
void Game::pollEvents()
{
	//Event poll
	while (this->window->pollEvent(this->e))
	{
		switch (this->e.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;

		case sf::Event::KeyPressed:
			if (this->e.key.code == sf::Keyboard::Escape)
				this->window->close();
			break;

		}
	}
}

void Game::updateMousePos()
{
	//Update the mouse pos with vector2i

	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateText()
{
	std::stringstream ss;

	ss << "Points: " << this->points << "\n" << "Health: " << this->health << "\n";

	this->uiText.setString(ss.str());
}

void Game::updateEnemies()
{
	//Remove the enemies at the edge of screen //TODO

	//Updating the timer for enemy spawning
	if (this->enemies.size() < this->maxEnemies)
	{
		//Spawn the enemy and reset the timer
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;
		}

		else
			this->enemySpawnTimer += 1.f;
	}

	//Moving and updating the enemies
	for (int i = 0; i <this->enemies.size(); i++)
	{
		bool deleted = false;

		this->enemies[i].move(0.f, 5.f);

		if (this->enemies[i].getPosition().y > this->window->getSize().y)
		{
			this->enemies.erase(this->enemies.begin() + i);
			this->health -= 1;
			std::cout << "Health: " << this->health << "\n";
		}
	}
	//Check if clicked upon 
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
	  if(this->mouseHeld == false)
	  { 
		this->mouseHeld == true;
		bool deleted = false;

		for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
		{
			if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
			{
				//Gain points
				if(this->enemies[i].getFillColor() == sf::Color::Red)
				this->points += 10;

				else if(this->enemies[i].getFillColor() == sf::Color::Magenta)
					this->points += 7;

				else if(this->enemies[i].getFillColor() == sf::Color::Blue)
					this->points += 5;

				else if(this->enemies[i].getFillColor() == sf::Color::Green)
					this->points += 3;

				else if(this->enemies[i].getFillColor() == sf::Color::White)
					this->points += 1;

				std::cout << "Points: " << this->points << "\n";

				//Delete the enemy
				deleted = true;
				this->enemies.erase(this->enemies.begin() + i);
			}
			
		}
	  }
	  
	}
	else
	{
		this->mouseHeld == false;
	}
	
}

void Game::update()
{
	this->pollEvents();


	if (this->endGame == false) 
	{
	this->updateMousePos();

	this->updateText();

	this->updateEnemies();
	}

	//End game condition
	if (this->health <= 0)
		this->endGame = true;
}

void Game::renderText(sf::RenderTarget& target)
{
	target.draw(this->uiText);
}

void Game::renderEnemies(sf::RenderTarget& target)
{
	//Rendering all enemies
	for (auto& en : this->enemies)
	{
		target.draw(en);
	}
}

void Game::render()//Renders the game objects
{
	this->window->clear();

	//Draw
	this->renderEnemies(*this->window);

	this->renderText(*this->window);

	this->window->display();
}