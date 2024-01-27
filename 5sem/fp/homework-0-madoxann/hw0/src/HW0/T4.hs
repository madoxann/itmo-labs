module HW0.T4
  ( fac
  , fib
  , map'
  , repeat'
  ) where

import Numeric.Natural (Natural)
import Data.Function (fix)

repeat' :: a -> [a]
repeat' a = fix (a :)

map' :: (a -> b) -> [a] -> [b]
map' f = fix (\f' lst -> case lst of
                           [] -> []
                           x:xa -> f x : f' xa)

fib :: Natural -> Natural
fib = fix (\rec n' n'' cnt -> if cnt == 0 then n' else rec (n' + n'') n' (cnt - 1)) 0 1

fac :: Natural -> Natural
fac = fix (\rec n' cnt -> if cnt == 0 then n' else rec (n' * cnt) (cnt - 1)) 1