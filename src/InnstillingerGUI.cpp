
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

#include "InnstillingerGUI.h"

InnstillingerGUI::InnstillingerGUI( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );
    lesConfig();
}

InnstillingerGUI::~InnstillingerGUI()
{

}

QPushButton* InnstillingerGUI::getBrukButton()
{
    return brukButton;
}

QPushButton* InnstillingerGUI::getEgenButton()
{
    return egenButton;
}

QPushButton* InnstillingerGUI::getBeskjedButton()
{
    return beskjedButton;
}

QColor InnstillingerGUI::hentEgenFarge()
{
    return egenLabel->palette().color( QPalette::WindowText );
}

QColor InnstillingerGUI::hentBeskjedFarge()
{
    return beskjedLabel->palette().color( QPalette::WindowText );
}

void InnstillingerGUI::settEgenFarge( QColor farge )
{
    QPalette palette;
    palette.setColor( QPalette::Active, QPalette::WindowText, farge );
    egenLabel->setPalette( palette );
}

void InnstillingerGUI::settBeskjedFarge( QColor farge )
{
    QPalette palette;
    palette.setColor( QPalette::Active, QPalette::WindowText, farge );
    beskjedLabel->setPalette( palette );
}

QString InnstillingerGUI::hentNick()
{
    return nickEdit->text();
}

void InnstillingerGUI::settNick( QString nick )
{
    nickEdit->setText( nick );
}

QDomDocument InnstillingerGUI::hentXML()
{
    QDomDocument doc( "kouchat" );
    QFile fil( "kouchat.xml" );

    if ( fil.open( QIODevice::ReadOnly ) )
        doc.setContent( &fil );

    fil.close();
    return doc;
}

void InnstillingerGUI::lagreXML( QDomDocument doc )
{
    QFile fil( "kouchat.xml" );

    if ( fil.open( QIODevice::ReadWrite ) )
    {
        QTextStream str( &fil );
        doc.save( str, 4 );
    }

    fil.flush();
    fil.close();
}

void InnstillingerGUI::lesConfig()
{
    QDomDocument doc = hentXML();
    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();

    while ( !n.isNull() )
    {
        QDomElement e = n.toElement();

        if ( !e.isNull() )
        {
            if ( e.tagName() == "nick" )
                nickEdit->setText( e.text() );
            else if ( e.tagName() == "egenfarge" )
                settEgenFarge( e.text().toInt() );
            else if ( e.tagName() == "beskjedfarge" )
                settBeskjedFarge( e.text().toInt() );
        }

        n = n.nextSibling();
    }
}

void InnstillingerGUI::lagreConfig()
{
    QDomDocument doc = hentXML();
    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();

    while ( !n.isNull() )
    {
        QDomElement e = n.toElement();

        if ( !e.isNull() )
        {
            if ( e.tagName() == "nick" )
                e.replaceChild( doc.createTextNode( nickEdit->text() ), e.firstChild() );
            else if ( e.tagName() == "egenfarge" )
                e.replaceChild( doc.createTextNode( QString::number( (signed) hentEgenFarge().rgb() ) ), e.firstChild() );
            else if ( e.tagName() == "beskjedfarge" )
                e.replaceChild( doc.createTextNode( QString::number( (signed) hentBeskjedFarge().rgb() ) ), e.firstChild() );
        }

        n = n.nextSibling();
    }

    lagreXML( doc );
}
