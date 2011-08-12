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
#include "combattable.h"
#include "combatactor.h"
#include "inputtable.h"

CombatTable::CombatTable(QObject *parent)
  : QSortFilterProxyModel(parent)
{
}

CombatTable::~CombatTable()
{
}

bool
CombatTable::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	InputTable *srcmodel = static_cast<InputTable*>(sourceModel());
	return srcmodel->at(left.row()) < srcmodel->at(right.row());
}

Qt::ItemFlags
CombatTable::flags(const QModelIndex & index ) const
{
	switch (index.column()) {
	case InputTable::CF_Actor:
	case InputTable::CF_Edge:
		return Qt::ItemIsEnabled;
	case InputTable::CF_Initiative:
	case InputTable::CF_Passes:
	case InputTable::CF_Wounded:
		return Qt::ItemIsEnabled | Qt::ItemIsEditable;
	}
	return Qt::NoItemFlags;
}

/** Remove the actor on the given row.
 * @param row The row of the actor to remove.
 */
void
CombatTable::removeActor(int row)
{
	InputTable *srcmodel = static_cast<InputTable*>(sourceModel());
	srcmodel->removeActor(mapToSource(index(row, 0)).row());
}

/** Display the initiative score and status as a row label.
 */
QVariant
CombatTable::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation != Qt::Vertical)
		return sourceModel()->headerData(section, orientation, role);
	if (role != Qt::DisplayRole)
		return QVariant();
	QModelIndex src = mapToSource(index(section, 0));
	InputTable *it = static_cast<InputTable*>(sourceModel());
	if (it->at(src.row()).passes >= current_pass)
		return QVariant(QString("(") + QString::number(it->at(src.row()).init_score >> 4) + QString(")"));
	return QVariant("--");
}

void
CombatTable::nextPass()
{
	if (current_pass == 0) {
		startNextTurn();
		return;
	}
	int end = rowCount();
	++current_pass;
	bool change_round = true;
	InputTable *srcmodel = static_cast<InputTable*>(sourceModel());
	for (int ii = 0; ii < end; ++ii) {
		Shadowrun::CombatActor &actor = srcmodel->at(ii);
		if (actor.passes >= current_pass) {
			change_round = false;
			break;
		}
	}
	if (change_round)
		startNextTurn();
	else
		emit headerDataChanged(Qt::Vertical, 0, rowCount() - 1);
}
/** Calculate the new initiatives and sort our table.
 *  Also reset our variables to indicate a new turn has started.
 */
void
CombatTable::startNextTurn()
{
	int end = rowCount();
	InputTable *srcmodel = static_cast<InputTable*>(sourceModel());
	for (int ii = 0; ii < end; ++ii) {
		Shadowrun::CombatActor &actor = srcmodel->at(ii);
		actor.calculateInitiativeScore(false);
	}
	sort(Qt::AscendingOrder);
	current_pass = 1;
	emit headerDataChanged(Qt::Vertical, 0, rowCount() - 1);
	emit nextTurn();
}

const Shadowrun::CombatActor &
CombatTable::at(int row) const
{
	InputTable *srcmodel = static_cast<InputTable*>(sourceModel());
	return srcmodel->at(mapToSource(index(row, 0)).row());
}

// TODO: Recalculate the initiative score when CF_Initiative or CF_Wounded changes.
// TODO: Recalculate Initiative Score (and resort) for an actor that has Wound Modifier changed.
// TODO: Recalculate Initiative Score (and resort) for an actor that has Initiative changed.
