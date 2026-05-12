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

#include "widevine-plugin-helper.h"

#include <QFile>
#include <QSettings>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QDebug>

#include "widevine-plugin-helper-consts.h"

const QString WidevinePluginHelper::settingName = "plugins/widevineIsEnabled";
const QString WidevinePluginHelper::widevineLibPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + relativeWidevineLibPath;

WidevinePluginHelper::WidevinePluginHelper(QObject *parent)
    : QObject(parent)
    , m_licenseProcess(QProcess())
    , m_extractProcess(QProcess())
    , m_cleanProcess(QProcess())
    , m_installProcess(QProcess())
    , m_uninstallProcess(QProcess())
{
    connect(&m_extractProcess, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &WidevinePluginHelper::extractingFinished);
    connect(this, &WidevinePluginHelper::extractingFinished, this, &WidevinePluginHelper::requestLicenseText);
    connect(&m_installProcess, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &WidevinePluginHelper::installingFinished);
    connect(&m_uninstallProcess, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &WidevinePluginHelper::uninstallingFinished);
    connect(&m_licenseProcess, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &WidevinePluginHelper::onLicenseProcessFinished);

    connect(this, &WidevinePluginHelper::downloadFinished, this, &WidevinePluginHelper::extractLib);
    connect(this, &WidevinePluginHelper::installingFinished, this, &WidevinePluginHelper::isPluginInstalledChanged);
    connect(this, &WidevinePluginHelper::installingFinished, this, &WidevinePluginHelper::checkInstallation);
    connect(this, &WidevinePluginHelper::uninstallingFinished, this, &WidevinePluginHelper::isPluginInstalledChanged);
    connect(this, &WidevinePluginHelper::uninstallingFinished, this, &WidevinePluginHelper::checkUninstallation);
    connect(this, &WidevinePluginHelper::errorOccurred, this, [](QString err, bool){ qDebug() << err; });
}

void WidevinePluginHelper::uninstallPlugin()
{
    QStringList args;
    args << QCoreApplication::applicationDirPath() + "/scripts/widevine_tool.sh"
         << "uninstall";
    m_uninstallProcess.start("bash", args);
}

void WidevinePluginHelper::abortInstallation()
{
    QStringList args;
    args << QCoreApplication::applicationDirPath() + "/scripts/widevine_tool.sh"
         << "clean";
    m_cleanProcess.start("bash", args);
}

void WidevinePluginHelper::continueInstallation()
{
    QStringList args;
    args << QCoreApplication::applicationDirPath() + "/scripts/widevine_tool.sh"
         << "install";
    m_installProcess.start("bash", args);
}

void WidevinePluginHelper::saveFilePath(QString path)
{
    m_downloadedFilePath = path;
    emit downloadFinished();
}

bool WidevinePluginHelper::isPluginInstalled() const
{
    return QFile(widevineLibPath).exists();
}

bool WidevinePluginHelper::isInstallationPossible() const
{
    return !widevineSourceFileUrl.isEmpty();
}

QString WidevinePluginHelper::licenseText() const
{
    return m_licenseText;
}

void WidevinePluginHelper::toggleWidevine(bool enabled)
{
    QSettings settings;
    settings.setValue(settingName, enabled);
}

bool WidevinePluginHelper::isWidevineEnabled() const
{
    if (!isPluginInstalled() || !isInstallationPossible()) {
        return false;
    }

    QSettings settings;
    return settings.value(settingName, false).toBool();
}

QString WidevinePluginHelper::getWidevineUrlSource() const
{
    return widevineSourceFileUrl;
}

void WidevinePluginHelper::extractLib()
{
    QStringList extractArgs;
    extractArgs << QCoreApplication::applicationDirPath() + "/scripts/widevine_tool.sh"
                << "extract" << m_downloadedFilePath;
    m_extractProcess.start("bash", extractArgs);
}

void WidevinePluginHelper::requestLicenseText()
{
    QStringList args;
    args << QCoreApplication::applicationDirPath() + "/scripts/widevine_tool.sh"
         << "license";
    m_licenseProcess.start("bash", args);
}

void WidevinePluginHelper::onLicenseProcessFinished(int)
{
    QByteArray output = m_licenseProcess.readAllStandardOutput();
    m_licenseText = QString(output);
    emit licenseTextChanged();
}

void WidevinePluginHelper::checkInstallation()
{
    if (!isPluginInstalled()) {
        emit errorOccurred(tr("Failed to copy files."), true);
    }
}

void WidevinePluginHelper::checkUninstallation()
{
    if (isPluginInstalled()) {
        emit errorOccurred(tr("Failed to delete files."), false);
    }
}
