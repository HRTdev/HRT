#include "openprojectmenu.h"

openProjectMenu::openProjectMenu(QWidget *parent) :
    QWidget(parent)
{
    this->setWindowTitle("Open...");
    projectsList = new QTableView;
    okButton = new QPushButton("OK");
    okButton->setEnabled(false);
    cancelButton = new QPushButton("Cancel");
    //connectToDatabase();

    QSqlTableModel* model = new QSqlTableModel;

    projectsList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    projectsList->setModel( model );
    model->setTable("projects");
    model->select();
    //model->setQuery("SELECT melUnitName, fortUnitName FROM unitNets WHERE isPlaced IS NOT NULL AND isPlaced != 1");
    projectsList->setSortingEnabled(1);
    projectsList->hideColumn(0);
    //projectsList->hideColumn(0);
    projectsList->resizeColumnsToContents();
    projectsList->show();

    QGridLayout *openProjectLayout = new QGridLayout;
    openProjectLayout->addWidget(projectsList,0,0,0,1);
    openProjectLayout->addWidget(okButton,1,0);
    openProjectLayout->addWidget(cancelButton,1,1);
    this->setLayout(openProjectLayout);


    connect(projectsList, SIGNAL(clicked(QModelIndex)), this, SLOT(on_projectsList_clicked(QModelIndex)));
    //connect(cancelButton, SIGNAL(clicked()), this, this->close());
    connect(okButton, SIGNAL(clicked()), this, SLOT(on_okButton_clicked()));
    //connect(okButton, SIGNAL(clicked()),


    this->resize(500, 500);
}


void openProjectMenu::connectToDatabase(){
        //db = new QSqlDatabase;
        //db->addDatabase("QSQLITE", "openProjectConnection");
        //db->setDatabaseName("D:\\Qt5.3\\HRTk0.db3");
        if (!(db->open())) {
            QString resErrorString=db->lastError().databaseText()+"\n"+db->lastError().driverText();
            qDebug()<<"opPrMenuErr"<<resErrorString;
        }
}

void openProjectMenu::on_projectsList_clicked(QModelIndex mi){
    projectSelected = projectsList->model()->data(mi).toString();
    okButton->setEnabled(true);
}

void openProjectMenu::on_okButton_clicked()
{
    emit openProject(projectSelected);
}
