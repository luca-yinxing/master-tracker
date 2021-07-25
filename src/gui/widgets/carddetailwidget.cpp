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

#include "carddetailwidget.hpp"

#include "ui_carddetailwidget.h"

#include <hsdata/conv.hpp>

using namespace mtk;

namespace mtk
{
CardDetailWidget::CardDetailWidget(QWidget* parent) :
    QWidget(parent), ui(new Ui_CardDetailWidget)
{
	ui->setupUi(this);

	ui->label_code->setVisible(false);
	ui->label_id->setVisible(false);
	ui->label_dbid->setVisible(false);
	ui->label_code->setVisible(false);
	ui->line_code->setVisible(false);
	ui->label_name->setVisible(false);
	ui->line_name->setVisible(false);
	ui->label_rarity->setVisible(false);
	ui->line_rarity->setVisible(false);
	ui->label_type->setVisible(false);
	ui->line_type->setVisible(false);
	ui->label_class->setVisible(false);
	ui->line_class->setVisible(false);
	ui->label_race->setVisible(false);
	ui->line_race->setVisible(false);
	ui->label_set->setVisible(false);
	ui->line_set->setVisible(false);
	ui->label_spellschool->setVisible(false);
	ui->line_spellschool->setVisible(false);
	ui->label_faction->setVisible(false);
	ui->line_faction->setVisible(false);
	ui->label_cost->setVisible(false);
	ui->label_collectible->setVisible(false);
	ui->check_collectible->setVisible(false);
	ui->check_collectible->setAttribute(Qt::WA_TransparentForMouseEvents);
	ui->check_collectible->setFocusPolicy(Qt::NoFocus);
	ui->label_elite->setVisible(false);
	ui->check_elite->setVisible(false);
	ui->check_elite->setAttribute(Qt::WA_TransparentForMouseEvents);
	ui->check_elite->setFocusPolicy(Qt::NoFocus);
	ui->label_mechanic->setVisible(false);
	ui->line_mechanic->setVisible(false);
	ui->label_text->setVisible(false);
	ui->line_text->setVisible(false);
	ui->label_artist->setVisible(false);
	ui->line_artist->setVisible(false);
	ui->label_flavor->setVisible(false);
	ui->line_flavor->setVisible(false);
	ui->label_attack->setVisible(false);
	ui->spin_attack->setVisible(false);
	ui->label_health->setVisible(false);
	ui->spin_health->setVisible(false);
	ui->label_overload->setVisible(false);
	ui->spin_overload->setVisible(false);
	ui->label_durability->setVisible(false);
	ui->spin_durability->setVisible(false);
	ui->spin_cost->setVisible(false);
	ui->label_spelldamage->setVisible(false);
	ui->spin_spelldamage->setVisible(false);
	ui->label_armor->setVisible(false);
	ui->spin_armor->setVisible(false);
	ui->label_tag->setVisible(false);
	ui->line_tag->setVisible(false);
	ui->label_questreward->setVisible(false);
	ui->line_questreward->setVisible(false);
	ui->spin_id->setVisible(false);
	ui->spin_dbid->setVisible(false);
}

CardDetailWidget::~CardDetailWidget() { }

static inline void
s_display_element_uint(QSpinBox* spin, QLabel* label, uint32_t data)
{
	if (data > 0) {
		spin->setVisible(true);
		label->setVisible(true);
		spin->setValue(data);
	} else {
		spin->setVisible(false);
		label->setVisible(false);
	}
}

static inline void
s_display_element_int(QSpinBox* spin, QLabel* label, int data)
{
	if (data >= 0) {
		spin->setVisible(true);
		label->setVisible(true);
		spin->setValue(data);
	} else {
		spin->setVisible(false);
		label->setVisible(false);
	}
}

static inline void
s_display_element_string(QLineEdit* line, QLabel* label, const char* data)
{
	if (strok(data)) {
		line->setVisible(true);
		label->setVisible(true);
		line->setText(data);
	} else {
		line->setVisible(false);
		label->setVisible(false);
	}
}

void
CardDetailWidget::init(const std::shared_ptr<Card>& pcard)
{
	if (pcard == nullptr) {
		return;
	}

	// s_display_element_int(ui->spin_id, ui->label_id, pcard->id);

	// const card* ptr = pcard->ptr;
	// if (ptr == nullptr) {
	//    return;
	//}

	s_display_element_uint(ui->spin_dbid, ui->label_dbid, pcard->id());
	s_display_element_string(ui->line_code,
				 ui->label_code,
				 pcard->code().c_str());
	s_display_element_string(ui->line_name,
				 ui->label_name,
				 pcard->name().c_str());
	s_display_element_string(ui->line_rarity,
				 ui->label_rarity,
				 conv_rarity_string(pcard->rarity()));
	s_display_element_string(ui->line_type,
				 ui->label_type,
				 conv_type_string(pcard->type()));
	// s_display_element_string(ui->line_class, ui->label_class,
	// conv_class_string(pcard->classes));
	s_display_element_string(ui->line_race,
				 ui->label_race,
				 conv_race_string(pcard->race()));
	s_display_element_string(ui->line_set,
				 ui->label_set,
				 conv_set_string(pcard->set()));
	s_display_element_string(ui->line_spellschool,
				 ui->label_spellschool,
				 conv_spellschool_string(pcard->spellschool()));
	s_display_element_string(ui->line_faction,
				 ui->label_faction,
				 conv_faction_string(pcard->faction()));
	s_display_element_int(ui->spin_cost, ui->label_cost, pcard->cost());
	ui->check_collectible->setChecked(pcard->collectible());
	ui->label_collectible->setVisible(true);
	ui->check_collectible->setVisible(true);
	ui->check_elite->setChecked(pcard->elite());
	ui->label_elite->setVisible(true);
	ui->check_elite->setVisible(true);
	// s_display_element_string(ui->line_tag, ui->label_tag,
	// conv_tag_string(pcard->tag));
	// s_display_element_string(ui->line_mechanic, ui->label_mechanic,
	// conv_mechanic_string(pcard->mechanic));
	s_display_element_string(ui->line_text,
				 ui->label_text,
				 pcard->text().c_str());
	s_display_element_string(ui->line_artist,
				 ui->label_artist,
				 pcard->artist().c_str());
	s_display_element_string(ui->line_flavor,
				 ui->label_flavor,
				 pcard->flavor().c_str());
	s_display_element_int(ui->spin_attack,
			      ui->label_attack,
			      pcard->attack());
	s_display_element_int(ui->spin_health,
			      ui->label_health,
			      pcard->health());
	s_display_element_int(ui->spin_overload,
			      ui->label_overload,
			      pcard->overload());
	s_display_element_int(ui->spin_durability,
			      ui->label_durability,
			      pcard->durability());
	s_display_element_int(ui->spin_spelldamage,
			      ui->label_spelldamage,
			      pcard->spelldamage());
	s_display_element_int(ui->spin_armor, ui->label_armor, pcard->armor());
	s_display_element_string(ui->line_questreward,
				 ui->label_questreward,
				 pcard->questreward().c_str());
}

};    // namespace mtk