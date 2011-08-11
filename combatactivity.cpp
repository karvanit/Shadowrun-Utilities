/*
 * Copyright © 2011 Konstantinos Arvanitis <konstantinos.arvanitis@gmail.com>
 *
 * This file is part of Shadowrun Utilities.
 *
 * Shadowrun Utilities program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "combatactivity.h"
#include "initiativeinput.h"
#include "combatactor.h"
#include "inputtable.h"

inline InitiativeInput *CombatActivity::inputPage() const
{
	return static_cast<InitiativeInput*>(widget(0));
}

CombatActivity::CombatActivity(QWidget *parent)
  : QStackedWidget(parent), actors()
{
	InputTable *it = new InputTable(actors, this);
	it->setObjectName("inputData");
	InitiativeInput *ii = new InitiativeInput(it);
	ii->setObjectName("inputPage");
	addWidget(ii);
	QMetaObject::connectSlotsByName(this);
}

CombatActivity::~CombatActivity()
{
}

// TODO: Use the information to run the combat.
