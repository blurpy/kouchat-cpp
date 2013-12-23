
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

#ifndef KOUGUI_H
#define KOUGUI_H

#include "ui_kougui.h"
#include "senderen.h"
#include "ListeModell.h"
#include "InnstillingerGUI.h"
#include <QtGui>

class KouGUI : public QMainWindow, private Ui::KouGUI
{
    Q_OBJECT

    public:
        KouGUI( QWidget *parent = 0 );
        ~KouGUI();
        void leggTilText( QString msg, QString ip );
        bool isRunning();

    protected:
         void closeEvent( QCloseEvent *e );

    private slots:
        void sendMelding();
        void idleri();
        void awayknapp();
        void topicknapp();
        void tomknapp();
        void minimerknapp();
        void avsluttknapp();
        void omknapp();
        void innstillingerknapp();
        void skriveStatus();
        void brukknapp();
        void egenknapp();
        void beskjedknapp();
        void nickMeny();
        void systrayTrykk( QSystemTrayIcon::ActivationReason grunn );
        void logon();

    private:
        Senderen *senderen;
        QString topic;
        QTimer *idletimer;
        QString hentTid();
        void fiksTittelOgTray();
        ListeModell *modell;
        bool avslutt();
        NickDTO *meg;
        InnstillingerGUI *innst;
        QAction *escAction;
        QMenu *systrayMenu;
        QAction *avsluttAction;
        QSystemTrayIcon *systray;
        bool running;
};

#endif
