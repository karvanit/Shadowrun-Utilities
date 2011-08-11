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
#include "initiativeinput.h"
#include "combatactivity.h"
#include "combatactor.h"
#include "inputtable.h"
using namespace Shadowrun;

InitiativeInput::InitiativeInput(InputTable *it, QWidget *parent)
  : QWidget(parent)
{
	ui.setupUi(this);
	QItemSelectionModel *m = ui.actorsView->selectionModel();
	ui.actorsView->setModel(it);
	m->deleteLater();
	ui.actorsView->setSelectionMode(QAbstractItemView::NoSelection);
	connect(ui.runCombat, SIGNAL(clicked()), SIGNAL(switchPage()));
}

InitiativeInput::~InitiativeInput()
{
}

void
InitiativeInput::on_addActor_clicked()
{
	static_cast<InputTable*>(ui.actorsView->model())->addActor();
}

//TODO: Add a right-click menu which allows removing an actor. This must only work over actual actors.
