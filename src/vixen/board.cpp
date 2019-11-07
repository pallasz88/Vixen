#include "board.h"
#include "move_generator.h"
#include "hash.h"

#include <iostream>
#include <bitset>
#include <charconv>
#include <regex>

namespace Vixen
{
    static constexpr unsigned castlePermission[Constants::SQUARE_NUMBER] = {
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
            bitBoards{Constants::EMPTY_BOARD},
            enPassantBitBoard(Constants::EMPTY_BOARD),
            hashBoard(Hash()),
            castlingRights(0),
            historyMovesNum(0),
            fiftyMoves(0),
            material(0),
            whiteToMove(false)
    {
        AntSliderUtils::InitKnightKingAttack();
        AntSliderUtils::InitPawnAttack();
        SliderUtils::InitMagics();
        Hash::InitZobristKeys();
        SetBoard(Constants::START_POSITION);
    }

    template<size_t N, char delimiter>
    [[nodiscard]] constexpr auto Board::SplitFenPosition(std::string_view position) const
    {
        std::array<std::string_view, N> parts;
        auto part = 0ULL;
        auto firstPosition = 0ULL;
        auto nextPosition = position.find_first_of(delimiter);
        while(part < N)
        {
            parts[part++] = position.substr(firstPosition, nextPosition - firstPosition);
            firstPosition = nextPosition + 1;
            nextPosition = position.find_first_of(delimiter, firstPosition);
        }
        return parts;
    }

    void Board::SetBoard(std::string_view position)
    {
        material = 0;
        bitBoards.fill(Constants::EMPTY_BOARD);
        pieceList.fill(' ');
        std::array parsedPosition = SplitFenPosition<6>(position);
        ParseFenPiecePart(parsedPosition[0]);
        ParseSideToMovePart(parsedPosition[1]);
        ParseCastlingRightPart(parsedPosition[2]);
        SumUpBitBoards();
        enPassantBitBoard = SquareToBitBoard(NotationToSquare(parsedPosition[3]));
        std::from_chars(begin(parsedPosition[4]), end(parsedPosition[4]), fiftyMoves);
        std::from_chars(begin(parsedPosition[5]), end(parsedPosition[5]), historyMovesNum);
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

    void Board::PrintBoard() const
    {
        std::cout << "\n+---+---+---+---+---+---+---+---+\n";
        for (unsigned squareIndex = 64; squareIndex-- != 0;)
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
        auto enPassantSquare = (enPassantBitBoard != Constants::EMPTY_BOARD) ? SquareToNotation(
                TrailingZeroCount(enPassantBitBoard)) : "-";
        std::cout << "En passant square: " << enPassantSquare << "\n";
        std::cout << "Castling rights: " << std::bitset<4>(static_cast<unsigned >(castlingRights)) << "\n";
        std::cout << "Position key: " << std::hex << hashBoard.GetHash() << std::dec << "\n";
        std::cout << "\n\n";
    }

    constexpr void Board::ParseFenPiecePart(std::string_view parsedPosition)
    {
        unsigned squareIndex = Constants::MAX_SQUARE_INDEX;
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
                    SetBit(bitBoards[static_cast<unsigned>(GetPieceIndex(fenChar))], squareIndex);
                    material += GetPieceMaterial(fenChar);
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
                    squareIndex -= static_cast<unsigned>(fenChar - '1');
                    break;
                default:
                    std::cerr << "ERROR IN FEN: PIECE POSITION" << "\n";
                    return;
            }
            --squareIndex;
        }
    }

    constexpr void Board::SumUpBitBoards()
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

    constexpr void Board::ParseSideToMovePart(std::string_view splittedFen)
    {
        if (splittedFen == "w")
            whiteToMove = true;

        else if (splittedFen == "b")
            whiteToMove = false;

        else
            std::cerr << "ERROR IN FEN: SIDE TO MOVE" << "\n";

    }

    constexpr void Board::ParseCastlingRightPart(std::string_view parsedPosition)
    {
        castlingRights = 0;
        for (const auto &it : parsedPosition)
        {
            switch (it)
            {
                case 'K':
                case 'Q':
                case 'k':
                case 'q':
                    SetBit( castlingRights, static_cast<unsigned>
                    (std::distance(begin(Constants::CASTLERIGHTS),
                            std::find(begin(Constants::CASTLERIGHTS), end(Constants::CASTLERIGHTS), it))));
                    break;
                case '-':
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

        if (enPassantBitBoard != Constants::EMPTY_BOARD)
        {
            hashBoard.HashEnPassant(enPassantBitBoard);
            enPassantBitBoard = Constants::EMPTY_BOARD;
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
            material += GetPieceMaterial(promotion);
            material -= (moveType & CAPTURE) ? GetPieceMaterial(movingPieceLetter) : 0;
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

    constexpr void Board::UpdateCastlingRights(unsigned int from, unsigned int to)
    {
        hashBoard.HashCastling(castlingRights);
        castlingRights &= castlePermission[from];
        castlingRights &= castlePermission[to];
        hashBoard.HashCastling(castlingRights);
    }

    constexpr void Board::MoveCastlingWhiteRook(unsigned int from, unsigned int to)
    {
        RemovePiece(from, 'R');
        AddPiece(to, 'R');
    }

    constexpr void Board::MoveCastlingBlackRook(unsigned int from, unsigned int to)
    {
        RemovePiece(from, 'r');
        AddPiece(to, 'r');
    }

    constexpr void Board::MakeDoublePawnPush(unsigned int enPassantSquare)
    {
        SetBit(enPassantBitBoard, enPassantSquare);
        hashBoard.HashEnPassant(enPassantBitBoard);
    }

    constexpr void Board::MakeCapture(unsigned int to, char capturedPieceLetter)
    {
        fiftyMoves = 0;
        material -= GetPieceMaterial(capturedPieceLetter);
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

        if (moveType == KING_CASTLE)
            whiteToMove ? MoveCastlingWhiteRook(F1, H1) : MoveCastlingBlackRook(F8, H8);

        else if (moveType == QUEEN_CASTLE)
            whiteToMove ? MoveCastlingWhiteRook(D1, A1) : MoveCastlingBlackRook(D8, A8);

        else if (moveType & PROMOTION)
        {
            char promotion = whiteToMove ? "NBRQ"[moveType & 3U] : "nbrq"[moveType & 3U];
            material -= GetPieceMaterial(promotion);
            material += (moveType & CAPTURE) ? GetPieceMaterial(movingPieceLetter) : 0;
            RemovePiece(to, promotion);
            AddPiece(to, movingPieceLetter);
        }

        RemovePiece(to, movingPieceLetter);
        AddPiece(from, movingPieceLetter);

        if (moveType == ENPASSANT)
            whiteToMove ? AddPiece(to - 8, 'p')
                        : AddPiece(to + 8, 'P');

        else if (moveType & CAPTURE)
        {
            AddPiece(to, capturedPieceLetter);
            material += GetPieceMaterial(capturedPieceLetter);
        }

        hashBoard.SetHash(lastPosition.hash);
    }

    constexpr void Board::RemovePiece(unsigned int position, char pieceType)
    {
        pieceList[position] = ' ';
        ClearBit(bitBoards[static_cast<unsigned>(GetPieceIndex(pieceType))], position);
        ClearBit(bitBoards[static_cast<unsigned>(GetPieceIndex(IsBlackMoving(pieceType) ? 'S' : 'F'))], position);
        SetBit(bitBoards[GetPieceIndex(' ')], position);
        hashBoard.HashPiece(position, pieceType);
    }

    constexpr void Board::AddPiece(unsigned int position, char pieceType)
    {
        pieceList[position] = pieceType;
        SetBit(bitBoards[static_cast<unsigned>(GetPieceIndex(pieceType))], position);
        SetBit(bitBoards[static_cast<unsigned>(GetPieceIndex(IsBlackMoving(pieceType) ? 'S' : 'F'))], position);
        ClearBit(bitBoards[GetPieceIndex(' ')], position);
        hashBoard.HashPiece(position, pieceType);
    }

    template <uint8_t moveType>
    MoveGenerator Board::CreateGenerator() const
    {
        MoveGenerator moveGenerator;
        whiteToMove ? moveGenerator.GenerateMoves<Colors::WHITE, moveType>(*this)
                    : moveGenerator.GenerateMoves<Colors::BLACK, moveType>(*this);
        return moveGenerator;
    }

    bool Board::MakeMove(std::string_view move)
    {
        const std::regex moveRegex{"[a-h][1-8][a-h][1-8][q|r|b|n]?"};
        std::match_results<std::string_view::const_iterator> iterator;
        if (!std::regex_match(begin(move), end(move), iterator, moveRegex))
            throw std::runtime_error("INVALID MOVE FORMAT. Use for example e2e4.");

        const auto from = static_cast<unsigned>(NotationToSquare(move.substr(0, 2)));
        const auto to   = static_cast<unsigned>(NotationToSquare(move.substr(2, 2)));

        char promoted = ' ';
        if (move.size() == 5)
            promoted = move.substr(4)[0];

        uint8_t moveType = 0;
        if (promoted != ' ')
        {
            moveType = static_cast<uint8_t>(GetPromotionType(promoted));
            char capturedPiece = GetPieceList()[to];
            if (capturedPiece != ' ')
            {
                moveType |= static_cast<uint8_t>(CAPTURE);
            }
        }

        const auto decodedPromotion = CreateMove(from, to, moveType);
        const auto decodedMove      = decodedPromotion & 0xFFFU;

        const auto generator    = CreateGenerator<ALL_MOVE>();
        const auto moves        = generator.GetMoveList();
        const auto moveListSize = generator.GetListSize();

        for (size_t i = 0; i < moveListSize; ++i)
            if ((moves[i] & 0xFFFU) == decodedMove)
                return (moveType == 0) ? MakeMove(moves[i]) : MakeMove(decodedPromotion);

        return false;
    }

    template VIXEN_API MoveGenerator Board::CreateGenerator<CAPTURE>() const;

    template VIXEN_API MoveGenerator Board::CreateGenerator<ALL_MOVE>() const;

}