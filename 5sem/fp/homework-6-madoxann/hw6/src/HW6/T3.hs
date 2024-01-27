module HW6.T3
  ( Config (..)
  , Cell (..)
  , CellState (..)
  , Comonad19Grid
  , simulate
  ) where

import System.Random (StdGen, randomR, split, mkStdGen)

import Data.Grid (Grid (..), gLeft, gRight, gUp, gDown)
import Data.ListZipper (lGenerator)
import Control.Comonad (Comonad (..))
import Control.Monad (liftM2)

data Config = Config
  { probability :: Double
  , incubationPeriod :: Int
  , illnessDuration :: Int
  , immunityDuration :: Int
  , seed :: Int
  } deriving Show

data CellState
  = Healthy
  | Infected Int
  | Ill Int
  | Immune Int
  deriving Show

data Cell = Cell
  { cellState :: CellState
  , cellRand :: StdGen
  }
  
instance Show Cell where
  show c = case cellState c of
    Healthy -> "_"
    Infected _ -> "i"
    Ill _ -> "#"
    Immune _ -> "@"

type Comonad19Grid = Grid Cell

neighbors :: [Grid a -> Grid a]
neighbors = horizontals ++ verticals ++ liftM2 (.) horizontals verticals
  where horizontals = [gLeft, gRight]
        verticals   = [gUp, gDown]

carrierNeighbors :: Comonad19Grid -> Int
carrierNeighbors g = illCount
                 $ map (\direction -> extract $ direction g) neighbors
                 where
                   illCount = length . filter (\c -> case cellState c of
                     Infected _ -> True
                     Ill _ -> True
                     _ -> False)

rule :: Config -> Comonad19Grid -> Cell
rule cfg g = case cellState c of 
  Healthy -> tryInfect c 
  Infected 0 -> keepGen $ Ill $ illnessDuration cfg
  Infected x -> keepGen $ Infected $ x - 1
  Ill 0 -> keepGen $ Immune $ immunityDuration cfg
  Ill x -> keepGen $ Ill $ x - 1
  Immune 0 -> keepGen Healthy
  Immune x -> keepGen $ Immune $ x - 1
  where
    c = extract g
    rnd = cellRand c
    keepGen state = c {cellState = state}
    
    tryInfect cell = let r = roll rnd $ carrierNeighbors g
                     in if snd r then Cell (Infected $ incubationPeriod cfg) (fst r)
                     else cell {cellRand = fst r}
    roll gen 0 = (gen, False)
    roll gen cnt = let r = randomR (1.0, 0.0) gen
                       r' = roll (snd r) (cnt - 1)
                   in (fst r', snd r' || (fst r <= probability cfg))

evolve :: Config -> Comonad19Grid -> Comonad19Grid
evolve cfg = extend $ rule cfg

-- | Creates an infinite list of grids using the given configuration.
-- Each element of this list represents one infection simulation step.
--
-- This function may take additional parameters (e.g. initial seed for random).
simulate :: Config -> [Comonad19Grid]
simulate cfg = iterate (evolve cfg) initialize where
  initialize = Grid $ lGenerator (newGen fst <$>) (newGen snd <$>) z
  z = lGenerator (newGen fst) (newGen snd) $ Cell (Infected $ incubationPeriod cfg) (mkStdGen $ seed cfg)
  newGen direction c = Cell {cellState = Healthy, cellRand = (direction . split . cellRand) c }
