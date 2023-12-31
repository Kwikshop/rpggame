/*
 * Copyright 2019-2022 Arx Libertatis Team (see the AUTHORS file)
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

#ifndef ARX_GRAPHICS_PARTICLE_PARTICLETEXTURES_H
#define ARX_GRAPHICS_PARTICLE_PARTICLETEXTURES_H

#include <cstddef>

class TextureContainer;

struct ParticleTextures {
	
	TextureContainer * smoke;
	TextureContainer * bloodsplat[6];
	TextureContainer * blood_splat;
	TextureContainer * water_splat[3];
	TextureContainer * water_drop[3];
	TextureContainer * healing;
	TextureContainer * tzupouf;
	TextureContainer * fire;
	TextureContainer * fire2;
	TextureContainer * fire_hit;
	TextureContainer * boom;
	
	// TextureContainer for animated explosion bitmaps (24 frames)
	static const size_t MAX_EXPLO = 24;
	TextureContainer * explo[MAX_EXPLO];
	
	void init();
	
};

extern ParticleTextures g_particleTextures;

#endif // ARX_GRAPHICS_PARTICLE_PARTICLETEXTURES_H
