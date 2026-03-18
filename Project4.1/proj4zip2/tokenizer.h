#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "provided.h"
#include <string>
#include <vector>

class Tokenizer : public TokenizerBase
{
public:
    Tokenizer();
    virtual void tokenize(const std::string& input);
    virtual bool next(std::string& token);

private:
    std::vector<std::string> m_tokens;
    int m_pos;
};

#endif // TOKENIZER_H
