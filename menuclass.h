#ifndef MENUCLASS_H
#define MENUCLASS_H

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

class newProjectMenu : public QWidget
{
    Q_OBJECT

public:
    explicit newProjectMenu(QWidget *parent = 0);
    QLineEdit *projectName;
    QLineEdit *fileNameField;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QPushButton *selectMelButton;
    QString getText() const;
    QString getIni() const;
    QString getFileName() const;
    QComboBox *iniSelection;
public slots:
    void on_selectMelButton_clicked();
    void on_projectName_textChanged(QString text);
    void on_fileNameCheckingResult(bool isCorrect);


private:
    QLabel *label;
    QLabel *comboBoxHeader;
    QLabel *projectNameChecking;
    QString melName;


signals:
    void buttonPressed();
    void checkProjectName(QString text);
};

#endif // MENUCLASS_H

