#pragma once

#include "Tile.h"

struct Viewport;

class Map
{
public:
	bool Create();
	void Destroy();

	void Update(float deltaTime);
	void Draw(const Viewport& view);

	std::array<std::array<Tile, MapSize>, MapSize> tiles;
	std::array<std::array<TileChunk, ChunkSize>, ChunkSize> chunks;
};