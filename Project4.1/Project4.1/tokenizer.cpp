#include "tokenizer.h"
#include <string>

Tokenizer::Tokenizer() : m_tokens({}), m_pos(0) {}

void Tokenizer::tokenize(const std::string& input)
{
	m_tokens.clear();
	m_pos = 0;
	std::string curToken = "";
	for (char c : input) {
		if (isalnum(static_cast<unsigned char>(c))) {
			curToken += tolower(c);
		}
		else {
			if (!curToken.empty()) {
				m_tokens.push_back(curToken);
			}
			curToken = "";
		}
	}
	if (!curToken.empty()) m_tokens.push_back(curToken);
}

bool Tokenizer::next(std::string& token)
{
	if (m_pos >= m_tokens.size()) {
		return false;
	}
	token = m_tokens[m_pos];
	m_pos++;
	return true;
}
