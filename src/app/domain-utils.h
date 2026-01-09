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
#include <QtCore/QRegularExpression>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QUrl>

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
#include <libpsl.h>
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
    /*
     * Special case: reject IPv4 addresses which look like domains
     * (e.g. http://127.0.0.1/, http://127.0.1/, http://127.1/).
     * Other IPv4 notations (e.g.  http://2130706433/, http://0x7f000001/)
     * and IPv6 addresses doesn't have to be caught, as they don't look like
     * domains and so libpsl will reject them and return NULL anyway.
     *
     * (Regex adapted from https://stackoverflow.com/a/36760050, CC BY-SA 4.0.)
     */
    QRegularExpression regex("^(((?!25?[6-9])[12]\\d|[1-9])?\\d\\.?\\b){2,4}$");
    if (regex.match(url.host()).hasMatch())
        return QString();

    QByteArray host = QUrl::toAce(url.host());
    QString tld = QString::fromLocal8Bit(
        psl_registrable_domain(psl_builtin(), host.constData()));

    return tld;
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
