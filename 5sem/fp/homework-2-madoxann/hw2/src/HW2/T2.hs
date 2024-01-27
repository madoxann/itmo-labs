module HW2.T2
  ( joinWith
  , splitOn
  ) where

import Data.List.NonEmpty (NonEmpty (..))

splitOn :: Eq a => a -> [a] -> NonEmpty [a]
splitOn sep = foldr f ([] :| [])
  where f i (x :| xs) | i == sep = [] :| (x : xs)
                      | otherwise = (i:x) :| xs

joinWith :: a -> NonEmpty [a] -> [a]
joinWith sep (x:|xs) = x ++ foldr f [] xs
  where f i lst = sep : i ++ lst
