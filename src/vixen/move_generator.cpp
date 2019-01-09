#include "move_generator.h"
#include "board.h"

namespace Vixen
{
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

    BitBoard leafs = 0;

    BitBoard MoveGenerator::PerftTest(int depth, Board &board)
    {
        BitBoard nodes = 0;
        MoveGenerator generator;

        if (depth == 0)
            return 1;

        board.IsWhiteToMove() ? generator.GenerateAllMoves<Colors::WHITE>(board)
                              : generator.GenerateAllMoves<Colors::BLACK>(board);
        for (const auto &move: generator.GetMoveList())
        {
            if (!board.MakeMove(move))
                continue;
            BitBoard cumNodes = leafs;
            nodes += Perft(depth - 1, board);
            board.TakeBack();
            BitBoard olds = leafs - cumNodes;
            std::cout << move << ", ";
            std::cout << SquareToNotation(static_cast<unsigned>(move & 0x3F))
                      << SquareToNotation(static_cast<unsigned>(move & 0xFC0) >> 6);
            std::cout << ": " << olds << std::endl;
        }
        return nodes;
    }

    BitBoard MoveGenerator::Perft(int depth, Board &board)
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
        for (const auto &move: generator.GetMoveList())
        {
            if (!board.MakeMove(move))
                continue;
            nodes += Perft(depth - 1, board);
            board.TakeBack();
        }
        return nodes;
    }

    template<Colors sideToMove>
    void MoveGenerator::GenerateAllMoves(const Vixen::Board &board)
    {
        //Timer t("gene");
        auto boards = board.GetBitBoards();
        auto enemies = sideToMove == Colors::WHITE ? boards.at('S') : boards.at('F');
        auto empties = boards.at(' ');
        GenerateCaptureMoves<sideToMove>(board, enemies);
        GenerateQuietMoves<sideToMove>(board, empties);
    }

    template<Colors sideToMove>
    void MoveGenerator::GenerateQuietMoves(const Board &board, BitBoard targets)
    {
        auto castlingRights = board.GetCastlingRights();
        auto boards = board.GetBitBoards();
        auto sliders = board.GetSlider();
        auto pawns = sideToMove == Colors::WHITE ? boards.at('P') : boards.at('p');
        auto kings = sideToMove == Colors::WHITE ? boards.at('K') : boards.at('k');
        auto knights = sideToMove == Colors::WHITE ? boards.at('N') : boards.at('n');
        auto bishops = sideToMove == Colors::WHITE ? boards.at('B') : boards.at('b');
        auto rooks = sideToMove == Colors::WHITE ? boards.at('R') : boards.at('r');
        auto queens = sideToMove == Colors::WHITE ? boards.at('Q') : boards.at('q');
        auto blockers = ~boards.at(' ');

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

        GenerateSliderMoves<Slider::BISHOP>(bishops | queens, blockers, targets, QUIET_MOVE, sliders);
        GenerateSliderMoves<Slider::ROOK>(rooks | queens, blockers, targets, QUIET_MOVE, sliders);

        GenerateCastlingMoves<sideToMove>(boards, castlingRights, sliders);

    }

    template<Colors sideToMove>
    void MoveGenerator::GenerateCaptureMoves(const Board &board, BitBoard targets)
    {
        auto boards = board.GetBitBoards();
        auto sliders = board.GetSlider();
        auto enPassant = board.GetEnPassant();
        auto pawns = sideToMove == Colors::WHITE ? boards.at('P') : boards.at('p');
        auto kings = sideToMove == Colors::WHITE ? boards.at('K') : boards.at('k');
        auto knights = sideToMove == Colors::WHITE ? boards.at('N') : boards.at('n');
        auto bishops = sideToMove == Colors::WHITE ? boards.at('B') : boards.at('b');
        auto rooks = sideToMove == Colors::WHITE ? boards.at('R') : boards.at('r');
        auto queens = sideToMove == Colors::WHITE ? boards.at('Q') : boards.at('q');
        auto blockers = ~boards.at(' ');

        auto pawnLeftCapture = sideToMove == Colors::WHITE ? 9 : -9;
        auto pawnRightCapture = sideToMove == Colors::WHITE ? 7 : -7;
        auto promotionRanks = sideToMove == Colors::WHITE ? RANK8 : RANK1;
        auto pawnPromotionLeft = PawnCaptureLeft<sideToMove>(pawns) & blockers & promotionRanks;
        auto pawnPromotionRight = PawnCaptureRight<sideToMove>(pawns) & blockers & promotionRanks;
        enPassant &= sideToMove == Colors::WHITE ? RANK6 : RANK3;

        GeneratePawnPromotionCaptureMoves(pawnLeftCapture, pawnPromotionLeft);
        GeneratePawnPromotionCaptureMoves(pawnRightCapture, pawnPromotionRight);
        GenerateAntiSliderMoves(targets & ~promotionRanks, pawns, pawnAttack[static_cast<int>(sideToMove)], CAPTURE);
        GenerateAntiSliderMoves(enPassant, pawns, pawnAttack[static_cast<int>(sideToMove)], ENPASSANT);

        GenerateAntiSliderMoves(targets, knights, knightAttack, CAPTURE);
        GenerateAntiSliderMoves(targets, kings, kingAttack, CAPTURE);
        GenerateSliderMoves<Slider::BISHOP>(bishops | queens, blockers, targets, CAPTURE, sliders);
        GenerateSliderMoves<Slider::ROOK>(rooks | queens, blockers, targets, CAPTURE, sliders);
    }

    template<Slider slider>
    void MoveGenerator::GenerateSliderMoves(BitBoard pieces, BitBoard blockers, BitBoard targets, uint8_t moveType,
                                            SliderAttacks sliders)
    {
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
    void MoveGenerator::GenerateCastlingMoves(const BitBoards &bitBoards, int castlingRights,
                                              const SliderAttacks &sliders)
    {
        if (sideToMove == Colors::WHITE)
        {
            if (castlingRights & WKCA)
                if (IsBitSet(bitBoards.at(' '), F1) &&
                    IsBitSet(bitBoards.at(' '), G1))
                    if (!IsInCheck<sideToMove>(bitBoards, sliders) && !IsSquareAttacked<sideToMove>(F1, bitBoards,
                                                                                                    sliders))
                        moveList.emplace_back(CreateMove(E1, G1, KING_CASTLE));

            if (castlingRights & WQCA)
                if (IsBitSet(bitBoards.at(' '), D1) &&
                    IsBitSet(bitBoards.at(' '), C1) &&
                    IsBitSet(bitBoards.at(' '), B1))
                    if (!IsInCheck<sideToMove>(bitBoards, sliders) && !IsSquareAttacked<sideToMove>(D1, bitBoards,
                                                                                                    sliders))
                        moveList.emplace_back(CreateMove(E1, C1, QUEEN_CASTLE));
        }
        else
        {
            if (castlingRights & BKCA)
                if (IsBitSet(bitBoards.at(' '), F8) &&
                    IsBitSet(bitBoards.at(' '), G8))
                    if (!IsInCheck<sideToMove>(bitBoards, sliders) && !IsSquareAttacked<sideToMove>(F8, bitBoards,
                                                                                                    sliders))
                        moveList.emplace_back(CreateMove(E8, G8, KING_CASTLE));

            if (castlingRights & BQCA)
                if (IsBitSet(bitBoards.at(' '), D8) &&
                    IsBitSet(bitBoards.at(' '), C8) &&
                    IsBitSet(bitBoards.at(' '), B8))
                    if (!IsInCheck<sideToMove>(bitBoards, sliders) && !IsSquareAttacked<sideToMove>(D8, bitBoards,
                                                                                                    sliders))
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
    bool MoveGenerator::IsSquareAttacked(int square, const BitBoards &bitBoards, SliderAttacks sliders)
    {
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
    bool MoveGenerator::IsInCheck(const BitBoards &bitBoards, const SliderAttacks &sliders)
    {
        auto kingBoard = sideToMove == Colors::WHITE ? bitBoards.at('K') : bitBoards.at('k');
        int kingSquare = TrailingZeroCount(kingBoard);
        return IsSquareAttacked<sideToMove>(kingSquare, bitBoards, sliders);
    }

    std::vector<Move> MoveGenerator::GetAllMoves(const Board &board)
    {
        MoveGenerator generator;
        board.IsWhiteToMove() ? generator.GenerateAllMoves<Colors::WHITE>(board)
                              : generator.GenerateAllMoves<Colors::BLACK>(board);
        return generator.GetMoveList();
    }
}
