#ifndef OPENPROJECTMENU_H
#define OPENPROJECTMENU_H

#include <QWidget>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QFileDialog>
#include <QDebug>
#include <QTableView>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QSqlTableModel>
//#include "ui_openprojectmenu.h"

//namespace Ui {
//    class openProjectMenu;
//}

class openProjectMenu : public QWidget
{
    Q_OBJECT
public:
    explicit openProjectMenu(QWidget *parent = 0);
//    ~openProjectMenu();
    QTableView *projectsList;
    QPushButton *okButton;
    QPushButton *cancelButton;

public slots:
    //void setDB(QSqlDatabase *database);
    void on_projectsList_clicked(QModelIndex mi);
    void on_okButton_clicked();
private:
//    Ui::openProjectMenu *m_ui;
    QLabel *titleProjects;
    QLabel *titleProjectsInfo;
    QLabel *titleProjectName;
    QLabel *titleMelFile;
    QLabel *titleFort;
    QLabel *titleBMK;
    QLabel *titleDate;
    void connectToDatabase();
    QSqlDatabase *db;
    QString projectSelected;

signals:
    void openProject(QString name);

};


#endif // OPENPROJECTMENU_H
