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

#ifndef HSJSON_JSONDL_H
#define HSJSON_JSONDL_H

#include <stdio.h>
#include <stdlib.h>

#include <utils/utils.hpp>

#include <string>
#include <vector>

namespace mtk
{
constexpr const char* URL_STR_HSJSON = "https://api.hearthstonejson.com/v1/";
constexpr const char* URL_STR_HSJSON_IMAGE =
    "https://art.hearthstonejson.com/v1/render/latest/";
constexpr const char* URL_STR_HSJSON_TILE =
    "https://art.hearthstonejson.com/v1/tiles/";
constexpr const char* URL_STR_HSJSON_ART =
    "https://art.hearthstonejson.com/v1/";

constexpr const char* DEF_HSJSON_BUILD	    = "latest";
constexpr const char* DEF_HSJSON_LANG	    = "all";
constexpr const char* DEF_HSJSON_CARDS_FILE = "cards.json";

enum image_resolution
{
	INVALID_RESOLUTION = 0,
	X256		   = 1,
	X512		   = 2,
	ORIG		   = 3
};

const char* image_resolution_string[] = { nullptr, "256x", "512x", "orig" };

void* init_json_curl() noexcept;

execode free_json_curl(void* curl_handle) noexcept;

/**
 * \brief Download a file to path from url
 *
 * \param curl_handle pointer of curl structure
 * \param path where save the file
 * \param url where download the file
 * \return EXE_INVALID_ARGS if url or path or curl_handle are nullptr
 * \return EXE_ERROR if cannot download the file
 * \return EXE_CANT_OPEN if cant open file in path
 * \return EXE_OK if success
 */
execode download_file(void* curl_handle,
		      const std::string& path,
		      bool override = false) noexcept;

execode download_json_cards(void* curl_handle,
			    const std::string& path,
			    const std::string& build = DEF_HSJSON_BUILD,
			    const std::string& lang  = DEF_HSJSON_LANG,
			    bool override	     = false) noexcept;

execode download_json_image_tile(void* curl_handle,
				 const std::string& path,
				 const std::string& code,
				 bool override = false) noexcept;

size_t download_json_image_tile(void* curl_handle,
				const std::string& path,
				const std::vector<std::string>& codelist,
				bool override = false) noexcept;

execode download_json_image(void* curl_handle,
			    const std::string& path,
			    const std::string& lang,
			    const std::string& code,
			    image_resolution resolution = X256,
			    bool override		= false) noexcept;

size_t download_json_image(void* curl_handle,
			   const std::string& path,
			   const std::string& lang,
			   const std::vector<std::string>& codelist,
			   image_resolution resolution = X256,
			   bool override	       = false) noexcept;

execode download_json_art(void* curl_handle,
			  const std::string& path,
			  const std::string& code,
			  image_resolution resolution = ORIG,
			  bool override		      = false) noexcept;

size_t download_json_art(void* curl_handle,
			 const std::string& path,
			 const std::vector<std::string>& codelist,
			 image_resolution resolution = ORIG,
			 bool override		     = false) noexcept;

}    // namespace mtk

#endif