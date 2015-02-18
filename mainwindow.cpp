#include "mainwindow.h"


//Определяем направления роутинга
#define NONE 0
#define RIGHT 1
#define LEFT 2
#define TOP 3
#define DOWN 4

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new MyScene(this);
    scene->setBackgroundBrush(SCENE_COLOR);

    //scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    //ui->graphicsView->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    //scene->setBspTreeDepth(15);
    //scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    //ui->graphicsView->scale(1,-1);

    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    connect(scene, SIGNAL(trassRoutingHorBegin(int)), this, SLOT(on_trassRoutingBegin(int)));
    connect(scene, SIGNAL(trassRoutingHorEnd(int)), this, SLOT(on_trassRoutingEnd(int)));
    connect(scene, SIGNAL(trassRoutingVerBegin(int)), this, SLOT(on_trassRoutingVerBegin(int)));
    connect(scene, SIGNAL(trassRoutingVerEnd(int)), this, SLOT(on_trassRoutingVerEnd(int)));
    connect(scene, SIGNAL(programStateChanged()), this, SLOT(on_programStateChanged()));
    connect(scene, SIGNAL(deletingTrace(int)), this, SLOT(on_deletingTrace(int)));
    //connect(ui->tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem)), this, SLOT(on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)));
    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_tableView_itemDoubleClicked(QModelIndex)));
    connect(scene, SIGNAL(unitPlacing()), this, SLOT(on_myScene_unitPlacing()));
    connect(scene, SIGNAL(testRout(int)),this,SLOT(reactOnTestRoute(int)));
    //connect()

            //ui->tableWidget->itemDoubleClicked();

    scaleFactor = 1;
    ui->horizontalSlider->setTickPosition(QSlider::TicksBelow);

    max_pos_delta = 0;
    max_neg_delta = 0;
    sessionCounter = 0;
    TABLE_HEIGTH = 1;
    TABLE_WIDTH = 1;


    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(pointsDetection()));
    m_timer->start(25);


    connectToDatabase();
    createActions();
    createMenus();
    loadFileIni("D:\\Qt\\HRT500.INI");

    loadFileDb("D:\\Qt\\FORT_NEW.DAT");
    //loadFileMel("D:\\Qt\\HV154k2_new.mel");
    //fillInfoTable();

    drawTable();
    ui->graphicsView->update();
    QCoreApplication::setOrganizationName("Roga i kopita");
    QCoreApplication::setOrganizationDomain("ya.ru");
    QCoreApplication::setApplicationName("HRT");

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    //ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->lineEdit->setText("Waiting for user interaction...");



}

void MainWindow::update(){

}

void MainWindow::drawTable(){
    m_timer->stop();
    quint32 colCnt = 0;
    quint32 rowCnt = 0;

    ui->graphicsView->resetCachedContent();
    scene->clear();

    basicCell* newCell = new basicCell;
    QVector<basicCell*> lineOfCells;
    cell.clear();
    lineOfCells.clear();
    for (quint32 x = 0; x < unitsInColumn; x++){
        lineOfCells.append(newCell);
    }
    for (quint32 y = 0; y < unitsInRow; y++){
        cell.append(lineOfCells);
    }

    gridCell* newGrid = new gridCell;
    QVector<gridCell*> gridOfCells;
    grid.clear();
    gridOfCells.clear();
    for (quint32 x = 0; x < unitsInColumn + 1; x++){
        gridOfCells.append(newGrid);
    }
    for (quint32 y = 0; y < unitsInRow; y++){
        grid.append(gridOfCells);
    }

    bufferCell* newBuf = new bufferCell;
    QVector<bufferCell*> gridOfBufs;
    gridOfBufs.clear();
    buffer.clear();
    for (quint32 x = 0; x < unitsInColumn + 1; x++){
        gridOfBufs.append(newBuf);
    }
    for (quint32 y = 0; y < unitsInRow; y++){
        buffer.append(gridOfBufs);
    }

    bufGrid* newBufGrid = new bufGrid;
    QVector<bufGrid*> arrOfBufGrids;
    arrOfBufGrids.clear();
    bGrid.clear();
    for (quint32 x = 0; x < unitsInColumn + 1; x++){
        arrOfBufGrids.append(newBufGrid);
    }
    for (quint32 y = 0; y < unitsInRow; y++){
        bGrid.append(arrOfBufGrids);
    }


    QPen temppe;
    temppe = QPen(Qt::darkMagenta);
    temppe.setWidth(4);
    scene->addRect(0,0,SIDE*TABLE_WIDTH, SIDE*TABLE_HEIGTH,temppe);
    scene->addRect(10*SIDE,10*SIDE,SIDE*TABLE_WIDTH-20*SIDE, SIDE*TABLE_HEIGTH-20*SIDE,QPen(Qt::green));

    scene->setSceneRect(0,0,SIDE*TABLE_WIDTH, SIDE*TABLE_HEIGTH);
    ui->graphicsView->fitInView(QRect(0,0,SIDE*TABLE_WIDTH, SIDE*TABLE_HEIGTH),Qt::KeepAspectRatio);


    //QHash< QString, QVector<quint32> >iniBufPos;
    QHashIterator< QString, QVector<quint32> > i(iniBufPos);
    QRegExp rxV("^VL.*");
    QVector<quint32> posData;
    QVector<quint32> postData(3);
    posData = iniBufPos["VLB"];
    for(quint32 i = 0; i < posData.size()/4; i++){
        postData[0] = TABLE_HEIGTH - posData[1+4*i] - 1;
        postData[1] = TABLE_HEIGTH - posData[2+4*i] - 1;
        postData[2] = TABLE_HEIGTH - posData[3+4*i] - 1;
        if(postData[0]<postData[2]){
            quint32 t = postData[0];
            postData[0] = postData[2];
            postData[2] = t;
        }
        buffer[0][i] = new bufferCell;
        buffer[0][i]->setPos(0,postData[2]*SIDE);

        bGrid[0][i] = new bufGrid;
        bGrid[0][i]->setPos(10*SIDE,postData[2]*SIDE);

        postData[0] -= postData[2];
        postData[1] -= postData[2];
        postData[2] -= postData[2];

        buffer[0][i]->initialSet(VLB, posData[0+4*i], postData);
        buffer[0][i]->setActive(false);

        bGrid[0][i]->setPolyPos(postData);
        bGrid[0][i]->setType(0);

        scene->addItem(buffer[0][i]);
        scene->addItem(bGrid[0][i]);
    }

    posData = iniBufPos["VRB"];
    for(quint32 i = 0; i < posData.size()/4; i++){
        postData[0] = TABLE_HEIGTH - posData[1+4*i] - 1;
        postData[1] = TABLE_HEIGTH - posData[2+4*i] - 1;
        postData[2] = TABLE_HEIGTH - posData[3+4*i] - 1;
        if(postData[0]<postData[2]){
            quint32 t = postData[0];
            postData[0] = postData[2];
            postData[2] = t;
        }
        buffer[1][i] = new bufferCell;
        buffer[1][i]->setPos((TABLE_WIDTH - 10)*SIDE,postData[2]*SIDE);

        bGrid[1][i] = new bufGrid;
        bGrid[1][i]->setPos((TABLE_WIDTH - 20)*SIDE,postData[2]*SIDE);

        postData[0] -= postData[2];
        postData[1] -= postData[2];
        postData[2] -= postData[2];

        buffer[1][i]->initialSet(VRB, posData[0+4*i], postData);
        buffer[1][i]->setActive(false);

        bGrid[1][i]->setPolyPos(postData);
        bGrid[1][i]->setType(1);

        scene->addItem(buffer[1][i]);
        scene->addItem(bGrid[1][i]);
    }

    posData = iniBufPos["HDB"];
    for(quint32 i = 0; i < posData.size()/4; i++){
        postData[0] = posData[1+4*i];
        postData[1] = posData[2+4*i];
        postData[2] = posData[3+4*i];
        if(postData[0]<postData[2]){
            quint32 t = postData[0];
            postData[0] = postData[2];
            postData[2] = t;
        }

        buffer[2][i] = new bufferCell;
        buffer[2][i]->setPos(postData[2]*SIDE,(TABLE_HEIGTH - 10)*SIDE);

        bGrid[2][i] = new bufGrid;
        bGrid[2][i]->setPos(postData[2]*SIDE,(TABLE_HEIGTH - 20)*SIDE);

        postData[0] -= postData[2];
        postData[1] -= postData[2];
        postData[2] -= postData[2];

        buffer[2][i]->initialSet(HDB, posData[0+4*i], postData);
        buffer[2][i]->setActive(false);

        bGrid[2][i]->setPolyPos(postData);
        bGrid[2][i]->setType(2);

        scene->addItem(buffer[2][i]);
        scene->addItem(bGrid[2][i]);
    }

    posData = iniBufPos["HUB"];
    for(quint32 i = 0; i < posData.size()/4; i++){
        postData[0] = posData[1+4*i];
        postData[1] = posData[2+4*i];
        postData[2] = posData[3+4*i];
        if(postData[0]<postData[2]){
            quint32 t = postData[0];
            postData[0] = postData[2];
            postData[2] = t;
        }
        buffer[3][i] = new bufferCell;
        buffer[3][i]->setPos(postData[2]*SIDE,0);

        bGrid[3][i] = new bufGrid;
        bGrid[3][i]->setPos(postData[2]*SIDE,10*SIDE);

        postData[0] -= postData[2];
        postData[1] -= postData[2];
        postData[2] -= postData[2];

        buffer[3][i]->initialSet(HUB, posData[0+4*i], postData);
        buffer[3][i]->setActive(false);

        bGrid[3][i]->setPolyPos(postData);
        bGrid[3][i]->setType(3);

        scene->addItem(buffer[3][i]);
        scene->addItem(bGrid[3][i]);
    }


    QVector<QString> tmp;
    for (quint32 x = 0; x < unitsInRow; x++){
        for(quint32 y = 0; y < unitsInColumn; y++){
            colCnt = x * unitWidth + horPeriodicBegin;
            rowCnt = y * unitHeigth + verPeriodicBegin;

            cell[x][y] = new basicCell;
            cell[x][y]->setPos((colCnt-1)*SIDE,SIDE*((rowCnt-1)+traceChannel));
            cell[x][y]->setParams("","",0,0,0,tmp,tmp,tmp,"");
            cell[x][y]->setActive(false);
            cell[x][y]->setRow(y);
            cell[x][y]->setColumn(x);

            connect(cell[x][y], SIGNAL(oversizedCell(QString, quint32, QPoint, QString)), this, SLOT(on_basicCell_oversizedCell(QString, quint32, QPoint, QString)));
            scene->addItem(cell[x][y]);
        }
    }

    for (quint32 x = 0; x < unitsInRow; x++){
        for(quint32 y = 0; y <= unitsInColumn; y++){
            colCnt = x * unitWidth + horPeriodicBegin;
            rowCnt = y * unitHeigth + verPeriodicBegin;

            grid[x][y] = new gridCell;
            grid[x][y]->setPos((colCnt-1)*SIDE,SIDE*(rowCnt-1));
            grid[x][y]->setRow(y);
            grid[x][y]->setColumn(x);
            grid[x][y]->setHeigth(traceChannel);
            if(y == unitsInColumn){
                grid[x][y]->setType(1);
            }else if(y == 0){
                grid[x][y]->setType(2);
            }else grid[x][y]->setType(0);

            //Test for m_map
            QVector<quint8> tmpvec1;
            quint8 tmp2=0;
            tmpvec1.append(tmp2);
            tmpvec1.append(tmp2);
            QVector< QVector<quint8> > tmpvec;
            tmpvec.append(tmpvec1);
            tmpvec.append(tmpvec1);
            tmpvec[1][0] = 1;
            //grid[x][y]->setMap(tmpvec);
            //cell[0][0]->setMap(tmpvec);
            //End of test
            QVector<QString> temp;
            QString tmp = "";
            temp.append(tmp);
            temp.append(tmp);
            temp.append(tmp);
            temp.append(tmp);
            grid[x][y]->setNets(temp);

            scene->addItem(grid[x][y]);
        }
    }


    //Drawing items, that already presence in project;
    QSqlQuery query;
    //query.prepare("SELECT unitId, unitRow, unitColumn FROM unitPlacement WHERE isHidden != 1");
    query.prepare("SELECT N.melUnitName, P.unitRow, P.unitColumn, P.unitId, N.macroUnitName FROM sourceData S, unitNets N, unitPlacement P, projects "
                  "WHERE P.unitId = N.unitId AND N.fileId = S.fileId "
                  "AND projects.projectName = :name AND projects.projectName = S.projectName "
                  "AND N.isPlaced = 1");
    query.bindValue(":name",projectName);
    query.exec();
    //qDebug()<<"Error 1: "<<query.lastError().text();
    while (query.next()) {
        melUnitChosen = query.value(0).toString();
        quint32 unitRow = query.value(1).toInt();
        quint32 unitColumn = query.value(2).toInt();
        melUnitChosenMacro = query.value(4).toString();

        lastPoint.setX(unitWidth * unitColumn + horPeriodicBegin - 1);
        lastPoint.setY((cellHeigth + traceChannel) * unitRow + verPeriodicBegin );
        if(dbUnitType.value(melUnitFName.value(melUnitChosenMacro).value(melUnitChosen)) != 1){
            on_myScene_initialUnitPlacing();
        }
        //qDebug() << "Note: Element loading function is working;";
    }
    ui->graphicsView->update();
    m_timer->start(25);
}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_radioButton_clicked()
{
    //square->setType(SquareItem::empty);
   // scene->setItemIndexMethod(QGraphicsScene::BspTreeIndex);

    scene->update();
}

void MainWindow::on_checkBox_stateChanged(){
    if(ui->checkBox->checkState()){
        ui->tableView->show();
    }else{
        ui->tableView->hide();
    }
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    qreal scale = qPow(qreal(2), (ui->horizontalSlider->value() - 250) / qreal(50));
    ui->graphicsView->scale(1/scaleFactor, 1/scaleFactor);
    ui->graphicsView->scale(scale, scale);
    scaleFactor = scale;
}


qint32 MainWindow::returnX(QPointF point){
    quint32 x = (int)(point.x() / SIDE);
    return x;
}

qint32 MainWindow::returnY(QPointF point){
    quint32 y = (int)(point.y() / SIDE);
    return y;
}



void MainWindow::on_doubleClick(QPointF point){

}


void MainWindow::on_hoverEnter(QPointF point){
    //qDebug()<<"Square enter";
    //Далее код, относящийся к отрисовке линии трассировки.

    quint32 x = MainWindow::returnX(point);//Координаты по х и у текущего элементарного квадратика на сцене размером SIDE*SIDE
    quint32 y = MainWindow::returnY(point);

    //Check if it in a grid area(but inside cell area), and detect in which cell
    if(x >= horPeriodicBegin && y>=verPeriodicBegin-1 && x <= TABLE_WIDTH - horPeriodicBegin + 1 && y<= TABLE_HEIGTH - verPeriodicBegin){
        for (quint32 rowNum = 0; rowNum<=unitsInColumn; rowNum++){
            quint32 grid_max_y = verPeriodicBegin + unitHeigth * rowNum + traceChannel;
            quint32 grid_min_y = verPeriodicBegin + unitHeigth * rowNum;
            if(y>= grid_min_y-1 && y<grid_max_y-1){
                quint32 coordX = (x - horPeriodicBegin)/unitWidth;//Координаты по х и у текущего элемента сцены типа grid.
                quint32 coordY = (y - verPeriodicBegin + 1)/unitHeigth;
                quint32 inGridX = x - coordX*unitWidth + horPeriodicBegin;
                quint32 inGridY = y - coordY*unitHeigth + verPeriodicBegin;
                qDebug()<<"In grid: "<<inGridX<<inGridY;


            }
        }
    }


    /*
    int startx, starty;
    startx = traceStartPoint.x();
    starty = traceStartPoint.y();

    if (scene->getProgramState() == MyScene::trass_routing_hor){
        if ( currentSquare_x - startx > max_pos_delta){
            max_pos_delta = currentSquare_x - startx;
        }
        if(currentSquare_x - startx < max_neg_delta){
            max_neg_delta = currentSquare_x - startx;
        }


        if(currentSquare_x - startx > 0){//Going ->
            routingDirection = RIGHT;
            for (int count = startx+1; count < currentSquare_x; count++){
                grid[count][starty]->modifyType(horizontalMe, ADD, sessionCounter);
            }
            for (int count = currentSquare_x+1; count <= startx + max_pos_delta; count++){
                grid[count][starty]->modifyType(horizontalMe, DEL, sessionCounter);
            }
            for (int count = startx; count >= startx + max_neg_delta; count--){
                grid[count][starty]->modifyType(horizontalMe, DEL, sessionCounter);
            }
            grid[startx][starty]->modifyType( 1 << rightMe, ADD, sessionCounter);
            grid[currentSquare_x][starty]->modifyType(1 << leftMe, ADD, sessionCounter);
        }
        else if (currentSquare_x - startx < 0){//Going <-
            routingDirection = LEFT;
            for (int count = startx-1; count > currentSquare_x; count--){
                grid[count][starty]->modifyType(horizontalMe, ADD, sessionCounter);
            }
            for (int count = startx + max_neg_delta; count <= currentSquare_x-1; count++){
                grid[count][starty]->modifyType(horizontalMe, DEL, sessionCounter);
            }
            for (int count = startx; count <= startx + max_pos_delta; count++){
                grid[count][starty]->modifyType(horizontalMe, DEL, sessionCounter);
            }
            grid[startx][starty]->modifyType(1 << leftMe, ADD, sessionCounter);
            grid[currentSquare_x][starty]->modifyType(1 << rightMe, ADD, sessionCounter);
        }else{
            routingDirection = NONE;
            for (int count = startx + max_neg_delta; count <= startx + max_pos_delta; count++){
                    square[count][starty]->modifyType(horizontalMe, DEL, sessionCounter);
            }
        }

        traceEndPoint.setY(starty);
        traceEndPoint.setX(currentSquare_x);
    }else if(scene->getProgramState() == MyScene::trass_routing_ver){
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if ( currentSquare_y - starty > max_pos_delta){
            max_pos_delta = currentSquare_y - starty;
        }
        if(currentSquare_y - starty < max_neg_delta){
            max_neg_delta = currentSquare_y - starty;
        }


        if(currentSquare_y - starty > 0){//Going down->
            routingDirection = DOWN;
            for (int count = starty+1; count < currentSquare_y; count++){
                grid[startx][count]->modifyType(verticalMe, ADD, sessionCounter);
            }
            for (int count = currentSquare_y+1; count <= starty + max_pos_delta; count++){
                grid[startx][count]->modifyType(verticalMe, DEL, sessionCounter);
            }
            for (int count = starty; count >= starty + max_neg_delta; count--){
                grid[startx][count]->modifyType(verticalMe, DEL, sessionCounter);
            }
            grid[startx][starty]->modifyType(1 << downMe, ADD, sessionCounter);
            grid[startx][currentSquare_y]->modifyType(1 << topMe, ADD, sessionCounter);
        }
        else if (currentSquare_y - starty < 0){//Going top<-
            routingDirection = TOP;
            for (int count = starty-1; count > currentSquare_y; count--){
                grid[startx][count]->modifyType(verticalMe, ADD, sessionCounter);
            }
            for (int count = starty + max_neg_delta; count <= currentSquare_y-1; count++){
                grid[startx][count]->modifyType(verticalMe, DEL, sessionCounter);
            }
            for (int count = starty; count <= starty + max_pos_delta; count++){
                grid[startx][count]->modifyType(verticalMe, DEL, sessionCounter);
            }
            grid[startx][starty]->modifyType(1 << topMe, ADD, sessionCounter);
            grid[startx][currentSquare_y]->modifyType(1 << downMe, ADD, sessionCounter);
        }else{
            routingDirection = NONE;
            for (int count = starty + max_neg_delta; count <= starty + max_pos_delta; count++){
                    grid[startx][count]->modifyType(verticalMe, DEL, sessionCounter);
            }
            //square[startx][starty]->modifyType(1 << circ, ADD, sessionCounter);
        }
        traceEndPoint.setY(currentSquare_y);
        traceEndPoint.setX(startx);
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    */
}


void MainWindow::on_hoverMove(QPointF point)
{
    //Обновление информации о уровне меню!
    quint32 currentSquare_x = MainWindow::returnX(point);
    quint32 currentSquare_y = MainWindow::returnY(point);
    if (currentSquare_x > TABLE_WIDTH){currentSquare_x = TABLE_WIDTH;}
    if (currentSquare_y > TABLE_HEIGTH){currentSquare_y = TABLE_HEIGTH;}
    if (currentSquare_x < 1){currentSquare_x = 1;}
    if (currentSquare_y < 1){currentSquare_y = 1;}
    if ((currentSquare_x != lastPoint.x()) || (currentSquare_y != lastPoint.y())){
        on_hoverEnter(point);
        update();
    }
    //lastPoint.setX(currentSquare_x);
    //lastPoint.setY(currentSquare_y);
    ui->MenuLevelLabel->setText(QString::number(square[currentSquare_x][currentSquare_y]->getType()));
    ui->ColumnLabel->setText(QString::number(currentSquare_x));
    ui->RowLabel->setText(QString::number(currentSquare_y));
    ui->sessionLabel->setText(melUnitChosen);
    //ui->typeLabel->setText(QString::number(square[currentSquare_x][currentSquare_y]->getLatestSessionType()));
}


void MainWindow::on_trassRoutingBegin(int beginKey)
{
    qDebug()<<"Horizontal trace start point is: "<<traceStartPoint<<lastPoint;
    max_pos_delta = 0;
    max_neg_delta = 0;
    routingDirection = NONE;
    if(beginKey == Qt::Key_C){
        traceStartPoint = lastPoint;
        traceEndPoint = lastPoint;//Нужно на случай комбинации: C+A+двигаем мышку.
        sessionCounter++;
        int startx, starty;
        startx = traceStartPoint.x();
        starty = traceStartPoint.y();
        qDebug()<<"1"<<startx<<starty;
        //square[startx][starty]->modifyType(1 << circ, ADD, sessionCounter);
        scene->update();
    }
}

void MainWindow::on_trassRoutingEnd(int endKey)
{
    int starty, endx;
    starty = traceStartPoint.y();
    endx = traceEndPoint.x();
/*
    if(endKey == Qt::Key_C){
        if (routingDirection == RIGHT){
            square[endx][starty]->modifyType(horizontalMe, DEL, sessionCounter);
            square[endx][starty]->modifyType(leftMeCirc, ADD, sessionCounter);
        }else if(routingDirection == LEFT){
            square[endx][starty]->modifyType(horizontalMe, DEL, sessionCounter);
            square[endx][starty]->modifyType(rightMeCirc, ADD, sessionCounter);
        }else{
            square[endx][starty]->modifyType(horizontalMe, DEL, sessionCounter);
            square[endx][starty]->modifyType(1 << circ, ADD, sessionCounter);
        }
        scene->setProgramState(MyScene::trass_menu);
    }else if (endKey == Qt::Key_L){
        scene->setProgramState(MyScene::trass_menu);
        if (routingDirection == RIGHT){
            square[endx][starty]->modifyType(horizontalMe, DEL, sessionCounter);
            square[endx][starty]->modifyType(1 << leftMe, ADD, sessionCounter);
        }else if(routingDirection == LEFT){
            square[endx][starty]->modifyType(horizontalMe, DEL, sessionCounter);
            square[endx][starty]->modifyType(1 << rightMe, ADD, sessionCounter);
        }
    }else if (endKey == Qt::Key_A){
        if (routingDirection == RIGHT){
            square[endx][starty]->modifyType(horizontalMe, DEL, sessionCounter);
            square[endx][starty]->modifyType(1 << leftMe, ADD, sessionCounter);
        }else if(routingDirection == LEFT){
            square[endx][starty]->modifyType(horizontalMe, DEL, sessionCounter);
            square[endx][starty]->modifyType(1 << rightMe, ADD, sessionCounter);
        }else{
            square[endx][starty]->modifyType(horizontalMe, DEL, sessionCounter);
        }
        scene->setProgramState(MyScene::trass_routing_ver);
    }
    traceStartPoint = traceEndPoint;
    scene->update();
    */
    qDebug()<<"Horizontal trace end point is: "<<traceEndPoint;
}

void MainWindow::on_trassRoutingVerBegin(int beginKey)
{

    max_pos_delta = 0;
    max_neg_delta = 0;
    routingDirection = NONE;

    qDebug()<<"Vertical trace start point is: "<<traceStartPoint;
}

void MainWindow::on_trassRoutingVerEnd(int endKey)
{
    int startx, endy;
    startx = traceStartPoint.x();
    endy = traceEndPoint.y();

    if(endKey == Qt::Key_C){
        scene->setProgramState(MyScene::trass_menu);
        if (routingDirection == TOP){
            square[startx][endy]->modifyType(verticalMe, DEL, sessionCounter);
            square[startx][endy]->modifyType(downMeCirc, ADD, sessionCounter);
        }else if(routingDirection == DOWN){
            square[startx][endy]->modifyType(verticalMe, DEL, sessionCounter);
            square[startx][endy]->modifyType(topMeCirc, ADD, sessionCounter);
        }else{
            square[startx][endy]->modifyType(verticalMe, DEL, sessionCounter);
            square[startx][endy]->modifyType(1 << circ, ADD, sessionCounter);
        }
    }else if (endKey == Qt::Key_L){
        scene->setProgramState(MyScene::trass_menu);
        if (routingDirection == TOP){
            square[startx][endy]->modifyType(verticalMe, DEL, sessionCounter);
            square[startx][endy]->modifyType(1 << downMe, ADD, sessionCounter);
        }else if(routingDirection == DOWN){
            square[startx][endy]->modifyType(verticalMe, DEL, sessionCounter);
            square[startx][endy]->modifyType(1 << topMe, ADD, sessionCounter);
        }
    }else if (endKey == Qt::Key_A){
        scene->setProgramState(MyScene::trass_routing_hor);
        if (routingDirection == TOP){
            square[startx][endy]->modifyType(verticalMe, DEL, sessionCounter);
            square[startx][endy]->modifyType(1 << downMe, ADD, sessionCounter);
        }else if(routingDirection == DOWN){
            square[startx][endy]->modifyType(verticalMe, DEL, sessionCounter);
            qDebug()<<"Before: "<<square[startx][endy]->getType();
            square[startx][endy]->modifyType(1 << topMe, ADD, sessionCounter);
            qDebug()<<"After: "<<square[startx][endy]->getType();
        }
    }
    traceStartPoint = traceEndPoint;
    scene->update();
    max_pos_delta = 0;
    max_neg_delta = 0;
    qDebug()<<"Vertical trace end point is: "<<traceEndPoint;
}


void MainWindow::on_programStateChanged()
{
    QString text;
    switch (scene->getProgramState()){
    case 0: text = "Main menu";
        break;
    case 1: text = "Trass menu";
        break;
    case 2: text = "Trass routing";
        break;
    case 3: text = "trass routing end";
        break;
    case 4: text = "vertical trass routing";
        break;
    case 6: text = "Deleting menu";
        break;
    case 8: text = "Highlighting";
        break;
    case 9: text = "Placing";
        break;
    case 11: text = "Node selecting";
        break;
    default: text = "undefined state";
    }
    ui->lineEdit->setText(text);
}

void MainWindow::on_deletingTrace(int delKey)
{
    int x = lastPoint.x();
    int y = lastPoint.y();

    qint32 lateSession = square[x][y]->getLatestSession();
    for (quint32 col_cnt = 1; col_cnt <= TABLE_WIDTH; col_cnt++){
        for (quint32 row_cnt = 1; row_cnt <= TABLE_HEIGTH; row_cnt++){
            square[col_cnt][row_cnt]->modifyType(horizontalMe, DEL, lateSession);
            square[col_cnt][row_cnt]->modifyType(verticalMe, DEL, lateSession);
            square[col_cnt][row_cnt]->modifyType(1<<circ, DEL, lateSession);
        }
    }
    scene->setProgramState(MyScene::deleting_menu);
}






void MainWindow::createActions(){
    createProject = new QAction(QIcon(":/images/new.png"), tr("New project"), this);
    createProject->setStatusTip(tr("Create a new project"));
    connect(createProject, SIGNAL(triggered()), this, SLOT( showNewProject() ));

    loadProject = new QAction(QIcon(":/images/open.png"), tr("Load project"), this);
    loadProject->setShortcut(QKeySequence::Open);
    loadProject->setStatusTip(tr("Open an existing project"));
    connect(loadProject, SIGNAL(triggered()), this, SLOT(openProject()));

    openActIni = new QAction(QIcon(":/images/open.png"), tr("&Open ini..."), this);
    //openActIni->setShortcuts(QKeySequence::Open);
    openActIni->setStatusTip(tr("Open an existing file"));
    connect(openActIni, SIGNAL(triggered()), this, SLOT(openIni()));

    openActDb = new QAction(QIcon(":/images/open.png"), tr("&Open db..."), this);
    openActDb->setStatusTip(tr("Open an existing file"));
    connect(openActDb, SIGNAL(triggered()), this, SLOT(openDb()));

    openActMel = new QAction(QIcon(":/images/open.png"), tr("&Open mel..."), this);
    openActMel->setStatusTip(tr("Open an existing file"));
    connect(openActMel, SIGNAL(triggered()), this, SLOT(openMel()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    wiringAction = new QAction(tr("wiring"), this);
    wiringAction->setCheckable(true);
    connect(wiringAction, SIGNAL(triggered()), this, SLOT(on_wiringAction()));
    ui->mainToolBar->addAction(wiringAction);
}


void MainWindow::createMenus(){
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(createProject);
    fileMenu->addAction(loadProject);
    fileMenu->addAction(openActIni);
    fileMenu->addAction(openActDb);
    fileMenu->addAction(openActMel);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);


}

void MainWindow::showNewProject(){
    projectMenu = new newProjectMenu();
    projectMenu->setVisible(true);
    connect(projectMenu->okButton, SIGNAL(clicked()), this, SLOT(newProjectNameCommitted()));
    connect(projectMenu->cancelButton, SIGNAL(clicked()), projectMenu, SLOT(close()));
    connect(projectMenu, SIGNAL(checkProjectName(QString)),this, SLOT(on_projectMenu_checkProjectName(QString)));
    connect(this, SIGNAL(fileNameCheckingResult(bool)),projectMenu,SLOT(on_fileNameCheckingResult(bool)));

}

void MainWindow::openProject(){
    openProjectWidget = new openProjectMenu();
    openProjectWidget->setVisible(true);
    //openProjectWidget->setDB(&db);
    connect(openProjectWidget, SIGNAL(openProject(QString)), this, SLOT(on_openProjectWidget_openProject(QString)));
    connect(openProjectWidget->cancelButton, SIGNAL(clicked()), openProjectWidget, SLOT(close()));
}

void MainWindow::openIni()
{
    if (true) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFileIni(fileName);
    }
}


void MainWindow::loadFileIni(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    QString bufPos;

    //QRegExp rxChip("^Chip:\\s(\\d+)");
    QRegExp rxTraceChannel("^Trace_Channel:\\s(\\d+)");
    QRegExp rxUnitsInRow("^Cell_Rows:\\s(\\d+)");
    QRegExp rxUnitsInColumn("Cell_Lines:\\s(\\d+)");
    //QRegExp rxBufCountHor("Buf_CountH:\\s(\\d+)");
    //QRegExp rxBufCountVer("Buf_CountV:\\s(\\d+)");
    QRegExp rxHorPeriodicBegin("Cell\\_Comp\\_MinX:\\s(\\d+)");
    QRegExp rxVerPeriodicBegin("Cell\\_Comp\\_MinY:\\s(\\d+)");
    //QRegExp rxBufVoidYL("Cell_Void_YL:\\s(\\d+)");
    //QRegExp rxBufVoidPosL("Cell_Void_PosL:\\s(\\d+)");
    //QRegExp rxBufVoidYR("Cell_Void_YR:\\s(\\d+)");
    //QRegExp rxBufVoidPosR("Cell_Void_PosR:\\s(\\d+)");
    QRegExp rxBufPos("^Buffer_pos:\\s([A-Z]+)");
    QRegExp rxBufPosData("^(\\d+)\\s(\\d+)\\s(\\d+)\\s(\\d+)");
    QString line;


    iniBufPos.clear();
    do {
        line = in.readLine();
        if (rxTraceChannel.indexIn(line) != -1){
            traceChannel = rxTraceChannel.cap(1).toInt();
            qDebug()<<"traceChannel"<<traceChannel;
        }
        if (rxUnitsInRow.indexIn(line) != -1){
            unitsInRow = rxUnitsInRow.cap(1).toInt();
        }
        if (rxUnitsInColumn.indexIn(line) != -1){
            unitsInColumn = rxUnitsInColumn.cap(1).toInt();
        }
        if (rxHorPeriodicBegin.indexIn(line) != -1){
            horPeriodicBegin = rxHorPeriodicBegin.cap(1).toInt() + 1;
        }
        if (rxVerPeriodicBegin.indexIn(line) != -1){
            verPeriodicBegin = rxVerPeriodicBegin.cap(1).toInt() + 1;
        }
        if (rxBufPos.indexIn(line) != -1){
            bufPos = rxBufPos.cap(1);
        }
        if (rxBufPosData.indexIn(line) != -1){
            iniBufPos[bufPos].append(rxBufPosData.cap(1).toInt());
            iniBufPos[bufPos].append(rxBufPosData.cap(2).toInt());
            iniBufPos[bufPos].append(rxBufPosData.cap(3).toInt());
            iniBufPos[bufPos].append(rxBufPosData.cap(4).toInt());
        }
    } while (!line.isNull());
    cellHeigth = 8;
    unitWidth = 4;
    TABLE_WIDTH = 2*(horPeriodicBegin - 1) + unitWidth * unitsInRow;
    TABLE_HEIGTH = 2*(verPeriodicBegin - 1) + cellHeigth*unitsInColumn + traceChannel*(unitsInColumn+1);

    //qDebug()<<"TABLE_WIDTH"<<TABLE_WIDTH;
    //qDebug()<<"TABLE_HEIGTH"<<TABLE_HEIGTH;

    unitHeigth = cellHeigth + traceChannel;
    //
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    //textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    //setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);





        //bool ok = db.open();
}

void MainWindow::openDb()
{
    if (true) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFileDb(fileName);
    }
}


void MainWindow::loadFileDb(const QString &fileName)
{
    dbUnitType.clear();
    dbUnitCellCnt.clear();
    dbUnitInnerTrs.clear();
    dbUnitPinsCnt.clear();
    dbUnitPinsInfo.clear();

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    //
    QString dbUnitName;
    QString line;
    QRegExp rxName("^Name:\\s(F\\d+)");
    QRegExp rxType("^Type:\\s(\\d+)");
    QRegExp rxCellCnt("^Cell_cnt:\\s(\\d+)");
    QRegExp rxInnerTrs("^Inner_tr:\\s(\\d+)");
    QRegExp rxPinsCnt("^Pins_cnt:\\s(\\d+)");
    QRegExp rxPinInfo("^C\\dP\\d:\\s([0-9A-Z!*]+)\\s(\\d+)\\s(\\d+)");
    do {
        line = in.readLine();
        if (rxName.indexIn(line) != -1){
            dbUnitName = rxName.cap(1);
            //qDebug()<<"BD name: "<<rxName.cap(1)<<dbUnitName;
        }
        if (rxType.indexIn(line) != -1){
            dbUnitType.insert(dbUnitName, rxType.cap(1).toInt());
            //qDebug()<<"type: "<<dbUnitType.value(dbUnitName)<<"name: "<<dbUnitName;
        }
        if (rxCellCnt.indexIn(line) != -1){
            dbUnitCellCnt.insert(dbUnitName, rxCellCnt.cap(1).toInt());
        }
        if (rxInnerTrs.indexIn(line) != -1){
            dbUnitInnerTrs.insert(dbUnitName, rxInnerTrs.cap(1).toInt());
        }
        if (rxPinsCnt.indexIn(line) != -1){
            dbUnitPinsCnt.insert(dbUnitName, rxPinsCnt.cap(1).toInt());
        }
        if (rxPinInfo.indexIn(line) != -1){

            dbUnitPinsInfo[dbUnitName].append(rxPinInfo.cap(1));
            dbUnitPinsInfo[dbUnitName].append(rxPinInfo.cap(2));
            dbUnitPinsInfo[dbUnitName].append(rxPinInfo.cap(3));
            //qDebug()<<"info: "<<dbUnitPinsInfo.value(dbUnitName)<<"name: "<<dbUnitName;
        }
    } while (!line.isNull());
    //
    qDebug()<<"Name: 3028";
    qDebug()<<"Type: "<<dbUnitType.value("F3028");
    qDebug()<<"Cell_cnt: "<<dbUnitCellCnt.value("F3028");
    qDebug()<<"Inner_trs: :"<<dbUnitInnerTrs.value("F3028");
    qDebug()<<"Pins_cnt: "<<dbUnitPinsCnt.value("F3028");
    qDebug()<<"Pins info: "<<dbUnitPinsInfo.value("F3028");
    //cell[2][2]->setParams(3006,1,dbUnitPinsCnt.value(3006),dbUnitPinsInfo.value(3006));
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    //textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    //setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
    ui->graphicsView->update();
}


void MainWindow::openMel()
{
    if (true) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFileMel(fileName);
    }
}

//Данная функция выполняет работу по парсингу файла, поступившего в качестве MEL файла.
//Также она записывает полученные данные в память программы и в базу данных.
//На данный момент в работе программы участвую оба источника данных.
//В будущем планируется перевести все на SQL.
void MainWindow::loadFileMel(const QString &fileName)
{
    ui->horizontalSlider->setValue(100);
    //melUnitFName.clear();
    //melUnitStatus.clear();
    //melUnitPinName.clear();
    //melUnitPinType.clear();

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    QString melUnitDDNum;
    QString fortUnitNum;
    QString line;
    QRegExp rxUnit("^(DD\\d{1,5})\\*(F\\d{4})");
    QRegExp rxMacro("^(DD\\d{1,5})\\*F([a-zA-Z0-9'_#]+)");
    QRegExp rxPin("^\\s(IN|OUT|INOUT)\\d+:\\s([A-Z0-9'_#]+)");
    QSqlQuery query;

    QSqlQuery id_query;
    id_query.prepare("SELECT fileId FROM sourceData WHERE projectName = :name AND fileName = :fileName LIMIT 1");
    id_query.bindValue(":name", projectName);
    id_query.bindValue(":fileName", fileName);
    id_query.exec();
    quint16 fileId;
    while(id_query.next()){
        fileId = id_query.value(0).toInt();
    }

    QSqlQuery getInputFile;
    QString coreInputAbsFilePath;
    getInputFile.prepare("SELECT coreInputFile FROM projects WHERE projectName = :name");
    //qDebug()<<getInputFile.lastQuery();
    getInputFile.bindValue(":name", projectName);
    getInputFile.exec();
    while(getInputFile.next()){
        coreInputAbsFilePath = getInputFile.value(0).toString();
    }

    QString macroMelName = "0";
    if(coreInputAbsFilePath != fileName){
        QFileInfo inputFile(fileName);
        QString fileBaseName = inputFile.baseName();
        //qDebug()<<fileBaseName<<"base name";
        QSqlQuery getMacroMelName;
        getMacroMelName.prepare("SELECT unitNets.melUnitName FROM unitNets, sourceData WHERE unitNets.melUnitType = 1 AND "
                             "unitNets.fileId = sourceData.fileId AND unitNets.fortUnitName = :fort AND "
                             "sourceData.projectName = :projectName AND sourceData.fileName = :fileName");
        getMacroMelName.bindValue(":projectName", projectName);
        getMacroMelName.bindValue(":fileName", coreInputAbsFilePath);
        getMacroMelName.bindValue(":fort",fileBaseName);
        getMacroMelName.exec();

        if( getMacroMelName.isSelect() ){
            getMacroMelName.first();
        }
        if(getMacroMelName.isValid()){
             macroMelName = getMacroMelName.value(0).toString();
           }

    }

    do {
        line = in.readLine();
        if (rxUnit.indexIn(line) != -1){
            melUnitDDNum = rxUnit.cap(1);
            fortUnitNum = rxUnit.cap(2);
            melUnitFName[macroMelName].insert(melUnitDDNum,fortUnitNum);
            query.prepare("INSERT INTO unitNets (fileId, melUnitName, fortUnitName, melUnitType, macroUnitName) "
                          "VALUES (:id, :mel, :fort, 0, :macro)");
            query.bindValue(":id", fileId);
            query.bindValue(":mel", melUnitDDNum);
            query.bindValue(":fort", fortUnitNum);
            query.bindValue(":macro", macroMelName);
            query.exec();


        }else if(rxMacro.indexIn(line) != -1){
            qDebug()<<rxMacro.cap(1)<<rxMacro.cap(2)<<" :macro";
            query.prepare("INSERT INTO unitNets (fileId, melUnitName, fortUnitName, melUnitType, macroUnitName) "
                          "VALUES (:id, :mel, :fort, 1, 0)");
            query.bindValue(":id", fileId);
            query.bindValue(":mel", rxMacro.cap(1));
            query.bindValue(":fort", rxMacro.cap(2));
            query.exec();

            }
            else if((rxPin.indexIn(line) != -1) && (melUnitDDNum != 0)){
                melUnitPinName[macroMelName][melUnitDDNum].append(rxPin.cap(2));
                melUnitPinType[macroMelName][melUnitDDNum].append(rxPin.cap(1));

                QSqlQuery query2;
                query2.prepare("SELECT unitNets.unitId FROM unitNets, sourceData WHERE "
                               "unitNets.melUnitName = :name AND unitNets.fileId = sourceData.fileId "
                               "AND sourceData.projectName = :projectName AND sourceData.fileName = :fileName");
                query2.bindValue(":name", melUnitDDNum);
                query2.bindValue(":projectName", projectName);
                query2.bindValue(":fileName", fileName);
                query2.exec();

                query2.first();
                //qDebug()<<query2.executedQuery()<<query2.boundValues()<<melUnitDDNum<<projectName<<fileName;

                query.prepare("SELECT COUNT(*) FROM unitPorts WHERE unitId = :id");
                query.bindValue(":id",query2.value(0));
                query.exec();
                query.first();
                if(dbUnitPinsCnt.value(fortUnitNum) > query.value(0).toInt()){
                    query.prepare("INSERT INTO unitPorts (unitId, portType, portName) "
                                  "VALUES (:id, :type, :name)");
                    query.bindValue(":id", query2.value(0));
                    query.bindValue(":type", rxPin.cap(1));
                    query.bindValue(":name", rxPin.cap(2));
                    query.exec();
                    qDebug()<<query.executedQuery()<<query.boundValues();
                }

                }

    } while (!line.isNull());


#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    //textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    //setCurrentFile(fileName);
    statusBar()->showMessage(tr("Loading in progress..."), 2000);

    ui->graphicsView->update();
}


void MainWindow::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    //qDebug()<<"emmited doubleclick";
    melUnitChosen = item->text();
}


void MainWindow::on_basicCell_oversizedCell(QString cellMelName, quint32 cellPlace, QPoint cellPoint, QString melMacro)
{
    QString str = melUnitFName.value(melMacro).value(cellMelName);
//    qDebug()<<"melUnitFName[melUnitChosenMacro]: "<<melUnitFName.value(melMacro);
//    qDebug()<<"Note: oversized: "<<cellMelName<<" macro "<<melMacro<<" here; Type of element is: "<<str<<" ;";
    if(!cell[cellPoint.x()+1][cellPoint.y()]->isActive()){
        cell[cellPoint.x()+1][cellPoint.y()]->setParams(cellMelName,
                                                        str,
                                                        dbUnitPinsCnt.value(str),
                                                        dbUnitCellCnt.value(str),
                                                        cellPlace,
                                                        dbUnitPinsInfo.value(str),
                                                        melUnitPinName.value(melMacro).value(cellMelName),
                                                        melUnitPinType.value(melMacro).value(cellMelName),
                                                        melMacro);
        cell[cellPoint.x()+1][cellPoint.y()]->update();
        cell[cellPoint.x()+1][cellPoint.y()]->setActive(true);

        QVector<QString> temp;
        temp = grid[cellPoint.x()+1][cellPoint.y()]->getNets();
        //qDebug()<<"here2"<<dbUnitPinsInfo.value(fName).at(1+0).toInt()-1<<melUnitPinName.value(melUnitChosenMacro).value(melUnitChosen);
        quint32 pinNum = dbUnitPinsInfo.value(str).at(cellPlace*9+1+3).toInt();
        if(pinNum != 0 && pinNum != 65535){
            temp[3] = melUnitPinName.value(melMacro).value(cellMelName).at(pinNum-1);
        }
        pinNum = dbUnitPinsInfo.value(str).at(cellPlace*9+1+0).toInt();
        if(pinNum != 0 && pinNum != 65535){
            temp[1] = melUnitPinName.value(melMacro).value(cellMelName).at(pinNum-1);
        }
        grid[cellPoint.x()+1][cellPoint.y()]->setNets(temp);

        //qDebug()<<"here3";
        temp.clear();
        temp = grid[cellPoint.x()+1][cellPoint.y()+1]->getNets();
        pinNum = dbUnitPinsInfo.value(str).at(cellPlace*9+1+0).toInt();
        if(pinNum != 0 && pinNum != 65535){
            temp[0] = melUnitPinName.value(melMacro).value(cellMelName).at(pinNum-1);
        }
        pinNum = dbUnitPinsInfo.value(str).at(cellPlace*9+1+3*2).toInt();
        if(pinNum != 0 && pinNum != 65535){
            temp[2] = melUnitPinName.value(melMacro).value(cellMelName).at(pinNum-1);
        }
        grid[cellPoint.x()+1][cellPoint.y()+1]->setNets(temp);


    }else{
        //qDebug()<<"Why are you trying to set cell, which already active?";
    }
    //qDebug()<<"Oversize response"<<cellMelName<<str<<dbUnitCellCnt.value(str.toInt())<<cellPlace;

}

void MainWindow::reactOnTestRoute(int Key)
{
    qDebug()<<"Really react";
    quint8 tmp2 = 0;
    QVector< QVector<quint8> > map;
    for (quint8 j = 0; j<=3; j++){
        QVector<quint8> tmp;
        for(quint8 i = 0; i<=7; i++){
            tmp2 += 1;
            tmp.push_back(1<<NONE);
        }
        map.push_back(tmp);
    }
    cell[0][0]->setMap(map);
}


void MainWindow::reactOnNodeSelecting()
{

}

void MainWindow::newProjectNameCommitted()
{
    projectName = projectMenu->getText();
    ui->sessionLabel->setText(projectName);
    iniFileName = projectMenu->getIni();

    ui->MenuLevelLabel->setText(projectMenu->getFileName());

    QFileInfo info(projectMenu->getFileName());
    QString path;
    path = info.absolutePath();
    path.append("/");
    iniFileName.prepend(path);

    QSqlQuery query;
    query.prepare("INSERT INTO projects (projectName, coreInputFile, iniPath) "
                  "VALUES (:name, :file, :ini)");
    query.bindValue(":name", projectName);
    query.bindValue(":file", projectMenu->getFileName());
    query.bindValue(":ini", iniFileName);
    bool res = query.exec();
    if(!res){
        qDebug()<<"Error: "<<query.lastError().text();
    }

    query.prepare("INSERT INTO sourceData (projectName, fileName) "
                  "VALUES (:name, :file)");
    query.bindValue(":name", projectName);
    query.bindValue(":file", projectMenu->getFileName());
    res = query.exec();
    if(!res){
        qDebug()<<"Error: "<<query.lastError().text();
    }


    loadFileIni(iniFileName);
    loadFileMel(projectMenu->getFileName());

    query.prepare("SELECT unitNets.fortUnitName FROM unitNets, sourceData, projects WHERE "
                  "unitNets.melUnitType = 1 AND unitNets.fileId = sourceData.fileId AND "
                  "sourceData.projectName = projects.projectName AND sourceData.fileName = :file AND "
                  "projects.projectName = :project");
    query.bindValue(":file", projectMenu->getFileName());
    query.bindValue(":project", projectName);
    query.exec();

    QSqlQuery query2;
    while(query.next()){
        //qDebug()<<query.value(0).toString()<<" query";
        path = info.absolutePath();
        //qDebug()<<info.absolutePath()<<" :path";
        path.append("/");
        path.append(query.value(0).toString());
        path.append(".mel");
        query2.prepare("INSERT INTO sourceData (projectName, fileName, fileType) "
                       "VALUES (:name, :file, :type)");
        query2.bindValue(":name", projectName);
        query2.bindValue(":file", path);
        query2.bindValue(":type", "macro");
        query2.exec();
        loadFileMel(path);
    }

    QCoreApplication::setApplicationName(projectName);
    projectMenu->close();
    fillInfoTable();
    drawTable();
}

void MainWindow::on_projectMenu_checkProjectName(QString text)
{
    QSqlQuery query;
    bool isFileNameCorrect = true;
    query.prepare("SELECT projectName FROM projects");
    query.exec();
    while (query.next()) {
        if(query.value(0).toString() == projectMenu->getText()){
            isFileNameCorrect = false;
        }
    }
    emit fileNameCheckingResult(isFileNameCorrect);
}

void MainWindow::on_openProjectWidget_openProject(QString project)
{
    scene->clear();
    melUnitFName.clear();
    melUnitPinName.clear();
    melUnitPinType.clear();
    square.clear();
    cell.clear();
    grid.clear();
    buffer.clear();
    bGrid.clear();

    QSqlQuery query;
    QString input;
    query.prepare("SELECT coreInputFile FROM projects WHERE projectName = :name");
    query.bindValue(":name", project);
    query.exec();
    while (query.next()) {
        input = query.value(0).toString();
    }
    projectName = project;

    QSqlQuery query2;
    query2.prepare("SELECT iniPath FROM projects WHERE projectName = :name");
    query2.bindValue(":name", project);
    query2.exec();
    while(query2.next()){
        loadFileIni(query2.value(0).toString());
    }
    //loadFileIni("D:\\Qt\\HRT500.INI");
    loadFileMel(input);
    QFileInfo info(input);
    QString path;
    query.prepare("SELECT unitNets.fortUnitName FROM unitNets, sourceData, projects WHERE "
                  "unitNets.melUnitType = 1 AND unitNets.fileId = sourceData.fileId AND "
                  "sourceData.projectName = projects.projectName AND sourceData.fileName = :file AND "
                  "projects.projectName = :project");
    query.bindValue(":file", input);
    query.bindValue(":project", project);
    query.exec();

    while(query.next()){
        //qDebug()<<query.value(0).toString()<<" query";
        path = info.absolutePath();
        //qDebug()<<info.absolutePath()<<" :path";
        path.append("/");
        path.append(query.value(0).toString());
        path.append(".mel");
        loadFileMel(path);
    }
    openProjectWidget->close();
    drawTable();
    fillInfoTable();
}

void MainWindow::on_wiringAction()
{

}

QString MainWindow::pointsDetection()
{
    QString type = "";   /* grid, cell, buffer, bufGrid, addGrid */
    QPoint point2 = ui->graphicsView->mapFromGlobal(QCursor::pos());//Находим глобальную позицию курсора
    QPointF point = ui->graphicsView->mapToScene(point2);           //Маппим позицию курсора на сцену
    quint32 tmpx = 1 + point.x()/SIDE;
    quint32 tmpy = 1 + TABLE_HEIGTH - point.y() / SIDE;

    coursorX = point.x() / SIDE;
    coursorY = point.y() / SIDE;


    /* Boundary check */
    if (coursorX > TABLE_WIDTH){
        coursorX = TABLE_WIDTH;
    }
    if(coursorY > TABLE_HEIGTH){
        coursorY = TABLE_HEIGTH;
    }
    if(coursorX < 0){
        coursorX = 0;
    }
    if(coursorY < 0){
        coursorY = 0;
    }
    /* End of boundary check */

    lastPoint.setX(coursorX);
    lastPoint.setY(coursorY);

    if(tmpx<1 || tmpx>TABLE_WIDTH || tmpy<1 || tmpy>TABLE_HEIGTH){
        ui->ColumnLabel->setText("Out of scene");
        ui->RowLabel->setText("Out of scene");
    }else{
        ui->ColumnLabel->setText(QString::number(tmpx));
        ui->RowLabel->setText(QString::number(tmpy));
    }

    QString area = "";

    QString infoName = "";
    QString infoPos = "";
    QString infoMacro = "";
    QString infoNames = "";
    QString infoTypes = "";

    //Check if it in buffer area
    //Проверка простая: берем из хэша типа (типа буфера; координаты буферов такого типа по 4 на буфер) собственно
    //координаты и проверяем, находится ли наш курсор внутри оных.
    //Если нет, то переходим дальше.
    if((coursorX <= 10 || coursorX >= TABLE_WIDTH - 10) || (coursorY <= 10 || coursorY >= TABLE_HEIGTH - 10)){
        if ( coursorY <= 10){
            for (quint32 j = 0; j< buffer.at(HUB).size(); j++){
                if(buffer.at(HUB).at(j)->isUnderMouse()){
                    type = "buffer";
                    bufferInfo.clear();
                    bufferInfo.append(HUB);
                    bufferInfo.append(j);
                    infoName = "Buffer cell under mouse!";

                    infoPos = QString::number(buffer.at(HUB).at(j)->getNumber());
                    infoPos.append(" at ");
                    infoPos.append(QString::number(buffer.at(HUB).at(j)->getOrientation()));
                }
            }
        }else if ( coursorY >= TABLE_HEIGTH - 10 ){
            for (quint32 j = 0; j< buffer.at(HDB).size(); j++){
                if(buffer.at(HDB).at(j)->isUnderMouse()){
                    bufferInfo.clear();
                    bufferInfo.append(HDB);
                    bufferInfo.append(j);
                    infoName = "Buffer cell under mouse!";

                    infoPos = QString::number(buffer.at(HDB).at(j)->getNumber());
                    infoPos.append(" at ");
                    infoPos.append(QString::number(buffer.at(HDB).at(j)->getOrientation()));
                }
            }
        }

        if ( coursorX >= TABLE_WIDTH - 10 ){
            for (quint32 j = 0; j< buffer.at(VRB).size(); j++){
                if(buffer.at(VRB).at(j)->isUnderMouse()){
                    type = "buffer";
                    bufferInfo.clear();
                    bufferInfo.append(VRB);
                    bufferInfo.append(j);
                    infoName = "Buffer cell under mouse!";

                    infoPos = QString::number(buffer.at(VRB).at(j)->getNumber());
                    infoPos.append(" at ");
                    infoPos.append(QString::number(buffer.at(VRB).at(j)->getOrientation()));
                }
            }
        }else if ( coursorX <= 10 ){
            for (quint32 j = 0; j< buffer.at(VLB).size(); j++){
                if(buffer.at(VLB).at(j)->isUnderMouse()){
                    type = "buffer";
                    bufferInfo.clear();
                    bufferInfo.append(VLB);
                    bufferInfo.append(j);
                    infoName = "Buffer cell under mouse!";

                    infoPos = QString::number(buffer.at(VLB).at(j)->getNumber());
                    infoPos.append(" at ");
                    infoPos.append(QString::number(buffer.at(VLB).at(j)->getOrientation()));
                }
            }
        }
        if(type == "buffer"){
            ui->unitNameLabel->setText(infoName);
            ui->unitPosLabel->setText(infoPos);
        }
    }

    //Show information
    ui->typeLabel->setText(area);
    area = "";
    //qDebug()<<"Type: "<<unitType<<";x y: "<<unitX<<unitY<<";x y in: "<<xInUnit<<yInUnit<<";coursor x y: "<<coursorX<<coursorY;


    //Заполнение информации о размещенных юнитах элементах трассировочной решетки
    if(lastPoint.y() >= verPeriodicBegin && lastPoint.x() >= horPeriodicBegin){
        quint32 gCoordX = (lastPoint.x() - horPeriodicBegin + 1)/unitWidth;
        quint32 gCoordY = (lastPoint.y() - verPeriodicBegin + 1)/(cellHeigth + traceChannel);//8+6

        //qDebug()<<"X-Y: "<<gCoordX<<"-"<<gCoordY;
        if(gCoordX < unitsInRow && gCoordY < unitsInColumn+1){
            QString gridInfoPos = "";
            QString gridInfoOwner = "";
            QString gridInfoName = "";
            QString gridInfoNet = "";
            QString gridInfoMetal = "";
            QVector< QVector<quint8> > traceMetal;

            gridInfoNet.append(QString::number(gCoordX));
            gridInfoNet.append("x");
            gridInfoNet.append(QString::number(gCoordY));


            if(grid[gCoordX][gCoordY]->isUnderMouse()){
                type = "grid";
                //grid[gCoordX][gCoordY]->setCursor(Qt::CrossCursor);
                gridInfoPos.append(QString::number(grid[gCoordX][gCoordY]->getColumn()));
                gridInfoPos.append("x");
                gridInfoPos.append(QString::number(grid[gCoordX][gCoordY]->getRow()));

                traceMetal = grid[gCoordX][gCoordY]->getMetal();
                &gridInfoMetal << traceMetal;

                for (qint32 i = 0; i < grid[gCoordX][gCoordY]->getNets().size(); ++i) {
                    gridInfoName.append(grid[gCoordX][gCoordY]->getNets().at(i));
                    if (i != grid[gCoordX][gCoordY]->getNets().size() - 1){
                        gridInfoName.append(", ");
                    }
                }

                //&gridInfoName << grid[gCoordX][gCoordY]->getNets();

            }else{
                gridInfoPos = "";
                gridInfoOwner = "";
                gridInfoName = "";
                //gridInfoNet = "";
                gridInfoMetal = "";
            }
            ui->gridPosLabel->setText(gridInfoPos);
            ui->pinOwnerLabel->setText(gridInfoOwner);
            ui->pinNameLabel->setText(gridInfoName);
            ui->pinNetLabel->setText(gridInfoNet);
            ui->pinMetalLabel->setText(gridInfoMetal);
        }
    }

    //Заполнение информации о размещенных юнитах.
    if(lastPoint.y() >= verPeriodicBegin+traceChannel && lastPoint.x() >= horPeriodicBegin && lastPoint.y() <= TABLE_HEIGTH - 10){
        quint32 coordX = (lastPoint.x() - horPeriodicBegin + 1)/unitWidth;
        quint32 coordY = (lastPoint.y() - verPeriodicBegin)/(cellHeigth + traceChannel);//8+6
        if(coordX < unitsInRow && coordY < unitsInColumn){
            QVector<QString> melPinsInfo;
            QVector<QString> melPinsType;
            if(cell[coordX][coordY]->isUnderMouse()){
                type = "cell";
                infoPos.append(QString::number(cell[coordX][coordY]->getColumn()));
                infoPos.append("x");
                infoPos.append(QString::number(cell[coordX][coordY]->getRow()));

                if(cell[coordX][coordY]->isActive()){
                    infoPos.append(", ");
                    infoPos.append(QString::number(cell[coordX][coordY]->getSize()));

                    infoName.append(cell[coordX][coordY]->getName());
                    infoName.append(", ");
                    infoName.append(cell[coordX][coordY]->getDBName());

                    infoMacro = cell[coordX][coordY]->getMacro();

                    melPinsInfo = cell[coordX][coordY]->getMelPinsInfo();
                    for (qint32 i = 0; i < melPinsInfo.size(); ++i) {
                        infoNames.append(melPinsInfo.at(i));
                        if (i != melPinsInfo.size() - 1){
                            infoNames.append(", ");
                        }
                    }

                    melPinsType = cell[coordX][coordY]->getMelPinsType();
                    for (qint32 i = 0; i < melPinsType.size(); ++i) {
                        infoTypes.append(melPinsType.at(i));
                        if (i != melPinsType.size() - 1){
                            infoTypes.append(", ");
                        }
                    }
                }
            }else{
                infoName = "";
                infoPos = "";
                infoMacro = "";
                infoNames = "";
                infoTypes = "";
            }
            ui->unitNameLabel->setText(infoName);
            ui->unitPosLabel->setText(infoPos);
            ui->unitMacroLabel->setText(infoMacro);
            ui->unitPinNamesLabel->setText(infoNames);
            ui->unitPinTypesLabel->setText(infoTypes);
        }
    }
    return type;
}



void MainWindow::on_myScene_unitPlacing()
{
    /* Check if this unit not placed already */
    QSqlQuery query;
    query.prepare("SELECT A.isPlaced FROM unitNets A, sourceData B, projects C "
                  "WHERE A.fileId = B.fileId AND C.projectName = :projName "
                  "AND A.melUnitName = :name AND A.macroUnitName = :mname "
                  "AND C.projectName = B.projectName");
    query.bindValue(":name", melUnitChosen);
    query.bindValue(":projName", projectName);
    query.bindValue(":mname", melUnitChosenMacro);
    query.exec();
    qint8 isPlaced;
    while (query.next()) {
        isPlaced = query.value(0).toInt();
    }
    if ( isPlaced == 1 ){
        qDebug() << isPlaced << "Error: Cant place unit, cause it's already placed;";
        return;
    }
    /* End of check */


    /* Check if this unit exists in database */
    query.prepare("SELECT COUNT(*) FROM unitNets A, sourceData B, projects C "
                  "WHERE A.fileId = B.fileId AND C.projectName = :projName "
                  "AND A.melUnitName = :name AND A.macroUnitName = :mname "
                  "AND C.projectName = B.projectName");
    query.bindValue(":name", melUnitChosen);
    query.bindValue(":projName", projectName);
    query.bindValue(":mname", melUnitChosenMacro);
    //qDebug()<<"Unit "<<melUnitChosen<<" from project `"<<projectName<<"`, with parent: "<<melUnitChosenMacro<<" chosen for placement;";
    query.exec();
    quint32 existance;
    while (query.next()) {
        existance = query.value(0).toInt();

    }
    if ( existance != 1 ){
        qDebug() << existance << "Error: Cant place unit, which is not exist in mysql db;";
        return;
    }
    /* End of check */

    if (pointsDetection() == "buffer"){
        bool cellsFreedomFlag = true;
        QString fName;
        fName = melUnitFName.value(melUnitChosenMacro).value(melUnitChosen);
        quint32 bufLine = bufferInfo.at(0);
        quint32 bufPos = bufferInfo.at(1);
        if(buffer.at(bufLine).at(bufPos)->isActive()){
                        cellsFreedomFlag = false;
                        qDebug()<<"NOTE: This buffer cell is already occupied, try other free cell;";
        }
        if (dbUnitType.value(fName) != 1){
            cellsFreedomFlag = false;
            qDebug()<<"NOTE: Unit "<<fName<<" must be placed in matrix cell, not in buffer;";
        }
        if(cellsFreedomFlag){
            qDebug()<<"NOTE: Cells are free, so we can place unit "<<melUnitChosen<<" macro "<<melUnitChosenMacro<<" here; Type of element is: "<<fName<<" ;";
            qDebug()<<melUnitPinType.value(melUnitChosenMacro).value(melUnitChosen);
            qDebug()<<bufLine<<bufPos;
            buffer.at(bufLine).at(bufPos)->setParams(melUnitChosen,
                                            fName,
                                            dbUnitPinsCnt.value(fName),
                                            dbUnitPinsInfo.value(fName),
                                            melUnitPinName.value(melUnitChosenMacro).value(melUnitChosen),
                                            melUnitPinType.value(melUnitChosenMacro).value(melUnitChosen),
                                            melUnitChosenMacro);

            buffer.at(bufLine).at(bufPos)->setActive(true);
            buffer.at(bufLine).at(bufPos)->update();
qDebug()<<"hhhh";
            QSqlQuery query4;
            query4.prepare("SELECT A.unitId FROM unitNets A, sourceData B, projects C "
                          "WHERE A.fileId = B.fileId AND C.projectName = :projName "
                          "AND A.macroUnitName = :mname AND A.melUnitName = :name "
                          "AND C.projectName = B.projectName");
            query4.bindValue(":name", melUnitChosen);
            query4.bindValue(":projName", projectName);
            query4.bindValue(":mname", melUnitChosenMacro);
            query4.exec();
            quint32 unitId;
            while (query4.next()) {
                unitId = query4.value(0).toInt();
            }


            QSqlQuery query2;
            query2.prepare("UPDATE unitNets SET isPlaced = 1 WHERE unitId = :id");
            query2.bindValue(":id", unitId);
            query2.exec();

            fillInfoTable();

            QSqlQuery query3;
            query3.prepare("INSERT INTO unitPlacement (unitId, unitRow, unitColumn) "
                           "VALUES (:id,:row,:col)");
            query3.bindValue(":id", unitId);
            query3.bindValue(":row", bufLine);
            query3.bindValue(":col", bufPos);
            query3.exec();
        }
    }

    if (pointsDetection() == "cell"){
        quint32 coordX = (lastPoint.x() - horPeriodicBegin + 1)/unitWidth;
        quint32 coordY = (lastPoint.y() - verPeriodicBegin)/(cellHeigth + traceChannel);//8+6
        bool cellsFreedomFlag = true;
        QString fName;
        /*
        QSqlQuery getMacroFortName;
        getMacroFortName.prepare("SELECT unitNets.fortUnitName FROM unitNets, sourceData, projects "
                                 "WHERE unitNets.melUnitName = :mel AND sourceData.projectName = :project "
                                 "AND   sourceData.fileName = projects.fileName AND unitNets.fileId = sourceData.fileId");
        getMacroFortName.bindValue(":mel", melUnitChosenMacro);
        getMacroFortName.bindValue(":project", projectName);
        getMacroFortName.exec();
        getMacroFortName.first();
        QString macroFortName = getMacroFortName.value(0).toString();
        */
        /*
        QSqlQuery query;
        query.prepare("SELECT A.fortUnitName FROM unitNets A, sourceData B, projects C "
                      "WHERE A.fileId = B.fileId AND C.projectName = :projName "
                      "AND A.melUnitName = :name AND A.macroUnitName = :mname "
                      "AND C.projectName = B.projectName");
        query.bindValue(":name", melUnitChosen);
        query.bindValue(":projName", projectName);
        query.bindValue(":mname", melUnitChosenMacro);
        query.exec();
        query.first();
        fName = query.value(0).toString();
        */
        fName = melUnitFName.value(melUnitChosenMacro).value(melUnitChosen);
        //qDebug()<<"fname: "<<fName;

        if(coordX + dbUnitCellCnt.value(fName) - 1 < unitsInRow){//Maybe we just have not enough cells to place this unit?
            for (quint16 i = 0; i< dbUnitCellCnt.value(fName); i++){
                    if (cell.at(coordX+i).at(coordY)->isActive()){
                        cellsFreedomFlag = false;
                        qDebug()<<"NOTE: Some cell are already occupied, try other free cell;";
                }
            }
        }else{
            cellsFreedomFlag = false;
            qDebug()<<"NOTE: Unit "<<fName<<" requires more cells than available;";
        }
        if (dbUnitType.value(fName) == 1){
            cellsFreedomFlag = false;
            qDebug()<<"NOTE: Unit "<<fName<<" must be placed in buffer cell, not in the matrix;";
        }
        if(cellsFreedomFlag){
            //qDebug()<<"NOTE: Cells are free, so we can place unit "<<melUnitChosen<<" macro "<<melUnitChosenMacro<<" here; Type of element is: "<<fName<<" ;";
            //qDebug()<<melUnitPinType.value(melUnitChosenMacro).value(melUnitChosen);
            cell[coordX][coordY]->setParams(melUnitChosen,
                                            fName,
                                            dbUnitPinsCnt.value(fName),
                                            dbUnitCellCnt.value(fName),
                                            0,
                                            dbUnitPinsInfo.value(fName),
                                            melUnitPinName.value(melUnitChosenMacro).value(melUnitChosen),
                                            melUnitPinType.value(melUnitChosenMacro).value(melUnitChosen),
                                            melUnitChosenMacro);

            cell[coordX][coordY]->setActive(true);
            cell[coordX][coordY]->update();

            QSqlQuery query4;
            query4.prepare("SELECT A.unitId FROM unitNets A, sourceData B, projects C "
                          "WHERE A.fileId = B.fileId AND C.projectName = :projName "
                          "AND A.macroUnitName = :mname AND A.melUnitName = :name "
                          "AND C.projectName = B.projectName");
            query4.bindValue(":name", melUnitChosen);
            query4.bindValue(":projName", projectName);
            query4.bindValue(":mname", melUnitChosenMacro);
            query4.exec();
            quint32 unitId;
            while (query4.next()) {
                unitId = query4.value(0).toInt();
                //qDebug() << unitId << "unitId for UPDATE";
            }


            QSqlQuery query2;
            query2.prepare("UPDATE unitNets SET isPlaced = 1 WHERE unitId = :id");
            query2.bindValue(":id", unitId);
            query2.exec();


            //######################Refresh tableView
            fillInfoTable();
            //#################end of refreshing


            QSqlQuery query3;
            query3.prepare("INSERT INTO unitPlacement (unitId, unitRow, unitColumn) "
                           "VALUES (:id,:row,:col)");
            query3.bindValue(":id", unitId);
            query3.bindValue(":row", coordY);
            query3.bindValue(":col", coordX);
            query3.exec();


            /* Some code to assign to Si+ names of nets, which belongs to corresponding unit we are placing now */
            QVector<QString> temp = grid[coordX][coordY]->getNets();
            quint32 pinNum;

            pinNum = dbUnitPinsInfo.value(fName).at(1+3).toInt();
            if(pinNum != 0 && pinNum != 65535)
                temp[3] = melUnitPinName.value(melUnitChosenMacro).value(melUnitChosen).at(pinNum-1);

            pinNum = dbUnitPinsInfo.value(fName).at(1+0).toInt();
            if(pinNum != 0 && pinNum != 65535)
                temp[1] = melUnitPinName.value(melUnitChosenMacro).value(melUnitChosen).at(pinNum-1);

            grid[coordX][coordY]->setNets(temp);
            temp.clear();
            temp = grid[coordX][coordY+1]->getNets();

            pinNum = dbUnitPinsInfo.value(fName).at(1+0).toInt();
            if(pinNum != 0 && pinNum != 65535)
                temp[0] = melUnitPinName.value(melUnitChosenMacro).value(melUnitChosen).at(pinNum-1);

            pinNum = dbUnitPinsInfo.value(fName).at(1+3*2).toInt();
            if(pinNum != 0 && pinNum != 65535)
                temp[2] = melUnitPinName.value(melUnitChosenMacro).value(melUnitChosen).at(pinNum-1);

            grid[coordX][coordY+1]->setNets(temp);

        }
    }
}

void MainWindow::on_myScene_initialUnitPlacing()//Now it's usable only for grid filling, not for buffers!
{
    quint32 coordX = (lastPoint.x() - horPeriodicBegin + 1)/unitWidth;
    quint32 coordY = (lastPoint.y() - verPeriodicBegin)/(cellHeigth + traceChannel);//8+6
    QString fName;

    QSqlQuery query;
    query.prepare("SELECT A.fortUnitName FROM unitNets A, sourceData B, projects C "
                  "WHERE A.fileId = B.fileId AND C.projectName = :projName "
                  "AND A.melUnitName = :name AND A.macroUnitName = :mname "
                  "AND C.projectName = B.projectName");
    query.bindValue(":name", melUnitChosen);
    query.bindValue(":projName", projectName);
    query.bindValue(":mname", melUnitChosenMacro);
    query.exec();
    query.first();
    fName = query.value(0).toString();

    cell.at(coordX).at(coordY)->setParams(melUnitChosen,
                                    fName,
                                    dbUnitPinsCnt.value(fName),
                                    dbUnitCellCnt.value(fName),
                                    0,
                                    dbUnitPinsInfo.value(fName),
                                    melUnitPinName.value(melUnitChosenMacro).value(melUnitChosen),
                                    melUnitPinType.value(melUnitChosenMacro).value(melUnitChosen),
                                    melUnitChosenMacro);

    cell.at(coordX).at(coordY)->setActive(true);
    cell.at(coordX).at(coordY)->update();

    /* Some code to assign to Si+ names of nets, which belongs to corresponding unit we are placing now */
    QVector<QString> temp = grid.at(coordX).at(coordY)->getNets();
    quint32 pinNum;

    pinNum = dbUnitPinsInfo.value(fName).at(1+3).toInt();
    if(pinNum != 0 && pinNum != 65535)
        temp[3] = melUnitPinName.value(melUnitChosenMacro).value(melUnitChosen).at(pinNum-1);

    pinNum = dbUnitPinsInfo.value(fName).at(1+0).toInt();
    if(pinNum != 0 && pinNum != 65535)
        temp[1] = melUnitPinName.value(melUnitChosenMacro).value(melUnitChosen).at(pinNum-1);

    grid.at(coordX).at(coordY)->setNets(temp);
    temp.clear();
    temp = grid.at(coordX).at(coordY+1)->getNets();

    pinNum = dbUnitPinsInfo.value(fName).at(1+0).toInt();
    if(pinNum != 0 && pinNum != 65535)
        temp[0] = melUnitPinName.value(melUnitChosenMacro).value(melUnitChosen).at(pinNum-1);

    pinNum = dbUnitPinsInfo.value(fName).at(1+3*2).toInt();
    if(pinNum != 0 && pinNum != 65535)
        temp[2] = melUnitPinName.value(melUnitChosenMacro).value(melUnitChosen).at(pinNum-1);

    grid.at(coordX).at(coordY+1)->setNets(temp);
}

void MainWindow::fillInfoTable()
{
    /*
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(melUnitFName.size());


    QHashIterator<QString, QString> i(melUnitFName);
    quint32 cnt = 0;
    while (i.hasNext()) {
        i.next();
        QTableWidgetItem *newItem = new QTableWidgetItem();
        newItem->setText(i.key());
        ui->tableWidget->setItem(cnt, 0, newItem);

        QTableWidgetItem *newItem2 = new QTableWidgetItem();
        newItem2->setText(i.value());
        newItem2->setFlags(Qt::NoItemFlags);
        ui->tableWidget->setItem(cnt, 1, newItem2);

        QTableWidgetItem *newItem3 = new QTableWidgetItem();
        QString str = "regular";
        if(dbUnitType.value(i.value()) == 1){
            str = "buffer";
        }
        newItem3->setText(str);
        newItem3->setFlags(Qt::NoItemFlags);
        ui->tableWidget->setItem(cnt, 2, newItem3);
        cnt++;
    }
    ui->tableWidget->sortByColumn(0,Qt::DescendingOrder);
    ui->tableWidget->setSortingEnabled(true);
    //ui->tableWidget->setHorizontalHeader();

    QStringList labels;
    labels << tr("Net") << tr("DB") << tr("Type");

    ui->tableWidget->setHorizontalHeaderLabels(labels);
    //ui->tableWidget->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
    //ui->tableWidget->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
    //ui->tableWidget->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableWidget->verticalHeader()->hide();



*/


    //#####################################################
    //QSqlQueryModel* model = new QSqlQueryModel;

    QSqlQuery query3;
    query3.prepare("SELECT sourceData.fileId FROM sourceData, projects WHERE projects.projectName = :name AND "
                   "sourceData.projectName = :name");
    query3.bindValue(":name", projectName);
    query3.exec();
    QVector<quint32> fileId;
    while(query3.next()){
        fileId.append(query3.value(0).toInt());
    }


    QSqlTableModel* model = new QSqlTableModel;
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setModel( model );
    model->setTable("unitNets");
    QString filterQuery = "isPlaced !=1 AND fileId IN ( ";
    for (int i = 0; i < fileId.size(); ++i) {
        filterQuery.append("'");
        filterQuery.append(QString::number(fileId.at(i)));
        filterQuery.append("'");
        if(i != fileId.size()-1){
            filterQuery.append(",");
        }else{
            filterQuery.append(")");
        }
    }
    //qDebug()<<filterQuery;
    model->setFilter(filterQuery);
    model->select();
    ui->tableView->setSortingEnabled(1);
    //ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    //ui->tableView->hideColumn(4);
    ui->tableView->hideColumn(6);
    ui->tableView->show();
}

void MainWindow::on_tableView_itemDoubleClicked(QModelIndex mi){
    melUnitChosen = ui->tableView->model()->data(mi).toString();

    QModelIndex index = ui->tableView->model()->index(mi.row(), mi.column()+3, QModelIndex());
    melUnitChosenMacro = ui->tableView->model()->data(index).toString();
    qDebug()<<melUnitChosenMacro<<melUnitChosen;
}

void MainWindow::adjustTableSize()
{
    ui->tableView->resizeColumnToContents(0);
    ui->tableView->resizeColumnToContents(1);

    QRect rect = ui->tableView->geometry();
    rect.setWidth(2 + ui->tableView->verticalHeader()->width() +
            ui->tableView->columnWidth(0) + ui->tableView->columnWidth(1));
    ui->tableView->setGeometry(rect);
}

void MainWindow::connectToDatabase(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //db.setHostName("localhost");
    //db.setPort(3306);
    db.setDatabaseName("D:\\Qt5.3\\HRT\\resources\\HRTk0.db3");
    //db.setUserName("root");
    //db.setPassword("123456");
    if (!(db.open())) {
        QString resErrorString=db.lastError().databaseText()+"\n"+db.lastError().driverText();
        qDebug()<<"DB not open with: "<<resErrorString;
    }
    QSqlQuery("PRAGMA page_size = 4096");
    QSqlQuery("PRAGMA cache_size = 16384");
    QSqlQuery("PRAGMA temp_store = MEMORY");
    QSqlQuery("PRAGMA journal_mode = OFF");
    //QSqlQuery("PRAGMA locking_mode = EXCLUSIVE");
    QSqlQuery("PRAGMA synchronous = OFF");


}
