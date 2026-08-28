#pragma once

#include <algorithm>
#include <cctype>
#include <string>
#include <unordered_map>
#include <vector>

class BPETokenizer {
private:
    std::unordered_map<std::string, int> vocab;
    std::unordered_map<int, std::string> inv_vocab;
    int next_token_id = 100;

    static std::string normalize_word(std::string word) {
        word.erase(
            std::remove_if(word.begin(), word.end(), [](unsigned char character) {
                return std::ispunct(character) != 0;
            }),
            word.end());

        std::transform(word.begin(), word.end(), word.begin(), [](unsigned char character) {
            return static_cast<char>(std::tolower(character));
        });
        return word;
    }

public:
    BPETokenizer() {
        build_vocab_node("hello", 1);
        build_vocab_node("what", 2);
        build_vocab_node("is", 3);
        build_vocab_node("ai", 4);
        build_vocab_node("code", 5);
    }

    void build_vocab_node(const std::string& word, int id) {
        vocab[word] = id;
        inv_vocab[id] = word;
    }

    std::vector<int> encode(const std::string& text) {
        std::vector<int> tokens;
        std::string current_word;

        auto flush_word = [&]() {
            if (current_word.empty()) {
                return;
            }

            std::string normalized_word = normalize_word(current_word);
            current_word.clear();
            if (normalized_word.empty()) {
                return;
            }

            auto existing_token = vocab.find(normalized_word);
            if (existing_token != vocab.end()) {
                tokens.push_back(existing_token->second);
                return;
            }

            int token_id = next_token_id++;
            vocab.emplace(normalized_word, token_id);
            inv_vocab.emplace(token_id, normalized_word);
            tokens.push_back(token_id);
        };

        for (unsigned char character : text) {
            if (std::isspace(character) != 0) {
                flush_word();
            } else {
                current_word.push_back(static_cast<char>(character));
            }
        }
        flush_word();
        return tokens;
    }
};
