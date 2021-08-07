/*
 * Copyright 2019 Chris Clime
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

#include "url-utils.h"

namespace
{
    const QString PdfViewerChromeExtension = "mhjfbmdgcfjbbpaeojofohoefgiehjai";
}

UrlUtils::UrlUtils(QObject* parent) : QObject(parent)
{
}

QUrl UrlUtils::urlFromString(const QString& urlString) const
{
    return QUrl(urlString);
}

QString UrlUtils::extractScheme(const QUrl& url) const
{
    return url.scheme();
}

QString UrlUtils::removeScheme(const QUrl& url) const
{
    QString urlWithRemovedScheme = url.adjusted(QUrl::RemoveScheme).toString();

    return urlWithRemovedScheme.startsWith("//") ? urlWithRemovedScheme.mid(2) : urlWithRemovedScheme;
}

bool UrlUtils::schemeIs(const QUrl& url, const QString& expected) const
{
    return (extractScheme(url) == expected);
}

bool UrlUtils::hasCustomScheme(const QUrl& url) const
{
    QStringList knownSchemes = { "chrome-extension", "http", "https", "file", "ftp", "data", "mailto" };

    return (! knownSchemes.contains(extractScheme(url)));
}

QString UrlUtils::extractHost(const QUrl& url) const
{
    return url.host();
}

bool UrlUtils::hostIs(const QUrl& url, const QString& expected) const
{
    return (extractHost(url) == expected);
}

QUrl UrlUtils::fixUrl(const QString& urlString) const
{
    return QUrl::fromUserInput(urlString);
}

bool UrlUtils::looksLikeAUrl(const QString& urlString) const
{
    QUrl url = QUrl::fromUserInput(urlString);

    // containing invalid chars, e.g. spaces
    if (! url.isValid())
    {
        return false;
    }

    // local file URL (scheme file)
    if (url.isLocalFile())
    {
        return true;
    }

    // it can still be, that the string is rather a search string, than a URL, e.g. a single word.
    if (!urlString.startsWith(url.scheme()))
    {
        // check if there are dots in the host (IPv4 address or domain)
        if (url.host().contains("."))
        {
            return true;
        }

        // check if it is an IPv6 address
        // e.g. if host() is "::1", the host part is "[::1]"
        QString hostPart = url.adjusted(QUrl::RemoveScheme | QUrl::RemoveUserInfo | QUrl::RemovePort | QUrl::RemovePath | QUrl::RemoveQuery | QUrl::RemoveFragment).toString().mid(2);

        if (hostPart.startsWith("[") && hostPart.contains(":") && hostPart.endsWith("]"))
        {
            return true;
        }

        // looks like a search (without spaces)
        return false;
    }

    // the url is valid (can have a custom scheme)
    return true;
}

QString UrlUtils::getPdfViewerExtensionUrlPrefix() const
{
    return QString("chrome-extension://%1/index.html?").arg(PdfViewerChromeExtension);
}

bool UrlUtils::isPdfViewerExtensionUrl(const QUrl& url) const
{
    return url.toString().startsWith(getPdfViewerExtensionUrlPrefix());
}


