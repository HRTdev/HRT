#include "gridcell.h"

QRectF gridCell::boundingRect() const
{
    return QRectF(0,0,4*SIDE,m_heigth*SIDE);
}

void gridCell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //qDebug()<<"hey!";
    borderPen = QPen(Qt::green);

    polyPen = QPen(Qt::darkYellow);
    polyPen.setWidth(POLY_PEN_WIDTH);

    cellPen = QPen(Qt::yellow);
    cellBrush = QBrush(Qt::darkCyan);
    //m_heigth = 6;
    painter->setPen(polyPen);
    switch(m_type){
        case 0:     painter->drawLine(QLine(SIDE/2,POLY_PEN_WIDTH/2,SIDE/2,m_heigth*SIDE-SIDE/2-POLY_PEN_WIDTH/2));
                    painter->drawLine(QLine(5*SIDE/2,POLY_PEN_WIDTH/2,5*SIDE/2,m_heigth*SIDE-SIDE/2-POLY_PEN_WIDTH/2));
                    painter->drawLine(QLine(3*SIDE/2,SIDE/2+POLY_PEN_WIDTH/2,3*SIDE/2,m_heigth*SIDE-POLY_PEN_WIDTH/2));
                    painter->drawLine(QLine(7*SIDE/2,SIDE/2+POLY_PEN_WIDTH/2,7*SIDE/2,m_heigth*SIDE-POLY_PEN_WIDTH/2));
                    break;
        case 1:     painter->drawLine(QLine(SIDE/2,POLY_PEN_WIDTH/2,SIDE/2,m_heigth*SIDE-SIDE/2-POLY_PEN_WIDTH/2));
                    painter->drawLine(QLine(5*SIDE/2,POLY_PEN_WIDTH/2,5*SIDE/2,m_heigth*SIDE-SIDE/2-POLY_PEN_WIDTH/2));
                    break;
        case 2:     painter->drawLine(QLine(3*SIDE/2,SIDE/2+POLY_PEN_WIDTH/2,3*SIDE/2,m_heigth*SIDE-POLY_PEN_WIDTH/2));
                    painter->drawLine(QLine(7*SIDE/2,SIDE/2+POLY_PEN_WIDTH/2,7*SIDE/2,m_heigth*SIDE-POLY_PEN_WIDTH/2));
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

void gridCell::setParams(QString cellMelName, QString cellDbName, quint32 cellDbPinsCnt, quint32 cellCnt,
                          quint32 cellPlace, QVector<QString> cellDbPinsInfo, QVector<QString> cellMelPinsInfo,
                          QVector<QString> cellMelPinsType)
{

    m_cellMelName       = cellMelName;    //Имя элемента типа DD
    m_cellDbName        = cellDbName;     //Имя элемента в базе данных F
    m_cellDbPinsCnt     = cellDbPinsCnt;  //Количество пинов из БД
    m_cellPlace         = cellPlace;      //Номер ячейки из всего элемента, который надо отрисовать
    m_cellCnt           = cellCnt;        //Количество ячеек под элемент
    m_cellDbPinsInfo    = cellDbPinsInfo; //Названия пинов типа Х1, Х2...
    m_cellMelPinsInfo   = cellMelPinsInfo;//Имена пинов
    m_cellMelPinsType   = cellMelPinsType;//Тип пина: вход, выход, двунаправленный
    //qDebug()<<"set"<<m_cellMelName<<m_cellDbName<<m_cellDbPinsCnt<<m_cellDbPinsInfo<<m_cellPlace;
}


void gridCell::setRow(quint16 arg)
{
    m_row = arg;
}

void gridCell::setColumn(quint16 arg)
{
    m_column = arg;
}

quint16 gridCell::getRow() const
{
    return m_row;
}

quint16 gridCell::getColumn() const
{
    return m_column;
}

bool gridCell::isActive() const
{
    return m_active;
}

void gridCell::setActive(bool arg)
{
    m_active = arg;
}

bool gridCell::isSelected() const
{
    return m_selected;
}

void gridCell::setSelected(bool arg)
{
    m_selected = arg;
}

void gridCell::setMacro(quint16 arg)
{
    m_macro = arg;
}

void gridCell::setMap(QVector<QVector<quint8> > map)
{
    m_map = map;
}

quint16 gridCell::getMacro() const
{
    return m_macro;
}

quint8 gridCell::getType() const
{
    return m_type;
}

void gridCell::setType(quint8 arg)
{
    m_type = arg;
}

quint8  gridCell::getHeigth() const
{
    return m_heigth;
}

void  gridCell::setHeigth(quint8 arg)
{
    m_heigth = arg;
}
