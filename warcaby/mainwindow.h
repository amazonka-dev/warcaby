#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "game.h"
#include "./piecewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    enum Turn
    {
        //! Brak ruchu (ani komputer ani gracz nie wykonuje ruchu)
        NO_TURN,
        //! Ruch komputera
        COMPUTER_TURN,
        //! Ruch gracza
        HUMAN_TURN
    };
    /**
     * @brief Pokazuje komunikat z podstawowymi informacjami o grze.
     */

    void showAbout();

    /**
     * @brief Zwraca indeks wiersza
     * @param index Indeks pola
     * @return Indeks liczony od 0.
     */
    int getRow(int index)
    {
        return index / 10;
    }

    /**
     * @brief Zwraca indeks kolumny
     * @param index Indeks pola
     * @return Indeks liczony od 0.
     */
    int getColumn(int index)
    {
        return index % 10;
    }

    /**
     * @brief Sprawdza, czy wskazane pole może być użyte podczas gry.
     * @param index Indeks pola
     * @return true, jesli pole może być używane do gry, false w przeciwnym wypadku.
     */
    bool isPlayable(int index);

    /**
     * @brief Aktualizuje widok planszy na podstawie stanu gry
     */
    void updateWidgets();

    /**
     * @brief Funkcja wywoływana po kliknięciu na dozwolone pole
     *
     * W zależności od stanu gry funkcja zaznacza pierwszy pionek do ruchu lub przesuwa pionek, lub wykonuje ruch bicia.
     * @param index Indeks pola, na które kliknięto.
     */
    void onClickBox(int index);

    /**
     * @brief Wszystkie dozwolone ruchy komputera
     * @return Zwraca wszystkie dozwolone ruchy komputera
     */
    std::vector<AvailableMovement> getAllAvailableComputerMovements();

    /**
     * @brief Wszystkie dozwolone ruchy gracza
     * @return zwraca wszystkie dozwolone ruchy gracza
     */
    std::vector<AvailableMovement> getAllAvailableHumanMovements();

    /**
     * @brief Wszystkie dozwolone bicia gracza
     * @return zwraca wszystkie dozwolone bicia gracza
     */
    std::vector<CapturedMovement> getAllAvailableHumanCaptures();

    /**
     * @brief Wszystkie dozwolone bicia komputera
     * @return zwraca wszystkie dozwolone bicia komputera
     */
    std::vector<CapturedMovement> getAllAvailableComputerCaptures();

private:

    /**
     * @brief modyfikuje stan gry
     *
     * Wykonuje ruch pionka zarejestrowanego na podstawie zebranych informacji.
     * Nie modyfikuje widoku planszy.
     */
    void moveMarkedPiece();

    /**
     * @brief modyfikuje stan gry
     *
     * Wykonuje bicie pionka zarejestrowanego na podstawie zebranych informacji.
     * Nie modyfikuje widoku planszy.
     */
    void captureMarkedPiece();

    /**
     * @brief Oblicza dozwolone indeksy pól, na które można przesunąć pionek.
     * @param indeks wskazanego pola
     * @return wektor indeksów dla wskazanego pola.
     */
    std::vector<int> getValidIndices(int sourceIndex);

    /**
     * @brief Oblicza dozwolone indeksy pól, z których gracz może zbić pionek komputera
     * @param indeks wskazanego pola
     * @return wektor obiektów z informacją o biciu dla wskazanego pola.
     */
    std::vector<CapturedMovement> getValidHumanCaptures(int sourceIndex);

    /**
     * @brief Oblicza dozwolone indeksy pól, z których komputer może zbić pionek gracza
     * @param indeks wskazanego pola
     * @return wektor obiektów z informacją o biciu dla wskazanego pola.
     */
    std::vector<CapturedMovement> getValidComputerCaptures(int sourceIndex);

    /**
     * @brief Sprawdza, czy pole jest puste i czym jest na liście dozwolonych ruchów.
     * @param indeks wskazanego pola
     * @param indeks docelowego pola
     * @return true, jesli na wybrane pole można przesunąć pionek, false w przeciwnym wypadku.
     */
    bool isValidTarget(int sourceIndex, int targetIndex);

    /**
     * @brief Zlicza indeksy pól, z których gracz może zbić pionek komputera.
     * @param indeks wskazanego pola
     * @param indeks docelowego pola
     * @return indeks bitego pionka lub -1, jeśli nie ma bicia.
     */
    int getHumanCapturedIndex(int sourceIndex, int targetIndex);

    /**
     * @brief Wykonuje ruch komputera i przekazuje możliwość ruchu graczowi.
     *
     * Najpierw komputer wykonuje ew. bicie. Jeśli nie ma możliwości bicia, to wykonuje losowy ruch z dozwolonych.
     */
    void doComputerTurn();

    /**
     * @brief Sprawdza, czy na danym polu stoi pionek komputera.
     * @param indeks wskazanego pola
     * @return true, jesli na danym polu stoi pionek komputera, false w przeciwnym wypadku.
     */
    bool isComputerPiece(int index);

    /**
     * @brief Sprawdza, czy na danym polu stoi pionek gracza.
     * @param indeks wskazanego pola
     * @return true, jesli na danym polu stoi pionek gracza, false w przeciwnym wypadku.
     */
    bool isHumanPiece(int index);

private slots:

    /**
     * @brief Ustawia białe pionki na planszy dla gracza, a czarne dla komputera po uruchomieniu gry.
     */
    void on_buttonWhite_clicked();

    /**
     * @brief Ustawia czarne pionki na planszy dla gracza, a białe dla komputera po uruchomieniu gry.
     */
    void on_buttonBlack_clicked();


    void onFrameClicked();

private:
    Ui::MainWindow *ui;


    SquareStatus boardStatus[100];
    std::vector<int> markedIndices;
    SquareStatus myColor = STATUS_EMPTY;
    PieceWidget* whitePieces[100];
    PieceWidget* blackPieces[100];
    Turn turn = NO_TURN;

};
#endif // MAINWINDOW_H

