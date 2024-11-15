#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include "state.h"
#include "ui_logindialog.h"
#include <QApplication>
#include <QDialog>
class LoginDialog : public QDialog, private Ui::LDialog {
  Q_OBJECT
public:
  LoginDialog(QWidget *parent = nullptr, State *state_ptr = nullptr);
private slots:
  void on_buttonBox_accepted();

private:
  State *state_ptr;
};

#endif  // LOGINDIALOG_H
