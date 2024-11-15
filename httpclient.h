#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QObject>
#include <QUrl>

class HttpClient : public QObject {
  Q_OBJECT

public:
  explicit HttpClient(QObject *parent = nullptr, QString *token = nullptr, const QString &serverUrl = "");
  ~HttpClient();

  QByteArray get(const QUrl &url, const QByteArray &data = "");

  QByteArray post(const QUrl &url, const QByteArray &data);

  QByteArray del(const QUrl &url);

private:
  QNetworkAccessManager *networkManager;
  QString *token, serverUrl;
};

#endif  // HTTPCLIENT_H
