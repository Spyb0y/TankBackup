#include "Gun.h"

#include <string>
#include <iostream>
#include <sstream>

#include "SDL_image.h"


Gun::Gun(GunType type, const Vec2& pos)
{
	LoadGun(type);
	pos_ = pos;
}


Gun::~Gun(void)
{
}

void Gun::LoadGun(GunType type)
{
	std::ifstream gunData;
	std::string filename;

	switch (type)
	{
	case STANDARDGUN:
		filename = "Player.dat";
		break;

	default:
		break;
	}

	gunData.open(filename, std::ios::in);

	if (gunData.is_open())
	{
		processDataFile(gunData);
	}
	else
	{
		gunData.close();
		std::cerr << "Error opening gun data: " << filename << std::endl;
	}

	gunData.close();
}

void Gun::processDataFile(std::ifstream& file)
{
	std::string buf;
	while (file.good())
	{
		file >> buf;

		if (buf[0] == '#')
		{
			std::getline(file, buf);
		}
		else if (buf == "image:")
		{
			file >> buf;
			pImage_ = IMG_Load(buf.c_str());
		}
		else if (buf == "cellWidth:")
		{
			file >> buf;
			std::stringstream ss(buf);
			ss >> frameWidth_;
		}
		else if (buf == "cellHeight:")
		{
			file >> buf;
			std::stringstream ss(buf);
			ss >> frameHeight_;
		}
		else if (buf == "Animation")
		{
			file >> buf;
			anims_.insert(std::pair<std::string, AnimationData>(buf, AnimationData()));
		}
		else if (buf == "frameRate")
		{
			std::string animName;
			file >> animName;

			file >> buf;
			std::stringstream ss(buf);
			ss >> anims_[animName].frameRate;
		}
		else if (buf == "startFrame")
		{
			std::string animName;
			file >> animName;

			file >> buf;
			std::stringstream ss(buf);
			ss >> anims_[animName].startFrame;
		}
		else if (buf == "numFrames")
		{
			std::string animName;
			file >> animName;

			file >> buf;
			std::stringstream ss(buf);
			ss >> anims_[animName].numFrames;
		}
		else if (buf == "defaultAnimation")
		{
			file >> buf;
			SetCurrAnim(buf);
		}
	}
	initAnim();

}
