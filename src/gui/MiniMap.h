/*
 * Copyright 2011-2022 Arx Libertatis Team (see the AUTHORS file)
 *
 * This file is part of Arx Libertatis.
 *
 * Arx Libertatis is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Arx Libertatis is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Arx Libertatis.  If not, see <http://www.gnu.org/licenses/>.
 */
/* Based on:
===========================================================================
ARX FATALIS GPL Source Code
Copyright (C) 1999-2010 Arkane Studios SA, a ZeniMax Media company.

This file is part of the Arx Fatalis GPL Source Code ('Arx Fatalis Source Code'). 

Arx Fatalis Source Code is free software: you can redistribute it and/or modify it under the terms of the GNU General Public 
License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Arx Fatalis Source Code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Arx Fatalis Source Code.  If not, see 
<http://www.gnu.org/licenses/>.

In addition, the Arx Fatalis Source Code is also subject to certain additional terms. You should have received a copy of these 
additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Arx 
Fatalis Source Code. If not, please request a copy in writing from Arkane Studios at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing Arkane Studios, c/o 
ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.
===========================================================================
*/
// Code: Cyril Meynier
//
// Copyright (c) 1999-2001 ARKANE Studios SA. All rights reserved

#ifndef ARX_GUI_MINIMAP_H
#define ARX_GUI_MINIMAP_H

#include <array>
#include <string>
#include <string_view>
#include <vector>

#include "game/EntityManager.h"
#include "game/Player.h"
#include "graphics/BaseGraphicsTypes.h"
#include "graphics/VertexBuffer.h"
#include "graphics/data/Mesh.h"
#include "gui/Interface.h"
#include "gui/Text.h"
#include "gui/book/Book.h"
#include "math/Types.h"
#include "platform/Platform.h"
#include "util/HandleContainer.h"


class TextureContainer;
struct SavedMiniMap;
struct TileData;

static const size_t MINIMAP_MAX_X = 50;
static const size_t MINIMAP_MAX_Z = 50;
static const size_t MAX_MINIMAP_LEVELS = 32;

class MiniMap {
	
public:
	
	struct MiniMapData {
		
		TextureContainer * m_texContainer = nullptr;
		
		unsigned char m_revealed[MINIMAP_MAX_X][MINIMAP_MAX_Z];
		
	};
	
	struct MapMarkerData {
		
		Vec2f m_pos = Vec3f(0.f);
		MapLevel m_level;
		std::string m_name;
		std::string m_text;
		
		MapMarkerData() arx_noexcept_default
		
	};
	
	MiniMap()
		: m_entities(nullptr)
		, m_activeBkg(nullptr)
		, m_worldToMapOffset(0.f)
		, m_pTexDetect(nullptr)
		, m_mapMarkerTexCont(nullptr)
		, m_player(nullptr)
		, m_playerLastPos(0.f)
	{ }
	
	void mapMarkerRemove(std::string_view name);
	void mapMarkerAdd(const Vec2f & pos, MapLevel level, std::string && name);
	void mapMarkerInit(size_t reserveSize = 0);
	size_t mapMarkerCount();
	MapMarkerData mapMarkerGet(size_t id);
	
	void firstInit(ARXCHARACTER * pl, EntityManager * entityMng); // This should be a constructor
	void reset();
	void purgeTexContainer();
	
	//! Calls revealPlayerPos if the player moved, also sets m_currentArea and m_playerPos
	void validatePlayerPos(AreaId currentArea, bool blockPlayerControls, ARX_INTERFACE_BOOK_MODE bookMode);
	
	//! Shows the top right minimap
	void showPlayerMiniMap(MapLevel level);
	
	//! Shows the zoomed-in minimap in the book
	void showBookMiniMap(MapLevel level, Rect rect, float scale);
	
	//! Shows the entire map in the book
	void showBookEntireMap(MapLevel level, Rect rect, float scale);
	
	//! Reveals entirely all levels
	void reveal();
	
	void clearMarkerTexCont();
	
	void load(const SavedMiniMap * saved, size_t size);
	void save(SavedMiniMap * toSave, size_t size);
	
	void setActiveBackground(TileData * activeBkg);
	
private:
	
	AreaId m_currentArea;
	EntityManager * m_entities;
	TileData * m_activeBkg;
	
	util::HandleArray<AreaId, Vec2f, MAX_MINIMAP_LEVELS> m_miniOffset;
	Vec2f m_worldToMapOffset;
	
	TextureContainer * m_pTexDetect;
	TextureContainer * m_mapMarkerTexCont;
	
	ARXCHARACTER * m_player;
	Vec2f m_playerLastPos;
	
	std::vector<MapMarkerData> m_mapMarkers;
	util::HandleArray<MapLevel, MiniMapData, MAX_MINIMAP_LEVELS> m_levels;
	
	void resetLevels();
	void loadOffsets();
	
	/*!
	* Reveals the direct surroundings of the player
	*/
	void revealPlayerPos(MapLevel level);
	
	/*!
	* Gets the id from the MapMarker's name. Returns -1 when not found.
	*
	* \return MapMarker's id (int).
	*/
	int mapMarkerGetID(std::string_view name);
	
	Vec2f worldToMapPos(Vec3f pos, float zoom);
	void drawBackground(MapLevel level, Rect boundaries, Vec2f start, float zoom,
	                    float fadeBorder = 0.f, bool invColor = false, float alpha = 1.f);
	void drawPlayer(float playerSize, Vec2f playerPos, bool alphaBlending);
	void drawDetectedEntities(Vec2f start, float zoom);
	
	std::vector<TexturedVertex> m_mapVertices;
	
};

extern MiniMap g_miniMap;

#endif // ARX_GUI_MINIMAP_H
