#include "move_generator.h"
#include "board.h"

#include <iostream>

namespace Vixen
{
    MoveGenerator::MoveGenerator(const Board &board)
    {
        auto bitBoards = board.GetBitBoards();
        auto enPassant = board.GetEnPassant();
        InitKnightKingAttack();
        InitPawnAttack();
        board.IsWhiteToMove() ? GenerateAllMoves<Colors::WHITE>(bitBoards, enPassant)
                              : GenerateAllMoves<Colors::BLACK>(bitBoards, enPassant);
#ifdef DEBUG
        PrintMoveList();
#endif // DEBUG
    }

    void MoveGenerator::PrintMoveList()
    {
        for (auto &move : moveList)
            std::cout << move << std::endl;
    }

    template<Colors sideToMove>
    void MoveGenerator::GenerateAllMoves(const BitBoards &bitBoards, BitBoard enPassant)
    {
        auto enemies = sideToMove == Colors::WHITE ? bitBoards.at('S') : bitBoards.at('F');
        auto empties = bitBoards.at(' ');
        GenerateCaptureMoves<sideToMove>(bitBoards, enemies, enPassant);
        GenerateQuietMoves<sideToMove>(bitBoards, empties);
    }

    template<Colors sideToMove>
    void MoveGenerator::GenerateQuietMoves(const BitBoards &bitBoards, BitBoard targets)
    {
        auto pawns = sideToMove == Colors::WHITE ? bitBoards.at('P') : bitBoards.at('p');
        auto kings = sideToMove == Colors::WHITE ? bitBoards.at('K') : bitBoards.at('k');
        auto knights = sideToMove == Colors::WHITE ? bitBoards.at('N') : bitBoards.at('n');
        auto bishops = sideToMove == Colors::WHITE ? bitBoards.at('B') : bitBoards.at('b');
        auto rooks = sideToMove == Colors::WHITE ? bitBoards.at('R') : bitBoards.at('r');
        auto queens = sideToMove == Colors::WHITE ? bitBoards.at('Q') : bitBoards.at('q');
        auto blockers = ~bitBoards.at(' ');

        auto pawnOffset = sideToMove == Colors::WHITE ? 8 : -8;
        auto promotionRanks = sideToMove == Colors::WHITE ? RANK8 : RANK1;
        auto doublePushStart = sideToMove == Colors::WHITE ? RANK3 : RANK6;

        auto onePawnPush = PushPawns<sideToMove>(pawns) & ~blockers & ~promotionRanks;
        auto doublePawnPush = ((onePawnPush & doublePushStart) << pawnOffset) & ~blockers;

        GeneratePawnMoves(pawnOffset, onePawnPush);
        GeneratePawnMoves(2 * pawnOffset, doublePawnPush);
        GenerateAntiSliderMoves(targets, knights, knightAttack);
        GenerateAntiSliderMoves(targets, kings, kingAttack);
        GenerateSliderMoves<Slider::BISHOP>(bishops | queens, blockers, targets);
        GenerateSliderMoves<Slider::ROOK>(rooks | queens, blockers, targets);
    }

    void MoveGenerator::GeneratePawnMoves(int pawnOffset, BitBoard pawnPushed)
    {
        while (pawnPushed)
        {
            auto to = GetPosition(pawnPushed);
            moveList.emplace_back(MakeMove(to - pawnOffset, to));
        }
    }

    template<Colors sideToMove>
    void MoveGenerator::GenerateCaptureMoves(const BitBoards &bitBoards, BitBoard targets, BitBoard enPassant)
    {
        auto pawns = sideToMove == Colors::WHITE ? bitBoards.at('P') : bitBoards.at('p');
        auto kings = sideToMove == Colors::WHITE ? bitBoards.at('K') : bitBoards.at('k');
        auto knights = sideToMove == Colors::WHITE ? bitBoards.at('N') : bitBoards.at('n');
        auto bishops = sideToMove == Colors::WHITE ? bitBoards.at('B') : bitBoards.at('b');
        auto rooks = sideToMove == Colors::WHITE ? bitBoards.at('R') : bitBoards.at('r');
        auto queens = sideToMove == Colors::WHITE ? bitBoards.at('Q') : bitBoards.at('q');
        auto blockers = ~bitBoards.at(' ');
        GenerateAntiSliderMoves(targets | enPassant, pawns, pawnAttack[static_cast<int>(sideToMove)]);
        GenerateAntiSliderMoves(targets, knights, knightAttack);
        GenerateAntiSliderMoves(targets, kings, kingAttack);
        GenerateSliderMoves<Slider::BISHOP>(bishops | queens, blockers, targets);
        GenerateSliderMoves<Slider::ROOK>(rooks | queens, blockers, targets);
    }

    void MoveGenerator::GenerateAntiSliderMoves(BitBoard targets, BitBoard pieces, const BitBoard *attackBoard)
    {
        auto attacks = EMPTY_BOARD;
        while (pieces)
        {
            unsigned int from = GetPosition(pieces);
            attacks |= attackBoard[from];
            attacks &= targets;
            while (attacks)
            {
                auto to = GetPosition(attacks);
                moveList.emplace_back(MakeMove(from, to));
            }
        }
    }

    template<Slider slider>
    void MoveGenerator::GenerateSliderMoves(BitBoard pieces, BitBoard blockers, BitBoard targets)
    {
        SliderAttacks sliders;
        auto attacks = EMPTY_BOARD;
        while (pieces)
        {
            unsigned int from = GetPosition(pieces);
            attacks |= slider == Slider::BISHOP ? sliders.GetBishopAttack(from, blockers) :
                       sliders.GetRookAttack(from, blockers);
            attacks &= targets;
            while (attacks)
            {
                auto to = GetPosition(attacks);
                moveList.emplace_back(MakeMove(from, to));
            }
        }
    }

    unsigned MoveGenerator::GetPosition(BitBoard &bitBoard) const
    {
        auto from = static_cast<unsigned>(TrailingZeroCount(bitBoard));
        bitBoard &= bitBoard - 1;
        return from;
    }

    std::string MoveGenerator::MakeMove(unsigned int from, unsigned to)
    {
        return SquareToNotation(from) + SquareToNotation(to);
    }
}
