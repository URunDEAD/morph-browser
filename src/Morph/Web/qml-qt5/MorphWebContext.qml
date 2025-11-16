/*
 * Copyright 2014-2015 Canonical Ltd.
 *
 * This file is part of webbrowser-app.
 *
 * webbrowser-app is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * webbrowser-app is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.4
import QtWebEngine 1.5

WebEngineProfile {
    id: oxideContext

    property alias userAgent: oxideContext.httpUserAgent
    property alias dataPath: oxideContext.persistentStoragePath
    property alias maxCacheSizeHint: oxideContext.httpCacheMaximumSize
    property alias incognito: oxideContext.offTheRecord
    property int userAgentId: 0
    property string customUserAgent: ""
    readonly property string defaultUserAgent: __ua.defaultUA
    // remember certificate errors for each domain (for click on symbol in address bar)
    readonly property var certificateErrorsMap: ({})

    offTheRecord: false

    dataPath: dataLocation

    cachePath: cacheLocation
    maxCacheSizeHint: cacheSizeHint

    userAgent: (customUserAgent !== "") ? customUserAgent : defaultUserAgent

    persistentCookiesPolicy: WebEngineProfile.ForcePersistentCookies

    userScripts: [
        WebEngineScript {
            name: "oxide://smartbanners/"
            sourceUrl: Qt.resolvedUrl("smartbanners.js")
            runOnSubframes: true
        },
        WebEngineScript {
            name: "oxide://scrollbar-theme/"
            injectionPoint: WebEngineScript.DocumentReady
            worldId: WebEngineScript.MainWorld
            sourceUrl: Qt.resolvedUrl("scrollbar-theme.js")
            runOnSubframes: true
        }
    ]

    property QtObject __ua: UserAgent02 {
    }

    /*
    devtoolsEnabled: webviewDevtoolsDebugPort !== -1
    devtoolsPort: webviewDevtoolsDebugPort
    devtoolsIp: webviewDevtoolsDebugHost

    hostMappingRules: webviewHostMappingRules
    */
}
