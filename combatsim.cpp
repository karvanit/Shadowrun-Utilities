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
#include <QMenu>

using namespace Shadowrun;

CombatSim::CombatSim(InputTable *it, QWidget *parent)
  : QWidget(parent)
{
	ui.setupUi(this);
	CombatTable *ct = new CombatTable(it, this);
	QItemSelectionModel *m = ui.actorsView->selectionModel();
	ui.actorsView->setModel(ct);
	m->deleteLater();
	connect(ui.switchToInput, SIGNAL(clicked()), SIGNAL(switchPage()));
	connect(ui.nextPass, SIGNAL(clicked()), ct, SLOT(nextPass()));
	available_actions = new QMenu(this);
	available_actions->addAction(ui.actionRemoveActor);
	available_actions->addSeparator();
	available_actions->addAction(ui.actionPlayFirst);
	available_actions->addAction(ui.actionRollEdge);
	available_actions->addAction(ui.actionAddEdge);
	available_actions->addAction(ui.actionAddPass);
}

CombatSim::~CombatSim()
{

}

/** Change the data table.
 */
void
CombatSim::setTable(InputTable *it)
{
	QItemSelectionModel *m = ui.actorsView->selectionModel();
	ui.actorsView->setModel(it);
	m->deleteLater();
}

void
CombatSim::on_actorsView_customContextMenuRequested(const QPoint &pos)
{
	actor_for_action = ui.actorsView->indexAt(pos);
	if (actor_for_action.isValid()) {
		//TODO: Enable / Disable available actions
		available_actions->exec(ui.actorsView->mapToGlobal(pos));
	}
}

/** Remove the selected actor from the combat.
 */
void
CombatSim::on_actionRemoveActor_triggered()
{
	CombatTable *ct = static_cast<CombatTable*>(ui.actorsView->model());
	ct->removeActor(actor_for_action.row());
}

/** Have the actor take a full defense action.
 *  ShadowrunRule: Full defense actions can be taken out of turn, and they use up the next available action for the character.
 *                 They can also use up the first action of the next round, if required.
 */
void
CombatSim::on_actionFullDefense_triggered()
{
	//TODO: Implement Full Defense
}

/** Play first, regardless of initiative.
 *  ShadowrunRule: An entity can spend a point of Edge (not tracked in our program)
 *                 and act first, playing before everyone who did not spend an Edge point to play first.
 */
void
CombatSim::on_actionPlayFirst_triggered()
{
	//TODO: Implement Play First
}

/** Roll initiative again, using Edge dice as well.
 */
void
CombatSim::on_actionRollEdge_triggered()
{
	//TODO: Implement initiative with Edge.
}

/** Add the results of Edge rolling into the Initiative Score.
 */
void
CombatSim::on_actionAddEdge_triggered()
{
	//TODO: Implement add Edge to initiative.
}

// TODO: Add Surprise Round
