#pragma once

#include <map>
#include <algorithm>

#if defined _WIN32
#define VIXEN_API __declspec(dllexport)
#else
#define VIXEN_API __attribute__ ((visibility ("default")))
#endif
#define START_POSITION "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define TESTPOS1 "2rq1rk1/3bppbp/p5p1/1ppPP3/2n2B2/2P1Q1PP/P2N1PB1/R4RK1 w - - 1 18"
#define TESTPOS2 "rnbqkb1r/pp1ppppp/5n2/8/3p1B2/4P3/PPP2PPP/RN1QKBNR w KQkq - 0 4"

//#define DEBUG

namespace Vixen
{

    enum Ranks
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

    enum Files
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

    enum Squares
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

    enum MoveTypes
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

        QUEEN_PROMO_CAPTURE
    };

    enum CastlingRights
    {
        BQCA = 1, BKCA = 2, WQCA = 4, WKCA = 8
    };

    enum class Colors
    {
        WHITE, BLACK
    };

    enum class Slider
    {
        BISHOP, ROOK
    };

    typedef int Move;

    typedef uint64_t BitBoard;

    typedef std::array<BitBoard, 15> BitBoards;

    typedef std::array<int, 2> Direction;

    typedef std::array<Direction, 4> SliderDirections;

    typedef std::array<Direction, 8> AntiSliderDirections;

    typedef std::array<Direction, 2> PawnDirections;

    typedef std::map<int, std::map<char, BitBoard>> PieceHashKeys;

    typedef uint64_t SideHashKey;

    typedef uint64_t PositionKey;

    typedef std::array<BitBoard, 16> CastleHashKeys;

    constexpr int MAX_SQUARE_INDEX = 63;

    constexpr int SQUARE_NUMBER = 64;

    constexpr int COLOR_NUMBER = 2;

    constexpr int BISHOP_ATTACK_TABLE_SIZE = 0x1480;

    constexpr int ROOK_ATTACK_TABLE_SIZE = 0x19000;

    constexpr BitBoard EMPTY_BOARD = 0ULL;

    static constexpr std::array<char, 12> pieceKeys = {'P', 'N', 'B', 'R', 'Q', 'K',
                                                       'p', 'n', 'b', 'r', 'q', 'k'};

    static constexpr std::array<char, 12> blackPieceKeys = {'p', 'n', 'b', 'r', 'q', 'k'};

    static constexpr std::array<std::pair<char, int>, 15> pieceMap = {std::make_pair('P', 0),
                                                                      std::make_pair('N', 1),
                                                                      std::make_pair('B', 2),
                                                                      std::make_pair('R', 3),
                                                                      std::make_pair('Q', 4),
                                                                      std::make_pair('K', 5),
                                                                      std::make_pair('p', 6),
                                                                      std::make_pair('n', 7),
                                                                      std::make_pair('b', 8),
                                                                      std::make_pair('r', 9),
                                                                      std::make_pair('q', 10),
                                                                      std::make_pair('k', 11),
                                                                      std::make_pair('F', 12),
                                                                      std::make_pair('S', 13),
                                                                      std::make_pair(' ', 14)};
//    C++20 find_if function is constexpr
//    inline constexpr auto GetPieceIndex(char c)
//    {
//        auto iterator = std::find_if(std::begin(pieceMap), std::end(pieceMap), [c](auto p){return p.first == c;});
//        return (iterator != std::end(pieceMap)) ? iterator->second : -1;
//    }

    inline constexpr auto GetPieceIndex(char c)
    {
        for (const auto& i : pieceMap)
        {
            if (i.first == c)
                return i.second;
        }
        return -1;
    }

    inline constexpr BitBoard SquareToBitBoard(int square)
    {
        if (square < 0)
            return EMPTY_BOARD;
        return static_cast<BitBoard>(1U) << static_cast<unsigned>(square);
    }

    inline constexpr bool IsValidCoordinate(int file, int rank)
    {
        return file >= 0 && rank >= 0 && file < 8 && rank < 8;
    }

    namespace
    {
    template<class T>
    inline constexpr bool IsBitSet(const T &bits, unsigned position)
    {
        return bits & (1ULL << position);
    }

    template<class T>
    inline constexpr void SetBit(T &bitBoard, unsigned position)
    {
        bitBoard |= 1ULL << position;
    }

    template<class T>
    inline constexpr void ClearBit(T &bitBoard, unsigned position)
    {
        bitBoard &= ~(1ULL << position);
    }

    template<Colors pawnColor>
    inline constexpr BitBoard PushPawns(BitBoard pawns)
    {
        return (pawnColor == Colors::WHITE) ? pawns << 8U : pawns >> 8U;
    }

    template<Colors pawnColor>
    inline constexpr BitBoard PawnCaptureLeft(BitBoard pawns)
    {
        pawns &= (pawnColor == Colors::WHITE) ? ~FILEA : ~FILEH;
        return (pawnColor == Colors::WHITE) ? pawns << 9U : pawns >> 9U;
    }

    template<Colors pawnColor>
    inline constexpr BitBoard PawnCaptureRight(BitBoard pawns)
    {
        pawns &= (pawnColor == Colors::WHITE) ? ~FILEH : ~FILEA;
        return (pawnColor == Colors::WHITE) ? pawns << 7U : pawns >> 7U;
    }
    }


    inline constexpr int PopCount(BitBoard bitBoard)
    {
        return __builtin_popcountll(bitBoard);
    }

    inline constexpr int TrailingZeroCount(BitBoard bitBoard)
    {
        return __builtin_ctzll(bitBoard);
    }

    inline auto SquareToNotation(unsigned square)
    {
        std::string notation;
        notation.push_back(static_cast<char>(7 - square % 8 + 'a'));
        notation.push_back(static_cast<char>(square / 8 + '1'));
        return notation;
    }

    inline auto NotationToSquare(const std::string &notation)
    {
        if (notation.at(0) < 'a' || notation.at(0) > 'h' ||
            notation.at(1) < '1' || notation.at(1) > '8')
            return -1;
        return 7 - (notation.at(0) - 'a') + 8 * (notation.at(1) - '1');
    }

    inline constexpr int GetPosition(BitBoard &bitBoard)
    {
        int from = TrailingZeroCount(bitBoard);
        bitBoard &= bitBoard - 1;
        return from;
    }

    inline constexpr Move CreateMove(unsigned from, unsigned to, uint8_t moveType)
    {
        return moveType << 12U | to << 6U | from;
    }

    inline constexpr bool IsMovingPawn(char movingPiece)
    {
        return movingPiece == 'P' || movingPiece == 'p';
    }

    /**
     * Returns true if the moving piece is black
     * C++20 find function will be constexpr.
     * @param c 
     * @return 
     */
    inline bool IsBlackMoving(char c)
    {
        return std::find(begin(blackPieceKeys), end(blackPieceKeys), c) != end(blackPieceKeys);
    }
}
