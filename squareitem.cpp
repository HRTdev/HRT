#include "squareitem.h"
#include <QHashIterator>

quint32 SquareItem::getType() const
{
    return m_type;
}


void SquareItem::setType(quint32 arg)
{

    if(!m_isBlocked){
        m_type = arg;
    }
}


void SquareItem::modifyType(quint32 arg, int plus, qint32 session)//Функция работает только с типами "металлизации".
{
    SquareItem::keypair tmp;
    quint32 outType;
    bool circle;
    bool left;
    bool right;
    bool top;
    bool down;
    qint32 vectorNum;
    circle  = false;
    left    = false;
    right   = false;
    top     = false;
    down    = false;
    vectorNum = -1;
    outType = 0;

    if (arg & (1 << leftMe)){
        left = true;
    }
    if (arg & (1 << rightMe)){
        right = true;
    }
    if (arg & (1 << topMe)){
        top = true;
    }
    if (arg & (1 << downMe)){
        down = true;
    }
    if (arg & (1 << circ)){
        circle = true;
    }


    for (int i = 0; i < m_session.size(); i++){
        if (m_session[i].key == session){
            vectorNum = i;
        }
    }



    if (vectorNum != -1){
        if(plus == 1){
            outType = m_session[vectorNum].value |  (circle << circ) |  (top << topMe) |  (down << downMe) |  (left << leftMe) |  (right << rightMe);
        }else{
            outType = m_session[vectorNum].value & ~(circle << circ) & ~(top << topMe) & ~(down << downMe) & ~(left << leftMe) & ~(right << rightMe);
        }
        m_session[vectorNum].value = outType;
    }else{
        if(plus == 1){
            outType = (circle << circ) |  (top << topMe) |  (down << downMe) |  (left << leftMe) |  (right << rightMe);
            tmp.key = session;
            tmp.value = outType;
            m_session.append(tmp);
        }
    }
        for (int i = 0; i < m_session.size(); i++){
            if (m_session[i].value == 0){
                m_session.remove(i);
            }
        }


    for (int j = 0; j < m_session.size(); j++){
        outType |= m_session[j].value;
    }
    setType(outType);
    update();
}


void SquareItem::modifyBaseType(quint32 arg, int plus)
{
    bool leftPolyBool;
    bool rightPolyBool;
    bool topPolyBool;
    bool downPolyBool;
    bool leftBorderBool;
    bool rightBorderBool;
    bool topBorderBool;
    bool downBorderBool;
    bool upperRightPolyBool;
    bool bottomLeftPolyBool;
    quint32 outType;
    leftPolyBool = false;
    rightPolyBool = false;
    topPolyBool = false;
    downPolyBool = false;
    leftBorderBool = false;
    rightBorderBool = false;
    topBorderBool = false;
    downBorderBool = false;
    upperRightPolyBool = false;
    bottomLeftPolyBool = false;
    outType = 0;

    if (arg & (1 << leftPoly)){
        leftPolyBool = true;
    }
    if (arg & (1 << rightPoly)){
        rightPolyBool = true;
    }
    if (arg & (1 << topPoly)){
        topPolyBool = true;
    }
    if (arg & (1 << downPoly)){
        downPolyBool = true;
    }
    if (arg & (1 << leftBorder)){
        leftBorderBool = true;
    }
    if (arg & (1 << rightBorder)){
        rightBorderBool = true;
    }
    if (arg & (1 << topBorder)){
        topBorderBool = true;
    }
    if (arg & (1 << downBorder)){
        downBorderBool = true;
    }
    if (arg & (1 << upperRightPoly)){
        upperRightPolyBool = true;
    }
    if (arg & (1 << bottomLeftPoly)){
        bottomLeftPolyBool = true;
    }



    if(plus == 1){
        outType = getType() |  (leftPolyBool << leftPoly) |  (rightPolyBool << rightPoly) |  (topPolyBool << topPoly) |  (downPolyBool << downPoly) |  (leftBorderBool << leftBorder)
        | (rightBorderBool << rightBorder) |  (topBorderBool << topBorder) |  (downBorderBool << downBorder) |  (upperRightPolyBool << upperRightPoly) |  (bottomLeftPolyBool << bottomLeftPoly);
    }else{
        outType = getType() & ~(leftPolyBool << leftPoly) & ~(rightPolyBool << rightPoly) & ~(topPolyBool << topPoly) & ~(downPolyBool << downPoly) & ~(leftBorderBool << leftBorder)
        & ~(rightBorderBool << rightBorder) & ~(topBorderBool << topBorder) & ~(downBorderBool << downBorder) & ~(upperRightPolyBool << upperRightPoly) & ~(bottomLeftPolyBool << bottomLeftPoly);
    }

    setType(outType);
    update();
}


void SquareItem::setLight(quint32 arg, int plus)
{
    quint32 outType;
    bool lightMe;
    bool lightPoly;
    lightMe   = false;
    lightPoly = false;
    outType = 0;

    if ((getType() | arg) & (1 << hlMe)){
        lightMe = true;
    }
    if ((getType() | arg) & (1 << hlPoly)){
        lightPoly = true;
    }


    for (int j = 0; j < m_session.size(); j++){
        outType |= m_session[j].value;
    }
            if(plus == 1){
                outType |=  (lightMe << hlMe)  |  (lightPoly << hlPoly);
            }else{
                outType &=  ~(lightMe << hlMe) & ~(lightPoly << hlPoly);
            }
    setType(outType);
    update();
}



qint32 SquareItem::getLatestSession() const{
    if(!m_session.isEmpty()){
        return m_session.last().key;
    }
    return 0;
}


qint32 SquareItem::getLatestSessionType() const{
    if(!m_session.isEmpty()){
        return m_session.last().value;
    }
    return 0;
}


QVector<SquareItem::keypair> SquareItem::getSession() const
{
    return m_session;
}


void SquareItem::setSession(QVector<SquareItem::keypair> arg)
{
    m_session = arg;
}





void SquareItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(flag == 0){
        borderPen = QPen(Qt::green);
        //borderPen.setWidth(1);
        flag = 1;
    }
    mePen = QPen(QColor(233, 233, 233));
    mePen.setWidth(ME_PEN_WIDTH);
    polyPen = QPen(Qt::darkYellow);
    polyPen.setWidth(POLY_PEN_WIDTH);

    if (m_type & (1<<hlMe)){
        mePen = QPen(Qt::blue);
        mePen.setWidth(ME_PEN_WIDTH);
    }
    if (m_type & (1<<hlPoly)){
        polyPen = QPen(Qt::white);
        polyPen.setWidth(POLY_PEN_WIDTH);
    }


    if (m_type & (1<<leftPoly | 1<<rightPoly | 1<<topPoly | 1<<downPoly | 1 << upperRightPoly | 1 << bottomLeftPoly)){
        painter->setPen(polyPen);
        if (m_type & (1<<leftPoly)){
            painter->drawLine(QLine(P_OFFSET, HALF, HALF, HALF));
        }
        if (m_type & (1<<rightPoly)){
            painter->drawLine(QLine(HALF, HALF, P_FULL, HALF));
        }
        if (m_type & (1<<topPoly)){
            painter->drawLine(QLine(HALF, P_OFFSET, HALF, HALF));
        }
        if (m_type & (1<<downPoly)){
            painter->drawLine(QLine(HALF, HALF, HALF, P_FULL));
        }
        if (m_type & (1 << upperRightPoly)){
            painter->drawLine(QLine(0, 0, SIDE/2, 0));
        }
        if (m_type & (1 << bottomLeftPoly)){
            painter->drawLine(QLine(SIDE/2, SIDE, SIDE, SIDE));
        }
    }


    if (m_type & (1<<leftBorder | 1<<rightBorder | 1<<topBorder | 1<<downBorder)){
        painter->setPen(borderPen);
        if (m_type & (1<<leftBorder)){
            painter->drawLine(QLine(0, 0, 0, SIDE));
        }
        if (m_type & (1<<rightBorder)){
            painter->drawLine(QLine(SIDE, 0, SIDE, SIDE));
        }
        if (m_type & (1<<topBorder)){
            painter->drawLine(QLine(0, 0, SIDE, 0));
        }
        if (m_type & (1<<downBorder)){
            painter->drawLine(QLine(0, SIDE, SIDE, SIDE));
        }
    }


    if (m_type & (1<<leftMe | 1<<rightMe | 1<<topMe | 1<<downMe | 1<<circ)){
        painter->setPen(mePen);
        if (m_type & (1<<leftMe)){
            painter->drawLine(QLine(M_OFFSET, HALF, HALF, HALF));
        }
        if (m_type & (1<<rightMe)){
            painter->drawLine(QLine(HALF, HALF, M_FULL, HALF));
        }
        if (m_type & (1<<topMe)){
            painter->drawLine(QLine(HALF, M_OFFSET, HALF, HALF));
        }
        if (m_type & (1<<downMe)){
            painter->drawLine(QLine(HALF, HALF, HALF, M_FULL));
        }
        if (m_type & (1<<circ)){
            painter->setBrush(SCENE_COLOR);
            painter->setRenderHint(QPainter::Antialiasing, true);
            painter->drawRoundedRect(SIDE/4, SIDE/4, SIDE/2, SIDE/2, SIDE/3, SIDE/3);
            painter->setRenderHint(QPainter::Antialiasing, false);
        }
    }

}


void SquareItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
     //qDebug()<<"hoverEnterEvent!";
     //setOpacity(0.4);

     QPointF point = event->scenePos();
     emit on_hoverEnter(point);

}


void SquareItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
     //qDebug()<<"hoverLeaveEvent!";
     //setOpacity(1);

}


quint16 SquareItem::getRow() const
{
    return m_row;
}

quint16 SquareItem::getColumn() const
{
    return m_column;
}



bool SquareItem::getIsBlocked() const
{
    return m_isBlocked;
}


quint32 SquareItem::getDefType() const
{
    return m_defType;
}


void SquareItem::setRow(quint16 arg)
{
    m_row = arg;
}


void SquareItem::setColumn(quint16 arg)
{
    m_column = arg;
}



void SquareItem::setIsBlocked(bool arg)
{
    m_isBlocked = arg;
}


void SquareItem::setDefType(quint32 arg)
{
    SquareItem::keypair hash;
    m_defType = arg;
    hash.key = 0;
    hash.value = arg;
    m_session.append(hash);
    //qDebug()<<m_session.last()[0];
}


void SquareItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event){
    QPointF point = event->scenePos();
    emit on_mouseDoubleClick(point);
}


void SquareItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QPointF point = event->scenePos();
    emit on_hoverMove(point);
}













