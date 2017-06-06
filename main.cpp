# include <iostream>
# include <cstdio>
# include <cstdlib>
# include <cmath>
# include <cstring>
# include <string>
# include <vector>
# include <sstream>
# include <iomanip>
# include <algorithm>

using namespace std ;
static int uTestNum ;
enum TokenType {
  T_EMPTY,

  T_NIL,
  T_LEFT_PAREN,
  T_RIGHT_PAREN,
  T_SYMBOL,

  T_INT,
  T_FLOAT,
  T_STRING,
  T_T,
  T_FUNCTION,
  T_ERROR,

  T_EOF,
  T_QUOTE,
  T_DOT
} ;


class Token {
  public:
  Token( ) { }

  Token( TokenType t ) {
    mType = t;
  } // Token()

  Token( TokenType t, string s ) {
    mType = t ;
    mSymbol = s ;
  } // Token()

  public:
    static Token Nil() { return Token( T_NIL, "nil" ) ; }

    static Token T() { return Token( T_T, "#t" ) ; }

    static Token Empty() { return Token( T_EMPTY ) ; }

  public:
  bool IsEql( Token const &b ) const {
    return b.mSymbol == mSymbol && b.mType == mType;
  } // IsEql()

  bool IsDot() const { return mType == T_DOT; }

  public:
  TokenType mType ;
  string mSymbol ;
  int mLine, mColumn ;

  public:
  string ToString() { // debug use
    string ret;
    if ( mType == T_EOF ) { ret = "End-Of-File"; }
    else if ( mType == T_LEFT_PAREN ) { ret = "Left-Paren"; }
    else if ( mType == T_RIGHT_PAREN ) { ret = "Right-Paren"; }
    else if ( mType == T_QUOTE ) { ret = "Quote"; }
    else if ( mType == T_STRING ) { ret = "String"; }
    else if ( mType == T_FLOAT ) { ret = "Float"; }
    else if ( mType == T_INT ) { ret = "Int"; }
    else if ( mType == T_T ) { ret = "T"; }
    else if ( mType == T_NIL ) { ret = "NIL"; }
    else if ( mType == T_SYMBOL ) { ret = "mSymbol"; }
    else if ( mType == T_DOT ) { ret = "Dot"; }
    else if ( mType == T_EMPTY ) { ret = "Empty"; }
    else if ( mType == T_FUNCTION ) { ret = "Function"; }
    else {  ret = "Undefined"; }
    ret += ":" + mSymbol;
    return ret;
  } // ToString()
} ; // Token()

class Node;
typedef Node* NodePtr ;
typedef const Node* ConstNodePtr ;

bool IsBuiltInFunctionSymbol( const ConstNodePtr node ) ;

class Node {
  public:
  Node() {
    mLeft = NULL ;
    mRight = NULL ;
    mLevel = -1 ;
  } // Node()

  Node( bool b ) {
    mLeft = NULL ;
    mRight = NULL ;
    mLevel = -1 ;
    if ( b ) { SetTrue(); }
    else   { SetFalse(); }
  } // Node()

  Node( Token d ) {
    mData = d ;
    mLeft = NULL ;
    mRight = NULL ;
    mLevel = -1 ;
  } // Node()

  static NodePtr Nil() { return new Node( false ) ; }

  static NodePtr True() { return new Node( true ) ; }

  static NodePtr Error( string s ) { return new Node( Token::Token( T_ERROR, s ) ); }

  void SetFunction() {
    mData.mType = T_FUNCTION;
  } // SetFunction()

  void SetTrue() {
    mData = Token::T();
  } // SetTrue()

  void SetFalse() {
    mData = Token::Nil();
  } // SetFalse()

  void SetLevel( int l ) { mLevel = l; }

  int GetLevel() const { return mLevel; }

  bool IsFunction() const { return mData.mType == T_FUNCTION ; }

  bool IsString() const { return mData.mType == T_STRING ; }

  bool IsSymbol() const { return mData.mType == T_SYMBOL; }

  bool IsAtom() const { return mLeft == NULL && mRight == NULL; }

  bool IsNil() const { return mData.mType == T_NIL; }

  bool IsQuote() const { return mData.mType == T_SYMBOL && mData.mSymbol == "quote" ; }

  bool IsError() const { return mData.mType == T_ERROR; }

  float ToNumber() const {
    float v ;
    stringstream ss ;
    ss << Symbol() ;
    ss >> v ;
    return v ;
  }  // ToNumber()

  string ToString() {
    return Symbol().substr( 1, Symbol().size()-2 );
  } // ToString()

  bool IsT() const { return mData.mType == T_T; }

  bool IsTrue() const {
    return !IsNil() ;
  } // IsTrue()

  bool IsNumber()  const { return mData.mType == T_INT || mData.mType == T_FLOAT; }

  bool IsFloat() const { return mData.mType == T_FLOAT ; }

  bool IsInteger() const { return mData.mType == T_INT ; }

  bool IsBool() const { return mData.mType == T_T || mData.mType == T_NIL; }

  bool IsList() const {
    if ( IsNil() ) { return false; }
    if ( mLeft == NULL || mRight == NULL ) { return false; }
    if ( mRight->IsNil() ) { return true; }
    return mRight->IsList();
  } // IsList()

  public:
  Token mData ;
  string Symbol() const { return mData.mSymbol ; }

  Node* mLeft ;
  Node* mRight ;

  private:
  int mLevel ;
} ; // struct Node


struct NodeDef {
  string name ;
  NodePtr ref ;
} ; // NodeDef

vector<vector<NodeDef> > gDefine;

typedef char Str[ 1000 ] ;
// void ReadSExp( NodePtr& nowNode, Token &token, int mLevel = 0)
void ReadSExp( NodePtr& nowNode, Token &token, int mLevel ) ;

void ReadSExp( NodePtr& nowNode, Token &token ) { // style sucks
  ReadSExp( nowNode, token, 0 ) ;
} // ReadSExp()

void ShowTree( NodePtr node, int mLevel ) ; // debug use
void ShowTree( NodePtr& nowNode )  { // style sucks
  ShowTree( nowNode, 0 ) ;
} // ShowTree()

NodePtr EvalBuiltInFunction( const NodePtr node ) ;

NodePtr CloneTree( const NodePtr node ) ;

NodePtr EvalErrorObject( const NodePtr node ) ;
NodePtr EvalCreateErrorObject( const NodePtr node ) ;
NodePtr EvalEval( const NodePtr node ) ;
NodePtr EvalSet( const NodePtr node ) ;
NodePtr EvalRead( const NodePtr node ) ;
NodePtr EvalWrite( const NodePtr node ) ;
NodePtr Eval( const NodePtr node ) ;
NodePtr EvalShowTree( const NodePtr node ) ;
NodePtr EvalCons( const NodePtr node ) ;
NodePtr EvalList( const NodePtr node ) ;
NodePtr EvalQuote( const NodePtr node ) ;
NodePtr EvalDefine( const NodePtr node ) ;
NodePtr EvalCar( const NodePtr node ) ;
NodePtr EvalCdr( const NodePtr node ) ;
NodePtr EvalAdd( const NodePtr node ) ;
NodePtr EvalMinus( const NodePtr node ) ;
NodePtr EvalTimes( const NodePtr node ) ;
NodePtr EvalDivide( const NodePtr node ) ;
NodePtr EvalInteger( const NodePtr node ) ;
NodePtr EvalPair( const NodePtr node ) ;
NodePtr EvalNull( const NodePtr node ) ;
NodePtr EvalReal( const NodePtr node ) ;
NodePtr EvalNumber( const NodePtr node ) ;
NodePtr EvalString( const NodePtr node ) ;
NodePtr EvalBoolean( const NodePtr node ) ;
NodePtr EvalSymbol( const NodePtr node ) ;
NodePtr EvalBigger( const NodePtr node ) ;
NodePtr EvalBiggerE( const NodePtr node ) ;
NodePtr EvalSmaller( const NodePtr node ) ;
NodePtr EvalSmallerE( const NodePtr node ) ;
NodePtr EvalEqual( const NodePtr node ) ;
NodePtr EvalNot( const NodePtr node ) ;
NodePtr EvalAnd( const NodePtr node ) ;
NodePtr EvalOr( const NodePtr node ) ;
NodePtr EvalIsString( const NodePtr node ) ;
NodePtr EvalIf( const NodePtr node ) ;
NodePtr EvalCond( const NodePtr node ) ;
NodePtr EvalStringA( const NodePtr node ) ;
NodePtr EvalStringB( const NodePtr node ) ;
NodePtr EvalBegin( const NodePtr node ) ;
NodePtr EvalEqv( const NodePtr node ) ;
NodePtr EvalEqualQ( const NodePtr node ) ;
NodePtr EvalExit( const NodePtr node ) ;
NodePtr EvalClean( const NodePtr node ) ;
NodePtr EvalLet( const NodePtr node ) ;
NodePtr EvalLambda( const NodePtr node ) ;
vector<NodeDef> GetDefines( const NodePtr node ) ;
string GetResult( NodePtr node, bool walkLeft, int mLevel, bool firstAtom ) ;
string GetResult( NodePtr node ) {
  return GetResult( node, true, 0, true ) ;
} // GetResult() ;

void PrintResult( NodePtr node ) ;
void ClearToLineEnd() ;
void ReadToLineEnd() ;
Token GetToken() ; // GetToken
bool IsFloatString( string mSymbol ) ; // 判斷是否是float
bool IsDigitString( string mSymbol ) ; // 判斷是否是int
bool gEndOfAll = false ; // end of whole project
bool gPrint = true ; // token build error so no need to print
bool gFirstDef = true ; // 用來判斷是否第一個define,為了接root

char ReadCountPost() ; // 去計算cin完的mColumn跟mLine放回class,在做error case use
string NumberToString( int num ) ;
int NumberOfArguments( NodePtr node ) ;
int MinNumberOfArguments( NodePtr node ) ;
int MaxNumberOfArguments( NodePtr node ) ;

int gLine = 1, gColumn = 0 ;

// map<string, NodePtr (*)(const NodePtr) throw(string)> gFunctionMap ;

string IncorrectNumberOfArgumentsError( string name ) {
  if ( name == "" ) name = "lambda expression" ;
  string ret = "ERROR (incorrect number of arguments) : " + name ;
  return ret ;
} // IncorrectNumberOfArgumentsError()

string IncorrectArgumentTypeError( string name, NodePtr result ) {
  if ( name == "" ) name = "lambda expression" ;
  string ret = "ERROR (" + name + " with incorrect argument type) : " + GetResult( result ) ;
  return ret ;
} // IncorrectArgumentTypeError()

string UnexpectedCharErrorString() {
  char ch = cin.peek();
  string str  ;
  stringstream ss ;

  if ( ch == '\n' ) str = "LINE-ENTER encountered" ;
  else  ss << "character '" << ch << "'", str = ss.str() ;
  string ret = "ERROR (unexpected character) : line " + NumberToString( gLine )
         + " column " + NumberToString( gColumn + 1 ) + " " + str ;
  return ret;
} // UnexpectedCharErrorString()

string UnexpectedCharErrorString( const Token t ) {
  stringstream ss ;
  ss << "character '" << t.mSymbol[0] << "'" ;
  string str = ss.str() ;
  string ret ;
  ret = "ERROR (unexpected character) : line " + NumberToString( t.mLine )
         + " column " + NumberToString( t.mColumn ) + " " + str ;
  return ret ;
} // UnexpectedCharErrorString()

void SetLevel( NodePtr node, int mLevel ) {
  if ( node == NULL ) return;
  node->SetLevel( mLevel ) ;
  SetLevel( node->mLeft, mLevel+1 );
  SetLevel( node->mRight, mLevel+1 );
} // SetLevel()

void SetLevel( NodePtr node ) {
  SetLevel( node, 0 );
} // SetLevel()

int main() {
  gDefine.resize( 1 );

  char charforenter ;
  scanf( "%d%c", &uTestNum, &charforenter ) ;
  cout <<  "Welcome to OurScheme!" << endl;
  while ( gEndOfAll != true ) {
    Node* root = NULL ;
    printf( "\n> " ) ;
    try {
      try {
        Token t = GetToken() ;
        ReadSExp( root, t, 0 ) ;
      } // try
      catch( string s ) { // parsing failed
        ReadToLineEnd() ;
        //  ClearToLineEnd() ;
        throw s ;
      } // catch

      ClearToLineEnd() ;
      SetLevel( root ) ;

      NodePtr result = Eval( root ) ;
      PrintResult( result ) ;
    } // try
    catch ( string s ) {
      cout << s << endl ;
      gDefine.resize( 1 ) ;
      if (s == "ERROR : END-OF-FILE encountered when there should be more input" ) gEndOfAll = true;
    } // catch

  } // while()

  cout << endl << "Thanks for using OurScheme!" ;

} // main()

void ShowTree( NodePtr node, int mLevel ) {
  if ( node != NULL ) {
    for ( int i = 0 ; i < mLevel * 4 ; ++i ) {
      cout << " " ;
    } // for

    cout << node->mData.ToString() << endl ;
    ShowTree( node->mLeft, mLevel+1 ) ;
    ShowTree( node->mRight, mLevel+1 ) ;
  } // if
} // ShowTree()

void ReadSExp( NodePtr &node, Token &t, int mLevel )  {
  node = new Node ;
  if ( t.mType == T_EMPTY ) { t = GetToken() ; }
  if ( t.mType == T_LEFT_PAREN ) { // meet '(' probably need to new a node
    t = GetToken() ;
    if ( t.mType == T_RIGHT_PAREN ) {
      if ( mLevel > 0 ) t = GetToken() ;
      node->mData = Token::Nil() ;
    } // if
    else {
      node->mData = Token::Empty() ;
      ReadSExp( node->mLeft, t, mLevel+1 ) ;
      { // block
        NodePtr parent = node;
        while ( !t.IsDot() && t.mType != T_RIGHT_PAREN && t.mType != T_EOF ) {
          parent->mRight = new Node ;
          parent->mRight->mData = Token::Token( T_EMPTY ) ;
          ReadSExp( parent->mRight->mLeft, t, mLevel+1 ) ;
          if ( t.mType == T_EMPTY ) { t = GetToken() ; }
          parent = parent->mRight;
        } // while()

        if ( t.mType == T_DOT ) {
          t = GetToken() ;
          ReadSExp( parent->mRight, t, mLevel+1 ) ;
        } // if
        else {
          parent->mRight = new Node ;
          parent->mRight->mData = Token::Nil() ;
        } // else
      } // else
      if ( t.mType == T_RIGHT_PAREN ) {
        if ( mLevel > 0 ) {
          t = GetToken() ;
        } // if
      } // if
      else {
        throw UnexpectedCharErrorString( t ) ;
      } // else
    } // else
  } // if
  else if ( t.mType == T_RIGHT_PAREN ) { // NULL
    // if ( mLevel == 0 ) { // change

    throw UnexpectedCharErrorString( t ) ;
    // } // if
  } // else if
  else if ( t.mType == T_QUOTE ) {
    t = GetToken();
    node->mData = Token::Token( T_EMPTY ) ;
    node->mLeft = new Node ;
    node->mLeft->mData = Token::Token( T_SYMBOL, "quote" ) ;
    node->mRight = new Node( Token::Token( T_EMPTY ) ) ;
    node->mRight->mRight = Node::Nil() ;
    ReadSExp( node->mRight->mLeft, t, mLevel ) ;
  } // else if
  else if ( t.IsDot() ) {

    throw UnexpectedCharErrorString() ;
  } // else if
  else {
    node->mData = t ;
    if ( mLevel != 0 ) { t = GetToken() ; }
  } // else
} // ReadSExp()


NodePtr EvalDisplayString( const NodePtr node ) {
  NodePtr result = node->mLeft;
  cout << result->Symbol().substr( 1, result->Symbol().size()-2 ) ;
  return result ;
} // EvalDisplayString()



NodePtr EvalNewline( const NodePtr node )  {
  cout << "\n" ;
  return Node::Nil() ;
} // EvalNewline()

NodePtr EvalRead( const NodePtr node )  {
  NodePtr ret = NULL ;
  try {
    Token t = GetToken() ;
    ReadSExp( ret, t ) ;
  } // try
  catch ( string s ) {
    ReadToLineEnd() ;
    return Node::Error( "\"" + s + "\"" ) ;
  } // catch

  return ret ;
} // EvalRead()

NodePtr EvalErrorObject( const NodePtr node )  {
  return new Node( node->mLeft->IsError() ) ;
} // EvalErrorObject()

NodePtr EvalCreateErrorObject( const NodePtr node )  {
  return Node::Error( node->mLeft->Symbol() ) ;
} // EvalCreateErrorObject()

NodePtr EvalEval( const NodePtr node )  {
  return Eval( node->mLeft ) ;
} // EvalEval()

NodePtr EvalWrite( const NodePtr node )  {
  NodePtr ret = node->mLeft ;
  cout << GetResult( ret ) ;
  return ret ;
} // EvalWrite()

NodePtr EvalFunction( const NodePtr node ) {
  NodePtr result = new Node() ;
  NodePtr cur = new Node() ;
  result->mLeft = cur ;
  NodePtr var   = node->mLeft->mLeft ;
  NodePtr value = node->mRight ;
  while ( !var->IsNil() ) {
    NodePtr newNode = new Node() ;
    newNode->mLeft  = new Node( var->mLeft->mData ) ;
    newNode->mRight = new Node() ;
    newNode->mRight->mLeft = value->mLeft ;
    newNode->mRight->mRight = Node::Nil() ;
    cur->mLeft = newNode ;
    cur->mRight = new Node() ;
    cur = cur->mRight ;
    var = var->mRight ;
    value = value->mRight ;
  } // while()

  cur->SetFalse() ;
  result->mRight = CloneTree( node->mLeft->mRight ) ;

  gDefine.push_back( GetDefines( result->mLeft ) ) ;
  cur = result->mRight ;
  NodePtr ret = NULL ;
  while ( !cur->IsNil() ) {
    ret = Eval( cur->mLeft ) ;
    cur = cur->mRight ;
  } // while

  gDefine.pop_back() ;
  return ret ;
} // EvalFunction()

NodePtr EvalBuiltInFunction( const NodePtr node ) {
  if ( node->mLeft->Symbol() == "create-error-object" ) return EvalCreateErrorObject( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "error-object?" ) return EvalErrorObject( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "set!" ) return  EvalSet( node->mRight ) ; // here
  else if ( node->mLeft->Symbol() == "eval" ) return EvalEval( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "newline" ) return EvalNewline( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "write" ) return EvalWrite( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "read" ) return EvalRead( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "show" ) return EvalShowTree( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "cons" ) return EvalCons( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "list" ) return EvalList( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "'" ) return EvalQuote( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "quote" ) return EvalQuote( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "define" ) return EvalDefine( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "car" ) return EvalCar( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "cdr" ) return EvalCdr( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "pair?" ) return EvalPair( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "null?" ) return EvalNull( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "integer?" ) return EvalInteger( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "real?" ) return EvalReal( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "number?" ) return EvalNumber( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "boolean?" ) return EvalBoolean( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "symbol?" ) return EvalSymbol( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "+" ) return EvalAdd( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "-" ) return EvalMinus( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "*" ) return EvalTimes( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "/" ) return EvalDivide( node->mRight ) ;
  else if ( node->mLeft->Symbol() == ">" ) return EvalBigger( node->mRight ) ;
  else if ( node->mLeft->Symbol() == ">=" ) return EvalBiggerE( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "<" ) return EvalSmaller( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "<=" ) return EvalSmallerE( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "=" ) return EvalEqual( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "not" ) return EvalNot( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "and" ) return EvalAnd( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "or" ) return EvalOr( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "if" ) return EvalIf( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "string-append" ) return  EvalStringA( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "string>?" ) return  EvalStringB( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "string?" ) return  EvalIsString( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "begin" ) return  EvalBegin( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "eqv?" ) return  EvalEqv( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "equal?" ) return  EvalEqualQ( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "cond" ) return  EvalCond( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "clean-environment" ) return EvalClean( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "exit" ) return  EvalExit( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "let" ) return  EvalLet( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "lambda" ) return  EvalLambda( node->mRight ) ;
  else if ( node->mLeft->Symbol() == "display-string" ) return  EvalDisplayString( node->mRight ) ;
  return new Node( T_EMPTY );
} // EvalBuiltInFunction()

bool IsBuiltInFunctionSymbol( const ConstNodePtr node ) {
  if ( node->Symbol() == "create-error-object" ) return true ;
  else if ( node->Symbol() == "error-object?" ) return true ;
  else if ( node->Symbol() == "set!" ) return true ;
  else if ( node->Symbol() == "eval" ) return true ;
  else if ( node->Symbol() == "newline" ) return true ;
  else if ( node->Symbol() == "write" ) return true ;
  else if ( node->Symbol() == "read" ) return true ;
  else if ( node->Symbol() == "show" ) return true ;
  else if ( node->Symbol() == "cons" ) return true ;
  else if ( node->Symbol() == "list" ) return true ;
  else if ( node->Symbol() == "'" ) return true ;
  else if ( node->Symbol() == "quote" ) return true ;
  else if ( node->Symbol() == "define" ) return true;
  else if ( node->Symbol() == "car" ) return true ;
  else if ( node->Symbol() == "cdr" ) return true ;
  else if ( node->Symbol() == "pair?" ) return true ;
  else if ( node->Symbol() == "null?" ) return true ;
  else if ( node->Symbol() == "integer?" ) return true ;
  else if ( node->Symbol() == "real?" ) return true ;
  else if ( node->Symbol() == "number?" ) return true ;
  else if ( node->Symbol() == "boolean?" ) return true ;
  else if ( node->Symbol() == "symbol?" ) return true ;
  else if ( node->Symbol() == "+" ) return true ;
  else if ( node->Symbol() == "-" ) return true ;
  else if ( node->Symbol() == "*" ) return true ;
  else if ( node->Symbol() == "/" ) return true ;
  else if ( node->Symbol() == ">" ) return true ;
  else if ( node->Symbol() == ">=" ) return true ;
  else if ( node->Symbol() == "<" ) return true ;
  else if ( node->Symbol() == "<=" ) return true ;
  else if ( node->Symbol() == "=" ) return true ;
  else if ( node->Symbol() == "not" ) return true ;
  else if ( node->Symbol() == "and" ) return true ;
  else if ( node->Symbol() == "or" ) return true ;
  else if ( node->Symbol() == "if" ) return true ;
  else if ( node->Symbol() == "string-append" ) return  true ;
  else if ( node->Symbol() == "string>?" ) return  true ;
  else if ( node->Symbol() == "string?" ) return  true ;
  else if ( node->Symbol() == "begin" ) return  true ;
  else if ( node->Symbol() == "eqv?" ) return  true ;
  else if ( node->Symbol() == "equal?" ) return  true ;
  else if ( node->Symbol() == "cond" ) return  true ;
  else if ( node->Symbol() == "clean-environment" ) return true ;
  else if ( node->Symbol() == "exit" ) return  true ;
  else if ( node->Symbol() == "let" ) return  true ;
  else if ( node->Symbol() == "lambda" ) return  true ;
  else if ( node->Symbol() == "display-string" ) return  true ;
  return false ;
} // IsBuiltInFunctionSymbol()

NodePtr EvalIsString( const NodePtr node ) {
  return new Node( node->mLeft->IsString() ) ;
} // EvalIsString()

NodePtr Eval( const NodePtr node ) {
  if ( node->IsAtom() && !node->IsSymbol() ) {
    return node;
  } // if
  else if ( node->IsSymbol() ) {
    // search built-in symbols
    if ( IsBuiltInFunctionSymbol( node ) ) {
      node->SetFunction() ;
      return node ;
    } // if

    // search defined symbols
    for ( int i = ( int ) gDefine.size() - 1 ; i >= 0 ; i-- ) {
      for ( int j = ( int ) gDefine[i].size() - 1 ; j >= 0 ; j-- ) {
        if ( gDefine[i][j].name == node->Symbol() ) {
          return gDefine[i][j].ref ;
        } // if
      } // for
    } // for

    // unbounded error
    stringstream err ;
    err << "ERROR (unbound symbol) : " << GetResult( node ) ;
    throw err.str() ;
  } // else if
  else {
    if ( !node->IsList() ) {
      stringstream err ;
      err << "ERROR (non-list) : " << GetResult( node ) ;
      throw err.str() ;
    } // if

    if ( node->mLeft->IsAtom() && !node->mLeft->IsSymbol() ) {
      stringstream err ;
      err << "ERROR (attempt to apply non-function) : " << GetResult( node->mLeft ) ;
      throw err.str() ;
    } // if

    NodePtr result = new Node();
    result->mLeft = Eval( node->mLeft ) ;

    if ( result->mLeft->IsFunction() ) {
      if ( result->mLeft->Symbol() != "" ) {
        // Built-in functions
        string sym = result->mLeft->Symbol();

        // check level of "clean-environment", "define", "exit"
        if ( node->GetLevel() != 0 ) {
          if ( sym == "clean-environment" ) {
            throw string( "ERROR (clean-environment format)" ) ;
          } // if
          else if ( sym == "define" ) {
            throw string( "ERROR (define format)" ) ;
          } // else if
          else if ( sym == "exit" ) {
            throw string( "ERROR (level of exit)" ) ;
          } // else if
        } // if

        // check format of "define", "let", "cond", "let", "set!"
        if ( sym == "define" ) {
          string err = "ERROR (define format)" ; // change
          // format 1 (define symbol S-exp)
          if ( NumberOfArguments( node->mRight ) < 2 ) throw err ;

          if ( node->mRight->mLeft->IsSymbol() ) {
            if ( NumberOfArguments( node->mRight ) > 2 ||
                 IsBuiltInFunctionSymbol( node->mRight->mLeft ) ) {
              throw err ;
            } // if
          } // if
          else {
            // format 2 (define (symbol ... ) S-exp S-exp ...)
            if ( !node->mRight->mLeft->IsList() ) throw err ;

            NodePtr cur = node->mRight->mLeft;
            do {
              if ( !cur->mLeft->IsSymbol() || IsBuiltInFunctionSymbol( cur->mLeft ) ) {
                throw err ;
              } // if

              cur = cur->mRight;
            } while ( !cur->IsNil() );
          } // if
        } // if
        else if ( sym == "let" ) {
          if ( NumberOfArguments( node->mRight ) < 2 ) {
            throw string( "ERROR (LET format)" ) ;
          } // if

          NodePtr cur = node->mRight->mLeft ;
          while ( !cur->IsNil() ) {
            if ( cur->mLeft == NULL ||
                 cur->mLeft->mLeft == NULL ||
                 cur->mLeft->mRight == NULL ||
                 cur->mLeft->mRight->mRight == NULL ||
                 !cur->mLeft->mRight->mRight->IsNil() ||
                 !cur->mLeft->mLeft->IsSymbol() ||
                 IsBuiltInFunctionSymbol( cur->mLeft->mLeft ) ) {
              throw string( "ERROR (LET format)" ) ;
            } // if

            cur = cur->mRight ;
          } // while
        } // else if
        else if ( sym == "cond" ) {
          if ( NumberOfArguments( node->mRight ) == 0 ) {
            throw IncorrectNumberOfArgumentsError( node->mLeft->Symbol() ) ;
          } // if

          string err = "ERROR (cond parameter format) : " ; // change
          NodePtr cur = node->mRight ;
          while ( !cur->IsNil() ) {
            if ( cur->mLeft->mLeft == NULL || cur->mLeft->mRight == NULL ||
                 cur->mLeft->mRight->mLeft == NULL )
              throw err + GetResult( cur->mLeft ) ;
            cur = cur->mRight ;
          } // while
        } // else if
        else if ( sym == "lambda" ) {
          string err = "ERROR (lambda format)" ;
          if ( NumberOfArguments( node->mRight ) < 2 ) {
            throw err ;
          } // if

          if ( node->mRight->mLeft->IsAtom() && !node->mRight->mLeft->IsNil() ) {
            throw err ;
          } // if

          NodePtr cur = node->mRight->mLeft ;
          while ( !cur->IsNil() ) {
            if ( !cur->mLeft->IsAtom() || !cur->mLeft->IsSymbol() ||
                 IsBuiltInFunctionSymbol( cur->mLeft ) ) {
              throw err ;
            } // if

            cur = cur->mRight ;
          } // while
        } // else if
        else if ( sym == "set!" ) {
          string err = "ERROR (set! format)" ; // change
          if ( NumberOfArguments( node->mRight ) != 2 ||
               IsBuiltInFunctionSymbol( node->mRight->mLeft ) ) {
            throw err ;
          } // if

          if ( !node->mRight->mLeft->IsSymbol() ) {
            throw err ;
          } // if
        } // else if
        else {
          // check number of arguments
          int numargs = NumberOfArguments( node->mRight );
          int minargs = MinNumberOfArguments( result->mLeft );
          int maxargs = MaxNumberOfArguments( result->mLeft );
          if ( ( minargs != -1 && numargs < minargs ) || ( maxargs != -1 && numargs > maxargs ) ) {
            throw IncorrectNumberOfArgumentsError( node->mLeft->Symbol() ) ;
          } // if
        } // else
      } // if
      else if ( NumberOfArguments( node->mRight ) != NumberOfArguments( result->mLeft->mLeft ) ) {
        throw IncorrectNumberOfArgumentsError( node->mLeft->Symbol() ) ;
      } // else if
    } // if
    else {
      stringstream err ;
      err << "ERROR (attempt to apply non-function) : " << GetResult( result->mLeft ) ;
      throw err.str() ;
    } // else

    // bipass the special functions
    if ( result->mLeft->IsFunction() ) {
      string sym = result->mLeft->Symbol() ;
      if ( sym == "quote" || sym == "define" || sym == "cond" ||
           sym == "if" || sym == "let"  || sym == "lambda"  || sym == "set!" ) {
        result->mRight = CloneTree( node->mRight ) ;
      } // if
    } // if

    // evaluate arguments
    if ( result->mRight == NULL ) {
      NodePtr cur = node->mRight ;
      NodePtr cur_r = result;
      while ( !cur->IsNil() ) {
        cur_r->mRight = new Node();
        cur_r = cur_r->mRight;
        cur_r->mLeft = Eval( cur->mLeft ) ;
        // check argument types
        if ( result->mLeft->IsFunction() ) {
          string sym = result->mLeft->Symbol() ;
          if ( sym == "car" || sym == "cdr" ) {
            if ( cur_r->mLeft->IsAtom() || cur_r->mLeft->IsFunction() ) {
              throw IncorrectArgumentTypeError( node->mLeft->Symbol(), cur_r->mLeft ) ;
            } // if
          } // if
          else if ( sym == "+" || sym == "-" || sym == "*" || sym == "/" || sym == ">" ||
                    sym == ">=" || sym == "<" || sym == "<=" || sym == "=" ) {
            if ( !cur_r->mLeft->IsNumber() ) {
              throw IncorrectArgumentTypeError( node->mLeft->Symbol(), cur_r->mLeft ) ;
            } // if
          } // else if
          else if ( sym == "string-append" || sym == "string>?" ||
                    sym == "create-error-object" ) {
            if ( !cur_r->mLeft->IsString() ) {
              throw IncorrectArgumentTypeError( node->mLeft->Symbol(), cur_r->mLeft ) ;
            } // if
          } // else if
          else if ( sym == "display-string" ) {
            if ( !cur_r->mLeft->IsString() && !cur_r->mLeft->IsError() ) {
              throw IncorrectArgumentTypeError( node->mLeft->Symbol(), cur_r->mLeft ) ;
            } // if
          } // else if
        } // if

        cur = cur->mRight;
      } // while

      cur_r->mRight = Node::Nil();
    } // if

    // ShowTree( result ) ;
    NodePtr ret = NULL;
    if ( result->mLeft->Symbol() != "" ) {
      ret = EvalBuiltInFunction( result ) ;
    } // if
    else {
      ret = EvalFunction( result ) ;
    } // else

    if ( ret == NULL ) {
      throw string( "ERROR (no return value) : " + node->mLeft->Symbol() ) ;
    } // if

    return ret ;
  } // else

  return NULL ;
} // Eval()

bool IsArgumentAllNumbers( NodePtr node ) {
  while ( !node->IsNil() ) {
    if ( !node->IsNumber() ) {
      return false;
    } // if

    node = node->mRight ;
  } // while

  return true ;
} // IsArgumentAllNumbers()

int NumberOfArguments( NodePtr node ) {
  int ret = 0;
  while ( !node->IsNil() ) {
    ++ret;
    node = node->mRight;
  } // while

  return ret;
} // NumberOfArguments()

int MinNumberOfArguments( NodePtr node ) {
  if ( node->Symbol() == "cons" ) { return 2 ; }
  if ( node->Symbol() == "list" ) { return 1 ; }
  if ( node->Symbol() == "quote" ) { return 1 ; }
  if ( node->Symbol() == "define" ) { return 2 ; }
  if ( node->Symbol() == "car" ) { return 1 ; }
  if ( node->Symbol() == "cdr" ) { return 1 ; }
  if ( node->Symbol() == "pair?" ) { return 1 ; }
  if ( node->Symbol() == "null?" ) { return 1 ; }
  if ( node->Symbol() == "integer?" ) { return 1 ; }
  if ( node->Symbol() == "real?" ) { return 1 ; }
  if ( node->Symbol() == "number?" ) { return 1 ; }
  if ( node->Symbol() == "string?" ) { return 1 ; }
  if ( node->Symbol() == "boolean?" ) { return 1 ; }
  if ( node->Symbol() == "symbol?" ) { return 1 ; }
  if ( node->Symbol() == "+" ) { return 2 ; }
  if ( node->Symbol() == "-" ) { return 2 ; }
  if ( node->Symbol() == "*" ) { return 2 ; }
  if ( node->Symbol() == "/" ) { return 2 ; }
  if ( node->Symbol() == "not" ) { return 1 ; }
  if ( node->Symbol() == "and" ) { return 2 ; }
  if ( node->Symbol() == "or" ) { return 2 ; }
  if ( node->Symbol() == ">" ) { return 2 ; }
  if ( node->Symbol() == ">=" ) { return 2 ; }
  if ( node->Symbol() == "<" ) { return 2 ; }
  if ( node->Symbol() == "<=" ) { return 2 ; }
  if ( node->Symbol() == "=" ) { return 2 ; }
  if ( node->Symbol() == "string-append" ) { return 2 ; }
  if ( node->Symbol() == "string>?" ) { return 2 ; }
  if ( node->Symbol() == "eqv?" ) { return 2 ; }
  if ( node->Symbol() == "equal?" ) { return 2 ; }
  if ( node->Symbol() == "begin" ) { return 1 ; }
  if ( node->Symbol() == "if" ) { return 3 ; }
  if ( node->Symbol() == "cond" ) { return 1 ; }
  if ( node->Symbol() == "exit" ) { return 0 ; }
  if ( node->Symbol() == "clean-environment" ) { return 0 ; }
  if ( node->Symbol() == "create-error-object" ) { return 1 ; }
  if ( node->Symbol() == "error-object?" ) { return 1 ; }
  if ( node->Symbol() == "read" ) { return 0 ; }
  if ( node->Symbol() == "write" ) { return 1 ; }
  if ( node->Symbol() == "display-string" ) { return 1 ; }
  if ( node->Symbol() == "eval" ) { return 1 ; }
  if ( node->Symbol() == "set!" ) { return 2 ; }
  if ( node->Symbol() == "newline" ) { return 0 ; }
  return -1;
} // MinNumberOfArguments()

int MaxNumberOfArguments( NodePtr node ) {
  if ( node->Symbol() == "cons" ) { return 2 ; }
  if ( node->Symbol() == "list" ) { return -1 ; }
  if ( node->Symbol() == "quote" ) { return 1 ; }
  if ( node->Symbol() == "define" ) { return 2 ; }
  if ( node->Symbol() == "car" ) { return 1 ; }
  if ( node->Symbol() == "cdr" ) { return 1 ; }
  if ( node->Symbol() == "pair?" ) { return 1 ; }
  if ( node->Symbol() == "null?" ) { return 1 ; }
  if ( node->Symbol() == "integer?" ) { return 1 ; }
  if ( node->Symbol() == "real?" ) { return 1 ; }
  if ( node->Symbol() == "number?" ) { return 1 ; }
  if ( node->Symbol() == "string?" ) { return 1 ; }
  if ( node->Symbol() == "boolean?" ) { return 1 ; }
  if ( node->Symbol() == "symbol?" ) { return 1 ; }
  if ( node->Symbol() == "+" ) { return -1 ; }
  if ( node->Symbol() == "-" ) { return -1 ; }
  if ( node->Symbol() == "*" ) { return -1 ; }
  if ( node->Symbol() == "/" ) { return -1 ; }
  if ( node->Symbol() == "not" ) { return 1 ; }
  if ( node->Symbol() == "and" ) { return 2 ; }
  if ( node->Symbol() == "or" ) { return 2 ; }
  if ( node->Symbol() == ">" ) { return -1 ; }
  if ( node->Symbol() == ">=" ) { return -1 ; }
  if ( node->Symbol() == "<" ) { return -1 ; }
  if ( node->Symbol() == "<=" ) { return -1 ; }
  if ( node->Symbol() == "=" ) { return -1 ; }
  if ( node->Symbol() == "string-append" ) { return -1 ; }
  if ( node->Symbol() == "string>?" ) { return -1 ; }
  if ( node->Symbol() == "eqv?" ) { return 2 ; }
  if ( node->Symbol() == "equal?" ) { return 2 ; }
  if ( node->Symbol() == "begin" ) { return -1 ; }
  if ( node->Symbol() == "if" ) { return 3 ; }
  if ( node->Symbol() == "cond" ) { return -1 ; }
  if ( node->Symbol() == "exit" ) { return 0 ; }
  if ( node->Symbol() == "clean-environment" ) { return 0 ; }
  if ( node->Symbol() == "create-error-object" ) { return 1 ; }
  if ( node->Symbol() == "error-object?" ) { return 1 ; }
  if ( node->Symbol() == "read" ) { return 0 ; }
  if ( node->Symbol() == "write" ) { return 1 ; }
  if ( node->Symbol() == "display-string" ) { return 1 ; }
  if ( node->Symbol() == "eval" ) { return 1 ; }
  if ( node->Symbol() == "set!" ) { return 2 ; }
  if ( node->Symbol() == "newline" ) { return 0 ; }
  return -1;
} // MaxNumberOfArguments()

NodePtr EvalCons( const NodePtr node ) {
  NodePtr ret = new Node ;
  ret->mLeft = node->mLeft ;
  ret->mRight = node->mRight->mLeft ;
  return ret ;
} // EvalCons()

NodePtr EvalList( const NodePtr node ) {
  return node;
} // EvalList()

NodePtr EvalQuote( const NodePtr node ) {
  NodePtr ret = CloneTree( node->mLeft ) ;
  return ret ;
} // EvalQuote()

NodePtr EvalDefine( const NodePtr node ) {
  NodePtr nodeForDefine = new Node ;
  NodeDef nodeDef ;
  if ( node->mLeft->IsSymbol() ) {
    nodeDef.name = node->mLeft->Symbol() ;
    nodeDef.ref  = Eval( node->mRight->mLeft );
  } // if
  else {
    // Second define ?
    nodeDef.name = node->mLeft->mLeft->Symbol();
    NodePtr result = new Node();
    result->SetFunction();
    result->mLeft = new Node() ;
    NodePtr cur = result->mLeft ;
    NodePtr var = node->mLeft->mRight ;
    while ( !var->IsNil() ) {
      cur->mLeft = new Node( var->mLeft->mData ) ;
      cur->mRight = new Node ;
      cur = cur->mRight ;
      var = var->mRight ;
    } // while()

    cur->SetFalse();
    result->mRight = CloneTree( node->mRight ) ;
    nodeDef.ref = result ;
  } // else

  nodeForDefine->mData = Token::Token( T_STRING, nodeDef.name + " defined" ) ;
  int i;
  bool end = false ;
  for ( i = 0 ; i < ( int ) gDefine[0].size() && !end ; ++i ) {
    if ( gDefine[0][i].name == nodeDef.name ) {
      gDefine[0][i] = nodeDef ;
      end = true ;
      i-- ;
    } // if
  } // for

  if ( i == gDefine[0].size() ) {
    gDefine[0].push_back( nodeDef ) ;
  } // if

  return nodeForDefine ; // temp

} // EvalDefine()

NodePtr EvalCar( const NodePtr node ) {
  return node->mLeft->mLeft ;
} // EvalCar()

NodePtr EvalCdr( const NodePtr node ) {
  return node->mLeft->mRight ;
} // EvalCdr()

NodePtr EvalPair( const NodePtr node ) {
  return new Node( !node->mLeft->IsAtom() && !node->mLeft->IsFunction() ) ;
} // EvalPair()

NodePtr EvalNull( const NodePtr node ) {
  return new Node( node->mLeft->IsNil() ) ;
} // EvalNull()

NodePtr EvalInteger( const NodePtr node ) {
  return new Node( node->mLeft->IsInteger() ) ;
} // EvalInteger()

NodePtr EvalReal( const NodePtr node ) {
  return new Node( node->mLeft->IsFloat() );
} // EvalReal()

NodePtr EvalNumber( const NodePtr node ) {
  return new Node( node->mLeft->IsNumber() );
} // EvalNumber()

NodePtr EvalString( const NodePtr node ) {
  return new Node( node->mLeft->IsString() );
} // EvalString()

NodePtr EvalBoolean( const NodePtr node ) {
  return new Node( node->mLeft->IsBool() );
} // EvalBoolean()

NodePtr EvalSymbol( const NodePtr node ) {
  return new Node( node->mLeft->IsSymbol() );
} // EvalSymbol()

NodePtr EvalAdd( const NodePtr node ) {
  float res = 0 ;
  TokenType ret_type = T_INT;
  NodePtr cur = node ;
  while ( !cur->IsNil() ) {
    float v ;
    NodePtr result = cur->mLeft ;
    if ( result->mData.mType == T_FLOAT ) {
      ret_type = T_FLOAT;
    } // if

    stringstream ss2 ;
    ss2 << result->Symbol() ;
    ss2 >> v ;
    res += v;
    cur = cur->mRight ;
  } // while

  stringstream ss ;
  ss << res ;
  return new Node( Token::Token( ret_type, ss.str() ) );
} // EvalAdd()

NodePtr EvalMinus( const NodePtr node ) {
  float res = 0 ;
  TokenType ret_type = T_INT;
  NodePtr cur = node ;
  while ( !cur->IsNil() ) {
    float v ;
    NodePtr result = cur->mLeft ;
    if ( result->mData.mType == T_FLOAT ) {
      ret_type = T_FLOAT;
    } // if

    stringstream ss2 ;
    ss2 << result->Symbol() ;
    ss2 >> v ;
    if ( cur == node ) {
      res = v;
    } // if
    else {
      res -= v;
    } // else

    cur = cur->mRight ;
  } // while

  stringstream ss ;
  ss << res ;
  return new Node( Token::Token( ret_type, ss.str() ) );
} // EvalMinus()

NodePtr EvalTimes( const NodePtr node ) {
  float res = 1 ;
  TokenType ret_type = T_INT;
  NodePtr cur = node ;
  while ( !cur->IsNil() ) {
    float v ;
    NodePtr result = cur->mLeft ;
    if ( result->mData.mType == T_FLOAT ) {
      ret_type = T_FLOAT;
    } // if

    stringstream ss2 ;
    ss2 << result->Symbol() ;
    ss2 >> v ;
    res *= v;
    cur = cur->mRight ;
  } // while

  stringstream ss ;
  ss << res ;
  return new Node( Token::Token( ret_type, ss.str() ) );
} // EvalTimes()

NodePtr EvalDivide( const NodePtr node ) {
  float res = 0 ;
  TokenType ret_type = T_INT;
  NodePtr cur = node ;
  while ( !cur->IsNil() ) {
    float v ;
    NodePtr result = cur->mLeft ;
    if ( result->mData.mType == T_FLOAT ) {
      ret_type = T_FLOAT;
    } // if

    stringstream ss2 ;
    ss2 << result->Symbol() ;
    ss2 >> v ;
    if ( cur == node ) {
      res = v ;
    } // if
    else if ( ret_type == T_INT ) {
      res = ( int ) res / ( int ) v ;
    } // else if
    else {
      res /= v ;
    } // else

    cur = cur->mRight ;
  } // while


  stringstream ss ;
  ss << res ;
  return new Node( Token::Token( ret_type, ss.str() ) ) ;
} // EvalDivide()

NodePtr EvalNot( const NodePtr node ) {
  return new Node( !node->mLeft->IsTrue() );
} // EvalNot()

NodePtr EvalAnd( const NodePtr node ) {
  return new Node( node->mLeft->IsTrue() && node->mRight->mLeft->IsTrue() );
} // EvalAnd()

NodePtr EvalOr( const NodePtr node ) {
  return new Node( node->mLeft->IsTrue() || node->mRight->mLeft->IsTrue() );
} // EvalOr()

NodePtr EvalBigger( const NodePtr node ) {
  bool ret = true ;
  float num = node->mLeft->ToNumber() ;
  NodePtr cur = node->mRight;
  while ( !cur->IsNil() && ret ) {
    float v = cur->mLeft->ToNumber() ;
    if ( ! ( num > v ) ) {
      ret = false;
    } // if
    else {
      num = v ;
      cur = cur->mRight;
    } // else
  } // while()

  return new Node( ret );
} // EvalBigger()

NodePtr EvalBiggerE( const NodePtr node ) {
  bool ret = true ;
  float num = node->mLeft->ToNumber() ;
  NodePtr cur = node->mRight;
  while ( !cur->IsNil() && ret ) {
    float v = cur->mLeft->ToNumber() ;
    if ( ! ( num >= v ) ) {
      ret = false;
    } // if
    else {
      num = v ;
      cur = cur->mRight;
    } // else
  } // while()

  return new Node( ret );
} // EvalBiggerE()

NodePtr EvalSmaller( const NodePtr node ) {
  bool ret = true ;
  float num = node->mLeft->ToNumber() ;
  NodePtr cur = node->mRight;
  while ( !cur->IsNil() && ret ) {
    float v = cur->mLeft->ToNumber() ;
    if ( ! ( num < v ) ) {
      ret = false;
    } // if
    else {
      num = v ;
      cur = cur->mRight;
    } // else
  } // while()

  return new Node( ret );
} // EvalSmaller()

NodePtr EvalSmallerE( const NodePtr node ) {
  bool ret = true ;
  float num = node->mLeft->ToNumber() ;
  NodePtr cur = node->mRight;
  while ( !cur->IsNil() && ret ) {
    float v = cur->mLeft->ToNumber() ;
    if ( ! ( num <= v ) ) {
      ret = false;
    } // if
    else {
      num = v ;
      cur = cur->mRight;
    } // else
  } // while()

  return new Node( ret );
} // EvalSmallerE()

NodePtr EvalEqual( const NodePtr node ) {
  bool ret = true ;
  float num = node->mLeft->ToNumber() ;
  NodePtr cur = node->mRight;
  while ( !cur->IsNil() && ret ) {
    float v = cur->mLeft->ToNumber() ;
    if ( ! ( num == v ) ) {
      ret = false;
    } // if
    else {
      num = v ;
      cur = cur->mRight;
    } // else
  } // while()

  return new Node( ret );
} // EvalEqual()

NodePtr EvalStringA( const NodePtr node )  {
  string sym = node->mLeft->Symbol() ;
  string ret = sym.substr( 1, sym.size() - 2 ) ;
  NodePtr cur = node->mRight ;
  while ( !cur->IsNil() ) {
    sym = cur->mLeft->Symbol() ;
    ret += sym.substr( 1, sym.size() - 2 ) ;
    cur = cur->mRight ;
  } // while()

  ret = "\"" + ret + "\"" ;
  return new Node( Token::Token( T_STRING, ret ) ) ;
} // EvalStringA()

NodePtr EvalStringB( const NodePtr node ) {
  bool ret = true ;
  string pre = node->mLeft->Symbol() ;

  NodePtr cur = node->mRight ;
  while ( !cur->IsNil() ) {
    if ( ! ( pre > cur->mLeft->Symbol() ) ) {
      ret = false;
    } // if

    pre = cur->mLeft->Symbol() ;
    cur = cur->mRight;
  } // while()

  return new Node( ret );
} // EvalStringB()

NodePtr LookUp( const NodePtr node ) {
  if ( !node->IsSymbol() ) return NULL ;
  // search defined symbols
  for ( int i = ( int ) gDefine.size() - 1 ; i >= 0 ; i-- ) {
    for ( int j = ( int ) gDefine[i].size() - 1 ; j >= 0 ; --j ) {
      if ( gDefine[i][j].name == node->Symbol() ) {
        return gDefine[i][j].ref ;
      } // if
    } // for
  } // for

  return NULL ;
} // LookUp()

NodePtr EvalEqv( const NodePtr node ) {
  NodePtr node1 = node->mLeft;
  NodePtr node2 = node->mRight->mLeft;
  if ( node1->IsSymbol() ) {
    vector<string> symbols ;
    // avoid circular resolving
    symbols.push_back( node1->Symbol() );
    NodePtr cur_node = LookUp( node1 ) ;
    while ( cur_node != NULL ) {
      node1 = cur_node;
      cur_node = LookUp( cur_node ) ;
      if ( find( symbols.begin(), symbols.end(), cur_node->Symbol() ) != symbols.end() ) {
        node1 = cur_node;
        cur_node = NULL ;
      } // if
    } // while
  } // if

  if ( node2->IsSymbol() ) {
    vector<string> symbols ;
    // avoid circular resolving
    symbols.push_back( node1->Symbol() );
    NodePtr cur_node = LookUp( node2 ) ;
    while ( cur_node != NULL ) {
      node2 = cur_node;
      cur_node = LookUp( cur_node ) ;
      if ( find( symbols.begin(), symbols.end(), cur_node->Symbol() ) != symbols.end() ) {
        node1 = cur_node;
        cur_node = NULL ;
      } // if
    } // while
  } // if

  if ( node1 == node2 ) {
    return new Node( true ) ;
  } // if

  if ( node1->IsString() || node2->IsString() ) {
    return new Node( false ) ;
  } // if

  if ( node1->IsAtom() && node2->IsAtom() ) {
    return new Node( node1->mData.IsEql( node2->mData ) );
  } // if

  return new Node( false );
} // EvalEqv()

bool IsTheSameTree( NodePtr a, NodePtr b ) {
  if ( a == NULL && b == NULL ) return true;
  if ( a != NULL && b == NULL ) return false;
  if ( a == NULL && b != NULL ) return false;
  if ( a->IsAtom() != b->IsAtom() ) return false ;

  if ( a->IsAtom() && b->IsAtom() ) {
    return ( a->mData.IsEql( b->mData ) );
  } // if

  return IsTheSameTree( a->mLeft, b->mLeft ) && IsTheSameTree( a->mRight, b->mRight );
} // IsTheSameTree()

NodePtr EvalEqualQ( const NodePtr node ) {
  return new Node( IsTheSameTree( node->mLeft, node->mRight->mLeft ) ) ;
} // EvalEqualQ()

NodePtr EvalBegin( const NodePtr node ) {
  NodePtr ret = NULL;
  NodePtr cur = node;
  while ( !cur->IsNil() ) {
    ret = cur->mLeft ;
    cur = cur->mRight;
  } // while()

  return ret;
} // EvalBegin()

NodePtr EvalIf( const NodePtr node ) {
  return Eval( node->mLeft )->IsTrue() ? Eval( node->mRight->mLeft ) : Eval( node->mRight->mRight->mLeft ) ;
} // EvalIf()

NodePtr EvalCond( const NodePtr node ) {
  NodePtr cur = node;
  while ( !cur->IsNil() ) {
    if ( Eval( cur->mLeft->mLeft )->IsTrue() ) {
      cur = cur->mLeft->mRight ;
      while ( 1 ) { //
        if ( cur->mRight->IsNil() )
          return Eval( cur->mLeft ) ;
        else Eval( cur->mLeft ) ;
        cur = cur->mRight;
      } // while()
    } // if

    cur = cur->mRight;
  } // while()

  return NULL;
} // EvalCond()

NodePtr EvalExit( const NodePtr node )  {
  gEndOfAll = true ;
  return new Node( Token::Token( T_EMPTY ) );
} // EvalExit()

NodePtr EvalClean( const NodePtr node ) {
  gDefine.clear() ;
  gDefine.resize( 1 ) ;
  cout << "environment cleaned" << endl ;
  return new Node( Token::Token( T_EMPTY ) ) ;
} // EvalClean()

vector<NodeDef> GetDefines( const NodePtr node ) {
  vector<NodeDef> ret;
  if ( node->IsNil() ) return ret ;

  NodePtr cur = node ;

  while ( !cur->IsNil() ) {
    NodeDef def;
    def.name = cur->mLeft->mLeft->Symbol() ;
    def.ref  = cur->mLeft->mRight->mLeft ;
    ret.push_back( def );
    cur = cur->mRight ;
  } // while

  return ret;
} // GetDefines()

NodePtr EvalLet( const NodePtr node )  {
  vector<NodeDef> defs = GetDefines( node->mLeft ) ;
  for ( size_t i = 0 ; i < defs.size() ; ++i ) {
    defs[i].ref = Eval( defs[i].ref ) ;
  } // for

  gDefine.push_back( defs ) ;
  NodePtr ret = NULL ;
  NodePtr cur = node->mRight;
  while ( !cur->IsNil() ) {
    ret = Eval( cur->mLeft ) ;
    cur = cur->mRight;
  } // while()

  gDefine.pop_back() ;
  return ret ;
} // EvalLet()

NodePtr EvalLambda( const NodePtr node ) {
  NodePtr ret = CloneTree( node ) ;
  ret->SetFunction() ;
  return ret ;
} // EvalLambda()


NodePtr EvalSet( const NodePtr node ) {
  NodePtr nodeForDefine = new Node ;
  NodeDef nodeDef ;
  nodeDef.name = node->mLeft->Symbol() ;
  nodeDef.ref  = Eval( node->mRight->mLeft ) ;

  nodeForDefine->mData = Token::Token( T_STRING, nodeDef.name + " defined" ) ;
  int i;
  bool end = false ;
  for ( i = 0 ; i < ( int ) gDefine[0].size() && !end ; ++i ) {
    if ( gDefine[0][i].name == nodeDef.name ) {
      gDefine[0][i] = nodeDef ;
      end = true ;
      i-- ;
    } // if
  } // for

  if ( i == gDefine[0].size() ) {
    gDefine[0].push_back( nodeDef ) ;
  } // if

  return Node::Nil() ;
} // EvalSet()


Token GetToken()  {
  Token ret ;
  bool first = true ;
  char p = cin.peek() ;
  bool commend = false ;

  while ( p == ' ' || p == '\n' || p == '\t' ) {
    ReadCountPost() ;
    p = cin.peek() ;
  } // while()

  if ( p == ';' ) {
    commend = true ;
    while ( commend ) {
      while ( p != '\n' && p != EOF ) p = ReadCountPost() ;
      if ( p == '\n' ) {
        p = cin.peek() ;
        while ( p == ' ' || p == '\n' || p == '\t' ) {
          ReadCountPost() ;
          p = cin.peek() ;
        } // while()
      } // if

      if ( p != ';' ) commend = false ;
    } // while
  } // if

  if ( p == EOF ) {
    ReadCountPost() ;
    throw string( "ERROR : END-OF-FILE encountered when there should be more input" );
  } // if
  else if ( p == '(' ) {
    ret = Token::Token( T_LEFT_PAREN, "(" ) ;
    ReadCountPost() ;
    ret.mColumn = gColumn, ret.mLine = gLine ;
  } // else if
  else if ( p == ')' ) {
    ret = Token::Token( T_RIGHT_PAREN, ")" ) ;
    ReadCountPost() ;
    ret.mColumn = gColumn, ret.mLine = gLine ;
  } // else if
  else if ( p == '\'' ) {
    ret = Token::Token( T_QUOTE, "'" ) ;
    ReadCountPost() ;
    ret.mColumn = gColumn, ret.mLine = gLine ;
  } // else if
  else if ( p == '"' ) {
    ret.mType = T_STRING ;
    ret.mColumn = gColumn + 1;
    ret.mLine   = gLine;
    do {
      ret.mSymbol += ReadCountPost() ;
      p = cin.peek();
    } while ( p != '"' && p != '\n' && p != EOF )  ;
    if ( p == EOF )
      throw string( "ERROR : END-OF-FILE encountered when there should be more input" );
    else if ( p == '\n' ) {
      throw UnexpectedCharErrorString();
    } // else if
    else {
      ret.mSymbol += ReadCountPost() ;
    } // else
  }  // else if

  else {
    // INT, mSymbol, FLOAT, NIL, T

    while ( p != '\t' && p != ' ' && p != '\n' && p != '(' && p != ')' &&
            p != '"' && p != '\'' && p != EOF ) {
      p = ReadCountPost() ;
      ret.mSymbol += p ;
      if ( first ) ret.mColumn = gColumn, ret.mLine = gLine, first = false ;
      p = cin.peek() ;
    } // while()

    if ( IsFloatString( ret.mSymbol ) ) ret.mType = T_FLOAT ;
    else if ( IsDigitString( ret.mSymbol ) ) ret.mType = T_INT ;
    else if ( ret.mSymbol == "t" || ret.mSymbol == "#t"  ) {
      ret = Token::T() ;
    }
    else if ( ret.mSymbol == "nil" || ret.mSymbol == "#f" ) {
      ret = Token::Nil() ;
    }
    else if ( ret.mSymbol == "." ) ret.mType = T_DOT;
    else  ret.mType = T_SYMBOL ;
    // 判斷mType
  } // else

  return ret ;
} // GetToken()

string NumberToString( int num ) {
  stringstream ss ;
  ss << num ;
  return ss.str() ;
} // NumberToString()

char ReadCountPost() {
  char p ;
  p = cin.get() ;
  if ( p == '\n' ) gColumn = 0, gLine++ ;
  else gColumn++ ;
  return p ;
} // ReadCountPost()

bool IsFloatString( string mSymbol ) {
  bool hasanydigit = false;

  int i = 0 ;
  if ( mSymbol[i] == '+' || mSymbol[i] == '-' ) ++i ;

  if ( mSymbol[i] == '\0' ) return false ;

  if ( mSymbol[i] == '0' && mSymbol[i+1] != '.' ) return false ;

  while ( mSymbol[i] != '.' && mSymbol[i] != '\0' ) {
    if ( !isdigit( mSymbol[i] ) ) return false ;
    hasanydigit = true ;
    i++ ;
  } // while

  if ( mSymbol[i] != '.' ) return false ;
  i++ ;

  while ( mSymbol[i] != '\0' ) {
    if ( !isdigit( mSymbol[i] ) ) return false ;
    hasanydigit = true ;
    i++ ;
  } // while

  return hasanydigit;

} // IsFloatString()

bool IsDigitString( string mSymbol ) {
  // sign
  int i = 0 ;
  if ( mSymbol[0] == '+' || mSymbol[0] == '-' ) i++ ;

  if ( mSymbol[i] == '\0' ) return false ;

  if ( mSymbol[i] == '0' ) return mSymbol[i+1] == '\0' ;

  while ( i < ( int ) mSymbol.size() ) {
    if ( !isdigit( mSymbol[i] ) ) {
      return false;
    } // if
    i++;
  } // while
  return true ;
} // IsDigitString()


string GetResult( NodePtr node, bool walkLeft, int mLevel, bool firstAtom ) {
  string ret;
  int nextLevel = mLevel ;
  if ( node == NULL ) return ret ;
  if ( node->IsAtom() && node->mData.mType == T_EMPTY ) return ret ;
  if ( node->IsAtom() || node->IsFunction() ) {
    if ( ( walkLeft || !node->IsNil() ) && node->mData.mType != T_EMPTY ) {
      if ( !firstAtom )
        for ( int i = 0 ; i < mLevel ; i++ )
          ret += "  " ;
      if ( node->IsQuote() && mLevel > 0 ) {
        ret += "quote";
      } // else if
      else if ( node->IsFloat() ) {
        stringstream s;
        s << setprecision( 3 ) << fixed << node->ToNumber() ;
        ret += s.str();
      } // else if
      else if ( node->IsInteger() ) {
        stringstream s;
        s  << ( int ) node->ToNumber() ;
        ret += s.str();
      } // else if
      else if ( node->IsFunction() ) {
        ret += "#function";
      } // else if
      else {
        ret += node->Symbol();
      } // else
    } // if
  } // if
  else {
    if ( walkLeft ) {
      if ( !firstAtom )
        for ( int i = 0 ; i < mLevel ; i++  )
          ret +=  "  " ;
      ret += "( " ; // need bool for ')'
      nextLevel++;
    } // if

    ret += GetResult( node->mLeft, true, nextLevel, walkLeft ) + "\n";
    if ( !node->mRight->IsNil() && (node->mRight->IsAtom() || node->mRight->IsFunction() ) ) {
      for ( int i = 0 ; i < nextLevel ; i++  )
        ret += "  " ;
      ret += ".";
      ret += "\n";
      ret += GetResult( node->mRight, false, nextLevel, false ) + "\n";
    } // if
    else {
      ret += GetResult( node->mRight, false, nextLevel, false );
    } // else

    if ( walkLeft ) {
      for ( int i = 0 ; i < mLevel ; i++ )
        ret += "  " ;
      ret += ")";
    } // if
  } // else

  return ret;
} // GetResult()

void PrintResult( NodePtr node ) {
  string ret = GetResult( node, true, 0, true ) ;
  if ( ret.size() ) cout << ret << endl ;
} // PrintResult()


NodePtr CloneTree( NodePtr node ) {
  NodePtr ret = new Node ;
  ret->mData = node->mData ;
  if ( !node->IsAtom() ) {
    ret->mLeft = CloneTree( node->mLeft ) ;
    ret->mRight = CloneTree( node->mRight ) ;
  } // if

  return ret ;
} // CloneTree()

void ClearToLineEnd() {
  gLine = 1, gColumn = 0 ;
  char p = cin.peek() ;
  bool end = false ;
  while ( ( p == ' ' || p == ';' ) && !end ) {
    if ( p == ';' ) {
      p = cin.peek();
      while ( p != '\n' && p != EOF ) {
        cin.get();
        p = cin.peek();
      } // while

      end = true ;
    } // if

    if ( !end ) {
      ReadCountPost() ;
      p = cin.peek() ;
    } // if
  } // while

  if ( p == '\n' ) {
    ReadCountPost() ;
    gLine = 1 ;
    gColumn = 0 ;
  } // if
} // ClearToLineEnd()

void ReadToLineEnd() {
  char p = cin.get();
  while ( p != '\n' && p != EOF ) {
    p = cin.get();
  } // while
  // cin.unget();
  gLine = 1 ;
  gColumn = 0 ;
} // ReadToLineEnd();

NodePtr EvalShowTree( const NodePtr node )  {
  ShowTree( node->mLeft ) ;
  cout << endl ;
  return NULL ;
} // EvalShowTree()
