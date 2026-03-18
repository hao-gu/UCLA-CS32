#include "index.h"
#include "provided.h"
#include <string>
#include <vector>
#include <fstream>
#include <set>
#include <algorithm>


Index::Index() : m_map(create_multimap()) {}
Index::~Index() { delete m_map; }

int Index::build_index(const std::string& path)
{
    std::vector<std::string> files = get_filenames(path);

    for (std::string file : files) {
        add_doc(file);
    }
    return (int)files.size(); // TODO: replace this line with your implementation
}

void Index::add_doc(const std::string& doc_file)
{
    TokenizerBase* t = create_tokenizer();
    std::ifstream infile(doc_file);
    if (!infile) {
        delete t;
        return;
    }

    std::string line, token;
    while (std::getline(infile, line)) {
        t->tokenize(line);
        while (t->next(token)) {
            m_map->put(token, doc_file);
        }
    }
    delete t;
}

std::vector<std::string> Index::query(const std::vector<std::string>& terms) const
{
    if (terms.empty()) return {};

    std::set<std::string> files;
    bool first = true;

    for (std::string term : terms) {
        MultimapBase::IteratorBase* it = m_map->get(term);
        std::set<std::string> current_files;
        std::string file;
        while (it->next(file)) {
            current_files.insert(file);
        }
        
        delete it;
        
        if (first) {
            files = current_files;
            first = false;
            continue;
        }

        std::set<std::string> intersect;
        std::set_intersection(files.begin(), files.end(),
            current_files.begin(), current_files.end(),
            std::inserter(intersect, intersect.begin()));

        files = intersect;
    }
    return std::vector<std::string>(files.begin(), files.end());
}
