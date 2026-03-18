#include "agent.h"
#include "provided.h"
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>

Agent::Agent(const IndexBase& index)
    : m_index(index), m_terms_prompt(""), m_summarize_prompt("")
{
    // TODO: implement
}

Agent::~Agent()
{
    // TODO: implement
}

std::vector<std::string> Agent::split(const std::string& s) {
    std::vector<std::string> result;
    std::string current;

    for (char c : s) {
        if (c == '\n') {
            result.push_back(current);
            current.clear();
        }
        else {
            current += c;
        }
    }
    result.push_back(current);
    return result;
}

bool Agent::load_prompts(const std::string& terms_file, const std::string& summarize_file)
{
    auto load_file = [](const std::string& filename, std::string& content) -> bool {
        std::ifstream file(filename);
        if (!file) return false;
        std::stringstream buffer;
        buffer << file.rdbuf();
        content = buffer.str();
        return true;
        };

    return load_file(terms_file, m_terms_prompt) && load_file(summarize_file, m_summarize_prompt);
}

bool Agent::query(const std::string& question, std::string& answer)
{

    if (m_terms_prompt.empty() || m_summarize_prompt.empty()) return false;

    // Get search terms from LLM
    std::string terms_full_prompt = m_terms_prompt;
    replace_all(terms_full_prompt, "{query}", question);

    std::string terms_llm_response;
    if (!query_llm("terms", terms_full_prompt, terms_llm_response))
        return false;

    //Get query for group of terms
    std::set<std::string> set_doc_names;
    std::vector<std::string> term_groups = split(terms_llm_response);


    std::cerr << "\nSearch terms from LLM:\n"; //delete later

    for (std::string term_group : term_groups) {
        if (term_group.empty()) continue;

        TokenizerBase* tokenizer = create_tokenizer();
        tokenizer->tokenize(term_group);

        std::vector<std::string> search_terms;
        std::string token;

        std::cerr << term_group << std::endl; //delete later
        
        while (tokenizer->next(token)) {
            search_terms.push_back(token);
        }
        delete tokenizer;

        std::vector<std::string> doc_names = m_index.query(search_terms); //set because unique
        //insert into union set
        for (std::string doc : doc_names) {
            set_doc_names.insert(doc);
        }
    }
    

    if (set_doc_names.empty()) {
        return false;
    }

    std::cerr << "\nDocuments matching all search terms:\n" << std::endl; //delete later

    // Get first 10 sorted document names (set is already sorted)
    std::string documents_content = "";
    int count = 0;
    for (const std::string& doc_name : set_doc_names) {
        if (count >= 10) break;
        std::cerr << doc_name << std::endl; //delete later
        std::ifstream doc_file(doc_name);
        if (doc_file) {
            std::stringstream buffer;
            buffer << doc_file.rdbuf();
            documents_content += buffer.str() + "\n";
            count++;
        }
    }

    // Prepare final prompt
    std::string summarize_prompt = m_summarize_prompt;
    replace_all(summarize_prompt, "{query}", question);
    replace_all(summarize_prompt, "{documents}", documents_content);

    // Get final answer
    return query_llm("summarize", summarize_prompt, answer);
}

void Agent::replace_all(std::string& context, const std::string& from, const std::string& to)
{
    size_t pos = 0;
    while ((pos = context.find(from, pos)) != std::string::npos) {
        context.replace(pos, from.length(), to);
        pos += to.length();
    }
}