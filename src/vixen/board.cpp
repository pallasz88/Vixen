#include "board.h"
#include "move_generator.h"
#include "hash.h"
#include "timer.h"

#include <iostream>
#include <exception>
#include <bitset>
#include <boost/algorithm/string.hpp>

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
            enPassantBitBoard(EMPTY_BOARD),
            castlingRights(0),
            historyMovesNum(0),
            fiftyMoves(0),
            whiteToMove(false)
    {
        AddMoveGenerator();
        SetBoard(START_POSITION);
        InitKnightKingAttack();
        InitPawnAttack();
        InitMagics();
#ifdef DEBUG
        PrintBoard();
#endif
    }

    void Board::SetBoard(const std::string &position)
    {
        this->fenPosition = position;
        InitBitBoards(bitBoards);
        std::vector<std::string> parsedPosition;
        SplitFenPosition(parsedPosition);
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

    void Board::AddMoveGenerator()
    {
        moveGenerator = std::make_shared<MoveGenerator>();
    }

    void Board::AddHashBoard()
    {
        hashBoard = std::make_unique<Hash>(*this);
    }

    void Board::ClearHistory()
    {
        if (!history.empty())
            history = std::stack<History>();
    }

    void Board::SplitFenPosition(std::vector<std::string> &fenParts)
    {
        boost::split(fenParts, fenPosition, boost::is_any_of(" "));
    }

    void Board::PrintBoard() const
    {
        std::cout << std::endl << "+---+---+---+---+---+---+---+---+" << std::endl;
        for (int squareIndex = MAX_SQUARE_INDEX; squareIndex >= H1; --squareIndex)
        {
            for (const auto &item : bitBoards)
            {
                if (IsBitSet(item.second, squareIndex) && item.first != 'F' && item.first != 'S')
                {
                    std::cout << "| " << item.first << " ";
                    break;
                }
            }
            if (squareIndex % 8 == 0)
            {
                std::cout << "| " << squareIndex / 8 + 1 << std::endl;
                std::cout << "+---+---+---+---+---+---+---+---+" << std::endl;
            }
        }

        std::cout << "  ";
        for (int rank = 0; rank < 8; ++rank)
            std::cout << static_cast<char>('a' + rank) << "   ";

        std::cout << std::endl << std::endl;
        auto enPassantSquare = (enPassantBitBoard != EMPTY_BOARD) ? SquareToNotation(
                TrailingZeroCount(enPassantBitBoard)) : "-";
        std::cout << "En passant square: " << enPassantSquare << std::endl;
        std::cout << "Castling rights: " << std::bitset<4>(static_cast<unsigned >(castlingRights)) << std::endl;
        std::cout << "Position key: " << std::hex << hashBoard->GetHash() << std::dec << std::endl;
        std::cout << "Fen position: " << fenPosition << std::endl;
        std::cout << std::endl << std::endl;
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
                    SetBit(bitBoards[fenChar], squareIndex);
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
                    std::cerr << "ERROR IN FEN: PIECE POSITION" << std::endl;
                    return;
            }
            --squareIndex;
        }
    }

    void Board::SumUpBitBoards()
    {
        bitBoards['S'] = bitBoards['k'] |
                         bitBoards['q'] |
                         bitBoards['r'] |
                         bitBoards['b'] |
                         bitBoards['n'] |
                         bitBoards['p'];
        bitBoards['F'] = bitBoards['K'] |
                         bitBoards['Q'] |
                         bitBoards['R'] |
                         bitBoards['B'] |
                         bitBoards['N'] |
                         bitBoards['P'];
        bitBoards[' '] = ~(bitBoards['F'] | bitBoards['S']);
    }

    void Board::ParseSideToMovePart(const std::string &parsedPosition)
    {
        if (parsedPosition == "w")
            whiteToMove = true;

        else if (parsedPosition == "b")
            whiteToMove = false;

        else
            std::cerr << "ERROR IN FEN: SIDE TO MOVE" << std::endl;

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
                    std::cerr << "ERROR IN FEN: CASTLING" << std::endl;
                    break;
            }
        }
    }

    void InitBitBoards(BitBoards &bitBoards)
    {
        bitBoards['P'] = EMPTY_BOARD;
        bitBoards['p'] = EMPTY_BOARD;
        bitBoards['B'] = EMPTY_BOARD;
        bitBoards['b'] = EMPTY_BOARD;
        bitBoards['N'] = EMPTY_BOARD;
        bitBoards['n'] = EMPTY_BOARD;
        bitBoards['K'] = EMPTY_BOARD;
        bitBoards['k'] = EMPTY_BOARD;
        bitBoards['r'] = EMPTY_BOARD;
        bitBoards['R'] = EMPTY_BOARD;
        bitBoards['Q'] = EMPTY_BOARD;
        bitBoards['q'] = EMPTY_BOARD;
        bitBoards['F'] = EMPTY_BOARD;
        bitBoards['S'] = EMPTY_BOARD;
        bitBoards[' '] = EMPTY_BOARD;
    }


    bool Board::MakeMove(Vixen::Move move)
    {
        int from = move & 0x3F;
        int to = (move >> 6) & 0x3F;
        int moveType = move >> 12;
        int enPassantSquare = whiteToMove ? to - 8 : to + 8;
        char movingPieceLetter = pieceList[from];
        char capturedPieceLetter = pieceList[to];

        history.emplace(enPassantBitBoard,
                        castlingRights,
                        fiftyMoves,
                        move,
                        movingPieceLetter,
                        capturedPieceLetter,
                        hashBoard->GetHash());

        ++fiftyMoves;

        if (enPassantBitBoard != EMPTY_BOARD)
        {
            hashBoard->HashEnPassant(enPassantBitBoard);
            enPassantBitBoard = EMPTY_BOARD;
        }

        if (tolower(movingPieceLetter) == 'p')
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
            char promotion = whiteToMove ? "NBRQ"[moveType & 3] : "nbrq"[moveType & 3];
            RemovePiece(to, movingPieceLetter);
            AddPiece(to, promotion);
        }

        whiteToMove = !whiteToMove;
        hashBoard->HashSide();
        ++historyMovesNum;

        if (whiteToMove ? Check::IsInCheck<Colors::BLACK>(bitBoards)
                        : Check::IsInCheck<Colors::WHITE>(bitBoards))
        {
            TakeBack();
            return false;
        }
        return true;
    }

    void Board::UpdateCastlingRights(int from, int to)
    {
        hashBoard->HashCastling(*this);
        castlingRights &= castlePermission[from];
        castlingRights &= castlePermission[to];
        hashBoard->HashCastling(*this);
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
        hashBoard->HashEnPassant(enPassantBitBoard);
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

        History lastPosition = history.top();
        history.pop();

        int from = lastPosition.move & 0x3F;
        int to = (lastPosition.move >> 6) & 0x3F;
        int moveType = lastPosition.move >> 12;
        char movingPieceLetter = lastPosition.movedPiece;
        char capturedPieceLetter = lastPosition.capturedPiece;

        --historyMovesNum;
        whiteToMove = !whiteToMove;
        fiftyMoves = lastPosition.fiftyMoves;
        enPassantBitBoard = lastPosition.enPassant;
        castlingRights = lastPosition.castlingRights;
        hashBoard->SetHash(lastPosition.hash);

        if (moveType == KING_CASTLE)
            whiteToMove ? MoveCastlingWhiteRook(F1, H1) : MoveCastlingBlackRook(F8, H8);

        else if (moveType == QUEEN_CASTLE)
            whiteToMove ? MoveCastlingWhiteRook(D1, A1) : MoveCastlingBlackRook(D8, A8);

        else if (moveType & PROMOTION)
        {
            char promotion = whiteToMove ? "NBRQ"[moveType & 3] : "nbrq"[moveType & 3];
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

        //IsBoardConsistent();
    }

    void Board::RemovePiece(int position, char pieceType)
    {
        pieceList[position] = ' ';
        ClearBit(bitBoards.at(pieceType), position);
        hashBoard->HashPiece(position, pieceType);
        ClearBit(bitBoards.at(islower(pieceType) ? 'S' : 'F'), position);
        SetBit(bitBoards.at(' '), position);
    }

    void Board::AddPiece(int position, char pieceType)
    {
        pieceList[position] = pieceType;
        SetBit(bitBoards.at(pieceType), position);
        hashBoard->HashPiece(position, pieceType);
        SetBit(bitBoards.at(islower(pieceType) ? 'S' : 'F'), position);
        ClearBit(bitBoards.at(' '), position);
    }

    bool Board::IsBoardConsistent() const
    {
        for (int square = 0; square < SQUARE_NUMBER; ++square)
        {
            if (!IsBitSet(bitBoards.at(pieceList[square]), square))
                throw std::runtime_error("Board is not ok: " + std::to_string(square) + pieceList[square]);
        }
    }

    std::vector<Move> Board::GetAllMoves()
    {
        moveGenerator.reset(new MoveGenerator);
        IsWhiteToMove() ? moveGenerator->GenerateAllMoves<Colors::WHITE>(*this)
                        : moveGenerator->GenerateAllMoves<Colors::BLACK>(*this);
        return moveGenerator->GetMoveList();
    }

}