#include "mainwindow.h"
#include "documentdialog.h"
#include "httpclient.h"
#include "logindialog.h"
#include "ui_mainwindow.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QStandardItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    state(),
    ui(new Ui::MainWindow),
    http_client(new HttpClient(this, &state.token, state.serverurl)),
    timer(new QTimer(this)) {
  LoginDialog dialog(this, &state);
  dialog.exec();
  ui->setupUi(this);
  connect(&timer, &QTimer::timeout, this, &MainWindow::pollData);
  state.date = ui->calendarWidget->selectedDate();
  ui->addDocButton->setText("Add Document in " + state.date.toString());
  timer.start(1000);
  docsModel = new QStandardItemModel(this);
  chatModel = new QStringListModel(this);
  onlineModel = new QStringListModel(this);
  ui->docsListView->setModel(docsModel);
  ui->messagesListView->setModel(chatModel);
  ui->onlineUsersListView->setModel(onlineModel);
  connect(ui->lineEdit, &QLineEdit::returnPressed, ui->sendButton, &QPushButton::click);
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::pollData() {
  http_client->post(QUrl(state.serverurl + "/ping"), state.id.toUtf8());

  QJsonDocument docsDoc = QJsonDocument::fromJson(http_client->get(QUrl(state.serverurl + "/docs"), state.date.toString().toUtf8()));
  auto docsArray = docsDoc.object()["list"].toArray();

  delete docsModel;
  docsModel = new QStandardItemModel(this);
  for (const QJsonValue &value : docsArray) {
    QJsonObject docObj = value.toObject();
    QStandardItem *item = new QStandardItem();
    item->setData(docObj["id"].toString());
    item->setText(docObj["title"].toString());
    docsModel->appendRow(item);
  }
  ui->docsListView->setModel(docsModel);

  QJsonDocument chatDocs = QJsonDocument::fromJson(http_client->get(QUrl(state.serverurl + "/chat")));
  QJsonArray chatArray = chatDocs.object()["list"].toArray();
  QStringList chatList;
  for (const auto &jsonValue : chatArray) {
    chatList.append(jsonValue.toString());
  }

  int old_length = chatModel->stringList().length();
  chatModel->setStringList(chatList);
  if (chatList.length() > old_length) {
    ui->messagesListView->scrollToBottom();
  }

  QJsonDocument onlineDocs = QJsonDocument::fromJson(http_client->post(QUrl(state.serverurl + "/online_users"), state.id.toUtf8()));
  QJsonArray onlineArray = onlineDocs.object()["list"].toArray();
  QStringList onlineList;
  for (const auto &jsonValue : onlineArray) {
    auto username = jsonValue.toString();
    if (username == state.id) {
      username += " (You)";
      onlineList.insert(0, username);
    } else {
      onlineList.append(username);
    }
  }
  onlineModel->setStringList(onlineList);
}
void MainWindow::on_sendButton_clicked() {
  auto message = ui->lineEdit->text();
  http_client->post(QUrl(state.serverurl + "/chat"), (state.id + " : " + message).toUtf8());
  ui->lineEdit->clear();
}

void MainWindow::on_calendarWidget_selectionChanged() {
  state.date = ui->calendarWidget->selectedDate();
  pollData();
  ui->addDocButton->setText("Add Document in " + ui->calendarWidget->selectedDate().toString());
}

void MainWindow::on_addDocButton_clicked() {
  DocumentDialog dialog(this, "", "", "");
  connect(&dialog, &DocumentDialog::dialogAccepted, this, &MainWindow::onDialogAccepted);
  dialog.exec();
}

void MainWindow::onDialogAccepted(QString id, QString title, QString content) {
  QJsonDocument sending;
  sending.setObject(QJsonObject{{"date", state.date.toString()}, {"title", title}, {"content", content}});
  http_client->post(QUrl(state.serverurl + "/docs/" + id), sending.toJson());
}

void MainWindow::on_docsListView_doubleClicked(const QModelIndex &index) {
  auto item = docsModel->itemFromIndex(index);
  QJsonDocument doc = QJsonDocument::fromJson(http_client->get(QUrl(state.serverurl + "/docs/" + item->data().toString())));
  QJsonObject obj = doc.object();
  DocumentDialog dialog(this, item->data().toString(), obj["title"].toString(), obj["content"].toString());
  connect(&dialog, &DocumentDialog::dialogAccepted, this, &MainWindow::onDialogAccepted);
  connect(&dialog, &DocumentDialog::dialogDiscarded, this, &MainWindow::onDialogDiscarded);
  dialog.exec();
}

void MainWindow::onDialogDiscarded(QString id) {
  http_client->del(QUrl(state.serverurl + "/docs/" + id));
}

void MainWindow::on_lineEdit_returnPressed() {
  ui->sendButton->click();
}
