module Main (main) where

import System.Console.Haskeline
import HW5.Parser (parse)
import HW5.Evaluator(eval)
import HW5.Pretty
import HW5.Action
import Control.Monad.IO.Class (liftIO)
import Data.Set (fromList)

main :: IO ()
main = runInputT defaultSettings handleInput
  where
    handleInput = do
      inp <- getInputLine "hi> "
      maybe handleInput handleValue inp

    handleValue v = do
      case parse v of
        Left e -> do outputStrLn $ show e
        Right e -> do
          r <- liftIO $ runHIO (eval e) (fromList [AllowRead, AllowWrite, AllowTime])
          either (outputStrLn . show) (outputStrLn . show . prettyValue) r
      handleInput
          