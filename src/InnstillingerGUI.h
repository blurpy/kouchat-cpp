
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

#ifndef INNSTILLINGERGUI_H
#define INNSTILLINGERGUI_H

#include "ui_innstillinger.h"
#include <QtGui>
#include <QtXml>

class InnstillingerGUI : public QDialog, private Ui::InnstillingerGUI
{
    Q_OBJECT

    public:
        InnstillingerGUI( QWidget *parent = 0 );
        ~InnstillingerGUI();
        QPushButton* getBrukButton();
        QPushButton* getEgenButton();
        QPushButton* getBeskjedButton();
        QColor hentEgenFarge();
        QColor hentBeskjedFarge();
        void settEgenFarge( QColor farge );
        void settBeskjedFarge( QColor farge );
        QString hentNick();
        void settNick( QString nick );
        void lesConfig();
        void lagreConfig();
        QDomDocument hentXML();
        void lagreXML( QDomDocument doc );
};

#endif
