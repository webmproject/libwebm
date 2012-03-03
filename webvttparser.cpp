#include "webvttparser.hpp"

namespace WebvttParser
{

IReader::IReader()
{
}

IReader::~IReader()
{
}

Parser::Parser(IReader* p) :
    m_pReader(p)
{
}

Parser::~Parser()
{
}

}  //end namespace WebvttParser
