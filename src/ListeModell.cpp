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

#include "ListeModell.h"

ListeModell::ListeModell( QObject *parent ) : QAbstractListModel( parent )
{
    liste = new QList<NickDTO*>();
}

ListeModell::~ListeModell()
{
    if ( liste != 0 )
    {
        delete liste;
        liste = 0;
    }
}

int ListeModell::rowCount( const QModelIndex &parent ) const
{
    return liste->count();
}

QVariant ListeModell::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    if ( index.row() >= liste->size() )
        return QVariant();

    if ( role == Qt::DisplayRole )
    {
        NickDTO *n = liste->at( index.row() );
        QString nicket = n->getNick();

        if ( n->getSkriver() )
            nicket += " *";

        return QVariant( nicket );
    }

    else if ( role == Qt::FontRole )
    {
        NickDTO *n = liste->at( index.row() );
        QFont fonten;

        if ( n->getMeg() )
            fonten.setBold( true );

        return QVariant( fonten );
    }

    else if ( role == Qt::TextColorRole )
    {
        NickDTO *n = liste->at( index.row() );
        QColor fargen;

        if ( n->getAway() )
            fargen.setNamedColor( "grey" );

        return QVariant( fargen );
    }

    else
        return QVariant();
}

QVariant ListeModell::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( role != Qt::DisplayRole )
        return QVariant();

    if ( orientation == Qt::Horizontal )
        return QString( "Column %1" ).arg( section );
    else
        return QString( "Row %1" ).arg( section );
}

Qt::ItemFlags ListeModell::flags( const QModelIndex &index ) const
{
    if ( !index.isValid() )
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags( index ) | Qt::ItemIsSelectable;
}

QString ListeModell::visInnhold()
{
    QString res = "";

    for (int i = 0; i < liste->size(); ++i)
    {
        NickDTO *n = liste->at( i );
        res += " " + n->getNick();
    }

    return res;
}

NickDTO* ListeModell::hentUtData( int rad )
{
    return liste->at( rad );
    //return liste[rad];
}

void ListeModell::settInnData( NickDTO *dto )
{
    int pos = liste->size();
    beginInsertRows( QModelIndex(), pos, pos );
    liste->append( dto );
    endInsertRows();
    sorter();
    QModelIndex ind = index( pos, 0 );
    emit dataChanged( ind, ind );
}

void ListeModell::slettData( NickDTO *dto )
{
    int pos = liste->indexOf( dto );
    beginRemoveRows( QModelIndex(), pos, pos );
    NickDTO *slett = liste->at( pos );
    liste->removeAt( pos );
    delete slett;
    endRemoveRows();
    QModelIndex ind = index( pos, 0 );
    emit dataChanged( ind, ind );
}

NickDTO* ListeModell::hentFraLista( QString kode )
{
    NickDTO *dto = 0;

    for ( int i = 0; i < liste->size(); i++ )
    {
        NickDTO *temp = liste->at( i );

        if ( temp->getKode() == kode )
        {
            dto = temp;
            break;
        }
    }

    return dto;
}

bool ListeModell::sjekkOmNy( QString kode )
{
    bool ny = true;

    for ( int i = 0; i < liste->size(); i++ )
    {
        NickDTO *temp = liste->at( i );

        if ( temp->getKode() == kode )
        {
            ny = false;
            break;
        }
    }

    return ny;
}

QList<NickDTO*>* ListeModell::hentListe()
{
    return liste;
}

void ListeModell::dataEndra()
{
    QModelIndex start = index( 0, 0 );
    QModelIndex stopp = index( liste->size(), 0 );
    emit dataChanged( start, stopp );
}

void ListeModell::sorter()
{
    qSort( liste->begin(), liste->end(), minLessThan );
}

bool minLessThan( NickDTO *dto1, NickDTO *dto2 )
{
    QString s1 = dto1->getNick();
    QString s2 = dto2->getNick();

    return s1.toLower() < s2.toLower();
}
