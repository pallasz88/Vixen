#include "board.h"
#include "move_generator.h"
#include "hash.h"
#include <iostream>

#include <bitset>
#include <iterator>
#include <sstream>

namespace Vixen
{
    constexpr int castlePermission[SQUARE_NUMBER] = {
            7, 15, 15, 3, 15, 15, 15, 11,
            15, 15, 15, 15, 15, 15, 15, 15,
            15, 15, 15, 15, 15, 15, 15, 15,
            15, 15, 15, 15, 15, 15, 15, 15,
            15, 15, 15, 15, 15, 15, 15, 15,
            15, 15, 15, 15, 15, 15, 15, 15,
            15, 15, 15, 15, 15, 15, 15, 15,
            13, 15, 15, 12, 15, 15, 15, 14
    };

    Board::Board() :
            pieceList{' '},
            bitBoards{EMPTY_BOARD},
            enPassantBitBoard(EMPTY_BOARD),
            castlingRights(0),
            historyMovesNum(0),
            fiftyMoves(0),
            whiteToMove(false),
            hashBoard(Hash())
    {
        SetBoard(START_POSITION);
        AntSliderUtils::InitKnightKingAttack();
        AntSliderUtils::InitPawnAttack();
        SliderUtils::InitMagics();
        Hash::InitZobristKeys();
#ifdef DEBUG
        PrintBoard();
#endif
    }

    void Board::SetBoard(const std::string &position)
    {
        fenPosition = position;
        bitBoards.fill(EMPTY_BOARD);
        pieceList.fill(' ');
        std::vector<std::string> parsedPosition = SplitFenPosition();
        ParseFenPiecePart(parsedPosition[0]);
        ParseSideToMovePart(parsedPosition[1]);
        ParseCastlingRightPart(parsedPosition[2]);
        SumUpBitBoards();
        enPassantBitBoard = SquareToBitBoard(NotationToSquare(parsedPosition[3]));
        fiftyMoves = stoi(parsedPosition[4]);
        historyMovesNum = stoi(parsedPosition[5]);
        AddHashBoard();
        ClearHistory();
    }

    void Board::AddHashBoard()
    {
        hashBoard.ComputePositionKey(*this);
    }

    void Board::ClearHistory()
    {
        if (!history.empty())
            history = std::stack<History>();
    }

    std::vector<std::string> Board::SplitFenPosition() const
    {
        std::istringstream iss(fenPosition);
        return {std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>()};
    }

    void Board::PrintBoard() const
    {
        std::cout << "\n+---+---+---+---+---+---+---+---+\n";
        for (int squareIndex = MAX_SQUARE_INDEX; squareIndex >= H1; --squareIndex)
        {
            std::cout << "| " << pieceList[squareIndex] << " ";
            if (squareIndex % 8 == 0)
            {
                std::cout << "| " << squareIndex / 8 + 1 << "\n";
                std::cout << "+---+---+---+---+---+---+---+---+\n";
            }
        }

        std::cout << "  ";
        for (int rank = 0; rank < 8; ++rank)
            std::cout << static_cast<char>('a' + rank) << "   ";

        std::cout << "\n\n";
        auto enPassantSquare = (enPassantBitBoard != EMPTY_BOARD) ? SquareToNotation(
                TrailingZeroCount(enPassantBitBoard)) : "-";
        std::cout << "En passant square: " << enPassantSquare << "\n";
        std::cout << "Castling rights: " << std::bitset<4>(static_cast<unsigned >(castlingRights)) << "\n";
        std::cout << "Position key: " << std::hex << hashBoard.GetHash() << std::dec << "\n";
        std::cout << "Fen position: " << fenPosition << "\n";
        std::cout << "\n\n";
    }

    void Board::ParseFenPiecePart(const std::string &parsedPosition)
    {
        unsigned squareIndex = MAX_SQUARE_INDEX;
        for (const auto &fenChar : parsedPosition)
        {
            switch (fenChar)
            {
                case 'P':
                case 'N':
                case 'B':
                case 'R':
                case 'Q':
                case 'K':
                case 'p':
                case 'n':
                case 'b':
                case 'r':
                case 'q':
                case 'k':
                    SetBit(bitBoards[GetPieceIndex(fenChar)], squareIndex);
                    pieceList[squareIndex] = fenChar;
                    break;
                case '/':
                    continue;
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                    squareIndex -= fenChar - '1';
                    break;
                default:
                    std::cerr << "ERROR IN FEN: PIECE POSITION" << "\n";
                    return;
            }
            --squareIndex;
        }
    }

    void Board::SumUpBitBoards()
    {
        bitBoards[GetPieceIndex('F')] = bitBoards[GetPieceIndex('K')] |
                                        bitBoards[GetPieceIndex('Q')] |
                                        bitBoards[GetPieceIndex('R')] |
                                        bitBoards[GetPieceIndex('B')] |
                                        bitBoards[GetPieceIndex('N')] |
                                        bitBoards[GetPieceIndex('P')];
        bitBoards[GetPieceIndex('S')] = bitBoards[GetPieceIndex('k')] |
                                        bitBoards[GetPieceIndex('q')] |
                                        bitBoards[GetPieceIndex('r')] |
                                        bitBoards[GetPieceIndex('b')] |
                                        bitBoards[GetPieceIndex('n')] |
                                        bitBoards[GetPieceIndex('p')];
        bitBoards[GetPieceIndex(' ')] = ~(bitBoards[GetPieceIndex('F')] | bitBoards[GetPieceIndex('S')]);
    }

    void Board::ParseSideToMovePart(const std::string &parsedPosition)
    {
        if (parsedPosition == "w")
            whiteToMove = true;

        else if (parsedPosition == "b")
            whiteToMove = false;

        else
            std::cerr << "ERROR IN FEN: SIDE TO MOVE" << "\n";

    }

    void Board::ParseCastlingRightPart(const std::string &parsedPosition)
    {
        castlingRights = 0;
        for (const auto &it : parsedPosition)
        {
            switch (it)
            {
                case 'K':
                    SetBit(castlingRights, 3);
                    break;
                case 'Q':
                    SetBit(castlingRights, 2);
                    break;
                case 'k':
                    SetBit(castlingRights, 1);
                    break;
                case 'q':
                    SetBit(castlingRights, 0);
                    break;
                case '-':
                    castlingRights = 0;
                    break;
                default:
                    std::cerr << "ERROR IN FEN: CASTLING\n";
                    break;
            }
        }
    }

    bool Board::MakeMove(Vixen::Move move)
    {
        const auto from = move & 0x3FU;
        const auto to = (move >> 6U) & 0x3FU;
        const auto moveType = move >> 12U;
        const auto enPassantSquare = whiteToMove ? to - 8 : to + 8;
        const char movingPieceLetter = pieceList[from];
        const char capturedPieceLetter = pieceList[to];

        history.emplace(enPassantBitBoard,
                        castlingRights,
                        fiftyMoves,
                        move,
                        movingPieceLetter,
                        capturedPieceLetter,
                        hashBoard.GetHash());

        ++fiftyMoves;

        if (enPassantBitBoard != EMPTY_BOARD)
        {
            hashBoard.HashEnPassant(enPassantBitBoard);
            enPassantBitBoard = EMPTY_BOARD;
        }

        if (IsMovingPawn(movingPieceLetter))
            fiftyMoves = 0;

        if (moveType & CAPTURE)
        {
            MakeCapture(to, capturedPieceLetter);
            if (moveType == ENPASSANT)
                whiteToMove ? RemovePiece(to - 8, 'p') : RemovePiece(to + 8, 'P');
        }

        else if (moveType == DOUBLE_PAWN_PUSH)
            MakeDoublePawnPush(enPassantSquare);

        else if (moveType == KING_CASTLE)
            whiteToMove ? MoveCastlingWhiteRook(H1, F1) : MoveCastlingBlackRook(H8, F8);

        else if (moveType == QUEEN_CASTLE)
            whiteToMove ? MoveCastlingWhiteRook(A1, D1) : MoveCastlingBlackRook(A8, D8);

        RemovePiece(from, movingPieceLetter);
        AddPiece(to, movingPieceLetter);
        UpdateCastlingRights(from, to);

        if (moveType & PROMOTION)
        {
            char promotion = whiteToMove ? "NBRQ"[moveType & 3U] : "nbrq"[moveType & 3U];
            RemovePiece(to, movingPieceLetter);
            AddPiece(to, promotion);
        }

        whiteToMove = !whiteToMove;
        hashBoard.HashSide();
        ++historyMovesNum;

        if (whiteToMove ? Check::IsInCheck<Colors::BLACK>(*this)
                        : Check::IsInCheck<Colors::WHITE>(*this))
        {
            TakeBack();
            return false;
        }
        return true;
    }

    void Board::UpdateCastlingRights(int from, int to)
    {
        hashBoard.HashCastling(*this);
        castlingRights &= castlePermission[from];
        castlingRights &= castlePermission[to];
        hashBoard.HashCastling(*this);
    }

    void Board::MoveCastlingWhiteRook(int from, int to)
    {
        RemovePiece(from, 'R');
        AddPiece(to, 'R');
    }

    void Board::MoveCastlingBlackRook(int from, int to)
    {
        RemovePiece(from, 'r');
        AddPiece(to, 'r');
    }

    void Board::MakeDoublePawnPush(int enPassantSquare)
    {
        SetBit(enPassantBitBoard, enPassantSquare);
        hashBoard.HashEnPassant(enPassantBitBoard);
    }

    void Board::MakeCapture(int to, char capturedPieceLetter)
    {
        fiftyMoves = 0;
        RemovePiece(to, capturedPieceLetter);
    }

    void Board::TakeBack()
    {
        if (history.empty())
            throw std::runtime_error("Empty history");

        const History &lastPosition = history.top();
        history.pop();

        const auto from                = lastPosition.move & 0x3FU;
        const auto to                  = (lastPosition.move >> 6U) & 0x3FU;
        const auto moveType            = lastPosition.move >> 12U;
        const char movingPieceLetter   = lastPosition.movedPiece;
        const char capturedPieceLetter = lastPosition.capturedPiece;

        --historyMovesNum;
        whiteToMove = !whiteToMove;
        fiftyMoves = lastPosition.fiftyMoves;
        enPassantBitBoard = lastPosition.enPassant;
        castlingRights = lastPosition.castlingRights;
        hashBoard.SetHash(lastPosition.hash);

        if (moveType == KING_CASTLE)
            whiteToMove ? MoveCastlingWhiteRook(F1, H1) : MoveCastlingBlackRook(F8, H8);

        else if (moveType == QUEEN_CASTLE)
            whiteToMove ? MoveCastlingWhiteRook(D1, A1) : MoveCastlingBlackRook(D8, A8);

        else if (moveType & PROMOTION)
        {
            char promotion = whiteToMove ? "NBRQ"[moveType & 3U] : "nbrq"[moveType & 3U];
            RemovePiece(to, promotion);
            AddPiece(to, movingPieceLetter);
        }

        RemovePiece(to, movingPieceLetter);
        AddPiece(from, movingPieceLetter);

        if (moveType == ENPASSANT)
            whiteToMove ? AddPiece(to - 8, 'p')
                        : AddPiece(to + 8, 'P');

        else if (moveType & CAPTURE)
            AddPiece(to, capturedPieceLetter);
    }

    void Board::RemovePiece(int position, char pieceType)
    {
        pieceList[position] = ' ';
        ClearBit(bitBoards[GetPieceIndex(pieceType)], position);
        hashBoard.HashPiece(position, pieceType);
        ClearBit(bitBoards[GetPieceIndex(IsBlackMoving(pieceType) ? 'S' : 'F')], position);
        SetBit(bitBoards[GetPieceIndex(' ')], position);
    }

    void Board::AddPiece(int position, char pieceType)
    {
        pieceList[position] = pieceType;
        SetBit(bitBoards[GetPieceIndex(pieceType)], position);
        hashBoard.HashPiece(position, pieceType);
        SetBit(bitBoards[GetPieceIndex(IsBlackMoving(pieceType) ? 'S' : 'F')], position);
        ClearBit(bitBoards[GetPieceIndex(' ')], position);
    }

    template <uint8_t moveType>
    MoveGenerator Board::CreateGenerator() const
    {
        MoveGenerator moveGenerator;
        whiteToMove ? moveGenerator.GenerateMoves<Colors::WHITE, moveType>(*this)
                    : moveGenerator.GenerateMoves<Colors::BLACK, moveType>(*this);
        return moveGenerator;
    }

    template VIXEN_API MoveGenerator Board::CreateGenerator<CAPTURE>() const;

    template VIXEN_API MoveGenerator Board::CreateGenerator<ALL_MOVE>() const;

}