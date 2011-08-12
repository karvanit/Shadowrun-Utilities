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
#include <QMenu>
#include <QAction>
using namespace Shadowrun;

InitiativeInput::InitiativeInput(InputTable *it, QWidget *parent)
  : QWidget(parent), current_actor(-1)
{
	actor_menu = new QMenu(this);
	QAction *action = actor_menu->addAction(tr("Remove actor"));
	action->setObjectName("removeActor");
	ui.setupUi(this);	//In here we connect signals based on object names.
	QItemSelectionModel *m = ui.actorsView->selectionModel();
	ui.actorsView->setModel(it);
	m->deleteLater();
	ui.actorsView->setSelectionMode(QAbstractItemView::NoSelection);
	connect(ui.runCombat, SIGNAL(clicked()), SIGNAL(switchPage()));
}

InitiativeInput::~InitiativeInput()
{
}

/** Add an actor at the end of our list.
 */
void
InitiativeInput::on_addActor_clicked()
{
	static_cast<InputTable*>(ui.actorsView->model())->addActor();
}

/** If we are over an actor, present a context menu with options.
 *  Only option for now is to delete the actor.
 *  We have to remember the actor index, since when the action triggers we don't have the position.
 * @param pos The widget coordinates of the mouse cursor.
 */
void
InitiativeInput::on_actorsView_customContextMenuRequested(const QPoint &pos)
{
	QModelIndex idx = ui.actorsView->indexAt(pos);
	if (idx.isValid()) {
		current_actor = idx.row();
		actor_menu->exec(ui.actorsView->mapToGlobal(pos));
	}
}

/** Remove the actor at the stored index.
 */
void
InitiativeInput::on_removeActor_triggered()
{
	static_cast<InputTable*>(ui.actorsView->model())->removeActor(current_actor);
}
