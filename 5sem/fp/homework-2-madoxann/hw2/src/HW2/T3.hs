module HW2.T3
  ( epart
  , mcat
  ) where

import Data.Foldable (fold)


mcat :: Monoid a => [Maybe a] -> a
mcat = foldr f mempty
  where f (Just i) x = i <> x
        f Nothing x = x

epart :: (Monoid a, Monoid b) => [Either a b] -> (a, b)
epart = foldMap f 
  where f (Left x) = (x, mempty)
        f (Right x) = (mempty, x)
        