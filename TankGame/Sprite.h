#pragma once

#include "SDL.h"
#include "MathUtils.h"

class Sprite
{
protected:
	SDL_Surface* pImage_;
	Vec2 pos_; 
	SDL_Rect srcRect_;
	Uint8 alpha_;

public:
	Sprite(void) :
	  pImage_(NULL)
	  {

	  }

	Sprite(SDL_Surface* image, const Vec2& pos, 
		   const SDL_Rect& srcRect,
		   Uint8 alpha) :
	pImage_(image),
	pos_(pos),
	srcRect_(srcRect),
	alpha_(alpha)
	{}

	
	virtual ~Sprite(void)
	{
		if(pImage_)
		{
			SDL_FreeSurface(pImage_);
		}
	}

	Vec2 GetPos() const
	{
		return pos_;
	}

	void SetPos(const Vec2& pos)
	{
		pos_ = pos;
	}

	Uint8 GetAlpha() const
	{
		return alpha_;
	}

	void SetAlpha(Uint8 alpha)
	{
		alpha_ = alpha;
	}

	const SDL_Rect& GetSrcRect() const
	{
		return srcRect_;
	}

	SDL_Surface const * GetImage() const
	{
		return pImage_;
	}

	virtual void Draw(SDL_Surface* surfaceToDrawOn);
	virtual void Draw(SDL_Surface* surfaceToDrawOn, const Vec2& cam);
};

