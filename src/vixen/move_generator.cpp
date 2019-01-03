#include "move_generator.h"
#include "board.h"

#include <iostream>

namespace Vixen
{
    MoveGenerator::MoveGenerator(const Board &board)
    {
        auto bitBoards = board.GetBitBoards();
        auto enPassant = board.GetEnPassant();
        auto castlingRights = board.GetCastlingRights();
        InitKnightKingAttack();
        InitPawnAttack();
        board.IsWhiteToMove() ? GenerateAllMoves<Colors::WHITE>(bitBoards, enPassant, castlingRights)
                              : GenerateAllMoves<Colors::BLACK>(bitBoards, enPassant, castlingRights);
#ifdef DEBUG
        PrintMoveList();
#endif // DEBUG
    }

    void MoveGenerator::PrintMoveList()
    {
        for (const auto &move : moveList)
        {
            std::cout << SquareToNotation(static_cast<unsigned>(move & 0x3F))
                      << SquareToNotation(static_cast<unsigned>(move & 0xFC0) >> 6);
            if ((move >> 12) & 8)
            {
                std::string promotions = "nbrq";
                std::cout << promotions.at(static_cast<uint8_t>((move >> 12) & 3));
            }
            std::cout << std::endl;
        }
    }

    template<Colors sideToMove>
    void MoveGenerator::GenerateAllMoves(const BitBoards &bitBoards, BitBoard enPassant, int castlingRights)
    {
        auto enemies = sideToMove == Colors::WHITE ? bitBoards.at('S') : bitBoards.at('F');
        auto empties = bitBoards.at(' ');
        GenerateCaptureMoves<sideToMove>(bitBoards, enemies, enPassant);
        GenerateQuietMoves<sideToMove>(bitBoards, empties, castlingRights);
    }

    template<Colors sideToMove>
    void MoveGenerator::GenerateQuietMoves(const BitBoards &bitBoards, BitBoard targets, int castlingRights)
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
        auto doublePawnPush = PushPawns<sideToMove>((onePawnPush & doublePushStart)) & ~blockers;
        auto pawnPromotion = PushPawns<sideToMove>(pawns) & ~blockers & promotionRanks;

        GeneratePawnMoves(pawnOffset, onePawnPush, QUIET_MOVE);
        GeneratePawnMoves(2 * pawnOffset, doublePawnPush, DOUBLE_PAWN_PUSH);
        GeneratePawnPromotionMoves(pawnOffset, pawnPromotion);

        GenerateAntiSliderMoves(targets, knights, knightAttack, QUIET_MOVE);
        GenerateAntiSliderMoves(targets, kings, kingAttack, QUIET_MOVE);

        GenerateSliderMoves<Slider::BISHOP>(bishops | queens, blockers, targets, QUIET_MOVE);
        GenerateSliderMoves<Slider::ROOK>(rooks | queens, blockers, targets, QUIET_MOVE);

        GenerateCastlingMoves<sideToMove>(bitBoards, castlingRights);

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

        auto pawnLeftCapture = sideToMove == Colors::WHITE ? 9 : -9;
        auto pawnRightCapture = sideToMove == Colors::WHITE ? 7 : -7;
        auto promotionRanks = sideToMove == Colors::WHITE ? RANK8 : RANK1;
        auto pawnPromotionLeft = PawnCaptureLeft<sideToMove>(pawns) & blockers & promotionRanks;
        auto pawnPromotionRight = PawnCaptureRight<sideToMove>(pawns) & blockers & promotionRanks;

        GeneratePawnPromotionCaptureMoves(pawnLeftCapture, pawnPromotionLeft);
        GeneratePawnPromotionCaptureMoves(pawnRightCapture, pawnPromotionRight);
        GenerateAntiSliderMoves(targets & ~promotionRanks, pawns, pawnAttack[static_cast<int>(sideToMove)], CAPTURE);
        GenerateAntiSliderMoves(enPassant, pawns, pawnAttack[static_cast<int>(sideToMove)], ENPASSANT_CAPTURE);

        GenerateAntiSliderMoves(targets, knights, knightAttack, CAPTURE);
        GenerateAntiSliderMoves(targets, kings, kingAttack, CAPTURE);
        GenerateSliderMoves<Slider::BISHOP>(bishops | queens, blockers, targets, CAPTURE);
        GenerateSliderMoves<Slider::ROOK>(rooks | queens, blockers, targets, CAPTURE);
    }

    template<Slider slider>
    void MoveGenerator::GenerateSliderMoves(BitBoard pieces, BitBoard blockers, BitBoard targets, uint8_t moveType)
    {
        SliderAttacks sliders;
        auto attacks = EMPTY_BOARD;
        while (pieces)
        {
            auto from = GetPosition(pieces);
            attacks |= slider == Slider::BISHOP ? sliders.GetBishopAttack(from, blockers) :
                       sliders.GetRookAttack(from, blockers);
            attacks &= targets;
            while (attacks)
            {
                auto to = GetPosition(attacks);
                moveList.emplace_back(CreateMove(from, to, moveType));
            }
        }
    }

    void MoveGenerator::GenerateAntiSliderMoves(BitBoard targets, BitBoard pieces, const BitBoard *attackBoard,
                                                uint8_t moveType)
    {
        auto attacks = EMPTY_BOARD;
        while (pieces)
        {
            auto from = GetPosition(pieces);
            attacks |= attackBoard[from];
            attacks &= targets;
            while (attacks)
            {
                auto to = GetPosition(attacks);
                moveList.emplace_back(CreateMove(from, to, moveType));
            }
        }
    }

    void MoveGenerator::GeneratePawnMoves(int pawnOffset, BitBoard pawnPushed, uint8_t moveType)
    {
        while (pawnPushed)
        {
            auto to = GetPosition(pawnPushed);
            moveList.emplace_back(CreateMove(static_cast<uint8_t>(to - pawnOffset), to, moveType));
        }
    }

    void MoveGenerator::GeneratePawnPromotionMoves(int offset, BitBoard promotion)
    {
        while (promotion)
        {
            auto to = GetPosition(promotion);
            moveList.emplace_back(CreateMove(static_cast<uint8_t>(to - offset), to, QUEEN_PROMOTION));
            moveList.emplace_back(CreateMove(static_cast<uint8_t>(to - offset), to, ROOK_PROMOTION));
            moveList.emplace_back(CreateMove(static_cast<uint8_t>(to - offset), to, BISHOP_PROMOTION));
            moveList.emplace_back(CreateMove(static_cast<uint8_t>(to - offset), to, KNIGHT_PROMOTION));
        }
    }

    void MoveGenerator::GeneratePawnPromotionCaptureMoves(int offset, BitBoard promotion)
    {
        while (promotion)
        {
            auto to = GetPosition(promotion);
            moveList.emplace_back(CreateMove(static_cast<uint8_t>(to - offset), to, QUEEN_PROMO_CAPTURE));
            moveList.emplace_back(CreateMove(static_cast<uint8_t>(to - offset), to, ROOK_PROMO_CAPTURE));
            moveList.emplace_back(CreateMove(static_cast<uint8_t>(to - offset), to, BISHOP_PROMO_CAPTURE));
            moveList.emplace_back(CreateMove(static_cast<uint8_t>(to - offset), to, KNIGHT_PROMO_CAPTURE));
        }
    }

    template<Colors sideToMove>
    void MoveGenerator::GenerateCastlingMoves(const BitBoards &bitBoards, int castlingRights)
    {
        if (sideToMove == Colors::WHITE)
        {
            if (castlingRights & WKCA)
                if (IsBitSet(bitBoards.at(' '), F1) &&
                    IsBitSet(bitBoards.at(' '), G1))
                    if (!IsInCheck<sideToMove>(bitBoards) && !IsSquareAttacked<sideToMove>(F1, bitBoards))
                        moveList.emplace_back(CreateMove(E1, G1, KING_CASTLE));

            if (castlingRights & WQCA)
                if (IsBitSet(bitBoards.at(' '), D1) &&
                    IsBitSet(bitBoards.at(' '), C1) &&
                    IsBitSet(bitBoards.at(' '), B1))
                    if (!IsInCheck<sideToMove>(bitBoards) && !IsSquareAttacked<sideToMove>(D1, bitBoards))
                        moveList.emplace_back(CreateMove(E1, C1, QUEEN_CASTLE));
        } else
        {
            if (castlingRights & BKCA)
                if (IsBitSet(bitBoards.at(' '), F8) &&
                    IsBitSet(bitBoards.at(' '), G8))
                    if (!IsInCheck<sideToMove>(bitBoards) && !IsSquareAttacked<sideToMove>(F8, bitBoards))
                        moveList.emplace_back(CreateMove(E8, G8, KING_CASTLE));

            if (castlingRights & BQCA)
                if (IsBitSet(bitBoards.at(' '), D8) &&
                    IsBitSet(bitBoards.at(' '), C8) &&
                    IsBitSet(bitBoards.at(' '), B8))
                    if (!IsInCheck<sideToMove>(bitBoards) && !IsSquareAttacked<sideToMove>(D8, bitBoards))
                        moveList.emplace_back(CreateMove(E8, C8, QUEEN_CASTLE));
        }
    }

    Move MoveGenerator::CreateMove(uint8_t from, uint8_t to, uint8_t moveType)
    {
        return moveType << 12 | to << 6 | from;
    }

    uint8_t MoveGenerator::GetPosition(BitBoard &bitBoard) const
    {
        auto from = static_cast<uint8_t>(TrailingZeroCount(bitBoard));
        bitBoard &= bitBoard - 1;
        return from;
    }

    template<Colors sideToMove>
    bool MoveGenerator::IsSquareAttacked(int square, const BitBoards &bitBoards)
    {
        SliderAttacks sliders;
        auto blockers = ~bitBoards.at(' ');
        auto pawns = sideToMove == Colors::WHITE ? bitBoards.at('p') : bitBoards.at('P');
        auto knights = sideToMove == Colors::WHITE ? bitBoards.at('n') : bitBoards.at('N');
        auto bishops = sideToMove == Colors::WHITE ? bitBoards.at('b') : bitBoards.at('B');
        auto rooks = sideToMove == Colors::WHITE ? bitBoards.at('r') : bitBoards.at('R');
        auto queens = sideToMove == Colors::WHITE ? bitBoards.at('q') : bitBoards.at('Q');
        auto kings = sideToMove == Colors::WHITE ? bitBoards.at('k') : bitBoards.at('K');

        return pawnAttack[static_cast<int>(sideToMove)][square] & pawns ||
               knightAttack[square] & knights ||
               sliders.GetBishopAttack(square, blockers) & (bishops | queens) ||
               sliders.GetRookAttack(square, blockers) & (rooks | queens) ||
               kingAttack[square] & kings;
    }

    template<Colors sideToMove>
    bool MoveGenerator::IsInCheck(const BitBoards &bitBoards)
    {
        auto kingBoard = sideToMove == Colors::WHITE ? bitBoards.at('K') : bitBoards.at('k');
        int kingSquare = TrailingZeroCount(kingBoard);
        return IsSquareAttacked<sideToMove>(kingSquare, bitBoards);
    }
}
