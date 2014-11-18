#include "menuclass.h"

newProjectMenu::newProjectMenu(QWidget *parent) :
    QWidget(parent)
{
    this->setWindowTitle("New project...");
    label = new QLabel("Enter new project name: ");
    comboBoxHeader = new QLabel("Choose chip class: ");
    projectNameChecking = new QLabel("Wrong");
    projectName = new QLineEdit;
    projectName->setPlaceholderText("Name...");
    projectName->setFrame(true);
    fileNameField = new QLineEdit("D:\\Qt\\HV154k2_new.mel");
    projectName->setMaxLength(20);
    fileNameField->setMaxLength(255);
    iniSelection = new QComboBox();
    iniSelection->addItem("HRT500.INI");
    iniSelection->addItem("HRT1500.INI");
    iniSelection->addItem("HRT3000.INI");
    okButton = new QPushButton("OK");
    okButton->setEnabled(false);
    cancelButton = new QPushButton("Cancel");
    selectMelButton = new QPushButton("Select MEL file...");

    connect(selectMelButton, SIGNAL(clicked()), this, SLOT(on_selectMelButton_clicked()));
    connect(projectName, SIGNAL(textChanged(QString)), this, SLOT(on_projectName_textChanged(QString)));

    QGridLayout *newProjectMenuLayout = new QGridLayout;
    newProjectMenuLayout->addWidget(label,0,0,1,0);
    //newProjectMenuLayout->setVerticalSpacing(6);
    newProjectMenuLayout->addWidget(projectName,1,0);
    newProjectMenuLayout->addWidget(projectNameChecking,1,1);
    newProjectMenuLayout->addWidget(comboBoxHeader,2,0,1,0);
    //newProjectMenuLayout->setVerticalSpacing(6);
    newProjectMenuLayout->addWidget(iniSelection,3,0,1,0);
    newProjectMenuLayout->addWidget(selectMelButton,4,0,1,0);
    newProjectMenuLayout->addWidget(fileNameField,5,0,1,0);
    newProjectMenuLayout->addWidget(okButton,6,0);
    newProjectMenuLayout->addWidget(cancelButton,6,1);
    newProjectMenuLayout->setMargin(11);
    newProjectMenuLayout->setSpacing(6);
    this->setLayout(newProjectMenuLayout);
    this->resize(300, 500);
}

QString newProjectMenu::getText() const
{
    return projectName->text();
}

QString newProjectMenu::getIni() const
{
    return iniSelection->currentText();
}

void newProjectMenu::on_selectMelButton_clicked(){
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()){
        fileNameField->setText(fileName);
    }
}

void newProjectMenu::on_projectName_textChanged(QString text)
{
    emit checkProjectName(text);
}

void newProjectMenu::on_fileNameCheckingResult(bool isCorrect)
{
    //qDebug()<<"Checking"<<isCorrect;
    if(isCorrect){
        projectNameChecking->setText("Correct");
        okButton->setEnabled(true);
    }else{
        projectNameChecking->setText("Wrong");
        okButton->setEnabled(false);

    }
}

QString newProjectMenu::getFileName() const
{
    //melName =
    return fileNameField->text();
}
