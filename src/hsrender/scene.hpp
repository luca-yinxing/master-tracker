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

#ifndef MTK_HSRENDER_SCENE_H
#define MTK_HSRENDER_SCENE_H

#include "renderobj.hpp"

#include <map>
#include <unordered_map>

#include <QPainter>

namespace mtk
{
class Scene
{
      private:
	std::map<uint32_t, std::unique_ptr<RenderObj>> m_render_list;

	QPainter m_painter;

	std::shared_ptr<QPixmap> m_pixmap;
	bool m_direct_draw = false;

	void add_asset(
	    const std::string& base_dir,
	    const std::string& asset_file,
	    std::shared_ptr<QPixmap>& asset_pixmap,
	    std::unordered_map<std::string, std::shared_ptr<QPixmap>>& assets);

      public:
	Scene() noexcept = default;
	Scene(int width, int height, bool direct_draw = false) noexcept :
	    m_pixmap(std::make_shared<QPixmap>(width, height)),
	    m_direct_draw(direct_draw){};
	Scene(const Scene& cpy) noexcept = default;
	~Scene() noexcept		 = default;

	QPainter& painter() noexcept { return m_painter; }

	std::shared_ptr<QPixmap> pixmap() const noexcept { return m_pixmap; }

	int width() const noexcept { return m_pixmap->width(); }
	int height() const noexcept { return m_pixmap->height(); }

	bool direct_draw() const noexcept { return m_direct_draw; }
	void direct_draw(bool v) noexcept { m_direct_draw = v; }

	uint32_t add_render_object(std::unique_ptr<RenderObj>&& pobj,
				   uint32_t id = 0) noexcept;

	void preload(
	    const std::string& base_dir,
	    std::unordered_map<std::string, std::shared_ptr<QPixmap>>& assets);

	void scale(int width, int height, bool fast = true);
	void render();
};
}    // namespace mtk

#endif