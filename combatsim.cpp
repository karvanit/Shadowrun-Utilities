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
#include "combatsim.h"
#include "combatactor.h"
#include "inputtable.h"
#include "combattable.h"

using namespace Shadowrun;

CombatSim::CombatSim(InputTable *it, QWidget *parent)
  : QWidget(parent)
{
	ui.setupUi(this);
	CombatTable *ct = new CombatTable(this);
	ct->setSourceModel(it);
	QItemSelectionModel *m = ui.actorsView->selectionModel();
	ui.actorsView->setModel(ct);
	m->deleteLater();
	connect(ui.switchToInput, SIGNAL(clicked()), SIGNAL(switchPage()));
	connect(ui.nextPass, SIGNAL(clicked()), ct, SLOT(nextPass()));
}

CombatSim::~CombatSim()
{

}

// TODO: Context action - Use Edge on initiative: Reroll initiative, using edge dice. Resort
// TODO: Context action - Add Edge on initiative: Change initiative score by adding Edge dice results. Resort
// TODO: Context action - Add action: Add another pass on the actor.
// TODO: Context action - Play First: Move this actor to play first. Resort
// TODO: Context action - Remove from combat: Remove this actor from the list.
// TODO: Add Surprise Round
