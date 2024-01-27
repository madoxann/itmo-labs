module HW4.T1
  ( EvaluationError (..)
  , ExceptState (..)
  , mapExceptState
  , wrapExceptState
  , joinExceptState
  , modifyExceptState
  , throwExceptState
  , eval
  ) where

import Control.Monad
import HW4.Types

data ExceptState e s a = ES { runES :: s -> Except e (Annotated s a) }

mapExceptState :: (a -> b) -> ExceptState e s a -> ExceptState e s b
mapExceptState f es = ES (f' . runES es) where
  f' (Error e)          = Error e
  f' (Success (a :# s)) = Success $ f a :# s

wrapExceptState :: a -> ExceptState e s a
wrapExceptState a = ES $ \s -> Success $ a :# s

joinExceptState :: ExceptState e s (ExceptState e s a) -> ExceptState e s a
joinExceptState es = ES (f' . runES es) where
  f' (Error e)          = Error e
  f' (Success (a :# s)) = runES a s

modifyExceptState :: (s -> s) -> ExceptState e s ()
modifyExceptState f = ES $ \s -> Success $ () :# f s

throwExceptState :: e -> ExceptState e s a
throwExceptState e = ES $ \_ -> Error e

instance Functor (ExceptState e s) where
  fmap = mapExceptState

instance Applicative (ExceptState e s) where
  pure = wrapExceptState
  (<*>) = Control.Monad.ap

instance Monad (ExceptState e s) where
  (>>=) m f = joinExceptState $ fmap f m

data EvaluationError = DivideByZero
  deriving Show

evalUnary :: Expr -> (Double -> Prim Double) -> (Double -> a) -> ExceptState EvaluationError [Prim Double] a
evalUnary a f f' = do st <- eval a; modifyExceptState (\s -> f st : s); return $ f' st;

evalBinary :: Expr -> Expr -> (Double -> Double -> Prim Double) -> (Double -> Double -> a) -> ExceptState EvaluationError [Prim Double] a
evalBinary a1 a2 f f' = do st1 <- eval a1; st2 <- eval a2; modifyExceptState (\s -> f st1 st2 : s); return $ f' st1 st2;

eval :: Expr -> ExceptState EvaluationError [Prim Double] Double
eval (Val d) = pure d
eval (Op (Add a1 a2)) = evalBinary a1 a2 Add (+)
eval (Op (Sub a1 a2)) = evalBinary a1 a2 Sub (-)
eval (Op (Mul a1 a2)) = evalBinary a1 a2 Mul (*)
eval (Op (Div a1 a2)) = do
  st1 <- eval a1
  st2 <- eval a2
  if st2 /= 0
    then modifyExceptState (\s -> Div st1 st2 : s)
    else throwExceptState DivideByZero
  return (st1 / st2)
eval (Op (Abs a)) = evalUnary a Abs abs
eval (Op (Sgn a)) = evalUnary a Sgn signum
