module HW2.T4
  ( DotString (..)
  , Fun (..)
  , Inclusive (..)
  , ListPlus (..)
  ) where

data ListPlus a = a :+ ListPlus a | Last a
  deriving Show

infixr 5 :+

instance Semigroup (ListPlus a) where
  (<>) (Last l) r  = l :+ r
  (<>) (l :+ ls) r =  l :+ (ls <> r)

data Inclusive a b = This a | That b | Both a b
  deriving Show

-- You may necessary constraints there
instance (Semigroup a, Semigroup b) => Semigroup (Inclusive a b) where
  (<>) (This a) (This b)         = This $ a <> b
  (<>) (That a) (That b)         = That $ a <> b
  (<>) (This a) (That b)         = Both a b
  (<>) (That a) (This b)         = Both b a

  (<>) (Both a1 b1) (This b)     = Both (a1 <> b) b1
  (<>) (Both a1 b1) (That b)     = Both a1 (b1 <> b)

  (<>) (This a) (Both a2 b2)     = Both (a <> a2) b2
  (<>) (That a) (Both a2 b2)     = Both a2 (a <> b2)

  (<>) (Both a1 b1) (Both a2 b2) = Both (a1 <> a2) (b1 <> b2)

newtype DotString = DS String
  deriving Show

instance Semigroup DotString where
  (<>) (DS "") r     = r
  (<>) l (DS "")     = l
  (<>) (DS l) (DS r) = DS $ l ++ "." ++ r

instance Monoid DotString where
  mempty = DS ""

newtype Fun a = F (a -> a)

instance Semigroup (Fun a) where
  (<>) (F f1) (F f2) = F $ f1 . f2

instance Monoid (Fun a) where
  mempty = F id
