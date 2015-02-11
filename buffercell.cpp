#include "buffercell.h"

void bufferCell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //qDebug()<<"hey!";
    borderPen = QPen(Qt::green);

    polyPen = QPen(Qt::darkYellow);
    polyPen.setWidth(POLY_PEN_WIDTH);

    cellPen = QPen(Qt::yellow);
    cellBrush = QBrush(Qt::darkCyan);

    QRect basic;

    if(m_orientation == "VLB"){
        painter->setPen(borderPen);
        if(m_positionData.size()>0){
            quint32 pos1 = m_positionData[0];
            quint32 pos2 = m_positionData[1];
            quint32 pos3 = m_positionData[2];
            //qDebug()<<pos1<<pos2<<pos3;
            //painter->drawRect(0, 0, 10*SIDE, pos1*SIDE);
            basic = QRect(0,0,10*SIDE, (pos1+1)*SIDE);

            painter->drawRect(9*SIDE, pos2*SIDE, SIDE, SIDE);
            painter->drawRect(9*SIDE, pos3*SIDE, SIDE, SIDE);
            painter->drawRect(9*SIDE, pos1*SIDE, SIDE, SIDE);
            painter->drawRect(9*SIDE, pos3*SIDE, SIDE, (pos1+1)*SIDE);
            //painter->drawRect(10*SIDE,0,11*SIDE,1*SIDE);
        }
    }
    if(m_orientation == "VRB"){
        painter->setPen(borderPen);
        if(m_positionData.size()>0){
            quint32 pos1 = m_positionData[0];
            quint32 pos2 = m_positionData[1];
            quint32 pos3 = m_positionData[2];
            //qDebug()<<pos1<<pos2<<pos3;
            //painter->drawRect(0, 0, 10*SIDE, pos1*SIDE);
            basic = QRect(0,0,10*SIDE, (pos1+1)*SIDE);

            painter->drawRect(0, pos2*SIDE, SIDE, SIDE);
            painter->drawRect(0, pos3*SIDE, SIDE, SIDE);
            painter->drawRect(0, pos1*SIDE, SIDE, SIDE);
            painter->drawRect(0, pos3*SIDE, SIDE, (pos1+1)*SIDE);
            //painter->drawRect(10*SIDE,0,11*SIDE,1*SIDE);
        }
    }
    if(m_orientation == "HDB"){
        painter->setPen(borderPen);
        if(m_positionData.size()>0){
            quint32 pos1 = m_positionData[0];
            quint32 pos2 = m_positionData[1];
            quint32 pos3 = m_positionData[2];
            //qDebug()<<pos1<<pos2<<pos3;
            //painter->drawRect(0, 0, 10*SIDE, pos1*SIDE);
            basic = QRect(0,0,(pos1+1)*SIDE,10*SIDE);

            painter->drawRect(pos2*SIDE,0, SIDE, SIDE);
            painter->drawRect(pos3*SIDE,0, SIDE, SIDE);
            painter->drawRect(pos1*SIDE,0, SIDE, SIDE);
            painter->drawRect(pos3*SIDE,0, (pos1+1)*SIDE, SIDE);
            //painter->drawRect(10*SIDE,0,11*SIDE,1*SIDE);
        }
    }
    if(m_orientation == "HUB"){
        painter->setPen(borderPen);
        if(m_positionData.size()>0){
            quint32 pos1 = m_positionData[0];
            quint32 pos2 = m_positionData[1];
            quint32 pos3 = m_positionData[2];
            //qDebug()<<pos1<<pos2<<pos3;
            //painter->drawRect(0, 0, 10*SIDE, pos1*SIDE);
            basic = QRect(0,0,(pos1+1)*SIDE,10*SIDE);

            painter->drawRect(pos2*SIDE,9*SIDE, SIDE, SIDE);
            painter->drawRect(pos3*SIDE,9*SIDE, SIDE, SIDE);
            painter->drawRect(pos1*SIDE,9*SIDE, SIDE, SIDE);
            painter->drawRect(pos3*SIDE,9*SIDE, (pos1+1)*SIDE, SIDE);
            //painter->drawRect(10*SIDE,0,11*SIDE,1*SIDE);

        }
    }


    //specify a new font.
    QFont newFont = painter->font();
    newFont.setPointSize(basic.height() / 3);
    painter->setFont(newFont);

    painter->drawRect(basic);
    painter->drawText(basic, Qt::AlignCenter, QString::number(m_number));


    bool trueMapIndexes;
    if(!m_map.isEmpty()){
        qDebug()<<m_map;
        if (m_map.size()<5){
            trueMapIndexes = true;
            for (quint8 i = 0; i<m_map.size(); i++){
                if (m_map[i].size()>=9){
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


QRectF bufferCell::boundingRect() const
{
    return QRectF(0,0,4*SIDE,8*SIDE);
}


QString bufferCell::getOrientation() const
{
    return m_orientation;
}

QVector<quint32> bufferCell::getPositionData() const
{
    return m_positionData;
}

void bufferCell::setOrientation(QString arg)
{
    m_orientation = arg;
}

void bufferCell::setPositionData(QVector<quint32> arg)
{
    m_positionData = arg;
}

bool bufferCell::isActive() const
{
    return m_active;
}

bool bufferCell::isSelected() const
{
    return m_selected;
}

quint16 bufferCell::getMacro() const
{
    return m_macro;
}


void bufferCell::setActive(bool arg)
{
    m_active = arg;
}

void bufferCell::setSelected(bool arg)
{
    m_selected = arg;
}

void bufferCell::setMacro(quint16 arg)
{
    m_macro = arg;
}

void bufferCell::initialSet(QString orientation, quint16 number, QVector<quint32> positionData)
{
    m_orientation = orientation;
    m_number = number;
    m_positionData = positionData;
}
