#ifndef PUZZLE_STATE_HPP
#define PUZZLE_STATE_HPP

#include "State.hpp"
#include <vector>
#include <cstdint>
#include <string>
#include <utility>

class PuzzleState : public State {
    private:
        std::size_t num_tiles;    // N tiles (e.g. 8, 15, 24)
        std::size_t side_length;  // sqrt(N + 1)
        std::size_t size;         // N + 1 total cells

        // internal 1d vector representing the board
        std::vector<std::uint8_t> flat_matrix;

        // cached 2d coordinates (row, col) of the empty tile (0)
        std::pair<std::size_t, std::size_t> zero_pos;

        // helper mapping function: row * side + col
        [[nodiscard]] std::size_t to_1d(std::size_t row, std::size_t col) const {
            return row * side_length + col;
        };

        // writes a tile in (row, col)
        void set_tile(std::size_t row, std::size_t col, std::uint8_t value) {
            flat_matrix[to_1d(row, col)] = value;
        }

        // validates if N + 1 is a perfect square and returns side length
        [[nodiscard]] static std::size_t compute_side_length(std::size_t n);

    public:
        // default constructor: 8-puzzle goal state (N = 8)
        PuzzleState();

        // constructor for N-puzzle goal state with N tiles
        explicit PuzzleState(std::size_t n);

        // custom constructor from a flat vector layout with N tiles
        PuzzleState(std::size_t n, const std::vector<std::uint8_t>& initial_board);

        // getters for puzzle attributes
        [[nodiscard]] std::size_t get_num_tiles() const {
            return num_tiles;
        }
        [[nodiscard]] std::size_t get_side_length() const {
            return side_length;
        }
        [[nodiscard]] std::size_t get_size() const {
            return size;
        }

        // 2d matrix access operator for reading: state(row, col)
        [[nodiscard]] std::uint8_t operator()(std::size_t row, std::size_t col) const {
            return flat_matrix[to_1d(row, col)];
        }

        // getter for cached zero position in O(1)
        [[nodiscard]] std::pair<std::size_t, std::size_t> get_zero_pos() const {
            return zero_pos;
        }

        // swap tiles and maintain zero_pos consistency
        void swap_tiles(std::size_t r1, std::size_t c1, std::size_t r2, std::size_t c2);

        // virtual interface overrides from base class State
        [[nodiscard]] bool operator==(const State& other) const override;
        [[nodiscard]] std::size_t hash() const override;
        [[nodiscard]] std::string to_string() const override;

};

#endif