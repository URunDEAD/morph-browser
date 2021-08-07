/*
 * Copyright 2013 Canonical Ltd.
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

// Qt
#include <QtTest/QtTest>

// local
#include "url-utils.h"

class UrlUtilsTests : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void test_extractHost_data()
    {
        QTest::addColumn<QUrl>("url");
        QTest::addColumn<QString>("host");
        QTest::newRow("row1") << QUrl("http://example.org/") << QString("example.org");
        QTest::newRow("row2") << QUrl("http://www.example.org/") << QString("www.example.org");
        QTest::newRow("row3") << QUrl("http://www.example.org/foo/bar") << QString("www.example.org");
        QTest::newRow("row4") << QUrl("http://example.org:2442/foo") << QString("example.org");
        QTest::newRow("row5") << QUrl("http://user:pwd@example.org/") << QString("example.org");
        QTest::newRow("row6") << QUrl("http://user:pwd@example.org:2442/") << QString("example.org");
        QTest::newRow("row7") << QUrl("ftp://user:pwd@example.org:21/foo/bar") << QString("example.org");
    }

    void test_extractHost()
    {
        QFETCH(QUrl, url);
        QFETCH(QString, host);
        UrlUtils urlUtils;
        QCOMPARE(urlUtils.extractHost(url), host);
    }

    void test_extractScheme_data()
    {
        QTest::addColumn<QUrl>("url");
        QTest::addColumn<QString>("scheme");
        QTest::newRow("row1") << QUrl("http://example.org/") << QString("http");
        QTest::newRow("row2") << QUrl("file://user:pwd@example.org:2442/") << QString("file");
        QTest::newRow("row3") << QUrl("file:///home/foo/bar.txt") << QString("file");
        QTest::newRow("row4") << QUrl("appid://com.ubuntu.terminal/terminal/current-user-version") << QString("appid");
        QTest::newRow("row5") << QUrl("www.example.org") << QString("");
    }

    void test_extractScheme()
    {
        QFETCH(QUrl, url);
        QFETCH(QString, scheme);
        UrlUtils urlUtils;
        QCOMPARE(urlUtils.extractScheme(url), scheme);
    }

    void test_removeScheme_data()
    {
        QTest::addColumn<QUrl>("url");
        QTest::addColumn<QString>("removed");
        QTest::newRow("row1") << QUrl("http://example.org/") << QString("example.org/");
        QTest::newRow("row2") << QUrl("file://user:pwd@example.org:2442/") << QString("user:pwd@example.org:2442/");
        QTest::newRow("row3") << QUrl("file:///home/foo/bar.txt") << QString("/home/foo/bar.txt");
        QTest::newRow("row4") << QUrl("appid://com.ubuntu.terminal/terminal/current-user-version") << QString("com.ubuntu.terminal/terminal/current-user-version");
        QTest::newRow("row5") << QUrl("www.example.org") << QString("www.example.org");
    }

    void test_removeScheme()
    {
        QFETCH(QUrl, url);
        QFETCH(QString, removed);
        UrlUtils urlUtils;
        QCOMPARE(urlUtils.removeScheme(url), removed);
    }

    void test_looksLikeAUrl_data()
    {
        QTest::addColumn<QString>("urlString");
        QTest::addColumn<bool>("looksLike");
        QTest::newRow("row1") << QString("") << false;
        QTest::newRow("row2") << QString("http://example.org/") << true;
        QTest::newRow("row3") << QString("example.org") << true;
        QTest::newRow("row4") << QString("http://www.example.org?q=foo bar") << true;
        QTest::newRow("row5") << QString("about:blank") << true;
        QTest::newRow("row6") << QString("file:///usr/foo/bar") << true;
        QTest::newRow("row7") << QString("hello://my/name/is/") << true;
        QTest::newRow("row8") << QString("192.168.1.0") << true;
        QTest::newRow("row9") << QString("foo") << false;
        QTest::newRow("row10") << QString("foo bar") << false;
        QTest::newRow("row11") << QString("www.example.org search") << false;
    }

    void test_looksLikeAUrl()
    {
        QFETCH(QString, urlString);
        QFETCH(bool, looksLike);
        UrlUtils urlUtils;
        QCOMPARE(urlUtils.looksLikeAUrl(urlString), looksLike);
    }

    void test_fixUrl_data()
    {
        QTest::addColumn<QString>("urlString");
        QTest::addColumn<QUrl>("fixedUrl");
        QTest::newRow("row1") << QString("About:BLANK") << QUrl("about:BLANK");
        QTest::newRow("row2") << QString("/usr/bin/") << QUrl("file:///usr/bin/");
        QTest::newRow("row3") << QString("example.org") << QUrl("http://example.org");
    }

    void test_fixUrl()
    {
       QFETCH(QString, urlString);
       QFETCH(QUrl, fixedUrl);
       UrlUtils urlUtils;
       QCOMPARE(urlUtils.fixUrl(urlString), fixedUrl);
    }

};

QTEST_MAIN(UrlUtilsTests)
#include "tst_UrlUtilsTests.moc"
