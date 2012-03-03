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

class Parser
{
private:
    Parser(const Parser&);
    Parser& operator=(const Parser&);

public:
    Parser(IReader*);
    ~Parser();

    int Parse();

    //need to know kind:
    // caption, description, metadata, etc
    // is this a webvtt stream?
    //
    //cue identifier
    //timestamp (or defaults?) + cue settings
    //payload
    //  preserve each line

    //machine states:
    //  parsing stream id ("webvtt")
    //  parsing file-wide metadata
    //  parsing cue
    //
    //within cue:
    //  parsing line
    //    is this a cue identifier or timestamp line?
    //
    //  parsing cue id
    //  parsing timestamp
    //  parsing lines, looking for end-of-cue

private:
    IReader* const m_pReader;

};


}  //end namespace WebvttParser

#endif
