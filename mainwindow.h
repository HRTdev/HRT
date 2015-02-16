#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QObject>
#include <QString>
#include <QTableWidgetItem>
#include <QtOpenGL>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QTableView>
#include "squareitem.h"
#include "myscene.h"
#include "basiccell.h"
#include "buffercell.h"
#include "gridcell.h"
#include "bufgrid.h"
#include "menuclass.h"
#include "ui_mainwindow.h"
#include "openprojectmenu.h"

//#define TABLE_WIDTH 218//218
//#define TABLE_HEIGTH 214//214
#define MAX_CELL_IN_A_ROW 120
#define MAX_SQUARE_IN_A_ROW 1000

#define ADD 1
#define DEL -1

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    MyScene *scene;
    
private slots:
    void on_radioButton_clicked();
    void on_checkBox_stateChanged();
    void on_horizontalSlider_valueChanged(int value);
    void on_doubleClick(QPointF point);
    void on_hoverEnter(QPointF point);
    void on_hoverMove(QPointF point);
    void on_trassRoutingBegin(int beginKey);
    void on_trassRoutingEnd(int endKey);
    void on_trassRoutingVerBegin(int beginKey);
    void on_trassRoutingVerEnd(int endKey);
    void on_programStateChanged();
    void on_deletingTrace(int delKey);
    void showNewProject(); //This is slot for pressing "New project" in menu "File"
    void openProject();    //
    void openIni();        //
    void openDb();         //
    void openMel();        //
    void on_myScene_unitPlacing();
    void update();
    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);
    void on_tableView_itemDoubleClicked(QModelIndex mi);

    void on_basicCell_oversizedCell(QString cellMelName, quint32 cellPlace, QPoint cellPoint, QString melMacro);

    void reactOnTestRoute(int Key);
    void reactOnNodeSelecting();
    void newProjectNameCommitted();
    void on_projectMenu_checkProjectName(QString text);
    void on_openProjectWidget_openProject(QString project);
    void on_wiringAction();
    void pointsDetection();
private:
    Ui::MainWindow *ui;
    QVector< QVector<SquareItem*> > square;
    QVector< QVector<basicCell*> > cell;
    QVector< QVector<gridCell*> > grid;
    QVector< QVector<bufferCell*> > buffer;
    QVector< QVector<bufGrid*> > bGrid;
    newProjectMenu *projectMenu;
    openProjectMenu *openProjectWidget;
    qreal scaleFactor;
    void drawTable500();
    void drawTable();
    void fillInfoTable();
    void createActions();
    void createMenus();
    void getItemType(QPointF point);
    void adjustTableSize();
    void connectToDatabase();
    QAction *createProject;
    QAction *loadProject;
    QAction *openActIni;
    QAction *openActDb;
    QAction *openActMel;
    QAction *exitAct;
    QAction *wiringAction; //Кнопка разводки.
    QMenu *fileMenu;
    void loadFileIni(const QString &fileName);
    void loadFileDb(const QString &fileName);
    void loadFileMel(const QString &fileName);

    qint32 returnX(QPointF point);
    qint32 returnY(QPointF point);
    QPointF lastPoint;
    QPoint lastCellPos;
    QPoint lastGridPos;

    qint32 max_pos_delta;
    qint32 max_neg_delta;
    qint32 sessionCounter;
    qint32 routingDirection;

    QPointF traceStartPoint;
    QPointF traceEndPoint;

    //Переменные, по которым определяется геометрия планшета
    quint32 traceChannel;
    quint32 unitsInRow;
    quint32 unitsInColumn;
    quint32 horBufCount;
    quint32 verBufCount;
    quint32 horPeriodicBegin;
    quint32 verPeriodicBegin;
    quint32 leftUnitVoidCoord;
    quint32 leftUnitVoidNum;
    quint32 rightUnitVoidCoord;
    quint32 rightUnitVoidNum;
    quint32 unitHeigth;
    quint32 unitWidth;
    quint32 cellHeigth;
    QHash< QString, QVector<quint32> >iniBufPos;
    quint32 TABLE_WIDTH;
    quint32 TABLE_HEIGTH;

   // QVector<quint32> dbUnitName;
    QHash<QString, quint32> dbUnitType;
    QHash<QString, quint32> dbUnitCellCnt;
    QHash<QString, quint32> dbUnitInnerTrs;
    QHash<QString, quint32> dbUnitPinsCnt;
    QHash< QString, QVector<QString> > dbUnitPinsInfo;


    QHash< QString, QHash< QString, QString > > melUnitFName;
    //QHash<QString, bool> melUnitStatus;
    QHash< QString, QHash< QString, QVector<QString> > > melUnitPinName;
    QHash< QString, QHash< QString, QVector<QString> > > melUnitPinType;
    //QHash< QString, QVector<QString> > melUnitPinName;
    //QHash< QString, QVector<QString> > melUnitPinType;
    QString melUnitChosen;//Выбранный юнит с именем из MEL файла
    QString melUnitChosenMacro;//Имя родительского юнита для выбранного юнита

    QString projectName;//Текущее имя проекта
    QString iniFileName;//Текущий ini файл

    qint32 coursorX;//Координаты курсора на графическом планшете
    qint32 coursorY;
    QString unitType;//cell, grid, buffer
    quint32 unitX;//Координаты юнита, находящегося под курсором.
    quint32 unitY;
    quint32 xInUnit;//Координаты курсора внутри юнита.
    quint32 yInUnit;

signals:
   // void itemDoubleClicked(QTableWidgetItem *item);
    void fileNameCheckingResult(bool result);

};

#endif // MAINWINDOW_H
