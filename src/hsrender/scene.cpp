// Copyright (C) 2021 Luca Gasperini
//
// This file is part of Master Tracker.
//
// Master Tracker is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Master Tracker is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Master Tracker.  If not, see <http://www.gnu.org/licenses/>.

#include "scene.hpp"

#include <filesystem>

namespace mtk
{
void
Scene::add_asset(
    const std::string& base_dir,
    const std::string& asset_file,
    std::shared_ptr<QPixmap>& asset_pixmap,
    std::unordered_map<std::string, std::shared_ptr<QPixmap>>& assets)
{
	if (asset_pixmap != nullptr) {
		return;
	}
	if (assets.contains(asset_file)) {
		asset_pixmap = assets.at(asset_file);
	} else {
		std::filesystem::path tmp_path(asset_file);
		if (tmp_path.is_absolute()) {
			tmp_path = asset_file;
		} else {
			tmp_path = base_dir + asset_file;
		}
		std::shared_ptr<QPixmap> ptr =
		    std::make_shared<QPixmap>(tmp_path.c_str());
		assets.insert({ asset_file, ptr });
		asset_pixmap = ptr;
	}
}

uint32_t
Scene::add_render_object(std::unique_ptr<RenderObj>&& pobj,
			 uint32_t id) noexcept
{
	if (id == 0 && m_render_list.size() > 0) {
		id = m_render_list.rbegin()->first + 1;
	}
	if (pobj->pixmap() == nullptr) {
		if (m_direct_draw) {
			pobj->init(m_pixmap);
		} else {
			pobj->init(Coords(width(), height()));
		}
	}
	m_render_list.insert({ id, std::move(pobj) });

	return id;
}

void
Scene::preload(
    const std::string& base_dir,
    std::unordered_map<std::string, std::shared_ptr<QPixmap>>& assets)
{
	for (auto& obj : m_render_list) {
		for (auto& texture : obj.second->textures()) {
			add_asset(base_dir,
				  texture.first,
				  texture.second,
				  assets);
		}
	}
}

void
Scene::scale(int width, int height, bool fast)
{
	Qt::AspectRatioMode arm;
	Qt::TransformationMode tm;

	if (width == this->width() && height == this->height()) {
		return;
	}

	if (width == 0 || height == 0) {
		arm = Qt::KeepAspectRatio;
	} else {
		arm = Qt::IgnoreAspectRatio;
	}

	if (width == 0 && height == 0) {
		return;
	} else if (width == 0) {
		float ratio = ((float)height) / this->height();
		width	    = roundf(ratio * this->width());
	} else if (height == 0) {
		float ratio = ((float)width) / this->width();
		height	    = roundf(ratio * this->height());
	}

	if (fast) {
		tm = Qt::FastTransformation;
	} else {
		tm = Qt::SmoothTransformation;
	}

	m_pixmap =
	    std::make_shared<QPixmap>(m_pixmap->scaled(width, height, arm, tm));
}

void
Scene::render()
{
	m_pixmap->fill(Qt::transparent);

	Coords tmp_coords;
	for (auto& obj : m_render_list) {
		if (!obj.second->is_rendered()) {
			if (obj.second->prefill() && !m_direct_draw) {
				obj.second->pixmap()->fill(
				    obj.second->prefill_color());
			}
			m_painter.begin(obj.second->pixmap().get());
			if (m_direct_draw) {
				m_painter.setCompositionMode(
				    obj.second->composition());
			}
			obj.second->render(m_painter);
			m_painter.end();
			if (!m_direct_draw) {
				obj.second->is_rendered(true);
			}
		}
		if (m_direct_draw) {
			continue;
		}
		m_painter.begin(m_pixmap.get());

		tmp_coords = obj.second->coords();
		if (tmp_coords.dw() == 0) {
			tmp_coords.dw(obj.second->pixmap()->width());
		}
		if (tmp_coords.dh() == 0) {
			tmp_coords.dh(obj.second->pixmap()->height());
		}
		if (tmp_coords.sw() == 0) {
			tmp_coords.sw(obj.second->pixmap()->width());
		}
		if (tmp_coords.sh() == 0) {
			tmp_coords.sh(obj.second->pixmap()->height());
		}

		m_painter.setCompositionMode(obj.second->composition());

		m_painter.drawPixmap(tmp_coords.dx(),
				     tmp_coords.dy(),
				     tmp_coords.dw(),
				     tmp_coords.dh(),
				     *obj.second->pixmap(),
				     tmp_coords.sx(),
				     tmp_coords.sy(),
				     tmp_coords.sw(),
				     tmp_coords.sh());
		m_painter.end();
	}
}
}    // namespace mtk