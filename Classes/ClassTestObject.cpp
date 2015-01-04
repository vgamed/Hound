#include "StdAfx.h"
#include "ClassTestObject.h"


ClassTestObject::ClassTestObject(void)
{
	body		= 0x10;
	body_level	= 0x20;
	armor		= 0x40;
	armor_level	= 0x80;
	engine		= 0x100;
	engine_level = 0x200;

	Barrel a;
	barrells.push_back(a);
	barrells.push_back(a);
	barrells.push_back(a);
}


ClassTestObject::~ClassTestObject(void)
{
}

BlobData* ClassTestObject::memorySerialize(void) const
{
	BlobData * blob = new(std::nothrow) BlobData(getSizeInByte());
	if (blob != nullptr)
	{
		*blob << body;
		*blob << body_level;
		*blob << armor;
		*blob << armor_level;
		*blob << engine;
		*blob << engine_level;

		std::vector<Barrel>::const_iterator it = barrells.begin();
		for (; it!= barrells.end(); it++)
		{
			const Barrel &b = *it;
			*blob << b.type;
			*blob << b.rotate;
			*blob << b.interval;
			*blob << b.proj_type;
			*blob << b.proj_level;
			*blob << b.proj_scale;
			*blob << b.proj_damage;
			*blob << b.proj_speed;
		}
	}

	return blob;
}

void ClassTestObject::memoryUnserialize(BlobData &blob)
{
	if (blob.getLength() < getSizeInByte())
	{
		return;
	}

	blob >> body;
	blob >> body_level;
	blob >> armor;
	blob >> armor_level;
	blob >> engine;
	blob >> engine_level;

	barrells.clear();
	while (!blob.isAtEnd())
	{
		Barrel b;
		blob >> b.type;
		blob >> b.rotate;
		blob >> b.interval;
		blob >> b.proj_type;
		blob >> b.proj_level;
		blob >> b.proj_scale;
		blob >> b.proj_damage;
		blob >> b.proj_speed;
		barrells.push_back(b);
	}
}

size_t ClassTestObject::getSizeInByte(void) const
{
	size_t s = 0;
	s += sizeof(body);
	s += sizeof(body_level);
	s += sizeof(armor);
	s += sizeof(armor_level);
	s += sizeof(engine);
	s += sizeof(engine_level);

	std::vector<Barrel>::const_iterator it = barrells.begin();
	for (; it!= barrells.end(); it++)
	{
		const Barrel &b = *it;
		s += sizeof(b.type);
		s += sizeof(b.rotate);
		s += sizeof(b.interval);
		s += sizeof(b.proj_type);
		s += sizeof(b.proj_level);
		s += sizeof(b.proj_scale);
		s += sizeof(b.proj_damage);
		s += sizeof(b.proj_speed);
	}

	return s;
}
