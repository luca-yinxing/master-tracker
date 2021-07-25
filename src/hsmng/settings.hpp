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

#ifndef MT_HSMNG_SETTINGS_H
#define MT_HSMNG_SETTINGS_H

#include <utils/utils.hpp>

namespace mtk
{
class settings
{
      private:
	std::string m_local_dir;
	std::string m_hsexe_dir;
	std::string m_hsuser_dir;
	std::string m_hslog_dir;
	std::string m_jsoncards;
	std::string m_hsbuild;

	std::string m_img_dir;
	std::string m_lang_dir;
	std::string m_hsconfig;
	std::string m_asset_dir;

      public:
	settings() noexcept		       = default;
	settings(const settings& cpy) noexcept = default;
	~settings() noexcept		       = default;

	void clear() noexcept { *this = settings(); }

	const std::string& local_dir() const noexcept { return m_local_dir; }
	const std::string& hsexe_dir() const noexcept { return m_hsexe_dir; }
	const std::string& hsuser_dir() const noexcept { return m_hsuser_dir; }
	const std::string& hslog_dir() const noexcept { return m_hslog_dir; }
	const std::string& jsoncards() const noexcept { return m_jsoncards; }
	const std::string& hsbuild() const noexcept { return m_hsbuild; }

	const std::string& img_dir() const noexcept { return m_img_dir; }
	const std::string& lang_dir() const noexcept { return m_lang_dir; }
	const std::string& hsconfig() const noexcept { return m_hsconfig; }
	const std::string& asset_dir() const noexcept { return m_asset_dir; }

	void local_dir(const std::string& v) noexcept { m_local_dir = v; }
	void hsexe_dir(const std::string& v) noexcept { m_hsexe_dir = v; }
	void hsuser_dir(const std::string& v) noexcept { m_hsuser_dir = v; }
	void hslog_dir(const std::string& v) noexcept { m_hslog_dir = v; }
	void jsoncards(const std::string& v) noexcept { m_jsoncards = v; }
	void hsbuild(const std::string& v) noexcept { m_hsbuild = v; }

	void img_dir(const std::string& v) noexcept { m_img_dir = v; }
	void lang_dir(const std::string& v) noexcept { m_lang_dir = v; }
	void hsconfig(const std::string& v) noexcept { m_hsconfig = v; }
	void asset_dir(const std::string& v) noexcept { m_asset_dir = v; }
};

}    // namespace mtk

#endif