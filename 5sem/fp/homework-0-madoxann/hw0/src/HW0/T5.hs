module HW0.T5
  ( Nat
  , nFromNatural
  , nmult
  , nplus
  , ns
  , nToNum
  , nz
  ) where

import Numeric.Natural

type Nat a = (a -> a) -> a -> a 

nz :: Nat a
nz _ z = z 

ns :: Nat a -> Nat a
ns n f z = n f (f z)

nplus :: Nat a -> Nat a -> Nat a
nplus n1 n2 f z = n1 f (n2 f z) 

nmult :: Nat a -> Nat a -> Nat a
nmult n1 n2 f = n1 (n2 f)

nFromNatural :: Natural -> Nat a
nFromNatural 0 = nz
nFromNatural n = ns $ nFromNatural (n - 1) 

nToNum :: Num a => Nat a -> a
nToNum n = n (+1) 0