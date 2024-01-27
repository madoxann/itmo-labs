module HW3.T4
  ( State (..)
  , Prim (..)
  , Expr (..)
  , mapState
  , wrapState
  , joinState
  , modifyState
  , eval
  ) where

import HW3.T1

newtype State s a = S { runS :: s -> Annotated s a }

mapState :: (a -> b) -> State s a -> State s b
mapState f s = S $ mapAnnotated f . runS s

wrapState :: a -> State s a
wrapState a = S (a :#)

joinState :: State s (State s a) -> State s a
joinState st = S f where
  f s = runS sta st' where
    sta :# st' = runS st s

modifyState :: (s -> s) -> State s ()
modifyState f = S (\s -> () :# f s)

instance Functor (State s) where
  fmap = mapState

instance Applicative (State s) where
  pure = wrapState
  (<*>) stf st = S (\s -> runS (fmap (unwrap $ runS stf s) st) s) where
    unwrap (f :# _) = f

instance Monad (State s) where
  (>>=) st f = joinState $ fmap f st

data Prim a =
    Add a a
  | Sub a a
  | Mul a a
  | Div a a
  | Abs a
  | Sgn a
  deriving Show

data Expr = Val Double | Op (Prim Expr)
  deriving Show

instance Num Expr where
  (+) x y = Op $ Add x y
  (-) x y = Op $ Sub x y
  (*) x y = Op $ Mul x y
  abs a = Op $ Abs a
  signum a = Op $ Sgn a
  fromInteger a = Val $ fromInteger a

instance Fractional Expr where
  (/) x y = Op $ Div x y
  fromRational a = Val $ fromRational a

eval :: Expr -> State [Prim Double] Double
eval (Val d)      = pure d
eval (Op (Add a1 a2)) = do { st1 <- eval a1; st2 <- eval a2; modifyState (\s -> Add st1 st2:s); return (st1 + st2); }
eval (Op (Sub a1 a2)) = do { st1 <- eval a1; st2 <- eval a2; modifyState (\s -> Sub st1 st2:s); return (st1 - st2); }
eval (Op (Mul a1 a2)) = do { st1 <- eval a1; st2 <- eval a2; modifyState (\s -> Mul st1 st2:s); return (st1 * st2); }
eval (Op (Div a1 a2)) = do { st1 <- eval a1; st2 <- eval a2; modifyState (\s -> Div st1 st2:s); return (st1 / st2); }
eval (Op (Abs a)) = do { st <- eval a; modifyState (\s -> Abs st:s); return (abs st); }
eval (Op (Sgn a)) = do { st <- eval a; modifyState (\s -> Sgn st:s); return (signum st); }
