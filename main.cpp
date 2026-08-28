#include "semantic_cache.hpp"
#include "tokenizer.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

int main() {
    std::cout << "========================================================\n"
              << " DeepCache Tokenizer & Inference Routing Server Online\n"
              << "========================================================\n\n";

    BPETokenizer tokenizer;
    SemanticCacheEngine cache;

    const std::string primary_response =
        "AI code refers to executable instructions generated or optimized by artificial intelligence models.";
    cache.store_response(tokenizer.encode("What is AI code"), primary_response);

    const std::vector<std::string> user_queries = {
        "What is AI code",
        "What is AI code exactly",
        "Hello universe tomorrow"
    };

    for (const std::string& query : user_queries) {
        std::cout << "User Query: \"" << query << "\"\n";
        const auto start = std::chrono::high_resolution_clock::now();
        const std::vector<int> current_tokens = tokenizer.encode(query);

        bool cache_hit = false;
        std::string final_reply = cache.evaluate_query(current_tokens, cache_hit);
        if (!cache_hit) {
            std::this_thread::sleep_for(std::chrono::milliseconds(800));
            final_reply = "[LLM Live Engine Output Generation Completed]";
            cache.store_response(current_tokens, final_reply);
        }

        const auto end = std::chrono::high_resolution_clock::now();
        const std::chrono::duration<double, std::milli> duration = end - start;
        std::cout << "Routing State: " << (cache_hit ? "SEMANTIC CACHE HIT" : "API MODEL CLOUD INFERENCE") << "\n"
                  << "Response: " << final_reply << "\n"
                  << "Processing Cost: " << std::fixed << std::setprecision(2) << duration.count() << " ms\n\n";
    }
    return 0;
}
