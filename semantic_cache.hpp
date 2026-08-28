#pragma once

#include <algorithm>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

struct CacheEntry {
    std::vector<int> token_signature;
    std::string model_response;
};

class SemanticCacheEngine {
private:
    std::vector<CacheEntry> cache_storage;
    static constexpr float similarity_threshold = 0.65f;

    static float jaccard_similarity(const std::vector<int>& left, const std::vector<int>& right) {
        std::set<int> left_set(left.begin(), left.end());
        std::set<int> right_set(right.begin(), right.end());
        if (left_set.empty() && right_set.empty()) {
            return 1.0f;
        }

        std::vector<int> intersection;
        std::set_intersection(
            left_set.begin(), left_set.end(), right_set.begin(), right_set.end(),
            std::back_inserter(intersection));

        std::vector<int> union_set;
        std::set_union(
            left_set.begin(), left_set.end(), right_set.begin(), right_set.end(),
            std::back_inserter(union_set));

        return union_set.empty()
            ? 0.0f
            : static_cast<float>(intersection.size()) / static_cast<float>(union_set.size());
    }

public:
    void store_response(const std::vector<int>& tokens, const std::string& response) {
        cache_storage.push_back({tokens, response});
    }

    std::string evaluate_query(const std::vector<int>& query_tokens, bool& cache_hit) const {
        cache_hit = false;
        for (const CacheEntry& entry : cache_storage) {
            if (jaccard_similarity(query_tokens, entry.token_signature) >= similarity_threshold) {
                cache_hit = true;
                return entry.model_response;
            }
        }
        return {};
    }
};
