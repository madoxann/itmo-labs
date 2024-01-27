module HW3.T2
  ( distOption
  , wrapOption
  , distPair
  , wrapPair
  , distQuad
  , wrapQuad
  , distAnnotated
  , wrapAnnotated
  , distExcept
  , wrapExcept
  , distPrioritised
  , wrapPrioritised
  , distStream
  , wrapStream
  , distList
  , wrapList
  , distFun
  , wrapFun
  , lstConcat
  ) where

import HW3.T1

distOption :: (Option a, Option b) -> Option (a, b)
distOption (None, _)        = None
distOption (_, None)        = None
distOption (Some a, Some b) = Some (a, b)

wrapOption :: a -> Option a
wrapOption = Some

distPair :: (Pair a, Pair b) -> Pair (a, b)
distPair (P a1 a2, P b1 b2) = P (a1, b1) (a2, b2)

wrapPair :: a -> Pair a
wrapPair a = P a a

distQuad :: (Quad a, Quad b) -> Quad (a, b)
distQuad (Q a1 a2 a3 a4, Q b1 b2 b3 b4) = Q (a1, b1) (a2, b2) (a3, b3) (a4, b4)

wrapQuad :: a -> Quad a
wrapQuad a = Q a a a a

distAnnotated :: Semigroup e => (Annotated e a, Annotated e b) -> Annotated e (a, b)
distAnnotated (a :# e1, b :# e2) = (a, b) :# e1 <> e2

wrapAnnotated :: Monoid e => a -> Annotated e a
wrapAnnotated = (:# mempty)

distExcept :: (Except e a, Except e b) -> Except e (a, b)
distExcept (Error e, _)           = Error e
distExcept (_, Error e)           = Error e
distExcept (Success a, Success b) = Success (a, b)

wrapExcept :: a -> Except e a
wrapExcept = Success

distPrioritised :: (Prioritised a, Prioritised b) -> Prioritised (a, b)
distPrioritised (Low a, Low b)       = Low (a, b)
distPrioritised (Low a, Medium b)    = Medium (a, b)
distPrioritised (Low a, High b)      = High (a, b)
distPrioritised (Medium a, Low b)    = Medium (a, b)
distPrioritised (Medium a, Medium b) = Medium (a, b)
distPrioritised (Medium a, High b)   = High (a, b)
distPrioritised (High a, Low b)      = High (a, b)
distPrioritised (High a, Medium b)   = High (a, b)
distPrioritised (High a, High b)     = High (a, b)

wrapPrioritised :: a -> Prioritised a
wrapPrioritised = Low

distStream :: (Stream a, Stream b) -> Stream (a, b)
distStream (a :> xa, b :> xb) = (a, b) :> distStream (xa, xb)

wrapStream :: a -> Stream a
wrapStream a = a :> wrapStream a

lstConcat :: List a -> List a -> List a
lstConcat Nil lst         = lst
lstConcat lst Nil         = lst
lstConcat (a' :. xa') lst = a' :. lstConcat xa' lst
  
distList :: (List a, List b) -> List (a, b)
distList (Nil, _) = Nil
distList (_, Nil) = Nil
distList (a :. xa, lst) = lstAssociate a lst `lstConcat` distList (xa, lst) where
  lstAssociate _ Nil        = Nil
  lstAssociate a' (b :. xb) = (a', b) :. lstAssociate a' xb

wrapList :: a -> List a
wrapList = (:. Nil) 

distFun :: (Fun i a, Fun i b) -> Fun i (a, b)
distFun (F fa, F fb) = F (\i -> (fa i, fb i))

wrapFun :: a -> Fun i a
wrapFun a = F (const a) 
