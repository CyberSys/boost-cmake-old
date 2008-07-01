/*=============================================================================
    Copyright (c) 2001-2007 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
///////////////////////////////////////////////////////////////////////////////
//
//  A Calculator example demonstrating the grammar and semantic actions
//  using plain functions. The parser prints code suitable for a stack
//  based virtual machine.
//
//  [ JDG May 10, 2002 ]    spirit1
//  [ JDG March 4, 2007 ]   spirit2
//
///////////////////////////////////////////////////////////////////////////////

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>

#include <iostream>
#include <string>

using namespace boost::spirit;
using namespace boost::spirit::qi;
using namespace boost::spirit::ascii;

///////////////////////////////////////////////////////////////////////////////
//  Semantic actions
///////////////////////////////////////////////////////////////////////////////
namespace
{
    void do_int(int n)  { std::cout << "push " << n << std::endl; }
    void do_add()       { std::cout << "add\n"; }
    void do_subt()      { std::cout << "subtract\n"; }
    void do_mult()      { std::cout << "mult\n"; }
    void do_div()       { std::cout << "divide\n"; }
    void do_neg()       { std::cout << "negate\n"; }
}

///////////////////////////////////////////////////////////////////////////////
//  Our calculator grammar
///////////////////////////////////////////////////////////////////////////////
template <typename Iterator>
struct calculator : grammar_def<Iterator, space_type>
{
    calculator()
    {
        expression =
            term
            >> *(   ('+' >> term            [&do_add])
                |   ('-' >> term            [&do_subt])
                )
            ;

        term =
            factor
            >> *(   ('*' >> factor          [&do_mult])
                |   ('/' >> factor          [&do_div])
                )
            ;

        factor =
            uint_                           [&do_int]
            |   '(' >> expression >> ')'
            |   ('-' >> factor              [&do_neg])
            |   ('+' >> factor)
            ;
    }

    rule<Iterator, space_type> expression, term, factor;
};

///////////////////////////////////////////////////////////////////////////////
//  Main program
///////////////////////////////////////////////////////////////////////////////
int
main()
{
    std::cout << "/////////////////////////////////////////////////////////\n\n";
    std::cout << "Expression parser...\n\n";
    std::cout << "/////////////////////////////////////////////////////////\n\n";
    std::cout << "Type an expression...or [q or Q] to quit\n\n";

    typedef std::string::const_iterator iterator_type;
    typedef calculator<iterator_type> calculator;

    calculator def; //  Our grammar definition
    grammar<calculator> calc(def, def.expression); // Our grammar

    std::string str;
    while (std::getline(std::cin, str))
    {
        if (str.empty() || str[0] == 'q' || str[0] == 'Q')
            break;

        std::string::const_iterator iter = str.begin();
        std::string::const_iterator end = str.end();
        bool r = phrase_parse(iter, end, calc, space);

        if (r && iter == end)
        {
            std::cout << "-------------------------\n";
            std::cout << "Parsing succeeded\n";
            std::cout << "-------------------------\n";
        }
        else
        {
            std::string rest(iter, end);
            std::cout << "-------------------------\n";
            std::cout << "Parsing failed\n";
            std::cout << "stopped at: \": " << rest << "\"\n";
            std::cout << "-------------------------\n";
        }
    }

    std::cout << "Bye... :-) \n\n";
    return 0;
}

