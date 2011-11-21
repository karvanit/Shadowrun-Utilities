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
#include "combatsim.h"
#include <QFileDialog>
#include <QFile>
#include <QDataStream>

inline InitiativeInput *CombatActivity::inputPage() const
{
	return static_cast<InitiativeInput*>(widget(0));
}

inline CombatSim *CombatActivity::combatPage() const
{
	return static_cast<CombatSim*>(widget(1));
}

CombatActivity::CombatActivity(QWidget *parent)
  : QStackedWidget(parent), actors(), input_table(0)
{
	InputTable *it = new InputTable(actors, this);
	it->setObjectName("inputData");
	InitiativeInput *ii = new InitiativeInput(it);
	ii->setObjectName("inputPage");
	addWidget(ii);
	CombatSim *cs = new CombatSim(it, this);
	cs->setObjectName("combatPage");
	addWidget(cs);
	input_table.reset(it);
	QMetaObject::connectSlotsByName(this);
}

CombatActivity::~CombatActivity()
{
}

/** Switch to the combat page.
 */
void
CombatActivity::on_inputPage_switchPage()
{
	setCurrentIndex(1);
}

/** Switch to the input page.
 */
void
CombatActivity::on_combatPage_switchPage()
{
	setCurrentIndex(0);
}

/** Save all actors to a file.
 */
void
CombatActivity::on_saveActors_clicked()
{
	QString fname = QFileDialog::getSaveFileName(this, tr("Save actors"),
						     QString(), tr("Combat files (*.combat)"));
	if (fname.isEmpty())
		return;
	QFile fout(fname);
	fout.open(QIODevice::WriteOnly);
	QDataStream out(&fout);
	for (QList<Shadowrun::CombatActor>::const_iterator ii = actors.begin(); ii != actors.end(); ++ii) {
		out << *ii;
	}
	fout.close();
}

/** Load all actors from a file.
 */
void
CombatActivity::on_loadActors_clicked()
{
	QString fname = QFileDialog::getOpenFileName(this, tr("Save actors"),
						     QString(), tr("Combat files (*.combat)"));
	if (fname.isEmpty())
		return;
	actors.clear();
	QFile fin(fname);
	fin.open(QIODevice::ReadOnly);
	QDataStream in(&fin);
	Shadowrun::CombatActor buf;
	while (!in.atEnd()) {
		in >> buf;
		actors.push_back(buf);
	}
	fin.close();
	InputTable *it = new InputTable(actors, this);
	it->setObjectName("inputData");
	inputPage()->setTable(it);
	combatPage()->setTable(it);
	input_table.reset(it);
}

QDataStream &
Shadowrun::operator<<(QDataStream &os, const Shadowrun::CombatActor &actor)
{
	os << actor.name;
	os << actor.edge;
	os << actor.initiative;
	os << actor.passes;
	os << actor.wound_mod;
	return os;
}

QDataStream &
Shadowrun::operator>>(QDataStream &is, Shadowrun::CombatActor &actor)
{
	is >> actor.name;
	is >> actor.edge;
	is >> actor.initiative;
	is >> actor.passes;
	is >> actor.wound_mod;
	return is;
}

// TODO: Use the information to run the combat.
