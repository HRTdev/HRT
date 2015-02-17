#include "basiccell.h"


void basicCell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //qDebug()<<"hey!";
    borderPen = QPen(Qt::green);

    polyPen = QPen(Qt::darkYellow);
    polyPen.setWidth(POLY_PEN_WIDTH);

    cellPen = QPen(Qt::yellow);
    cellBrush = QBrush(Qt::darkCyan);
    quint32 x = SIDE;
    static const QPointF points[16] = {
        QPointF(0, x),
        QPointF(0, 8*x),
        QPointF(x, 8*x),
        QPointF(x, 7*x),
        QPointF(2*x, 7*x),
        QPointF(2*x, 8*x),
        QPointF(3*x, 8*x),
        QPointF(3*x, 7*x),
        QPointF(4*x, 7*x),
        QPointF(4*x, 0),
        QPointF(3*x, 0),
        QPointF(3*x, x),
        QPointF(2*x, x),
        QPointF(2*x, 0),
        QPointF(x, 0),
        QPointF(x, x),
     };

    QRegExp rxUnit("(F\\d{4})");
    if (rxUnit.indexIn(m_cellDbName) != -1){
        painter->setBrush(cellBrush);
        painter->drawPolygon(points, 16);

        QString str;
        str.append(m_cellMelName);
        str.append("/");
        str.append(m_cellDbName);
        if(m_active){
            //str.append("\nact");
        }
        str.append("\nM: ");
        str.append(m_cellMelMacro);
        painter->setBrush(Qt::NoBrush);
        QRect basic(0,0,4*SIDE,8*SIDE);
        if(!m_active){
            painter->setPen(borderPen);
            //painter->drawRect(basic);
        }else{
            painter->setPen(QPen(Qt::darkCyan));
        }
        painter->drawRect(basic);
        painter->setPen(QPen(Qt::black));
        if(m_cellDbPinsInfo.size()>=9){
            painter->drawText(5*SIDE/4,SIDE, m_cellDbPinsInfo.at(0+9*m_cellPlace));
            painter->drawText(SIDE/4,8*SIDE-SIDE/4, m_cellDbPinsInfo.at(0+9*m_cellPlace));
            painter->drawText(13*SIDE/4,SIDE, m_cellDbPinsInfo.at(3+9*m_cellPlace));
            painter->drawText(9*SIDE/4,8*SIDE-SIDE/4, m_cellDbPinsInfo.at(6+9*m_cellPlace));
        }
        if(m_cellPlace < m_cellCnt-1){
            painter->setPen(QPen(Qt::darkCyan));
            painter->drawLine(4*SIDE,0,4*SIDE, 8*SIDE);
            emit oversizedCell(m_cellMelName, m_cellPlace + 1, QPoint(m_column, m_row), m_cellMelMacro);
        }
        painter->setPen(borderPen);
        //QRect textRect(-m_cellCnt*SIDE,0,4*m_cellCnt*SIDE,8*SIDE);
        if(m_cellPlace == 0){
            painter->drawText(basic, Qt::AlignCenter, str);
        }
    }else{
        painter->setPen(polyPen);
        painter->drawLine(3*SIDE/2,POLY_PEN_WIDTH/2,3*SIDE/2,4*SIDE);//xyxy
        painter->drawLine(3*SIDE/2,4*SIDE,SIDE/2,4*SIDE);
        painter->drawLine(SIDE/2,4*SIDE,SIDE/2,8*SIDE-POLY_PEN_WIDTH/2);
        painter->drawLine(7*SIDE/2,POLY_PEN_WIDTH/2,7*SIDE/2,3*SIDE);
        painter->drawLine(5*SIDE/2,5*SIDE,5*SIDE/2,8*SIDE-POLY_PEN_WIDTH/2);

        painter->setPen(borderPen);
        painter->drawRect(0,0,4*SIDE,8*SIDE);
    }

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

void basicCell::setParams(QString cellMelName, QString cellDbName, quint32 cellDbPinsCnt, quint32 cellCnt,
                          quint32 cellPlace, QVector<QString> cellDbPinsInfo, QVector<QString> cellMelPinsInfo,
                          QVector<QString> cellMelPinsType, QString melMacro)
{

    m_cellMelName       = cellMelName;    //Имя элемента типа DD
    m_cellDbName        = cellDbName;     //Имя элемента в базе данных F
    m_cellDbPinsCnt     = cellDbPinsCnt;  //Количество пинов из БД
    m_cellPlace         = cellPlace;      //Номер ячейки из всего элемента, который надо отрисовать
    m_cellCnt           = cellCnt;        //Количество ячеек под элемент
    m_cellDbPinsInfo    = cellDbPinsInfo; //Названия пинов типа Х1, Х2...
    //qDebug()<<m_cellDbPinsInfo;
    m_cellMelPinsInfo   = cellMelPinsInfo;//Имена пинов
    //qDebug()<<m_cellMelPinsInfo;
    m_cellMelPinsType   = cellMelPinsType;//Тип пина: вход, выход, двунаправленный
    m_cellMelMacro      = melMacro;       //Имя макроэлемента, которому принадлежит элемент
    //qDebug()<<"set"<<m_cellMelName<<m_cellDbName<<m_cellDbPinsCnt<<m_cellDbPinsInfo<<m_cellPlace;

    m_nets.clear();
    if(m_cellMelPinsInfo.size()>=2 && m_cellDbPinsInfo.size()>=9){
        for (quint8 i = 0; i<3;i++){
            quint32 str = m_cellDbPinsInfo[1+3*(i+3*m_cellPlace)].toInt();
            //qDebug()<<str<<1+3*(i+3*m_cellPlace);
            if (str != 65535 && str !=0){
                m_nets.append(m_cellMelPinsInfo[str-1]);
                //qDebug()<<i<<m_cellMelPinsInfo[str-1]<<str-1;
            }else{
                m_nets.append(" ");
            }
        }
        //qDebug()<<m_nets;
    }
}


void basicCell::setRow(quint16 arg)
{
    m_row = arg;
}

void basicCell::setColumn(quint16 arg)
{
    m_column = arg;
}

quint16 basicCell::getRow() const
{
    return m_row;
}

quint16 basicCell::getColumn() const
{
    return m_column;
}

bool basicCell::isActive() const
{
    return m_active;
}

void basicCell::setActive(bool arg)
{
    m_active = arg;
}

bool basicCell::isSelected() const
{
    return m_selected;
}

void basicCell::setSelected(bool arg)
{
    m_selected = arg;
}

void basicCell::setMacro(QString arg)
{
    m_cellMelMacro = arg;
}

void basicCell::setMap(QVector<QVector<quint8> > map)
{
    m_map = map;
}

QString basicCell::getMacro() const
{
    return m_cellMelMacro;
}

QString basicCell::getName() const
{
    return m_cellMelName;
}

QString basicCell::getDBName() const
{
    return m_cellDbName;
}

quint32 basicCell::getSize()
{
    return m_cellCnt;
}

QVector<QString> basicCell::getMelPinsInfo()
{
    return m_cellMelPinsInfo;
}

QVector<QString> basicCell::getMelPinsType()
{
    return m_cellMelPinsType;
}
