{-# LANGUAGE DerivingVia #-}

module HW5.Action
  ( HIO(..)
  , HiPermission(..)
  , PermissionException(..)
  ) where

import Data.Set (Set)
import HW5.Base

import qualified Data.Sequence as SQ
import Control.Exception.Base (Exception)
import Control.Monad.Trans.Reader
import Control.Exception (throwIO)
import qualified Data.ByteString as BS
import Control.Monad.IO.Class (liftIO)
import qualified Data.Text.Encoding as E
import qualified Data.Text as T
import System.Directory
import Data.Time.Clock (getCurrentTime)
import qualified System.Random as R

data HiPermission =
    AllowRead
  | AllowWrite
  | AllowTime deriving (Show, Eq, Ord)

data PermissionException = PermissionRequired HiPermission deriving (Show, Eq, Ord)

instance Exception PermissionException

newtype HIO a = HIO { runHIO :: Set HiPermission -> IO a }
  deriving (Functor, Applicative, Monad) via (ReaderT (Set HiPermission) IO)

instance HiMonad HIO where
  runAction a = case a of
    HiActionRead fp -> HIO $ \p -> require p AllowRead $ do isFile <- doesFileExist fp
                                                            if isFile
                                                            then
                                                              do f <- liftIO $ BS.readFile fp
                                                                 return $ either (const $ HiValueBytes f) HiValueString (E.decodeUtf8' f)
                                                            else
                                                              do lst <- listDirectory fp
                                                                 return $ HiValueList $ SQ.fromList $ HiValueString . T.pack <$> lst
    HiActionWrite fp bs -> HIO $ \p -> require p AllowWrite $ liftIO (BS.writeFile fp bs) >> pure HiValueNull
    HiActionMkDir fp -> HIO $ \p -> require p AllowWrite $ liftIO (createDirectoryIfMissing True fp) >> pure HiValueNull
    HiActionChDir fp -> HIO $ \p -> require p AllowRead $ liftIO (setCurrentDirectory fp) >> pure HiValueNull
    HiActionCwd -> HIO $ \p -> require p AllowRead $ HiValueString . T.pack <$> getCurrentDirectory
    HiActionNow -> HIO $ \p -> require p AllowTime $ HiValueTime <$> liftIO getCurrentTime
    HiActionRand l r -> HIO $ \_ -> HiValueNumber . toRational <$> R.getStdRandom (R.uniformR (l, r))
    HiActionEcho s -> HIO $ \p -> require p AllowWrite $ liftIO (print s) >> pure HiValueNull
    where require set p tr = if p `elem` set
                             then tr
                             else throwIO $ PermissionRequired p