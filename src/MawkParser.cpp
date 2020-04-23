#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix.hpp>

#include <iostream>
#include <fstream>
#include <string>

#include "MawkParser.h"

using namespace boost::spirit;
using boost::phoenix::val;

///////////////////////////////////////////////////////////////////////////////
//  Token definition
///////////////////////////////////////////////////////////////////////////////
template <typename Lexer>
struct AwkLexer : lex::lexer<Lexer>
{
    AwkLexer()
    {
        // define the tokens to match
        identifier = "[a-zA-Z_][a-zA-Z0-9_]*";
        ere = "\\/[a-zA-Z0-9.${}+*]+\\/";
        constant = "[0-9]+";
        print =  "print";
        Begin = "BEGIN";
        End = "END";

        // associate the tokens and the token set with the lexer
        this->self = lex::token_def<>('{') | '}' | print | Begin | End | ere | constant | identifier | '.';

        // define the whitespace to ignore (spaces, tabs, newlines and C-style 
        // comments)
        this->self("WS")
            =   lex::token_def<>("[ \\t\\n]+") 
            |   "\\/\\*[^*]*\\*+([^/*][^*]*\\*+)*\\/"
            ;

//        this->self += lex::string("."); // Catch all for error handling
    }

    //[example4_token_def

    // The following two tokens have an associated attribute type, 'identifier'
    // carries a string (the identifier name) and 'constant' carries the 
    // matched integer value.
    //
    // Note: any token attribute type explicitly specified in a token_def<>
    //       declaration needs to be listed during token type definition as 
    //       well (see the typedef for the token_type below).
    //
    // The conversion of the matched input to an instance of this type occurs
    // once (on first access), which makes token attributes as efficient as 
    // possible. Moreover, token instances are constructed once by the lexer
    // library. From this point on tokens are passed by reference only, 
    // avoiding them being copied around.
    lex::token_def<> print, Begin, End;
    lex::token_def<std::string> identifier, ere;
    lex::token_def<unsigned int> constant;
    //]
};

///////////////////////////////////////////////////////////////////////////////
//  Grammar definition
///////////////////////////////////////////////////////////////////////////////
template <typename Iterator, typename Lexer>
struct AwkParser : qi::grammar<Iterator, qi::in_state_skipper<Lexer> >
{
    template <typename TokenDef>
        AwkParser(TokenDef const& tok)
        : AwkParser::base_type(program)
        {
            using boost::spirit::_val;

            program 
                =  +block
                ;

            block
                =    -(pattern) >> action 
                ;

            action =   boost::spirit::ascii::char_('{') >> '}'
                ;

            pattern = tok.Begin 
                | tok.End
                | expr
                    ;

            expr = non_unary_expr
                | unary_expr
                ;
    
            unary_expr = '+' >> expr
                | '-' >> expr
                ; 
            non_unary_expr = tok.constant
                | tok.identifier
                | tok.ere
                ;
            program.name("Program");
            block.name("Block");
            action.name("Action");
            pattern.name("Pattern");

            boost::spirit::qi::on_error<boost::spirit::qi::fail>
            (
                program,
                std::cerr 
                << boost::phoenix::val("ERROR : Expecting ")
                << boost::spirit::_4
                << boost::phoenix::val("here : \"")
                << boost::phoenix::construct<std::string>(boost::spirit::_3, boost::spirit::_2)
                << boost::phoenix::val("\"\n")
            );

            debug(program);

        }

    typedef boost::variant<unsigned int, std::string> expression_type;

    qi::rule<Iterator, qi::in_state_skipper<Lexer> > program, block;
    qi::rule<Iterator, qi::in_state_skipper<Lexer> >  action, pattern, expr, 
                non_unary_expr,
                unary_expr;
};

bool MawkParser::parse_phrase(std::string & phrase)
{
    // iterator type used to expose the underlying input stream
    typedef std::string::iterator base_iterator_type;

    //[example4_token
    // This is the lexer token type to use. The second template parameter lists 
    // all attribute types used for token_def's during token definition (see 
    // calculator_tokens<> above). Here we use the predefined lexertl token 
    // type, but any compatible token type may be used instead.
    //
    // If you don't list any token attribute types in the following declaration 
    // (or just use the default token type: lexertl_token<base_iterator_type>)  
    // it will compile and work just fine, just a bit less efficient. This is  
    // because the token attribute will be generated from the matched input  
    // sequence every time it is requested. But as soon as you specify at 
    // least one token attribute type you'll have to list all attribute types 
    // used for token_def<> declarations in the token definition class above, 
    // otherwise compilation errors will occur.
    typedef lex::lexertl::token<
        base_iterator_type, boost::mpl::vector<unsigned int, std::string> 
        > token_type;
    //]
    // Here we use the lexertl based lexer engine.
    typedef lex::lexertl::lexer<token_type> lexer_type;

    // This is the token definition type (derived from the given lexer type).
    typedef AwkLexer<lexer_type> AwkLexerImp;

    // this is the iterator type exposed by the lexer 
    typedef AwkLexerImp::iterator_type iterator_type;

    // this is the type of the grammar to parse
    typedef AwkParser<iterator_type, AwkLexerImp::lexer_def> AwkParserImp;

    // now we use the types defined above to create the lexer and grammar
    // object instances needed to invoke the parsing process
    AwkLexerImp tokens;                         // Our lexer
    AwkParserImp calc(tokens);                  // Our parser

    // At this point we generate the iterator pair used to expose the
    // tokenized input stream.
    std::string::iterator it = phrase.begin();
    iterator_type iter = tokens.begin(it, phrase.end());
    iterator_type end = tokens.end();

    // Parsing is done based on the token stream, not the character 
    // stream read from the input.
    // Note how we use the lexer defined above as the skip parser. It must
    // be explicitly wrapped inside a state directive, switching the lexer 
    // state for the duration of skipping whitespace.
    bool r = qi::phrase_parse(iter, end, calc, qi::in_state("WS")[tokens.self]);

    return (r && iter == end);
}
