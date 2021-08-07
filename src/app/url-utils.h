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

#ifndef __URL_UTILS_H__
#define __URL_UTILS_H__

#include <QtCore/QObject>
#include <QUrl>

class UrlUtils : public QObject
{
    Q_OBJECT

public:
    explicit UrlUtils(QObject* parent=0);

    Q_INVOKABLE QUrl urlFromString(const QString& urlString) const;
    Q_INVOKABLE QString extractScheme(const QUrl& url) const;
    Q_INVOKABLE QString removeScheme(const QUrl& url) const;
    Q_INVOKABLE bool schemeIs(const QUrl& url, const QString& expected) const;
    Q_INVOKABLE bool hasCustomScheme(const QUrl& url) const;
    Q_INVOKABLE QString extractHost(const QUrl& url) const;
    Q_INVOKABLE bool hostIs(const QUrl& url, const QString& expected) const;
    Q_INVOKABLE QUrl fixUrl(const QString& urlString) const;
    Q_INVOKABLE bool looksLikeAUrl(const QString& urlString) const;

    // special extension urls
    Q_INVOKABLE QString getPdfViewerExtensionUrlPrefix() const;
    Q_INVOKABLE bool isPdfViewerExtensionUrl(const QUrl& url) const;

    // inline functions with url provided as string from QML
    Q_INVOKABLE QString inline extractScheme(const QString& urlString) const
    {
        return extractScheme(QUrl(urlString));
    }
    Q_INVOKABLE QString inline removeScheme(const QString& urlString) const
    {
        return removeScheme(QUrl(urlString));
    }
    Q_INVOKABLE bool inline schemeIs(const QString& urlString, const QString& expected) const
    {
        return schemeIs(QUrl(urlString), expected);
    }
    Q_INVOKABLE bool inline hasCustomScheme(const QString& urlString) const
    {
        return hasCustomScheme(QUrl(urlString));
    }
    Q_INVOKABLE QString inline extractHost(const QString& urlString) const
    {
        return extractHost(QUrl(urlString));
    }
    Q_INVOKABLE bool inline hostIs(const QString& urlString, const QString& expected) const
    {
        return hostIs(QUrl(urlString), expected);
    }

    Q_INVOKABLE bool inline isPdfViewerExtensionUrl(const QString& urlString) const
    {
        return isPdfViewerExtensionUrl(QUrl(urlString));
    }
};

#endif // __URL_UTILS_H__
