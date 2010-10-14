/**
 * KQOAuth - An OAuth authentication library for Qt.
 *
 * Author: Johan Paul (johan.paul@gmail.com)
 *         http://www.johanpaul.com
 *
 *  KQOAuth is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  KQOAuth is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with KQOAuth.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "ut_kqoauth.h"

// Qt includes
#include <QtDebug>
#include <QTest>
#include <QUrl>

// Project includes
#include <QtKOAuth>
#include <kqoauthrequest_p.h>
#include <kqoauthutils.h>

const QString Ut_KQOAuth::twitterExampleBaseString = QString("POST&https%3A%2F%2Fapi.twitter.com%2Foauth%2Frequest_token&oauth_callback%3Dhttp%253A%252F%252Flocalhost%253A3005%252Fthe_dance%252Fprocess_callback%253Fservice_provider_id%253D11%26oauth_consumer_key%3DGDdmIQH6jhtmLUypg82g%26oauth_nonce%3DQP70eNmVz8jvdPevU3oJD2AfF7R7odC2XJcn4XlZJqk%26oauth_signature_method%3DHMAC-SHA1%26oauth_timestamp%3D1272323042%26oauth_version%3D1.0");

void Ut_KQOAuth::init()
{
    r = new KQOAuthRequest;
    d_ptr = r->d_ptr;
}

void Ut_KQOAuth::cleanup()
{
    delete r;
}

void Ut_KQOAuth::constructor()
{
    QVERIFY( r );
    QVERIFY( r->d_ptr );
}

void Ut_KQOAuth::ut_requestBaseString_data() {
    QTest::addColumn<QUrl>("callback");
    QTest::addColumn<QString>("consumerKey");
    QTest::addColumn<QString>("nonce");
    QTest::addColumn<QString>("signatureMethod");
    QTest::addColumn<QString>("timestamp");
    QTest::addColumn<QString>("version");
    QTest::addColumn<QUrl>("endpoint");

    QTest::newRow("happyTwitterCase")
            << QUrl("http://localhost:3005/the_dance/process_callback?service_provider_id=11")
            << QString("GDdmIQH6jhtmLUypg82g")
            << QString("QP70eNmVz8jvdPevU3oJD2AfF7R7odC2XJcn4XlZJqk")
            << QString("HMAC-SHA1")
            << QString("1272323042")
            << QString("1.0")
            << QUrl("https://api.twitter.com/oauth/request_token");
}

void Ut_KQOAuth::ut_requestBaseString() {
    QFETCH(QUrl, callback);
    QFETCH(QString, consumerKey);
    QFETCH(QString, nonce);
    QFETCH(QString, signatureMethod);
    QFETCH(QString, timestamp);
    QFETCH(QString, version);
    QFETCH(QUrl, endpoint);

    r->requestType = KQOAuthRequest::TemporaryCredentials;
    d_ptr->oauthRequestEndpoint = endpoint;
    d_ptr->oauthCallbackUrl = callback;
    d_ptr->oauthConsumerKey = consumerKey;
    d_ptr->oauthNonce_ = nonce;
    d_ptr->oauthSignatureMethod = signatureMethod;
    d_ptr->oauthTimestamp_ = timestamp;
    d_ptr->oauthVersion = version;

    QCOMPARE(d_ptr->validateRequest(), true);
    d_ptr->prepareRequest();

    QByteArray baseString = d_ptr->requestBaseString();

    QCOMPARE(baseString, QByteArray("POST&https%3A%2F%2Fapi.twitter.com%2Foauth%2Frequest_token&oauth_callback%3Dhttp%253A%252F%252Flocalhost%253A3005%252Fthe_dance%252Fprocess_callback%253Fservice_provider_id%253D11%26oauth_consumer_key%3DGDdmIQH6jhtmLUypg82g%26oauth_nonce%3DQP70eNmVz8jvdPevU3oJD2AfF7R7odC2XJcn4XlZJqk%26oauth_signature_method%3DHMAC-SHA1%26oauth_timestamp%3D1272323042%26oauth_version%3D1.0"));

}

void Ut_KQOAuth::ut_hmac_sha1_data() {
    QTest::addColumn<QString>("message");
    QTest::addColumn<QString>("key");
    QTest::addColumn<QString>("result");

    QTest::newRow("twitterExample")
            << QString(twitterExampleBaseString)
            << QString("MCD8BKwGdgPHvAuvgvz4EQpqDAtx89grbuNMRd7Eh98&")
            << QString("8wUi7m5HFQy76nowoCThusfgB+Q=");
}

void Ut_KQOAuth::ut_hmac_sha1() {
    QFETCH(QString, message);
    QFETCH(QString, key);
    QFETCH(QString, result);

    QString hmac_sha1 = KQOAuthUtils::hmac_sta1(message, key);

    QCOMPARE(hmac_sha1, result);
}

QTEST_MAIN(Ut_KQOAuth)
