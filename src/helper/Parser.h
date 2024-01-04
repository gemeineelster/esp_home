#ifndef PARSER_H
#define PARSER_H

#pragma once

#include <WString.h>

class Parser
{
public:
    Parser();
    ~Parser();
    
    void parseMQTTMessage(String msg);

private:

};

#endif