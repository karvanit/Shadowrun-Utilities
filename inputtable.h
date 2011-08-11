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


#ifndef INPUTTABLE_H_
#define INPUTTABLE_H_
#include <QAbstractTableModel>

namespace Shadowrun { class CombatActor; }

/** Present a list of CombatActor objects according to QAbstractTableModel.
 */
class InputTable: public QAbstractTableModel {
public:
	/** Name the columns in our table, so we don't have to change everything if the actors change. */
	enum ColumnFields { CF_Actor = 0, CF_Initiative, CF_Passes, CF_Edge, CF_Wounded };
	InputTable(QList<Shadowrun::CombatActor> &actors, QObject *parent = 0) : QAbstractTableModel(parent), actors(actors) {}

	//Implementing QAbstractTableModel
	int rowCount (const QModelIndex & = QModelIndex() ) const { return actors.size(); }
	int columnCount(const QModelIndex & = QModelIndex() ) const { return int(CF_Wounded) + 1; }
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	Qt::ItemFlags flags(const QModelIndex & index ) const;

	//Allow us to add / remove actors
	void addActor();
	void removeActor(int row);
//NEEDED?	void notifyChanges() { emit dataChanged(index(0,0), index(rowCount() - 1, columnCount() - 1)); emit layoutAboutToBeChanged(); emit layoutChanged(); }

private:
	QList<Shadowrun::CombatActor> &actors;
};
#endif /* INPUTTABLE_H_ */
