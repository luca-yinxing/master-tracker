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

#ifndef MTK_HSRENDER_SCENECARD_H
#define MTK_HSRENDER_SCENECARD_H

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
class SceneCard : public Scene
{
      private:
	std::shared_ptr<Card> m_card;
	std::string m_artfile;

	// char* font_title = nullptr;
	// char* font_body = nullptr;
	// char* font_gem = nullptr;
	// int font_size_body = 60;

	static constexpr int CARD_WIDTH	 = 670;
	static constexpr int CARD_HEIGHT = 1000;

      public:
	// FIXME: Can lead to SEGFAULT if m_card == nullptr
	SceneCard() noexcept :
	    Scene(CARD_WIDTH, CARD_HEIGHT, true), m_card(nullptr){};
	SceneCard(const std::shared_ptr<Card>& card,
		  const std::string& artfile) noexcept :
	    Scene(CARD_WIDTH, CARD_HEIGHT, true),
	    m_card(card), m_artfile(artfile){};
	SceneCard(const SceneCard& cpy) noexcept = default;
	~SceneCard() noexcept			 = default;

	void init()
	{
		add_render_object(
		    std::make_unique<CardArt>(m_card->type(), m_artfile));

		add_render_object(
		    std::make_unique<CardFrame>(m_card->type(),
						m_card->get_main_class()));
		if (m_card->type() != HERO_POWER) {
			if (m_card->rarity() == LEGENDARY || m_card->elite()) {
				add_render_object(std::make_unique<EliteDragon>(
				    m_card->type()));
			}
			add_render_object(
			    std::make_unique<GemRarity>(m_card->type(),
							m_card->rarity()));
			add_render_object(
			    std::make_unique<CardBannerName>(m_card->type()));
		}
		add_render_object(
		    std::make_unique<CardName>(m_card->type(), m_card->name()));
		add_render_object(
		    std::make_unique<GemMana>(m_card->type(), m_card->cost()));
		if (m_card->type() == MINION) {
			add_render_object(
			    std::make_unique<GemAttack>(m_card->type(),
							m_card->attack()));
			add_render_object(
			    std::make_unique<GemHealth>(m_card->type(),
							m_card->health()));
		} else if (m_card->type() == WEAPON) {
			add_render_object(
			    std::make_unique<GemAttack>(m_card->type(),
							m_card->attack()));
			add_render_object(
			    std::make_unique<GemHealth>(m_card->type(),
							m_card->durability()));
		} else if (m_card->type() == HERO) {
			add_render_object(
			    std::make_unique<GemHealth>(m_card->type(),
							m_card->armor()));
		}
		add_render_object(
		    std::make_unique<Watermark>(m_card->type(), m_card->set()));
		QColor text_color =
		    m_card->type() == WEAPON ? Qt::white : Qt::black;
		add_render_object(
		    std::make_unique<TextBlock>(m_card->type(),
						m_card->text(),
						"Franklin Gothic Medium Cond",
						38.f,
						1.f,
						40.f,
						text_color));

		if (m_card->type() == MINION) {
			add_render_object(
			    std::make_unique<BannerRace>(m_card->race()));
		}
		if (m_card->type() == SPELL) {
			add_render_object(std::make_unique<BannerSpell>(
			    m_card->spellschool()));
		}
	}
};
}    // namespace mtk

#endif