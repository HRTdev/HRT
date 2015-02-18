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
    QRect contact1;
    QRect contact2;
    QRect contact3;

    if(m_positionData.size()>0){
        quint32 pos1 = m_positionData[0];
        quint32 pos2 = m_positionData[1];
        quint32 pos3 = m_positionData[2];
        painter->setPen(borderPen);

        switch(m_orientation){
        case VLB:{
            basic = QRect(0,0,10*SIDE, (pos1+1)*SIDE);
            contact1 = QRect(9*SIDE, pos2*SIDE, SIDE, SIDE);
            contact2 = QRect(9*SIDE, pos3*SIDE, SIDE, SIDE);
            contact3 = QRect(9*SIDE, pos1*SIDE, SIDE, SIDE);
            //painter->drawRect(9*SIDE, pos3*SIDE, SIDE, (pos1+1)*SIDE);
            break;
        }
        case VRB:{
            basic = QRect(0,0,10*SIDE, (pos1+1)*SIDE);
            contact1 = QRect(0, pos2*SIDE, SIDE, SIDE);
            contact2 = QRect(0, pos3*SIDE, SIDE, SIDE);
            contact3 = QRect(0, pos1*SIDE, SIDE, SIDE);
            //painter->drawRect(0, pos3*SIDE, SIDE, (pos1+1)*SIDE);
            break;
        }
        case HDB:{
            basic = QRect(0,0,(pos1+1)*SIDE,10*SIDE);
            contact1 = QRect(pos2*SIDE,0, SIDE, SIDE);
            contact2 = QRect(pos3*SIDE,0, SIDE, SIDE);
            contact3 = QRect(pos1*SIDE,0, SIDE, SIDE);
            //painter->drawRect(pos3*SIDE,0, (pos1+1)*SIDE, SIDE);
            break;
        }
        case HUB:{
            basic = QRect(0,0,(pos1+1)*SIDE,10*SIDE);
            contact1 = QRect(pos2*SIDE,9*SIDE, SIDE, SIDE);
            contact2 = QRect(pos3*SIDE,9*SIDE, SIDE, SIDE);
            contact3 = QRect(pos1*SIDE,9*SIDE, SIDE, SIDE);
            //painter->drawRect(pos3*SIDE,9*SIDE, (pos1+1)*SIDE, SIDE);
            break;
        }
        default:
            break;
        }
    }


    QFont newFont = painter->font();
    newFont.setPointSize(16);
    painter->setFont(newFont);

    if (m_active){
        painter->setBrush(cellBrush);
        painter->drawRect(basic);

        painter->setBrush(Qt::NoBrush);
        painter->drawRect(contact1);
        painter->drawRect(contact2);
        painter->drawRect(contact3);

        painter->setPen(borderPen);
        painter->drawText(contact1, Qt::AlignCenter, m_cellDbPinsInfo.at(0));
        painter->drawText(contact2, Qt::AlignCenter, m_cellDbPinsInfo.at(3));
        painter->drawText(contact3, Qt::AlignCenter, m_cellDbPinsInfo.at(6));

        QString str;
        str.append(m_cellMelName);
        str.append("/");
        str.append(m_cellDbName);
        painter->drawText(basic, Qt::AlignCenter, str);
    }else{
        painter->drawRect(basic);
        painter->drawRect(contact1);
        painter->drawRect(contact2);
        painter->drawRect(contact3);
        painter->drawText(basic, Qt::AlignCenter, QString::number(m_number));
    }

}

void bufferCell::setParams(QString cellMelName, QString cellDbName, quint32 cellDbPinsCnt, QVector<QString> cellDbPinsInfo, QVector<QString> cellMelPinsInfo,
                          QVector<QString> cellMelPinsType, QString melMacro)
{

    m_cellMelName       = cellMelName;    //Имя элемента типа DD
    m_cellDbName        = cellDbName;     //Имя элемента в базе данных F
    m_cellDbPinsCnt     = cellDbPinsCnt;  //Количество пинов из БД
    m_cellDbPinsInfo    = cellDbPinsInfo; //Названия пинов типа Х1, Х2...
    //qDebug()<<m_cellDbPinsInfo;
    m_cellMelPinsInfo   = cellMelPinsInfo;//Имена пинов
    //qDebug()<<m_cellMelPinsInfo;
    m_cellMelPinsType   = cellMelPinsType;//Тип пина: вход, выход, двунаправленный
    m_cellMelMacro      = melMacro;       //Имя макроэлемента, которому принадлежит элемент
    qDebug()<<"set"<<m_cellMelName<<m_cellDbName<<m_cellDbPinsCnt<<m_cellDbPinsInfo;

    m_nets.clear();
//    if(m_cellMelPinsInfo.size()>=2 && m_cellDbPinsInfo.size()>=9){
//        for (quint8 i = 0; i<3;i++){
//            quint32 str = m_cellDbPinsInfo[1+3*(i+3)].toInt();
//            //qDebug()<<str<<1+3*(i+3*m_cellPlace);
//            if (str != 65535 && str !=0){
//                m_nets.append(m_cellMelPinsInfo[str-1]);
//                //qDebug()<<i<<m_cellMelPinsInfo[str-1]<<str-1;
//            }else{
//                m_nets.append(" ");
//            }
//        }
//        //qDebug()<<m_nets;
//    }
}


QRectF bufferCell::boundingRect() const
{
    QRectF basic = QRectF(0,0,10*SIDE,10*SIDE);
    if(m_positionData.size()>0){
        switch(m_orientation){
        case VLB:{
            basic = QRectF(0,0,10*SIDE, (m_positionData[0]+1)*SIDE);
            break;
        }
        case VRB:{
            basic = QRectF(0,0,10*SIDE, (m_positionData[0]+1)*SIDE);
            break;
        }
        case HDB:{
            basic = QRectF(0,0,(m_positionData[0]+1)*SIDE,10*SIDE);
            break;
        }
        case HUB:{
            basic = QRectF(0,0,(m_positionData[0]+1)*SIDE,10*SIDE);
            break;
        }
        default:
            break;
        }
    }
    return basic;
}


quint8 bufferCell::getOrientation() const
{
    return m_orientation;
}

quint32 bufferCell::getNumber() const
{
    return m_number;
}

QVector<quint32> bufferCell::getPositionData() const
{
    return m_positionData;
}

void bufferCell::setOrientation(quint8 arg)
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

void bufferCell::setActive(bool arg)
{
    m_active = arg;
}

void bufferCell::setSelected(bool arg)
{
    m_selected = arg;
}

void bufferCell::setMacro(QString arg)
{
    m_cellMelMacro = arg;
}

void bufferCell::initialSet(quint8 orientation, quint16 number, QVector<quint32> positionData)
{
    m_orientation = orientation;
    m_number = number;
    m_positionData = positionData;
}

QString bufferCell::getMacro() const
{
    return m_cellMelMacro;
}

QString bufferCell::getName() const
{
    return m_cellMelName;
}

QString bufferCell::getDBName() const
{
    return m_cellDbName;
}


QVector<QString> bufferCell::getMelPinsInfo() const
{
    return m_cellMelPinsInfo;
}

QVector<QString> bufferCell::getMelPinsType() const
{
    return m_cellMelPinsType;
}
