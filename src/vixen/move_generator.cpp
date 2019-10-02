#include "move_generator.h"
#include "board.h"
#include "timer.h"
#include <iostream>

namespace Vixen
{
    namespace Test
    {
        BitBoard leafs = 0;

        /**
         * Helper function to validate move generation:
         *  - Perft method: https://www.chessprogramming.org/Perft
         *  - Perft results: https://www.chessprogramming.org/Perft_Results
         * @param depth
         * @param board
         * @return visited node number
         */
        BitBoard PerftTest(int depth, Board &board)
        {
            BitBoard nodes = 0;
            MoveGenerator generator;

            if (depth == 0)
                return 1;

            board.IsWhiteToMove() ? generator.GenerateAllMoves<Colors::WHITE>(board)
                                  : generator.GenerateAllMoves<Colors::BLACK>(board);
            for (int i = 0; i < generator.GetListSize(); ++i)
            {
                Move move = generator.GetMoveList()[i];
                if (!board.MakeMove(move))
                    continue;

                BitBoard cumNodes = leafs;
                nodes += Perft(depth - 1, board);
                board.TakeBack();
                BitBoard olds = leafs - cumNodes;
                std::cout << move << ", ";
                std::cout << SquareToNotation(move & 0x3F)
                          << SquareToNotation((move & 0xFC0) >> 6);
                std::cout << ": " << olds << std::endl;
            }
            return nodes;
        }

        BitBoard Perft(int depth, Board &board)
        {
            BitBoard nodes = 0;
            MoveGenerator generator;

            if (depth == 0)
            {
                ++leafs;
                return 1;
            }

            board.IsWhiteToMove() ? generator.GenerateAllMoves<Colors::WHITE>(board)
                                  : generator.GenerateAllMoves<Colors::BLACK>(board);
            for (int i = 0; i < generator.GetListSize(); ++i)
            {
                if (!board.MakeMove(generator.GetMoveList()[i]))
                    continue;
                nodes += Perft(depth - 1, board);
                board.TakeBack();
            }
            return nodes;
        }
    }

    template<Colors sideToMove>
    void MoveGenerator::GenerateQuietMoves(const Board &board)
    {
        auto targets = board.bitBoards.at(' ');
        auto pawns = sideToMove == Colors::WHITE ? board.bitBoards.at('P') : board.bitBoards.at('p');
        auto kings = sideToMove == Colors::WHITE ? board.bitBoards.at('K') : board.bitBoards.at('k');
        auto knights = sideToMove == Colors::WHITE ? board.bitBoards.at('N') : board.bitBoards.at('n');
        auto bishops = sideToMove == Colors::WHITE ? board.bitBoards.at('B') : board.bitBoards.at('b');
        auto rooks = sideToMove == Colors::WHITE ? board.bitBoards.at('R') : board.bitBoards.at('r');
        auto queens = sideToMove == Colors::WHITE ? board.bitBoards.at('Q') : board.bitBoards.at('q');
        auto blockers = ~targets;

        auto pawnOffset = sideToMove == Colors::WHITE ? 8 : -8;
        auto promotionRanks = sideToMove == Colors::WHITE ? RANK8 : RANK1;
        auto doublePushStart = sideToMove == Colors::WHITE ? RANK3 : RANK6;

        auto onePawnPush = PushPawns<sideToMove>(pawns) & ~blockers & ~promotionRanks;
        auto doublePawnPush = PushPawns<sideToMove>((onePawnPush & doublePushStart)) & ~blockers;
        auto pawnPromotion = PushPawns<sideToMove>(pawns) & ~blockers & promotionRanks;

        GeneratePawnMoves(pawnOffset, onePawnPush, QUIET_MOVE);
        GeneratePawnMoves(2 * pawnOffset, doublePawnPush, DOUBLE_PAWN_PUSH);
        GeneratePawnPromotionMoves(pawnOffset, pawnPromotion);

        GenerateAntiSliderMoves(targets, knights, AntSliderUtils::knightAttack, QUIET_MOVE);
        GenerateAntiSliderMoves(targets, kings, AntSliderUtils::kingAttack, QUIET_MOVE);

        GenerateSliderMoves<Slider::BISHOP>(bishops | queens, blockers, targets, QUIET_MOVE);
        GenerateSliderMoves<Slider::ROOK>(rooks | queens, blockers, targets, QUIET_MOVE);

        GenerateCastlingMoves<sideToMove>(board);
    }

    template<Colors sideToMove>
    void MoveGenerator::GenerateCaptureMoves(const Board &board)
    {
        auto targets = sideToMove == Colors::WHITE ? board.bitBoards.at('S') : board.bitBoards.at('F');
        auto enPassant = board.GetEnPassant();
        auto pawns = sideToMove == Colors::WHITE ? board.bitBoards.at('P') : board.bitBoards.at('p');
        auto kings = sideToMove == Colors::WHITE ? board.bitBoards.at('K') : board.bitBoards.at('k');
        auto knights = sideToMove == Colors::WHITE ? board.bitBoards.at('N') : board.bitBoards.at('n');
        auto bishops = sideToMove == Colors::WHITE ? board.bitBoards.at('B') : board.bitBoards.at('b');
        auto rooks = sideToMove == Colors::WHITE ? board.bitBoards.at('R') : board.bitBoards.at('r');
        auto queens = sideToMove == Colors::WHITE ? board.bitBoards.at('Q') : board.bitBoards.at('q');
        auto blockers = ~board.bitBoards.at(' ');

        auto pawnLeftCapture = sideToMove == Colors::WHITE ? 9 : -9;
        auto pawnRightCapture = sideToMove == Colors::WHITE ? 7 : -7;
        auto promotionRanks = sideToMove == Colors::WHITE ? RANK8 : RANK1;
        auto pawnPromotionLeft = PawnCaptureLeft<sideToMove>(pawns) & blockers & promotionRanks;
        auto pawnPromotionRight = PawnCaptureRight<sideToMove>(pawns) & blockers & promotionRanks;
        enPassant &= sideToMove == Colors::WHITE ? RANK6 : RANK3;

        GeneratePawnPromotionCaptureMoves(pawnLeftCapture, pawnPromotionLeft);
        GeneratePawnPromotionCaptureMoves(pawnRightCapture, pawnPromotionRight);
        GenerateAntiSliderMoves(targets & ~promotionRanks, pawns,
                                AntSliderUtils::pawnAttack[static_cast<int>(sideToMove)], CAPTURE);
        GenerateAntiSliderMoves(enPassant, pawns, AntSliderUtils::pawnAttack[static_cast<int>(sideToMove)], ENPASSANT);

        GenerateAntiSliderMoves(targets, knights, AntSliderUtils::knightAttack, CAPTURE);
        GenerateAntiSliderMoves(targets, kings, AntSliderUtils::kingAttack, CAPTURE);
        GenerateSliderMoves<Slider::BISHOP>(bishops | queens, blockers, targets, CAPTURE);
        GenerateSliderMoves<Slider::ROOK>(rooks | queens, blockers, targets, CAPTURE);
    }

    template<Slider slider>
    void MoveGenerator::GenerateSliderMoves(BitBoard pieces, BitBoard blockers, BitBoard targets, uint8_t moveType)
    {
        auto attacks = EMPTY_BOARD;
        while (pieces)
        {
            int from = GetPosition(pieces);
            attacks |= slider == Slider::BISHOP ? SliderUtils::GetBishopAttack(from, blockers)
                                                : SliderUtils::GetRookAttack(from, blockers);
            attacks &= targets;
            while (attacks)
            {
                int to = GetPosition(attacks);
                moveList[size++] = CreateMove(from, to, moveType);
            }
        }
    }

    constexpr void
    MoveGenerator::GenerateAntiSliderMoves(BitBoard targets, BitBoard pieces, const BitBoard *attackBoard,
                                           uint8_t moveType)
    {
        auto attacks = EMPTY_BOARD;
        while (pieces)
        {
            int from = GetPosition(pieces);
            attacks |= attackBoard[from];
            attacks &= targets;
            while (attacks)
            {
                int to = GetPosition(attacks);
                moveList[size++] = CreateMove(from, to, moveType);
            }
        }
    }

    constexpr void MoveGenerator::GeneratePawnMoves(int pawnOffset, BitBoard pawnPushed, uint8_t moveType)
    {
        while (pawnPushed)
        {
            int to = GetPosition(pawnPushed);
            moveList[size++] = CreateMove(to - pawnOffset, to, moveType);
        }
    }

    constexpr void MoveGenerator::GeneratePawnPromotionMoves(int offset, BitBoard promotion)
    {
        while (promotion)
        {
            int to = GetPosition(promotion);
            moveList[size++] = CreateMove(to - offset, to, QUEEN_PROMOTION);
            moveList[size++] = CreateMove(to - offset, to, ROOK_PROMOTION);
            moveList[size++] = CreateMove(to - offset, to, BISHOP_PROMOTION);
            moveList[size++] = CreateMove(to - offset, to, KNIGHT_PROMOTION);
        }
    }

    constexpr void MoveGenerator::GeneratePawnPromotionCaptureMoves(int offset, BitBoard promotion)
    {
        while (promotion)
        {
            int to = GetPosition(promotion);
            moveList[size++] = CreateMove(to - offset, to, QUEEN_PROMO_CAPTURE);
            moveList[size++] = CreateMove(to - offset, to, ROOK_PROMO_CAPTURE);
            moveList[size++] = CreateMove(to - offset, to, BISHOP_PROMO_CAPTURE);
            moveList[size++] = CreateMove(to - offset, to, KNIGHT_PROMO_CAPTURE);
        }
    }

    template<Colors sideToMove>
    constexpr void MoveGenerator::GenerateCastlingMoves(const Board &board)
    {
        int castlingRights = board.castlingRights;
        if constexpr (sideToMove == Colors::WHITE)
        {
            if ((castlingRights & WKCA) &&
                IsBitSet(board.bitBoards.at(' '), F1) &&
                IsBitSet(board.bitBoards.at(' '), G1) &&
                !Check::IsInCheck<sideToMove>(board.bitBoards) &&
                !Check::IsSquareAttacked<sideToMove>(F1, board.bitBoards))
                moveList[size++] = CreateMove(E1, G1, KING_CASTLE);

            if ((castlingRights & WQCA) &&
                IsBitSet(board.bitBoards.at(' '), D1) &&
                IsBitSet(board.bitBoards.at(' '), C1) &&
                IsBitSet(board.bitBoards.at(' '), B1) &&
                !Check::IsInCheck<sideToMove>(board.bitBoards) &&
                !Check::IsSquareAttacked<sideToMove>(D1, board.bitBoards))
                moveList[size++] = CreateMove(E1, C1, QUEEN_CASTLE);
        }
        else
        {
            if ((castlingRights & BKCA) &&
                IsBitSet(board.bitBoards.at(' '), F8) &&
                IsBitSet(board.bitBoards.at(' '), G8) &&
                !Check::IsInCheck<sideToMove>(board.bitBoards) &&
                !Check::IsSquareAttacked<sideToMove>(F8, board.bitBoards))
                moveList[size++] = CreateMove(E8, G8, KING_CASTLE);

            if ((castlingRights & BQCA) &&
                IsBitSet(board.bitBoards.at(' '), D8) &&
                IsBitSet(board.bitBoards.at(' '), C8) &&
                IsBitSet(board.bitBoards.at(' '), B8) &&
                !Check::IsInCheck<sideToMove>(board.bitBoards) &&
                !Check::IsSquareAttacked<sideToMove>(D8, board.bitBoards))
                moveList[size++] = CreateMove(E8, C8, QUEEN_CASTLE);
        }
    }
}
