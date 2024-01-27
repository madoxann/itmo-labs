module Main (main) where

import HW6.T3
import Data.ListZipper(toList)
import Control.Monad (forM_, replicateM_)
import Data.Grid (unGrid)
import System.Info ()
import Options.Applicative
import qualified System.Random as R

data Args = Args
  { prob       :: Double
  , incub      :: Int
  , ill        :: Int
  , immun      :: Int
  , gridSize   :: Int
  , iterations :: Int}

parser :: Parser Args
parser = Args
      <$> option auto
          ( long "prob"
         <> metavar "PROBABILITY"
         <> help "probability of ill or infected cell to pass on an infection" )
      <*> option auto
          ( long "incub"
         <> metavar "INCUBATION"
         <> help "duration of infection's incubational period")
      <*> option auto
          ( long "ill"
         <> metavar "ILL"
         <> help "duration of infection's illness period")
      <*> option auto
          ( long "immun"
         <> metavar "IMMUNITY"
         <> help "duration of infection's immunity period")
      <*> option auto
          ( long "grid-size"
         <> metavar "GRID-SIZE"
         <> help "size of Comonad19 grid")
      <*> option auto
          ( long "iterations"
         <> metavar "ITERATIONS"
         <> help "number of days to monitor infection")

main :: IO ()
main = do
  args <- execParser opts
  cfg <- getConfig args
  case cfg of
    Right err -> do
      putStrLn err
      return ()
    Left cfg' -> do
      let sz = gridSize args
      let g = ((`toList` sz) <$>) . (`toList` sz) <$> (unGrid <$> take (iterations args) (simulate cfg'))
      forM_ g (\d -> do
        forM_ d (\line -> do putStrLn $ mconcat (show <$> line))
        replicateM_ 2 (putStrLn ""))
  where
    opts = info (parser <**> helper) fullDesc
    getConfig args | let p = prob args in p < 0 || p > 1 = return $ Right "Probability must be in range 0..1"
                   | incub args < 0 = return $ Right "Incubational period cannot be negative"
                   | ill args < 0 = return $ Right "Ill period cannot be negative"
                   | immun args < 0 = return $ Right "Immunity period cannot be negative"
                   | gridSize args < 0 = return $ Right "grid-size cannot be negative"
                   | iterations args <= 0 = return $ Right "number of iterations must be positive"
                   | otherwise = do rndSeed <- R.getStdRandom (R.uniformR (minBound, maxBound))
                                    return $ Left $ Config (prob args) ((incub args - 1) `max` 0) ((ill args - 1) `max` 0) ((immun args - 1) `max` 0) rndSeed
