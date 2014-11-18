#ifndef BUFFERCELL_H
#define BUFFERCELL_H

#include <QGraphicsItem>
#include <QtCore>
#include <QtGui>
//что за бред?
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

class bufferCell : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_PROPERTY(QString orientation              READ getOrientation     WRITE setOrientation)
    Q_PROPERTY(QVector<quint32> positionData    READ getPositionData    WRITE setPositionData)
    Q_PROPERTY(bool active                      READ isActive           WRITE setActive)
    Q_PROPERTY(bool selected                    READ isSelected         WRITE setSelected)
    Q_PROPERTY(quint16 macro                    READ getMacro           WRITE setMacro)
public:
    bufferCell( QGraphicsItem * parent = 0) : QGraphicsItem (parent){};
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QString getOrientation() const;
    QVector<quint32> getPositionData() const;
    bool isActive() const;
    bool isSelected() const;
    quint16 getMacro() const;

signals:
    
public slots:
    void setOrientation(QString arg);
    void setPositionData(QVector<quint32> arg);
    void setActive(bool arg);
    void setSelected(bool arg);
    void setMacro(quint16 arg);
    //void setParams(QString bufferNetListName, QString);
    void initialSet(QString orientation, quint16 number, QVector<quint32> positionData);

private:
    QString m_orientation;
    quint16 m_number;
    QVector<quint32> m_positionData;

    QPen polyPen;
    QPen borderPen;
    QPen cellPen;
    QPen mePen;
    QBrush cellBrush;


    QString m_bufferNetListName;
    QString m_bufferDbName;
    quint32 m_bufferDbPinsCnt;
    QVector<QString> m_bufferDbPinsInfo;
    QVector<QString> m_bufferNetListPinsInfo;
    QVector<QString> m_bufferNetListPinsType;
    bool m_selected;
    quint16 m_macro;
    bool m_active;
    QVector< QVector<quint8> > m_map;
};

#endif // BUFFERCELL_H
