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

#ifndef COMBATACTIVITY_H_
#define COMBATACTIVITY_H_

#include <QStackedWidget>
#include <QList>
class InitiativeInput;
class CombatSim;
class InputTable;

namespace Shadowrun { class CombatActor; }

/** Handle initiative for Shadowrun combats.
 *  This class keeps track of the initiative score of actors in a combat and
 *  the amount of passes that are allowed. It also displays a sorted list
 *  in combat order.
 */
class CombatActivity: public QStackedWidget {
	Q_OBJECT

public:
	CombatActivity(QWidget *parent = 0);
	~CombatActivity();

private:
	InitiativeInput *inputPage() const;
	CombatSim *combatPage() const;

private slots:
	void on_inputPage_switchPage();
	void on_combatPage_switchPage();
	void on_saveActors_clicked();
	void on_loadActors_clicked();

private:
	QList<Shadowrun::CombatActor> actors;
	QScopedPointer<InputTable> input_table;
};

#endif /* COMBATACTIVITY_H_ */
