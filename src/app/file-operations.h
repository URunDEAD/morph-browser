/*
 * Copyright 2014-2015 Canonical Ltd.
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

#ifndef __FILE_OPERATIONS_H__
#define __FILE_OPERATIONS_H__

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QThread>
#include <QtGui/QImage>

class QUrl;

class ImageSaverWorker : public QObject
{
    Q_OBJECT

public:
    using QObject::QObject;

    void save(const QImage& image, const QString& path);

Q_SIGNALS:
    void saved(const QString& path, bool success);
};

class FileOperations : public QObject
{
    Q_OBJECT

public:
    explicit FileOperations(QObject* parent=0);
    ~FileOperations();

    Q_INVOKABLE QString baseName(const QString& path) const;
    Q_INVOKABLE QString extension(const QString& path) const;
    Q_INVOKABLE bool exists(const QUrl& path) const;
    Q_INVOKABLE bool remove(const QUrl& file) const;
    Q_INVOKABLE bool removeDirRecursively(const QUrl& dir) const;
    Q_INVOKABLE bool mkpath(const QUrl& path) const;
    Q_INVOKABLE QStringList filesInDirectory(const QUrl& directory,
                                             const QStringList& filters) const;

    // Encodes and writes the image on a background thread, so callers on the
    // GUI thread (e.g. a tab-switch preview capture) are not blocked by
    // PNG encoding + disk I/O. Emits imageSaved() once done.
    Q_INVOKABLE void saveImageAsync(const QImage& image, const QString& path);

Q_SIGNALS:
    void imageSaved(const QString& path, bool success);

private:
    QThread m_workerThread;
    ImageSaverWorker* m_worker;
};

#endif // __FILE_OPERATIONS_H__
