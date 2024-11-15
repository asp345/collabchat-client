#include "logindialog.h"
#include "httpclient.h"
#include <QDebug>
#include <QDialogButtonBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent, State *state_ptr) :
    QDialog(parent), state_ptr(state_ptr) {
  setupUi(this);
}

void LoginDialog::on_buttonBox_accepted() {
  state_ptr->id = lineEdit_4->text();
  state_ptr->workspace = lineEdit->text();
  state_ptr->password = lineEdit_2->text();
  state_ptr->serverurl = lineEdit_3->text();
  HttpClient *http_client = new HttpClient(this, &state_ptr->token, state_ptr->serverurl);
  QJsonObject logininfo;
  logininfo["id"] = state_ptr->id;
  logininfo["password"] = state_ptr->password;
  QJsonDocument doc(logininfo);
  state_ptr->token = http_client->post(state_ptr->serverurl + "/login", doc.toJson());
  if (state_ptr->token == "") {
    QMessageBox msgBox;
    msgBox.setText("Server login failed!");
    msgBox.exec();
    exit(0);
  }
  delete http_client;
  accept();
}
