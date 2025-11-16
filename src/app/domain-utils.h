/*
 * Copyright 2013 Canonical Ltd.
 *
 * This file is part of morph-browser.
 *
 * morph-browser is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * morph-browser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __DOMAIN_UTILS_H__
#define __DOMAIN_UTILS_H__

// Qt
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QUrl>

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
#include <QtCore/private/qurl_p.h>
#include <QtNetwork/private/qtldurl_p.h>
#endif

namespace DomainUtils {

static const QString TOKEN_LOCAL = "(local)";
static const QString TOKEN_NONE = "(none)";

static QString getTopLevelDomain(const QUrl &url)
{
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    return url.topLevelDomain();
#pragma GCC diagnostic pop
#else
    // QUrl::topLevelDomain() was removed in Qt6.
    // The following is copied from the old "qTopLevelDomain" code in Qt6::Network.
    // It was removed in this commit: https://github.com/qt/qtbase/commit/50b30976837be0969efdccced68cfb584d99981a
    const QString domainLower = url.host().toLower();
    QVector<QStringView> sections = QStringView{domainLower}.split(QLatin1Char('.'), Qt::SkipEmptyParts);
    if (sections.isEmpty())
        return QString();

    QString level, tld;
    for (int j = sections.count() - 1; j >= 0; --j) {
        level.prepend(QLatin1Char('.') + sections.at(j).toString());
        if (qIsEffectiveTLD(QStringView{level}.right(level.size() - 1)))
            tld = level;
    }

    //return qt_ACE_do(tld, ToAceOnly, AllowLeadingDot, {});
    // TODO - QUrl::toAce() uses ForbidLeadingDot, while the old QUrl::topLevelDomain() used AllowLeadingDot. Does this matter?
    return QString(QUrl::toAce(tld));
#endif
}

static QString extractTopLevelDomainName(const QUrl& url)
{
    if (url.isLocalFile()) {
        return TOKEN_LOCAL;
    }
    QString host = url.host();
    if (host.isEmpty()) {
        // XXX: (when) can this happen?
        return TOKEN_NONE;
    }
    QString tld = getTopLevelDomain(url);
    if (tld.isEmpty()) {
        return host;
    }
    host.chop(tld.size());
    QString sld = host.split(".").last();
    return sld + tld;
}

static QString getDomainWithoutSubdomain(const QString& domain)
{
    // e.g. ci.ubports.com (does handle domains like .co.uk correctly)
    // .com
    QString topLevelDomain = getTopLevelDomain(QUrl("//" + domain));

    // invalid top level domain (e.g. local device or IP address)
    if (topLevelDomain.isEmpty())
    {
        QString lastPartOfDomain = domain.mid(domain.lastIndexOf('.'));

        // last part is numeric -> seems to be an IP address
        bool convertToIntOk;
        lastPartOfDomain.mid(1).toInt(&convertToIntOk);

        if (convertToIntOk)
        {
          return domain;
        }

        topLevelDomain = lastPartOfDomain;
    }

    // ci.ubports
    QString urlWithoutTopLevelDomain = domain.mid(0, domain.length() - topLevelDomain.length());
    // ubports (if no . is found, the string stays the same because lastIndexOf is -1)
    QString hostName = urlWithoutTopLevelDomain.mid(urlWithoutTopLevelDomain.lastIndexOf('.') + 1);
    // ubports.com
    return hostName + topLevelDomain;
}

} // namespace DomainUtils

#endif // __DOMAIN_UTILS_H__
