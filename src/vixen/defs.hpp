#ifndef SRC_VIXEN_DEFS_HPP_
#define SRC_VIXEN_DEFS_HPP_

#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>
#include <optional>
#include <stdexcept>
#if _MSC_VER >= 1910
#include <sstream>
#endif
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

#if _MSC_VER >= 1910
#define VIXEN_API __declspec(dllexport)
#else
#define VIXEN_API __attribute__((visibility("default")))
#endif

namespace vixen
{

enum class Ranks : std::uint64_t
{
    RANK1 = 255ULL,
    RANK2 = 65280ULL,
    RANK3 = 16711680ULL,
    RANK6 = 280375465082880ULL,
    RANK7 = 71776119061217280ULL,
    RANK8 = 18374686479671623680ULL,
    RANK12 = 65535ULL,
    RANK18 = 18374686479671623935ULL,
    RANK78 = 18446462598732840960ULL
};

enum class Files : std::uint64_t
{
    FILEA = 9259542123273814144ULL,
    FILEB = 4629771061636907072ULL,
    FILEC = 2314885530818453536ULL,
    FILED = 1157442765409226768ULL,
    FILEE = 578721382704613384ULL,
    FILEF = 289360691352306692ULL,
    FILEG = 144680345676153346ULL,
    FILEH = 72340172838076673ULL
};

// clang-format off
enum class Squares : uint8_t
{
    H1, G1, F1, E1, D1, C1, B1, A1,
    H2, G2, F2, E2, D2, C2, B2, A2,
    H3, G3, F3, E3, D3, C3, B3, A3,
    H4, G4, F4, E4, D4, C4, B4, A4,
    H5, G5, F5, E5, D5, C5, B5, A5,
    H6, G6, F6, E6, D6, C6, B6, A6,
    H7, G7, F7, E7, D7, C7, B7, A7,
    H8, G8, F8, E8, D8, C8, B8, A8
};
// clang-format on

enum class MoveTypes : std::uint16_t
{
    QUIET_MOVE,

    DOUBLE_PAWN_PUSH,

    KING_CASTLE = 2,

    QUEEN_CASTLE,

    CAPTURE,

    ENPASSANT,

    PROMOTION = 8,

    KNIGHT_PROMOTION = 8,

    BISHOP_PROMOTION,

    ROOK_PROMOTION,

    QUEEN_PROMOTION,

    KNIGHT_PROMO_CAPTURE = 12,

    BISHOP_PROMO_CAPTURE,

    ROOK_PROMO_CAPTURE,

    QUEEN_PROMO_CAPTURE,

    ALL_MOVE
};

enum class CastlingRights : uint8_t
{
    /// 0001
    BKCA = 1,

    /// 0010
    BQCA = 2,

    /// 0100
    WKCA = 4,

    /// 1000
    WQCA = 8
};

enum class Colors
{
    WHITE,
    BLACK
};

enum class Slider
{
    BISHOP,
    ROOK
};

static constexpr unsigned BITBOARD_ELEMENT_NUMBER = 15U;

static constexpr unsigned COORDIBATES_2D = 2U;

static constexpr unsigned SQUARE_NUMBER = 64U;

static constexpr unsigned PAWN_CAPTURE_DIRECTION_NUMBER = 4U;

static constexpr unsigned SLIDER_DIRECTION_NUMBER = 4U;

static constexpr unsigned ANTI_SLIDER_DIRECTION_NUMBER = 8U;

static constexpr unsigned PIECE_TYPE_NUMBER = 13U;

static constexpr unsigned CASTLING_STATE_NUMBER = 16U;

using PieceType = std::uint16_t;

using BitBoard = std::uint64_t;

using BitBoards = std::array<BitBoard, BITBOARD_ELEMENT_NUMBER>;

using DirectionVector = std::array<int, COORDIBATES_2D>;

using SliderDirections = std::array<DirectionVector, SLIDER_DIRECTION_NUMBER>;

using AntiSliderDirections = std::array<DirectionVector, ANTI_SLIDER_DIRECTION_NUMBER>;

using PawnDirections = std::array<DirectionVector, PAWN_CAPTURE_DIRECTION_NUMBER>;

using PieceHashKeys = std::array<std::array<BitBoard, PIECE_TYPE_NUMBER>, 64>;

using SideHashKey = std::uint64_t;

using PositionKey = std::uint64_t;

using CastleHashKeys = std::array<BitBoard, CASTLING_STATE_NUMBER>;

struct Constants
{
    static constexpr unsigned MAX_MOVELIST_SIZE = 256U;

    static constexpr unsigned MAX_SQUARE_INDEX = 63U;

    static constexpr unsigned SQUARE_NUMBER = 64U;

    static constexpr unsigned COLOR_NUMBER = 2U;

    static constexpr unsigned BISHOP_ATTACK_TABLE_SIZE = 0x1480U;

    static constexpr unsigned ROOK_ATTACK_TABLE_SIZE = 0x19000U;

    static constexpr std::size_t WHITE_PAWN_INDEX = 0U;

    static constexpr std::size_t WHITE_KNIGHT_INDEX = 1U;

    static constexpr std::size_t WHITE_BISHOP_INDEX = 2U;

    static constexpr std::size_t WHITE_ROOK_INDEX = 3U;

    static constexpr std::size_t WHITE_QUEEN_INDEX = 4U;

    static constexpr std::size_t WHITE_KING_INDEX = 5U;

    static constexpr std::size_t BLACK_PAWN_INDEX = 6U;

    static constexpr std::size_t BLACK_KNIGHT_INDEX = 7U;

    static constexpr std::size_t BLACK_BISHOP_INDEX = 8U;

    static constexpr std::size_t BLACK_ROOK_INDEX = 9U;

    static constexpr std::size_t BLACK_QUEEN_INDEX = 10U;

    static constexpr std::size_t BLACK_KING_INDEX = 11U;

    static constexpr std::size_t ALL_WHITE_INDEX = 12U;

    static constexpr std::size_t ALL_BLACK_INDEX = 13U;

    static constexpr std::size_t ALL_EMPTY_INDEX = 14U;

    static constexpr std::size_t ALL_OCCUPIED_INDEX = 15U;

    static constexpr BitBoard EMPTY_BOARD = 0ULL;

    static constexpr std::string_view START_POSITION = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    static constexpr std::string_view TESTPOS1 = "2rq1rk1/3bppbp/p5p1/1ppPP3/2n2B2/2P1Q1PP/P2N1PB1/R4RK1 w - - 1 18";

    static constexpr std::string_view TESTPOS2 = "rnbqkb1r/pp1ppppp/5n2/8/3p1B2/4P3/PPP2PPP/RN1QKBNR w KQkq - 0 4";

    static constexpr std::string_view TESTPOS3 = "r1bq1b1r/ppp2kpp/2n5/3np3/2B5/5Q2/PPPP1PPP/RNB1K2R b KQ - 0 1";

    static constexpr std::string_view CASTLERIGHTS = "kqKQ";

    static constexpr std::array<PieceType, 12> pieceKeys = {
        Constants::WHITE_PAWN_INDEX, Constants::WHITE_KNIGHT_INDEX, Constants::WHITE_BISHOP_INDEX,
        Constants::WHITE_ROOK_INDEX, Constants::WHITE_QUEEN_INDEX,  Constants::WHITE_KING_INDEX,
        Constants::BLACK_PAWN_INDEX, Constants::BLACK_KNIGHT_INDEX, Constants::BLACK_BISHOP_INDEX,
        Constants::BLACK_ROOK_INDEX, Constants::BLACK_QUEEN_INDEX,  Constants::BLACK_KING_INDEX};

    static constexpr std::array<unsigned char, 6> blackPieceKeys = {'p', 'n', 'b', 'r', 'q', 'k'};

    static constexpr std::array<std::pair<unsigned char, unsigned>, 15> pieceMap = {
        std::make_pair(' ', 14U), std::make_pair('B', 2U),  std::make_pair('F', 12U), std::make_pair('K', 5U),
        std::make_pair('N', 1U),  std::make_pair('P', 0U),  std::make_pair('Q', 4U),  std::make_pair('R', 3U),
        std::make_pair('S', 13U), std::make_pair('b', 8U),  std::make_pair('k', 11U), std::make_pair('n', 7U),
        std::make_pair('p', 6U),  std::make_pair('q', 10U), std::make_pair('r', 9U)};

    static constexpr std::array<std::pair<PieceType, int>, 12> materialMap = {
        std::make_pair(Constants::WHITE_PAWN_INDEX, 100),    std::make_pair(Constants::WHITE_KNIGHT_INDEX, 300),
        std::make_pair(Constants::WHITE_BISHOP_INDEX, 300),  std::make_pair(Constants::WHITE_ROOK_INDEX, 500),
        std::make_pair(Constants::WHITE_QUEEN_INDEX, 900),   std::make_pair(Constants::WHITE_KING_INDEX, 2000),
        std::make_pair(Constants::BLACK_PAWN_INDEX, -100),   std::make_pair(Constants::BLACK_KNIGHT_INDEX, -300),
        std::make_pair(Constants::BLACK_BISHOP_INDEX, -300), std::make_pair(Constants::BLACK_ROOK_INDEX, -500),
        std::make_pair(Constants::BLACK_QUEEN_INDEX, -900),  std::make_pair(Constants::BLACK_KING_INDEX, -2000)};

    static constexpr std::array<std::pair<unsigned char, int>, 4> promotionMap = {
        std::make_pair('q', static_cast<int>(MoveTypes::QUEEN_PROMOTION)),
        std::make_pair('r', static_cast<int>(MoveTypes::ROOK_PROMOTION)),
        std::make_pair('b', static_cast<int>(MoveTypes::BISHOP_PROMOTION)),
        std::make_pair('n', static_cast<int>(MoveTypes::KNIGHT_PROMOTION))};

    static constexpr SliderDirections rookDirections = {{{-1, 0}, {0, -1}, {1, 0}, {0, 1}}};

    static constexpr SliderDirections bishopDirections = {{{-1, -1}, {-1, 1}, {1, -1}, {1, 1}}};

    static constexpr std::array RookMagic = {
        0xA180022080400230ULL, 0x0040100040022000ULL, 0x0080088020001002ULL, 0x0080080280841000ULL,
        0x4200042010460008ULL, 0x04800A0003040080ULL, 0x0400110082041008ULL, 0x008000A041000880ULL,
        0x10138001A080C010ULL, 0x0000804008200480ULL, 0x00010011012000C0ULL, 0x0022004128102200ULL,
        0x000200081201200CULL, 0x202A001048460004ULL, 0x0081000100420004ULL, 0x4000800380004500ULL,
        0x0000208002904001ULL, 0x0090004040026008ULL, 0x0208808010002001ULL, 0x2002020020704940ULL,
        0x8048010008110005ULL, 0x6820808004002200ULL, 0x0A80040008023011ULL, 0x00B1460000811044ULL,
        0x4204400080008EA0ULL, 0xB002400180200184ULL, 0x2020200080100380ULL, 0x0010080080100080ULL,
        0x2204080080800400ULL, 0x0000A40080360080ULL, 0x02040604002810B1ULL, 0x008C218600004104ULL,
        0x8180004000402000ULL, 0x488C402000401001ULL, 0x4018A00080801004ULL, 0x1230002105001008ULL,
        0x8904800800800400ULL, 0x0042000C42003810ULL, 0x008408110400B012ULL, 0x0018086182000401ULL,
        0x2240088020C28000ULL, 0x001001201040C004ULL, 0x0A02008010420020ULL, 0x0010003009010060ULL,
        0x0004008008008014ULL, 0x0080020004008080ULL, 0x0282020001008080ULL, 0x50000181204A0004ULL,
        0x48FFFE99FECFAA00ULL, 0x48FFFE99FECFAA00ULL, 0x497FFFADFF9C2E00ULL, 0x613FFFDDFFCE9200ULL,
        0xFFFFFFE9FFE7CE00ULL, 0xFFFFFFF5FFF3E600ULL, 0x0010301802830400ULL, 0x510FFFF5F63C96A0ULL,
        0xEBFFFFB9FF9FC526ULL, 0x61FFFEDDFEEDAEAEULL, 0x53BFFFEDFFDEB1A2ULL, 0x127FFFB9FFDFB5F6ULL,
        0x411FFFDDFFDBF4D6ULL, 0x0801000804000603ULL, 0x0003FFEF27EEBE74ULL, 0x7645FFFECBFEA79EULL};

    static constexpr std::array BishopMagic = {
        0xFFEDF9FD7CFCFFFFULL, 0xFC0962854A77F576ULL, 0x5822022042000000ULL, 0x2CA804A100200020ULL,
        0x0204042200000900ULL, 0x2002121024000002ULL, 0xFC0A66C64A7EF576ULL, 0x7FFDFDFCBD79FFFFULL,
        0xFC0846A64A34FFF6ULL, 0xFC087A874A3CF7F6ULL, 0x1001080204002100ULL, 0x1810080489021800ULL,
        0x0062040420010A00ULL, 0x5028043004300020ULL, 0xFC0864AE59B4FF76ULL, 0x3C0860AF4B35FF76ULL,
        0x73C01AF56CF4CFFBULL, 0x41A01CFAD64AAFFCULL, 0x040C0422080A0598ULL, 0x4228020082004050ULL,
        0x0200800400E00100ULL, 0x020B001230021040ULL, 0x7C0C028F5B34FF76ULL, 0xFC0A028E5AB4DF76ULL,
        0x0020208050A42180ULL, 0x001004804B280200ULL, 0x2048020024040010ULL, 0x0102C04004010200ULL,
        0x020408204C002010ULL, 0x02411100020080C1ULL, 0x102A008084042100ULL, 0x0941030000A09846ULL,
        0x0244100800400200ULL, 0x4000901010080696ULL, 0x0000280404180020ULL, 0x0800042008240100ULL,
        0x0220008400088020ULL, 0x04020182000904C9ULL, 0x0023010400020600ULL, 0x0041040020110302ULL,
        0xDCEFD9B54BFCC09FULL, 0xF95FFA765AFD602BULL, 0x1401210240484800ULL, 0x0022244208010080ULL,
        0x1105040104000210ULL, 0x2040088800C40081ULL, 0x43FF9A5CF4CA0C01ULL, 0x4BFFCD8E7C587601ULL,
        0xFC0FF2865334F576ULL, 0xFC0BF6CE5924F576ULL, 0x80000B0401040402ULL, 0x0020004821880A00ULL,
        0x8200002022440100ULL, 0x0009431801010068ULL, 0xC3FFB7DC36CA8C89ULL, 0xC3FF8A54F4CA2C89ULL,
        0xFFFFFCFCFD79EDFFULL, 0xFC0863FCCB147576ULL, 0x040C000022013020ULL, 0x2000104000420600ULL,
        0x0400000260142410ULL, 0x0800633408100500ULL, 0xFC087E8E4BB2F736ULL, 0x43FF9E4EF4CA2C89ULL};

    static constexpr AntiSliderDirections knightOffset = {
        {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}}};

    static constexpr AntiSliderDirections kingOffset = {
        {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}};

    static constexpr PawnDirections pawnDirections = {{{-1, 1}, {1, 1}}};

    static inline std::unordered_map<CastlingRights, uint8_t> rookSquareAfterCastling = {
        {{CastlingRights::BQCA, static_cast<uint8_t>(Squares::D8)},
         {CastlingRights::BKCA, static_cast<uint8_t>(Squares::F8)},
         {CastlingRights::WQCA, static_cast<uint8_t>(Squares::D1)},
         {CastlingRights::WKCA, static_cast<uint8_t>(Squares::F1)}}};

    static inline std::unordered_map<CastlingRights, uint8_t> castlingLookUp = {
        {{CastlingRights::WKCA, static_cast<uint8_t>(3U)},
         {CastlingRights::WQCA, static_cast<uint8_t>(2U)},
         {CastlingRights::BKCA, static_cast<uint8_t>(1U)},
         {CastlingRights::BQCA, static_cast<uint8_t>(0U)}}};
};

constexpr bool IsWhitePiece(char piece) noexcept
{
    return piece >= 'A' && piece < 'Z';
}

constexpr std::optional<int> GetPieceMaterial(PieceType c) noexcept
{
    if (const auto &it =
            std::ranges::find_if(Constants::materialMap, [c](const auto &element) { return c == element.first; });
        it != end(Constants::materialMap))
        return it->second;
    else
        return {};
}

constexpr std::optional<int> GetPromotionType(char c) noexcept
{
    if (const auto &it =
            std::ranges::find_if(Constants::promotionMap, [c](const auto &element) { return c == element.first; });
        it != end(Constants::promotionMap))
        return it->second;
    else
        return {};
}

constexpr BitBoard SquareToBitBoard(int square) noexcept
{
    if (square < 0)
        return Constants::EMPTY_BOARD;
    return static_cast<BitBoard>(1U) << static_cast<unsigned>(square);
}

constexpr bool IsValidCoordinate(int file, int rank) noexcept
{
    return file >= 0 && rank >= 0 && file < 8 && rank < 8;
}

namespace
{
template <class T, class... Position> constexpr bool IsBitSet(T bits, Position &&...p) noexcept
{
    return ((bits & (1ULL << static_cast<BitBoard>(p))) && ...);
}

template <class T> constexpr void SetBit(T &bitBoard, unsigned position) noexcept
{
    bitBoard |= static_cast<T>(1ULL << position);
}

template <class T> constexpr void ClearBit(T &bitBoard, unsigned position) noexcept
{
    bitBoard &= ~(1ULL << position);
}

template <Colors pawnColor> constexpr BitBoard PushPawns(BitBoard pawns) noexcept
{
    return (pawnColor == Colors::WHITE) ? pawns << 8U : pawns >> 8U;
}

template <Colors pawnColor> constexpr BitBoard PawnCaptureLeft(BitBoard pawns) noexcept
{
    pawns &= (pawnColor == Colors::WHITE) ? ~static_cast<BitBoard>(Files::FILEA) : ~static_cast<BitBoard>(Files::FILEH);
    return (pawnColor == Colors::WHITE) ? pawns << 9U : pawns >> 9U;
}

template <Colors pawnColor> constexpr BitBoard PawnCaptureRight(BitBoard pawns) noexcept
{
    pawns &= (pawnColor == Colors::WHITE) ? ~static_cast<BitBoard>(Files::FILEH) : ~static_cast<BitBoard>(Files::FILEA);
    return (pawnColor == Colors::WHITE) ? pawns << 7U : pawns >> 7U;
}
} // namespace

constexpr unsigned PopCount(BitBoard bitBoard) noexcept
{
    return static_cast<unsigned>(std::popcount(bitBoard));
}

constexpr unsigned TrailingZeroCount(BitBoard bitBoard) noexcept
{
    return static_cast<unsigned>(std::countr_zero(bitBoard));
}

constexpr unsigned GetPosition(BitBoard &bitBoard) noexcept
{
    const auto from = TrailingZeroCount(bitBoard);
    bitBoard &= bitBoard - 1;
    return from;
}

constexpr bool IsMovingPawn(PieceType movingPiece) noexcept
{
    return movingPiece == Constants::WHITE_PAWN_INDEX || movingPiece == Constants::BLACK_PAWN_INDEX;
}

/**
 * Returns true if the moving piece is black
 * @param c
 * @return
 */
constexpr bool IsBlackMoving(PieceType c) noexcept
{
    return c >= Constants::BLACK_PAWN_INDEX;
}

template <class Iterator> constexpr auto PickBest(const Iterator &begin, const Iterator &end)
{
    auto maxElement = std::max_element(begin, end);
    std::swap(*begin, *maxElement);
    return *begin;
}

} // namespace vixen

#endif // SRC_VIXEN_DEFS_HPP_