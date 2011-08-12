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
#include "inputtable.h"
#include "combatactor.h"

using namespace Shadowrun;

Qt::ItemFlags
InputTable::flags(const QModelIndex & index ) const
{
	switch (index.column()) {
	case CF_Actor:
	case CF_Initiative:
	case CF_Passes:
	case CF_Edge:
	case CF_Wounded:
		return Qt::ItemIsEnabled | Qt::ItemIsEditable;
	}
	return Qt::NoItemFlags;
}

QVariant
InputTable::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole) {
		if (orientation == Qt::Horizontal) {
			switch (section) {
			case CF_Actor:
				return tr("Name");
			case CF_Initiative:
				return tr("Initiative");
			case CF_Passes:
				return tr("Passes");
			case CF_Edge:
				return tr("Edge");
			case CF_Wounded:
				return tr("Wounds");
			}
		}
	}
	return QVariant();
}

QVariant
InputTable::data(const QModelIndex & index, int role) const
{
	if (role == Qt::EditRole || role == Qt::DisplayRole) {
		const CombatActor &actor = actors.at(index.row());
		switch (index.column()) {
		case CF_Actor:
			return actor.name;
		case CF_Initiative:
			return actor.initiative;
		case CF_Passes:
			return actor.passes;
		case CF_Edge:
			return actor.edge;
		case CF_Wounded:
			return actor.wound_mod;
		}
	}
	return QVariant();
}

bool
InputTable::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if (role != Qt::EditRole)
		return false;
	CombatActor &actor = actors[index.row()];
	switch (index.column()) {
	case CF_Actor:
		actor.name = value.toString();
		break;
	case CF_Initiative:
		actor.initiative = value.toInt();
		break;
	case CF_Passes:
		actor.passes = value.toInt();
		break;
	case CF_Edge:
		actor.edge = value.toInt();
		break;
	case CF_Wounded:
		actor.init_score -= actor.wound_mod << 4;
		actor.wound_mod = value.toInt();
		actor.init_score += actor.wound_mod << 4;
		break;
	default:
		return false;
	}
	emit dataChanged(index, index);
	return true;
}

/** Add a single actor at the end of the list.
 *  It will have the default values for all it's fields.
 */
void
InputTable::addActor()
{
	beginInsertRows(QModelIndex(), actors.size(), actors.size());
	actors.push_back(CombatActor());
	endInsertRows();
}

/** Remove the actor on the given row.
 * @param row The row of the actor to remove.
 */
void
InputTable::removeActor(int row)
{
	beginRemoveRows(QModelIndex(), row, row);
	actors.removeAt(row);
	endRemoveRows();
}
