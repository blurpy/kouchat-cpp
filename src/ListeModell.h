/***************************************************************************
 *   Copyright 2006-2007 Christian Ihle                                    *
 *   kontakt@usikkert.net                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef LISTEMODELL_H
#define LISTEMODELL_H

#include "NickDTO.h"
#include <QtGui>

class ListeModell : public QAbstractListModel
{
	Q_OBJECT

	public:
		ListeModell( QObject *parent = 0 );
		~ListeModell();
		int rowCount( const QModelIndex &parent = QModelIndex() ) const;
		QVariant data( const QModelIndex &index, int role ) const;
		QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
		Qt::ItemFlags flags( const QModelIndex &index ) const;
		QVariant getData( const QModelIndex &index, int role ) const;
		QString visInnhold();
		NickDTO* hentUtData( int rad );
		void settInnData( NickDTO *dto );
		bool sjekkOmNy( QString kode );
		NickDTO* hentFraLista( QString kode );
		void slettData( NickDTO *dto );
		QList<NickDTO*>* hentListe();
		void dataEndra();
		void sorter();

	private:
		QList<NickDTO*> *liste;
};

bool minLessThan( NickDTO *dto1, NickDTO *dto2 );

#endif
