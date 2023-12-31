/*
 * Copyright 2015-2022 Arx Libertatis Team (see the AUTHORS file)
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

#include "graphics/effects/RotatingCone.h"

#include "animation/AnimationRender.h"
#include "core/GameTime.h"
#include "graphics/RenderBatcher.h"
#include "math/Random.h"

RotatingCone::RotatingCone()
	: m_pos(0.f)
	, m_currdurationang(0)
	, m_ang(0.f)
	, m_coneScale(0.f)
	, m_tsouffle(nullptr)
{ }

void RotatingCone::Init(float rbase, float rhaut, float hauteur) {
	
	Vec3f * vertex = conevertex;
	float a = 0.f;
	float da = 360.f / float(Def);
	
	int nb = VertexCount / 2;
	
	while(nb) {
		*vertex++ = Vec3f(rhaut * std::cos(glm::radians(a)), -hauteur, rhaut * std::sin(glm::radians(a)));
		*vertex++ = Vec3f(rbase * std::cos(glm::radians(a)), 0.f, rbase * std::sin(glm::radians(a)));
		a += da;
		nb--;
	}
	
	m_tsouffle = TextureContainer::Load("graph/obj3d/textures/(fx)_sebsouffle");
	
}

void RotatingCone::Update(ShortGameDuration timeDelta, Vec3f pos, float coneScale) {
	
	arx_assume(timeDelta >= 0 && timeDelta <= GameTime::MaxFrameDuration);
	
	m_currdurationang += timeDelta;
	
	m_pos = pos;
	m_coneScale = coneScale;
	
	m_ang = m_currdurationang / 1s;
	
	if(m_ang > 1.f) {
		m_currdurationang = 0;
		m_ang = 1.f;
	}
	
	for(math::RandomFlicker & flicker : m_flicker) {
		flicker.update(toMsf(timeDelta) * 0.03f);
	}
	
}

void RotatingCone::Render() {
	
	float u = m_ang;
	float du = 2.f / float(Def);
	
	Vec3f * vertex = conevertex;
	math::RandomFlicker * flicker = m_flicker;
	TexturedVertexUntransformed * d3dv = coned3d;
	int nb = VertexCount / 2;
	
	while(nb) {
		
		d3dv->p = m_pos + *(vertex + 1) + ((*vertex - *(vertex + 1)) * m_coneScale);
		d3dv->color = Color4f::gray(flicker->get() * 0.32f, flicker->get() * 0.32f).toRGBA();
		d3dv->uv.x = u;
		d3dv->uv.y = 0.f;
		vertex++;
		flicker++;
		d3dv++;
		
		d3dv->p = m_pos + Vec3f(vertex->x, 0.f, vertex->z);
		d3dv->color = Color4f::gray(0.f, flicker->get() * 0.32f).toRGBA();
		d3dv->uv.x = u;
		d3dv->uv.y = 1.f;
		vertex++;
		flicker++;
		d3dv++;
		
		u += du;
		nb--;
	}
	
	coned3d[std::size(coned3d) - 2].color = coned3d[0].color;
	coned3d[std::size(coned3d) - 1].color = coned3d[1].color;
	
	RenderMaterial mat;
	mat.setDepthTest(true);
	mat.setBlendType(RenderMaterial::Additive);
	mat.setWrapMode(TextureStage::WrapMirror);
	mat.setTexture(m_tsouffle);
	
	int i = FaceCount - 2;
	int j = 0;
	while(i--) {
		drawTriangle(mat, &coned3d[j]);
		j++;
	}
	
}
