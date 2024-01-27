{-# LANGUAGE DerivingStrategies         #-}
{-# LANGUAGE GeneralisedNewtypeDeriving #-}

module HW4.T2
  ( ParseError (..)
  , runP
  , pChar
  , parseError
  , parseExpr
  ) where

import Control.Applicative
import Control.Monad
import Numeric.Natural (Natural)

import Data.Char (isDigit, isSpace)
import Data.Scientific
import HW4.T1 (ExceptState (..))
import HW4.Types

data ParseError = ErrorAtPos Natural
  deriving Show

newtype Parser a = P (ExceptState ParseError (Natural, String) a)
  deriving newtype (Functor, Applicative, Monad)

runP :: Parser a -> String -> Except ParseError a
runP (P es) str = case runES es (0, str) of
  Error e          -> Error e
  Success (a :# _) -> Success a

-- Just an example of parser that may be useful
-- in the implementation of 'parseExpr'
pChar :: Parser Char
pChar = P $ ES $ \(pos, s) ->
  case s of
    []     -> Error (ErrorAtPos pos)
    (c:cs) -> Success (c :# (pos + 1, cs))

parseError :: Parser a
parseError = P $ ES $ \(pos, _) -> Error (ErrorAtPos pos)

instance Alternative Parser where
  empty = parseError
  (<|>) (P p1) (P p2) = P $ ES $ \s ->
    case runES p1 s of
      Error _   -> runES p2 s
      Success a -> Success a

-- No metohds
instance MonadPlus Parser

pEof :: Parser ()
pEof = P $ ES $ \s@(pos, str) ->
  case str of
    [] -> Success $ () :# s
    _  -> Error $ ErrorAtPos pos

pWs :: Parser String
pWs = many $ mfilter isSpace pChar

pCharTake :: Char -> Parser Char
pCharTake ch = mfilter (==ch) pChar

--E→TE′
--E′→+E∣ε
--T→FT′
--T′→×T∣ε
--F→n∣(E)

operatorMap :: Char -> Maybe (a -> a -> Prim a)
operatorMap sgn = case sgn of
  '+' -> Just Add
  '-' -> Just Sub
  '*' -> Just Mul
  '/' -> Just Div
  _   -> Nothing

pPriority :: Char -> Char -> Expr -> Parser Expr
pPriority c1 c2 e = do
  sgn <- pWs *> (pCharTake c1 <|> pCharTake c2)
  t <- pWs *> pe
  case operatorMap sgn of
    Just op -> pure $ Op $ op e t
    Nothing -> empty

pTryExtend :: Parser a -> (a -> Parser a) -> Parser a
pTryExtend fRequired fOptional = do
  e <- pWs *> fRequired
  fOptional e <|> pure e

parseExpr :: String -> Except ParseError Expr
parseExpr = runP $ do pe <* pWs <* pEof

pe :: Parser Expr
pe = pTryExtend pt pe'

pe' :: Expr -> Parser Expr
pe' = pPriority '+' '-'

pt :: Parser Expr
pt = pTryExtend pf pt'

pt' :: Expr -> Parser Expr
pt' = pPriority '*' '/'

pf :: Parser Expr
pf = pWs *> fmap Val pVal <|> pCharTake '(' *> pe <* pCharTake ')'

pVal :: Parser Double
pVal = pDouble <|> pInt where
  pDouble = do
    int <- pWs *> pStrInt
    _ <- pCharTake '.'
    frac <- pStrInt
    pure $ toRealFloat $ scientific (read $ int++frac) (-length frac)

  pInt = do read <$> pStrInt

  pStrInt = some $ mfilter isDigit pChar
