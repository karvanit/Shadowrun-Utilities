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
#ifndef COMBATTABLE_H_
#define COMBATTABLE_H_
#include <QSortFilterProxyModel>
/** Handle our list of Combat actors during a combat.
 */
class CombatTable: public QSortFilterProxyModel {
	Q_OBJECT
public:
	CombatTable(QObject *parent = 0);
	~CombatTable();

	bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
	Qt::ItemFlags flags(const QModelIndex & index ) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	/** Remove an actor based on the row. */
	void removeActor(int row);

public slots:
	/** Prepare the table for a new pass.
	 *  Advance to next round if all actors have no more passes.
	 */
	void nextPass();

signals:
	/** A new combat turn has started.
	 *  All the initiative scores have already been calculated and the new order is displayed.
	 */
	void nextTurn();

private:
	int current_pass;

	void startNextTurn();
};



#endif /* COMBATTABLE_H_ */
