#ifndef SQUAREITEM_H
#define SQUAREITEM_H

#include <QGraphicsItem>
#include <QtCore>
#include <QtGui>
#include <QObject>
#include <QHash>
#include <QVector>
#include <QGraphicsSceneMouseEvent>

#define SIDE 20
#define ME_PEN_WIDTH 2
#define POLY_PEN_WIDTH 4
#define SCENE_COLOR Qt::black

#define M_OFFSET ME_PEN_WIDTH / 2
#define M_HALF SIDE / 2 - M_OFFSET
#define M_FULL SIDE - M_OFFSET

#define P_OFFSET POLY_PEN_WIDTH / 2
#define P_HALF SIDE / 2 - P_OFFSET
#define P_FULL SIDE - P_OFFSET

#define HALF SIDE / 2


/* ќпределим названи€ дл€ позиций битов их комбинаций в типе €чейки */
#define circ        0
#define leftMe      1
#define rightMe     2
#define topMe       3
#define downMe      4
#define leftPoly    5
#define rightPoly   6
#define topPoly     7
#define downPoly    8
#define leftBorder  9
#define rightBorder 10
#define topBorder   11
#define downBorder  12
#define upperRightPoly 13
#define bottomLeftPoly 14
#define hlMe        15
#define hlPoly      16

#define horizontalMe            (1 << leftMe)  | (1 << rightMe)
#define verticalMe              (1 << topMe)   | (1 << downMe)
#define leftMeCirc              (1 << leftMe)  | (1 << circ)
#define rightMeCirc             (1 << rightMe) | (1 << circ)
#define verticalPoly            (1 << topPoly) | (1 << downPoly)
#define verticalPolyDownHalf    (1 << downPoly)
#define verticalPolyTopHalf     (1 << topPoly)
#define LeftTopBorder           (1 << leftBorder) | (1 << topBorder)
#define downMeCirc              (1 << downMe)  | (1 << circ)
#define topMeCirc               (1 << topMe)   | (1 << circ)
#define vPolyTopBorder          (1 << topPoly) | (1 << downPoly) | (1 << topBorder)
#define topBorder_t             (1 << topBorder)
#define vPolyRightTopBorder     (1 << topPoly) | (1 << downPoly) | (1 << rightBorder) | (1 << topBorder)
#define LeftBorder              (1 << leftBorder)
#define empty                   0
#define vPolyRightBorder        (1 << topPoly) | (1 << downPoly) | (1 << rightBorder)
#define vLeftBottomPoly         (1 << topPoly) | (1 << downPoly) | (1 << bottomLeftPoly)
#define vRightTopPolyLeftBorder (1 << topPoly) | (1 << downPoly) | (1 << upperRightPoly) | (1 << leftBorder)
#define vPolyLeftBorder         (1 << topPoly) | (1 << downPoly) | (1 << leftBorder)
#define vPolyLeftBottomBorder   (1 << topPoly) | (1 << downPoly) | (1 << leftBorder) | (1 << downBorder)
#define vPolyBottomBorder       (1 << topPoly) | (1 << downPoly) | (1 << downBorder)
#define rightDownBorder         (1 << rightBorder) | (1 << downBorder)

/* ќпределим названи€ дл€ позиций битов в ICG - interconnection grid'е */
#define throughCon              0
#define leftCon                 1
#define rightCon                2
#define topCon                  3
#define downCon                 4
#define upperRightCornerCon     5
#define bottomRightCornerCon    6
#define upperLeftCornerCon      7
#define bottomLeftCornerCon     8

class SquareItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_PROPERTY(quint32 type     READ getType    WRITE setType)
    Q_PROPERTY(quint32 defType  READ getDefType WRITE setDefType)
    Q_PROPERTY(quint16 row       READ getRow     WRITE setRow)
    Q_PROPERTY(quint16 column    READ getColumn  WRITE setColumn)
    Q_PROPERTY(bool isBlocked   READ getIsBlocked   WRITE setIsBlocked)


public:
    struct keypair {
        quint16 key;
        quint16 value;
    };
    SquareItem( QGraphicsItem * parent = 0) : QGraphicsItem (parent){};

    QRectF boundingRect() const
    {
        return QRectF(0,0,SIDE, SIDE);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    quint32 getType() const;
    quint16 getRow() const;
    quint16 getColumn() const;
    bool getIsBlocked() const;
    quint32 getDefType() const;
    QVector<keypair> getSession() const;
    qint32 getLatestSession() const;
    qint32 getLatestSessionType() const;

public slots:
    void setType(quint32 arg);
    void modifyType(quint32 arg, int plus, qint32 session);
    void modifyBaseType(quint32 arg, int plus);
    void setRow(quint16 arg);
    void setColumn(quint16 arg);
    void setIsBlocked(bool arg);
    void setDefType(quint32 arg);
    void setSession(QVector<keypair> arg);
    void setLight(quint32 arg, int plus);

private:
    quint32 m_type;
    quint32 m_defType;
    QPen solidPen;
    QPen mePen;
    QPen polyPen;
    QPen borderPen;
    quint16 m_row;
    quint16 m_column;
    bool m_isBlocked;
    bool flag;
    QVector<keypair> m_session;

    //ќбработка событий мышки
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);



signals:
    void on_mouseDoubleClick(QPointF point);
    void on_hoverEnter(QPointF point);
    void on_hoverMove(QPointF point);

};

#endif // SQUAREITEM_H
