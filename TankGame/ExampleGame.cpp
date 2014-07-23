#include "ExampleGame.h"

#include "Level.h"

void ExampleGame::loadAssets()
{
	pLevel_ = new Level("GameWorld.dat", "TileData.dat");
	//------------Test code------------
//	SDL_Surface* temp = IMG_Load("main_character.png");
//	SDL_Surface* temp2 = SDL_DisplayFormatAlpha(temp);
	
//	SDL_FreeSurface(temp);

	/*SDL_Rect srcRect = SDL_Rect();
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = 32;
	srcRect.h = 48;*/
	pPlayerOne_ = new Player(Vec2(100.0f, 1000.0f));
	camera_.x = pPlayerOne_->GetPos().x - (SCREEN_WIDTH / 2.0f);
	camera_.y = pPlayerOne_->GetPos().y - (SCREEN_HEIGHT / 2.0f);

	pPlayerTwo_ = new Character(Character::CharacterType::PLAYER, Vec2(400.0f, 200.0f));
	//-----------------------------------------------
}

Vec2 ExampleGame::getAARROverlap(const SDL_Rect& r1, const SDL_Rect& r2)
{
	Sint16 r1CentreX = r1.x + (r1.w / 2);
	Sint16 r1CentreY = r1.y + (r1.h / 2);
	Sint16 r2CentreX = r2.x + (r2.w / 2);
	Sint16 r2CentreY = r2.y + (r2.h / 2);

	Sint16 diffX = r1CentreX - r2CentreX;
	Sint16 diffY = r1CentreY - r2CentreY;
	Uint16 halfWidths = (r1.w + r2.w) / 2;
	Uint16 halfHeights = (r1.h + r2.h) / 2;

	Sint16 overlapX = halfWidths - abs(diffX);
	Sint16 overlapY = halfHeights - abs(diffY);

	if(overlapX > 0 &&
	   overlapY > 0)
	{
		//check to see which axis to correct on (smallest overlap)
		if(overlapX > overlapY)
		{
			//correct on y
			if(r1CentreY < r2CentreY)
			{
				return Vec2(0.0f, -overlapY);
			}
			else
			{
				return Vec2(0.0f, overlapY);
			}
		}
		else
		{
			//correct on x
			if(r1CentreX < r2CentreX)
			{
				return Vec2(-overlapX, 0.0f);
			}
			else
			{
				return Vec2(overlapX, 0.0f);
			}
		}
	}

	return Vec2(0.0f, 0.0f);
}

bool ExampleGame:: rectRectCollision(const SDL_Rect& r1, 
										const SDL_Rect& r2)
{
	Sint16 diffX = r1.x - r2.x;
	Sint16 diffY = r1.y - r2.y;
	Uint16 halfWidths = (r1.w + r2.w) / 2;
	Uint16 halfHeights = (r1.h + r2.h) / 2;

	if(abs(diffX) < halfWidths &&
	   abs(diffY) < halfHeights)
	{
		return true;
	}

	return false;
}

void ExampleGame::characterLevelCollision(Character* character)
{
	float x = character->GetPos().x - (character->GetWidth() / 2);
	float y = character->GetPos().y - (character->GetHeight() / 2);

	float w = character->GetWidth();
	float h = character->GetHeight();

	SDL_Rect charRect;
	charRect.x = (Sint16)x;
	charRect.y = (Sint16)y;
	charRect.w = (Sint16)w;
	charRect.h = (Sint16)h;

	Uint16 leftCol = charRect.x / pLevel_->GetTileWidth();
	Uint16 rightCol = (charRect.x + charRect.w) / pLevel_->GetTileWidth();
	Uint16 topRow = charRect.y / pLevel_->GetTileHeight();
	Uint16 bottomRow = (charRect.y + charRect.h) / pLevel_->GetTileHeight();

	for(int col = leftCol; col <= rightCol; ++col)
	{
		for(int row = topRow; row <= bottomRow; ++row)
		{
			if(pLevel_->GetCollisionData()[row][col] != 0)
			{
				SDL_Rect tileRect;
				tileRect.x = col * pLevel_->GetTileWidth();
				tileRect.y = row * pLevel_->GetTileHeight();
				tileRect.w = pLevel_->GetTileWidth();
				tileRect.h = pLevel_->GetTileHeight();

				Vec2 overlap = getAARROverlap(charRect, tileRect);
				//broad phase rect / rect check
				if(overlap.x != 0.0f || overlap.y != 0.0f)
				{
					//Temp / test -> tells us if the rect rect works
					pPlayerOne_->SetPos(pPlayerOne_->GetPos() +
										 overlap);
					pPlayerOne_->SetVel(Vec2(0.0f, 0.0f));
					//-----------------------------------------------

					//do per pixel collision between the rectangles
					//if(perPixelCollision(charRect, character, tileRect))
						//handle the collision
				}
			}
		}
	}
}


bool ExampleGame::perPixelCollision(const Sprite* s1, const Sprite* s2)
{
	SDL_Rect overlapRect = GetOverlapRect(s1, s2);
	Sint16 oRectBot = overlapRect.y + overlapRect.h;
	Sint16 oRectRight = overlapRect.x + overlapRect.w;

	for (Uint16 row = (Uint16)overlapRect.y; row < oRectBot; ++row)
	{
		for (Uint16 col = (Uint16)overlapRect.x; col < oRectRight; ++col)
		{
			//get alpha from s1 /s2

			//first get the row / col relative to the source image
			Uint16 s1Row = row - (Uint16)s1->GetPos().y;//gets the row relative to the current frame
			s1Row = s1Row + (Uint16)s1->GetSrcRect().y;//gets the row relative to the entire image

			Uint16 s1Col = col - (Uint16)s1->GetPos().x;
			s1Col += (Uint16)s1->GetSrcRect().x;

			Uint16 s2Row = row - (Uint16)s2->GetPos().y;//gets the row relative to the current frame
			s2Row = s2Row + (Uint16)s2->GetSrcRect().y;//gets the row relative to the entire image

			Uint16 s2Col = col - (Uint16)s2->GetPos().x;
			s2Col += (Uint16)s2->GetSrcRect().x;

			SDL_PixelFormat *fmt;
			Uint32 temp;
			Uint8* pixels;
			Uint8 alpha1, alpha2;

			fmt = s1->GetImage()->format;
			pixels = (Uint8*)s1->GetImage()->pixels;
			Uint32 index = (Uint32)s1Row * (Uint32)s1->GetImage()->w + (Uint32)s1Col;

			/*
			Uint16 index = (Uint32)s1Row * (Uint16)fmt->BytesPerPixel;
			(Uint32)s1->GetImage()->w + (Uint32)s1Col;
			(Uint32)s1->GetImage()->w + (Uint32)s1Col;
			(Uint32)fmt->BytesPerPixel;
			*/

			temp = ((Uint32*)pixels)[index] & fmt->Amask; //Isololate alpha component
			temp = temp >> fmt->Ashift; //Shift it down to 8-bit
			temp = temp << fmt->Aloss; // Expand to a full 8 bit number
			alpha1 = (Uint8)temp;

			fmt = s2->GetImage()->format;
			pixels = (Uint8*)s2->GetImage()->pixels;
			index = (Uint32)s2Row * (Uint32)s2->GetImage()->w + (Uint32)s2Col;

			/*
			Uint16 index = (Uint32)s2Row * (Uint16)fmt->BytesPerPixel;
			(Uint32)s2->GetImage()->w + (Uint32)s2Col;
			(Uint32)s2->GetImage()->w + (Uint32)s2Col;
			(Uint32)fmt->BytesPerPixel;
			*/

			temp = ((Uint32*)pixels)[index] & fmt->Amask; //Isololate alpha component
			temp = temp >> fmt->Ashift; //Shift it down to 8-bit
			temp = temp << fmt->Aloss; // Expand to a full 8 bit number
			alpha2 = (Uint8)temp;


			if (alpha1 > 0 && alpha2 > 0)
			{
				return true;
			}

		}
	}
	return false;
}

SDL_Rect ExampleGame::GetOverlapRect(const Sprite* s1, const Sprite* s2)
{
	SDL_Rect ret;
	ret.x = (Sint16)max(s1->GetPos().x, s2->GetPos().x);
	ret.y = (Sint16)max(s1->GetPos().y, s2->GetPos().y);
	ret.w = (Uint16)min(s1->GetPos().x + (Sint16)s1->GetSrcRect().w,
		s2->GetPos().x + (Sint16)s2->GetSrcRect().w);
	//check if no overlap, then set to 0, otherwise set width approperiately
	ret.w = (Uint16)max(0, (Sint16)ret.w - ret.x);

	ret.h = (Uint16)min(s1->GetPos().y + (Sint16)s1->GetSrcRect().h,
			s2->GetPos().y + (Sint16)s2->GetSrcRect().h);
	//check if no overlap, then set to 0, otherwise set width approperiately
	ret.h = (Uint16)max(0, (Sint16)ret.h - ret.y);

	return ret;
}

float CAMERA_STIFFNESS = 500.0f;
void ExampleGame::updateCamera()
{
	//calculate centre of screen
	Vec2 centre;
	centre.x = camera_.x + (SCREEN_WIDTH / 2.0f);
	centre.y = camera_.y + (SCREEN_HEIGHT / 2.0f);
	
	//calculate vector to the player
	Vec2 toPlayer;
	toPlayer.x = pPlayerOne_->GetPos().x - centre.x;
	toPlayer.y = pPlayerOne_->GetPos().y - centre.y;

	float mag = VectorMath::Magnitude(&toPlayer);
	toPlayer = VectorMath::Normalize(&toPlayer);
	float d = mag / CAMERA_STIFFNESS;

	camera_ += toPlayer * d;

	camera_.x = max(0.0f, camera_.x);
	float levelRightEdge = pLevel_->GetLevelRight();
	camera_.x = min(levelRightEdge - SCREEN_WIDTH, camera_.x);

	camera_.y = max(0.0f, camera_.y);
	camera_.y = min(pLevel_->GetLevelBottom() - SCREEN_HEIGHT, camera_.y);
}

void ExampleGame::checkLevelBounds()
{
	Vec2 playerPos = pPlayerOne_->GetPos();
	
	playerPos.x = max(pPlayerOne_->GetWidth() / 2.0f, playerPos.x);
	playerPos.x = min(pLevel_->GetLevelRight() - pPlayerOne_->GetWidth() / 2.0f,
					  playerPos.x);

	playerPos.y = max(pPlayerOne_->GetHeight() / 2.0f, playerPos.y);
	playerPos.y = min(pLevel_->GetLevelBottom() - pPlayerOne_->GetHeight() / 2.0f,
					  playerPos.y);

	pPlayerOne_->SetPos(playerPos);
}

const int MAX_COLLISION_CHECKS = 10;
void ExampleGame::update(float deltaTime)
{
	pPlayerOne_->Update(deltaTime);

	for (int i = 0; i < MAX_COLLISION_CHECKS && perPixelCollision(pPlayerOne_, pPlayerTwo_); ++i)
	{
		Vec2 toPlayer = pPlayerOne_->GetPos() - pPlayerTwo_->GetPos();
		toPlayer = VectorMath::Normalize(&toPlayer);
		pPlayerOne_->SetPos(pPlayerOne_->GetPos() + toPlayer);
	}
	
	checkLevelBounds();

	characterLevelCollision(pPlayerOne_);	

	updateCamera();
}

void ExampleGame::drawLevel()
{
	Sint16 lCol = camera_.x / pLevel_->GetTileWidth();
	Sint16 rCol = (camera_.x + SCREEN_WIDTH) /
					pLevel_->GetTileWidth();

	Sint16 tRow = camera_.y / pLevel_->GetTileHeight();
	Sint16 bRow = (camera_.y + SCREEN_HEIGHT) /
					pLevel_->GetTileHeight();

	lCol = max(min(lCol, pLevel_->GetLevelTilesWide() - 1), 0);
	rCol = max(min(rCol, pLevel_->GetLevelTilesWide() - 1), 0);
	tRow = max(min(tRow, pLevel_->GetLevelTilesHigh() - 1), 0);
	bRow = max(min(bRow, pLevel_->GetLevelTilesHigh() - 1), 0);

	Uint16 const * const * const mapData = pLevel_->GetMapData();
	//Uint16 const * const * const objectData = pLevel_->GetObjectData();
	//Uint16 const * const * const collisionData = pLevel_->GetCollisionData();
	
	for(Uint16 col = lCol; col <= rCol; ++col)
	{
		for(Uint16 row = tRow; row <= bRow; ++row)
		{
			renderLevelTile(row, col, mapData);
			//renderLevelTile(row, col, objectData);
			//renderLevelTile(row, col, collisionData);
		}
	}

	bool playerDrawn = false;
	float playerBase = pPlayerOne_->GetPos().y + (pPlayerOne_->GetHeight() / 2.0f);
	const std::vector<Level::Object>& objects = pLevel_->GetObjects();
	for(int i = 0; i < objects.size(); ++i)
	{
		if(!playerDrawn && objects[i].baseY > playerBase)
		{
			pPlayerOne_->Draw(pDisplay_, camera_);
			playerDrawn = true;
		}
		const Level::ObjectData& objectData = pLevel_->GetObjectData().at(objects[i].id);
		SDL_Rect srcRect;
		srcRect.x = objectData.x;
		srcRect.y = objectData.y;
		srcRect.w = objectData.width;
		srcRect.h = objectData.height;	

		SDL_Rect dstRect;
		dstRect.x = objects[i].pos.x - camera_.x;
		dstRect.y = objects[i].pos.y - camera_.y;
		dstRect.w = srcRect.w;
		dstRect.h = srcRect.h;

		SDL_BlitSurface(pLevel_->GetTileSheet(), &srcRect, 
						pDisplay_, &dstRect);
	}

	if(!playerDrawn)
	{
		pPlayerOne_->Draw(pDisplay_, camera_);
		playerDrawn = true;
	}
}

void ExampleGame::renderLevelTile(Uint16 row, Uint16 col, 
								  Uint16 const * const * const levelData)
{
	if(levelData[row][col] != 0)
	{
		Uint16 index = levelData[row][col] - 1;					
				
		SDL_Rect srcRect;
		Uint16 tileSheetCol = index % pLevel_->GetSrcTilesWide();
		srcRect.x = tileSheetCol * pLevel_->GetTileWidth();

		Uint16 tileSheetRow = index / pLevel_->GetSrcTilesWide();
		srcRect.y = tileSheetRow * pLevel_->GetTileHeight();

		srcRect.w = pLevel_->GetTileWidth();
		srcRect.h = pLevel_->GetTileHeight();

		SDL_Rect dstRect;
		dstRect.x = (col * pLevel_->GetTileWidth()) - camera_.x;
		dstRect.y = (row * pLevel_->GetTileHeight()) - camera_.y;
		dstRect.w = pLevel_->GetTileWidth();
		dstRect.h = pLevel_->GetTileHeight();

		SDL_BlitSurface(pLevel_->GetTileSheet(), &srcRect, 
						pDisplay_, &dstRect);
	}
}

void ExampleGame::draw()
{
	drawLevel();
	pPlayerTwo_->Draw(pDisplay_, camera_);
	Game::draw();
}

void ExampleGame::onKeyDown(Uint16 key)
{
	pPlayerOne_->OnKeyDown(key);
}

void ExampleGame::onKeyUp(Uint16 key)
{
	pPlayerOne_->OnKeyUp(key);
}