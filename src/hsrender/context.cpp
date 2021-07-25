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

#include "context.hpp"

#include <QFile>

namespace mtk
{
uint32_t
Context::add(std::unique_ptr<Scene>&& scene, uint32_t id) noexcept
{
	if (id == 0 && m_scene.size() > 0) {
		id = m_scene.rbegin()->first + 1;
	}
	scene->preload(m_base_dir, m_assets);

	m_scene.insert({ id, std::move(scene) });

	return id;
}

void
Context::render(uint32_t id)
{
	m_scene.at(id)->render();
}

void
Context::save(uint32_t id, const std::string& output_file)
{
	QFile file(output_file.c_str());
	m_scene.at(id)->pixmap()->save(&file);
}

}    // namespace mtk