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

#include "filterwidget.hpp"

namespace mtk
{
FilterWidget::FilterWidget(QWidget* parent) : QListWidget(parent)
{
	setFlow(QListView::LeftToRight);
	setSelectionMode(QAbstractItemView::MultiSelection);
	setIconSize(QSize(32, 32));
}

FilterWidget::~FilterWidget() { }

void
FilterWidget::add_item(int value, const std::string& icon_path)
{
	QListWidgetItem* new_item =
	    new QListWidgetItem(QIcon(icon_path.c_str()), "");

	new_item->setData(Qt::UserRole, value);

	this->addItem(new_item);
}

}    // namespace mtk