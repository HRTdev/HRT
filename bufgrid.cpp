#include "bufgrid.h"

QRectF bufGrid::boundingRect() const
{
    QRectF basic = QRectF(0,0,10*SIDE,10*SIDE);
    if(m_polyPos.size()>0){
        switch(m_type){
        case VLB:{
            basic = QRectF(0,0,10*SIDE, (m_polyPos[0]+1)*SIDE);
            break;
        }
        case VRB:{
            basic = QRectF(0,0,10*SIDE, (m_polyPos[0]+1)*SIDE);
            break;
        }
        case HDB:{
            basic = QRectF(0,0,(m_polyPos[0]+1)*SIDE,10*SIDE);
            break;
        }
        case HUB:{
            basic = QRectF(0,0,(m_polyPos[0]+1)*SIDE,10*SIDE);
            break;
        }
        default:
            break;
        }
    }
    return basic;
}

void bufGrid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //qDebug()<<"hey!";
    borderPen = QPen(Qt::green);

    polyPen = QPen(Qt::darkYellow);
    polyPen.setWidth(POLY_PEN_WIDTH);

    cellPen = QPen(Qt::yellow);
    cellBrush = QBrush(Qt::darkCyan);
    //m_heigth = 6;
    painter->setPen(polyPen);

    quint32 pos1 = m_polyPos[0];
    quint32 pos2 = m_polyPos[1];
    quint32 pos3 = m_polyPos[2];
    switch(m_type){//10 - опытно полученная длина линий поликремния
        case VLB:    painter->drawLine(QLine(2,pos1*SIDE+SIDE/2,10*SIDE-SIDE/2,pos1*SIDE+SIDE/2));
                     painter->drawLine(QLine(2,pos2*SIDE+SIDE/2,10*SIDE-SIDE/2,pos2*SIDE+SIDE/2));
                     painter->drawLine(QLine(2,pos3*SIDE+SIDE/2,10*SIDE-SIDE/2,pos3*SIDE+SIDE/2));
                     break;
        case VRB:    painter->drawLine(QLine(SIDE/2,pos1*SIDE+SIDE/2,10*SIDE-2,pos1*SIDE+SIDE/2));
                     painter->drawLine(QLine(SIDE/2,pos2*SIDE+SIDE/2,10*SIDE-2,pos2*SIDE+SIDE/2));
                     painter->drawLine(QLine(SIDE/2,pos3*SIDE+SIDE/2,10*SIDE-2,pos3*SIDE+SIDE/2));
                     break;
        case HDB:    painter->drawLine(QLine(pos1*SIDE+SIDE/2,SIDE/2,pos1*SIDE+SIDE/2,10*SIDE-2));
                     painter->drawLine(QLine(pos2*SIDE+SIDE/2,SIDE/2,pos2*SIDE+SIDE/2,10*SIDE-2));
                     painter->drawLine(QLine(pos3*SIDE+SIDE/2,SIDE/2,pos3*SIDE+SIDE/2,10*SIDE-2));
                     break;
        case HUB:    painter->drawLine(QLine(pos1*SIDE+SIDE/2,10*SIDE-SIDE/2,pos1*SIDE+SIDE/2,2));
                     painter->drawLine(QLine(pos2*SIDE+SIDE/2,10*SIDE-SIDE/2,pos2*SIDE+SIDE/2,2));
                     painter->drawLine(QLine(pos3*SIDE+SIDE/2,10*SIDE-SIDE/2,pos3*SIDE+SIDE/2,2));
                     break;
        default:    break;

    }

    bool trueMapIndexes;
    if(!m_map.isEmpty()){
        //qDebug()<<m_map;
        if (m_map.size()<5){
            trueMapIndexes = true;
            for (quint8 i = 0; i<m_map.size(); i++){
                if (m_map[i].size()>=m_heigth){
                    trueMapIndexes = false;
                }
            }

            if(trueMapIndexes){
                for(quint8 x = 0; x<m_map.size(); x++){
                    //qDebug()<<"m_mapWHAT?1";
                    for(quint8 y = 0; y<m_map[x].size();y++){
                        //qDebug()<<"m_mapWHAT?";
                        ///#####################
                        mePen = QPen(QColor(233,233,233));
                        mePen.setWidth(ME_PEN_WIDTH);
                        painter->setPen(mePen);
                        if (m_map[x][y] & (1<<LEFT)){
                            painter->drawLine(QLine(SIDE*x + M_OFFSET, SIDE*y + HALF, SIDE*x + HALF, SIDE*y + HALF));
                        }
                        if (m_map[x][y] & (1<<RIGTH)){
                            painter->drawLine(QLine(SIDE*x + HALF, SIDE*y + HALF, SIDE*x + M_FULL,SIDE*y +  HALF));
                        }
                        if (m_map[x][y] & (1<<TOP)){
                            painter->drawLine(QLine(SIDE*x + HALF, SIDE*y + M_OFFSET, SIDE*x + HALF, SIDE*y + HALF));
                        }
                        if (m_map[x][y] & (1<<DOWN)){
                            painter->drawLine(QLine(SIDE*x + HALF, SIDE*y + HALF, SIDE*x + HALF, SIDE*y + M_FULL));
                        }
                        if (m_map[x][y] & (1<<CIRC)){
                            painter->setBrush(SCENE_COLOR);
                            painter->setRenderHint(QPainter::Antialiasing, true);
                            painter->drawRoundedRect(SIDE*x + SIDE/4,  SIDE*y + SIDE/4, SIDE/2, SIDE/2, SIDE/3, SIDE/3);
                            painter->setRenderHint(QPainter::Antialiasing, false);
                        }
                        ///#####################

                    }
                }
            }
        }
    }
}


void bufGrid::setRow(quint16 arg)
{
    m_row = arg;
}

void bufGrid::setColumn(quint16 arg)
{
    m_column = arg;
}

quint16 bufGrid::getRow() const
{
    return m_row;
}

quint16 bufGrid::getColumn() const
{
    return m_column;
}

bool bufGrid::isActive() const
{
    return m_active;
}

void bufGrid::setActive(bool arg)
{
    m_active = arg;
}


void bufGrid::setMap(QVector<QVector<quint8> > map)
{
    m_map = map;
}


quint8 bufGrid::getType() const
{
    return m_type;
}

void bufGrid::setType(quint8 arg)
{
    m_type = arg;
}

void bufGrid::setNets(QVector<QString> arg)
{
    m_nets = arg;
}

quint8  bufGrid::getHeigth() const
{
    return m_heigth;
}

QVector<QString> bufGrid::getNets() const
{
    return m_nets;
}

QVector<QVector<quint8> > bufGrid::getMetal() const
{
    return m_map;
}

void  bufGrid::setHeigth(quint8 arg)
{
    m_heigth = arg;
}

void bufGrid::setPolyPos(QVector<quint32> arg)
{
    m_polyPos = arg;
}
