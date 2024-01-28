#ifndef GAME_H
#define GAME_H

enum SquareStatus
{
    STATUS_EMPTY,
    STATUS_WHITE,
    STATUS_BLACK
};

struct AvailableMovement {
    int sourceIndex;
    int targetIndex;
};

struct CapturedMovement : public AvailableMovement {
    int capturedIndex;

    CapturedMovement(int src, int capt, int trg)
    {
        sourceIndex = src;
        capturedIndex = capt;
        targetIndex = trg;
    }
};

#endif // GAME_H
