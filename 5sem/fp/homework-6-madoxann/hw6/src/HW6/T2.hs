{-# LANGUAGE DataKinds #-}
{-# LANGUAGE TypeFamilies #-}
{-# LANGUAGE TypeOperators #-}
{-# LANGUAGE PolyKinds #-}
{-# LANGUAGE UndecidableInstances #-}

module HW6.T2
  ( TSet

  , Contains
  , Add
  , Delete
  ) where

import GHC.TypeLits

type TSet = [Symbol]

type family Contains (name :: Symbol) (set :: TSet) :: Bool where
  Contains x (x ': _) = 'True
  Contains x (_ ': xs) = Contains x xs
  Contains _ _ = 'False

type family Delete (name :: Symbol) (set :: TSet) :: TSet where
  Delete name (name ': xs) = xs
  Delete name (x ': xs) =  x ': Delete name xs
  Delete name '[] = '[]

type family Add (v :: Symbol) (set :: TSet) :: TSet where
  Add name s =  name ': Delete name s