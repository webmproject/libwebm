#ifndef WEBVTTPARSER_HPP
#define WEBVTTPARSER_HPP

namespace WebvttParser
{

class IReader
{
protected:
    IReader();
    virtual ~IReader();
    
public:
    virtual int Read(char& c) = 0;

};



}  //end namespace WebvttParser

#endif
