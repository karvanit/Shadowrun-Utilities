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
#ifndef INITIATIVEACTIVITY_H
#define INITIATIVEACTIVITY_H

#include <QtGui/QWidget>
#include "ui_initiativeinput.h"
class CombatActivity;
class InputTable;

class InitiativeInput : public QWidget {
	Q_OBJECT

public:
	InitiativeInput(InputTable *it, QWidget *parent = 0);
	~InitiativeInput();

signals:
	void switchPage();

private slots:
	void on_addActor_clicked();
	void on_actorsView_customContextMenuRequested(const QPoint &pos);
	void on_removeActor_triggered();

private:
	int current_actor;	//!< Used by the context menu.
	QMenu *actor_menu;	//!< The context menu on actors.
	Ui::InitiativeInputClass ui;
};

#endif // INITIATIVEACTIVITY_H
