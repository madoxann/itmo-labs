{-# LANGUAGE LambdaCase #-}
{-# LANGUAGE Strict #-}
{-# LANGUAGE TypeApplications #-}

module HW5.Evaluator
  ( eval
  ) where

import Control.Monad.Trans.Except (ExceptT (..), runExceptT, throwE)
import Data.Semigroup (stimes)
import qualified Data.Sequence as SQ
import qualified Data.Text as T
import qualified Data.ByteString as BS
import GHC.Real (Ratio ((:%)))
import HW5.Base
import Data.Maybe (fromMaybe)
import qualified Codec.Compression.Zlib as Z
import qualified Codec.Serialise as S
import Data.ByteString.Lazy (toStrict, fromStrict)
import Data.Text.Encoding as E
import qualified Data.Time.Clock as T
import Text.Read (readMaybe)

eval :: HiMonad m => HiExpr -> m (Either HiError HiValue)
eval expr = runExceptT $ evalExpect expr

evalExpect :: HiMonad m => HiExpr -> ExceptT HiError m HiValue
evalExpect e = case e of
  HiExprValue v -> pure v
  HiExprApply term args -> evalApply term args
  HiExprRun a -> evalAction a

evalApply :: HiMonad m => HiExpr -> [HiExpr] -> ExceptT HiError m HiValue
evalApply f lst = do
  u <- evalExpect f
  u' <- aritize u
  tryApply u' lst

evalAction :: HiMonad m => HiExpr -> ExceptT HiError m HiValue
evalAction a = evalExpect a >>= \case HiValueAction ua -> ExceptT $ Right <$> runAction ua
                                      _ -> throwE HiErrorInvalidFunction

tryApply :: HiMonad m => AritizedHiFun -> [HiExpr] -> ExceptT HiError m HiValue
tryApply f lst = if f `elem` circutedAritizedFun 
  then shortCircuit f lst
  else case f of
        Zero uf -> matchLength 0 uf
        Unary uf -> matchLength 1 uf
        Binary uf -> matchLength 2 uf
        Ternary uf -> matchLength 3 uf
        Vararg uf -> ok uf
  where
    matchLength l f' = if length lst == l then ok f' else notOk
    ok f' = evalMass f' lst
    notOk = throwE HiErrorArityMismatch

shortCircuit :: HiMonad m => AritizedHiFun -> [HiExpr] -> ExceptT HiError m HiValue
shortCircuit (Binary And) [a1, a2] = do
  a1' <- evalExpect a1
  case a1' of
    HiValueBool False -> pure a1'
    HiValueNull -> pure a1'
    _ -> evalExpect a2
shortCircuit (Binary And) _ = throwE HiErrorArityMismatch
shortCircuit (Binary Or) [a1, a2] = do
  a1' <- evalExpect a1
  case a1' of
    HiValueBool False -> evalExpect a2
    HiValueNull -> evalExpect a2
    _ -> pure a1'
shortCircuit (Binary Or) _ = throwE HiErrorArityMismatch
shortCircuit (Ternary If) [e, a1, a2] = do
   e' <- evalExpect e
   case e' of
     (HiValueBool b) -> if b then evalExpect a1 else evalExpect a2
     _ -> throwE HiErrorInvalidArgument
shortCircuit (Ternary If) _ = throwE HiErrorArityMismatch
shortCircuit f args = tryApply f args

evalMass :: HiMonad m => EvalHiFun -> [HiExpr] -> ExceptT HiError m HiValue
evalMass f lst = do
  e <- traverse evalExpect lst
  applyEvalHiFun f e

stimeize :: (HiMonad m, Semigroup b) => b -> Rational -> ExceptT HiError m b
stimeize lst n = do
  n' <- intOrThrow n
  pure $ n' `stimes` lst

bytesToHiNumber :: BS.ByteString -> [HiValue]
bytesToHiNumber b = HiValueNumber . toRational <$> BS.unpack b

applyEvalHiFun :: HiMonad m => EvalHiFun -> [HiValue] -> ExceptT HiError m HiValue
-- t1
applyEvalHiFun Div [HiValueNumber n1, HiValueNumber n2]
  | n2 == 0 = throwE HiErrorDivideByZero
  | otherwise = pure $ HiValueNumber $ n1 / n2
applyEvalHiFun Div [HiValueString s1, HiValueString s2] = pure $ HiValueString $ s1 <> T.pack "/" <> s2
applyEvalHiFun Mul [HiValueNumber n1, HiValueNumber n2] = pure $ HiValueNumber $ n1 * n2
applyEvalHiFun Mul [HiValueString s, HiValueNumber n] = HiValueString <$> stimeize s n
applyEvalHiFun Mul [HiValueList l, HiValueNumber n] = HiValueList <$> stimeize l n
applyEvalHiFun Mul [HiValueBytes b, HiValueNumber n] = HiValueBytes <$> stimeize b n
applyEvalHiFun Add [HiValueNumber n1, HiValueNumber n2] = pure $ HiValueNumber $ n1 + n2
applyEvalHiFun Add [HiValueString s1, HiValueString s2] = pure $ HiValueString $ s1 <> s2
applyEvalHiFun Add [HiValueList l1, HiValueList l2] = pure $ HiValueList $ l1 <> l2
applyEvalHiFun Add [HiValueBytes b1, HiValueBytes b2] = pure $ HiValueBytes $ b1 <> b2
applyEvalHiFun Add [HiValueTime t, HiValueNumber n] = pure $ HiValueTime $ T.addUTCTime (fromRational n) t
applyEvalHiFun Sub [HiValueNumber n1, HiValueNumber n2] = pure $ HiValueNumber $ n1 - n2
applyEvalHiFun Sub [HiValueTime t1, HiValueTime t2] = pure $ HiValueNumber $ toRational $ T.diffUTCTime t1 t2
-- t2
applyEvalHiFun Not [HiValueBool b] = pure $ HiValueBool $ not b
applyEvalHiFun And [HiValueBool b1, HiValueBool b2] = pure $ HiValueBool $ b1 && b2
applyEvalHiFun Or [HiValueBool b1, HiValueBool b2] = pure $ HiValueBool $ b1 || b2
applyEvalHiFun LessThan [a1, a2] = pure $ HiValueBool $ a1 < a2
applyEvalHiFun GreaterThan [a1, a2] = pure $ HiValueBool $ a1 > a2
applyEvalHiFun Equals [a1, a2] = pure $ HiValueBool $ a1 == a2
applyEvalHiFun NotLessThan [a1, a2] = pure $ HiValueBool $ a1 >= a2
applyEvalHiFun NotGreaterThan [a1, a2] = pure $ HiValueBool $ a1 <= a2
applyEvalHiFun NotEquals [a1, a2] = pure $ HiValueBool $ a1 /= a2
applyEvalHiFun If [HiValueBool b, a1, a2] = pure $ if b then a1 else a2
-- t4
applyEvalHiFun Length [HiValueString s] = pure $ (HiValueNumber . toRational) $ T.length s
applyEvalHiFun Length [HiValueList l] = pure $ (HiValueNumber . toRational) $ length l
applyEvalHiFun ToUpper [HiValueString s] = pure $ HiValueString $ T.toUpper s
applyEvalHiFun ToLower [HiValueString s] = pure $ HiValueString $ T.toLower s
applyEvalHiFun Reverse [HiValueString s] = pure $ HiValueString $ T.reverse s
applyEvalHiFun Reverse [HiValueList l] = pure $ HiValueList $ SQ.reverse l
applyEvalHiFun Trim [HiValueString s] = pure $ HiValueString $ T.strip s

applyEvalHiFun (StringAsFunction (HiValueString s)) [HiValueNumber n] = maybe HiValueNull (HiValueString . T.singleton) <$> index (T.unpack s) n
applyEvalHiFun f@(StringAsFunction (HiValueString _)) [HiValueNull, n@(HiValueNumber _)] = applyEvalHiFun f [HiValueNumber 0, n]
applyEvalHiFun f@(StringAsFunction (HiValueString s)) [n@(HiValueNumber _), HiValueNull] = applyEvalHiFun f [n, HiValueNumber $ (toRational . T.length) s]
applyEvalHiFun (StringAsFunction (HiValueString s)) [HiValueNumber n1, HiValueNumber n2] = maybe HiValueNull (HiValueString . T.pack) <$> slice (T.unpack s) n1 n2
--t5
applyEvalHiFun (ListAsFunction (HiValueList l)) [HiValueNumber n] = fromMaybe HiValueNull <$> index (foldr (:) [] l) n
applyEvalHiFun f@(ListAsFunction (HiValueList _)) [HiValueNull, n@(HiValueNumber _)] = applyEvalHiFun f [HiValueNumber 0, n]
applyEvalHiFun f@(ListAsFunction (HiValueList l)) [n@(HiValueNumber _), HiValueNull] = applyEvalHiFun f [n, HiValueNumber $ (toRational . length) l]
applyEvalHiFun (ListAsFunction (HiValueList l)) [HiValueNumber n1, HiValueNumber n2] = maybe HiValueNull (HiValueList . SQ.fromList) <$> slice (foldr (:) [] l) n1 n2

applyEvalHiFun List args = pure $ HiValueList $ SQ.fromList args
applyEvalHiFun Range [HiValueNumber n1, HiValueNumber n2] = pure $ HiValueList $ HiValueNumber <$> SQ.fromList [n1 .. n2]
applyEvalHiFun Fold [HiValueFunction f, HiValueList lst] = do
  f' <- aritize $ HiValueFunction f
  case f' of
    (Binary ef) -> if null lst then pure HiValueNull else foldl1 (\a1 a2 -> do l <- a1; r <- a2; evalMass ef $ HiExprValue <$> [l, r]) (pure <$> lst)
    _ -> throwE HiErrorInvalidFunction
--t6
applyEvalHiFun (BytesAsFunction (HiValueBytes b)) [HiValueNumber n] = fromMaybe HiValueNull <$> index (bytesToHiNumber b) n
applyEvalHiFun f@(BytesAsFunction (HiValueBytes _)) [HiValueNull, n@(HiValueNumber _)] = applyEvalHiFun f [HiValueNumber 0, n]
applyEvalHiFun f@(BytesAsFunction (HiValueBytes b)) [n@(HiValueNumber _), HiValueNull] = applyEvalHiFun f [n, HiValueNumber $ (toRational . BS.length) b]
applyEvalHiFun (BytesAsFunction(HiValueBytes b)) [HiValueNumber n1, HiValueNumber n2] = maybe HiValueNull (HiValueBytes . BS.pack) <$> slice (BS.unpack b) n1 n2

applyEvalHiFun PackBytes [HiValueList b] = HiValueBytes . BS.pack <$> traverse w8 (foldr (:) [] b) where
  w8 = \case
         (HiValueNumber n) -> do
             n' <- intOrThrow n
             if 0 <= n' && n' <= 255 
             then pure $ fromIntegral n'
             else throwE HiErrorInvalidArgument
         _ -> throwE HiErrorInvalidArgument
applyEvalHiFun UnpackBytes [HiValueBytes b] = pure $ HiValueList $ SQ.fromList $ bytesToHiNumber b
applyEvalHiFun Zip [HiValueBytes b] = pure $ HiValueBytes $ toStrict $ Z.compressWith Z.defaultCompressParams { Z.compressLevel = Z.bestCompression } (fromStrict b)
applyEvalHiFun Unzip [HiValueBytes b] = pure $ HiValueBytes $ toStrict $ Z.decompressWith Z.defaultDecompressParams (fromStrict b)
applyEvalHiFun EncodeUtf8 [HiValueString s] = pure $ HiValueBytes $ E.encodeUtf8 s
applyEvalHiFun DecodeUtf8 [HiValueBytes b] = pure $ either (const HiValueNull) HiValueString $ E.decodeUtf8' b
applyEvalHiFun Serialise [hi] = pure $ HiValueBytes $ toStrict $ S.serialise hi
applyEvalHiFun Deserialise [HiValueBytes b] = case S.deserialiseOrFail $ fromStrict b of
                                                Left _ -> throwE HiErrorInvalidArgument
                                                Right d -> pure d
-- t7
applyEvalHiFun Read [HiValueString s] = pure $ HiValueAction $ HiActionRead $ T.unpack s
applyEvalHiFun Write [HiValueString s1, HiValueString s2] = pure $ HiValueAction $ HiActionWrite (T.unpack s1) (encodeUtf8 s2)
applyEvalHiFun MkDir [HiValueString s] = pure $ HiValueAction $ HiActionMkDir $ T.unpack s
applyEvalHiFun ChDir [HiValueString s] = pure $ HiValueAction $ HiActionChDir $ T.unpack s
--t8
applyEvalHiFun ParseTime [HiValueString s] = pure $ maybe HiValueNull HiValueTime (readMaybe @T.UTCTime (T.unpack s))
--t9
applyEvalHiFun Rand [HiValueNumber n1, HiValueNumber n2] = do n1' <- intOrThrow n1
                                                              n2' <- intOrThrow n2
                                                              pure $ HiValueAction $ HiActionRand n1' n2'
--t10
applyEvalHiFun Echo [HiValueString s] = pure $ HiValueAction $ HiActionEcho s
applyEvalHiFun _ _ = throwE HiErrorInvalidArgument

intOrThrow :: HiMonad m => Rational -> ExceptT HiError m Int
intOrThrow (n :% d)
  | d /= 1 = throwE HiErrorInvalidArgument
  | otherwise = pure $ fromInteger n

index :: HiMonad m => [a] -> Rational -> ExceptT HiError m (Maybe a)
index lst real = do
  n <- intOrThrow real
  pure $ if (n >= 0) && (n < length lst) then Just (lst !! n) else Nothing

slice :: HiMonad m => [a] -> Rational -> Rational -> ExceptT HiError m (Maybe [a])
slice lst l r = do
  l' <- intOrThrow l
  r' <- intOrThrow r
  pure $ if idx l' > idx r' || (l' == r' && l' > length lst) then Nothing else Just $ cut (idx l') (idx r')
  where
    cut from to = take (to - from) (drop from lst)
    idx p = if p < 0 then length lst - abs p else p

aritize :: HiMonad m => HiValue -> ExceptT HiError m AritizedHiFun
aritize (HiValueFunction f) = let ef = toEvalHiFun f in pure $ getArity ef ef
  where
    getArity f' = case f' of
      Div -> Binary
      Mul -> Binary
      Add -> Binary
      Sub -> Binary
      Not -> Unary
      And -> Binary
      Or -> Binary
      LessThan -> Binary
      GreaterThan -> Binary
      Equals -> Binary
      NotLessThan -> Binary
      NotGreaterThan -> Binary
      NotEquals -> Binary
      If -> Ternary
      Length -> Unary
      ToUpper -> Unary
      ToLower -> Unary
      Reverse -> Unary
      Trim -> Unary
      (StringAsFunction _) -> Vararg
      List -> Vararg
      Range -> Binary
      Fold -> Binary
      (ListAsFunction _) -> Vararg
      PackBytes -> Unary
      UnpackBytes -> Unary
      EncodeUtf8 -> Unary
      DecodeUtf8 -> Unary
      Zip -> Unary
      Unzip -> Unary
      Serialise -> Unary
      Deserialise -> Unary
      (BytesAsFunction _) -> Vararg
      Read -> Unary
      Write -> Binary
      MkDir -> Unary
      ChDir -> Unary
      (Action _) -> Zero
      ParseTime -> Unary
      Rand -> Binary
      Echo -> Unary
aritize (HiValueAction a) = pure $ Zero $ Action a
aritize (HiValueNumber _) = throwE HiErrorInvalidFunction
aritize (HiValueBool _) = throwE HiErrorInvalidFunction
aritize (HiValueTime _) = throwE HiErrorInvalidFunction
aritize HiValueNull = throwE HiErrorInvalidFunction
aritize s@(HiValueString _) = pure $ Vararg $ StringAsFunction s
aritize l@(HiValueList _) = pure $ Vararg $ ListAsFunction l
aritize b@(HiValueBytes _) = pure $ Vararg $ BytesAsFunction b

toEvalHiFun :: HiFun -> EvalHiFun
toEvalHiFun f = case f of
  HiFunDiv -> Div
  HiFunMul -> Mul
  HiFunAdd -> Add
  HiFunSub -> Sub
  HiFunNot -> Not
  HiFunAnd -> And
  HiFunOr -> Or
  HiFunLessThan -> LessThan
  HiFunGreaterThan -> GreaterThan
  HiFunEquals -> Equals
  HiFunNotLessThan -> NotLessThan
  HiFunNotGreaterThan -> NotGreaterThan
  HiFunNotEquals -> NotEquals
  HiFunIf -> If
  HiFunLength -> Length
  HiFunToUpper -> ToUpper
  HiFunToLower -> ToLower
  HiFunReverse -> Reverse
  HiFunTrim -> Trim
  HiFunList -> List
  HiFunRange -> Range
  HiFunFold -> Fold
  HiFunPackBytes -> PackBytes
  HiFunUnpackBytes -> UnpackBytes
  HiFunEncodeUtf8 -> EncodeUtf8
  HiFunDecodeUtf8 -> DecodeUtf8
  HiFunZip -> Zip
  HiFunUnzip -> Unzip
  HiFunSerialise -> Serialise
  HiFunDeserialise -> Deserialise
  HiFunRead -> Read
  HiFunWrite -> Write
  HiFunMkDir -> MkDir
  HiFunChDir -> ChDir
  HiFunParseTime -> ParseTime
  HiFunRand -> Rand
  HiFunEcho -> Echo
