#include "./Pattern.hpp"

namespace rgx {

    Pattern::Pattern()
    {
        idx = 0;
        reached_end = false;
    }
    Pattern::Pattern(Pattern const &other) 
    {
        *this = other;
    }

    Pattern::~Pattern()
    {
        clear();

    }

    Pattern &Pattern::operator=(Pattern const &other)
    {
        if (this != &other)
        {
            idx = other.idx;
            content = other.content;
            reached_end = other.reached_end;
            clear();
            vector<AToken *>::const_iterator it = other.tokens.begin();
            vector<AToken *>::const_iterator end = other.tokens.end();
            for (; it != end; it++)
                tokens.push_back((*it)->clone());
        }
        return *this;
    }

    void Pattern::clear()
    {
        vector<AToken *>::const_iterator it = tokens.begin();
        vector<AToken *>::const_iterator end = tokens.end();
        for (; it != end; it++)
            delete (*it);
        tokens.clear();
    }

    Pattern &Pattern::append(AToken const &token) {
        tokens.push_back(token.clone());
        return *this;
    }

    Pattern &Pattern::append(Pattern const &pattern)
    {
        for (size_t i = 0; i < pattern.tokens.size(); i++)
        {
            tokens.push_back(pattern.tokens[i]->clone());
        }
        return *this;
    }

    bool Pattern::find(std::string const &str) {
        stringstream ss;
        idx = 0;
        return find(str, idx);
    }

    bool Pattern::find(std::string const &str, size_t &idx) {
        stringstream ss;
        size_t i;

        content = "";
        this->idx = idx;
        for (i = 0; i < tokens.size(); i++)
        {
            if (tokens[i]->find(str, this->idx, ss) == false)
            {
                reached_end = tokens[i]->is_reached_end();
                return false;
            }
        }
        if (!tokens.empty())
            reached_end = tokens[tokens.size() - 1]->is_reached_end();
        idx = this->idx;
        content = ss.str();
        return true;
    }

    bool Pattern::match(std::string const &str) {
        idx = 0;
        for (size_t i = 0; i < tokens.size(); i++)
        {
            if (tokens[i]->match(str, idx) == false)
                return false;
        }
        return idx == str.size();
    }

    bool Pattern::is_reached_end() const
    {
        return reached_end;
    }

    string const &Pattern::get_content() {
        return content;
    }
}