#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "httpclient.h"
#include "state.h"
#include <QMainWindow>
#include <QStandardItem>
#include <QStringListModel>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void loadData(QString &token);
  State state;

private slots:
  void on_sendButton_clicked();

  void on_calendarWidget_selectionChanged();

  void on_addDocButton_clicked();

  void onDialogAccepted(QString id, QString title, QString content);

  void on_docsListView_doubleClicked(const QModelIndex &index);

  void onDialogDiscarded(QString id);

  void on_lineEdit_returnPressed();

  void on_viewAllDocsButton_clicked();

private:
  Ui::MainWindow *ui;
  void pollData();
  HttpClient *http_client;
  QTimer timer;
  QStringListModel *chatModel, *onlineModel;
  QStandardItemModel *docsModel;
};
#endif  // MAINWINDOW_H
