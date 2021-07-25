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

#ifndef MTK_HSRENDER_CONTEXT_H
#define MTK_HSRENDER_CONTEXT_H

#include "renderobj.hpp"
#include "scene.hpp"

#include <QPainter>
#include <QPixmap>

#include <memory>
#include <unordered_map>

namespace mtk
{
class Context
{
      private:
	std::unordered_map<std::string, std::shared_ptr<QPixmap>> m_assets;

	std::map<uint32_t, std::unique_ptr<Scene>> m_scene;

	std::string m_base_dir;

      public:
	Context() noexcept		     = default;
	Context(const Context& cpy) noexcept = default;
	~Context() noexcept		     = default;

	// void clear() noexcept { *this = Context(); }

	const std::string& base_dir() const noexcept { return m_base_dir; }

	void base_dir(const std::string& v) noexcept { m_base_dir = v; }

	std::shared_ptr<QPixmap> asset_get(const std::string& asset)
	{
		auto result = m_assets.insert(
		    { asset, std::make_shared<QPixmap>(asset.c_str()) });
		return result.first->second;
	}

	uint32_t add(std::unique_ptr<Scene>&& scene, uint32_t id = 0) noexcept;

	std::unique_ptr<Scene>& scene(uint32_t id) noexcept
	{
		return m_scene.at(id);
	}

	const std::unique_ptr<Scene>& scene(uint32_t id) const noexcept
	{
		return m_scene.at(id);
	}

	void render(uint32_t id);

	void save(uint32_t id, const std::string& output_file);
};
}    // namespace mtk

#endif