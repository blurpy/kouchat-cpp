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

#include "mottaker.h"

Mottaker::Mottaker( QWidget *parent ) : QDialog( parent )
{
    struct ip_mreq mreq;

    mreq.imr_multiaddr.s_addr = inet_addr( "224.168.5.200" ); //htonl( 0xE0A805C8 );
    mreq.imr_interface.s_addr = INADDR_ANY; //htonl( INADDR_ANY );

    udpSocket = new QUdpSocket( this );
    udpSocket->bind( QHostAddress( "0.0.0.0" ), 40556, QUdpSocket::ShareAddress );
    setsockopt( udpSocket->socketDescriptor(), IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof( mreq ) );

    connect( udpSocket, SIGNAL( readyRead() ), this, SLOT( processPendingDatagrams() ) );

    enku = new KouGUI();
}

Mottaker::~Mottaker()
{
    delete udpSocket;
    delete enku;
}

void Mottaker::processPendingDatagrams()
{
    while ( udpSocket->hasPendingDatagrams() && enku->isRunning() )
    {
        QHostAddress sender;
        QByteArray datagram;
        datagram.resize( udpSocket->pendingDatagramSize() );
        udpSocket->readDatagram( datagram.data(), datagram.size(), &sender );
        QString msg = tr( datagram.data() );
        QString ip = sender.toString();
        enku->leggTilText( msg, ip );
    }
}
