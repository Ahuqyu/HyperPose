#pragma once

/// \file human.hpp
/// \brief About human topology and visualization.

#include <opencv2/opencv.hpp>

namespace hyperpose {

constexpr int COCO_N_PARTS = 18;
constexpr int COCO_N_PAIRS = 19;

using idx_pair_t = std::pair<int, int>;
using coco_pair_list_t = std::vector<idx_pair_t>;

inline const coco_pair_list_t COCOPAIRS_NET = {
    { 12, 13 }, // 6
    { 20, 21 }, // 10
    { 14, 15 }, // 7
    { 16, 17 }, // 8
    { 22, 23 }, // 11
    { 24, 25 }, // 12
    { 0, 1 }, // 0
    { 2, 3 }, // 1
    { 4, 5 }, // 2
    { 6, 7 }, // 3
    { 8, 9 }, // 4
    { 10, 11 }, // 5
    { 28, 29 }, // 14
    { 30, 31 }, // 15
    { 34, 35 }, // 17
    { 32, 33 }, // 16
    { 36, 37 }, // 18
    { 18, 19 }, // 9
    { 26, 27 }, // 13
};

inline const coco_pair_list_t COCOPAIRS = {
    { 1, 2 }, // 6
    { 1, 5 }, // 10
    { 2, 3 }, // 7
    { 3, 4 }, // 8
    { 5, 6 }, // 11
    { 6, 7 }, // 12
    { 1, 8 }, // 0
    { 8, 9 }, // 1
    { 9, 10 }, // 2
    { 1, 11 }, // 3
    { 11, 12 }, // 4
    { 12, 13 }, // 5
    { 1, 0 }, // 14
    { 0, 14 }, // 15
    { 14, 16 }, // 17
    { 0, 15 }, // 16
    { 15, 17 }, // 18
    { 2, 16 }, // * 9
    { 5, 17 }, // * 13
}; // See https://www.cnblogs.com/caffeaoto/p/7793994.html.

inline bool is_virtual_pair(int pair_id) { return pair_id > 16; }

/// \brief Class to describe a key point.
struct body_part_t {
    bool has_value = false; ///< Whether this key point is valid.
    float x = 0; ///< X coordinate of the key point.
    float y = 0; ///< Y coordinate of the key point.
    float score = 0; ///< The inferred score(higher means more key-point-like) of the key point.
};

/// Template class to describe a human.
/// \tparam J The maximum key point of a human.
template <size_t J>
struct human_t_ {
    std::array<body_part_t, J> parts; ///< An array to tell all key point information. The index means the position.
    float score;

    inline void print() const
    {
        for (int i = 0; i < J; ++i) {
            const auto p = parts[i];
            if (p.has_value) {
                printf("BodyPart:%d-(%.2f, %.2f) score=%.2f ", i, p.x, p.y, p.score);
            }
        }
        printf("score=%.2f\n", score);
    }
};

/// \brief Class to describe a COCO human type(18 parts and 19 pairs).
/// \see　
using human_t = human_t_<COCO_N_PARTS>;

struct connection_candidate {
    int idx1;
    int idx2;
    float score;
    float etc;
};

inline bool operator>(const connection_candidate& a, const connection_candidate& b)
{
    return a.score > b.score;
}

struct connection {
    int cid1;
    int cid2;
    float score;
    int peak_id1;
    int peak_id2;
};

struct body_part_ret_t {
    int id = -1; ///< id of peak in the list of all peaks
};

template <int J>
struct human_ref_t_ {
    int id;
    body_part_ret_t parts[J];
    float score;
    int n_parts;

    human_ref_t_()
        : id(-1)
        , score(0)
        , n_parts(0)
    {
    }

    inline bool touches(const std::pair<int, int>& p, const connection& conn) const
    {
        return parts[p.first].id == conn.cid1 || parts[p.second].id == conn.cid2;
    }
};

using human_ref_t = human_ref_t_<COCO_N_PARTS>;

/// Function to visualize the human topology on an image.
/// \param img Image to be visualized.
/// \param human Human topology.
void draw_human(cv::Mat& img, const human_t& human);

} // namespace hyperpose