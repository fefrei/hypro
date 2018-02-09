
// Generated from Formula.g4 by ANTLR 4.7



#include "FormulaParser.h"


	#include <map>
	#include <string>


using namespace antlrcpp;
using namespace antlr4;

FormulaParser::FormulaParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

FormulaParser::~FormulaParser() {
  delete _interpreter;
}

std::string FormulaParser::getGrammarFileName() const {
  return "Formula.g4";
}

const std::vector<std::string>& FormulaParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& FormulaParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- ReplacedexprContext ------------------------------------------------------------------

FormulaParser::ReplacedexprContext::ReplacedexprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> FormulaParser::ReplacedexprContext::NUMBER() {
  return getTokens(FormulaParser::NUMBER);
}

tree::TerminalNode* FormulaParser::ReplacedexprContext::NUMBER(size_t i) {
  return getToken(FormulaParser::NUMBER, i);
}

tree::TerminalNode* FormulaParser::ReplacedexprContext::EQUALS() {
  return getToken(FormulaParser::EQUALS, 0);
}

std::vector<tree::TerminalNode *> FormulaParser::ReplacedexprContext::MINUS() {
  return getTokens(FormulaParser::MINUS);
}

tree::TerminalNode* FormulaParser::ReplacedexprContext::MINUS(size_t i) {
  return getToken(FormulaParser::MINUS, i);
}


size_t FormulaParser::ReplacedexprContext::getRuleIndex() const {
  return FormulaParser::RuleReplacedexpr;
}


FormulaParser::ReplacedexprContext* FormulaParser::replacedexpr() {
  ReplacedexprContext *_localctx = _tracker.createInstance<ReplacedexprContext>(_ctx, getState());
  enterRule(_localctx, 0, FormulaParser::RuleReplacedexpr);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(21);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == FormulaParser::MINUS) {
      setState(20);
      match(FormulaParser::MINUS);
    }
    setState(23);
    match(FormulaParser::NUMBER);
    setState(24);
    match(FormulaParser::EQUALS);
    setState(26);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == FormulaParser::MINUS) {
      setState(25);
      match(FormulaParser::MINUS);
    }
    setState(28);
    match(FormulaParser::NUMBER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstantexprContext ------------------------------------------------------------------

FormulaParser::ConstantexprContext::ConstantexprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* FormulaParser::ConstantexprContext::CONSTANT() {
  return getToken(FormulaParser::CONSTANT, 0);
}

tree::TerminalNode* FormulaParser::ConstantexprContext::EQUALS() {
  return getToken(FormulaParser::EQUALS, 0);
}

tree::TerminalNode* FormulaParser::ConstantexprContext::NUMBER() {
  return getToken(FormulaParser::NUMBER, 0);
}

tree::TerminalNode* FormulaParser::ConstantexprContext::MINUS() {
  return getToken(FormulaParser::MINUS, 0);
}


size_t FormulaParser::ConstantexprContext::getRuleIndex() const {
  return FormulaParser::RuleConstantexpr;
}


FormulaParser::ConstantexprContext* FormulaParser::constantexpr() {
  ConstantexprContext *_localctx = _tracker.createInstance<ConstantexprContext>(_ctx, getState());
  enterRule(_localctx, 2, FormulaParser::RuleConstantexpr);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(30);
    dynamic_cast<ConstantexprContext *>(_localctx)->constantToken = match(FormulaParser::CONSTANT);
    setState(31);
    match(FormulaParser::EQUALS);
    setState(33);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == FormulaParser::MINUS) {
      setState(32);
      dynamic_cast<ConstantexprContext *>(_localctx)->minusToken = match(FormulaParser::MINUS);
    }
    setState(35);
    dynamic_cast<ConstantexprContext *>(_localctx)->numberToken = match(FormulaParser::NUMBER);

    	std::cout << "In constantexpr! MINUS text is: " << (dynamic_cast<ConstantexprContext *>(_localctx)->minusToken != nullptr ? dynamic_cast<ConstantexprContext *>(_localctx)->minusToken->getText() : "") << std::endl;
    	if((dynamic_cast<ConstantexprContext *>(_localctx)->minusToken != nullptr ? dynamic_cast<ConstantexprContext *>(_localctx)->minusToken->getText() : "") != ""){
    		constants.insert({(dynamic_cast<ConstantexprContext *>(_localctx)->constantToken != nullptr ? dynamic_cast<ConstantexprContext *>(_localctx)->constantToken->getText() : ""), (dynamic_cast<ConstantexprContext *>(_localctx)->minusToken != nullptr ? dynamic_cast<ConstantexprContext *>(_localctx)->minusToken->getText() : "").append((dynamic_cast<ConstantexprContext *>(_localctx)->numberToken != nullptr ? dynamic_cast<ConstantexprContext *>(_localctx)->numberToken->getText() : ""))});
    		std::cout << "Constant " << (dynamic_cast<ConstantexprContext *>(_localctx)->constantToken != nullptr ? dynamic_cast<ConstantexprContext *>(_localctx)->constantToken->getText() : "") << " with value " << (dynamic_cast<ConstantexprContext *>(_localctx)->minusToken != nullptr ? dynamic_cast<ConstantexprContext *>(_localctx)->minusToken->getText() : "").append((dynamic_cast<ConstantexprContext *>(_localctx)->numberToken != nullptr ? dynamic_cast<ConstantexprContext *>(_localctx)->numberToken->getText() : "")) << " was put in map!\n";
    	} else {
    		constants.insert({(dynamic_cast<ConstantexprContext *>(_localctx)->constantToken != nullptr ? dynamic_cast<ConstantexprContext *>(_localctx)->constantToken->getText() : ""), (dynamic_cast<ConstantexprContext *>(_localctx)->numberToken != nullptr ? dynamic_cast<ConstantexprContext *>(_localctx)->numberToken->getText() : "")});
    		std::cout << "Constant " << (dynamic_cast<ConstantexprContext *>(_localctx)->constantToken != nullptr ? dynamic_cast<ConstantexprContext *>(_localctx)->constantToken->getText() : "") << " with value " << (dynamic_cast<ConstantexprContext *>(_localctx)->numberToken != nullptr ? dynamic_cast<ConstantexprContext *>(_localctx)->numberToken->getText() : "") << " was put in map!\n";
    	}

   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConnectorContext ------------------------------------------------------------------

FormulaParser::ConnectorContext::ConnectorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* FormulaParser::ConnectorContext::PLUS() {
  return getToken(FormulaParser::PLUS, 0);
}

tree::TerminalNode* FormulaParser::ConnectorContext::MINUS() {
  return getToken(FormulaParser::MINUS, 0);
}


size_t FormulaParser::ConnectorContext::getRuleIndex() const {
  return FormulaParser::RuleConnector;
}


FormulaParser::ConnectorContext* FormulaParser::connector() {
  ConnectorContext *_localctx = _tracker.createInstance<ConnectorContext>(_ctx, getState());
  enterRule(_localctx, 4, FormulaParser::RuleConnector);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(38);
    _la = _input->LA(1);
    if (!(_la == FormulaParser::PLUS

    || _la == FormulaParser::MINUS)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TermContext ------------------------------------------------------------------

FormulaParser::TermContext::TermContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> FormulaParser::TermContext::NUMBER() {
  return getTokens(FormulaParser::NUMBER);
}

tree::TerminalNode* FormulaParser::TermContext::NUMBER(size_t i) {
  return getToken(FormulaParser::NUMBER, i);
}

std::vector<tree::TerminalNode *> FormulaParser::TermContext::VARIABLE() {
  return getTokens(FormulaParser::VARIABLE);
}

tree::TerminalNode* FormulaParser::TermContext::VARIABLE(size_t i) {
  return getToken(FormulaParser::VARIABLE, i);
}

std::vector<tree::TerminalNode *> FormulaParser::TermContext::TIMES() {
  return getTokens(FormulaParser::TIMES);
}

tree::TerminalNode* FormulaParser::TermContext::TIMES(size_t i) {
  return getToken(FormulaParser::TIMES, i);
}

std::vector<FormulaParser::ConnectorContext *> FormulaParser::TermContext::connector() {
  return getRuleContexts<FormulaParser::ConnectorContext>();
}

FormulaParser::ConnectorContext* FormulaParser::TermContext::connector(size_t i) {
  return getRuleContext<FormulaParser::ConnectorContext>(i);
}


size_t FormulaParser::TermContext::getRuleIndex() const {
  return FormulaParser::RuleTerm;
}


FormulaParser::TermContext* FormulaParser::term() {
  TermContext *_localctx = _tracker.createInstance<TermContext>(_ctx, getState());
  enterRule(_localctx, 6, FormulaParser::RuleTerm);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(40);
    _la = _input->LA(1);
    if (!(_la == FormulaParser::NUMBER

    || _la == FormulaParser::VARIABLE)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(51);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == FormulaParser::TIMES) {
      setState(41);
      match(FormulaParser::TIMES);
      setState(45);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == FormulaParser::PLUS

      || _la == FormulaParser::MINUS) {
        setState(42);
        connector();
        setState(47);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(48);
      _la = _input->LA(1);
      if (!(_la == FormulaParser::NUMBER

      || _la == FormulaParser::VARIABLE)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(53);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PolynomContext ------------------------------------------------------------------

FormulaParser::PolynomContext::PolynomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<FormulaParser::TermContext *> FormulaParser::PolynomContext::term() {
  return getRuleContexts<FormulaParser::TermContext>();
}

FormulaParser::TermContext* FormulaParser::PolynomContext::term(size_t i) {
  return getRuleContext<FormulaParser::TermContext>(i);
}

std::vector<FormulaParser::ConnectorContext *> FormulaParser::PolynomContext::connector() {
  return getRuleContexts<FormulaParser::ConnectorContext>();
}

FormulaParser::ConnectorContext* FormulaParser::PolynomContext::connector(size_t i) {
  return getRuleContext<FormulaParser::ConnectorContext>(i);
}


size_t FormulaParser::PolynomContext::getRuleIndex() const {
  return FormulaParser::RulePolynom;
}


FormulaParser::PolynomContext* FormulaParser::polynom() {
  PolynomContext *_localctx = _tracker.createInstance<PolynomContext>(_ctx, getState());
  enterRule(_localctx, 8, FormulaParser::RulePolynom);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(57);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == FormulaParser::PLUS

    || _la == FormulaParser::MINUS) {
      setState(54);
      connector();
      setState(59);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(60);
    term();
    setState(70);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(62); 
        _errHandler->sync(this);
        _la = _input->LA(1);
        do {
          setState(61);
          connector();
          setState(64); 
          _errHandler->sync(this);
          _la = _input->LA(1);
        } while (_la == FormulaParser::PLUS

        || _la == FormulaParser::MINUS);
        setState(66);
        term(); 
      }
      setState(72);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IntervalContext ------------------------------------------------------------------

FormulaParser::IntervalContext::IntervalContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> FormulaParser::IntervalContext::NUMBER() {
  return getTokens(FormulaParser::NUMBER);
}

tree::TerminalNode* FormulaParser::IntervalContext::NUMBER(size_t i) {
  return getToken(FormulaParser::NUMBER, i);
}

std::vector<tree::TerminalNode *> FormulaParser::IntervalContext::MINUS() {
  return getTokens(FormulaParser::MINUS);
}

tree::TerminalNode* FormulaParser::IntervalContext::MINUS(size_t i) {
  return getToken(FormulaParser::MINUS, i);
}


size_t FormulaParser::IntervalContext::getRuleIndex() const {
  return FormulaParser::RuleInterval;
}


FormulaParser::IntervalContext* FormulaParser::interval() {
  IntervalContext *_localctx = _tracker.createInstance<IntervalContext>(_ctx, getState());
  enterRule(_localctx, 10, FormulaParser::RuleInterval);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(73);
    match(FormulaParser::SBOPEN);
    setState(75);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == FormulaParser::MINUS) {
      setState(74);
      match(FormulaParser::MINUS);
    }
    setState(77);
    match(FormulaParser::NUMBER);
    setState(78);
    match(FormulaParser::COMMA);
    setState(80);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == FormulaParser::MINUS) {
      setState(79);
      match(FormulaParser::MINUS);
    }
    setState(82);
    match(FormulaParser::NUMBER);
    setState(83);
    match(FormulaParser::SBCLOSE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EquationContext ------------------------------------------------------------------

FormulaParser::EquationContext::EquationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* FormulaParser::EquationContext::VARIABLE() {
  return getToken(FormulaParser::VARIABLE, 0);
}

tree::TerminalNode* FormulaParser::EquationContext::EQUALS() {
  return getToken(FormulaParser::EQUALS, 0);
}

FormulaParser::PolynomContext* FormulaParser::EquationContext::polynom() {
  return getRuleContext<FormulaParser::PolynomContext>(0);
}

FormulaParser::ConnectorContext* FormulaParser::EquationContext::connector() {
  return getRuleContext<FormulaParser::ConnectorContext>(0);
}

FormulaParser::IntervalContext* FormulaParser::EquationContext::interval() {
  return getRuleContext<FormulaParser::IntervalContext>(0);
}


size_t FormulaParser::EquationContext::getRuleIndex() const {
  return FormulaParser::RuleEquation;
}


FormulaParser::EquationContext* FormulaParser::equation() {
  EquationContext *_localctx = _tracker.createInstance<EquationContext>(_ctx, getState());
  enterRule(_localctx, 12, FormulaParser::RuleEquation);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(85);
    match(FormulaParser::VARIABLE);
    setState(86);
    match(FormulaParser::EQUALS);
    setState(87);
    polynom();
    setState(91);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == FormulaParser::PLUS

    || _la == FormulaParser::MINUS) {
      setState(88);
      connector();
      setState(89);
      interval();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstraintContext ------------------------------------------------------------------

FormulaParser::ConstraintContext::ConstraintContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<FormulaParser::PolynomContext *> FormulaParser::ConstraintContext::polynom() {
  return getRuleContexts<FormulaParser::PolynomContext>();
}

FormulaParser::PolynomContext* FormulaParser::ConstraintContext::polynom(size_t i) {
  return getRuleContext<FormulaParser::PolynomContext>(i);
}

tree::TerminalNode* FormulaParser::ConstraintContext::BOOLRELATION() {
  return getToken(FormulaParser::BOOLRELATION, 0);
}

tree::TerminalNode* FormulaParser::ConstraintContext::EQUALS() {
  return getToken(FormulaParser::EQUALS, 0);
}


size_t FormulaParser::ConstraintContext::getRuleIndex() const {
  return FormulaParser::RuleConstraint;
}


FormulaParser::ConstraintContext* FormulaParser::constraint() {
  ConstraintContext *_localctx = _tracker.createInstance<ConstraintContext>(_ctx, getState());
  enterRule(_localctx, 14, FormulaParser::RuleConstraint);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(93);
    polynom();
    setState(94);
    _la = _input->LA(1);
    if (!(_la == FormulaParser::EQUALS

    || _la == FormulaParser::BOOLRELATION)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(95);
    polynom();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IntervalexprContext ------------------------------------------------------------------

FormulaParser::IntervalexprContext::IntervalexprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* FormulaParser::IntervalexprContext::VARIABLE() {
  return getToken(FormulaParser::VARIABLE, 0);
}

tree::TerminalNode* FormulaParser::IntervalexprContext::IN() {
  return getToken(FormulaParser::IN, 0);
}

FormulaParser::IntervalContext* FormulaParser::IntervalexprContext::interval() {
  return getRuleContext<FormulaParser::IntervalContext>(0);
}


size_t FormulaParser::IntervalexprContext::getRuleIndex() const {
  return FormulaParser::RuleIntervalexpr;
}


FormulaParser::IntervalexprContext* FormulaParser::intervalexpr() {
  IntervalexprContext *_localctx = _tracker.createInstance<IntervalexprContext>(_ctx, getState());
  enterRule(_localctx, 16, FormulaParser::RuleIntervalexpr);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(97);
    match(FormulaParser::VARIABLE);
    setState(98);
    match(FormulaParser::IN);
    setState(99);
    interval();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstrsetContext ------------------------------------------------------------------

FormulaParser::ConstrsetContext::ConstrsetContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<FormulaParser::ConstraintContext *> FormulaParser::ConstrsetContext::constraint() {
  return getRuleContexts<FormulaParser::ConstraintContext>();
}

FormulaParser::ConstraintContext* FormulaParser::ConstrsetContext::constraint(size_t i) {
  return getRuleContext<FormulaParser::ConstraintContext>(i);
}

std::vector<FormulaParser::IntervalexprContext *> FormulaParser::ConstrsetContext::intervalexpr() {
  return getRuleContexts<FormulaParser::IntervalexprContext>();
}

FormulaParser::IntervalexprContext* FormulaParser::ConstrsetContext::intervalexpr(size_t i) {
  return getRuleContext<FormulaParser::IntervalexprContext>(i);
}


size_t FormulaParser::ConstrsetContext::getRuleIndex() const {
  return FormulaParser::RuleConstrset;
}


FormulaParser::ConstrsetContext* FormulaParser::constrset() {
  ConstrsetContext *_localctx = _tracker.createInstance<ConstrsetContext>(_ctx, getState());
  enterRule(_localctx, 18, FormulaParser::RuleConstrset);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(103); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(103);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx)) {
      case 1: {
        setState(101);
        constraint();
        break;
      }

      case 2: {
        setState(102);
        intervalexpr();
        break;
      }

      }
      setState(105); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << FormulaParser::PLUS)
      | (1ULL << FormulaParser::MINUS)
      | (1ULL << FormulaParser::NUMBER)
      | (1ULL << FormulaParser::VARIABLE))) != 0));
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

// Static vars and initialization.
std::vector<dfa::DFA> FormulaParser::_decisionToDFA;
atn::PredictionContextCache FormulaParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN FormulaParser::_atn;
std::vector<uint16_t> FormulaParser::_serializedATN;

std::vector<std::string> FormulaParser::_ruleNames = {
  "replacedexpr", "constantexpr", "connector", "term", "polynom", "interval", 
  "equation", "constraint", "intervalexpr", "constrset"
};

std::vector<std::string> FormulaParser::_literalNames = {
  "", "'in'", "'par'", "'jumps'", "'urgent'", "'guard'", "'reset'", "'parallelotope aggregation'", 
  "'box aggregation'", "'->'", "':='", "", "'='", "", "'+'", "'-'", "'*'", 
  "'['", "']'", "'{'", "'}'", "','"
};

std::vector<std::string> FormulaParser::_symbolicNames = {
  "", "IN", "PAR", "JUMPS", "URGENT", "GUARD", "RESET", "PARALLELOTOPE", 
  "BOX", "JUMP", "DEFINE", "COMMENT", "EQUALS", "BOOLRELATION", "PLUS", 
  "MINUS", "TIMES", "SBOPEN", "SBCLOSE", "CBOPEN", "CBCLOSE", "COMMA", "NUMBER", 
  "CONSTANT", "VARIABLE", "WS"
};

dfa::Vocabulary FormulaParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> FormulaParser::_tokenNames;

FormulaParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0x1b, 0x6e, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 0x9, 
    0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 0x4, 
    0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 0xb, 0x9, 
    0xb, 0x3, 0x2, 0x5, 0x2, 0x18, 0xa, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 
    0x5, 0x2, 0x1d, 0xa, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x5, 0x3, 0x24, 0xa, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x7, 0x5, 0x2e, 0xa, 0x5, 0xc, 
    0x5, 0xe, 0x5, 0x31, 0xb, 0x5, 0x3, 0x5, 0x7, 0x5, 0x34, 0xa, 0x5, 0xc, 
    0x5, 0xe, 0x5, 0x37, 0xb, 0x5, 0x3, 0x6, 0x7, 0x6, 0x3a, 0xa, 0x6, 0xc, 
    0x6, 0xe, 0x6, 0x3d, 0xb, 0x6, 0x3, 0x6, 0x3, 0x6, 0x6, 0x6, 0x41, 0xa, 
    0x6, 0xd, 0x6, 0xe, 0x6, 0x42, 0x3, 0x6, 0x3, 0x6, 0x7, 0x6, 0x47, 0xa, 
    0x6, 0xc, 0x6, 0xe, 0x6, 0x4a, 0xb, 0x6, 0x3, 0x7, 0x3, 0x7, 0x5, 0x7, 
    0x4e, 0xa, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x5, 0x7, 0x53, 0xa, 0x7, 
    0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 
    0x3, 0x8, 0x3, 0x8, 0x5, 0x8, 0x5e, 0xa, 0x8, 0x3, 0x9, 0x3, 0x9, 0x3, 
    0x9, 0x3, 0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xb, 0x3, 
    0xb, 0x6, 0xb, 0x6a, 0xa, 0xb, 0xd, 0xb, 0xe, 0xb, 0x6b, 0x3, 0xb, 0x2, 
    0x2, 0xc, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 0x2, 
    0x5, 0x3, 0x2, 0x10, 0x11, 0x4, 0x2, 0x18, 0x18, 0x1a, 0x1a, 0x3, 0x2, 
    0xe, 0xf, 0x2, 0x70, 0x2, 0x17, 0x3, 0x2, 0x2, 0x2, 0x4, 0x20, 0x3, 
    0x2, 0x2, 0x2, 0x6, 0x28, 0x3, 0x2, 0x2, 0x2, 0x8, 0x2a, 0x3, 0x2, 0x2, 
    0x2, 0xa, 0x3b, 0x3, 0x2, 0x2, 0x2, 0xc, 0x4b, 0x3, 0x2, 0x2, 0x2, 0xe, 
    0x57, 0x3, 0x2, 0x2, 0x2, 0x10, 0x5f, 0x3, 0x2, 0x2, 0x2, 0x12, 0x63, 
    0x3, 0x2, 0x2, 0x2, 0x14, 0x69, 0x3, 0x2, 0x2, 0x2, 0x16, 0x18, 0x7, 
    0x11, 0x2, 0x2, 0x17, 0x16, 0x3, 0x2, 0x2, 0x2, 0x17, 0x18, 0x3, 0x2, 
    0x2, 0x2, 0x18, 0x19, 0x3, 0x2, 0x2, 0x2, 0x19, 0x1a, 0x7, 0x18, 0x2, 
    0x2, 0x1a, 0x1c, 0x7, 0xe, 0x2, 0x2, 0x1b, 0x1d, 0x7, 0x11, 0x2, 0x2, 
    0x1c, 0x1b, 0x3, 0x2, 0x2, 0x2, 0x1c, 0x1d, 0x3, 0x2, 0x2, 0x2, 0x1d, 
    0x1e, 0x3, 0x2, 0x2, 0x2, 0x1e, 0x1f, 0x7, 0x18, 0x2, 0x2, 0x1f, 0x3, 
    0x3, 0x2, 0x2, 0x2, 0x20, 0x21, 0x7, 0x19, 0x2, 0x2, 0x21, 0x23, 0x7, 
    0xe, 0x2, 0x2, 0x22, 0x24, 0x7, 0x11, 0x2, 0x2, 0x23, 0x22, 0x3, 0x2, 
    0x2, 0x2, 0x23, 0x24, 0x3, 0x2, 0x2, 0x2, 0x24, 0x25, 0x3, 0x2, 0x2, 
    0x2, 0x25, 0x26, 0x7, 0x18, 0x2, 0x2, 0x26, 0x27, 0x8, 0x3, 0x1, 0x2, 
    0x27, 0x5, 0x3, 0x2, 0x2, 0x2, 0x28, 0x29, 0x9, 0x2, 0x2, 0x2, 0x29, 
    0x7, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x35, 0x9, 0x3, 0x2, 0x2, 0x2b, 0x2f, 
    0x7, 0x12, 0x2, 0x2, 0x2c, 0x2e, 0x5, 0x6, 0x4, 0x2, 0x2d, 0x2c, 0x3, 
    0x2, 0x2, 0x2, 0x2e, 0x31, 0x3, 0x2, 0x2, 0x2, 0x2f, 0x2d, 0x3, 0x2, 
    0x2, 0x2, 0x2f, 0x30, 0x3, 0x2, 0x2, 0x2, 0x30, 0x32, 0x3, 0x2, 0x2, 
    0x2, 0x31, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x32, 0x34, 0x9, 0x3, 0x2, 0x2, 
    0x33, 0x2b, 0x3, 0x2, 0x2, 0x2, 0x34, 0x37, 0x3, 0x2, 0x2, 0x2, 0x35, 
    0x33, 0x3, 0x2, 0x2, 0x2, 0x35, 0x36, 0x3, 0x2, 0x2, 0x2, 0x36, 0x9, 
    0x3, 0x2, 0x2, 0x2, 0x37, 0x35, 0x3, 0x2, 0x2, 0x2, 0x38, 0x3a, 0x5, 
    0x6, 0x4, 0x2, 0x39, 0x38, 0x3, 0x2, 0x2, 0x2, 0x3a, 0x3d, 0x3, 0x2, 
    0x2, 0x2, 0x3b, 0x39, 0x3, 0x2, 0x2, 0x2, 0x3b, 0x3c, 0x3, 0x2, 0x2, 
    0x2, 0x3c, 0x3e, 0x3, 0x2, 0x2, 0x2, 0x3d, 0x3b, 0x3, 0x2, 0x2, 0x2, 
    0x3e, 0x48, 0x5, 0x8, 0x5, 0x2, 0x3f, 0x41, 0x5, 0x6, 0x4, 0x2, 0x40, 
    0x3f, 0x3, 0x2, 0x2, 0x2, 0x41, 0x42, 0x3, 0x2, 0x2, 0x2, 0x42, 0x40, 
    0x3, 0x2, 0x2, 0x2, 0x42, 0x43, 0x3, 0x2, 0x2, 0x2, 0x43, 0x44, 0x3, 
    0x2, 0x2, 0x2, 0x44, 0x45, 0x5, 0x8, 0x5, 0x2, 0x45, 0x47, 0x3, 0x2, 
    0x2, 0x2, 0x46, 0x40, 0x3, 0x2, 0x2, 0x2, 0x47, 0x4a, 0x3, 0x2, 0x2, 
    0x2, 0x48, 0x46, 0x3, 0x2, 0x2, 0x2, 0x48, 0x49, 0x3, 0x2, 0x2, 0x2, 
    0x49, 0xb, 0x3, 0x2, 0x2, 0x2, 0x4a, 0x48, 0x3, 0x2, 0x2, 0x2, 0x4b, 
    0x4d, 0x7, 0x13, 0x2, 0x2, 0x4c, 0x4e, 0x7, 0x11, 0x2, 0x2, 0x4d, 0x4c, 
    0x3, 0x2, 0x2, 0x2, 0x4d, 0x4e, 0x3, 0x2, 0x2, 0x2, 0x4e, 0x4f, 0x3, 
    0x2, 0x2, 0x2, 0x4f, 0x50, 0x7, 0x18, 0x2, 0x2, 0x50, 0x52, 0x7, 0x17, 
    0x2, 0x2, 0x51, 0x53, 0x7, 0x11, 0x2, 0x2, 0x52, 0x51, 0x3, 0x2, 0x2, 
    0x2, 0x52, 0x53, 0x3, 0x2, 0x2, 0x2, 0x53, 0x54, 0x3, 0x2, 0x2, 0x2, 
    0x54, 0x55, 0x7, 0x18, 0x2, 0x2, 0x55, 0x56, 0x7, 0x14, 0x2, 0x2, 0x56, 
    0xd, 0x3, 0x2, 0x2, 0x2, 0x57, 0x58, 0x7, 0x1a, 0x2, 0x2, 0x58, 0x59, 
    0x7, 0xe, 0x2, 0x2, 0x59, 0x5d, 0x5, 0xa, 0x6, 0x2, 0x5a, 0x5b, 0x5, 
    0x6, 0x4, 0x2, 0x5b, 0x5c, 0x5, 0xc, 0x7, 0x2, 0x5c, 0x5e, 0x3, 0x2, 
    0x2, 0x2, 0x5d, 0x5a, 0x3, 0x2, 0x2, 0x2, 0x5d, 0x5e, 0x3, 0x2, 0x2, 
    0x2, 0x5e, 0xf, 0x3, 0x2, 0x2, 0x2, 0x5f, 0x60, 0x5, 0xa, 0x6, 0x2, 
    0x60, 0x61, 0x9, 0x4, 0x2, 0x2, 0x61, 0x62, 0x5, 0xa, 0x6, 0x2, 0x62, 
    0x11, 0x3, 0x2, 0x2, 0x2, 0x63, 0x64, 0x7, 0x1a, 0x2, 0x2, 0x64, 0x65, 
    0x7, 0x3, 0x2, 0x2, 0x65, 0x66, 0x5, 0xc, 0x7, 0x2, 0x66, 0x13, 0x3, 
    0x2, 0x2, 0x2, 0x67, 0x6a, 0x5, 0x10, 0x9, 0x2, 0x68, 0x6a, 0x5, 0x12, 
    0xa, 0x2, 0x69, 0x67, 0x3, 0x2, 0x2, 0x2, 0x69, 0x68, 0x3, 0x2, 0x2, 
    0x2, 0x6a, 0x6b, 0x3, 0x2, 0x2, 0x2, 0x6b, 0x69, 0x3, 0x2, 0x2, 0x2, 
    0x6b, 0x6c, 0x3, 0x2, 0x2, 0x2, 0x6c, 0x15, 0x3, 0x2, 0x2, 0x2, 0xf, 
    0x17, 0x1c, 0x23, 0x2f, 0x35, 0x3b, 0x42, 0x48, 0x4d, 0x52, 0x5d, 0x69, 
    0x6b, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

FormulaParser::Initializer FormulaParser::_init;