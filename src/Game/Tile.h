#pragma once

constexpr unsigned MapSize = 100;
constexpr unsigned ChunkSize = 5;

// TODO: переделать под битовое поле так как вмещается в 8 бит
// идея такая - по этому значению оно решает сколько сторон тайла будет отрисовано
struct TileSide
{
	uint8_t left = 1;
	uint8_t right = 1;
	uint8_t forward = 1;
	uint8_t back = 1;
	uint8_t top = 1;
	uint8_t bottom = 1;
};

enum class TileType : uint8_t
{
	air,
	grass,
	sand,
	stone,
	road,
	dirt,
};

enum class TileModel : uint8_t
{
	SolidBox,
	Model1,
	Model2,
	Model3,
};

class Tile
{
public:
	Color color{ 255, 255, 255, 255 };
	TileSide tileSideVisible;
	TileType type;
	TileModel model;
	float heightOffset = 0.0f;
};

class TileChunk
{
public:
	GeometryBufferRef geom;
};