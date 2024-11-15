#include "httpclient.h"
#include <QDebug>
#include <QEventLoop>
#include <QNetworkReply>
#include <QNetworkRequest>

HttpClient::HttpClient(QObject *parent, QString *token, const QString &serverUrl) :
    QObject(parent),
    token(token),
    serverUrl(serverUrl) {
  networkManager = new QNetworkAccessManager(this);
}

HttpClient::~HttpClient() {
  delete networkManager;
}

QByteArray HttpClient::get(const QUrl &url, const QByteArray &data) {
  QNetworkRequest request(url);
  request.setRawHeader("Authorization", token->toUtf8());
  QEventLoop eventLoop;

  QNetworkReply *reply = networkManager->get(request, data);
  connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);

  eventLoop.exec();

  if (reply->error() != QNetworkReply::NoError) {
    qWarning() << "GET request failed:" << reply->errorString();
    reply->deleteLater();
    return QByteArray();
  }

  QByteArray responseData = reply->readAll();
  reply->deleteLater();
  return responseData;
}

QByteArray HttpClient::post(const QUrl &url, const QByteArray &data) {
  QNetworkRequest request(url);
  request.setRawHeader("Authorization", token->toUtf8());
  QEventLoop eventLoop;

  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

  QNetworkReply *reply = networkManager->post(request, data);
  connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);

  eventLoop.exec();

  if (reply->error() != QNetworkReply::NoError) {
    qWarning() << "POST request failed:" << reply->errorString();
    reply->deleteLater();
    return QByteArray();
  }

  QByteArray responseData = reply->readAll();
  reply->deleteLater();
  return responseData;
}

QByteArray HttpClient::del(const QUrl &url) {
  QNetworkRequest request(url);
  request.setRawHeader("Authorization", token->toUtf8());
  QEventLoop eventLoop;

  QNetworkReply *reply = networkManager->deleteResource(request);
  connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);

  eventLoop.exec();

  if (reply->error() != QNetworkReply::NoError) {
    qWarning() << "DELETE request failed:" << reply->errorString();
    reply->deleteLater();
    return QByteArray();
  }

  QByteArray responseData = reply->readAll();
  reply->deleteLater();
  return responseData;
}
