#pragma once

#include <fstream>
#include "MathUtils.h"
#include "SpriteAnimation.h"

class Gun :
	public SpriteAnimation
{
public:
	enum GunType
	{
		STANDARDGUN
	};

protected:
	Vec2 velocity_;

	void processDataFile(std::ifstream& file);

public:
	Gun(GunType type, const Vec2& pos);
	Gun(const Vec2& pos) { pos_ = pos; }
	virtual void LoadGun(GunType type);
	~Gun(void);

	Vec2 GetVel()
	{
		return velocity_;
	}

	void SetVel(const Vec2& vel)
	{
		velocity_ = vel;
	}
};

