#ifndef BASICCELL_H
#define BASICCELL_H

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
class basicCell : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_PROPERTY(quint16 row      READ getRow       WRITE setRow)
    Q_PROPERTY(quint16 column   READ getColumn    WRITE setColumn)
    Q_PROPERTY(bool active      READ isActive     WRITE setActive)
    Q_PROPERTY(bool selected    READ isSelected   WRITE setSelected)
    Q_PROPERTY(quint16 macro    READ getMacro     WRITE setMacro)
public:
    basicCell( QGraphicsItem * parent = 0) : QGraphicsItem (parent){};

    QRectF boundingRect() const
    {
        return QRectF(0,0,4*SIDE,8*SIDE);
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    quint16 getRow() const;
    quint16 getColumn() const;
    bool isActive() const;
    bool isSelected() const;
    quint16 getMacro() const;

signals:
    void oversizedCell(QString m_cellMelName, quint32 m_cellPlace, QPoint m_cellPoint, QString m_cellMelMacro);

public slots:
    void setParams(QString cellMelName, QString cellDbName, quint32 cellDbPinsCnt, quint32 cellCnt,
                   quint32 cellPlace, QVector<QString> cellDbPinsInfo, QVector<QString> cellMelPinsInfo,
                   QVector<QString> cellMelPinsType, QString melMacro);
    void setRow(quint16 arg);
    void setColumn(quint16 arg);
    void setActive(bool arg);
    void setSelected(bool arg);
    void setMacro(quint16 arg);
    void setMap(QVector< QVector<quint8> > map);

private:
    QPen mePen;
    QPen polyPen;
    QPen borderPen;
    QPen cellPen;
    QBrush cellBrush;
    quint16 m_row;
    quint16 m_column;
    bool m_active;
    QString m_cellMelName;
    QString m_cellDbName;
    quint32 m_cellDbPinsCnt;
    quint32 m_cellPlace;
    quint32 m_cellCnt;
    QVector<QString> m_cellDbPinsInfo;
    QVector<QString> m_cellMelPinsInfo;
    QVector<QString> m_cellMelPinsType;
    bool m_selected;
    QString m_cellMelMacro;
    quint16 m_macro;
    QVector< QVector<quint8> > m_map;//Карта линий металлизации
    QVector<QString> m_nets;//Карта названий цепей, длина вектора - 3.
};

#endif // BASICCELL_H
