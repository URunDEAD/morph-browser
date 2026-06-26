/*
 * Copyright 2014-2016 Canonical Ltd.
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

import QtQuick 2.4
import QtQuick.Window 2.2
import Lomiri.SystemInfo
import Lomiri.Components

Window {
    id: window

    color: "#FF111111"

    property bool developerExtrasEnabled: false
    property bool forceFullscreen: false
    property var currentWebview: null
    property bool hasTouchScreen: false

    minimumWidth: units.gu(40)
    minimumHeight: units.gu(20)

    width: units.gu(100)
    height: units.gu(75)

    QtObject {
        id: internal
        property int currentWindowState: Window.Windowed
    }

    ScreenSaver {
        id: screenSaver
        screenSaverEnabled: ! ( window.active && window.currentWebview && (window.currentWebview.isFullScreen || window.currentWebview.recentlyAudible) )
    }

    Connections {
        target: window.currentWebview
        function onIsFullScreenChanged() { window.setFullscreen(window.currentWebview.isFullScreen) }
    }

    function setFullscreen(fullscreen) {
        if (!window.forceFullscreen) {
            if (fullscreen) {
                if (window.visibility != Window.FullScreen) {
                    internal.currentWindowState = window.visibility
                    window.visibility = Window.FullScreen
                }
            } else {
                window.visibility = internal.currentWindowState
            }
        }
    }

    // Show a snapshot of the window, created when suspending
    Image {
        id: windowSnapshot
        anchors.fill: window.contentItem
        visible: false
        z: window.contentItem.z + 1
    }

    Timer {
        id: hideSnapshotTimer
        repeat: false
        running: false
        interval: 750
        onTriggered: {
            windowSnapshot.visible = false;
        }
    }

    // Workaround for QtWebEngine 6.10 with libhybris ANGLE & Ozone buffer sharing
    // The current web page would turn black if the
    // app gets resumed from suspend. Enqueueing a
    // redraw through visibility changes restores the page.
    // Ensure we fill a snapshot image in for flicker-free visuals.
    property bool prevVisible : true;
    property bool grabbing : false;
    property bool grabInterrupted : false;

    // Force redraw once process is  again
    Connections {
        target: Application
        function onStateChanged() {
            if (Application.state === Qt.ApplicatonHidden ||
                Application.state === Qt.ApplicationSuspended)
            {
                orchestrateRedraw(false);
            }
            else if (Application.state === Qt.ApplicationActive)
            {
                orchestrateRedraw(true);
            }
        }
    }

    onActiveChanged: {
        orchestrateRedraw(active);
    }

    onWidthChanged: {
        orchestrateRedraw(true);
    }
    onHeightChanged: {
        orchestrateRedraw(true);
    }

    function orchestrateRedraw(finalize) {
        if (!__onHybris) {
            return;
        }

        if (!finalize)
        {
            if (hideSnapshotTimer.running) {
                hideSnapshotTimer.stop();
            }

            if (!window.grabbing && !windowSnapshot.visible
                    && currentWebview.visible) {
                window.prevVisible = currentWebview.visible;
                window.grabbing = true;
                const success = window.contentItem.grabToImage(function(result) {
                    if (window.grabInterrupted) {
                        window.grabInterrupted = false;
                        window.grabbing = false;
                        return;
                    }
                    windowSnapshot.source = result.url;
                    windowSnapshot.visible = true;
                    window.grabbing = false;
                    currentWebview.visible = false;
                }, Qt.size(window.contentItem.width, window.contentItem.height));
                window.grabbing = success;
            }
        } else {
            hideSnapshotTimer.restart();
            currentWebview.visible = window.prevVisible;
            if (window.grabbing) {
                window.grabInterrupted = true;
            }
        }
    }
}
