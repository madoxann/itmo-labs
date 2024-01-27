module HW1.T2
  ( N (..)
  , nplus
  , nmult
  , nsub
  , nFromNatural
  , nToNum
  , ncmp
  , nEven
  , nOdd
  , ndiv
  , nmod
  ) where

import Data.Maybe (fromJust, isNothing)
import Numeric.Natural

data N = Z | S N

nplus :: N -> N -> N
nplus Z n           = n
nplus n Z           = n
nplus (S n1) (S n2) = S (S (nplus n1 n2))

nmult :: N -> N -> N
nmult Z _       = Z
nmult _ Z       = Z
nmult n1 (S n2) = n1 `nplus` (nmult n1 n2)

nsub :: N -> N -> Maybe N
nsub n Z           = Just n
nsub Z (S _)       = Nothing
nsub (S n1) (S n2) = nsub n1 n2

ncmp :: N -> N -> Ordering
ncmp Z Z           = EQ
ncmp Z _           = LT
ncmp _ Z           = GT
ncmp (S n1) (S n2) = ncmp n1 n2

nFromNatural :: Natural -> N
nFromNatural 0 = Z
nFromNatural n = S (nFromNatural (n-1))

nToNum :: Num a => N -> a
nToNum Z     = 0
nToNum (S n) = 1 + nToNum n

nEven :: N -> Bool
nEven Z         = True
nEven (S Z)     = False
nEven (S (S n)) = nEven n

nOdd :: N -> Bool
nOdd n = not (nEven n)

ndiv :: N -> N -> N
ndiv n1 n2 | isNothing s = Z
         | otherwise = (S Z) `nplus` (ndiv (fromJust s) n2)
           where s = nsub n1 n2

nmod :: N -> N -> N
nmod n1 n2 = fromJust (nsub n1 (n2 `nmult` ndiv n1 n2))
