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

#define VLB 0
#define VRB 1
#define HDB 2
#define HUB 3

class bufferCell : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_PROPERTY(quint8 orientation              READ getOrientation     WRITE setOrientation)
    Q_PROPERTY(QVector<quint32> positionData    READ getPositionData    WRITE setPositionData)
    Q_PROPERTY(bool active                      READ isActive           WRITE setActive)
    Q_PROPERTY(bool selected                    READ isSelected         WRITE setSelected)
public:
    bufferCell( QGraphicsItem * parent = 0) : QGraphicsItem (parent){};
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    quint8 getOrientation() const;
    quint32 getNumber() const;
    QVector<quint32> getPositionData() const;
    bool isActive() const;
    bool isSelected() const;

    QString getName() const;
    QString getDBName() const;
    QVector<QString> getMelPinsInfo() const;
    QVector<QString> getMelPinsType() const;
    QString getMacro() const;

signals:
    
public slots:
    void setParams(QString cellMelName, QString cellDbName, quint32 cellDbPinsCnt,
                   QVector<QString> cellDbPinsInfo, QVector<QString> cellMelPinsInfo,
                   QVector<QString> cellMelPinsType, QString melMacro);
    void setOrientation(quint8 arg);
    void setPositionData(QVector<quint32> arg);
    void setActive(bool arg);
    void setSelected(bool arg);
    void setMacro(QString arg);
    //void setParams(QString bufferNetListName, QString);
    void initialSet(quint8 orientation, quint16 number, QVector<quint32> positionData);

private:
    quint8 m_orientation;
    quint16 m_number;
    QVector<quint32> m_positionData;

    QPen polyPen;
    QPen borderPen;
    QPen cellPen;
    QPen mePen;
    QBrush cellBrush;

    quint32 m_cellDbPinsCnt;
    QString m_cellMelName;
    QString m_cellDbName;
    QString m_cellMelMacro;
    QVector<QString> m_cellDbPinsInfo;
    QVector<QString> m_cellMelPinsInfo;
    QVector<QString> m_cellMelPinsType;
    QVector<QString> m_nets;// арта названий цепей, длина вектора - 3.


    quint16 m_macro;

    bool m_active;
    bool m_selected;
};

#endif // BUFFERCELL_H
