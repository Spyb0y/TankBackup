#pragma once
#include "Game.h"
#include "Player.h"
#include "Level.h"

class ExampleGame : public Game
{
protected: 
	virtual void loadAssets();
	virtual void update(float deltaTime);
	void drawLevel();
	virtual void draw();

	virtual void onKeyDown(Uint16 key);
	virtual void onKeyUp(Uint16 key);

	void renderLevelTile(Uint16 row, Uint16 col, 
		Uint16 const * const * const levelData);

	bool rectRectCollision(const SDL_Rect& r1, const SDL_Rect& r2);

	//get the overlap vector from 2 axis aligned rectangles.
	Vec2 getAARROverlap(const SDL_Rect& r1, const SDL_Rect& r2);

	bool perPixelCollision(const Sprite* s1, const Sprite* s2);

	SDL_Rect GetOverlapRect(const Sprite* s1, const Sprite* s2);

	void characterLevelCollision(Character* character);

	void updateCamera();

	void checkLevelBounds();

	//-------test code----------
	Player* pPlayerOne_;
	Character* pPlayerTwo_;
	Level* pLevel_;

	Vec2 camera_;

public:
	ExampleGame(void) : Game(), 
		pPlayerOne_(NULL)
	{}

	virtual ~ExampleGame(void)
	{
		if(pPlayerOne_)
		{
			delete pPlayerOne_;
		}
	}
};

