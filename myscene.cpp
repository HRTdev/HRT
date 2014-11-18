#include "myscene.h"

void MyScene::keyPressEvent(QKeyEvent * keyEvent)
{
    //qDebug()<<"test";
    int var = keyEvent->key();
    switch(getProgramState()){
    case(MyScene::main_menu):
        switch(keyEvent->key()){
        case Qt::Key_T:
            qDebug()<<"Go to trass_menu";
            setProgramState(MyScene::trass_menu);
            break;
        case Qt::Key_D:
            qDebug()<<"Go to deleting menu";
            setProgramState(MyScene::deleting_menu);
            break;
        case Qt::Key_H:
            qDebug()<<"Go to higlighting menu";
            setProgramState(MyScene::highlighting);
            break;
        case Qt::Key_P:
            qDebug()<<"Go to placing menu";
            setProgramState(MyScene::placing);
            emit unitPlacing();
            break;
        case Qt::Key_Q:
            qDebug()<<"Go to test routing";
            setProgramState(MyScene::test_rout);
            emit testRout(Qt::Key_Q);
            break;
        case Qt::Key_N:
            qDebug()<<"Go to node selected state";
            setProgramState(MyScene::nodeSelected);
        default:
            qDebug()<<"Nowhere to going to!";
        }
        break;
    case(MyScene::nodeSelected):
        switch(keyEvent->key()){
        case Qt::Key_N:
            qDebug()<<"Select node";
            emit nodeSelect();
            break;
        default:
            setProgramState(MyScene::main_menu);
            qDebug()<<"Gone to main menu!";
            break;
        }
        break;
    case(MyScene::trass_menu):
        switch(keyEvent->key()){
        case Qt::Key_C:
            qDebug()<<"Go to trass_routing";
            setProgramState(MyScene::trass_routing_hor);
            emit trassRoutingHorBegin(var);
            break;
        default:
            setProgramState(MyScene::main_menu);
            qDebug()<<"Gone to main menu!";
            break;
        }
        break;
    case(MyScene::trass_routing_hor):
        switch(keyEvent->key()){
        case Qt::Key_C:
            setProgramState(MyScene::trass_routing_hor_end);
            emit trassRoutingHorEnd(var);
            break;
        case Qt::Key_L:
            setProgramState(MyScene::trass_routing_hor_end);
            emit trassRoutingHorEnd(var);
            break;
        case Qt::Key_A:
            setProgramState(MyScene::trass_routing_ver);
            emit trassRoutingHorEnd(var);
            emit trassRoutingVerBegin(var);
            break;
        default:
            setProgramState(MyScene::main_menu);
            qDebug()<<"Gone to main menu!";
            break;
        }
        break;
    case(MyScene::trass_routing_ver):
        switch(keyEvent->key()){
        case Qt::Key_C:
            setProgramState(MyScene::trass_routing_ver_end);
            emit trassRoutingVerEnd(var);
            break;
        case Qt::Key_L:
            setProgramState(MyScene::trass_routing_ver_end);
            emit trassRoutingVerEnd(var);
            break;
        case Qt::Key_A:
            setProgramState(MyScene::trass_routing_hor);
            emit trassRoutingVerEnd(var);
            //emit trassRoutingHorBegin(var);
            break;
        default:
            setProgramState(MyScene::main_menu);
            qDebug()<<"Gone to main menu!";
            break;
        }
        break;

    case(MyScene::trass_routing_hor_end):
        qDebug()<<"scene hor trass routing end state";
        setProgramState(MyScene::trass_menu);
        break;
    case(MyScene::trass_routing_ver_end):
        qDebug()<<"scene ver trass routing end state";
        setProgramState(MyScene::trass_menu);
        break;
    case MyScene::deleting_menu:
        switch(keyEvent->key()){
        case Qt::Key_E:
            setProgramState(MyScene::deleting_trace);
            emit deletingTrace(var);
            break;
        default:
            setProgramState(MyScene::main_menu);
            qDebug()<<"Gone to main menu!";
            break;
        }
        break;
    case MyScene::highlighting:
        switch(keyEvent->key()){
        case Qt::Key_L:
            setProgramState(MyScene::highlighting);
            emit highlight(true);
            break;
        case Qt::Key_D:
            setProgramState(MyScene::highlighting);
            emit highlight(false);
            break;
        default:
            setProgramState(MyScene::main_menu);
            qDebug()<<"Gone to main menu!";
            break;
        }
        break;
    default:
        setProgramState(MyScene::main_menu);
        qDebug()<<"Gone to main menu!";
        break;
    }
    emit programStateChanged();
}
