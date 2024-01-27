module HW3.T1
  ( Option (..)
  , Pair (..)
  , Quad (..)
  , Annotated (..)
  , Except (..)
  , Prioritised (..)
  , Stream (..)
  , List (..)
  , Fun (..)
  , Tree (..)
  , mapOption
  , mapPair
  , mapQuad
  , mapAnnotated
  , mapExcept
  , mapPrioritised
  , mapStream
  , mapList
  , mapFun
  , mapTree
  ) where

data Option a = None | Some a
  deriving Show

mapOption :: (a -> b) -> (Option a -> Option b)
mapOption f = f' where
  f' None     = None
  f' (Some a) = Some $ f a

data Pair a = P a a
  deriving Show

mapPair :: (a -> b) -> (Pair a -> Pair b)
mapPair f = f' where
  f' (P a1 a2) = P (f a1) (f a2)

data Quad a = Q a a a a
  deriving Show

mapQuad :: (a -> b) -> (Quad a -> Quad b)
mapQuad f = f' where
  f' (Q a1 a2 a3 a4) = Q (f a1) (f a2) (f a3) (f a4)

data Annotated e a = a :# e
  deriving Show

infix 0 :#

mapAnnotated :: (a -> b) -> (Annotated e a -> Annotated e b)
mapAnnotated f = f' where
  f' (a :# e) = f a :# e

data Except e a = Error e | Success a
  deriving Show

mapExcept :: (a -> b) -> (Except e a -> Except e b)
mapExcept f = f' where
  f' (Error e)   = Error e
  f' (Success a) = Success $ f a

data Prioritised a = Low a | Medium a | High a
  deriving Show

mapPrioritised :: (a -> b) -> (Prioritised a -> Prioritised b)
mapPrioritised f = f' where
  f' (Low a)    = Low $ f a
  f' (Medium a) = Medium $ f a
  f' (High a)   = High $ f a


data Stream a = a :> Stream a
  deriving Show

infixr 5 :>

mapStream :: (a -> b) -> (Stream a -> Stream b)
mapStream f = f' where
  f' (a :> ax) = f a :> f' ax

data List a = Nil | a :. List a
  deriving Show

infixr 5 :.

mapList :: (a -> b) -> (List a -> List b)
mapList f = f' where
  f' Nil       = Nil
  f' (a :. ax) = f a :. f' ax

data Fun i a = F (i -> a)

mapFun :: (a -> b) -> (Fun i a -> Fun i b)
mapFun f = f' where
  f' (F fa) = F $ f . fa

data Tree a = Leaf | Branch (Tree a) a (Tree a)
  deriving Show

mapTree :: (a -> b) -> (Tree a -> Tree b)
mapTree f = f' where
  f' Leaf             = Leaf
  f' (Branch tl a tr) = Branch (f' tl) (f a) (f' tr)
