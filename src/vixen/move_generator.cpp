#include "move_generator.h"
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
                std::cout << ": " << olds << "\n";
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
        auto targets = board.GetBitBoard(' ');
        auto pawns = sideToMove == Colors::WHITE ? board.GetBitBoard('P') : board.GetBitBoard('p');
        auto kings = sideToMove == Colors::WHITE ? board.GetBitBoard('K') : board.GetBitBoard('k');
        auto knights = sideToMove == Colors::WHITE ? board.GetBitBoard('N') : board.GetBitBoard('n');
        auto bishops = sideToMove == Colors::WHITE ? board.GetBitBoard('B') : board.GetBitBoard('b');
        auto rooks = sideToMove == Colors::WHITE ? board.GetBitBoard('R') : board.GetBitBoard('r');
        auto queens = sideToMove == Colors::WHITE ? board.GetBitBoard('Q') : board.GetBitBoard('q');
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

        GenerateAntiSliderMoves<QUIET_MOVE>(targets, knights, AntSliderUtils::knightAttack);
        GenerateAntiSliderMoves<QUIET_MOVE>(targets, kings, AntSliderUtils::kingAttack);

        GenerateSliderMoves<Slider::BISHOP, QUIET_MOVE>(bishops | queens, blockers, targets);
        GenerateSliderMoves<Slider::ROOK, QUIET_MOVE>(rooks | queens, blockers, targets);

        GenerateCastlingMoves<sideToMove>(board);
    }

    template<Colors sideToMove>
    void MoveGenerator::GenerateCaptureMoves(const Board &board)
    {
        auto targets = sideToMove == Colors::WHITE ? board.GetBitBoard('S') : board.GetBitBoard('F');
        auto enPassant = board.GetEnPassant();
        auto pawns = sideToMove == Colors::WHITE ? board.GetBitBoard('P') : board.GetBitBoard('p');
        auto kings = sideToMove == Colors::WHITE ? board.GetBitBoard('K') : board.GetBitBoard('k');
        auto knights = sideToMove == Colors::WHITE ? board.GetBitBoard('N') : board.GetBitBoard('n');
        auto bishops = sideToMove == Colors::WHITE ? board.GetBitBoard('B') : board.GetBitBoard('b');
        auto rooks = sideToMove == Colors::WHITE ? board.GetBitBoard('R') : board.GetBitBoard('r');
        auto queens = sideToMove == Colors::WHITE ? board.GetBitBoard('Q') : board.GetBitBoard('q');
        auto blockers = ~board.GetBitBoard(' ');

        auto pawnLeftCapture = sideToMove == Colors::WHITE ? 9 : -9;
        auto pawnRightCapture = sideToMove == Colors::WHITE ? 7 : -7;
        auto promotionRanks = sideToMove == Colors::WHITE ? RANK8 : RANK1;
        auto pawnPromotionLeft = PawnCaptureLeft<sideToMove>(pawns) & blockers & promotionRanks;
        auto pawnPromotionRight = PawnCaptureRight<sideToMove>(pawns) & blockers & promotionRanks;
        enPassant &= sideToMove == Colors::WHITE ? RANK6 : RANK3;

        GeneratePawnPromotionCaptureMoves(pawnLeftCapture, pawnPromotionLeft);
        GeneratePawnPromotionCaptureMoves(pawnRightCapture, pawnPromotionRight);
        GenerateAntiSliderMoves<CAPTURE>(targets & ~promotionRanks, pawns,
                                         AntSliderUtils::pawnAttack[static_cast<int>(sideToMove)]);
        GenerateAntiSliderMoves<ENPASSANT>(enPassant, pawns, AntSliderUtils::pawnAttack[static_cast<int>(sideToMove)]);

        GenerateAntiSliderMoves<CAPTURE>(targets, knights, AntSliderUtils::knightAttack);
        GenerateAntiSliderMoves<CAPTURE>(targets, kings, AntSliderUtils::kingAttack);
        GenerateSliderMoves<Slider::BISHOP, CAPTURE>(bishops | queens, blockers, targets);
        GenerateSliderMoves<Slider::ROOK, CAPTURE>(rooks | queens, blockers, targets);
    }

    template<Slider slider, uint8_t moveType>
    void MoveGenerator::GenerateSliderMoves(BitBoard pieces, BitBoard blockers, BitBoard targets)
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

    template<uint8_t moveType>
    constexpr void
    MoveGenerator::GenerateAntiSliderMoves(BitBoard targets, BitBoard pieces, const BitBoard *attackBoard)
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
        int castlingRights = board.GetCastlingRights();
        if constexpr (sideToMove == Colors::WHITE)
        {
            if ((castlingRights & WKCA) &&
                IsBitSet(board.GetBitBoard(' '), F1) &&
                IsBitSet(board.GetBitBoard(' '), G1) &&
                !Check::IsInCheck<sideToMove>(board) &&
                !Check::IsSquareAttacked<sideToMove>(F1, board))
                moveList[size++] = CreateMove(E1, G1, KING_CASTLE);

            if ((castlingRights & WQCA) &&
                IsBitSet(board.GetBitBoard(' '), D1) &&
                IsBitSet(board.GetBitBoard(' '), C1) &&
                IsBitSet(board.GetBitBoard(' '), B1) &&
                !Check::IsInCheck<sideToMove>(board) &&
                !Check::IsSquareAttacked<sideToMove>(D1, board))
                moveList[size++] = CreateMove(E1, C1, QUEEN_CASTLE);
        }
        else
        {
            if ((castlingRights & BKCA) &&
                IsBitSet(board.GetBitBoard(' '), F8) &&
                IsBitSet(board.GetBitBoard(' '), G8) &&
                !Check::IsInCheck<sideToMove>(board) &&
                !Check::IsSquareAttacked<sideToMove>(F8, board))
                moveList[size++] = CreateMove(E8, G8, KING_CASTLE);

            if ((castlingRights & BQCA) &&
                IsBitSet(board.GetBitBoard(' '), D8) &&
                IsBitSet(board.GetBitBoard(' '), C8) &&
                IsBitSet(board.GetBitBoard(' '), B8) &&
                !Check::IsInCheck<sideToMove>(board) &&
                !Check::IsSquareAttacked<sideToMove>(D8, board))
                moveList[size++] = CreateMove(E8, C8, QUEEN_CASTLE);
        }
    }

    template<Colors sideToMove>
    void MoveGenerator::GenerateAllMoves(const Vixen::Board &board)
    {
        GenerateCaptureMoves<sideToMove>(board);
        GenerateQuietMoves<sideToMove>(board);
    }

    template void MoveGenerator::GenerateCaptureMoves<Colors::WHITE>(const Board &);

    template void MoveGenerator::GenerateCaptureMoves<Colors::BLACK>(const Board &);

    template void MoveGenerator::GenerateQuietMoves<Colors::WHITE>(const Board &);

    template void MoveGenerator::GenerateQuietMoves<Colors::BLACK>(const Board &);

    template void MoveGenerator::GenerateAllMoves<Colors::WHITE>(const Board &board);

    template void MoveGenerator::GenerateAllMoves<Colors::BLACK>(const Board &board);
}
