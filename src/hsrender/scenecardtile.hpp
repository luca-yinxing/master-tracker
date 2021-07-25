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

#ifndef MTK_HSRENDER_SCENECARDTILE_H
#define MTK_HSRENDER_SCENECARDTILE_H

#include <hsdata/card.hpp>

#include <hsrender/obj/bannerrace.hpp>
#include <hsrender/obj/bannerspell.hpp>
#include <hsrender/obj/cardart.hpp>
#include <hsrender/obj/cardbannername.hpp>
#include <hsrender/obj/cardframe.hpp>
#include <hsrender/obj/cardname.hpp>
#include <hsrender/obj/cardtile.hpp>
#include <hsrender/obj/center_text.hpp>
#include <hsrender/obj/elitedragon.hpp>
#include <hsrender/obj/gemattack.hpp>
#include <hsrender/obj/gemhealth.hpp>
#include <hsrender/obj/gemmana.hpp>
#include <hsrender/obj/gemrarity.hpp>
#include <hsrender/obj/linear_gradient.hpp>
#include <hsrender/obj/textblock.hpp>
#include <hsrender/obj/watermark.hpp>
#include <hsrender/scene.hpp>

namespace mtk
{
class SceneCardTile : public Scene
{
      private:
	std::shared_ptr<Card> m_card;
	std::string m_artfile;

	static constexpr int TILE_WIDTH	 = 256;
	static constexpr int TILE_HEIGHT = 59;

      public:
	SceneCardTile() noexcept :
	    Scene(TILE_WIDTH, TILE_HEIGHT, true), m_card(nullptr){};
	SceneCardTile(const std::shared_ptr<Card>& card,
		      const std::string& artfile) noexcept :
	    Scene(TILE_WIDTH, TILE_HEIGHT, true),
	    m_card(card), m_artfile(artfile){};
	SceneCardTile(const SceneCardTile& cpy) noexcept = default;
	~SceneCardTile() noexcept			 = default;

	void init()
	{
		std::map<float, QColor> linear_gradient_map;
		const unsigned char* color =
		    conv_class_color(m_card->get_main_class());
		linear_gradient_map.insert(
		    { 0.f, QColor(color[0], color[1], color[2]) });
		linear_gradient_map.insert(
		    { 0.2f, QColor(color[0], color[1], color[2]) });
		linear_gradient_map.insert({ 1.f, QColor(0, 0, 0, 0) });

		add_render_object(std::make_unique<CardTile>(m_card->code()));
		add_render_object(
		    std::make_unique<LinearGradient>(linear_gradient_map));
		add_render_object(std::make_unique<CenterText>(m_card->name(),
							       "Belwe Bd BT",
							       29.f,
							       1.5f));
	}
};
}    // namespace mtk

#endif