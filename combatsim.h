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
#ifndef COMBATSIM_H
#define COMBATSIM_H
#include <QModelIndex>
#include <QtGui/QWidget>
#include "ui_combatsim.h"

class InputTable;
namespace Shadowrun { class CombatActor; }

class CombatSim : public QWidget {
	Q_OBJECT

public:
	CombatSim(InputTable *at, QWidget *parent = 0);
	~CombatSim();

signals:
	void switchPage();

private slots:
	void on_actorsView_customContextMenuRequested(const QPoint &pos);
	void on_actionRemoveActor_triggered();
	void on_actionFullDefense_triggered();
	void on_actionPlayFirst_triggered();
	void on_actionRollEdge_triggered();
	void on_actionAddEdge_triggered();

private:
	Ui::CombatSimClass ui;
	QMenu *available_actions;
	QModelIndex actor_for_action;
};

#endif // COMBATSIM_H
