module HW5.Pretty
  ( prettyValue
  ) where

import Prettyprinter (Doc, pretty, viaShow, list, fillSep, concatWith)
import Prettyprinter.Render.Terminal (AnsiStyle)

import Data.Foldable (toList)
import Data.Scientific
import HW5.Base 
import GHC.Real (Ratio((:%)))
import Data.Maybe (isNothing)
import qualified Data.ByteString as BS
import Numeric (showHex)

prettyValue :: HiValue -> Doc AnsiStyle
prettyValue (HiValueNumber n) = prettifyNumber n
prettyValue (HiValueFunction f) = prettifyFunction f
prettyValue (HiValueBool b) = pretty $ if b then "true" else "false"
prettyValue (HiValueString s) = viaShow s
prettyValue HiValueNull = pretty "null"
prettyValue (HiValueList l) = list $ toList $ prettyValue <$> l
prettyValue (HiValueBytes b) = pretty "[#" <> fillSep (pretty . hex <$> BS.unpack b) <> pretty "#]" where
  hex b' = (if b' <= 15 then "0" else mempty) <> showHex b' mempty
prettyValue (HiValueAction action) = 
  case action of
      (HiActionRead a) -> mimicFunction HiFunRead a
      (HiActionWrite a1 a2) -> mimicFunction HiFunWrite [viaShow a1, viaShow a2]
      (HiActionMkDir a) -> mimicFunction HiFunMkDir a
      (HiActionChDir a) -> mimicFunction HiFunChDir a
      HiActionCwd -> pretty "cwd"
      HiActionNow -> pretty "now"
      (HiActionRand l r) -> mimicFunction HiFunRand [l, r]
      (HiActionEcho s) -> mimicFunction HiFunEcho [s]
prettyValue (HiValueTime t) = mimicFunction HiFunParseTime [t]

mimicFunction :: Show a => HiFun -> [a] -> Doc ann
mimicFunction f args =  prettifyFunction f <> pretty "(" <> concatWith (\s1 s2 -> s1 <> pretty ", " <> s2) (viaShow <$> args) <> pretty ")"

prettifyNumber :: Rational -> Doc ann
prettifyNumber f@(n :% d) | d == 1 = pretty n
                          | isNothing period = pretty $ formatScientific Fixed Nothing s
                          | abs n > d = pretty w <> (if signum r > 0 then pretty '+' <> pretty r else pretty r) <> pretty '/' <> pretty d
                          | otherwise = pretty n <> pretty '/' <> pretty d
  where (s, period) = fromRationalRepetendUnlimited f
        (w, r) = quotRem n d

prettifyFunction :: HiFun -> Doc ann
prettifyFunction f = pretty $ case f of 
-- t1  
  HiFunDiv -> "div"
  HiFunMul -> "mul"
  HiFunAdd -> "add"
  HiFunSub -> "sub"
-- t2
  HiFunNot -> "not"
  HiFunAnd -> "and"
  HiFunOr -> "or"
  HiFunLessThan -> "less-than"
  HiFunGreaterThan -> "greater-than"
  HiFunEquals -> "equals"
  HiFunNotLessThan -> "not-less-than"
  HiFunNotGreaterThan -> "not-greater-than"
  HiFunNotEquals -> "not-equals"
  HiFunIf ->  "if"
-- t4
  HiFunLength -> "length"
  HiFunToUpper -> "to-upper"
  HiFunToLower -> "to-lower"
  HiFunReverse -> "reverse"
  HiFunTrim -> "trim"
-- t5
  HiFunList -> "list"
  HiFunRange -> "range"
  HiFunFold -> "fold"
-- t6
  HiFunPackBytes -> "pack-bytes"
  HiFunUnpackBytes -> "unpack-bytes"
  HiFunDeserialise -> "deserialise"
  HiFunSerialise -> "serialise"
  HiFunDecodeUtf8 -> "decode-utf8"
  HiFunEncodeUtf8 -> "encode-utf8"
  HiFunUnzip -> "unzip"
  HiFunZip -> "zip"
  HiFunRead -> "read"
  HiFunWrite -> "write"
  HiFunMkDir -> "mkdir"
  HiFunChDir -> "cd"
-- t8
  HiFunParseTime -> "parse-time"
--t9
  HiFunRand -> "rand"
--t10
  HiFunEcho -> "echo"