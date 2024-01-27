{-# LANGUAGE DerivingStrategies #-}
{-# LANGUAGE DeriveGeneric #-}

module HW5.Base
  ( HiError (..)
  , HiExpr (..)
  , HiFun (..)
  , HiValue (..)
  , AritizedHiFun (..)
  , EvalHiFun (..)
  , HiMonad (..)
  , HiAction (..)
  , circutedAritizedFun
  ) where
    
import Data.Text
import Data.Sequence (Seq, fromList)
import Data.ByteString (ByteString)
import Codec.Serialise
import GHC.Generics (Generic)
import Data.Time.Clock

data HiFun =
  HiFunDiv
  | HiFunMul
  | HiFunAdd
  | HiFunSub
  | HiFunNot
  | HiFunAnd
  | HiFunOr
  | HiFunLessThan
  | HiFunGreaterThan
  | HiFunEquals
  | HiFunNotLessThan
  | HiFunNotGreaterThan
  | HiFunNotEquals
  | HiFunIf
  | HiFunLength
  | HiFunToUpper
  | HiFunToLower
  | HiFunReverse
  | HiFunTrim
  | HiFunList
  | HiFunRange
  | HiFunFold
  | HiFunPackBytes
  | HiFunUnpackBytes
  | HiFunEncodeUtf8
  | HiFunDecodeUtf8
  | HiFunZip
  | HiFunUnzip
  | HiFunSerialise
  | HiFunDeserialise
  | HiFunRead
  | HiFunWrite
  | HiFunMkDir
  | HiFunChDir
  | HiFunParseTime
  | HiFunRand
  | HiFunEcho
  deriving(Eq, Ord, Show, Generic)

data HiValue =
   HiValueBool Bool
  | HiValueNumber Rational
  | HiValueFunction HiFun
  | HiValueNull
  | HiValueString Text
  | HiValueList (Seq HiValue)
  | HiValueBytes ByteString
  | HiValueAction HiAction
  | HiValueTime UTCTime
  deriving(Eq, Ord, Show, Generic)

data HiAction =
    HiActionRead  FilePath
  | HiActionWrite FilePath ByteString
  | HiActionMkDir FilePath
  | HiActionChDir FilePath
  | HiActionCwd
  | HiActionNow
  | HiActionRand Int Int
  | HiActionEcho Text
  deriving(Eq, Ord, Show, Generic)

data HiExpr =
   HiExprValue HiValue
  | HiExprApply HiExpr [HiExpr]
  | HiExprRun HiExpr

data HiError =
   HiErrorInvalidArgument
  | HiErrorInvalidFunction
  | HiErrorArityMismatch
  | HiErrorDivideByZero
  deriving(Show)

instance Serialise HiValue
instance Serialise HiFun
instance Serialise HiAction

class Monad m => HiMonad m where
  runAction :: HiAction -> m HiValue
  
-- helper types of Evaluator
data EvalHiFun =
  Div
  | Add
  | Sub
  | Mul
  | Not
  | And
  | Or
  | LessThan
  | GreaterThan
  | Equals
  | NotLessThan
  | NotGreaterThan
  | NotEquals
  | If
  | Length
  | ToUpper
  | ToLower
  | Reverse
  | Trim
  | StringAsFunction HiValue
  | List
  | Range
  | Fold
  | ListAsFunction HiValue
  | PackBytes
  | UnpackBytes
  | EncodeUtf8
  | DecodeUtf8
  | Zip
  | Unzip
  | Serialise
  | Deserialise
  | BytesAsFunction HiValue
  | Read
  | Write
  | MkDir
  | ChDir
  | Action HiAction
  | ParseTime
  | Rand
  | Echo
  deriving(Eq)

data AritizedHiFun =
  Zero EvalHiFun
  | Unary EvalHiFun
  | Binary EvalHiFun
  | Ternary EvalHiFun
  | Vararg EvalHiFun
  deriving(Eq)

-- functions, that require special evaluation
circutedAritizedFun :: Seq AritizedHiFun
circutedAritizedFun = fromList [Binary And, Binary Or, Ternary If]
