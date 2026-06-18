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
#include <QGuiApplication>
#include <QStandardPaths>
#include <QDebug>
#include <QClipboard>
#include "config.h"

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
    connect(&m_extractProcess, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &WidevinePluginHelper::onExtractingFinished);
    connect(this, &WidevinePluginHelper::extractingFinished, this, &WidevinePluginHelper::requestLicenseText);
    connect(&m_installProcess, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &WidevinePluginHelper::onInstallingProcessFinished);
    connect(&m_uninstallProcess, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &WidevinePluginHelper::onUninstallProcessFinished);
    connect(&m_licenseProcess, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &WidevinePluginHelper::onLicenseProcessFinished);

    connect(this, &WidevinePluginHelper::downloadFinished, this, &WidevinePluginHelper::extractLib);
    connect(this, &WidevinePluginHelper::installingFinished, this, &WidevinePluginHelper::isPluginInstalledChanged);
    connect(this, &WidevinePluginHelper::installingFinished, this, &WidevinePluginHelper::checkInstallation);
    connect(this, &WidevinePluginHelper::uninstallingFinished, this, &WidevinePluginHelper::isPluginInstalledChanged);
    connect(this, &WidevinePluginHelper::uninstallingFinished, this, &WidevinePluginHelper::checkUninstallation);
    connect(this, &WidevinePluginHelper::errorOccurred, this, [](QString err, bool){ qCritical() << "Widevine error occurred: " << err; });

    connect(&m_licenseProcess, &QProcess::errorOccurred, this, &WidevinePluginHelper::checkError);
    connect(&m_extractProcess, &QProcess::errorOccurred, this, &WidevinePluginHelper::checkError);
    connect(&m_cleanProcess, &QProcess::errorOccurred, this, &WidevinePluginHelper::checkError);
    connect(&m_installProcess, &QProcess::errorOccurred, this, &WidevinePluginHelper::checkError);
    connect(&m_uninstallProcess, &QProcess::errorOccurred, this, &WidevinePluginHelper::checkError);
}

void WidevinePluginHelper::uninstallPlugin()
{
    QStringList args;
    args << UbuntuBrowserDirectory() + "/webbrowser/scripts/widevine_tool.sh"
         << "uninstall";
    m_uninstallProcess.start("bash", args);
}

void WidevinePluginHelper::abortInstallation()
{
    QStringList args;
    args << UbuntuBrowserDirectory() + "/webbrowser/scripts/widevine_tool.sh"
         << "clean";
    m_cleanProcess.start("bash", args);
}

void WidevinePluginHelper::continueInstallation()
{
    QStringList args;
    args << UbuntuBrowserDirectory() + "/webbrowser/scripts/widevine_tool.sh"
         << "install";
    m_installProcess.start("bash", args);
}

void WidevinePluginHelper::saveFilePath(QString path)
{
    m_downloadedFilePath = path;
    emit downloadFinished();
}

void WidevinePluginHelper::copyToClipboard(QString text)
{
    QGuiApplication::clipboard()->setText(text, QClipboard::Clipboard);
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
    extractArgs << UbuntuBrowserDirectory() + "/webbrowser/scripts/widevine_tool.sh"
                << "extract" << m_downloadedFilePath;
    m_extractProcess.start("bash", extractArgs);
}

void WidevinePluginHelper::requestLicenseText()
{
    QStringList args;
    args << UbuntuBrowserDirectory() + "/webbrowser/scripts/widevine_tool.sh"
         << "license";
    m_licenseProcess.start("bash", args);
}

void WidevinePluginHelper::onLicenseProcessFinished(int, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::NormalExit) {
        QByteArray output = m_licenseProcess.readAllStandardOutput();
        m_licenseText = QString(output);
        emit licenseTextChanged();
    } else {
        emit errorOccurred(tr("Failed to get license. Please check the application logs for details"), true);
    }
}

void WidevinePluginHelper::checkInstallation()
{
    if (!isPluginInstalled()) {
        qCritical() << "Failed to copy files";
        emit errorOccurred(tr("Failed to copy files"), true);
    }
}

void WidevinePluginHelper::checkUninstallation()
{
    if (isPluginInstalled()) {
        qCritical() << "Failed to delete files";
        emit errorOccurred(tr("Failed to delete files"), false);
    }
}

void WidevinePluginHelper::onExtractingFinished(int, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::NormalExit) {
        emit extractingFinished();
    } else {
        emit errorOccurred(tr("Failed to extract widevine. Please check the application logs for details"), true);
    }
}

void WidevinePluginHelper::onInstallingProcessFinished(int, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::NormalExit) {
        emit installingFinished();
    } else {
        emit errorOccurred(tr("Failed to install widevine. Please check the application logs for details"), true);
    }
}

void WidevinePluginHelper::onUninstallProcessFinished(int, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::NormalExit) {
        emit uninstallingFinished();
    } else {
        emit errorOccurred(tr("Failed to uninstall widevine. Please check the application logs for details"), false);
    }
}

void WidevinePluginHelper::checkError()
{
    QProcess *target = qobject_cast<QProcess *>(sender());
    if (!target) {
        return;
    }
    if (target->error() != QProcess::UnknownError) {
        qCritical() << "running" << target->program() << target->arguments() << "failed:" << target->error();
    }
    if (target->exitStatus() == QProcess::CrashExit) {
        qCritical() << target->program() << target->arguments() << "crashed";
    }
    if (target->exitCode() != 0) {
        qCritical() << target->program() << target->arguments() << "failed, exit code:" << target->exitCode();
    }
}

