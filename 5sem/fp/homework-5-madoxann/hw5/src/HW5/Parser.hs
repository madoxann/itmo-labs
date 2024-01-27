module HW5.Parser
  ( parse
  ) where

import Data.Void (Void)

import HW5.Base
import qualified Text.Megaparsec as MP
import Text.Megaparsec.Char
import qualified Text.Megaparsec.Char.Lexer as L
import Control.Monad.Combinators.Expr
import Text.Megaparsec(Parsec, eof, choice, empty, between, sepBy, (<|>), notFollowedBy, try, manyTill, sepEndBy, some)
import Text.Megaparsec.Error (ParseErrorBundle)
import Data.Text (pack)
import qualified Data.ByteString as BS
import Numeric (readHex)
type Parser = Parsec Void String

parse :: String -> Either (ParseErrorBundle String Void) HiExpr
parse = MP.parse (between space eof expr) "" where
  expr = makeExprParser expr' operatorTable
  expr' = do
    t <- term
    parenthesis t

  term = choice [
      bytes,
      list,
      betweenSymbols "(" expr ")",
      parseValue
    ]

  list = HiExprApply (HiExprValue $ HiValueFunction HiFunList) <$> wrapLexeme (betweenSymbols "[" commaSeparated "]")
  bytes = HiExprValue . HiValueBytes . BS.pack <$>  betweenSymbols "[#" (wrapLexeme $ sepEndBy hex space1) "#]"

  hex = do
    h1 <- hexDigitChar
    h2 <- hexDigitChar
    pure $ fst $ head $ readHex [h1, h2]
  parenthesis t = (do
    prev <- HiExprApply t <$> betweenSymbols "(" commaSeparated ")"
    parenthesis prev) <|> pure t
  commaSeparated = sepBy expr $ wrapSymbol ","

operatorTable :: [[Operator Parser HiExpr]]
operatorTable = [
    [ Postfix $ foldr1 (.) <$> some (HiExprRun <$ wrapSymbol "!") ],
    [
      bl "*" HiFunMul,
      InfixL $ applyBinary HiFunDiv <$ (wrapLexeme . try) (wrapSymbol "/" <* notFollowedBy (wrapSymbol "="))
    ],
    [
      bl "+" HiFunAdd,
      bl "-" HiFunSub
    ],
    [
      bn ">=" HiFunNotLessThan,
      bn "<=" HiFunNotGreaterThan,
      bn "==" HiFunEquals,
      bn "/=" HiFunNotEquals,
      bn "<" HiFunLessThan,
      bn ">" HiFunGreaterThan
    ],
    [
      br "&&" HiFunAnd
    ],
    [
      br "||" HiFunOr
    ]
  ] where
  applyBinary f a1 a2 = HiExprApply (HiExprValue $ HiValueFunction f) [a1, a2]
  bl name f = InfixL $ applyBinary f <$ wrapSymbol name
  br name f = InfixR $ applyBinary f <$ wrapSymbol name
  bn name f = InfixN $ applyBinary f <$ wrapSymbol name

parseValue :: Parser HiExpr
parseValue = (<$>) HiExprValue $ parseFunction <|> parseAction <|> parseBool <|> parseString <|> parseNumber where
  parseNumber = wrapLexeme $ HiValueNumber . toRational <$> L.signed spaceConsumer L.scientific
  parseFunction = HiValueFunction <$> wrapLexeme (choice 
    [
    HiFunNotGreaterThan <$ string "not-greater-than"
    , HiFunNotLessThan <$ string "not-less-than"
    , HiFunGreaterThan <$ string "greater-than"
    , HiFunUnpackBytes <$ string "unpack-bytes"
    , HiFunNotEquals <$ string "not-equals"
    , HiFunDeserialise <$ string "deserialise"
    , HiFunSerialise <$ string "serialise"
    , HiFunDecodeUtf8 <$ string "decode-utf8"
    , HiFunEncodeUtf8 <$ string "encode-utf8"
    , HiFunParseTime <$ string "parse-time"
    , HiFunPackBytes <$ string "pack-bytes"
    , HiFunLessThan <$ string "less-than"
    , HiFunToUpper <$ string "to-upper"
    , HiFunToLower <$ string "to-lower"
    , HiFunEquals <$ string "equals"
    , HiFunLength <$ string "length"
    , HiFunReverse <$ string "reverse"
    , HiFunRange <$ string "range"
    , HiFunUnzip <$ string "unzip"
    , HiFunMkDir <$ string "mkdir"
    , HiFunWrite <$ string "write"
    , HiFunTrim <$ string "trim"
    , HiFunFold <$ string "fold"
    , HiFunList <$ string "list"
    , HiFunRead <$ string "read"
    , HiFunRand <$ string "rand"
    , HiFunEcho <$ string "echo"
    , HiFunDiv <$ string "div"
    , HiFunMul <$ string "mul"
    , HiFunAdd <$ string "add"
    , HiFunSub <$ string "sub"
    , HiFunAnd <$ string "and"
    , HiFunNot <$ string "not"
    , HiFunZip <$ string "zip"
    , HiFunOr <$ string "or"
    , HiFunIf <$ string "if"
    , HiFunChDir <$ string "cd"
    ])
  parseAction = HiValueAction <$> wrapLexeme (choice [HiActionCwd <$ string "cwd", HiActionNow <$ string "now"])
  parseBool = HiValueBool <$> wrapLexeme (choice [True <$ string "true", False <$ string "false"])
  parseString = wrapLexeme $ choice [ HiValueNull <$ string "null", HiValueString . pack <$> (char '"' *> manyTill L.charLiteral (char '"')) ]

-- helper functions
spaceConsumer :: Parser ()
spaceConsumer = L.space space1 empty empty

wrapLexeme :: Parser a -> Parser a
wrapLexeme = L.lexeme spaceConsumer

wrapSymbol :: String -> Parser String
wrapSymbol = L.symbol spaceConsumer

betweenSymbols :: String ->  Parser a -> String -> Parser a
betweenSymbols l p r = between (wrapSymbol l) (wrapSymbol r) p
