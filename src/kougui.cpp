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

/*
* TODO:
* - Sjekke nick-crash ved pålogging
*/

#include "kougui.h"
#include "NickDTO.h"
#include <iostream>

KouGUI::KouGUI( QWidget *parent ) : QMainWindow( parent )
{
    setupUi( this );
    running = true;

    innst = new InnstillingerGUI( this );
    modell = new ListeModell();
    listView->setModel( modell );
    listView->setAlternatingRowColors( true );
    textEdit->setReadOnly( true );
    statusbar->setVisible( false );

    idletimer = new QTimer( this );
    senderen = new Senderen();

    std::srand( std::time( 0 ) );
    QString kode = QString::number( std::rand() );

    if ( kode.size() < 8 )
        kode.append( QString::number( std::rand() ) );

    if ( kode.size() > 8 )
        kode.resize( 8 );

    modell->settInnData( new NickDTO( innst->hentNick(), kode ) );
    meg = modell->hentUtData( 0 );
    meg->setMeg( true );

    textEdit->setTextColor( innst->hentBeskjedFarge() );
    textEdit->append( hentTid() + "*** Velkommen til KouChat v0.5.2!" );
    senderen->sendMsg( kode + "!EXPOSE#" + meg->getNick() + ":" );

    textEdit->setFocusPolicy( Qt::NoFocus );
    listView->setFocusPolicy( Qt::NoFocus );
    tomButton->setFocusPolicy( Qt::TabFocus );
    awayButton->setFocusPolicy( Qt::TabFocus );
    topicButton->setFocusPolicy( Qt::TabFocus );
    minimerButton->setFocusPolicy( Qt::TabFocus );
    listView->setContextMenuPolicy ( Qt::CustomContextMenu );

    escAction = new QAction( parent );
    escAction->setShortcut( QKeySequence( Qt::Key_Escape ) );
    addAction( escAction );

    listView->setSelectionMode( QAbstractItemView::ExtendedSelection );

    systrayMenu = new QMenu();
    avsluttAction = new QAction( parent );
    avsluttAction->setText( "Avslutt" );
    systrayMenu->addAction( avsluttAction );
    systray = new QSystemTrayIcon( QIcon( "bilder/kou_normal.png" ), parent );
    systray->setContextMenu ( systrayMenu );
    systray->show();

    connect( lineEdit, SIGNAL( returnPressed() ), this, SLOT( sendMelding() ) );
    connect( lineEdit, SIGNAL( textEdited( const QString ) ), this, SLOT( skriveStatus() ) );
    connect( idletimer, SIGNAL( timeout() ), this, SLOT( idleri() ) );
    connect( listView, SIGNAL( customContextMenuRequested( const QPoint &) ), this, SLOT( nickMeny() ) );
    connect( systray, SIGNAL( activated ( QSystemTrayIcon::ActivationReason ) ), this, SLOT( systrayTrykk( QSystemTrayIcon::ActivationReason ) ) );

    connect ( tomButton, SIGNAL( clicked() ), this, SLOT( tomknapp() ) );
    connect ( awayButton, SIGNAL( clicked() ), this, SLOT( awayknapp() ) );
    connect ( minimerButton, SIGNAL( clicked() ), this, SLOT( minimerknapp() ) );
    connect ( topicButton, SIGNAL( clicked() ), this, SLOT( topicknapp() ) );

    connect( avsluttAction, SIGNAL( triggered() ), this, SLOT( avsluttknapp() ) );
    connect( escAction, SIGNAL( triggered() ), this, SLOT( minimerknapp() ) );
    connect ( actionMinimer, SIGNAL( triggered() ), this, SLOT( minimerknapp() ) );
    connect ( actionAvslutt, SIGNAL( triggered() ), this, SLOT( avsluttknapp() ) );
    connect ( actionT_m_chat, SIGNAL( triggered() ), this, SLOT( tomknapp() ) );
    connect ( actionSett_away, SIGNAL( triggered() ), this, SLOT( awayknapp() ) );
    connect ( actionEndre_topic, SIGNAL( triggered() ), this, SLOT( topicknapp() ) );
    connect ( actionInnstillinger, SIGNAL( triggered() ), this, SLOT( innstillingerknapp() ) );
    connect ( actionOm, SIGNAL( triggered() ), this, SLOT( omknapp() ) );

    connect ( innst->getBrukButton(), SIGNAL( clicked() ), this, SLOT( brukknapp() ) );
    connect ( innst->getBeskjedButton(), SIGNAL( clicked() ), this, SLOT( beskjedknapp() ) );
    connect ( innst->getEgenButton(), SIGNAL( clicked() ), this, SLOT( egenknapp() ) );

    idletimer->start( 15000 );
    fiksTittelOgTray();

    QTimer::singleShot( 100, this, SLOT( logon() ) );
    senderen->sendMsg( kode + "!GETTOPIC#" + meg->getNick() + ":" );
    senderen->sendMsg( kode + "!LOGON#" + meg->getNick() + ":" );
    std::cout << "Starta..." << std::endl;

    setWindowIcon( QIcon( "bilder/kou_normal.png" ) );
    show();
    lineEdit->setFocus();
}

KouGUI::~KouGUI()
{
    running = false;

    if ( modell != 0 )
    {
        delete modell;
        modell = 0;
    }

    if ( idletimer != 0 )
    {
        delete idletimer;
        idletimer = 0;
    }

    if ( senderen != 0 )
    {
        delete senderen;
        senderen = 0;
    }

    if ( meg != 0 )
    {
        delete meg;
        meg = 0;
    }

    if ( escAction != 0 )
    {
        delete escAction;
        escAction = 0;
    }

    if ( systray != 0 )
    {
        delete systray;
        systray = 0;
    }
}

void KouGUI::logon()
{
    textEdit->append( hentTid() + "*** Du logget på som " + meg->getNick() + " fra " + meg->getIpadr() + "..." );
}

void KouGUI::fiksTittelOgTray()
{
    QString tittel = "KouChat v0.5.2 - Nick: " + meg->getNick();
    QString tooltip = "KouChat v0.5.2 - " + meg->getNick();

    if ( meg->getAway() )
    {
        tittel += " (Away)";
        tooltip += " (Away)";
    }

    int hhake = topic.indexOf( "]" );
    int vpara = topic.indexOf( "(" );
    int hpara = topic.indexOf( ")" );

    QString nicket = topic.mid( vpara +1, hpara - vpara -1 );
    QString topicen = topic.mid( hhake + 1 );

    tittel += " - Topic: (" + nicket + ") " + topicen;

    setWindowTitle(  tittel );
    systray->setToolTip( tooltip );
}

QString KouGUI::hentTid()
{
    QTime tid = QTime::currentTime();

    QString time = QString::number( tid.hour() );
    QString min = QString::number( tid.minute() );
    QString sek = QString::number( tid.second() );

    if ( time.size() == 1 )
        time.insert( 0, "0" );
    if ( min.size() == 1 )
        min.insert( 0, "0" );
    if ( sek.size() == 1 )
        sek.insert( 0, "0" );

    QString dato = "[" + time + ":" + min + ":" + sek + "] ";

    return dato;
}

void KouGUI::leggTilText( QString mld, QString ipen )
{
    std::cout << hentTid().toStdString() << mld.toStdString() << std::endl;

    QString mldkode = mld;
    mldkode.resize( 8 );

    NickDTO *tmpnicket = modell->hentFraLista( mldkode );

    if ( tmpnicket != 0 )
    {
        tmpnicket->setIpadr( ipen );
        tmpnicket = 0;
    }

    if ( mldkode != meg->getKode() )
    {
        int utropstegn = mld.indexOf( "!" );
        int firkanttast = mld.indexOf( "#" );
        int kolon = mld.indexOf( ":" );
        QString type = mld.mid( utropstegn +1, firkanttast - utropstegn -1 );
        QString mldnick = mld.mid( firkanttast +1, kolon - firkanttast -1 );
        QString melding = mld.mid( kolon +1 );

        if ( type == "MSG" )
        {
            int vhake = melding.indexOf( "[" );
            int hhake = melding.indexOf( "]" );
            int rgb = melding.mid( vhake +1, hhake - vhake -1 ).toInt();
            QString mldmsg = melding.mid( hhake +1 );
            textEdit->setTextColor( rgb );
            textEdit->append( hentTid() + "<" + mldnick + ">: " + mldmsg );

            if ( !isVisible() && meg->getAway() )
                systray->setIcon( QIcon( "bilder/kou_away_activity.png" ) );
            else if ( !isVisible() )
                systray->setIcon( QIcon( "bilder/kou_normal_activity.png" ) );

            raise();
        }

        else if ( type == "LOGON" )
        {
            NickDTO *nynick = new NickDTO( mldnick, mldkode );
            nynick->setSistidle( QDateTime::currentDateTime().toTime_t() );
            modell->settInnData( nynick );
            textEdit->setTextColor( innst->hentBeskjedFarge() );
            textEdit->append( hentTid() + "*** " + mldnick + " logget på fra " + ipen + "..." );
        }

        else if ( type == "EXPOSING" )
        {
            if ( modell->sjekkOmNy( mldkode ) )
            {
                NickDTO *nynick = new NickDTO( mldnick, mldkode );
                nynick->setAwaymsg( melding );

                if ( melding.length() > 0 )
                {
                    nynick->setAway( true );
                    nynick->setAwaymsg( melding );
                }

                nynick->setSistidle( QDateTime::currentDateTime().toTime_t() );
                modell->settInnData( nynick );
            }
        }

        else if ( type == "LOGOFF" )
        {
            NickDTO *logoffnick = modell->hentFraLista( mldkode );
            modell->slettData( logoffnick );
            textEdit->setTextColor( innst->hentBeskjedFarge() );
            textEdit->append( hentTid() + "*** " + mldnick + " logget av..." );
        }

        else if ( type == "AWAY" )
        {
            NickDTO *temp = modell->hentFraLista( mldkode );
            temp->setAway( true );
            temp->setAwaymsg( melding );
            textEdit->setTextColor( innst->hentBeskjedFarge() );
            textEdit->append( hentTid() + "*** " + mldnick + " ble away: " + melding );
            modell->dataEndra();
        }

        else if ( type == "BACK" )
        {
            NickDTO *temp = modell->hentFraLista( mldkode );
            temp->setAway( false );
            temp->setAwaymsg( "" );
            textEdit->setTextColor( innst->hentBeskjedFarge() );
            textEdit->append( hentTid() + "*** " + mldnick + " kom tilbake..." );
            modell->dataEndra();
        }

        else if ( type == "EXPOSE" )
        {
            senderen->sendMsg( meg->getKode() + "!EXPOSING#" + meg->getNick() + ":" + meg->getAwaymsg() );
        }

        else if ( type == "WRITING" )
        {
            NickDTO *temp = modell->hentFraLista( mldkode );
            temp->setSkriver( true );
            modell->dataEndra();
        }

        else if ( type == "STOPPEDWRITING" )
        {
            NickDTO *temp = modell->hentFraLista( mldkode );
            temp->setSkriver( false );
            modell->dataEndra();
        }

        else if ( type == "GETTOPIC" )
        {
            senderen->sendMsg( meg->getKode() + "!TOPIC#" + meg->getNick() + ":" + topic );
        }

        else if ( type == "TOPIC" )
        {
            int vhake = melding.indexOf( "[" );
            int hhake = melding.indexOf( "]" );
            int vpara = melding.indexOf( "(" );
            int hpara = melding.indexOf( ")" );

            if ( hhake != -1 && vhake != -1 )
            {
                if ( topic == 0 )
                    topic = "";

                QString nicket = melding.mid( vpara +1, hpara - vpara -1 );
                QString topicen = "";

                if ( melding.length() > hhake + 1 )
                {
                    topicen = melding.mid( hhake + 1 );

                    if ( topic != melding )
                    {
                        textEdit->setTextColor( innst->hentBeskjedFarge() );
                        textEdit->append( hentTid() + "*** " + nicket + " satt topic til: " + topicen );
                        topic = melding;
                        fiksTittelOgTray();
                    }
                }

                else
                {
                    if ( topic != melding )
                    {
                        textEdit->setTextColor( innst->hentBeskjedFarge() );
                        textEdit->append( hentTid() + "*** " + nicket + " fjernet topic..." );
                        topic = "";
                        fiksTittelOgTray();
                    }
                }
            }
        }

        else if ( type == "NICK" )
        {
            NickDTO *temp = modell->hentFraLista( mldkode );
            QString gmlnick = temp->getNick();
            temp->setNick( mldnick );
            textEdit->setTextColor( innst->hentBeskjedFarge() );
            textEdit->append( hentTid() + "*** " + gmlnick + " bytta nick til " + mldnick );
            modell->sorter();
            modell->dataEndra();
        }

        else if ( type == "IDLE" )
        {
            if ( modell->sjekkOmNy( mldkode ) )
            {
                senderen->sendMsg( meg->getKode() + "!EXPOSE#" + meg->getNick() + ":" );
            }

            else
            {
                NickDTO *temp = modell->hentFraLista( mldkode );
                temp->setSistidle( QDateTime::currentDateTime().toTime_t() );
            }
        }
    }
}

void KouGUI::sendMelding()
{
    QString msg = lineEdit->text();

    if ( msg.trimmed().length() > 0 )
    {
        if ( msg.startsWith( "/" ) )
        {
            textEdit->setTextColor( innst->hentBeskjedFarge() );
            textEdit->append( hentTid() + "*** Ingen kommandoer enda..." );
        }

        else
        {
            textEdit->setTextColor( innst->hentEgenFarge() );
            textEdit->append( hentTid() + "<" + meg->getNick() + ">: " + msg );
            senderen->sendMsg( meg->getKode() + "!MSG#" + meg->getNick() + ":[" + QString::number( (signed) innst->hentEgenFarge().rgb() ) + "]" + msg );
        }

        lineEdit->clear();
    }
}

void KouGUI::closeEvent( QCloseEvent *event )
{
    if ( avslutt() )
        event->accept();
    else
        event->ignore();
}

bool KouGUI::avslutt()
{
    int svar = QMessageBox::question( this, "KouChat - Avslutte?", "Er du sikker på at du vil avslutte?","&Ja", "&Avbryt", QString(), 0, 1 );

    if ( svar == 0 )
    {
        std::cout << "Lukker..." << std::endl;
        running = false;
        senderen->sendMsg( meg->getKode() + "!LOGOFF#" + meg->getNick() + ":" );

        if ( modell != 0 )
        {
            delete modell;
            modell = 0;
        }

        if ( idletimer != 0 )
        {
            delete idletimer;
            idletimer = 0;
        }

        if ( senderen != 0 )
        {
            delete senderen;
            senderen = 0;
        }

        if ( meg != 0 )
        {
            delete meg;
            meg = 0;
        }

        if ( escAction != 0 )
        {
            delete escAction;
            escAction = 0;
        }

        if ( systray != 0 )
        {
            delete systray;
            systray = 0;
        }

        return true;
    }

    else
        return false;
}

void KouGUI::idleri()
{
    senderen->sendMsg( meg->getKode() + "!IDLE#" + meg->getNick() + ":" );

    long tid = QDateTime::currentDateTime().toTime_t();
    QList<NickDTO*> *liste = modell->hentListe();

    for ( int i = 0; i < liste->size(); i++ )
    {
        NickDTO *temp = liste->at( i );

        if ( temp->getMeg() != true && temp->getSistidle() < tid - 120 )
        {
            textEdit->setTextColor( innst->hentBeskjedFarge() );
            textEdit->append( hentTid() + "*** " + temp->getNick() + " timet ut..." );
            modell->slettData( temp );
            i--;
        }
    }
}

void KouGUI::awayknapp()
{
    if ( meg->getAway() )
    {
        int valg = QMessageBox::question( this, "KouChat - Away?", "Tilbake fra '" + meg->getAwaymsg() + "'?","&Ja", "&Avbryt", QString(), 0, 1 );

        if ( valg == 0 )
        {
            systray->setIcon( QIcon( "bilder/kou_normal.png" ) );
            meg->setAway(  false );
            meg->setAwaymsg( "" );
            fiksTittelOgTray();
            lineEdit->setEnabled( true );
            actionInnstillinger->setEnabled( true );
            actionEndre_topic->setEnabled( true );
            topicButton->setEnabled( true );
            textEdit->setTextColor( innst->hentBeskjedFarge() );
            textEdit->append( hentTid() + "*** Du kom tilbake..." );
            senderen->sendMsg( meg->getKode() + "!BACK#" + meg->getNick() + ":" );
            lineEdit->setFocus();
        }
    }

    else
    {
        bool ok;
        QString grunn = QInputDialog::getText( this, "KouChat - Away", "Grunn til away?", QLineEdit::Normal, "", &ok );

        if ( ok && grunn.trimmed().length() > 0 )
        {
            systray->setIcon( QIcon( "bilder/kou_away.png" ) );
            meg->setAway(  true );
            meg->setAwaymsg( grunn );
            fiksTittelOgTray();
            lineEdit->setEnabled( false );
            actionInnstillinger->setEnabled( false );
            actionEndre_topic->setEnabled( false );
            topicButton->setEnabled( false );
            textEdit->setTextColor( innst->hentBeskjedFarge() );
            textEdit->append( hentTid() + "*** Du ble away: " + meg->getAwaymsg() );
            senderen->sendMsg( meg->getKode() + "!AWAY#" + meg->getNick() + ":" + grunn );
        }
    }
}

void KouGUI::topicknapp()
{
    bool ok;
    int hhake = topic.indexOf( "]" );
    QString topicen = topic.mid( hhake + 1 );

    QString nytopic = QInputDialog::getText( this, "KouChat - Topic", "Endre topic?", QLineEdit::Normal, topicen, &ok );

    if ( ok && nytopic != topicen )
    {
        if ( nytopic.trimmed().length() > 0 )
        {
            textEdit->setTextColor( innst->hentBeskjedFarge() );
            textEdit->append( hentTid() + "*** Du satt topic til: " + nytopic );
            topic = "(" + meg->getNick() + ")[0] " + nytopic;
        }

        else
        {
            textEdit->setTextColor( innst->hentBeskjedFarge() );
            textEdit->append( hentTid() + "*** Du fjernet topic..." );
            topic = "";
        }

        senderen->sendMsg( meg->getKode() + "!TOPIC#" + meg->getNick() + ":(" + meg->getNick() + ")[0]" + nytopic );
        fiksTittelOgTray();
    }
}

void KouGUI::tomknapp()
{
    textEdit->clear();
}

void KouGUI::minimerknapp()
{
    setVisible( false );
    //setWindowState( Qt::WindowMinimized );
}

void KouGUI::avsluttknapp()
{
    close();
}

void KouGUI::omknapp()
{
    QMessageBox::information( this, "KouChat - Om", "KouChat v0.5.2\n\nCopyright 2006-2007 Christian Ihle\nkontakt@usikkert.net\nhttp://www.usikkert.net/\n\nKode tilgjengelig under GNU GPLv2.\nSe COPYING for detaljer." );
}

void KouGUI::innstillingerknapp()
{
    int svar = innst->exec();

    if ( svar == 1 )
        innst->lagreConfig();
}

void KouGUI::skriveStatus()
{
    QString tekst = lineEdit->text();

    if ( tekst.length() > 0 )
    {
        if ( meg->getSkriver() == false )
        {
            meg->setSkriver( true );
            senderen->sendMsg( meg->getKode() + "!WRITING#" + meg->getNick() + ":" );
            modell->dataEndra();
        }
    }
    else
    {
        if ( meg->getSkriver() == true )
        {
            meg->setSkriver( false );
            senderen->sendMsg( meg->getKode() + "!STOPPEDWRITING#" + meg->getNick() + ":" );
            modell->dataEndra();
        }
    }
}

void KouGUI::brukknapp()
{
    QString nicket = innst->hentNick().trimmed();
    nicket = nicket.replace( " ", "" );
    innst->settNick( nicket );

    if ( nicket != meg->getNick() )
    {
        QList<NickDTO*> *liste = modell->hentListe();
        bool ok = true;

        if ( nicket.length() == 0 )
        {
            ok = false;
            QMessageBox::warning( this, "KouChat - Nick", "Kan ikke ha tomt nick..." );
        }

        else if ( nicket.length() > 10 )
        {
            ok = false;
            QMessageBox::warning( this, "KouChat - Nick", "Maks 10 bokstaver i nicket..." );
        }

        else
        {
            for ( int i = 0; i < liste->size(); i++ )
            {
                NickDTO *temp = liste->at( i );

                if ( temp->getNick().toLower() == nicket.toLower() && temp->getMeg() == false )
                {
                    ok = false;
                    QMessageBox::warning( this, "KouChat - Nick", "Nicket er opptatt..." );
                    break;
                }
            }
        }

        if ( ok )
        {
            meg->setNick( nicket );
            modell->dataEndra();
            senderen->sendMsg( meg->getKode() + "!NICK#" + meg->getNick() + ":" );
            textEdit->setTextColor( innst->hentBeskjedFarge() );
            textEdit->append( hentTid() + "*** Du bytta nick til " + meg->getNick() );
            fiksTittelOgTray();
        }
    }
}

void KouGUI::egenknapp()
{
    QColor farge = QColorDialog::getColor( innst->hentEgenFarge() );

    if ( farge.isValid() )
        innst->settEgenFarge( farge );
}

void KouGUI::beskjedknapp()
{
    QColor farge = QColorDialog::getColor( innst->hentBeskjedFarge() );

    if ( farge.isValid() )
        innst->settBeskjedFarge( farge );
}

void KouGUI::nickMeny()
{
    QModelIndex ind = listView->selectionModel()->currentIndex();

    if ( ind.row() != -1 )
    {
        NickDTO *temp = modell->hentUtData( ind.row() );
        QString info = "Nick: " + temp->getNick();
        info += "\nIP adresse: " + temp->getIpadr();

        if ( temp->getAway() )
            info += "\nAway melding: " + temp->getAwaymsg();

        QMessageBox::information( this, "KouChat - Info", info );
    }
}

void KouGUI::systrayTrykk( QSystemTrayIcon::ActivationReason grunn )
{
    if ( grunn == QSystemTrayIcon::Trigger )
    {
        setVisible( !isVisible() );

        if ( meg->getAway() )
            systray->setIcon( QIcon( "bilder/kou_away.png" ) );
        else
            systray->setIcon( QIcon( "bilder/kou_normal.png" ) );
    }
}

bool KouGUI::isRunning()
{
    return running;
}
