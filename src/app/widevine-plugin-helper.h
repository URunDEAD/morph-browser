/*
 * Copyright 2026 UBports Foundation
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

#pragma once

#include <QObject>
#include <QProcess>

class WidevinePluginHelper : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isPluginInstalled READ isPluginInstalled NOTIFY isPluginInstalledChanged)
    Q_PROPERTY(bool isInstallationPossible READ isInstallationPossible CONSTANT)
    Q_PROPERTY(QString licenseText READ licenseText NOTIFY licenseTextChanged)
    Q_PROPERTY(bool isWidevineEnabled READ isWidevineEnabled WRITE toggleWidevine NOTIFY isWidevineEnabledChanged)
    Q_PROPERTY(QString widevineUrlSource READ getWidevineUrlSource CONSTANT)

public:
    static const QString widevineLibPath;
    static const QString settingName;

    WidevinePluginHelper(QObject* parent = nullptr);
    ~WidevinePluginHelper() = default;

    Q_INVOKABLE void uninstallPlugin();
    Q_INVOKABLE void abortInstallation();
    Q_INVOKABLE void continueInstallation();
    Q_INVOKABLE void saveFilePath(QString path);
    Q_INVOKABLE void copyToClipboard(QString text);

signals:
    void isPluginInstalledChanged();
    void licenseTextChanged();
    void isWidevineEnabledChanged();

    void downloadFinished();

    void extractingFinished();
    void errorOccurred(QString text, bool isInstalling);
    void installingFinished();
    void uninstallingFinished();

private:
    QString m_licenseText;
    QString m_downloadedFilePath;
    QProcess m_licenseProcess;
    QProcess m_extractProcess;
    QProcess m_cleanProcess;
    QProcess m_installProcess;
    QProcess m_uninstallProcess;

    bool isPluginInstalled() const;
    bool isInstallationPossible() const;
    QString licenseText() const;
    void toggleWidevine(bool enabled);
    bool isWidevineEnabled() const;
    QString getWidevineUrlSource() const;

private slots:
    void extractLib();
    void requestLicenseText();
    void onLicenseProcessFinished(int, QProcess::ExitStatus exitStatus);
    void checkInstallation();
    void checkUninstallation();

    void onExtractingFinished(int, QProcess::ExitStatus exitStatus);
    void onInstallingProcessFinished(int, QProcess::ExitStatus exitStatus);
    void onUninstallProcessFinished(int, QProcess::ExitStatus exitStatus);
};
