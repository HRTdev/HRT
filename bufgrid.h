#ifndef BUFGRID_H
#define BUFGRID_H

#include <QGraphicsItem>
#include <QtCore>
#include <QtGui>

#define CIRC      0
#define RIGTH     1
#define LEFT      2
#define TOP       3
#define DOWN      4
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

#define VLB 0
#define VRB 1
#define HDB 2
#define HUB 3


class bufGrid : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    bufGrid( QGraphicsItem * parent = 0) : QGraphicsItem (parent){};
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    quint16 getRow() const;
    quint16 getColumn() const;
    bool isActive() const;
    quint8 getType() const;
    quint8 getHeigth() const;
    QVector<QString> getNets() const;
    QVector< QVector<quint8> > getMetal() const;

signals:

public slots:
    void setRow(quint16 arg);
    void setColumn(quint16 arg);
    void setActive(bool arg);
    void setMap(QVector< QVector<quint8> > map);
    void setType(quint8 arg);
    void setNets(QVector<QString> arg);
    void setHeigth(quint8 arg);
    void setPolyPos(QVector<quint32> arg);

private:
    QPen mePen;
    QPen polyPen;
    QPen borderPen;
    QPen cellPen;
    QBrush cellBrush;
    quint8 m_type;
    quint16 m_row;
    quint16 m_column;
    bool m_active;
    QVector< QVector<quint8> > m_map;//Карта линий металлизации
    QVector<QString> m_nets;//Карта названий цепей поликремниевых дорожек.
    quint8 m_heigth;
    QVector<quint32> m_polyPos;

};

#endif // BUFGRID_H
