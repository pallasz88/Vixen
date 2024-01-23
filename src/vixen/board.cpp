#include "board.hpp"

#include <bitset>
#include <charconv>
#include <iostream>
#include <regex>

#include "defs.hpp"
#include "move_generator.hpp"

namespace vixen
{
// clang-format off
static constexpr std::array castlePermission = {
    11U, 15U, 15U,  3U, 15U, 15U, 15U,  7U,
    15U, 15U, 15U, 15U, 15U, 15U, 15U, 15U,
    15U, 15U, 15U, 15U, 15U, 15U, 15U, 15U,
    15U, 15U, 15U, 15U, 15U, 15U, 15U, 15U,
    15U, 15U, 15U, 15U, 15U, 15U, 15U, 15U,
    15U, 15U, 15U, 15U, 15U, 15U, 15U, 15U,
    15U, 15U, 15U, 15U, 15U, 15U, 15U, 15U,
    14U, 15U, 15U, 12U, 15U, 15U, 15U, 13U};
// clang-format on

Board::Board()
{
    Hash::InitZobristKeys();
    SetBoard(Constants::START_POSITION);
}

template <size_t N, char delimiter>
[[nodiscard]] constexpr auto Board::SplitFenPosition(std::string_view position) const noexcept
{
    std::array<std::string_view, N> parts;
    auto part = 0ULL;
    auto firstPosition = 0ULL;
    auto nextPosition = position.find_first_of(delimiter);
    while (part < N)
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
    pieceList.fill(Constants::ALL_EMPTY_INDEX);
    std::array parsedPosition = SplitFenPosition<5>(position);
    ParseFenPiecePart(parsedPosition[0]);
    ParseSideToMovePart(parsedPosition[1]);
    ParseCastlingRightPart(parsedPosition[2]);
    SumUpBitBoards();
    enPassantBitBoard = SquareToBitBoard(Move::NotationToSquare(parsedPosition[3]));
    std::from_chars(parsedPosition[4].data(), parsedPosition[4].data() + parsedPosition[4].size(), fiftyMoves);
    historyPly = fiftyMoves;
    AddHashBoard();
}

void Board::AddHashBoard()
{
    hashBoard.ComputePositionKey(*this);
}

void Board::PrintBoard() const noexcept
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
    auto enPassantSquare = (enPassantBitBoard != Constants::EMPTY_BOARD)
                               ? Move::SquareToNotation(TrailingZeroCount(enPassantBitBoard))
                               : "-";
    std::cout << "En passant square: " << enPassantSquare << "\n";
    std::cout << "Castling rights: " << std::bitset<4>(castlingRights) << "\n";
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
            material += GetPieceMaterial(Constants::WHITE_PAWN_INDEX).value();
            pieceList[squareIndex] = Constants::WHITE_PAWN_INDEX;
            SetBit(bitBoards[Constants::WHITE_PAWN_INDEX], squareIndex);
            break;
        case 'N':
            material += GetPieceMaterial(Constants::WHITE_KNIGHT_INDEX).value();
            pieceList[squareIndex] = Constants::WHITE_KNIGHT_INDEX;
            SetBit(bitBoards[Constants::WHITE_KNIGHT_INDEX], squareIndex);
            break;
        case 'B':
            material += GetPieceMaterial(Constants::WHITE_BISHOP_INDEX).value();
            pieceList[squareIndex] = Constants::WHITE_BISHOP_INDEX;
            SetBit(bitBoards[Constants::WHITE_BISHOP_INDEX], squareIndex);
            break;
        case 'R':
            material += GetPieceMaterial(Constants::WHITE_ROOK_INDEX).value();
            pieceList[squareIndex] = Constants::WHITE_ROOK_INDEX;
            SetBit(bitBoards[Constants::WHITE_ROOK_INDEX], squareIndex);
            break;
        case 'Q':
            material += GetPieceMaterial(Constants::WHITE_QUEEN_INDEX).value();
            pieceList[squareIndex] = Constants::WHITE_QUEEN_INDEX;
            SetBit(bitBoards[Constants::WHITE_QUEEN_INDEX], squareIndex);
            break;
        case 'K':
            material += GetPieceMaterial(Constants::WHITE_KING_INDEX).value();
            pieceList[squareIndex] = Constants::WHITE_KING_INDEX;
            SetBit(bitBoards[Constants::WHITE_KING_INDEX], squareIndex);
            break;
        case 'p':
            material += GetPieceMaterial(Constants::BLACK_PAWN_INDEX).value();
            pieceList[squareIndex] = Constants::BLACK_PAWN_INDEX;
            SetBit(bitBoards[Constants::BLACK_PAWN_INDEX], squareIndex);
            break;
        case 'n':
            material += GetPieceMaterial(Constants::BLACK_KNIGHT_INDEX).value();
            pieceList[squareIndex] = Constants::BLACK_KNIGHT_INDEX;
            SetBit(bitBoards[Constants::BLACK_KNIGHT_INDEX], squareIndex);
            break;
        case 'b':
            material += GetPieceMaterial(Constants::BLACK_BISHOP_INDEX).value();
            pieceList[squareIndex] = Constants::BLACK_BISHOP_INDEX;
            SetBit(bitBoards[Constants::BLACK_BISHOP_INDEX], squareIndex);
            break;
        case 'r':
            material += GetPieceMaterial(Constants::BLACK_ROOK_INDEX).value();
            pieceList[squareIndex] = Constants::BLACK_ROOK_INDEX;
            SetBit(bitBoards[Constants::BLACK_ROOK_INDEX], squareIndex);
            break;
        case 'q':
            material += GetPieceMaterial(Constants::BLACK_QUEEN_INDEX).value();
            pieceList[squareIndex] = Constants::BLACK_QUEEN_INDEX;
            SetBit(bitBoards[Constants::BLACK_QUEEN_INDEX], squareIndex);
            break;
        case 'k':
            material += GetPieceMaterial(Constants::BLACK_KING_INDEX).value();
            pieceList[squareIndex] = Constants::BLACK_KING_INDEX;
            SetBit(bitBoards[Constants::BLACK_KING_INDEX], squareIndex);
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
            std::cerr << "ERROR IN FEN: PIECE POSITION"
                      << "\n";
            return;
        }
        --squareIndex;
    }
}

constexpr void Board::SumUpBitBoards()
{
    bitBoards[Constants::ALL_WHITE_INDEX] =
        bitBoards[Constants::WHITE_KING_INDEX] | bitBoards[Constants::WHITE_QUEEN_INDEX] |
        bitBoards[Constants::WHITE_ROOK_INDEX] | bitBoards[Constants::WHITE_BISHOP_INDEX] |
        bitBoards[Constants::WHITE_KNIGHT_INDEX] | bitBoards[Constants::WHITE_PAWN_INDEX];
    bitBoards[Constants::ALL_BLACK_INDEX] =
        bitBoards[Constants::BLACK_KING_INDEX] | bitBoards[Constants::BLACK_QUEEN_INDEX] |
        bitBoards[Constants::BLACK_ROOK_INDEX] | bitBoards[Constants::BLACK_BISHOP_INDEX] |
        bitBoards[Constants::BLACK_KNIGHT_INDEX] | bitBoards[Constants::BLACK_PAWN_INDEX];
    bitBoards[Constants::ALL_EMPTY_INDEX] =
        ~(bitBoards[Constants::ALL_WHITE_INDEX] | bitBoards[Constants::ALL_BLACK_INDEX]);
}

constexpr void Board::ParseSideToMovePart(std::string_view splittedFen)
{
    if (splittedFen == "w")
        whiteToMove = true;

    else if (splittedFen == "b")
        whiteToMove = false;

    else
        std::cerr << "ERROR IN FEN: SIDE TO MOVE\n";
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
            SetBit(castlingRights,
                   static_cast<unsigned>(
                       std::distance(begin(Constants::CASTLERIGHTS), std::ranges::find(Constants::CASTLERIGHTS, it))));
            break;
        case '-':
            break;
        default:
            std::cerr << "ERROR IN FEN: CASTLING\n";
            break;
        }
    }
}

bool Board::MakeMove(vixen::Move move)
{
    const auto from = move.GetFromSquare();
    const auto to = move.GetToSquare();
    const auto moveType = move.GetMoveType();
    const auto enPassantSquare = whiteToMove ? to - 8 : to + 8;
    const auto movingPiece{pieceList[from]};
    const auto capturedPiece{pieceList[to]};

    history[historyPly++] =
        History{enPassantBitBoard, hashBoard.GetHash(), castlingRights, fiftyMoves, move, movingPiece, capturedPiece};

    ++fiftyMoves;

    RemovePiece(from, movingPiece);
    UpdateCastlingRights(from, to);

    if (enPassantBitBoard != Constants::EMPTY_BOARD)
    {
        hashBoard.HashEnPassant(enPassantBitBoard);
        enPassantBitBoard = Constants::EMPTY_BOARD;
    }

    if (IsMovingPawn(movingPiece))
        fiftyMoves = 0;

    if (moveType & static_cast<uint8_t>(MoveTypes::CAPTURE))
        MakeCapture(to, capturedPiece, moveType);

    else if (moveType == static_cast<uint8_t>(MoveTypes::DOUBLE_PAWN_PUSH))
        MakeDoublePawnPush(enPassantSquare);

    else if (moveType == static_cast<uint8_t>(MoveTypes::KING_CASTLE))
        whiteToMove ? MoveCastlingWhiteRook(static_cast<unsigned>(Squares::H1), static_cast<unsigned>(Squares::F1))
                    : MoveCastlingBlackRook(static_cast<unsigned>(Squares::H8), static_cast<unsigned>(Squares::F8));

    else if (moveType == static_cast<uint8_t>(MoveTypes::QUEEN_CASTLE))
        whiteToMove ? MoveCastlingWhiteRook(static_cast<unsigned>(Squares::A1), static_cast<unsigned>(Squares::D1))
                    : MoveCastlingBlackRook(static_cast<unsigned>(Squares::A8), static_cast<unsigned>(Squares::D8));

    if (moveType & static_cast<uint8_t>(MoveTypes::PROMOTION))
    {
        PieceType promotion = static_cast<PieceType>(
            whiteToMove ? std::array{Constants::WHITE_KNIGHT_INDEX, Constants::WHITE_BISHOP_INDEX,
                                     Constants::WHITE_ROOK_INDEX, Constants::WHITE_QUEEN_INDEX}[moveType & 3U]
                        : std::array{Constants::BLACK_KNIGHT_INDEX, Constants::BLACK_BISHOP_INDEX,
                                     Constants::BLACK_ROOK_INDEX, Constants::BLACK_QUEEN_INDEX}[moveType & 3U]);
        material += GetPieceMaterial(promotion).value() - GetPieceMaterial(movingPiece).value();
        AddPiece(to, promotion);
    }
    else
        AddPiece(to, movingPiece);

    whiteToMove = !whiteToMove;
    hashBoard.HashSide();

    if (whiteToMove ? Check::IsInCheck<Colors::BLACK>(*this) : Check::IsInCheck<Colors::WHITE>(*this))
    {
        TakeBack();
        return false;
    }
    return true;
}

constexpr void Board::UpdateCastlingRights(unsigned int from, unsigned int to) noexcept
{
    hashBoard.HashCastling(castlingRights);
    castlingRights &= castlePermission[from];
    castlingRights &= castlePermission[to];
    hashBoard.HashCastling(castlingRights);
}

constexpr void Board::MoveCastlingWhiteRook(unsigned int from, unsigned int to)
{
    RemovePiece(from, Constants::WHITE_ROOK_INDEX);
    AddPiece(to, Constants::WHITE_ROOK_INDEX);
}

constexpr void Board::MoveCastlingBlackRook(unsigned int from, unsigned int to)
{
    RemovePiece(from, Constants::BLACK_ROOK_INDEX);
    AddPiece(to, Constants::BLACK_ROOK_INDEX);
}

constexpr void Board::MakeDoublePawnPush(unsigned int enPassantSquare) noexcept
{
    SetBit(enPassantBitBoard, enPassantSquare);
    hashBoard.HashEnPassant(enPassantBitBoard);
}

constexpr void Board::MakeCapture(unsigned int to, PieceType capturedPiece, unsigned int moveType)
{
    fiftyMoves = 0;
    if (moveType != static_cast<uint8_t>(MoveTypes::ENPASSANT))
    {
        RemovePiece(to, capturedPiece);
        material -= GetPieceMaterial(capturedPiece).value();
    }
    else
    {
        whiteToMove ? RemovePiece(to - 8, Constants::BLACK_PAWN_INDEX)
                    : RemovePiece(to + 8, Constants::WHITE_PAWN_INDEX);
        material -= whiteToMove ? GetPieceMaterial(Constants::BLACK_PAWN_INDEX).value()
                                : GetPieceMaterial(Constants::WHITE_PAWN_INDEX).value();
    }
}

void Board::TakeBack()
{
    if (historyPly == 0)
        throw std::runtime_error("Empty history");

    const History &lastPosition = history[--historyPly];

    const auto from = lastPosition.move.GetFromSquare();
    const auto to = lastPosition.move.GetToSquare();
    const auto moveType = lastPosition.move.GetMoveType();
    const auto movingPiece = lastPosition.movedPiece;
    const auto capturedPiece = lastPosition.capturedPiece;

    whiteToMove = !whiteToMove;
    fiftyMoves = lastPosition.fiftyMoves;
    enPassantBitBoard = lastPosition.enPassant;
    castlingRights = lastPosition.castlingRights;

    if (moveType == static_cast<uint8_t>(MoveTypes::KING_CASTLE))
        whiteToMove ? MoveCastlingWhiteRook(static_cast<unsigned>(Squares::F1), static_cast<unsigned>(Squares::H1))
                    : MoveCastlingBlackRook(static_cast<unsigned>(Squares::F8), static_cast<unsigned>(Squares::H8));

    else if (moveType == static_cast<uint8_t>(MoveTypes::QUEEN_CASTLE))
        whiteToMove ? MoveCastlingWhiteRook(static_cast<unsigned>(Squares::D1), static_cast<unsigned>(Squares::A1))
                    : MoveCastlingBlackRook(static_cast<unsigned>(Squares::D8), static_cast<unsigned>(Squares::A8));

    else if (moveType & static_cast<uint8_t>(MoveTypes::PROMOTION))
    {
        PieceType promotion = static_cast<PieceType>(
            whiteToMove ? std::array{Constants::WHITE_KNIGHT_INDEX, Constants::WHITE_BISHOP_INDEX,
                                     Constants::WHITE_ROOK_INDEX, Constants::WHITE_QUEEN_INDEX}[moveType & 3U]
                        : std::array{Constants::BLACK_KNIGHT_INDEX, Constants::BLACK_BISHOP_INDEX,
                                     Constants::BLACK_ROOK_INDEX, Constants::BLACK_QUEEN_INDEX}[moveType & 3U]);
        material -= GetPieceMaterial(promotion).value() - GetPieceMaterial(movingPiece).value();
        RemovePiece(to, promotion);
        AddPiece(to, movingPiece);
    }

    RemovePiece(to, movingPiece);
    AddPiece(from, movingPiece);

    if (moveType == static_cast<uint8_t>(MoveTypes::ENPASSANT))
    {
        whiteToMove ? AddPiece(to - 8, Constants::BLACK_PAWN_INDEX) : AddPiece(to + 8, Constants::WHITE_PAWN_INDEX);
        material += whiteToMove ? GetPieceMaterial(Constants::BLACK_PAWN_INDEX).value()
                                : GetPieceMaterial(Constants::WHITE_PAWN_INDEX).value();
    }

    else if (moveType & static_cast<uint8_t>(MoveTypes::CAPTURE))
    {
        material += GetPieceMaterial(capturedPiece).value();
        AddPiece(to, capturedPiece);
    }

    hashBoard.SetHash(lastPosition.hash);
}

constexpr void Board::RemovePiece(unsigned int position, PieceType pieceType)
{
    pieceList[position] = Constants::ALL_EMPTY_INDEX;
    ClearBit(bitBoards[pieceType], position);
    ClearBit(bitBoards[IsBlackMoving(pieceType) ? Constants::ALL_BLACK_INDEX : Constants::ALL_WHITE_INDEX], position);
    SetBit(bitBoards[Constants::ALL_EMPTY_INDEX], position);
    hashBoard.HashPiece(position, static_cast<uint8_t>(pieceType));
}

constexpr void Board::AddPiece(unsigned int position, PieceType pieceType)
{
    pieceList[position] = pieceType;
    SetBit(bitBoards[pieceType], position);
    SetBit(bitBoards[IsBlackMoving(pieceType) ? Constants::ALL_BLACK_INDEX : Constants::ALL_WHITE_INDEX], position);
    ClearBit(bitBoards[Constants::ALL_EMPTY_INDEX], position);
    hashBoard.HashPiece(position, static_cast<uint8_t>(pieceType));
}

template <MoveTypes moveType> FixedList<Move> Board::GetMoveList() const noexcept
{
    MoveGenerator moveGenerator;
    if (whiteToMove)
        moveGenerator.GenerateMoves<Colors::WHITE, moveType>(*this);
    else
        moveGenerator.GenerateMoves<Colors::BLACK, moveType>(*this);
    return moveGenerator.GetMoveList();
}

bool Board::MakeMove(std::string_view notation)
{
    const std::regex moveRegex{"[a-h][1-8][a-h][1-8][q|r|b|n]?"};

    if (std::match_results<std::string_view::const_iterator> iterator;
        !std::regex_match(begin(notation), end(notation), iterator, moveRegex))
        throw std::runtime_error("INVALID MOVE FORMAT. Use for example e2e4.");

    const auto from = static_cast<unsigned>(Move::NotationToSquare(notation.substr(0, 2)));
    const auto to = static_cast<unsigned>(Move::NotationToSquare(notation.substr(2, 2)));

    char promoted = ' ';
    if (notation.size() == 5)
        promoted = notation.back();

    uint8_t moveType = 0;
    if (promoted != ' ')
    {
        moveType = static_cast<uint8_t>(GetPromotionType(promoted).value());
        if (const PieceType capturedPiece{GetPieceList()[to]}; capturedPiece != Constants::ALL_EMPTY_INDEX)
            moveType |= static_cast<uint8_t>(MoveTypes::CAPTURE);
    }

    const auto decodedPromotion = Move(from, to, moveType);
    const auto decodedMove = decodedPromotion.RemoveMoveType();

    for (const auto &move : GetMoveList<MoveTypes::ALL_MOVE>())
        if ((move.RemoveMoveType()) == decodedMove)
            return (moveType == 0) ? MakeMove(move) : MakeMove(decodedPromotion);

    return false;
}

template VIXEN_API FixedList<Move> Board::GetMoveList<MoveTypes::CAPTURE>() const noexcept;

template VIXEN_API FixedList<Move> Board::GetMoveList<MoveTypes::ALL_MOVE>() const noexcept;

} // namespace vixen