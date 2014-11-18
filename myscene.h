#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>
#include <QtGui>
#include <QtCore>

#include <QObject>
class MyScene : public  QGraphicsScene
{
    Q_OBJECT
    Q_ENUMS(program_state)
    Q_PROPERTY(program_state programState READ getProgramState WRITE setProgramState NOTIFY programStateChanged)
public:
    MyScene(QObject *parent = 0) : QGraphicsScene(parent){};
    enum program_state{
        main_menu = 0,
        trass_menu = 1,
        trass_routing_hor = 2,
        trass_routing_hor_end = 3,
        trass_routing_ver = 4,
        trass_routing_ver_end = 5,
        deleting_menu =  6,
        deleting_trace = 7,
        highlighting = 8,
        placing = 9,
        test_rout = 10,
        nodeSelected = 11
    };

    program_state getProgramState() const
    {
        return m_programState;
    }

public slots:
    void setProgramState(program_state arg)
    {
        m_programState = arg;
    }

protected:
    //virtual void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void  keyPressEvent (QKeyEvent * keyEvent);
private:

    program_state m_programState;

signals:
    void trassRoutingHorBegin(int beginKey);
    void trassRoutingHorEnd(int endKey);
    void trassRoutingVerBegin(int beginKey);
    void trassRoutingVerEnd(int endKey);
    void testRout(int Key);
    void programStateChanged();
    void deletingTrace(int delKey);
    void highlight(bool arg);
    void unitPlacing();
    void nodeSelect();
};

#endif // MYSCENE_H
