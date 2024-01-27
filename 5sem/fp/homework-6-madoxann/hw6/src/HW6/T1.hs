{-# LANGUAGE FlexibleContexts #-}
module HW6.T1
  ( BucketsArray
  , CHT (..)
  
  , newCHT
  , getCHT
  , putCHT
  , sizeCHT

  , initCapacity
  , loadFactor
  ) where

import Control.Concurrent.Classy (STM, MonadConc, atomically)
import Control.Concurrent.Classy.STM (TArray, TVar, newTVar, readTVar, writeTVar)
import Data.Array.Base (newArray, getNumElements, readArray, writeArray, getElems)
import Data.Hashable (hash, Hashable)
import Control.Monad (when, forM_)

initCapacity :: Int
initCapacity = 16

loadFactor :: Double
loadFactor = 0.75

type Bucket k v = [(k, v)]
type BucketsArray stm k v = TArray stm Int (Bucket k v)

data CHT stm k v = CHT
  { chtBuckets :: TVar stm (BucketsArray stm k v)
  , chtSize    :: TVar stm Int
  }

newCHT :: MonadConc m => m (CHT (STM m) k v)
newCHT = atomically $ do
  arr <- newArray (0, initCapacity - 1) []
  b <- newTVar arr
  sz <- newTVar 0
  return CHT {chtBuckets = b, chtSize = sz}

getCHT
   :: ( MonadConc m
      , Eq k
      , Hashable k
      )
   => k
   -> CHT (STM m) k v
   -> m (Maybe v)
getCHT k cht = atomically $ do
  b <- readTVar $ chtBuckets cht
  cp <- getNumElements b
  arr <- readArray b (hash k `mod` cp)
  return $ lookup k arr

putCHT
   :: ( MonadConc m
      , Eq k
      , Hashable k
      )
   => k
   -> v
   -> CHT (STM m) k v
   -> m ()
putCHT k v cht = atomically $ do
  b <- readTVar $ chtBuckets cht
  sz <- readTVar $ chtSize cht
  cp <- getNumElements b
  when (fromIntegral sz + 1 >= fromIntegral cp * loadFactor) $ do 
    b' <- rehash b
    writeTVar (chtBuckets cht) b'
  
  newB <- readTVar $ chtBuckets cht
  newCp <- getNumElements newB
  let i = hash k `mod` newCp
  newArr <- readArray newB i
  case lookup k newArr of
    Just _ -> writeArray newB i (map (\(k', v') -> if k' == k then (k', v) else (k', v')) newArr)
    Nothing -> do
       writeArray newB i ((k, v) : newArr)
       writeTVar (chtSize cht) (sz + 1)
  return ()
  where
    rehash b = do
      cp <- getNumElements b
      let newCp = cp * 2
      newArr <- newArray (0, newCp - 1) []
      b' <- getElems b
      let elems = concat b'
      forM_ elems (\(k', v') -> do
        let i = hash k' `mod` newCp
        n <- readArray newArr i
        writeArray newArr i ((k', v') : n))
      return newArr

sizeCHT :: MonadConc m => CHT (STM m) k v -> m Int
sizeCHT cht = atomically $ readTVar $ chtSize cht
