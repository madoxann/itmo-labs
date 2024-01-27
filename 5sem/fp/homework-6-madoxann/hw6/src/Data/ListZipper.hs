-- | This module defines 'ListZipper' datatype.
-- Feel free to define additional helper functions to work
-- with this datatype in this module.
module Data.ListZipper
  ( ListZipper (..)
  , lLeft
  , lRight
  , lWrite
  , toList
  , lGenerator
  ) where

import Control.Comonad (Comonad (..))

data ListZipper a = LZ [a] a [a]

instance Functor ListZipper where
  fmap f (LZ ls x rs) = LZ (map f ls) (f x) (map f rs)

iterateTail :: (a -> a) -> a -> [a]
iterateTail f = tail . iterate f -- "tail" will never return a bottom
  
lGenerator :: (a -> a)     -- ^ The left generator
           -> (a -> a)     -- ^ The right generator
           -> a            -- ^ The focus
           -> ListZipper a -- ^ The resulting list zipper
lGenerator f g x = LZ (iterateTail f x) x (iterateTail g x)

lLeft, lRight :: ListZipper a -> ListZipper a

lLeft  (LZ (l : ls) c rs) = LZ ls l (c : rs)
lLeft  lz = lz

lRight (LZ ls c (r : rs)) = LZ (c : ls) r rs
lRight lz = lz

instance Comonad ListZipper where
  extract (LZ _ x _) = x
  duplicate = lGenerator lLeft lRight 
  
lWrite :: a -> ListZipper a -> ListZipper a
lWrite x (LZ ls _ rs) = LZ ls x rs

toList :: ListZipper a -> Int -> [a]
toList (LZ ls x rs) n = reverse (take n ls) ++ [x] ++ take n rs