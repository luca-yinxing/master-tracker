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

#include "jsondl.hpp"

#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <curl/curl.h>

namespace mtk
{
inline static size_t
s_write_data(void* ptr, size_t size, size_t nmemb, void* stream) noexcept
{
	size_t written = fwrite(ptr, size, nmemb, (FILE*)stream);
	return written;
}

execode
download_file(void* curl_handle,
	      const std::string& path,
	      const std::string& url,
	      bool override) noexcept
{
	if (path.empty() || url.empty() || curl_handle == nullptr) {
		return EXE_INVALID_ARGS;
	}

	if (override == false) {
		if (file_exists(path)) {
			return EXE_OK;
		}
	}

	CURLcode res;
	FILE* fd;

	/* set URL to get here */
	curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());

	/* open the file */
	fd = fopen(path.c_str(), "wb");

	if (!fd) {
		return EXE_CANT_OPEN;
	}

	/* write the page body to this file handle */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, fd);

	res = curl_easy_perform(curl_handle);

	if (res != CURLE_OK) {
		fclose(fd);
		return EXE_ERROR;
	}

	/* close the file */
	fclose(fd);

	return EXE_OK;
}

void*
init_json_curl() noexcept
{
	CURL* curl_handle;

	curl_global_init(CURL_GLOBAL_ALL);

	/* init the curl session */
	curl_handle = curl_easy_init();

	/* Switch on full protocol/debug output while testing */
	// curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);

	/* disable progress meter, set to 0L to enable it */
	// curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);

	/* send all data to this function  */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, s_write_data);

	curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 1L);
	curl_easy_setopt(curl_handle,
			 CURLOPT_SSLVERSION,
			 CURL_SSLVERSION_TLSv1_2);
	curl_easy_setopt(curl_handle, CURLOPT_FAILONERROR, 1L);
	curl_easy_setopt(curl_handle,
			 CURLOPT_FOLLOWLOCATION,
			 1L);	 // follow redirects
	curl_easy_setopt(curl_handle,
			 CURLOPT_HTTPPROXYTUNNEL,
			 1L);	 // corp. proxies etc.

	return curl_handle;
}

execode
free_json_curl(void* curl_handle) noexcept
{
	/* cleanup curl stuff */
	curl_easy_cleanup(curl_handle);

	curl_global_cleanup();

	return EXE_OK;
}

execode
download_json_cards(void* curl_handle,
		    const std::string& path,
		    const std::string& build,
		    const std::string& lang,
		    bool override) noexcept
{
	if (path.empty() || curl_handle == nullptr || build.empty() ||
	    lang.empty()) {
		return EXE_INVALID_ARGS;
	}

	std::string tmp_url = URL_STR_HSJSON + build + STR_URL_SEP + lang +
			      STR_URL_SEP + DEF_HSJSON_CARDS_FILE;

	return download_file(curl_handle, path, tmp_url, override);
}

execode
download_json_image_tile(void* curl_handle,
			 const std::string& path,
			 const std::string& code,
			 bool override) noexcept
{
	if (code.empty() || curl_handle == nullptr) {
		return EXE_INVALID_ARGS;
	}

	const char* IMAGE_FORMAT = ".jpg";

	std::string tmp_url = URL_STR_HSJSON_TILE + code + IMAGE_FORMAT;

	std::string tmp_path = path + STR_OS_DIR_SEP + code + IMAGE_FORMAT;

	if (os_mkdir(path) != EXE_OK) {
		return EXE_ERROR_OS;
	}

	return download_file(curl_handle, tmp_path, tmp_url, override);
}

size_t
download_json_image_tile(void* curl_handle,
			 const std::string& path,
			 const std::vector<std::string>& codelist,
			 bool override) noexcept
{
	if (path.empty() || curl_handle == nullptr) {
		return EXE_INVALID_ARGS;
	}

	size_t pushed_items = 0;
	execode res;
	const char* IMAGE_FORMAT = ".jpg";

	std::string tmp_url =
	    URL_STR_HSJSON_TILE;    // + nullptr + IMAGE_FORMAT;

	std::string tmp_path =
	    path + STR_OS_DIR_SEP;    //+ nullptr + IMAGE_FORMAT

	if (os_mkdir(path) != EXE_OK) {
		return EXE_ERROR_OS;
	}

	for (const std::string& code : codelist) {
		res = download_file(curl_handle,
				    tmp_path + code + IMAGE_FORMAT,
				    tmp_url + code + IMAGE_FORMAT,
				    override);

		if (res != EXE_OK) {
			// remove(path_buffer);
		} else {
			pushed_items++;
		}
	}

	return pushed_items;
}

execode
download_json_image(void* curl_handle,
		    const std::string& path,
		    const std::string& lang,
		    const std::string& code,
		    image_resolution resolution,
		    bool override) noexcept
{
	if (path.empty() || lang.empty() || code.empty() ||
	    curl_handle == nullptr) {
		return EXE_INVALID_ARGS;
	}

	const char* IMAGE_FORMAT = ".png";

	std::string tmp_url = URL_STR_HSJSON_IMAGE + lang + STR_URL_SEP +
			      image_resolution_string[resolution] +
			      STR_URL_SEP + code + IMAGE_FORMAT;

	std::string tmp_path = path + STR_OS_DIR_SEP + code + IMAGE_FORMAT;

	if (os_mkdir(path) != EXE_OK) {
		return EXE_ERROR_OS;
	}

	return download_file(curl_handle, tmp_path, tmp_url, override);
}

size_t
download_json_image(void* curl_handle,
		    const std::string& path,
		    const std::string& lang,
		    const std::vector<std::string>& codelist,
		    image_resolution resolution,
		    bool override) noexcept
{
	if (path.empty() || lang.empty() || curl_handle == nullptr) {
		return EXE_INVALID_ARGS;
	}

	size_t pushed_items = 0;
	execode res;

	const char* IMAGE_FORMAT = ".png";

	std::string tmp_url = URL_STR_HSJSON_IMAGE + lang + STR_URL_SEP +
			      image_resolution_string[resolution] + STR_URL_SEP;
	// nullptr, IMAGE_FORMAT;

	std::string tmp_path =
	    path + STR_OS_DIR_SEP;    // nullptr, IMAGE_FORMAT };

	if (os_mkdir(path) != EXE_OK) {
		return EXE_ERROR_OS;
	}

	for (const std::string& code : codelist) {
		res = download_file(curl_handle,
				    tmp_path + code + IMAGE_FORMAT,
				    tmp_url + code + IMAGE_FORMAT,
				    override);

		if (res != EXE_OK) {
			// remove(path_buffer);
		} else {
			pushed_items++;
		}
	}

	return pushed_items;
}

execode
download_json_art(void* curl_handle,
		  const std::string& path,
		  const std::string& code,
		  image_resolution resolution,
		  bool override) noexcept
{
	if (code.empty() || curl_handle == nullptr) {
		return EXE_INVALID_ARGS;
	}

	const char* IMAGE_FORMAT = ".png";

	std::string tmp_url = std::string(URL_STR_HSJSON_ART) +
			      image_resolution_string[resolution] +
			      STR_URL_SEP + code + IMAGE_FORMAT;

	std::string tmp_path = path + STR_OS_DIR_SEP + code + IMAGE_FORMAT;

	if (os_mkdir(path) != EXE_OK) {
		return EXE_ERROR_OS;
	}

	return download_file(curl_handle, tmp_path, tmp_url, override);
}

size_t
download_json_art(void* curl_handle,
		  const std::string& path,
		  const std::vector<std::string>& codelist,
		  image_resolution resolution,
		  bool override) noexcept
{
	if (path.empty() || curl_handle == nullptr) {
		return EXE_INVALID_ARGS;
	}

	size_t pushed_items = 0;
	execode res;
	const char* IMAGE_FORMAT = ".png";

	std::string tmp_url = std::string(URL_STR_HSJSON_ART) +
			      image_resolution_string[resolution] +
			      STR_URL_SEP;    // nullptr, IMAGE_FORMAT;

	std::string tmp_path = path + STR_OS_DIR_SEP;
	// nullptr, IMAGE_FORMAT

	if (os_mkdir(path) != EXE_OK) {
		return EXE_ERROR_OS;
	}

	for (const std::string& code : codelist) {
		res = download_file(curl_handle,
				    tmp_path + code + IMAGE_FORMAT,
				    tmp_url + code + IMAGE_FORMAT,
				    override);

		if (res != EXE_OK) {
			// remove(path_buffer);
		} else {
			pushed_items++;
		}
	}

	return pushed_items;
}

}    // namespace mtk