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

#include "NickDTO.h"

NickDTO::NickDTO()
{
    nick = "tom";
    kode = "";
    sistidle = 0;
    awaymsg = "";
    skriver = false;
    away = false;
    ipadr = "";
    meg = false;
}

NickDTO::NickDTO( QString n, QString k ) : nick( n ), kode( k )
{
    sistidle = 0;
    awaymsg = "";
    skriver = false;
    away = false;
    ipadr = "";
    meg = false;
}

NickDTO::NickDTO ( const NickDTO& n )
{
    nick = n.nick;
    kode = n.kode;
    sistidle = n.sistidle;
    awaymsg = n.awaymsg;
    skriver = n.skriver;
    away = n.away;
    ipadr = n.ipadr;
    meg = n.meg;
}

NickDTO::~NickDTO()
{

}

bool NickDTO::operator == ( const NickDTO dto )
{
    if ( kode != dto.kode )
        return false;
    if ( nick != dto.nick )
        return false;
    if ( ipadr != dto.ipadr )
        return false;
    if ( meg != dto.meg )
        return false;
    if ( away != dto.away )
        return false;
    if ( awaymsg != dto.awaymsg )
        return false;
    if ( sistidle != dto.sistidle )
        return false;
    if ( skriver != dto.skriver )
        return false;

    return true;
}

QString NickDTO::getNick()
{
    return nick;
}

void NickDTO::setNick( QString n )
{
    nick = n;
}

QString NickDTO::getKode()
{
    return kode;
}

void NickDTO::setKode( QString k )
{
    kode = k;
}

bool NickDTO::getSkriver()
{
    return skriver;
}

void NickDTO::setSkriver( bool s )
{
    skriver = s;
}

QString NickDTO::getAwaymsg()
{
    return awaymsg;
}

void NickDTO::setAwaymsg( QString a )
{
    awaymsg = a;
}

QString NickDTO::getIpadr()
{
    return ipadr;
}

void NickDTO::setIpadr( QString i )
{
    ipadr = i;
}

long NickDTO::getSistidle()
{
    return sistidle;
}

void NickDTO::setSistidle( long s )
{
    sistidle = s;
}

bool NickDTO::getAway()
{
    return away;
}

void NickDTO::setAway( bool a )
{
    away = a;
}

bool NickDTO::getMeg()
{
    return meg;
}

void NickDTO::setMeg( bool m )
{
    meg = m;
}
