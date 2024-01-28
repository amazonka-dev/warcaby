#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./piecewidget.h"
#include <QMessageBox>
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QGridLayout>
#include <QFrame>
#include <QLabel>
#include <QAction>
#include <vector>
#include "box.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->menuAbout, &QAction::triggered, this, &MainWindow::showAbout);
    for (int i = 0; i < 100; i++)
    {
        boardStatus[i] = STATUS_EMPTY;
        whitePieces[i] = nullptr;
        blackPieces[i] = nullptr;
    }

    //szachownica

    //rozmiar pól
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHeightForWidth(true);

    //stworzenie szachownicy 10x10
    for (int index = 0; index < 100; index++)
    {
        int row = getRow(index);
        int col = getColumn(index);
        Box *square = new Box(this, index);
        square->setSizePolicy(sizePolicy);
        if (isPlayable(index))
        {
            square->setStyleSheet("background-color: #753;");
        }
        else
        {
            square->setStyleSheet("background-color: #FFD;");
        }

        if (isPlayable(index))
        {
            // białe pionki
            PieceWidget *whitePiece = new PieceWidget(square, PieceWidget::WHITE);
            whitePiece->hide();
            QSizePolicy whiteSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            whiteSizePolicy.setHeightForWidth(true);
            whitePiece->setSizePolicy(whiteSizePolicy);
            whitePieces[index] = whitePiece;

            // czarne pionki
            PieceWidget *blackPiece = new PieceWidget(square, PieceWidget::BLACK);
            blackPiece->hide();
            QSizePolicy blackSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            blackSizePolicy.setHeightForWidth(true);
            blackPiece->setSizePolicy(blackSizePolicy);
            blackPieces[index] = blackPiece;
        }

        ui->board->addWidget(square, row, col);
    }

    updateWidgets();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, "O programie", "Warcaby, Ewa Śnieżyk-Milczyńska 2023");
}

void MainWindow::onFrameClicked()
{
    QMessageBox::information(this, "Informacja", "Kliknięto w ramkę!");
}

void MainWindow::on_buttonWhite_clicked()
{
    markedIndices.clear();
    for (int index = 0; index < 100; index++)
    {
        if (isPlayable(index))
        {
            int row = getRow(index);
            if (row == 0 || row == 1 || row == 2)
                boardStatus[index] = STATUS_BLACK;
            else if (row == 7 || row == 8 || row == 9)
                boardStatus[index] = STATUS_WHITE;
            else
                boardStatus[index] = STATUS_EMPTY;
        }
    }
    myColor = STATUS_WHITE;
    turn = HUMAN_TURN;
    updateWidgets();
}

void MainWindow::on_buttonBlack_clicked()
{
//    QMessageBox::about(this, "Gra", "Graj czarnymi");
    markedIndices.clear();
    for (int index = 0; index < 100; index++)
    {
        if (isPlayable(index))
        {
            int row = getRow(index);
            if (row == 0 || row == 1 || row == 2)
                boardStatus[index] = STATUS_WHITE;
            else if (row == 7 || row == 8 || row == 9)
                boardStatus[index] = STATUS_BLACK;
            else
                boardStatus[index] = STATUS_EMPTY;
        }
    }
    myColor = STATUS_BLACK;
    turn = COMPUTER_TURN;
    updateWidgets();
    doComputerTurn();
}

bool MainWindow::isPlayable(int index)
{
    int row = getRow(index);
    int col = getColumn(index);
    bool isBlack = (row + col) % 2 != 0;
    if (isBlack && index >= 0 && index < 100)
        return true;
    else
        return false;
}

void MainWindow::updateWidgets()
{
    for (int index = 0; index < 100; index++)
    {
        if (isPlayable(index))
        {
            auto it = std::find(markedIndices.begin(), markedIndices.end(), index);
            if (it != markedIndices.end())
            {
                whitePieces[index]->setMarked(true);
                blackPieces[index]->setMarked(true);
            }
            else
            {
                whitePieces[index]->setMarked(false);
                blackPieces[index]->setMarked(false);
            }
            switch (boardStatus[index])
            {
                case STATUS_EMPTY:
                    whitePieces[index]->hide();
                    blackPieces[index]->hide();
                    break;
                case STATUS_WHITE:
                    whitePieces[index]->show();
                    blackPieces[index]->hide();
                    break;
                case STATUS_BLACK:
                    whitePieces[index]->hide();
                    blackPieces[index]->show();
                    break;
            }
            whitePieces[index]->repaint();
            blackPieces[index]->repaint();
        }
    }
}

void MainWindow::onClickBox(int index)
{
    if (isPlayable(index))
    {
        if (markedIndices.empty())
        {
            // sprawdzenie, czy jest obowiązkowe bicie
            auto allHumanCaptures = getAllAvailableHumanCaptures();
            if (allHumanCaptures.empty())
            {
                // wybór pionka
                if (boardStatus[index] != STATUS_EMPTY && boardStatus[index] == myColor)
                {
                    markedIndices.push_back(index);
                    updateWidgets();
                }
            }
            else
            {
                // wybór pionka, jeśli może zbić przeciwnika
                auto hereCaptures = getValidHumanCaptures(index);
                if (hereCaptures.empty())
                {
                    // ten pionek nie może bić - nie zaznaczam pionka
                }
                else
                {
                    // wybór pionka
                    if (boardStatus[index] != STATUS_EMPTY && boardStatus[index] == myColor)
                    {
                        markedIndices.push_back(index);
                        updateWidgets();
                    }
                }
            }
        }
        else
        {
            auto last = markedIndices.rbegin();
            int sourceIndex = *last;
            if (sourceIndex == index)
            {
                // rezygnacja z ruchu
                markedIndices.clear();
                updateWidgets();
            }
            else
            {
                // wybór pola docelowego
                auto allHumanCaptures = getAllAvailableHumanCaptures();
                if (!allHumanCaptures.empty())
                {
                    // obowiązkowe bicie
                    int capt = getHumanCapturedIndex(sourceIndex, index);
                    if (capt >= 0)
                    {
                        // bicie
                        markedIndices.push_back(capt);
                        markedIndices.push_back(index);
                        captureMarkedPiece();
                        updateWidgets();
                        turn = COMPUTER_TURN;
                        doComputerTurn();
                    }
                }
                else
                {
                    if (isValidTarget(sourceIndex, index))
                    {
                        // ruch
                        markedIndices.push_back(index);
                        moveMarkedPiece();
                        updateWidgets();
                        turn = COMPUTER_TURN;
                        doComputerTurn();
                    }
                    else
                    {
                        int capt = getHumanCapturedIndex(sourceIndex, index);
                        if (capt >= 0)
                        {
                            // bicie
                            markedIndices.push_back(capt);
                            markedIndices.push_back(index);
                            captureMarkedPiece();
                            updateWidgets();
                            turn = COMPUTER_TURN;
                            doComputerTurn();
                        }
                    }
                }
            }
        }
    }
}

std::vector<int> MainWindow::getValidIndices(int sourceIndex)
{
    std::vector<int> validIndices;
    int i = sourceIndex;
    switch (turn)
    {
    case COMPUTER_TURN:
        i += 9;
        break;
    case HUMAN_TURN:
        i -= 9;
        break;
    case NO_TURN:
        break;
    }
    if (isPlayable(i) && boardStatus[i] == STATUS_EMPTY)
    {
        validIndices.push_back(i);
    }

    int j = sourceIndex;
    switch (turn)
    {
    case COMPUTER_TURN:
        j += 11;
        break;
    case HUMAN_TURN:
        j -= 11;
        break;
    case NO_TURN:
        break;
    }
    if (isPlayable(j) && boardStatus[j] == STATUS_EMPTY)
    {
        validIndices.push_back(j);
    }
    return validIndices;
}

std::vector<CapturedMovement> MainWindow::getValidHumanCaptures(int sourceIndex)
{
    std::vector<CapturedMovement> movements;
    int i1 = sourceIndex + 9;//indeks bitego pionka komputera lewy dolny
    int i2 = sourceIndex + 18; //indeks pustego za bitym pionkiem
    if (isPlayable(i1) && isComputerPiece(i1) && isPlayable(i2) && boardStatus[i2] == STATUS_EMPTY)
    {
        movements.push_back(CapturedMovement(sourceIndex, i1, i2));
    }

    i1 = sourceIndex - 9;
    i2 = sourceIndex - 18;
    if (isPlayable(i1) && isComputerPiece(i1) && isPlayable(i2) && boardStatus[i2] == STATUS_EMPTY)
    {
        movements.push_back(CapturedMovement(sourceIndex, i1, i2));
    }

    i1 = sourceIndex + 11;
    i2 = sourceIndex + 22;
    if (isPlayable(i1) && isComputerPiece(i1) && isPlayable(i2) && boardStatus[i2] == STATUS_EMPTY)
    {
        movements.push_back(CapturedMovement(sourceIndex, i1, i2));
    }

    i1 = sourceIndex - 11;
    i2 = sourceIndex - 22;
    if (isPlayable(i1) && isComputerPiece(i1) && isPlayable(i2) && boardStatus[i2] == STATUS_EMPTY)
    {
        movements.push_back(CapturedMovement(sourceIndex, i1, i2));
    }
    return movements;
}

std::vector<CapturedMovement> MainWindow::getValidComputerCaptures(int sourceIndex)
{
    std::vector<CapturedMovement> movements;
    int i1 = sourceIndex + 9;
    int i2 = sourceIndex + 18;
    if (isPlayable(i1) && isHumanPiece(i1) && isPlayable(i2) && boardStatus[i2] == STATUS_EMPTY)
    {
        movements.push_back(CapturedMovement(sourceIndex, i1, i2));
    }

    i1 = sourceIndex - 9;
    i2 = sourceIndex - 18;
    if (isPlayable(i1) && isHumanPiece(i1) && isPlayable(i2) && boardStatus[i2] == STATUS_EMPTY)
    {
        movements.push_back(CapturedMovement(sourceIndex, i1, i2));
    }

    i1 = sourceIndex + 11;
    i2 = sourceIndex + 22;
    if (isPlayable(i1) && isHumanPiece(i1) && isPlayable(i2) && boardStatus[i2] == STATUS_EMPTY)
    {
        movements.push_back(CapturedMovement(sourceIndex, i1, i2));
    }

    i1 = sourceIndex - 11;
    i2 = sourceIndex - 22;
    if (isPlayable(i1) && isHumanPiece(i1) && isPlayable(i2) && boardStatus[i2] == STATUS_EMPTY)
    {
        movements.push_back(CapturedMovement(sourceIndex, i1, i2));
    }
    return movements;
}

bool MainWindow::isValidTarget(int sourceIndex, int targetIndex)
{
    if (boardStatus[targetIndex] == STATUS_EMPTY)
    {
        std::vector<int> valid = getValidIndices(sourceIndex);
        auto it = std::find(valid.begin(), valid.end(), targetIndex);
        if (it != valid.end())
            return true;
    }

    return false;
}

int MainWindow::getHumanCapturedIndex(int sourceIndex, int targetIndex)
{
    std::vector<CapturedMovement> valid = getValidHumanCaptures(sourceIndex);
    for (auto m : valid)
    {
        if (m.targetIndex == targetIndex)
            return m.capturedIndex;
    }
    return -1;
}

void MainWindow::moveMarkedPiece()
{
    if (markedIndices.size() == 2)
    {
        int sourceIndex = markedIndices[0];
        SquareStatus sourceColor = boardStatus[sourceIndex];
        int targetIndex = markedIndices[1];

        boardStatus[sourceIndex] = STATUS_EMPTY;
        boardStatus[targetIndex] =  sourceColor;
        markedIndices.clear();
    }
}

void MainWindow::captureMarkedPiece()
{
    if (markedIndices.size() == 3)
    {
        int sourceIndex = markedIndices[0];
        SquareStatus sourceColor = boardStatus[sourceIndex];
        int capturedIndex = markedIndices[1];
        int targetIndex = markedIndices[2];

        boardStatus[sourceIndex] = STATUS_EMPTY;
        boardStatus[capturedIndex] = STATUS_EMPTY;
        boardStatus[targetIndex] = sourceColor;
        markedIndices.clear();
    }
}

std::vector<AvailableMovement> MainWindow::getAllAvailableComputerMovements()
{
    std::vector<AvailableMovement> movements;

    for (int index = 0; index <100; index++)
    {
        if (isPlayable(index) && isComputerPiece(index))
        {
            auto targetIndices = getValidIndices(index);
            for (auto target : targetIndices)
            {
                movements.push_back({index, target});
            }
        }
    }

    return movements;
}

std::vector<AvailableMovement> MainWindow::getAllAvailableHumanMovements()
{
    std::vector<AvailableMovement> movements;

    for (int index = 0; index <100; index++)
    {
        if (isPlayable(index) && isHumanPiece(index))
        {
            auto targetIndices = getValidIndices(index);
            for (auto target : targetIndices)
            {
                movements.push_back({index, target});
            }
        }
    }

    return movements;
}

std::vector<CapturedMovement> MainWindow::getAllAvailableHumanCaptures()
{
    std::vector<CapturedMovement> allCaptures;

    for (int index = 0; index <100; index++)
    {
        if (isPlayable(index) && isHumanPiece(index))
        {
            std::vector<CapturedMovement> captures = getValidHumanCaptures(index);
            for (auto capt : captures)
            {
                allCaptures.push_back(capt);
            }
        }
    }

    return allCaptures;
}

std::vector<CapturedMovement> MainWindow::getAllAvailableComputerCaptures()
{
    std::vector<CapturedMovement> allCaptures;

    for (int index = 0; index <100; index++)
    {
        if (isPlayable(index) && isComputerPiece(index))
        {
            std::vector<CapturedMovement> captures = getValidComputerCaptures(index);
            for (auto capt : captures)
            {
                allCaptures.push_back(capt);
            }
        }
    }

    return allCaptures;
}

void MainWindow::doComputerTurn()
{
    // najpierw bicie
    std::vector<CapturedMovement> captures = getAllAvailableComputerCaptures();
    if (!captures.empty())
    {
        CapturedMovement capt = captures[0];
        markedIndices.push_back(capt.sourceIndex);
        markedIndices.push_back(capt.capturedIndex);
        markedIndices.push_back(capt.targetIndex);
        captureMarkedPiece();
        updateWidgets();
        turn = HUMAN_TURN;

        std::vector<AvailableMovement> humanMovements = getAllAvailableHumanMovements();
        if (humanMovements.empty())
        {
            //gracz nie może się ruszać
            turn = NO_TURN;
            QMessageBox::about(this, "Gra", "Przegrałeś!");
        }
        return;
    }

    std::vector<AvailableMovement> currentMovements = getAllAvailableComputerMovements();
    if (currentMovements.empty())
    {
        //komunikat koniec gry
        turn = NO_TURN;
        QMessageBox::about(this, "Gra", "Wygrałeś!");
    }
    else
    {
        //komputer wykonuje losowy ruch spośród dostępnych
        int n = std::rand() % currentMovements.size();
        auto mov = currentMovements[n];

        boardStatus[mov.sourceIndex] = STATUS_EMPTY;
        if (myColor == STATUS_BLACK)
            boardStatus[mov.targetIndex] = STATUS_WHITE;
        else
            boardStatus[mov.targetIndex] = STATUS_BLACK;
        updateWidgets();
        turn = HUMAN_TURN;

        std::vector<AvailableMovement> currentMovements = getAllAvailableHumanMovements();
        if (currentMovements.empty())
        {
            //komunikat koniec gry
            turn = NO_TURN;
            QMessageBox::about(this, "Gra", "Przegrałeś!");
        }
    }
}



bool MainWindow::isComputerPiece(int index)
{
    if (boardStatus[index] != myColor && boardStatus[index] != STATUS_EMPTY)
    {
        return true;
    }
    return false;
}

bool MainWindow::isHumanPiece(int index)
{
    if (boardStatus[index] == myColor && boardStatus[index] != STATUS_EMPTY)
    {
        return true;
    }
    return false;
}
