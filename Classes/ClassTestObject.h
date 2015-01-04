#pragma once

#include "BlobData.h"

struct Barrel
{
	int		type;
	float	rotate;
	float	interval;
	int		proj_type;
	int		proj_level;
	float	proj_scale;
	float	proj_damage;
	float	proj_speed;

	Barrel(void)
		: type(0)
		, rotate(360.0f)
		, interval(2)
		, proj_type(4)
		, proj_level(8)
		, proj_scale(1.0f)
		, proj_damage(100)
		, proj_speed(1000)
	{
	}
};

class ClassTestObject
{
public:
	ClassTestObject(void);
	ClassTestObject(void *blob, int length);
	~ClassTestObject(void);

	BlobData* memorySerialize(void) const;
	void memoryUnserialize(BlobData &blob);
	size_t getSizeInByte(void) const;

	int body;
	int body_level;
	int armor;
	int armor_level;
	int engine;
	int engine_level;
	std::vector<Barrel> barrells;
};

